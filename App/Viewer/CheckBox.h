#pragma once
#include "Model.h"
#include "View.h"
#include <kvs/CheckBox>

namespace local
{

class CheckBox : public kvs::CheckBox
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    CheckBox( local::Model* model, local::View* view );
    void stateChanged();
};

class ReversePlayBox : public kvs::CheckBox
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    ReversePlayBox( local::Model* model, local::View* view );
    void stateChanged();
};

class BirdsEyeBox : public kvs::CheckBox
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    BirdsEyeBox( local::Model* model, local::View* view );
};

class OrientationAxisBox : public kvs::CheckBox
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    OrientationAxisBox( local::Model* model, local::View* view );
};

class FocusModeBox : public kvs::CheckBox
{
private:
    local::Model* m_model;
    local::View* m_view;

public:
    FocusModeBox( local::Model* model, local::View* view );
    void stateChanged();
};

} // end of namespace local
