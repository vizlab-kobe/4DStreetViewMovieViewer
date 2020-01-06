#pragma once
#include "Application.h"
#include "Screen.h"
#include "Model.h"

namespace local
{

class MovieScreen : public local::Screen
{
public:
    MovieScreen( local::Application* app );
    void setup( local::Model* model );
    void update( local::Model* model );
    void update( local::Model* model, const int index );
};

} // end of namespace local
