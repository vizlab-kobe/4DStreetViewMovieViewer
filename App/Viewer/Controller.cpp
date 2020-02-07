#include "Controller.h"
#include <kvs/TimerEventListener>
#include <kvs/Directory>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
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
        if ( m_model->isSet() )
        {
            typedef lib4dsv::SphericalMapMovieRenderer Renderer;
            Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
            const int index = renderer->currentFrameIndex();

            if ( renderer->isEnabledAutoPlay() )
            {
                if ( !renderer->isEnabledLoopPlay() )
                {
                    if ( !renderer->isEnabledReversePlay() )
                    {
                        const int nframes = (int)m_model->objectPointer()->device().numberOfFrames();
                        if ( index == nframes - 1 )
                        {
                            renderer->disableAutoPlay();
                        }
                    }
                    else
                    {
                        if ( index == 0 )
                        {
                            renderer->disableAutoPlay();
                        }
                    }
                }
            }
            m_controller->update();
            screen()->redraw();
        }
    }
};

inline float FrameRate2MSec( const float fps )
{
    if ( fps < 1 )
    {
        return 1000.0f / 1000;
    }
    else
    {
        return 1000.0f / fps;
    }
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

    this->createAutoPlayButton();
    this->createFlipDataButton();
    this->createFrameIndex();
    this->createCameraPosition();
    this->createModeBoxes();

    this->showWidget();
}

void Controller::createAutoPlayButton()
{
    m_auto_button = new QPushButton( "Play" );
    connect( m_auto_button, SIGNAL( clicked() ), this, SLOT( autoButtonPressed() ) );

    m_loop_box = new QCheckBox( "&Loop" );
    connect( m_loop_box, SIGNAL( toggled(bool) ), this, SLOT( loopBoxStateChanged() ) );

    m_reverse_box = new QCheckBox( "Reverse" );
    connect( m_reverse_box, SIGNAL( toggled(bool) ), this, SLOT( reverseBoxStateChanged() ) );

    if (!( m_model->isSet() ) )
    {
        m_loop_box->setCheckable( false );
        m_reverse_box->setCheckable( false );
    }
}

void Controller::createFlipDataButton()
{
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
    if ( !( m_model->isSet() ) )
    {
        m_flip_data_button->setVisible( false );
    }

    connect( m_flip_data_button, SIGNAL( clicked() ), this, SLOT( flipDataButtonPressed() ) );
}

void Controller::createFrameIndex()
{
    m_slider = new QSlider( Qt::Horizontal );
    m_slider->setValue( 0 );
    if ( m_model->isSet() )
    {
        m_slider->setRange( 0, m_model->objectPointer()->device().numberOfFrames() - 1 );
    }
    else
    {
        m_slider->setRange( 0, 100 );
    }
    m_slider->setSingleStep( 1 );
    connect( m_slider, SIGNAL( valueChanged(int) ), this, SLOT( currentIndexChanged(int) ) );

    m_current_index_spbox = new QSpinBox();
    m_current_index_spbox->setValue( 0 );
    if ( m_model->isSet() )
    {
        m_current_index_spbox->setRange( 0, m_model->objectPointer()->device().numberOfFrames() - 1 );
    }
    else
    {
        m_current_index_spbox->setRange( 0, 100 );
    }
    m_current_index_spbox->setSingleStep( 1 );
    connect( m_current_index_spbox, SIGNAL( valueChanged(int) ), this, SLOT( currentIndexChanged(int) ) );
}

void Controller::createCameraPosition()
{
    m_x_label = new QLabel( "X" );
    m_y_label = new QLabel( "Y" );
    m_z_label = new QLabel( "Z" );
    m_x_position = new QSlider( Qt::Horizontal );
    m_y_position = new QSlider( Qt::Horizontal );
    m_z_position = new QSlider( Qt::Horizontal );
    m_x_position->setValue( 0 );
    m_y_position->setValue( 0 );
    m_z_position->setValue( 0 );
    if ( m_model->isSet() )
    {
        m_x_position->setRange( 0, m_model->cameraArrayDimensions().x() - 1 );
        m_y_position->setRange( 0, m_model->cameraArrayDimensions().y() - 1 );
        m_z_position->setRange( 0, m_model->cameraArrayDimensions().z() - 1 );
    }
    else
    {
        m_x_position->setRange( 0, 10 );
        m_y_position->setRange( 0, 10 );
        m_z_position->setRange( 0, 10 );
    }
    m_x_position->setSingleStep( 1 );
    m_y_position->setSingleStep( 1 );
    m_z_position->setSingleStep( 1 );
    m_x_pos_spbox = new QSpinBox();
    m_y_pos_spbox = new QSpinBox();
    m_z_pos_spbox = new QSpinBox();
    m_x_pos_spbox->setValue( 0 );
    m_y_pos_spbox->setValue( 0 );
    m_z_pos_spbox->setValue( 0 );
    if ( m_model->isSet() )
    {
        m_x_pos_spbox->setRange( 0, m_model->cameraArrayDimensions().x() - 1 );
        m_y_pos_spbox->setRange( 0, m_model->cameraArrayDimensions().y() - 1 );
        m_z_pos_spbox->setRange( 0, m_model->cameraArrayDimensions().z() - 1 );
    }
    else
    {
        m_x_pos_spbox->setRange( 0, 1 );
        m_y_pos_spbox->setRange( 0, 1 );
        m_z_pos_spbox->setRange( 0, 1 );
    }
    m_x_pos_spbox->setSingleStep( 1 );
    m_y_pos_spbox->setSingleStep( 1 );
    m_z_pos_spbox->setSingleStep( 1 );
    connect( m_x_pos_spbox, SIGNAL( valueChanged(int) ), this, SLOT( spboxCameraPositionChanged() ) );
    connect( m_y_pos_spbox, SIGNAL( valueChanged(int) ), this, SLOT( spboxCameraPositionChanged() ) );
    connect( m_z_pos_spbox, SIGNAL( valueChanged(int) ), this, SLOT( spboxCameraPositionChanged() ) );
    connect( m_x_position, SIGNAL( valueChanged(int) ), this, SLOT( sliderCameraPositionChanged() ) );
    connect( m_y_position, SIGNAL( valueChanged(int) ), this, SLOT( sliderCameraPositionChanged() ) );
    connect( m_z_position, SIGNAL( valueChanged(int) ), this, SLOT( sliderCameraPositionChanged() ) );
}

