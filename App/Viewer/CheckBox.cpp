#include "CheckBox.h"
#include <4DStreetViewMovieViewer/Lib/SphericalMapMovieRenderer.h>

namespace local
{

CheckBox::CheckBox( local::Model* model, local::View* view ):
    kvs::CheckBox( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    this->setCaption("Loop");
    this->setState( renderer->isEnabledLoopPlay() );
}

void CheckBox::stateChanged()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    renderer->setEnabledLoopPlay( this->state() );
}

ReversePlayBox::ReversePlayBox( local::Model* model, local::View* view ):
    kvs::CheckBox( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );

    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    this->setCaption("Reverse");
    this->setState( renderer->isEnabledReversePlay() );
}

void ReversePlayBox::stateChanged()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    renderer->setEnableReversePlay( this->state() );
}

BirdsEyeBox::BirdsEyeBox( local::Model* model, local::View* view ):
    kvs::CheckBox( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    this->setCaption("Bird's Eye View");
}

OrientationAxisBox::OrientationAxisBox( local::Model* model, local::View* view ):
    kvs::CheckBox( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    this->setCaption("Orientation Axis");
}

FocusModeBox::FocusModeBox( local::Model* model, local::View* view ):
    kvs::CheckBox( &(view->movieScreen()) ),
    m_model( model ),
    m_view( view )
{
    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    this->setCaption("Focus Mode");
}

void FocusModeBox::stateChanged()
{
    typedef lib4dsv::SphericalMapMovieRenderer Renderer;
    Renderer* renderer = Renderer::DownCast( m_view->movieScreen().scene()->renderer("Renderer") );
    renderer->setEnableFocusMode( this->state() );
}

} // end of namespace local
