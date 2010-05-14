#include "rect.h"

Rect::Rect()
{
    m_min = Vec(0,0,0);
    m_max = Vec(0,0,0);
}

Rect::Rect(const Vec& min, const Vec& max)
{
    m_min = min;
    m_max = max;
}

Rect::~Rect()
{

}

const Vec& Rect::min()
{
    return m_min;
}

const Vec& Rect::max()
{
    return m_max;
}

Rect& Rect::operator= (const Rect& another)
{
    m_min = another.m_min;
    m_max = another.m_max;
}

