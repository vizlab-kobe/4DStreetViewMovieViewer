/* ***************************************************************************/
/**
* @file Event.cpp
* @brief Eventクラスの実装
*/
/* ***************************************************************************/
#include "Event.h"
#include "Model.h"
#include "View.h"
#include "MainWindow.h"
#include "Controller.h"
#include "Program.h"
#include <kvs/Timer>
#include <kvs/Camera>
#include <kvs/ObjectManager>
#include <kvs/Directory>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>

namespace local
{

/*==========================================================================*/
/**
* @brief コンストラクタ
* @param model Modelへのポインタ
* @param view Viewへのポインタ
* @param controller Controllerへのポインタ
*/
/*==========================================================================*/
Event::Event( local::Model* model, local::View* view, local::Controller* controller ):
    m_model( model ),
    m_view( view ),
    m_controller( controller ),
    m_enable_auto_play( false ),
    m_enable_loop_play( false ),
    m_enable_focus_mode( false )
{
    setEventType(
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent |
        kvs::EventBase::MouseDoubleClickEvent |
        kvs::EventBase::KeyPressEvent );
}

/*==========================================================================*/
/**
* @brief FocusMode時に実行する関数
* @bug FocusMode実行時、挙動が少しおかしい気がする。要確認。
*/
/*==========================================================================*/
void Event::focusMode()
{
    m_view->movieScreen().scene()->objectManager()->resetXform();

    const kvs::Vec3i dimensions = m_model->cameraArrayDimensions();
    float target_x = ( dimensions.x() - 1.0 ) / 2.0;
    float target_y = ( dimensions.y() - 1.0 ) / 2.0;
    float target_z = ( dimensions.z() - 1.0 ) / 2.0;

    const kvs::Vec3 target = kvs::Vec3 ( target_x, target_y, target_z );
    const kvs::Vec3 eye = kvs::Vec3( m_model->cameraPosition() );
    const kvs::Vec3 look_at = target - eye;
    const kvs::Vec3 up = kvs::Vec3(0.0, 1.0, 0.0);
    const kvs::Vec3 eye_x = look_at.cross(up);
    const kvs::Vec3 eye_y = look_at.cross(eye_x);
    const kvs::Vec3 eye_z = -look_at.normalized();
    const kvs::Mat3 rot = kvs::Mat3(eye_x.x(), eye_y.x(), eye_z.x(),
                                    eye_x.y(), eye_y.y(), eye_z.y(),
                                    eye_x.z(), eye_y.z(), eye_z.z()
        );

    m_view->movieScreen().scene()->objectManager()->rotate( rot );
}

/*==========================================================================*/
/**
* @brief マウスが押された時に実行する関数
* @param event MouseEventへのポインタ
* @details AutoPlayを一旦中断する
*/
/*==========================================================================*/
void Event::mousePressEvent( kvs::MouseEvent* event )
{
    if ( m_model->isSet() )
    {
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    m_enable_auto_play = renderer->isEnabledAutoPlay();
    if ( m_enable_auto_play ) { renderer->disableAutoPlay(); }
    }
}

/*==========================================================================*/
/**
* @brief マウスがドラッグされた時に実行する関数
* @param event MouseEventへのポインタ
*/
/*==========================================================================*/
void Event::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( m_model->isSet() )
    {
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    local::Program::Logger().pushRayChangeTime( renderer->timer().msec() );
    }
}

/*==========================================================================*/
/**
* @brief マウスが離された時に実行する関数
* @param event MouseEventへのポインタ
* @details 一旦中断していたAutoPlayを再開する
* @note Bird'sEyeView / OrientationAxis Modeは未実装
*/
/*==========================================================================*/
void Event::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( m_model->isSet() )
    {
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    if ( m_enable_auto_play ) { renderer->enableAutoPlay(); }

    if ( m_controller->birdsEyeBox().isChecked() )
    {
    }
    else
    {
    }

    if ( m_controller->orientationAxisBox().isChecked() )
    {
        m_controller->showWidget();
    }
    else
    {
    }

    }
}

