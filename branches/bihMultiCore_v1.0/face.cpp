#include "face.h"

Face::Face()
{

}

Face::~Face()
{

}

/*Face::Face(const Face& another)
{
	for(int i=0; i<another.m_vertices.size(); ++i)
	{
		m_vertices.push_back(another.m_vertices[i]);
	}
	
    m_boundingBox = another.m_boundingBox;;
    m_normal = another.m_normal;
}*/

void Face::addVertex(Vertex* vertex)
{
    m_vertices.push_back(vertex);
}

Vertex* Face::getVertex(const int& index)
{
    if((index < 0)||(index > (int)(m_vertices.size()-1)))
        return 0;
    
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

