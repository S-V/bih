#ifndef _LIGHT_H
#define _LIGHT_H

//#include "color.h"
#include "vec.h"

class Light
{
public:
    Light(){}
    ~Light(){}

    Vec m_color;
    Vec m_position;
};

#endif
