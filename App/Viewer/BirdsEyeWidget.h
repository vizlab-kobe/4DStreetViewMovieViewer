#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/OrientationAxis>
#include <kvs/WidgetBase>

namespace local
{

class BirdsEyeWidget : public kvs::WidgetBase
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    BirdsEyeWidget( local::Model* model, local::View* view );

private:
    void paintEvent();
    void draw_boundary();
    void draw_cameras();
    void draw_frustum();
    void draw_arrowglyph();
    void draw_sphereglyph();
    void initialize();
};

} // end of namespace local
