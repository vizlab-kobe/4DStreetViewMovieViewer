/****************************************************************************/
/**
* @file Logger.h
* @brief Loggerクラスの定義
*/
/****************************************************************************/
#pragma once
#include <vector>
#include <string>


namespace local
{

/*==========================================================================*/
/**
* @brief Logを記録するLoggerクラスの定義
*/
/*==========================================================================*/
class Logger
{
private:
    std::vector<float> m_position_change_times; //< カメラ位置変更時間
    std::vector<float> m_ray_change_times; //< カメラアングル変更時間

public:
    /** @brief コンストラクタ */
    Logger() {}

    /**
    * @brief カメラ位置変更時間をm_position_change_timesの末尾に追加する
    * @param msec
    */
    void pushPositionChangeTime( const float msec ) { m_position_change_times.push_back( msec ); }
    /**
    * @brief カメラアングル変更時間をm_ray_change_timesの末尾に追加する
    * @param msec
    */
    void pushRayChangeTime( const float msec ) { m_ray_change_times.push_back( msec ); }
    void write( const std::string& basename = "log" ) const;
};

} // end of namespace local
