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

//-↓↓---------try---18Nov20--
class ReversePlayBox : public kvs::CheckBox
{
private:
	local::Model* m_model;
	local::View* m_view;

public:
	ReversePlayBox( local::Model* model, local::View* view );
	void stateChanged();
};
//-↑↑---------------18Nov20--

} // end of namespace local
