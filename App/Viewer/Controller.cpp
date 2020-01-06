#include "Controller.h"
#include <kvs/TimerEventListener>
#include <kvs/Directory>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>

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
                        m_controller->autoButton().setText( "Play" );
                    }
                }
                else
                {
                    if ( index == 0 )
                    {
                        renderer->disableAutoPlay();
                        renderer->setCurrentFrameIndex( index );
                        m_controller->autoButton().setText( "Play" );
                    }
                }
            }
        }
        m_view->info().update();
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
    m_timer( ::FrameRate2MSec( model->frameRate() ) )
{
    m_view->movieScreen().addEvent( &(m_event) );
    m_view->movieScreen().addTimerEvent( new ::TimerEvent( model, view, this ), &m_timer );
    m_timer.start();

    m_auto_button = new QPushButton( "Play" );
    connect( m_auto_button, SIGNAL( clicked() ), this, SLOT( autoButtonPressed() ) );

    m_flip_data_button = new QPushButton( " " );
    if ( m_model->isDirectory() )
    {
        std::string path = m_model->directoryPath( m_model->flipData() );
        kvs::Directory directory( path );
        QString directory_name( QString::fromStdString( directory.name() ) );
        m_flip_data_button->setText( directory_name );
    }
    if ( m_model->isFile() )
    {
        m_flip_data_button->setVisible( false );
    }
    connect( m_flip_data_button, SIGNAL( clicked() ), this, SLOT( flipDataButtonPressed() ) );

    m_slider = new QSlider( Qt::Horizontal );
    m_slider->setValue( 0 );
    m_slider->setMaximum( m_model->objectPointer()->device().numberOfFrames() - 1 );
    m_slider->setMinimum( 0 );
    connect( m_slider, SIGNAL( valueChanged(int) ), this, SLOT( sliderValueChanged(int) ) );

    m_loop_box = new QCheckBox( "&Loop" );
    connect( m_loop_box, SIGNAL( toggled(bool) ), this, SLOT( loopBoxStateChanged() ) );

    m_reverse_box = new QCheckBox( "Reverse" );
    connect( m_reverse_box, SIGNAL( toggled(bool) ), this, SLOT( reverseBoxStateChanged() ) );

    m_orientation_axis_box = new QCheckBox( "Orientation Axis" );
    connect( m_orientation_axis_box, SIGNAL( toggled(bool) ), this, SLOT( orientationAxisBoxStateChanged() ) );

    m_birds_eye_box = new QCheckBox( "Bird's Eye View" );
    connect( m_birds_eye_box, SIGNAL( toggled(bool) ), this, SLOT( birdsEyeBoxStateChanged() ) );

    m_focus_mode_box = new QCheckBox( "Focus Mode" );
    connect( m_focus_mode_box, SIGNAL( toggled(bool) ), this, SLOT( focusModeBoxStateChanged() ) );

    this->showWidget();
}

void Controller::showWidget()
{
    QVBoxLayout *left_layout = new QVBoxLayout;
    left_layout->addWidget( m_orientation_axis_box );
    left_layout->addWidget( m_birds_eye_box );
    left_layout->addWidget( m_focus_mode_box );

    QVBoxLayout *play_mode_layout = new QVBoxLayout;
    play_mode_layout->addWidget( m_loop_box );
    play_mode_layout->addWidget( m_reverse_box );

    QHBoxLayout *right_middle_layout = new QHBoxLayout;
    right_middle_layout->addWidget( m_auto_button );
    right_middle_layout->addLayout( play_mode_layout );

    QVBoxLayout *right_layout = new QVBoxLayout;
    right_layout->addWidget( m_slider );
    right_layout->addLayout( right_middle_layout );
    right_layout->addWidget( m_flip_data_button );

    QHBoxLayout *main_layout = new QHBoxLayout;
    main_layout->addLayout( left_layout );
    main_layout->addLayout( right_layout );

    setLayout( main_layout );
}

void Controller::update()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    if ( renderer->isEnabledAutoPlay() )
    {
        m_auto_button->setText("Stop");
    }
    else
    {
        m_auto_button->setText("Play");
    }

    if ( m_model->isDirectory() )
    {
        std::string path = m_model->directoryPath( m_model->flipData() );
        kvs::Directory directory( path );
        QString directory_name( QString::fromStdString( directory.name() ) );
        m_flip_data_button->setText( directory_name );
        m_flip_data_button->setVisible( true );
    }
    if ( m_model->isFile() )
    {
        m_flip_data_button->setVisible( false );
    }

    m_slider->setSliderPosition( 0 );
    m_slider->setMaximum( m_model->objectPointer()->device().numberOfFrames() - 1 );
    m_slider->setMinimum( 0 );

}

void Controller::autoButtonPressed()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    if ( renderer->isEnabledAutoPlay() )
    {
        renderer->setEnabledAutoPlay( false ); // Stop
        m_auto_button->setText("Play");
    }
    else
    {
        renderer->setEnabledAutoPlay( true ); // Play
        m_auto_button->setText("Stop");
    }
}

void Controller::flipDataButtonPressed()
{
    const kvs::Vec3i pos = m_model->cameraPosition();

    size_t number_of_directories = m_model->numberOfDirectories();
    if ( m_model->flipData() == number_of_directories - 1 )
    {
        m_model->setFlipData( 0 );
        kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
        m_flip_data_button->setText( QString::fromStdString( directory.name() ) );
    }
    else
    {
        size_t flip_data = m_model->flipData();
        flip_data++;
        m_model->setFlipData( flip_data );
        kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
        m_flip_data_button->setText( QString::fromStdString( directory.name() ) );
    }

    m_model->updateCameraPosition( pos);
    m_view->movieScreen().update( m_model );
}

void Controller::sliderValueChanged( int value )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    renderer->setCurrentFrameIndex( value );
    renderer->setFrameIndex( value );
}

void Controller::loopBoxStateChanged()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    if ( m_loop_box->checkState() == Qt::Checked )
    {
        renderer->setEnabledLoopPlay( true );
    }
    else
    {
        renderer->setEnabledLoopPlay( false );
    }
}

void Controller::reverseBoxStateChanged()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    if ( m_reverse_box->checkState() == Qt::Checked )
    {
        renderer->setEnableReversePlay( true );
    }
    else
    {
        renderer->setEnableReversePlay( false );
    }
}

void Controller::orientationAxisBoxStateChanged()
{
}

void Controller::birdsEyeBoxStateChanged()
{
}

void Controller::focusModeBoxStateChanged()
{
}

} // end of namespace local
