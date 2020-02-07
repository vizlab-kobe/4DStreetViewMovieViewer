#include "View.h"


namespace local
{

View::View( local::Application* app, local::Model* model ):
    m_model( model ),
    m_movie_screen( app ),
    m_info( &m_movie_screen )
{
}

void View::setup()
{
    m_movie_screen.setup( m_model );
    m_info.setup( m_model );
}

} // end of namespace local
