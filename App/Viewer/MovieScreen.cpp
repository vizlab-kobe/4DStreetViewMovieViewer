/* ***************************************************************************/
/**
* @file MovieScreen.cpp
* @brief MovieScreenクラスの実装
*/
/* ***************************************************************************/
#include "MovieScreen.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>
#include <kvs/Camera>

namespace local
{

/*==========================================================================*/
/**
* @brief コンストラクタ
* @param app Applicationへのポインタ
* @details カメラの視野を90度に変更
*/
/*==========================================================================*/
MovieScreen::MovieScreen( local::Application* app ):
    local::Screen( app )
{
    scene()->camera()->setFieldOfView(90);
}

/*==========================================================================*/
/**
* @brief 動画を表示するためのセットアップを行う関数
* @param model Modelへのポインタ
* @details スクリーンサイズの設定
* @details 描画するオブジェクトをレンダラーに登録
*/
/*==========================================================================*/
void MovieScreen::setup( local::Model* model )
{
    const size_t width = 512;
    const size_t height = 512;
    this->setSize( width, height ); // スクリーンサイズの設定
    {
        typedef lib4dsv::MovieObject Object;
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;

        Object* object = model->object(); // モデルのオブジェクトを取得
        if ( !object ) { return; }
        object->setName("Object");

        Renderer* renderer = new Renderer(); // 登録するレンダラー
        renderer->setName("Renderer");
        renderer->setEnabledAutoPlay( false );
        renderer->setEnabledLoopPlay( false );

        this->registerObject( object, renderer ); // 描画するオブジェクトをレンダラーに登録
    }
}

/*==========================================================================*/
/**
* @brief 動画を表示するオブジェクトの更新をする関数
* @param model Modelへのポインタ
*/
/*==========================================================================*/
void MovieScreen::update( local::Model* model )
{
    if ( !scene()->object("Object") ) { return; }

    typedef lib4dsv::MovieObject Object;
    const int index = static_cast<Object*>( scene()->object("Object") )->device().nextFrameIndex();

    Object* object = model->object();
    object->setName("Object");
    object->device().setNextFrameIndex( index ); // 次に描画するフレーム番号をセットする

    scene()->replaceObject( "Object", object ); // "Object"という名のオブジェクトをobjectに置き換える
}

/*==========================================================================*/
/**
* @brief 動画を表示するオブジェクトの更新をする関数
* @param model Modelへのポインタ
* @param index 表示するフレーム番号
*/
/*==========================================================================*/
void MovieScreen::update( local::Model* model, const int index )
{
    typedef lib4dsv::MovieObject Object;

    Object* object = model->object();
    object->setName("Object");
    object->device().setNextFrameIndex( index );

    scene()->replaceObject( "Object", object ); // "Object"という名のオブジェクトをobjectに置き換える
}

} // end of namespace
