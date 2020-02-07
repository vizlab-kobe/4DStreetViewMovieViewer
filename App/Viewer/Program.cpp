#include "Program.h"
#include "Input.h"
#include "Model.h"
#include "View.h"
#include "MainWindow.h"
#include "Controller.h"
#include <cstdlib>

namespace
{

void Exit()
{
    local::Program::Logger().write("log");
}

}

namespace local
{

local::Logger Program::m_logger;

Program::Program()
{
    atexit( ::Exit );
}

int Program::exec( int argc, char** argv )
{
    local::Application app( argc, argv );

    local::Input input;
    local::Model model( &input );
    local::View view( &app, &model );
    local::Controller controller( &model, &view );

    local::MainWindow main_window( input, model, view, controller );
    main_window.setAttribute( Qt::WA_DeleteOnClose );
    main_window.show();
    main_window.raise();

    return app.run();
}

} // end of namespace local
