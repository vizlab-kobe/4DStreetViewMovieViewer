/****************************************************************************/
/**
* @file  main.cpp
* @brief main関数を定義
*/
/****************************************************************************/
#include <kvs/ShaderSource>
#include "Program.h"
#include <iostream>


/*==========================================================================*/
/**
* @brief main処理
* @param argc 引数の数
* @param argv 引数の値の配列
* @return リターンコード
*/
/*==========================================================================*/
int main( int argc, char** argv )
{
    kvs::ShaderSource::AddSearchPath("../../Lib"); /** シェーダーのソースコードのパスを追加 */
    local::Program program( argc, argv );          /** programオブジェクトの生成 */
    return program.run();
}
