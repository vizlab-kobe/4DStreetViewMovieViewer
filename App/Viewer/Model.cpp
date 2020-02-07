#include "Model.h"
#include <kvs/File>
#include <kvs/Directory>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>

namespace local
{

Model::Model( local::Input* input ):
    m_input( input ),
    m_is_set( false )
{}

void Model::load()
{
    std::string dirname = m_input->dirname();

    kvs::Directory data_dir( dirname );
    if ( !data_dir.exists() ) { throw; }
    if ( !data_dir.isDirectory() ) { throw; }

    /*------------------------------------------*/
    /* dirname以下のファイル構造を調べる        */
    /*------------------------------------------*/
    /* directoryならパスを取得　(m_directories) */
    /* fileなら全てのfileを取得 (all_files)  　 */
    /*------------------------------------------*/
    kvs::FileList all_files;

    DIR* dir = opendir( data_dir.path().c_str() );
    if ( !dir ) { throw; }

    struct dirent* ent;
    while ( ( ent = readdir( dir ) ) != NULL )
    {
        const std::string path( data_dir.path() );
        const std::string name( ent->d_name );
        if ( name != "." )
        {
            if ( name != ".." )
            {
                if ( name != ".DS_Store" )
                {
                    kvs::Directory all_dir( path + data_dir.Separator() + name );
                    if ( all_dir.isDirectory() )
                    {
                        m_directories.push_back( all_dir.path() );
                        std::sort( m_directories.begin(), m_directories.end() );
                        m_is_directory = true;
                        m_is_file = false;
                    }
                    else
                    {
                        kvs::File file( path + data_dir.Separator() + name );
                        if ( file.isFile() )
                        {
                            all_files.push_back( file );
                            std::sort( all_files.begin(), all_files.end() );
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
                if ( !type_dir.exists() ) { throw; }
                if ( !type_dir.isDirectory() ) { throw; }

                const std::string ext = m_input->extension();
                const kvs::FileList& files = type_dir.fileList( true );
                kvs::FileList file_list;
                for ( size_t j = 0; j < files.size(); j++ )
                {
                    if ( files[j].extension() == ext )
                    {
                        file_list.push_back( files[j] );
                    }
                }
                m_file_lists.push_back( file_list );
        }
    }

    if ( m_is_file )
    {
        const std::string ext = m_input->extension();
        for ( size_t i = 0; i < all_files.size(); i++ )
        {
            if ( all_files[i].extension() == ext )
            {
                m_files.push_back( all_files[i] );
            }
        }
    }

    m_camera_position = m_input->position();
    m_camera_array_dimensions = m_input->dimensions();
    m_frame_rate = m_input->frame_rate();
    m_data_info = m_input->dataInfo();
    m_flip_data = 0;
    this->setup_object( this->camera_position_index() );
}

void Model::clear()
{
    m_file_lists.clear();
    m_files.clear();
    m_directories.clear();
}

void Model::update()
{
    this->clear();
    this->load();
}

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

Model::Object* Model::object() const
{
    Object* object = new Object();
    object->shallowCopy( *( m_object_pointer.get() ) );
    return object;
}

void Model::updateCameraPosition( const kvs::Vec3i& position )
{
    const kvs::Vec3i dims = m_camera_array_dimensions;
    const int x = kvs::Math::Clamp( position.x(), 0, dims.x() - 1 );
    const int y = kvs::Math::Clamp( position.y(), 0, dims.y() - 1 );
    const int z = kvs::Math::Clamp( position.z(), 0, dims.z() - 1 );
    m_camera_position = kvs::Vec3i( x, y, z );

    this->setup_object( this->camera_position_index() );
}


void Model::setup_object( const size_t index )
{
    kvs::File file;
    if ( m_is_directory )
    {
        kvs::FileList file_list( m_file_lists[ m_flip_data ]);
        file = file_list[ index ];
    }
    else if ( m_is_file )
    {
        file = m_files[ index ];
    }
    if ( !file.exists() ) { throw; }
    if ( !file.isFile() ) { throw; }

    const std::string filename = file.filePath();
    m_object_pointer = ObjectPointer( new Object( filename ) );
    m_is_set = true;
}

size_t Model::camera_position_index() const
{
    const kvs::Vec3i& dims = m_camera_array_dimensions;
    const kvs::Vec3i& pos = m_camera_position;
    return pos.x() + dims.x() * pos.y() + dims.x() * dims.y() * pos.z();
}

} // end of namespace local
