/* ***************************************************************************/
/**
* @file  SphericalMapImageRenderer.h
* @brief Definition of SphericalMapImageRendererr class
*/
/* ***************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/ImageObject>
#include <kvs/Module>
#include <kvs/ProgramObject>


namespace lib4dsv
{

class ObjectBase;
class Camera;
class Light;

/*==========================================================================*/
/**
 *  Image renderer class.
 */
/*==========================================================================*/
class SphericalMapImageRenderer : public kvs::RendererBase
{
    kvsModule( lib4dsv::SphericalMapImageRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

public:
    enum Type
    {
        Stretching = 0,
        Centering = 1
    };

private:
    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    Type m_type; ///< rendering type
    kvs::Texture2D m_texture; ///< texture image
    kvs::ProgramObject m_shader_program; ///< shader program

public:
    SphericalMapImageRenderer( const Type& type = SphericalMapImageRenderer::Centering );
    virtual ~SphericalMapImageRenderer();
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void create_shader_program();
    void create_texture( const kvs::ImageObject* image );
    void center_alignment( const double width, const double height );
};

} // end of namespace lib4dsv
