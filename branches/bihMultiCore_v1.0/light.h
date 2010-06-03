#ifndef _LIGHT_H
#define _LIGHT_H

#include "color.h"
#include "vec.h"

class Light
{
public:
    Light(){}
    ~Light(){}

//TODO: make member variables private
    Color m_color;
    Vec m_position;
};

#endif
