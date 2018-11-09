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

//-↓↓---------try---18Oct25--
FlipCameraButton::FlipCameraButton( local::Model* model, local::View* view ):
	kvs::PushButton( &(view->movieScreen()) ),
	m_model( model ),
	m_view( view )
{
		this->setCaption("Flip Cameras");
}

void FlipCameraButton::pressed()
{
//-↓↓---------try---18Nov02--
    const kvs::Vec3i pos = m_model->cameraPosition();
	if ( m_model->flipCameraOn() == false )
	{
		m_model->setFlipCameraOn( true );
	}
	else
	{
		m_model->setFlipCameraOn( false );
	}

//-↑↑---------------18Nov02--
    m_model->updateCameraPosition( pos);
    m_view->movieScreen().update( m_model );
	this->setCaption("Flip");
}
//-↑↑---------------18Oct25--



} // end of namespace local
