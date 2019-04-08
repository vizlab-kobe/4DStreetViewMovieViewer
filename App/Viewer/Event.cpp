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
    m_enable_auto_play( false )//,
//	previous_width( 512 ),				/*----18Nov09----*/
//	previous_height( 512 )				/*----18Nov09----*/
{
    setEventType(
	    kvs::EventBase::ResizeEvent |	/*----18Nov05----*/
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
}

void Event::mouseDoubleClickEvent( kvs::MouseEvent* event )
{
    const kvs::Vec3 p = kvs::Vec3( 0, 0, 0 );
    const kvs::Vec3 a = kvs::Vec3( 0, 0, 1 );
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

    switch ( event->key() )
    {
    case kvs::Key::Space:
    {
        m_controller->button().pressed();
        break;
    }
    case kvs::Key::l:
    {
        const bool state = m_controller->checkBox().state();
        m_controller->checkBox().setState( !state );
        m_controller->checkBox().stateChanged();
        break;
    }
//-↓↓---------try---18Nov20--
    case kvs::Key::r:
    {
        const bool state = m_controller->reverseBox().state();
        m_controller->reverseBox().setState( !state );
        m_controller->reverseBox().stateChanged();
        break;
    }
//-↑↑---------------18Nov20--
    case kvs::Key::a:
    {
        const int index = 0;
        m_controller->slider().setValue( index );
        m_controller->slider().sliderMoved();
        break;
    }
    case kvs::Key::e:
    {
        const int index = m_model->objectPointer()->device().numberOfFrames() - 1;
        m_controller->slider().setValue( index );
        m_controller->slider().sliderMoved();
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
		break;
    }
//-↓↓---------try---18Nov09--
	case kvs::Key::s:
	{
		int sc_width = m_view->movieScreen().width() - 50;
		int sc_height = m_view->movieScreen().height() - 50;
			
		m_view->movieScreen().resize( sc_width, sc_height );
		break;
	}
	case kvs::Key::b:
	{
		int sc_width = m_view->movieScreen().width() + 50;
		int sc_height = m_view->movieScreen().height() + 50;
		
		m_view->movieScreen().resize( sc_width, sc_height );
		break;
	}
//-↑↑---------------18Nov09--
    default: break;
    }

    m_view->movieScreen().update( m_model );
}

//-↓↓---------try---18Nov09--

void Event::resizeEvent(int width, int height)
{
//	std::cout << "width:" << previous_width  << ",height:" << previous_height << std::endl;
//	std::cout << "width2:" << width << ",height2:" << height << std::endl;
	
//	int difference_x = kvs::Math::Abs( width - previous_width );
//	int difference_y = kvs::Math::Abs( height - previous_height );
//	int s_width;

//	if ( difference_x >= difference_y )
//	{	
//		s_width = width;
//	}
//	else
//	{	
//		s_width = height;
//	}
	int s_width = kvs::Math::Max( width, height );

	m_view->movieScreen().resize( s_width , s_width );
	m_controller->resizeShow( s_width, s_width );

//	previous_width = s_width;
//	previous_height = s_width;
}
//-↑↑---------------18Nov09--

} // end of namespace local
