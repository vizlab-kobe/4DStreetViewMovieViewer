/* ***************************************************************************/
/**
* @file Event.h
* @brief Eventクラスの定義
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
* @brief MouseやKeyからイベントを受け取るEventクラスの定義
*/
/*==========================================================================*/
class Event : public kvs::EventListener
{
private:
    local::Model* m_model; ///< pointer to the model
    local::View* m_view; ///< pointer to the view
    local::Controller* m_controller; ///< pointer to the controller
    bool m_enable_auto_play; ///< AutoPlay ON(true)/OFF(false)のフラグ
    bool m_enable_loop_play; ///< LoopPlay ON(true)/OFF(false)のフラグ
    bool m_enable_focus_mode; ///< FocusMode ON(true)/OFF(false)のフラグ

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
