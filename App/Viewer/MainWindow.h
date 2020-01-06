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

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    local::Input& m_input;
    local::Model& m_model;
    local::View& m_view;
    local::Controller& m_controller;

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
