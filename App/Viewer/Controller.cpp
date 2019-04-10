#include "Controller.h"
#include <kvs/TimerEventListener>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>


namespace
{

class TimerEvent : public kvs::TimerEventListener
{
private:
    local::Model* m_model;
    local::View* m_view;
    local::Controller* m_controller;

public:
    TimerEvent( local::Model* model, local::View* view, local::Controller* controller ):
        m_model( model ),
        m_view( view ),
        m_controller( controller ) {}

    void update( kvs::TimeEvent* event )
    {
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
        if ( renderer->isEnabledAutoPlay() )
        {
            screen()->redraw();
            const int index = renderer->frameIndex();
            m_controller->slider().setValue( index );

            if ( !renderer->isEnabledLoopPlay() )
            {
//-↓↓---------try---18Nov26--
                if ( !renderer->isEnabledReversePlay() )
                {
//-↑↑---------------18Nov26--
                    const int nframes = (int)m_model->objectPointer()->device().numberOfFrames();
                    if ( index == nframes - 1 )
                    {
                        renderer->disableAutoPlay();
                        renderer->setCurrentFrameIndex( index );
                        m_controller->button().setCaption("Play");
                    }
//-↓↓---------try---18Nov26--
                }
                else
                {
                    if ( index == 0 )
                    {
                        renderer->disableAutoPlay();
					    renderer->setCurrentFrameIndex( index );
                        m_controller->button().setCaption("Play");
                    }
			    }
//-↑↑---------------18Nov26--
		    }
        }
    }
};

inline float FrameRate2MSec( const float fps )
{
    return 1000.0f / fps;
}

}

namespace local
{

Controller::Controller( local::Model* model, local::View* view ):
    m_model( model ),
    m_view( view ),
    m_event( model, view, this ),
    m_slider( model, view ),
    m_button( model, view ),
//-↓↓---------try---18Oct25--
	m_flip_camera_button( model, view ),
//-↑↑---------------18Oct25--
    m_check_box( model, view ),
//-↓↓---------try---18Nov20--
	m_reverse_box( model, view ),
//-↑↑---------------18Nov20--
    m_timer( ::FrameRate2MSec( model->frameRate() ) ),
//-↓↓---------try---18Nov09--
	widget_width(150),
	widget_height(30)
//-↑↑---------------18Nov09--
{
    m_view->movieScreen().addEvent( &m_event );
    m_view->movieScreen().addTimerEvent( new ::TimerEvent( model, view, this ), &m_timer );

    m_slider.setMargin( 0 );
    m_slider.setSize( widget_width, widget_height );
    m_button.setSize( widget_width / 2, widget_height * 2 );
//-↓↓---------try---18Oct25--
	m_flip_camera_button.setSize( widget_width, widget_height );
//-↑↑---------------18Oct25--
    m_check_box.setSize( widget_width / 2, widget_height );
//-↓↓---------try---18Nov20--
	m_reverse_box.setSize( widget_width / 2, widget_height );
//-↑↑---------------18Nov20--

//-↓↓---------try---18Nov09--
	this->showWidget( m_view->movieScreen().width(), m_view->movieScreen().height() );
//-↑↑---------------18Nov09--

}


//-↓↓---------try---18Nov09--
void Controller::showWidget( const int width, const int height )
{
	const size_t screen_width = width;
	const size_t screen_height = height;
	const size_t margin = 10;

//-↓↓---------try---18Nov20--
	m_slider.setPosition( screen_width - widget_width - margin, screen_height - widget_height * 4 - margin * 3 );
    m_button.setPosition( screen_width - widget_width - margin, screen_height - widget_height * 3 - margin );
	m_flip_camera_button.setPosition( screen_width - widget_width - margin, screen_height - widget_height - margin / 2 );
    m_check_box.setPosition( screen_width - widget_width / 2 - margin / 5, screen_height - widget_height * 3 - margin );
	m_reverse_box.setPosition( screen_width - widget_width / 2 - margin / 5, screen_height - widget_height * 2 - margin );
//-↑↑---------------18Nov20--

    m_slider.show();
    m_button.show();
	m_flip_camera_button.show();
    m_check_box.show();
//-↓↓---------try---18Nov20--
	m_reverse_box.show();
//-↑↑---------------18Nov20--

}
//-↑↑---------------18Nov09--



//-↓↓---------try---18Nov05--
void Controller::resizeShow( const int width, const int height )
{
	this->showWidget( width, height );
}
//-↑↑---------------18Nov05--


} // end of namespace local
