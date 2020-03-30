/****************************************************************************/
/**
* @file Program.cpp
* @brief Programクラスの実装
*/
/****************************************************************************/
#include "Program.h"
#include "Input.h"
#include "Model.h"
#include "View.h"
#include "MainWindow.h"
#include "Controller.h"
#include <cstdlib>

namespace
{

/*==========================================================================*/
/**
* @brief 通常の方法で終了する際に呼ばれる関数の定義
* @details ログを書く
*/
/*==========================================================================*/
void Exit()
{
    local::Program::Logger().write("log");
}

}

namespace local
{

local::Logger Program::m_logger;

/*==========================================================================*/
/**
* @brief コンストラクタ
* @details 通常の方法で終了する際に呼ばれる関数の登録
*/
/*==========================================================================*/
Program::Program()
{
    atexit( ::Exit );
}

/*==========================================================================*/
/**
* @brief プログラムの実行
* @param argc 引数の数
* @param argv 引数の値の配列
* @return リターンコード
* @details 4DStreetViewMovieViewerの各オブジェクトの生成
* @details MainWindowウィジェットの設定・表示
*/
/*==========================================================================*/
int Program::exec( int argc, char** argv )
{
    local::Application app( argc, argv );

    local::Input input;
    local::Model model( &input );
    local::View view( &app, &model );
    local::Controller controller( &model, &view );

    local::MainWindow main_window( input, model, view, controller );
    main_window.setAttribute( Qt::WA_DeleteOnClose ); // ウィジェットが閉じられると自動でdelete
    main_window.show(); // ウィジェットを表示する
    main_window.raise(); // ウィジェットを一番前に表示する

    return app.run();
}

} // end of namespace local
