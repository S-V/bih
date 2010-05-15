#ifndef _RECT_H
#define _RECT_H

#include "vec.h"

class Rect
{
public:
    Rect();
    Rect(const Vec& min, const Vec& max);
    virtual ~Rect();

    Vec& min();
    Vec& max();

    const Vec& min() const;
    const Vec& max() const;

    Rect& operator = (const Rect& another);

private:
    Vec m_min;
    Vec m_max;
};

#endif

