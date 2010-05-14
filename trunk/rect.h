#ifndef _RECT_H
#define _RECT_H

#include "vec.h"

class Rect
{
public:
    Rect();
    Rect(const Vec& min, const Vec& max);
    virtual ~Rect();

    const Vec& min();
    const Vec& max();

    Rect& operator = (const Rect& another);

private:
    Vec m_min;
    Vec m_max;
};

#endif

