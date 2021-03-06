/* ***************************************************************************/
/**
* @file  MovieRenderer.h
* @brief Definition of MovieRenderer class
*/
/* ***************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/Module>
#include "MovieObject.h"

namespace kvs
{
class ObjectBase;
class Camera;
class Light;
}

namespace lib4dsv
{

/*===========================================================================*/
/**
 *  @brief  Movie renderer.
 */
/*===========================================================================*/
class MovieRenderer : public kvs::RendererBase
{
    kvsModuleName( lib4dsv::MovieRenderer );
    kvsModuleCategory( Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

public:
    enum Type
    {
        Stretching = 0,
        Centering  = 1
    };

private:
    double m_initial_aspect_ratio; ///< initial aspect ratio
    double m_left; ///< screen left position
    double m_right; ///< screen right position
    double m_bottom; ///< screen bottom position
    double m_top; ///< screen top position
    Type m_type; ///< rendering type
    kvs::Texture2D m_texture; ///< texture image

public:
    MovieRenderer( const Type type = MovieRenderer::Centering );
    virtual ~MovieRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void create_texture( const lib4dsv::MovieObject* movie );
    void centering( const double width, const double height );
};

} // end of namespace lib4dsv
