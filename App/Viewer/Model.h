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

private:
    kvs::FileList m_files;
//-↓↓---------try---18Nov02--
	kvs::FileList m_gray_files;
//-↑↑---------------18Nov02--	
    kvs::Vec3i m_camera_position;
    kvs::Vec3i m_camera_array_dimensions;
    ObjectPointer m_object_pointer;
    float m_frame_rate;
//-↓↓---------try---18Nov02--
	bool m_flip_camera_on;
//-↑↑---------------18Nov02--	

public:
    Model( const local::Input& input );

    const kvs::File& file() const { return m_files[this->camera_position_index()]; }
    const std::string filename() const { return m_files[this->camera_position_index()].fileName(); }
    const kvs::Vec3i& cameraPosition() const { return m_camera_position; }
    const kvs::Vec3i& cameraArrayDimensions() const { return m_camera_array_dimensions; }
    const ObjectPointer& objectPointer() const { return m_object_pointer; }
//-↓↓---------try---18Nov02--
	const bool flipCameraOn() const { return m_flip_camera_on; }
	void setFlipCameraOn( const bool  state ) { m_flip_camera_on = state; }
//-↑↑---------------18Nov02--	
    float frameRate() const { return m_frame_rate; }
    Object* object() const;
    void updateCameraPosition( const kvs::Vec3i& position );

private:
    void setup_object( const size_t index );
    size_t camera_position_index() const;
};

} // end of namespace local
