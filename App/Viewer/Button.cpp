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
ChangeButton::ChangeButton( local::Model* model, local::View* view ):
	kvs::PushButton( &(view->movieScreen()) ),
	m_model( model ),
	m_view( view )
{
		this->setCaption("TRY!");
}

void ChangeButton::pressed()
{
//-↓↓---------try---18Oct30--
    const kvs::Vec3i try_pos( 5, 5, 5);
    m_model->updateCameraPosition( try_pos);
//	std::string try_path = "/Users/otsujikeiko/Work/Data/4DStreetViewData/movies/test.mp4";
//	m_model->changeFilePath( try_path );
    m_view->movieScreen().update( m_model );
//-↑↑---------------18Oct30--
	this->setCaption("OK!!");
}
//-↑↑---------------18Oct25--



} // end of namespace local
