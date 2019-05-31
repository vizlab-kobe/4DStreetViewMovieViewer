#include "BirdsEyeWidget.h"
#include <kvs/ObjectManager>

namespace local
{

BirdsEyeWidget::BirdsEyeWidget( local::Model* model, local::View* view ):
    kvs::WidgetBase( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    //機能させたいイベントを指定する。
    addEventType( kvs::EventBase::PaintEvent );
}

void BirdsEyeWidget::paintEvent()
{
    //非表示指定の場合、処理しない。
    if( !isShown() ) return;

    painter().begin( screen() );
    drawBackground();

    //現在のビューポート(スクリーン全体)を一旦保存する。
    const kvs::Vec4 vp = kvs::OpenGL::Viewport();
    {
        //現在のOpenGLの状態を全て退避させる。※スコープアウト後勝手に復元される。
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
//                const float field_of_view = 90.0f;
//                const float aspect = 1.0f;
//                kvs::OpenGL::SetPerspective( field_of_view, aspect, front, back );
                const float left = -9.0f;
                const float bottom = -9.0f;
                const float right = 9.0f;
                const float top = 9.0f;
                kvs::OpenGL::SetOrtho( left, right, bottom, top, front, back );

                //ウィジェットの表示領域(ビューポート)を設定する。
                const int x = x0() + margin();
                const int y = screen()->height() - y0() - height() + margin();
                const int w = width() - margin();
                const int h = height() - margin();
                kvs::Vec3i camera_position = m_model->cameraPosition();
                kvs::OpenGL::SetViewport( x, y, w, h );

                //モデルビュー行列を退避させる。
                kvs::OpenGL::WithPushedMatrix p2( GL_MODELVIEW );
                p2.loadIdentity();
                {
                    //視点情報を設定する。
                    const kvs::Vec3 eye( 11.0f, 11.0f, 12.0f );
                    const kvs::Vec3 center( 0.0f, 0.0f, 0.0f );
                    const kvs::Vec3 up( 0.0f, 1.0f, 0.0f );
                    kvs::OpenGL::SetLookAt( eye, center, up );

                    this->initialize();
                    this->draw_boundary();
                    this->draw_cameras();
                    {
                        kvs::OpenGL::MultMatrix( kvs::Xform::Translation( kvs::Vec3( camera_position ) ) );
                        kvs::OpenGL::MultMatrix( kvs::Xform::Rotation( m_view->movieScreen().scene()->objectManager()->xform().rotation() ) );
                        this->draw_arrowglyph();
                        this->draw_sphereglyph();
                    }
                }
            }
        }
    }

    //ビューポートを元に戻す。
    kvs::OpenGL::SetViewport( vp );

    painter().end();
}

void BirdsEyeWidget::draw_boundary()
{
    //計算境界（線分）を描画する。
    //点の位置（幾何情報）
    GLdouble vertex[][3] = {
        { 0.0, 6.0, 6.0 }, /* A(0) */
        { 0.0, 6.0, 3.0 }, /* B(1) */
        { 0.0, 3.0, 3.0 }, /* C(2) */
        { 0.0, 3.0, 6.0 }, /* D(3) */
        { 9.0, 6.0, 6.0 }, /* E(4) */
        { 9.0, 6.0, 3.0 }, /* F(5) */
        { 9.0, 3.0, 3.0 }, /* G(6) */
        { 9.0, 3.0, 6.0 }  /* H(7) */
    };

    //線分(位相情報)  
    GLint edge[][2] = {
        { 0, 1 }, /* A-B */
        { 1, 2 }, /* B-C */ 
        { 2, 3 }, /* C-D */ 
        { 3, 0 }, /* D-A */ 
        { 4, 5 }, /* E-F */ 
        { 5, 6 }, /* F-G */ 
        { 6, 7 }, /* G-H */ 
        { 7, 4 }, /* H-E */ 
        { 0, 4 }, /* A-E */ 
        { 1, 5 }, /* B-F */ 
        { 2, 6 }, /* C-G */ 
        { 3, 7 }  /* D-H */ 
    };

    kvs::OpenGL::Begin( GL_LINES );
    for ( int i = 0; i < 12; i++ )
    {
        kvs::OpenGL::Vertex3( vertex[ edge[i][0] ] );
        kvs::OpenGL::Vertex3( vertex[ edge[i][1] ] );
    }
    kvs::OpenGL::End();
}

void BirdsEyeWidget::draw_cameras()
{
    //カメラ位置（点）を描画する。
    GLint i;
    GLint j;
    GLint k;

    kvs::OpenGL::SetPointSize(3);
    kvs::OpenGL::Begin( GL_POINTS );
    for ( i = 0; i < 10; i++ )
    {
        for ( j = 0; j < 10; j++ )
        {
            for ( k = 0; k < 10; k++ )
            {
                kvs::OpenGL::Color( 0.0, 0.0, 0.0 );
                kvs::OpenGL::Vertex( i, j, k );
            }
        }
    }
    kvs::OpenGL::End();
}

