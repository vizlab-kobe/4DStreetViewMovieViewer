/* ***************************************************************************/
/**
* @file MovieObject.h
* @brief MovieObjectクラスの定義
*/
/* ***************************************************************************/
#pragma once
#include <kvs/ObjectBase>
#include <kvs/Module>
#include <kvs/SharedPointer>
#include <kvs/opencv/CaptureDevice>
#include <string>


namespace lib4dsv
{

/*===========================================================================*/
/**
 *  @brief  Movie object.
 */
/*===========================================================================*/
class MovieObject : public kvs::ObjectBase
{
    kvsModuleName( lib4dsv::MovieObject );
    kvsModuleCategory( Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    typedef kvs::SharedPointer<kvs::opencv::CaptureDevice> CaptureDevice;

    enum PixelType
    {
        Gray8 = 8, ///< 8 bit gray pixel
        Color24 = 24  ///< 24 bit RGB color pixel (8x8x8 bits)
    };

private:
    int m_device_id; ///< capture device ID
    CaptureDevice m_device; ///< video capture device
    PixelType m_type; ///< pixel type
    size_t m_width; ///< capture width
    size_t m_height; ///< capture height
    size_t m_nchannels; ///< number of channels

public:
    MovieObject();
    MovieObject( const std::string& filename );
    virtual ~MovieObject() {}

    ObjectType objectType() const;
    void shallowCopy( const MovieObject& other );

    int deviceID() const { return m_device_id; } ///< getter for the m_device_id
    const kvs::opencv::CaptureDevice& device() const { return *( m_device.get() ); } ///< getter for the video capture device
    PixelType type() const { return m_type; } ///< getter for the m_type
    size_t width() const { return m_width; } ///< getter for the m_width
    size_t height() const { return m_height; } ///< getter for the m_height
    size_t nchannels() const { return m_nchannels; } ///< getter for the m_nchannels

public:
    const bool initialize( const std::string& filename );
};

} // end of namespace lib4dsv
