/* ***************************************************************************/
/**
* @file MainWindow.cpp
* @brief MainWindowクラスの実装
*/
/* ***************************************************************************/
#include "MainWindow.h"
#include <QDockWidget>
#include <QSizePolicy>

namespace
{

/*==========================================================================*/
/**
* @brief Dockクラスの定義
*/
/*==========================================================================*/
class Dock : public QDockWidget
{
public:
    /*==========================================================================*/
    /**
    * @brief コンストラクタ
    * @param parent 親クラスへのポインタ
    * @param widget ウィジェットへのポインタ
    * @param width Dockウィジェットの幅
    * @param height Dockウィジェットの高さ
    */
    /*==========================================================================*/
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

/*==========================================================================*/
/**
* @brief コンストラクタ
* @param input Inputのアドレス
* @param model Modelのアドレス
* @param view Viewのアドレス
* @param controller Controllerのアドレス
* @details メインウィンドウのサイズ設定
* @details アクションの作成
* @details メニューバーの作成
* @details Dockウィジェットの設定
*/
/*==========================================================================*/
MainWindow::MainWindow( local::Input& input, local::Model& model, local::View& view, local::Controller& controller ):
    m_input( input ),
    m_model( model ),
    m_view( view ),
    m_controller( controller )
{
    int w0 = 512;
    int h0 = 512;
    int w1 = 400;
    int h1 = h0 * 2 / 5;
    int h2 = h0 * 3 / 5;
    int width = w0 + w1;
    int height = h0; 
    
    this->resize( width, height );
    this->setMinimumSize( width, height );
    this->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum);

    createActions();
    createMenus();

    ::Dock* info_area = new ::Dock( this, &( m_view.info() ), w1, h1 );
    ::Dock* button_area = new ::Dock( this, &( m_controller ), w1, h2 );
    button_area->setAllowedAreas( Qt::RightDockWidgetArea );
    info_area->setAllowedAreas( Qt::RightDockWidgetArea );

    this->addDockWidget( Qt::RightDockWidgetArea, info_area );
    this->addDockWidget( Qt::RightDockWidgetArea, button_area );

    this->setWindowTitle( "4DStreetViewMovieViewer" );
    this->setCentralWidget( &( m_view.movieScreen() ) );
}

/*==========================================================================*/
/**
* @brief メニューバーでFile Openした場合に実行される関数
* @details FileDialogを開き、**.4dsvのファイルのみ選択できるように設定
* @details 選択されたファイルを読み込み、modelのupdateを行う
*/
/*==========================================================================*/
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName( this, "Open Config file", ".", "4DSV files (*.4dsv)" );
    if ( !fileName.isEmpty() )
    {
        std::string filename = fileName.toUtf8().toStdString();
        m_input.read( filename );
        if ( !( m_model.isSet() ) )
        {
            m_model.update();
            m_view.setup();
            m_controller.loopBox().setCheckable( true );
            m_controller.reverseBox().setCheckable( true );
            m_controller.orientationAxisBox().setCheckable( true );
            m_controller.birdsEyeBox().setCheckable( true );
            m_controller.focusModeBox().setCheckable( true );
        }
        else
        {
            m_model.update();
        }
        m_view.movieScreen().update( &( m_model ) );
        m_model.objectPointer()->device().setNextFrameIndex( 0.0 );
        m_view.info().update();
        m_controller.show();
    }
}

/*==========================================================================*/
/**
* @brief アクションを作成する関数
*/
/*==========================================================================*/
void MainWindow::createActions()
{
    openAct = new QAction( "Open", this );
    connect( openAct, SIGNAL( triggered() ), this, SLOT( open() ) ); 
}

/*==========================================================================*/
/**
* @brief メニューバーを作成する関数
*/
/*==========================================================================*/
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( "File" );
    fileMenu->addAction( openAct );
}

}//end of namespace local
