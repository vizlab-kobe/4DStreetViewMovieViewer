/* ***************************************************************************/
/**
* @file  SphericalMapMovieRenderer.cpp
* @brief Implementation of SphericalMapMovieRenderer class
*/
/* ***************************************************************************/
#include "SphericalMapMovieRenderer.h"
#include <kvs/Camera>
#include <kvs/ImageObject>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>
#include <kvs/ShaderSource>


namespace lib4dsv
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new SphericalMapMovieRenderer class.
 *  @param  type [in] rendering type
 */
/*==========================================================================*/
SphericalMapMovieRenderer::SphericalMapMovieRenderer( const SphericalMapMovieRenderer::Type& type ):
    m_enable_auto_play( false ),
    m_enable_loop_play( false ),
    m_enable_reverse_play( false ),
    m_frame_index( 0 ),
    m_current_frame_index( 0 )
{
    m_type = type;
}

/*==========================================================================*/
/**
 *  @brief  Destruct the SphericalMapMovieRenderer class.
 */
/*==========================================================================*/
SphericalMapMovieRenderer::~SphericalMapMovieRenderer()
{
}

/*==========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 *  @details Create texture.
 *  @details Create shader program.
 *  @details Setup OpenGL-related states
 *  @details Grab a frame from the video object (movie data), crop the rendering image and render it onto the window
 *  @details Update the frame index for next rendering
 */
/*==========================================================================*/
void SphericalMapMovieRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    BaseClass::startTimer();
    lib4dsv::MovieObject* video = reinterpret_cast<lib4dsv::MovieObject*>( object );

    if ( !m_texture.isCreated() )
    {
        this->create_texture( video ); // textureを作成
    }

    if ( !m_shader_program.isCreated() )
    {
        this->create_shader_program(); // shader programを作成
    }

//    if ( m_type == Centering )
//    {
//        this->center_alignment( camera->windowWidth(), camera->windowHeight() );
//    }

//    kvs::OpenGL::SetClearDepth( 1.0 );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // カラー/デプスバッファのクリア

    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS ); // glPushAttrib( GL_ALL_ATTRIB_BITS )
    {
        kvs::OpenGL::Disable( GL_DEPTH_TEST ); // glDisable( GL_DEPTH_TEST )：深部テストの無効化
        kvs::OpenGL::Disable( GL_TEXTURE_1D ); // 1次元テクスチャを無効化
        kvs::OpenGL::Disable( GL_TEXTURE_3D ); // 3次元テクスチャを無効化
        kvs::OpenGL::Enable( GL_TEXTURE_2D ); // 2次元テクスチャを有効化

        kvs::ProgramObject::Binder shader( m_shader_program ); // 描画に使用するProgramObject(m_shader_program)を設定
        kvs::Texture::Binder unit( m_texture, 0 ); // 使用するテクスチャユニットをm_textureに関連づける

        const IplImage* frame = video->device().queryFrame(); // cvQueryFrame：fileから1つのフレームを取り出す
        const int width = frame->width; // フレームの幅
        const int height = frame->height; // フレームの高さ
        const char* data = frame->imageData; // BGRBGRBGR... ：画像データへのポインタ
        m_texture.load( width, height, data ); // GPUへテクスチャデータをダウンロードする

        m_shader_program.setUniform( "spherical_map", 0 ); // "spherical_map"というデータをテクスチャID:0としてシェーダに渡す
        m_shader_program.setUniform( "R", object->xform().rotation() ); // "R"というuniform変数に回転行列を指定する
        m_shader_program.setUniform( "image_size", kvs::Vec2( video->width(), video->height() ) ); // "image_size"というuniform変数に画像の幅/高さを指定する
        m_shader_program.setUniform( "screen_size", kvs::Vec2( camera->windowWidth(), camera->windowHeight() ) ); // "screen_size"というuniform変数にwindowの幅/高さを指定する

        kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW ); // モデルビューモードに変換（以降モデルビュー変換行列演算を行う）
        p1.loadIdentity(); // 変換行列を単位行列に初期化する
        {
            kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION ); // 投影変換モードに変換（以降投影変換行列演算を行う）
            p2.loadIdentity(); // 変換行列を単位行列に初期化する
            {
                kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 ); // m_left, m_right, m_bottom, m_top, front(-1), back(1)で囲まれる範囲を平行投影する
                kvs::OpenGL::Begin( GL_QUADS ); // 4つの頂点をペアとしてそれぞれ独立した４角形を描画する
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0, 0.0 ), kvs::Vec2( 0.0, 1.0 ) ); // テクスチャ切出し座標と貼付け座標の指定
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0, 1.0 ), kvs::Vec2( 0.0, 0.0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0, 1.0 ), kvs::Vec2( 1.0, 0.0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0, 0.0 ), kvs::Vec2( 1.0, 1.0 ) );
                kvs::OpenGL::End(); // 描画終了
            }
        }
    }

    if ( m_enable_auto_play ) // AutoPlay実行中
    {
        m_current_frame_index = m_frame_index;  // 現在描画中のframe_indexを取得(以降のm_frame_indexは次の描画への準備)

        if ( m_enable_reverse_play ) // ReversePlay実行中
        {
            int reverse_play_index = video->device().nextFrameIndex() - 2;
            m_frame_index--;

            if ( m_enable_loop_play ) // LoopPlay実行中
            {
                if ( video->device().nextFrameIndex() == 1 ) // nextFrameIndexが1の時（ 現在描画中のframeIndexは0 ）
                {
                    reverse_play_index = video->device().numberOfFrames() - 1;
                    m_frame_index = video->device().numberOfFrames() - 1; // nextFrameIndexを一番最後のフレームに指定する
                }
            }
            else // LoopPlay OFFの場合
            {
                if ( video->device().nextFrameIndex() == 1 )
                {
                    reverse_play_index = 0;
                    m_frame_index = 0;
                }
            }
            video->device().setNextFrameIndex( reverse_play_index ); // nextFrameIndexをOpenCVに反映する
        }
        else // ReversePlay OFFの場合
        {
            m_frame_index++;
            if ( m_enable_loop_play ) // LooPlay 実行中
            {
                if ( video->device().nextFrameIndex() == video->device().numberOfFrames() ) // 最後のフレームまで行ったら
                {
                    video->device().setNextFrameIndex(0); // 次回は最初のフレームへ
                    m_frame_index = 0;
                }
            }
            else // LoopPlay OFFの場合
            {
                if ( video->device().nextFrameIndex() == video->device().numberOfFrames() ) // 最後のフレームまできたら
                {
                    video->device().setNextFrameIndex( video->device().numberOfFrames() - 1 ); // 以降も最後のフレームを保持
                    m_frame_index = video->device().numberOfFrames() - 1;
                }
            }
        }
    }
    else // AutoPlay OFFの場合
    {
        const int index = kvs::Math::Clamp( m_frame_index, 0, (int)video->device().numberOfFrames() - 1 );
        video->device().setNextFrameIndex( index );
    }
    BaseClass::stopTimer();
}

