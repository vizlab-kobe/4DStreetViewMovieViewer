/* ***************************************************************************/
/**
* @file View.h
* @brief Viewクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include "Application.h"
#include "Model.h"
#include "Info.h"
#include "MovieScreen.h"
#include <kvs/Label>


namespace local
{

/*==========================================================================*/
/**
* @brief 画像や情報を表示するためのViewクラスの定義
*/
/*==========================================================================*/
class View
{
private:
    local::Model* m_model; ///< pointer to the model
    local::MovieScreen m_movie_screen; ///< MovieScreenクラスのm_movie_screenの宣言
    local::Info m_info;   ///< Infoクラスのm_infoの宣言

public:
    View( local::Application* app, local::Model* model );
    local::MovieScreen& movieScreen() { return m_movie_screen; } ///< m_movie_screenのgetter
    local::Info& info() { return m_info; } ///< m_infoのgetter

    void setup();
};

} // end of namespace local
