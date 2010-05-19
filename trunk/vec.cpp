#include "vec.h"
#include <math.h>

Vec::Vec()
{
    m_val = new float[3];
}

Vec::Vec(const float& xx, const float& yy, const float& zz)
{
    m_val = new float[3];
    m_val[0] = xx;
    m_val[1] = yy;
    m_val[2] = zz;
}

Vec::Vec(const Vec& another)
{
    m_val = new float[3];
    m_val[0] = another.m_val[0];
    m_val[1] = another.m_val[1];
    m_val[2] = another.m_val[2];
}

Vec::~Vec()
{
    if(m_val)
        delete[] m_val;
}

const float& Vec::x()
{
    return m_val[0];
}

const float& Vec::y()
{
    return m_val[1];
}

const float& Vec::z()
{
    return m_val[2];
}

float Vec::mag(void) const
{
    return sqrt(m_val[0]*m_val[0] + m_val[1]*m_val[1] + m_val[2]*m_val[2]);	
}

Vec& Vec::operator = (const Vec& another)
{
    m_val[0] = another.m_val[0];
    m_val[1] = another.m_val[1];
    m_val[2] = another.m_val[2];
}

float& Vec::operator [] (const int& index)
{
    return m_val[index];
}

const float& Vec::operator[] (const int& index) const
{
    return m_val[index];
}

Vec Vec::operator - (void) const
{
    return Vec(-m_val[0],-m_val[1],-m_val[2]);
}

Vec Vec::operator ! (void) const
{
    float li = 1/(mag());
    return Vec( m_val[0]*li, m_val[1]*li, m_val[2]*li );
}

Vec Vec::operator + (const Vec& v) const
{
    return Vec( m_val[0]+v.m_val[0], m_val[1]+v.m_val[1], m_val[2]+v.m_val[2] );
}

Vec Vec::operator - (const Vec& v) const
{
    return Vec( m_val[0]-v.m_val[0], m_val[1]-v.m_val[1], m_val[2]-v.m_val[2] );
}

Vec Vec::operator * (float num) const
{
    return Vec( m_val[0]*num, m_val[1]*num, m_val[2]*num );
}

float Vec::operator * (const Vec& v) const
{
    return ((m_val[0]*v.m_val[0])+(m_val[1]*v.m_val[1])+(m_val[2]*v.m_val[2]));
}

Vec Vec::operator % (const Vec& v) const
{
    return Vec( m_val[1]*v.m_val[2]-m_val[2]*v.m_val[1], m_val[2]*v.m_val[0]-m_val[0]*v.m_val[2], m_val[0]*v.m_val[1]-m_val[1]*v.m_val[0] );
}

Vec Vec::operator & (const Vec& v) const
{
    return Vec(m_val[0]*v.m_val[0],m_val[1]*v.m_val[1],m_val[2]*v.m_val[2]); 
}


