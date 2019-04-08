#include "Info.h"
#include <kvs/Font>
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>


namespace local
{

Info::Info( local::Screen* screen ):
    kvs::Label( screen ),
    m_model( NULL )
{
    kvs::Font font;
    font.setSize( 24 );
    font.setFamilyToSans();
    font.setStyleToBold();
    font.setColor( kvs::RGBColor::White() );
    font.setShadowColor( kvs::RGBColor::Black() );
    font.setEnabledShadow( true );

    this->setFont( font );
    this->setMargin( 10 );
}

void Info::screenUpdated()
{
    if ( m_model )
    {
        const kvs::Vec3i dims = m_model->cameraArrayDimensions();
        const kvs::Vec3i pos = m_model->cameraPosition();
        const int nframes = (int)m_model->objectPointer()->device().numberOfFrames();

        typedef lib4dsv::SphericalMapMovieRenderer Renderer;
        local::Screen* local_screen = static_cast<local::Screen*>( screen() );
        Renderer* renderer = Renderer::DownCast( local_screen->scene()->renderer("Renderer") );
//-↓↓---------try---18Nov26--
//        const int index = renderer->frameIndex();
		const int index = renderer->currentFrameIndex();
//-↑↑---------------18Nov26--

        this->setText( "Filename: %s", m_model->filename().c_str() );
        this->addText( "Image resolution: %d x %d", screen()->width(), screen()->height() );
        this->addText( "Camera array dimensions: %d x %d x %d", dims.x(), dims.y(), dims.z() );
        this->addText( "Camera position: (%d, %d, %d)", pos.x(), pos.y(), pos.z() );
        this->addText( "Number of frames: %d", nframes );
        this->addText( "Current frame index: %d", index );
    }
}

} // end of namespace local
