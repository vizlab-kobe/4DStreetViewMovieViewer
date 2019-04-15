#include "Button.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>


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

FlipCameraButton::FlipCameraButton( local::Model* model, local::View* view ):
    kvs::PushButton( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    this->setCaption("Cameras ON");
}

void FlipCameraButton::pressed()
{
    const kvs::Vec3i pos = m_model->cameraPosition();
    if ( m_model->flipCameraOn() == false )
    {
        m_model->setFlipCameraOn( true );
        this->setCaption("Cameras OFF");
    }
    else
    {
        m_model->setFlipCameraOn( false );
        this->setCaption("Cameras ON");
    }

    m_model->updateCameraPosition( pos);
    m_view->movieScreen().update( m_model );
}

} // end of namespace local