void BirdsEyeWidget::draw_frustum()
{
    //視錐体（ポリゴン）を描画する。
    kvs::Vec3d peak_point( 0.0, 0.0, 0.0 );
    int div_num = 12; 
    double radius = 1.0;
    double pi = 3.1415926535897932384;
    double angle = 2 * pi / div_num;

    kvs::OpenGL::Begin( GL_TRIANGLE_FAN );
    kvs::OpenGL::Color( 0.0, 0.0, 1.0, 0.9 );
    kvs::OpenGL::Vertex( peak_point[0], peak_point[1], peak_point[2] );
    for ( int i = 0; i <= div_num; i++ )
    {
        double x = std::cos( angle * i ) * radius;
        double y = std::sin( angle * i ) * radius;
        kvs::OpenGL::Vertex( peak_point[0] + x, peak_point[1] + y, peak_point[2] - radius );
    }

    kvs::OpenGL::End();

    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( 0.0, 0.0, 0.0 );
    kvs::OpenGL::Vertex( peak_point[0], peak_point[1], peak_point[2] );
    kvs::OpenGL::Vertex( peak_point[0] + radius, peak_point[1] + 0.0, peak_point[2] - radius );
    for ( int i = 1; i <= div_num; i++ )
    {
        double x_0 = std::cos( angle * ( i - 1 ) ) * radius;
        double y_0 = std::sin( angle * ( i - 1 ) ) * radius;
        double x = std::cos( angle * i ) * radius;
        double y = std::sin( angle * i ) * radius;
        kvs::OpenGL::Vertex( peak_point[0] + x_0, peak_point[1] + y_0, peak_point[2] - radius );
        kvs::OpenGL::Vertex( peak_point[0] + x, peak_point[1] + y, peak_point[2] - radius );
    }
    kvs::OpenGL::End();
}

void BirdsEyeWidget::draw_arrowglyph()
{
    const kvs::Real32 R = -180.0f;   //rotation angle  (init = -90.0)
    const kvs::Vec3 V( 1.0f, 0.0f, 0.0f ); // rotation vector
    const kvs::Vec3 T0( 0.0f, 0.0f, 1.0f ); // translation vector (cone) (init( 0.0, 0.0, 0.7))
    const kvs::Vec3 T1( 0.0f, 0.0f, 0.0f ); // translation vector (cylinder) (init(0.0, 0.0, 0.0))

    kvs::OpenGL::PushMatrix();
    kvs::OpenGL::Rotate( R, V );
    {
        //Cone
        kvs::OpenGL::Color( 0.7, 0.8, 0.3, 0.0 );
        kvs::OpenGL::PushMatrix();
        {
            const GLdouble base = 0.25; //0.15
            const GLdouble top = 0.0;   //0.0
            const GLdouble height = 0.5;//0.3
            const GLint slices = 20;    //20
            const GLint stacks = 5;     //5
            kvs::OpenGL::Translate( T0 );
            kvs::OpenGL::DrawCylinder( base, top, height, slices, stacks );
         }
         kvs::OpenGL::PopMatrix();
        //Cilynder
        kvs::OpenGL::Color( 0.2, 0.4, 1.0, 0.0 );
        kvs::OpenGL::PushMatrix();
        {
            const GLdouble base = 0.11; //0.07
            const GLdouble top = 0.11;  //0.07
            const GLdouble height = 1.0;//0.7
            const GLint slices = 20;    //20
            const GLint stacks = 2;     //2
            kvs::OpenGL::Translate( T1 );
            kvs::OpenGL::DrawCylinder( base, top, height, slices, stacks );
         }
         kvs::OpenGL::PopMatrix();
    }
    kvs::OpenGL::PopMatrix();
}

void BirdsEyeWidget::draw_sphereglyph()
{
    const GLdouble radius = 0.15; //0.1
    size_t m_nslices = 10;       //10
    size_t m_nstacks = 10;       //10
    const GLint slices = static_cast<GLint>( m_nslices );
    const GLint stacks = static_cast<GLint>( m_nstacks );

    kvs::OpenGL::Color( 1.0, 0.3, 0.3, 0.0 );
    kvs::OpenGL::DrawSphere( radius, slices, stacks );
}

void BirdsEyeWidget::initialize()
{
    //init
//    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
//    kvs::OpenGL::SetColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    kvs::OpenGL::SetLightModel( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

    kvs::OpenGL::Disable( GL_LINE_SMOOTH );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::Enable( GL_COLOR_MATERIAL );
    kvs::OpenGL::Enable( GL_NORMALIZE );
    kvs::OpenGL::Enable( GL_LIGHTING );
}

} // end of namespace local
