#include "Slider.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>


namespace local
{

Slider::Slider( local::Model* model, local::View* view ):
    kvs::Slider( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    this->setCaption( "" );
    this->setValue( 0 );
    this->setRange( 0, m_model->objectPointer()->device().numberOfFrames() - 1 );
    this->hideRange();
}

void Slider::sliderMoved()
{
    const int v = this->value();
    this->setValue( v );

    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
//-↓↓---------try---18Nov26--
    renderer->setCurrentFrameIndex( v );
//-↑↑---------------18Nov26--
    renderer->setFrameIndex( v );
}

} // end of namespace local
