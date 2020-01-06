#include "Input.h"
#include <fstream>
#include <kvs/Message>

namespace local
{

Input::Input( const std::string& filename )
{
    this->read( filename );
}

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
