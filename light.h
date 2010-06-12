#ifndef _LIGHT_H
#define _LIGHT_H

#include "color.h"
#include "vec.h"

class Light
{
public:
    Light(){}
    ~Light(){}

    Color m_color;
    Vec m_position;
};

#endif
