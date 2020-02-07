#pragma once
#include "Input.h"
#include <kvs/SharedPointer>
#include <kvs/FileList>
#include <4DStreetViewMovieViewer/Lib/MovieObject.h>

namespace local
{

class Model
{
public:
    typedef lib4dsv::MovieObject Object;
    typedef kvs::SharedPointer<Object> ObjectPointer;
    typedef std::vector<std::string> DirectoryList;
    typedef std::vector<kvs::FileList> FileLists;

private:
    local::Input* m_input;

    kvs::FileList m_files;
    kvs::Vec3i m_camera_position;
    kvs::Vec3i m_camera_array_dimensions;
    ObjectPointer m_object_pointer;
    DirectoryList m_directories;
    FileLists m_file_lists;
    float m_frame_rate;
    std::string m_data_info;
    size_t m_flip_data;
    bool m_is_directory;
    bool m_is_file;
    bool m_is_set;

public:
    Model( local::Input* input );

    const kvs::File& file() const { return m_files[this->camera_position_index()]; }
    const std::string& directoryPath( const size_t dir_index ) const { return m_directories[ dir_index ]; }
    const kvs::Vec3i& cameraPosition() const { return m_camera_position; }
    const kvs::Vec3i& cameraArrayDimensions() const { return m_camera_array_dimensions; }
    const ObjectPointer& objectPointer() const { return m_object_pointer; }
    const size_t numberOfDirectories() const { return m_directories.size(); }
    float frameRate() const { return m_frame_rate; }
    std::string dataInfo() const { return m_data_info; }
    const size_t flipData() const { return m_flip_data; }
    void setFlipData( const size_t num ) { m_flip_data = num; }
    const bool isDirectory() const { return m_is_directory; }
    const bool isFile() const { return m_is_file; }
    const bool isSet() const { return m_is_set; }

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
