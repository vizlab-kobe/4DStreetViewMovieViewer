#include "Event.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Program.h"
#include <kvs/Timer>
#include <kvs/Camera>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>

namespace local
{

Event::Event( local::Model* model, local::View* view, local::Controller* controller ):
    m_model( model ),
    m_view( view ),
    m_controller( controller ),
    m_enable_auto_play( false ),
    m_enable_loop_play( false )
{
    setEventType(
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent |
        kvs::EventBase::MouseDoubleClickEvent |
        kvs::EventBase::KeyPressEvent );
}

void Event::mousePressEvent( kvs::MouseEvent* event )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    m_enable_auto_play = renderer->isEnabledAutoPlay();
    if ( m_enable_auto_play ) { renderer->disableAutoPlay(); }
}

void Event::mouseMoveEvent( kvs::MouseEvent* event )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    local::Program::Logger().pushRayChangeTime( renderer->timer().msec() );
}

void Event::mouseReleaseEvent( kvs::MouseEvent* event )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    if ( m_enable_auto_play ) { renderer->enableAutoPlay(); }
    if ( m_controller->birdsEyeBox().state() )
    {
        m_controller->showWidget( m_view->movieScreen().width(), m_view->movieScreen().height() );
    }
    else
    {
        m_controller->hideWidget();
    }
    if ( m_controller->orientationAxisBox().state() )
    {
        m_controller->showWidget( m_view->movieScreen().width(), m_view->movieScreen().height() );
    }
    else
    {
        m_controller->hideWidget();
    }
}

void Event::mouseDoubleClickEvent( kvs::MouseEvent* event )
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
    timer.stop();
    local::Program::Logger().pushPositionChangeTime( timer.msec() );

    m_view->movieScreen().update( m_model );
}

void Event::keyPressEvent( kvs::KeyEvent* event )
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
    timer.stop();
    local::Program::Logger().pushPositionChangeTime( timer.msec() );
    m_view->movieScreen().update( m_model );

    switch ( event->key() )
    {
    case kvs::Key::Space:
    {
        m_controller->button().pressed();
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::a:
    {
        const int index = 0;
        m_controller->slider().setValue( index );
        m_controller->slider().sliderMoved();
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
        if ( m_model->flipCameraOn() == false )
        {
            m_model->setFlipCameraOn( true );
            m_controller->flip_camera_button().setCaption("Cameras OFF");
        }
        else
        {
            m_model->setFlipCameraOn( false );
            m_controller->flip_camera_button().setCaption("Cameras ON");
        }

        m_model->updateCameraPosition( pos );
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::e:
    {
        const int index = m_model->objectPointer()->device().numberOfFrames() - 1;
        m_controller->slider().setValue( index );
        m_controller->slider().sliderMoved();
        m_view->movieScreen().update( m_model, index );
        break;
    }

    case kvs::Key::i:
    {
        if ( m_view->info().isShown() )
        {
            m_view->info().hide();
        }
        else
        {
            m_view->info().show();
        }
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::l:
    {
        const bool state = m_controller->checkBox().state();
        m_controller->checkBox().setState( !state );
        m_controller->checkBox().stateChanged();
        m_view->movieScreen().update( m_model );
        break;
    }

    case kvs::Key::r:
    {
        const bool state = m_controller->reverseBox().state();
        m_controller->reverseBox().setState( !state );
        m_controller->reverseBox().stateChanged();
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
        m_controller->slider().sliderMoved();
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
        m_controller->slider().sliderMoved();
        m_view->movieScreen().update( m_model, index );
        break;
    }

    default: break;
    }
}

void Event::resizeEvent(int width, int height)
{
    int s_width = kvs::Math::Max( width, height );

    m_view->movieScreen().resize( s_width , s_width );
    m_controller->resizeShow( s_width, s_width );
}

} // end of namespace local
