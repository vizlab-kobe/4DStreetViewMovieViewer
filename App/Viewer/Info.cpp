#include "Info.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>

namespace local
{
CameraInfo::CameraInfo( local::Model* model, local::Screen* screen, QWidget *parent ):
    m_model( model ),
    m_screen( screen ),
    QWidget(parent)
{
    m_title_filename = new QLabel( "Filename : ");
    m_title_img_reso = new QLabel( "Image resolution : ");
    m_title_cam_array = new QLabel( "Camera array dimensions : ");
    m_title_cam_pos = new QLabel( "Camera position : ");
    m_title_num_frames = new QLabel( "Number of frames : ");
    m_title_frame_index = new QLabel( "Current frame index : ");
 
    m_filename = new QLabel( "" );
    m_img_reso = new QLabel( "" );
    m_cam_array = new QLabel( "" );
    m_cam_pos = new QLabel( "" );
    m_num_frames = new QLabel( "" );
    m_frame_index = new QLabel( "" );

    QHBoxLayout *filename_layout = new QHBoxLayout;
    filename_layout->addWidget( m_title_filename );
    filename_layout->addWidget( m_filename );

    QHBoxLayout *img_reso_layout = new QHBoxLayout;
    img_reso_layout->addWidget( m_title_img_reso );
    img_reso_layout->addWidget( m_img_reso );

    QHBoxLayout *cam_array_lauout = new QHBoxLayout;
    cam_array_lauout->addWidget( m_title_cam_array );
    cam_array_lauout->addWidget( m_cam_array );

    QHBoxLayout *cam_pos_layout = new QHBoxLayout;
    cam_pos_layout->addWidget( m_title_cam_pos );
    cam_pos_layout->addWidget( m_cam_pos );

    QHBoxLayout *num_frames_layout = new QHBoxLayout;
    num_frames_layout->addWidget( m_title_num_frames );
    num_frames_layout->addWidget( m_num_frames );

    QHBoxLayout *frame_index_layout = new QHBoxLayout;
    frame_index_layout->addWidget( m_title_frame_index );
    frame_index_layout->addWidget( m_frame_index );

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout( filename_layout );
    main_layout->addLayout( img_reso_layout );
    main_layout->addLayout( cam_array_lauout );
    main_layout->addLayout( cam_pos_layout );
    main_layout->addLayout( num_frames_layout );
    main_layout->addLayout( frame_index_layout );

    setLayout( main_layout );
}

void CameraInfo::update()
{
    const int width = m_screen->width();
    int height = m_screen->height();
    std::string img_reso = std::to_string( width ) + " x " + std::to_string( height );

    const kvs::Vec3i dims = m_model->cameraArrayDimensions();
    std::string dim_x = std::to_string( dims.x() );
    std::string dim_y = std::to_string( dims.y() );
    std::string dim_z = std::to_string( dims.z() );
    std::string cam_array = dim_x + " x " + dim_y + " x " + dim_z;

    const kvs::Vec3i pos = m_model->cameraPosition();
    std::string pos_x = std::to_string( pos.x() );
    std::string pos_y = std::to_string( pos.y() );
    std::string pos_z = std::to_string( pos.z() );
    std::string cam_pos = "( " + pos_x + " , " + pos_y + " , " + pos_z + " )";

    const int nframes = (int)m_model->objectPointer()->device().numberOfFrames();
    std::string num_frames = std::to_string( nframes );

    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_screen->scene()->renderer("Renderer") );
    const int index = renderer->currentFrameIndex();
    std::string frame_index = std::to_string( index );

    const std::string std_filename( m_model->filename().c_str() );
    const std::string std_img_reso( img_reso.c_str() );
    const std::string std_cam_array( cam_array.c_str() );
    const std::string std_cam_pos( cam_pos.c_str() );
    const std::string std_num_frames( num_frames.c_str() );
    const std::string std_frame_index( frame_index.c_str() );

    const QString qt_file_name( QString::fromStdString( std_filename ));
    const QString qt_image_resolution( QString::fromStdString( std_img_reso ));
    const QString qt_camera_array( QString::fromStdString( std_cam_array ) );
    const QString qt_camera_position( QString::fromStdString( std_cam_pos ) );
    const QString qt_number_of_frames( QString::fromStdString( std_num_frames ) );
    const QString qt_current_frame_index( QString::fromStdString( std_frame_index ) );

    m_filename->setText( qt_file_name );
    m_img_reso->setText( qt_image_resolution );
    m_cam_array->setText( qt_camera_array );
    m_cam_pos->setText( qt_camera_position);
    m_num_frames->setText( qt_number_of_frames );
    m_frame_index->setText( qt_current_frame_index );
}

DataInfo::DataInfo( local::Model* model, QWidget *parent ):
    m_model( model ),
    QWidget(parent)
{
    m_information = new QLabel( "" );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( m_information );
    setLayout(layout);

    m_information->setWordWrap(true);
}

void DataInfo::update()
{

    const std::string std_information( m_model->dataInfo().c_str() );
    const QString qt_data_info( QString::fromStdString( std_information ) );

    m_information->setText( qt_data_info );
}

Info::Info( local::Screen* screen, QWidget *parent ):
    m_model( NULL ),
    QWidget(parent)
{
    tabWidget = new QTabWidget;

    m_camera_info = new CameraInfo( m_model, screen );
    m_data_info = new DataInfo( m_model );

    tabWidget->addTab( m_camera_info, "Camera Info" );
    tabWidget->addTab( m_data_info, "Data Info" );

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( tabWidget);
    setLayout( mainLayout );
}


void Info::setup( local::Model* model )
{
    m_model = model;
    m_camera_info->setup( model );
    m_data_info->setup( model );
}

void Info::update()
{
    if ( m_model )
    {
        m_camera_info->update();
        m_data_info->update();
    }
}

} // end of namespace local