/*==========================================================================*/
/**
* @brief マウスがダブルクリックされた時に実行する関数
* @param event MouseEventへのポインタ
* @details 視線方向に一番近いカメラ位置へ移動する
*/
/*==========================================================================*/
void Event::mouseDoubleClickEvent( kvs::MouseEvent* event )
{
    if ( m_model->isSet() )
    {
    const kvs::Vec3 p = kvs::Vec3( 0, 0, 0 );
    const kvs::Vec3 a = kvs::Vec3( 0, 0, -1 );
    const kvs::Mat3 R = m_view->movieScreen().scene()->object()->xform().rotation();
    const kvs::Vec3 dir = ( a - p ) * R.inverted();

    const int x = kvs::Math::Round( dir.x() );
    const int y = kvs::Math::Round( dir.y() );
    const int z = kvs::Math::Round( dir.z() );
    const kvs::Vec3i& pos = m_model->cameraPosition();
    kvs::Vec3i d( x, y, z );
    switch ( event->modifiers() )
    {
    case kvs::Key::ShiftModifier: d *= -1; break;
    default: break;
    }

    kvs::Timer timer( kvs::Timer::Start );
    m_model->updateCameraPosition( pos + d );

    if ( m_controller->focusModeBox().checkState() == Qt::Checked )
    {
        this->focusMode();
    }

    timer.stop();
    local::Program::Logger().pushPositionChangeTime( timer.msec() );

    m_view->movieScreen().update( m_model );
    }
}