void Controller::createModeBoxes()
{
    m_orientation_axis_box = new QCheckBox( "Orientation Axis" );
    connect( m_orientation_axis_box, SIGNAL( toggled(bool) ), this, SLOT( orientationAxisBoxStateChanged() ) );

    m_birds_eye_box = new QCheckBox( "Bird's Eye View" );
    connect( m_birds_eye_box, SIGNAL( toggled(bool) ), this, SLOT( birdsEyeBoxStateChanged() ) );

    m_focus_mode_box = new QCheckBox( "Focus Mode" );
    connect( m_focus_mode_box, SIGNAL( toggled(bool) ), this, SLOT( focusModeBoxStateChanged() ) );

    if (!( m_model->isSet() ) )
    {
        m_orientation_axis_box->setCheckable( false );
        m_birds_eye_box->setCheckable( false );
        m_focus_mode_box->setCheckable( false );
    }
}

void Controller::showWidget()
{
    //Camera Position Layout
    QHBoxLayout *x_layout = new QHBoxLayout;
    x_layout->addWidget( m_x_label );
    x_layout->addWidget( m_x_position );
    x_layout->addWidget( m_x_pos_spbox );
    QHBoxLayout *y_layout = new QHBoxLayout;
    y_layout->addWidget( m_y_label );
    y_layout->addWidget( m_y_position );
    y_layout->addWidget( m_y_pos_spbox );
    QHBoxLayout *z_layout = new QHBoxLayout;
    z_layout->addWidget( m_z_label );
    z_layout->addWidget( m_z_position );
    z_layout->addWidget( m_z_pos_spbox );
    QGroupBox *cam_pos = new QGroupBox( "Camera Position" );
    QVBoxLayout *cam_pos_layout = new QVBoxLayout;
    cam_pos_layout->addLayout( x_layout );
    cam_pos_layout->addLayout( y_layout );
    cam_pos_layout->addLayout( z_layout );
    cam_pos->setLayout( cam_pos_layout );

    //Current Frame Index Layout
    QGroupBox *frameIndex = new QGroupBox( "Current Frame Index" );
    QHBoxLayout *frame_index_layout = new QHBoxLayout;
    frame_index_layout->addWidget( m_slider );
    frame_index_layout->addWidget( m_current_index_spbox );
    frameIndex->setLayout( frame_index_layout );

    //Mode Boxes Layout
    QHBoxLayout *low_layout = new QHBoxLayout;
    low_layout->addWidget( m_orientation_axis_box );
    low_layout->addWidget( m_birds_eye_box );
    low_layout->addWidget( m_focus_mode_box );

    //Auto Play Layout
    QVBoxLayout *mode_box_layout = new QVBoxLayout;
    mode_box_layout->addWidget( m_loop_box );
    mode_box_layout->addWidget( m_reverse_box );

    QHBoxLayout *middle_right_layout = new QHBoxLayout;
    middle_right_layout->addStretch();
    middle_right_layout->addWidget( m_auto_button );
    middle_right_layout->addLayout( mode_box_layout );

    QVBoxLayout *play_mode_layout = new QVBoxLayout;
    play_mode_layout->addLayout( middle_right_layout );
    play_mode_layout->addWidget( m_flip_data_button );

    QHBoxLayout *middle_layout = new QHBoxLayout;
    middle_layout->addWidget( cam_pos );
    middle_layout->addLayout( play_mode_layout );

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addStretch();
    main_layout->addWidget( frameIndex );
    main_layout->addLayout( middle_layout );
    main_layout->addLayout( low_layout );
    main_layout->addStretch();

    setLayout( main_layout );
}

