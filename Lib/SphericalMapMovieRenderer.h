#pragma once
#include <kvs/RendererBase>
#include <kvs/Texture2D>
#include <kvs/Module>
#include <kvs/ProgramObject>
#include "MovieObject.h"


namespace kvs
{
class ObjectBase;
class Camera;
class Light;
}

namespace lib4dsv
{

/*==========================================================================*/
/**
 *  Image renderer class.
 */
/*==========================================================================*/
class SphericalMapMovieRenderer : public kvs::RendererBase
{
    kvsModule( lib4dsv::SphericalMapMovieRenderer, Renderer );
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
    bool m_enable_auto_play;
    bool m_enable_loop_play;
    bool m_enable_reverse_play;
    bool m_enable_focus_mode;
    int m_frame_index;
    int m_current_frame_index;

public:
    SphericalMapMovieRenderer( const Type& type = SphericalMapMovieRenderer::Centering );
    virtual ~SphericalMapMovieRenderer();
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    int frameIndex() const { return m_frame_index; }
    int currentFrameIndex() const { return m_current_frame_index; }
    bool isEnabledAutoPlay() const { return m_enable_auto_play; }
    bool isEnabledLoopPlay() const { return m_enable_loop_play; }
    bool isEnabledReversePlay() const { return m_enable_reverse_play; }
    bool isEnabledFocusMode() const { return  m_enable_focus_mode; }

    void setFrameIndex( const int index ) { m_frame_index = index; }
    void setCurrentFrameIndex( const int index ) { m_current_frame_index = index; }
    void setEnabledAutoPlay( const bool enable ) { m_enable_auto_play = enable; }
    void setEnabledLoopPlay( const bool enable ) { m_enable_loop_play = enable; }
    void setEnableReversePlay( const bool enable ) { m_enable_reverse_play = enable; }
    void setEnableFocusMode( const bool enable ) { m_enable_focus_mode = enable; }
    void enableAutoPlay() { this->setEnabledAutoPlay( true ); }
    void enableLoopPlay() { this->setEnabledLoopPlay( true ); }
    void enableReversePlay() { this->setEnableReversePlay( true ); }
    void enableFocusMode() { this->setEnableFocusMode( true ); }
    void disableAutoPlay() { this->setEnabledAutoPlay( false ); }
    void disableLoopPlay() { this->setEnabledLoopPlay( false ); }
    void disableReversePlay() { this->setEnableReversePlay( false ); }
    void disableFocusMode() { this->setEnableFocusMode( false ); }

private:
    void create_shader_program();
    void create_texture( const lib4dsv::MovieObject* movie );
    void center_alignment( const double width, const double height );
};

} // end of namespace lib4dsv
