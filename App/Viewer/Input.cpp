/* ***************************************************************************/
/**
* @file Input.cpp
* @brief Inputクラスの実装
*/
/* ***************************************************************************/
#include "Input.h"
#include <fstream>
#include <kvs/Message>

namespace local
{

/*==========================================================================*/
/**
* @brief コンストラクタ
* @param filename ファイル名
*/
/*==========================================================================*/
Input::Input( const std::string& filename )
{
    this->read( filename );
}

/*==========================================================================*/
/**
* @brief 設定ファイルの読み込みを行う関数
* @param filename ファイル名
* @return 正常に読込...true / 読込失敗...false
*/
/*==========================================================================*/
bool Input::read( const std::string& filename )
{
        std::ifstream ifs( filename.c_str() );
        if ( !ifs.is_open() )
        {
            kvsMessageError() << "Cannot open " << filename << "." << std::endl;
            return false;
        }

        ifs >> m_dirname;
        ifs >> m_extension;
        ifs >> m_dimensions[0] >> m_dimensions[1] >> m_dimensions[2];
        ifs >> m_position[0] >> m_position[1] >> m_position[2];
        ifs >> m_frame_rate;
        ifs >> m_data_info;

        return true;
}

} // end of namespace local
