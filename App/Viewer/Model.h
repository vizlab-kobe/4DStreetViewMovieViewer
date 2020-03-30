/* ***************************************************************************/
/**
* @file Model.h
* @brief Modelクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include "Input.h"
#include <kvs/SharedPointer>
#include <kvs/FileList>
#include <4DStreetViewMovieViewer/Lib/MovieObject.h>

namespace local
{

/*==========================================================================*/
/**
* @brief インプット情報からモデルを取得するModelクラスの定義
*/
/*==========================================================================*/
class Model
{
public:
    typedef lib4dsv::MovieObject Object;
    typedef kvs::SharedPointer<Object> ObjectPointer;
    typedef std::vector<std::string> DirectoryList;
    typedef std::vector<kvs::FileList> FileLists;

private:
    local::Input* m_input; ///< pointer to the input

    kvs::Vec3i m_camera_position; ///< カメラ位置( x, y, z )
    kvs::Vec3i m_camera_array_dimensions; ///< カメラ位置の範囲( x × y × z )
    float m_frame_rate; ///< フレームレート
    std::string m_data_info; ///< データ情報
    ObjectPointer m_object_pointer; ///< オブジェクトのポインタ
    kvs::FileList m_files; ///< インプットで得たディレクトリ以下がファイルだった場合のファイルリスト
    DirectoryList m_directories; ///< インプットで得たディレクトリ以下のファイルのディレクトリリスト
    FileLists m_file_lists; ///< インプットで得たディレクトリ以下がフォルダの場合のファイルリスト
    size_t m_flip_data; ///< インプットで得たディレクトリ以下にあるフォルダを切り替えるための配列の番号
    bool m_is_directory; ///< インプットで得たディレクトリ以下のフォルダの有無のフラグ
    bool m_is_file; ///< インプットで得たディレクトリ以下のファイルの有無のフラグ
    bool m_is_set; ///< モデルの有無のフラグ

public:
    Model( local::Input* input );

    /**
    * @brief カメラ位置に該当するファイルを取得する関数
    */
    const kvs::File& file() const { return m_files[this->camera_position_index()]; }
    /**
    * @brief m_directoriesから指定のインデックスのディレクトリパスを取得する関数
    * @param dir_index ディレクトリの配列のインデックス
    */
    const std::string& directoryPath( const size_t dir_index ) const { return m_directories[ dir_index ]; }
    const kvs::Vec3i& cameraPosition() const { return m_camera_position; } ///< m_camera_positionのgetter
    const kvs::Vec3i& cameraArrayDimensions() const { return m_camera_array_dimensions; } ///< m_camera_array_dimensionsのgetter
    const ObjectPointer& objectPointer() const { return m_object_pointer; } ///< m_object_pointerのgetter
    const size_t numberOfDirectories() const { return m_directories.size(); } ///< m_directoriesの配列の数を取得する関数
    float frameRate() const { return m_frame_rate; } ///< m_frame_rateのgetter
    std::string dataInfo() const { return m_data_info; } ///< m_data_infoのgetter
    const size_t flipData() const { return m_flip_data; } ///< m_flip_dataのgetter
    void setFlipData( const size_t num ) { m_flip_data = num; } ///< m_flip_dataに指定の番号をセットする関数
    const bool isDirectory() const { return m_is_directory; } ///< m_is_directoryのgetter
    const bool isFile() const { return m_is_file; } ///< m_is_fileのgetter
    const bool isSet() const { return m_is_set; } ///< m_is_setのgetter

    void load();
    void clear();
    void update();

    const std::string filename() const;
    Object* object() const;
    void updateCameraPosition( const kvs::Vec3i& position );

private:
    void setup_object( const size_t index );
    size_t camera_position_index() const;
};

} // end of namespace local
