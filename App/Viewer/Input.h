#pragma once
#include <string>
#include <kvs/Vector3>

namespace local
{

struct Input
{
private:
    std::string m_dirname;
    std::string m_extension;
    kvs::Vec3i m_dimensions;
    kvs::Vec3i m_position;
    float m_frame_rate;
    std::string m_data_info;

public:
    Input() {}
    Input( const std::string& filename );

    const std::string& dirname() const { return m_dirname; }
    const std::string& extension() const { return m_extension; }
    const kvs::Vec3i& dimensions() const { return m_dimensions; }
    const kvs::Vec3i& position() const { return m_position; }
    const float frame_rate() const { return m_frame_rate;}
    const std::string& dataInfo() const { return m_data_info; }

    bool read( const std::string& filename );

};

} // end of namespace local
