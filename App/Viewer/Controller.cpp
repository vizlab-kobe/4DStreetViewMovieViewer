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
                if ( !renderer->isEnabledReversePlay() )
                {
                    const int nframes = (int)m_model->objectPointer()->device().numberOfFrames();
                    if ( index == nframes - 1 )
                    {
                        renderer->disableAutoPlay();
                        renderer->setCurrentFrameIndex( index );
                        m_controller->button().setCaption("Play");
                    }
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
    m_flip_data_button( model, view ),
    m_check_box( model, view ),
    m_reverse_box( model, view ),
    m_birds_eye_box( model, view ),
    m_orientation_axis_box( model, view ),
    m_focus_mode_box( model, view ),
    m_birds_eye_widget( model, view ),
    m_orientation_axis( &(view->movieScreen()), view->movieScreen().scene() ),
    m_timer( ::FrameRate2MSec( model->frameRate() ) ),
    widget_width(150),
    widget_height(30)
{
    m_view->movieScreen().addEvent( &m_event );
    m_view->movieScreen().addTimerEvent( new ::TimerEvent( model, view, this ), &m_timer );

    m_slider.setMargin( 0 );
    m_slider.setSize( widget_width, widget_height );
    m_button.setSize( widget_width / 2, widget_height * 2 );
    m_flip_data_button.setSize( widget_width, widget_height );
    m_check_box.setSize( widget_width / 2, widget_height );
    m_reverse_box.setSize( widget_width / 2, widget_height );
    m_birds_eye_box.setSize( widget_width / 2, widget_height );
    m_orientation_axis_box.setSize( widget_width / 2, widget_height );
    m_focus_mode_box.setSize( widget_width / 2, widget_height );
    m_birds_eye_widget.setSize( 300, 300 );

    this->showWidget( m_view->movieScreen().width(), m_view->movieScreen().height() );
}

void Controller::showWidget( const int width, const int height )
{
    const size_t screen_width = width;
    const size_t screen_height = height;
    const size_t margin = 10;

    m_slider.setPosition( screen_width - widget_width - margin, screen_height - widget_height * 4 - margin * 3 );
    m_button.setPosition( screen_width - widget_width - margin, screen_height - widget_height * 3 - margin );
    m_flip_data_button.setPosition( screen_width - widget_width - margin, screen_height - widget_height - margin / 2 );
    m_check_box.setPosition( screen_width - widget_width / 2 - margin / 5, screen_height - widget_height * 3 - margin / 2 );
    m_reverse_box.setPosition( screen_width - widget_width / 2 - margin / 5, screen_height - widget_height * 2 - margin / 2 );
    m_birds_eye_box.setPosition( screen_width - widget_width * 2 - margin / 5, screen_height - widget_height * 2 - margin / 2 );
    m_orientation_axis_box.setPosition( screen_width - widget_width * 2 - margin / 5, screen_height - widget_height * 3 - margin / 2 );
    m_focus_mode_box.setPosition( screen_width - widget_width * 2 - margin / 5, screen_height - widget_height - margin / 2 );
    m_birds_eye_widget.setPosition( 30, 182 );
    m_birds_eye_widget.setBackgroundColor( kvs::RGBAColor( 0, 0, 0, 0.5 ) );
    m_orientation_axis.setPosition( screen_width - widget_width / 2 - margin * 2, 30 );
    m_orientation_axis.setBoxType( kvs::OrientationAxis::SolidBox );

    m_slider.show();
    m_button.show();

    if ( !m_model->isFile() )
    {
        if ( m_model->isDirectory() )
        {
            m_flip_data_button.show();
        }
    }

    m_check_box.show();
    m_reverse_box.show();
    m_birds_eye_box.show();
    m_orientation_axis_box.show();
    m_focus_mode_box.show();
    if ( m_birds_eye_box.state() )
    {
        m_birds_eye_widget.show();
    }
    if ( m_orientation_axis_box.state() )
    {
        m_orientation_axis.show();
    }
}

void Controller::resizeShow( const int width, const int height )
{
    m_birds_eye_widget.setSize( width - 212, width - 212 );
    this->showWidget( width, height );
}

void Controller::hideWidget()
{
    if ( !m_birds_eye_box.state() )
    {
        m_birds_eye_widget.hide();
    }
    if ( !m_orientation_axis_box.state() )
    {
        m_orientation_axis.hide();
    }
}

} // end of namespace local
