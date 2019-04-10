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
//-↓↓---------try---18Nov26--
	bool m_enable_loop_play;
//-↑↑---------------18Nov26--
//-↓↓---------try---18Nov09--
//	int previous_width;
//	int previous_height;
//-↑↑---------------18Nov09--


public:
    Event( local::Model* model, local::View* view, local::Controller* controller );

private:
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
    void mouseDoubleClickEvent( kvs::MouseEvent* event );
    void keyPressEvent( kvs::KeyEvent* event );
//-↓↓---------try---18Nov05--
	void resizeEvent( int width, int height );
//-↑↑---------------18Nov05--
};

} // end of namespace local
