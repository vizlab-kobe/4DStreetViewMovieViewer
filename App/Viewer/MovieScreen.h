/* ***************************************************************************/
/**
* @file MovieScreen.h
* @brief MovieScreenクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include "Application.h"
#include "Screen.h"
#include "Model.h"

namespace local
{

/*==========================================================================*/
/**
* @brief 動画を表示するMovieScreenクラスの定義
*/
/*==========================================================================*/
class MovieScreen : public local::Screen
{
public:
    MovieScreen( local::Application* app );
    void setup( local::Model* model );
    void update( local::Model* model );
    void update( local::Model* model, const int index );
};

} // end of namespace local
