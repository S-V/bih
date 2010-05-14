#include "face.h"

Face::Face()
{

}

Face::~Face()
{

}

void Face::addVertex(Vertex* vertex)
{
    m_vertices.push_back(vertex);
}

Vertex* Face::getVertex(const int& index)
{
    return m_vertices[index];
}

void Face::setBoundingBox(const Rect& boundingBox)
{
    m_boundingBox = boundingBox;
}

const Rect& Face::boundingBox()
{
    return m_boundingBox;
}

void Face::setNormal(const Vec& normal)
{
    m_normal = normal;
}

const Vec& Face::normal()
{
    return m_normal;
}

