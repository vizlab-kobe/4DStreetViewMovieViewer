#pragma once
#include "Model.h"
#include "View.h"
#include "Event.h"
#include "Slider.h"
#include "Button.h"
#include "CheckBox.h"
#include "Timer.h"


namespace local
{

class Controller
{
private:
    local::Model* m_model;
    local::View* m_view;
    local::Event m_event;
    local::Slider m_slider;
    local::Button m_button;
//-↓↓---------try---18Oct25--
	local::FlipCameraButton m_flip_camera_button;
//-↑↑---------------18Oct25--
    local::CheckBox m_check_box;
    local::Timer m_timer;
//-↓↓---------try---18Nov09--
	const size_t widget_width;
	const size_t widget_height;
//-↑↑---------------18Nov09--

public:
    Controller( local::Model* model, local::View* view );
    local::Slider& slider() { return m_slider; }
    local::Button& button() { return m_button; }
//-↓↓---------try---18Oct25--
    local::FlipCameraButton& flip_camera_button() { return m_flip_camera_button; }
//-↑↑---------------18Oct25--
    local::CheckBox& checkBox() { return m_check_box; }

//-↓↓---------try---18Nov09--
	void showWidget( const int width, const int height ); 
//-↑↑---------------18Nov09--

//-↓↓---------try---18Nov05--
	void resizeShow( const int width, const int height );
//-↑↑---------------18Nov05--
};

} // end of namespace local
