/* ***************************************************************************/
/**
* @file  Event.h
* @brief Definition of Event class
*/
/* ***************************************************************************/
#pragma once
#include <kvs/EventListener>

namespace local
{

class Model;
class View;
class Controller;

/*==========================================================================*/
/**
* @brief Event class receives events from mouse and key.
*/
/*==========================================================================*/
class Event : public kvs::EventListener
{
private:
    local::Model* m_model; ///< pointer to the model
    local::View* m_view; ///< pointer to the view
    local::Controller* m_controller; ///< pointer to the controller
    bool m_enable_auto_play; ///< Flag for AutoPlay ON(true)/OFF(false)
    bool m_enable_loop_play; ///< Flag for LoopPlay ON(true)/OFF(false)
    bool m_enable_focus_mode; ///< Flag for FocusMode ON(true)/OFF(false)

public:
    Event( local::Model* model, local::View* view, local::Controller* controller );

    void focusMode();

private:
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
    void mouseDoubleClickEvent( kvs::MouseEvent* event );
    void keyPressEvent( kvs::KeyEvent* event );
    void resizeEvent( int width, int height );
};

} // end of namespace local
