#include "MainWindow.h"
#include <QDockWidget>

namespace
{

class Dock : public QDockWidget
{
public:
    Dock( QWidget* parent, QWidget* widget, int width, int height )
        :QDockWidget( parent )
    {
        setWidget( widget );
        setTitleBarWidget( new QWidget() );
        setFeatures( QDockWidget::NoDockWidgetFeatures );
        setMinimumWidth( width );
        setMinimumHeight( height );
    }
};

}//end of namespace

namespace local
{

MainWindow::MainWindow( local::Input& input, local::Model& model, local::View& view, local::Controller& controller ):
    m_input( input ),
    m_model( model ),
    m_view( view ),
    m_controller( controller )
{
    int w0 = 512;
    int h0 = 512;
    int w1 = 400;
    int h1 = h0 * 3 / 5;
    int h2 = h0 * 2 / 5;
    int width = w0 + w1;
    int height = h0; 
    
    this->resize( width, height );

    createActions();
    createMenus();

    ::Dock* info_area = new ::Dock( this, &( m_view.info() ), w1, h1 );
    ::Dock* button_area = new ::Dock( this, &( m_controller ), w1, h2 );
    button_area->setAllowedAreas( Qt::RightDockWidgetArea );
    info_area->setAllowedAreas( Qt::RightDockWidgetArea );

    this->addDockWidget( Qt::RightDockWidgetArea, info_area );
    this->addDockWidget( Qt::RightDockWidgetArea, button_area );
    this->setCentralWidget( &( m_view.movieScreen() ) );
    std::cout << "This is Qt Version." << std::endl;
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Open Config file", ".", "4DSV files (*.4dsv)" );
    if ( !fileName.isEmpty() )
    {
        std::string filename = fileName.toUtf8().toStdString();
        m_input.read( filename );
        m_model.update( m_input );
        m_view.movieScreen().update( &( m_model ) );
        m_model.objectPointer()->device().setNextFrameIndex( 0.0 );
        m_view.info().update();
        m_controller.update();
    }
}

void MainWindow::createActions()
{
    openAct = new QAction( "Open", this );
    connect( openAct, SIGNAL( triggered() ), this, SLOT( open() ) ); 
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( "File" );
    fileMenu->addAction( openAct );
}


}//end of namespace local
