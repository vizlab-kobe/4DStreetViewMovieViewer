/****************************************************************************/
/**
* @file Program.h
* @brief Programクラスの定義
*/
/****************************************************************************/
#pragma once
#include <kvs/Program>
#include "Logger.h"

namespace local
{

/*==========================================================================*/
/**
* @brief 4DStreetViewMovieViewerの各要素を実行するProgramクラスの定義
*/
/*==========================================================================*/
class Program : public kvs::Program
{
    static local::Logger m_logger; ///< Loggerクラスのm_loggerの宣言
public:
    /**
    * @brief  m_loggerのgetter
    * @return m_logger
    */
    static local::Logger& Logger() { return m_logger; }

    Program(); ///< コンストラクタ

    int exec( int argc, char** argv );
};

} // end of namespace local
