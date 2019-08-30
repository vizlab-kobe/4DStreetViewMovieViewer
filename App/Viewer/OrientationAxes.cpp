#include "OrientationAxes.h"
#include <kvs/ObjectManager>

namespace local
{

OrientationAxes::OrientationAxes( local::Model* model, local::View* view ):
    kvs::WidgetBase( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    //機能させたいイベントを指定する。
    addEventType( kvs::EventBase::PaintEvent );
}

void OrientationAxes::paintEvent()
{
    //非表示指定の場合、処理しない。
    if( !isShown() ) return;

    drawBackground();

    //現在のビューポート(スクリーン全体）を一旦保存する。
    const kvs::Vec4 vp = kvs::OpenGL::Viewport();
    {
        //現在のOpenGLの状態を全て退避させる。※ スコープアウト後勝手に復元される。
        kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
        {
            //常に一番手前に表示させる。
            kvs::OpenGL::SetClearDepth( 1.0 );
            kvs::OpenGL::Clear( GL_DEPTH_BUFFER_BIT );

            //投影行列を退避させる。
            kvs::OpenGL::WithPushedMatrix p1( GL_PROJECTION );
            p1.loadIdentity();
            {
                //投影情報を設定する。
                const float front = 1.0f;
                const float back = 2000.0f;
                const float left = -5.0f;
                const float bottom = -5.0f;
                const float right = 5.0f;
                const float top = 5.0f;
                kvs::OpenGL::SetOrtho( left, right, bottom, top, front, back );

                //ウィジェットの表示領域(ビューポート)を設定する。
                const int x = x0() + margin();
                const int y = screen()->height() - y0() - height() + margin();
                const int w = width() - margin();
                const int h = height() - margin();
                kvs::OpenGL::SetViewport( x, y, w, h );

                //モデルビュー行列を退避させる。
                kvs::OpenGL::WithPushedMatrix p2( GL_MODELVIEW );
                p2.loadIdentity();
                {
                    //視点情報を設定する。
                    const kvs::Vec3 eye( 0.0f, 0.0f, 12.0f );
                    const kvs::Vec3 center( 0.0f, 0.0f, 0.0f );
                    const kvs::Vec3 up( 0.0f, 1.0f, 0.0f );
                    kvs::OpenGL::SetLookAt( eye, center, up );

                    kvs::OpenGL::MultMatrix( kvs::Xform::Rotation( m_view->movieScreen().scene()->objectManager()->xform().rotation() ) );

                    const float box_length = 4.0;
                    const float axis_length = 8.0;

                    this->draw_solid_box( box_length );
                    this->draw_cornered_axis( axis_length );
                }
            }
        }
    }
    kvs::OpenGL::SetViewport( vp );
}

void OrientationAxes::draw_solid_box( const float length )
{
    const float x = -length/2.0f;
    const float y = -length/2.0f;
    const float z = -length/2.0f;

    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v2( x + length, y, z + length );
    const kvs::Vec3 v3( x , y, z + length );
    const kvs::Vec3 v4( x, y + length, z );
    const kvs::Vec3 v5( x + length, y + length, z );
    const kvs::Vec3 v6( x + length, y + length, z + length );
    const kvs::Vec3 v7( x, y + length, z + length );

    kvs::OpenGL::SetPolygonOffset( 1, 1 );
    kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );

    kvs::OpenGL::Begin( GL_QUADS );
    kvs::OpenGL::Color( kvs::RGBColor( 200, 200, 200 ) );
    kvs::OpenGL::Vertices( v0, v1, v2, v3 ); // bottom
    kvs::OpenGL::Vertices( v7, v6, v5, v4 ); // top
    kvs::OpenGL::Vertices( v0, v4, v5, v1 ); // back
    kvs::OpenGL::Vertices( v1, v5, v6, v2 ); // right
    kvs::OpenGL::Vertices( v2, v6, v7, v3 ); // front
    kvs::OpenGL::Vertices( v0, v3, v7, v4 ); // left
    kvs::OpenGL::End();

    kvs::OpenGL::SetLineWidth( 1.0 );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( kvs::RGBColor( 10, 10, 10 ) );
    {
        // ※ v0→v1はX軸（draw_cornered_axis()にて描画）
        kvs::OpenGL::Vertices( v1, v2 );
        kvs::OpenGL::Vertices( v2, v3 );
        // ※ v3→v0はZ軸（draw_cornered_axis()にて描画）
        kvs::OpenGL::Vertices( v4, v5 );
        kvs::OpenGL::Vertices( v5, v6 );
        kvs::OpenGL::Vertices( v6, v7 );
        kvs::OpenGL::Vertices( v7, v4 );
        // ※ v0→v4はY軸（draw_cornered_axis()にて描画）
        kvs::OpenGL::Vertices( v1, v5 );
        kvs::OpenGL::Vertices( v2, v6 );
        kvs::OpenGL::Vertices( v3, v7 );
    }
    kvs::OpenGL::End();
}

void OrientationAxes::draw_cornered_axis( const float length )
{
    const float x = -length/4.0f;
    const float y = -length/4.0f;
    const float z = -length/4.0f;
    const kvs::Vec3 v0( x, y, z );
    const kvs::Vec3 v1( x + length, y, z );
    const kvs::Vec3 v3( x, y, z + length );
    const kvs::Vec3 v4( x, y + length, z );

    const float offset = length / 12.0f;
    const kvs::Vec3 offsetx( 0.0f, offset, offset );
    const kvs::Vec3 offsety( offset, 0.0f, offset );
    const kvs::Vec3 offsetz( offset, offset, 0.0f );

    const kvs::RGBColor x_axis_color( 180, 10, 10 );
    const kvs::RGBColor x_axis_color2( 250, 180, 180 );
    const kvs::RGBColor y_axis_color( 10, 180, 10 );
    const kvs::RGBColor y_axis_color2( 180, 250, 180 );
    const kvs::RGBColor z_axis_color( 10, 10, 180 );
    const kvs::RGBColor z_axis_color2( 180, 180, 250 );

    const std::string x_tag = "X";
    const std::string y_tag = "Y";
    const std::string z_tag = "Z";

    kvs::OpenGL::SetLineWidth( 4.0 );

    // X2-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( x_axis_color2 );
    kvs::OpenGL::Vertices( v0, v1 );
    kvs::OpenGL::End();
    
    // Y2-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( y_axis_color2 );
    kvs::OpenGL::Vertices( v0, v4 );
    kvs::OpenGL::End();

    // Z2-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( z_axis_color2 );
    kvs::OpenGL::Vertices( v0, v3 );
    kvs::OpenGL::End();

    kvs::OpenGL::SetLineWidth( 2.0 );

    // X-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( x_axis_color );
    kvs::OpenGL::Vertices( v0, v1 );
    kvs::OpenGL::End();

    // Y-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( y_axis_color );
    kvs::OpenGL::Vertices( v0, v4 );
    kvs::OpenGL::End();

    // Z-axis.
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( z_axis_color );
    kvs::OpenGL::Vertices( v0, v3 );
    kvs::OpenGL::End();

    painter().begin( screen() );
    {
        // X-axis label
        painter().font().setColor( x_axis_color );
        painter().drawText( v1 - offsetx, x_tag );
        // Y-axis label
        painter().font().setColor( y_axis_color );
        painter().drawText( v4 - offsety, y_tag );
        // Z-axis label
        painter().font().setColor( z_axis_color );
        painter().drawText( v3 - offsetz, z_tag );
    }
    painter().end();
}

} // end of namespace local
