#pragma once
//#include "Model.h"
//#include "View.h"
#include <kvs/EventListener>

namespace local
{

class Model;
class View;
class Controller;

class Event : public kvs::EventListener
{
private:
    local::Model* m_model;
    local::View* m_view;
    local::Controller* m_controller;
    bool m_enable_auto_play;
    bool m_enable_loop_play;
    bool m_enable_focus_mode;

public:
    Event( local::Model* model, local::View* view, local::Controller* controller );

private:
    void focusMode();
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
    void mouseDoubleClickEvent( kvs::MouseEvent* event );
    void keyPressEvent( kvs::KeyEvent* event );
    void resizeEvent( int width, int height );
};

} // end of namespace local
