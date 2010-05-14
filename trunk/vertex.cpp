#include "vertex.h"

Vertex::Vertex(const float& xx, const float& yy, const float& zz, const Vec& nn)
{
    m_position = Vec(xx,yy,zz);
    m_normal = nn;
}

Vertex::~Vertex()
{
}

const Vec& Vertex::position()
{
    return m_position;
}

const Vec& Vertex::normal()
{
    return m_normal;
}

Vertex& Vertex::operator = (const Vertex& another)
{
    m_position = another.m_position;
    m_normal = another.m_normal;
}


