/* ***************************************************************************/
/**
* @file View.cpp
* @brief Viewクラスの実装
*/
/* ***************************************************************************/
#include "View.h"


namespace local
{

/*==========================================================================*/
/**
* @brief コンストラクタ
* @params app Applicationへのポインタ
* @params model Modelへのポインタ
* @details メンバ変数の初期化
*/
/*==========================================================================*/
View::View( local::Application* app, local::Model* model ):
    m_model( model ),
    m_movie_screen( app ),
    m_info( &m_movie_screen )
{
}

/*==========================================================================*/
/**
* @brief 表示するscreen・informationのsetupを行う関数
* @details メンバ変数の初期化
*/
/*==========================================================================*/
void View::setup()
{
    m_movie_screen.setup( m_model );
    m_info.setup( m_model );
}

} // end of namespace local
