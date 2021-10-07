/* ***************************************************************************/
/**
* @file Model.cpp
* @brief Modelクラスの実装
*/
/* ***************************************************************************/
#include "Model.h"
#include <kvs/File>
#include <kvs/Directory>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>

namespace local
{

/*==========================================================================*/
/**
* @brief コンストラクタ
* @param input インプット
* @details メンバ変数の初期化
*/
/*==========================================================================*/
Model::Model( local::Input* input ):
    m_input( input ),
    m_is_set( false )
{}

/*==========================================================================*/
/**
* @brief インプット情報からモデルを読み込む処理を行う関数
* @details インプット情報から得たディレクトリ以下のフォルダ/ファイルの有無を確認
* @details 各フォルダ/ファイルの情報を取得する
* @details 指定された拡張子のデータのみ取得する
* @details 表示するカメラ位置のデータをsetup_object関数へ渡す
*/
/*==========================================================================*/
void Model::load()
{
    std::string dirname = m_input->dirname();

    kvs::Directory data_dir( dirname ); // inputで得たディレクトリ名からディレクトリクラスのdata_dirを定義
    if ( !data_dir.exists() ) { return; }
    if ( !data_dir.isDirectory() ) { return; }

    /*------------------------------------------*/
    /* dirname以下のファイル構造を調べる        */
    /*------------------------------------------*/
    /* directoryならパスを取得　(m_directories) */
    /* fileなら全てのfileを取得 (all_files)  　 */
    /*------------------------------------------*/
    kvs::FileList all_files;

    DIR* dir = opendir( data_dir.path().c_str() ); // ディレクトリをopenする
    if ( !dir ) { return; }

    struct dirent* ent;
    while ( ( ent = readdir( dir ) ) != NULL )
    {
        const std::string path( data_dir.path() ); // パス
        const std::string name( ent->d_name ); // ファイル名
        if ( name != "." )
        {
            if ( name != ".." )
            {
                if ( name != ".DS_Store" )
                {
                    kvs::Directory all_dir( path + data_dir.Separator() + name );
                    if ( all_dir.isDirectory() ) // ディレクトリであれば
                    {
                        m_directories.push_back( all_dir.path() ); // m_directoriesにパスをpush_backする
                        std::sort( m_directories.begin(), m_directories.end() ); // 昇順にsortする
                        m_is_directory = true;
                        m_is_file = false;
                    }
                    else // ファイルであれば
                    {
                        kvs::File file( path + data_dir.Separator() + name );
                        if ( file.isFile() )
                        {
                            all_files.push_back( file ); // all_filesにfileをpush_backする
                            std::sort( all_files.begin(), all_files.end() ); // 昇順にsortする
                            m_is_directory = false;
                            m_is_file = true;
                        }
                    }
                }
            }
        }
    }
    /*-----------------------------------*/
    /* directory内のファイルを取得する   */
    /*-----------------------------------*/
    /* m_directories → m_file_lists      */
    /* all_files     → m_files           */
    /*-----------------------------------*/
    if ( m_is_directory )
    {
        for ( size_t i = 0; i < m_directories.size(); i++ )
        {
                kvs::Directory type_dir( m_directories[i] );
                if ( !type_dir.exists() ) { return; }
                if ( !type_dir.isDirectory() ) { return; }

                const std::string ext = m_input->extension();
                const kvs::FileList& files = type_dir.fileList( true );
                kvs::FileList file_list;
                for ( size_t j = 0; j < files.size(); j++ )
                {
                    if ( files[j].extension() == ext )
                    {
                        file_list.push_back( files[j] ); // 指定の拡張子のファイルのみfile_listへpush_backする
                    }
                }
                m_file_lists.push_back( file_list ); // file_listをm_file_listsへpush_backする
        }
    }

    if ( m_is_file )
    {
        const std::string ext = m_input->extension();
        for ( size_t i = 0; i < all_files.size(); i++ )
        {
            if ( all_files[i].extension() == ext )
            {
                m_files.push_back( all_files[i] ); // 指定の拡張子のファイルのみm_filesへpush_backする
            }
        }
    }

    // 各変数の値を取得
    m_camera_position = m_input->position();
    m_camera_array_dimensions = m_input->dimensions();
    m_frame_rate = m_input->frame_rate();
    m_data_info = m_input->dataInfo();
    m_flip_data = 0;

    this->setup_object( this->camera_position_index() ); // カメラ位置に相当するモデルをオブジェクトに登録
}

/*==========================================================================*/
/**
* @brief 各ファイルリストの中身を削除する関数
* @details m_file_lists, m_file_lists, m_directoriesを全て削除する
*/
/*==========================================================================*/
void Model::clear()
{
    m_file_lists.clear();
    m_files.clear();
    m_directories.clear();
}

/*==========================================================================*/
/**
* @brief モデルの情報を更新する関数
* @details clear関数とload関数を順に実行する
*/
/*==========================================================================*/
void Model::update()
{
    this->clear();
    this->load();
}

/*==========================================================================*/
/**
* @brief ファイル名を取得する関数
* @return ファイル名
*/
/*==========================================================================*/
const  std::string Model::filename() const
{
    kvs::File file;
    if ( m_is_directory )
    {
        kvs::FileList file_list = m_file_lists[ m_flip_data ];
        file = file_list[ this->camera_position_index() ];
    }
    else if ( m_is_file )
    {
        file = m_files[ this->camera_position_index() ];
    }

    return file.fileName();
}

/*==========================================================================*/
/**
* @brief 表示するオブジェクトのポインタを取得する関数
* @return オブジェクトのポインタ
*/
/*==========================================================================*/
Model::Object* Model::object() const
{
    if ( !m_object_pointer.get() ) { return nullptr; }

    Object* object = new Object();
    object->shallowCopy( *( m_object_pointer.get() ) ); // オブジェクトのポインタをコピーする
    return object;
}

/*==========================================================================*/
/**
* @brief 表示オブジェクトを指定のカメラ位置に該当するオブジェクトに変更する関数
* @param position カメラ位置
*/
/*==========================================================================*/
void Model::updateCameraPosition( const kvs::Vec3i& position )
{
    const kvs::Vec3i dims = m_camera_array_dimensions;
    const int x = kvs::Math::Clamp( position.x(), 0, dims.x() - 1 );
    const int y = kvs::Math::Clamp( position.y(), 0, dims.y() - 1 );
    const int z = kvs::Math::Clamp( position.z(), 0, dims.z() - 1 );
    m_camera_position = kvs::Vec3i( x, y, z );

    this->setup_object( this->camera_position_index() );
}

/*==========================================================================*/
/**
* @brief ファイルリストから指定の配列番号のデータを取得し、表示オブジェクトとして登録する関数
* @param index ファイルの配列の番号
*/
/*==========================================================================*/
void Model::setup_object( const size_t index )
{
    kvs::File file;
    if ( m_is_directory )
    {
        kvs::FileList file_list( m_file_lists[ m_flip_data ]);
        if ( file_list.size() <= index ) { return; }
        file = file_list[ index ];
    }
    else if ( m_is_file )
    {
        if ( m_files.size() <= index ) { return; }
        file = m_files[ index ];
    }

    if ( !file.exists() ) { return; }
    if ( !file.isFile() ) { return; }

    const std::string filename = file.filePath();
    m_object_pointer = ObjectPointer( new Object( filename ) );
    m_is_set = true;
}

/*==========================================================================*/
/**
* @brief カメラ位置座標からそのファイルデータのインデックスを算出する関数
* @return ファイルデータのインデックス
*/
/*==========================================================================*/
size_t Model::camera_position_index() const
{
    const kvs::Vec3i& dims = m_camera_array_dimensions;
    const kvs::Vec3i& pos = m_camera_position;
    return pos.x() + dims.x() * pos.y() + dims.x() * dims.y() * pos.z();
}

} // end of namespace local