/*==========================================================================*/
/**
* @brief キーが押された時に実行する関数
* @param event KeyEventへのポインタ
* @details → / ← / ↑ / ↓ / PageUp / PageDown ：カメラ位置をx,y,z方向に±1移動する
* @details Space：AutoPlay ButtonのON/OFFを切替える
* @details a：FrameIndexを一番最初に戻す ( FrameIndex = 0 )
* @details b：（空）
* @details c：物理量データ表示切替
* @details e：FrameIndexを一番最後にする ( FrameIndex = 総フレーム数 - 1 )
* @details i：（空）
* @details l：LoopPlayのON/OFFを切替える
* @details r：ReversePlayのON/OFFを切替える
* @details s：スクリーンサイズを今より50大きくする
* @details S：スクリーンサイズを今より50小さくする
* @details t：FrameIndexを1つ前に進める
* @details T：FrameIndexを1つ戻す
* @bug s / Sキーのスクリーンサイズを変更する機能の中身は、GLUT版のままであり、
*      Qt版としては変更していないため、動作がおかしい。要確認。
*/
/*==========================================================================*/
void Event::keyPressEvent( kvs::KeyEvent* event )
{
    if ( m_model->isSet() )
    {
    const kvs::Vec3i& pos = m_model->cameraPosition();
    kvs::Vec3i d( 0, 0, 0 );
    switch ( event->key() )
    {
    case kvs::Key::Right: d.x() = +1; break;
    case kvs::Key::Left: d.x() = -1; break;
    case kvs::Key::Up: d.y() = +1; break;
    case kvs::Key::Down: d.y() = -1; break;
    case kvs::Key::PageUp: d.z() = +1; break;
    case kvs::Key::PageDown: d.z() = -1; break;
    default: break;
    }

    kvs::Timer timer( kvs::Timer::Start );
    m_model->updateCameraPosition( pos + d );
    m_controller->cameraPositionChanged();

    if ( m_controller->focusModeBox().checkState() == Qt::Checked )
    {
        this->focusMode();
    }

    timer.stop();
    local::Program::Logger().pushPositionChangeTime( timer.msec() );
    m_view->movieScreen().update( m_model );

    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    switch ( event->key() )
    {
    case kvs::Key::Space:
    {
        if ( renderer->isEnabledAutoPlay() )
        {
            renderer->setEnabledAutoPlay( false );
            m_controller->autoButton().setText( "Play" );
        }
        else
        {
            renderer->setEnabledAutoPlay( true );
            m_controller->autoButton().setText( "Stop" );
        }
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::a:
    {
        const int index = 0;
        m_controller->slider().setValue( index );
        renderer->setCurrentFrameIndex( index );
        renderer->setFrameIndex( index );
        m_view->movieScreen().update( m_model, index );
        break;
    }

    case kvs::Key::b:
    {
        break;
    }

    case kvs::Key::c:
    {
        const kvs::Vec3i pos = m_model->cameraPosition();

        size_t number_of_directories = m_model->numberOfDirectories();
        if ( m_model->flipData() == number_of_directories - 1 )
        {
            m_model->setFlipData( 0 );
            kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
            QString directory_name( QString::fromStdString( directory.name() ) );
            m_controller->flipDataButton().setText( directory_name );
        }
        else
        {
            size_t flip_data = m_model->flipData();
            flip_data++;
            m_model->setFlipData( flip_data );
            kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
            QString directory_name( QString::fromStdString( directory.name() ) );
            m_controller->flipDataButton().setText( directory_name );
        }

        m_model->updateCameraPosition( pos );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::e:
    {
        const int index = m_model->objectPointer()->device().numberOfFrames() - 1;
        m_controller->slider().setValue( index );
        renderer->setCurrentFrameIndex( index );
        renderer->setFrameIndex( index );
        m_view->movieScreen().update( m_model, index );
        break;
    }

    case kvs::Key::i:
    {
    }

    case kvs::Key::l:
    {
        const bool state = m_controller->loopBox().isChecked();
        m_controller->loopBox().setChecked( !state );
        renderer->setEnabledLoopPlay( !state );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::r:
    {
        const bool state = m_controller->reverseBox().isChecked();
        m_controller->reverseBox().setChecked( !state );
        renderer->setEnableReversePlay( !state );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::s:
    {
        int sc_width = m_view->movieScreen().width() + 50;
        int sc_height = m_view->movieScreen().height() + 50;

        m_view->movieScreen().resize( sc_width, sc_height );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::S:
    {
        int sc_width = m_view->movieScreen().width() - 50;
        int sc_height = m_view->movieScreen().height() - 50;

        m_view->movieScreen().resize( sc_width, sc_height );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::t:
    {
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
        int index = renderer->frameIndex();
        m_enable_loop_play = renderer->isEnabledLoopPlay();
        index++;

        if ( m_enable_loop_play )
        {
            if ( index == m_model->objectPointer()->device().numberOfFrames() )
            {
                index = 0;
            }
        }
        else
        {
            if ( index == m_model->objectPointer()->device().numberOfFrames() )
            {
                index = m_model->objectPointer()->device().numberOfFrames() - 1;
            }
        }
        renderer->setFrameIndex( index );
        m_model->objectPointer()->device().setNextFrameIndex( index );
        m_controller->slider().setValue( index );   //sliderに今の位置を反映
        renderer->setCurrentFrameIndex( index );
        renderer->setFrameIndex( index );
        m_view->movieScreen().update( m_model, index );
        break;
    }

    case kvs::Key::T:
    {
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
        int index = renderer->frameIndex();
        m_enable_loop_play = renderer->isEnabledLoopPlay();
        index--;

        if ( m_enable_loop_play )
        {
            if ( index < 0 )
            {
                index = m_model->objectPointer()->device().numberOfFrames() - 1;
            }
        }
        else
        {
            if ( index < 0 )
            {
                index = 0;
            }
        }
        renderer->setFrameIndex( index );
        m_model->objectPointer()->device().setNextFrameIndex( index );
        m_controller->slider().setValue( index );
        renderer->setCurrentFrameIndex( index );
        renderer->setFrameIndex( index );
        m_view->movieScreen().update( m_model, index );
        break;
    }

    default: break;
    }//end of switch( event->key() )
    }//end of if( m_model->isSet() )
}

/*==========================================================================*/
/**
* @brief Resize Eventが発生した時に実行する関数
* @param width スクリーンの幅
* @param height スクリーンの高さ
* @bug ここも上記s / Sキーと同様、GLUT版からQt版へ移行時未修正のため、要見直し。
*/
/*==========================================================================*/
void Event::resizeEvent(int width, int height)
{
    int s_width = kvs::Math::Max( width, height );

    m_view->movieScreen().resize( s_width , s_width );
}

} // end of namespace local
