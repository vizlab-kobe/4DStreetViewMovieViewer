#include "Button.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>
#include <kvs/Directory>


namespace local
{

Button::Button( local::Model* model, local::View* view ):
    kvs::PushButton( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    this->setCaption("Play");
}

void Button::pressed()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    if ( renderer->isEnabledAutoPlay() )
    {
        renderer->setEnabledAutoPlay( false ); // Stop
        this->setCaption("Play");
    }
    else
    {
        renderer->setEnabledAutoPlay( true ); // Play
        this->setCaption("Stop");
    }
}

FlipDataButton::FlipDataButton( local::Model* model, local::View* view ):
    kvs::PushButton( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    if ( model->isDirectory() )
    {
        std::string path = m_model->directoryPath( m_model->flipData() );
        kvs::Directory directory( path );
        this->setCaption( directory.name() );
    }
}

void FlipDataButton::pressed()
{
    const kvs::Vec3i pos = m_model->cameraPosition();

    size_t number_of_directories = m_model->numberOfDirectories();
    if ( m_model->flipData() == number_of_directories - 1 )
    {
        m_model->setFlipData( 0 );
        kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
        this->setCaption( directory.name() );
    }
    else
    {
        size_t flip_data = m_model->flipData();
        flip_data++;
        m_model->setFlipData( flip_data );
        kvs::Directory directory( m_model->directoryPath( m_model->flipData() ) );
        this->setCaption( directory.name() );
    }

    m_model->updateCameraPosition( pos);
    m_view->movieScreen().update( m_model );
}

} // end of namespace local
