/* ***************************************************************************/
/**
* @file MainWindow.h
* @brief MainWindowクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "Input.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"

namespace local
{

/*==========================================================================*/
/**
* @brief メイン画面を構成するMainWindowクラスの定義
*/
/*==========================================================================*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    local::Input& m_input; ///< address to the input
    local::Model& m_model; ///< address to the model
    local::View& m_view; ///< address to the view
    local::Controller& m_controller; ///< address to the controller

public:
    MainWindow( local::Input& input, local::Model& model, local::View& view, local::Controller& controller );

private slots:
    void open();

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *openAct;

};

}//end of namespace local
