#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/OrientationAxis>
#include <kvs/WidgetBase>

namespace local
{

class OrientationAxes : public kvs::WidgetBase
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    OrientationAxes( local::Model* model, local::View* view );

private:
    void paintEvent();
    void draw_solid_box( const float length);
    void draw_cornered_axis( const float length );
};

} // end of namespace local