void Controller::show() // MainWindow::open()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

//*** Flip Data Button ***
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

//*** Current Frame Index ***
    const int index = renderer->currentFrameIndex();
    m_slider->setSliderPosition( index );
    m_slider->setMaximum( m_model->objectPointer()->device().numberOfFrames() - 1 );
    m_current_index_spbox->setMaximum( m_model->objectPointer()->device().numberOfFrames() - 1 );
    m_current_index_spbox->setValue( index );

//*** Camera Position ***
    const kvs::Vec3i pos = m_model->cameraPosition();
    const kvs::Vec3i dims = m_model->cameraArrayDimensions();
    m_x_position->setMaximum( dims.x() - 1 );
    m_y_position->setMaximum( dims.y() - 1 );
    m_z_position->setMaximum( dims.z() - 1 );
    m_x_pos_spbox->setMaximum( dims.x() - 1 );
    m_y_pos_spbox->setMaximum( dims.y() - 1 );
    m_z_pos_spbox->setMaximum( dims.z() - 1 );
    this->cameraPositionChanged();
}

void Controller::cameraPositionChanged()
{
    const kvs::Vec3i pos = m_model->cameraPosition();

    m_x_position->blockSignals( true );
    m_y_position->blockSignals( true );
    m_z_position->blockSignals( true );
    m_x_position->setSliderPosition( pos.x() );
    m_y_position->setSliderPosition( pos.y() );
    m_z_position->setSliderPosition( pos.z() );
    m_x_position->blockSignals( false );
    m_y_position->blockSignals( false );
    m_z_position->blockSignals( false );

    m_x_pos_spbox->blockSignals( true );
    m_y_pos_spbox->blockSignals( true );
    m_z_pos_spbox->blockSignals( true );
    m_x_pos_spbox->setValue( pos.x() );
    m_y_pos_spbox->setValue( pos.y() );
    m_z_pos_spbox->setValue( pos.z() );
    m_x_pos_spbox->blockSignals( false );
    m_y_pos_spbox->blockSignals( false );
    m_z_pos_spbox->blockSignals( false );

    if ( m_focus_mode_box->checkState() == Qt::Checked )
    {
        m_event.focusMode();
    }

    m_view->movieScreen().update( m_model );
}

void Controller::update()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    m_slider->blockSignals(true);
    m_current_index_spbox->blockSignals(true);

    if ( renderer->isEnabledAutoPlay() )
    {
        m_slider->setValue( renderer->currentFrameIndex() );
        m_current_index_spbox->setValue( renderer->currentFrameIndex() );
    }
    else
    {
        m_slider->setValue( renderer->frameIndex() );
        m_current_index_spbox->setValue( renderer->frameIndex() );
    }

    m_slider->blockSignals(false);
    m_current_index_spbox->blockSignals(false);
}

void Controller::autoButtonPressed()
{
    if ( m_model->isSet() )
    {
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
        if ( renderer->isEnabledAutoPlay() )
        {
            renderer->disableAutoPlay(); // Stop
            m_auto_button->setText("Play");
        }
        else
        {
            renderer->enableAutoPlay(); // Play
            m_auto_button->setText("Stop");
        }
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

void Controller::currentIndexChanged( int value )
{
    if ( m_model->isSet() )
    {
        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
        renderer->setCurrentFrameIndex( value );
        renderer->setFrameIndex( value );

        m_view->movieScreen().update( m_model );

        m_current_index_spbox->blockSignals( true );
        m_current_index_spbox->setValue( value );
        m_current_index_spbox->blockSignals( false );
        m_slider->blockSignals( true );
        m_slider->setValue( value );
        m_slider->blockSignals( false );
    }
}

void Controller::sliderCameraPositionChanged()
{
    if ( m_model->isSet() )
    {
        kvs::Vec3i pos( m_x_position->value(), m_y_position->value(), m_z_position->value() );
        m_model->updateCameraPosition( pos );
        this->cameraPositionChanged();
    }
}

void Controller::spboxCameraPositionChanged()
{
    if ( m_model->isSet() )
    {
        kvs::Vec3i pos( m_x_pos_spbox->value(), m_y_pos_spbox->value(), m_z_pos_spbox->value() );
        m_model->updateCameraPosition( pos );
        this->cameraPositionChanged();
    }
}

void Controller::loopBoxStateChanged()
{
    if ( m_model->isSet() )
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
}

void Controller::reverseBoxStateChanged()
{
    if ( m_model->isSet() )
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
}

void Controller::orientationAxisBoxStateChanged()
{
    if( m_model->isSet() )
    {
    }
}

void Controller::birdsEyeBoxStateChanged()
{
    if( m_model->isSet() )
    {
    }
}

void Controller::focusModeBoxStateChanged()
{
    if( m_model->isSet() )
    {
    }
}

} // end of namespace local
