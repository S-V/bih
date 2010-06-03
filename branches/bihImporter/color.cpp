#include "color.h"

Color::Color()
{
    //m_val = new float[3];
    m_val[0] = 0;
    m_val[1] = 0;
    m_val[2] = 0;
}

Color::Color(const float& xx, const float& yy, const float& zz)
{
    //m_val = new float[3];
    m_val[0] = xx;
    m_val[1] = yy;
    m_val[2] = zz;
}

Color::~Color()
{
    //delete[] m_val;
}

const float& Color::x()
{
    return m_val[0];
}

const float& Color::y()
{
    return m_val[1];
}

const float& Color::z()
{
    return m_val[2];
}

Color& Color::operator = (const Color& another)
{
    //if(!m_val)
    //    delete[] m_val;

    //m_val = new float[3];
    m_val[0] = another.m_val[0];
    m_val[1] = another.m_val[1];
    m_val[2] = another.m_val[2];

    return *this;
}

float& Color::operator [] (const int& index)
{
    return m_val[index];
}

const float& Color::operator[] (const int& index) const
{
    return m_val[index];
}

Color Color::operator + (const Color& v) const
{
    return Color( m_val[0]+v.m_val[0], m_val[1]+v.m_val[1], m_val[2]+v.m_val[2] );
}

Color Color::operator * (float num) const
{
    return Color( m_val[0]*num, m_val[1]*num, m_val[2]*num );
}

