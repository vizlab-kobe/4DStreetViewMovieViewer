#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/PushButton>


namespace local
{

class Button : public kvs::PushButton
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    Button( local::Model* model, local::View* view );
    void pressed();
};

//-↓↓---------try---18Oct25--
class ChangeButton : public kvs::PushButton
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    ChangeButton( local::Model* model, local::View* view );
    void pressed();
};
//-↑↑---------try------------

} // end of namespace local