/*==========================================================================*/
/**
* @brief Create the shader program
*/
/*==========================================================================*/
void SphericalMapMovieRenderer::create_shader_program()
{
    kvs::ShaderSource vert("spherical_map.vert"); // "spherical_map.vert"というファイルを探して読み込む
    kvs::ShaderSource frag("spherical_map.frag"); // "spherical_map.frag"というファイルを探して読み込む
    m_shader_program.build( vert, frag ); // vert/fragの2つのshader objectをcompile → linkしてshader programを作成する
}

/*==========================================================================*/
/**
 *  @brief  Creates the texture region on the GPU.
 *  @param  image [in] pointer to the image object
 */
/*==========================================================================*/
void SphericalMapMovieRenderer::create_texture( const lib4dsv::MovieObject* movie )
{
    const double width  = movie->width();
    const double height = movie->height();
    m_initial_aspect_ratio = width / height;
    m_left = 0.0;
    m_right = 1.0;
    m_bottom = 0.0;
    m_top = 1.0;

    if ( movie->type() == lib4dsv::MovieObject::Gray8 )
    {
        m_texture.setPixelFormat( GL_INTENSITY8, GL_LUMINANCE, GL_UNSIGNED_BYTE ); // Set the pixcel format.
    }
    else if ( movie->type() == lib4dsv::MovieObject::Color24 )
    {
#ifdef GL_BGR_EXT
        m_texture.setPixelFormat( GL_RGB8, GL_BGR_EXT, GL_UNSIGNED_BYTE );
#else
        m_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
#endif
    }
    else
    {
        kvsMessageError("Unknown pixel color type.");
    }

//    const IplImage* frame = movie->device().queryFrame();

    kvs::Texture::SetEnv( GL_TEXTURE_ENV_MODE, GL_REPLACE ); // textureの合成環境の設定(背景の色を無視してtextureの色に変換)
    m_texture.setWrapS( GL_REPEAT ); // wrap method for s-axis
    m_texture.setWrapT( GL_REPEAT ); // wrap method for t-axis
    m_texture.setMagFilter( GL_LINEAR ); // magnification filter (拡大)
    m_texture.setMinFilter( GL_LINEAR ); // minification filter (縮小)
//    m_texture.create( frame->width, frame->height );
    m_texture.create( width, height ); // texture objectを作成し、これに画像を設定する。
}

/*==========================================================================*/
/**
 *  @brief  Calculates centering parameters.
 *  @param  width [in] image width
 *  @param  height [in] image height
 */
/*==========================================================================*/
void SphericalMapMovieRenderer::center_alignment( const double width, const double height )
{
    const double current_aspect_ratio = width / height; // アスペクト比
    const double aspect_ratio = current_aspect_ratio / m_initial_aspect_ratio;
    if( aspect_ratio >= 1.0 )
    {
        m_left = ( 1.0 - aspect_ratio ) * 0.5;
        m_right = ( 1.0 + aspect_ratio ) * 0.5;
        m_bottom = 0.0;
        m_top = 1.0;
    }
    else
    {
        m_left = 0.0;
        m_right = 1.0;
        m_bottom = ( 1.0 - 1.0 / aspect_ratio ) * 0.5;
        m_top = ( 1.0 + 1.0 / aspect_ratio ) * 0.5;
    }
}

} // end of namespace lib4dsv
