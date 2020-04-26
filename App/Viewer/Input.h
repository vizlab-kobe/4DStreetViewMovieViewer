/* ***************************************************************************/
/**
* @file Input.h
* @brief Inputクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include <string>
#include <kvs/Vector3>

namespace local
{

/*==========================================================================*/
/**
* @brief 設定ファイル(**.4dsv)からの情報を読み込むInputクラスの定義
*/
/*==========================================================================*/
struct Input
{
private:
    std::string m_dirname; ///< データのディレクトリ名
    std::string m_extension; ///< データの拡張子
    kvs::Vec3i m_dimensions; ///< データの数( x × y × z )
    kvs::Vec3i m_position; ///< 最初のカメラ位置( x, y, z )
    float m_frame_rate; ///< フレームレート
    std::string m_data_info; ///< データについての情報

public:
    Input() {} ///< コンストラクタ
    Input( const std::string& filename );

    const std::string& dirname() const { return m_dirname; } ///< m_dirnameのgetter
    const std::string& extension() const { return m_extension; } ///< m_extensionのgetter
    const kvs::Vec3i& dimensions() const { return m_dimensions; } ///< m_dimensionsのgetter
    const kvs::Vec3i& position() const { return m_position; } ///< m_positionのgetter
    const float frame_rate() const { return m_frame_rate;} ///< m_frame_rateのgetter
    const std::string& dataInfo() const { return m_data_info; } ///< m_data_infoのgetter

    bool read( const std::string& filename );

};

} // end of namespace local
