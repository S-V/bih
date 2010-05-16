#include "object.h"

#include <stdio.h>

Object::Object()
{
}

Object::~Object()
{
    for(int i=0; i<m_faces.size(); i++)
    {
        delete m_faces[i];
    }

    for(int j=0; j<m_vertices.size(); j++)
    {
        delete m_vertices[j];
    }
}

void Object::addFace(Face* face)
{
    m_faces.push_back(face);
}

Face* Object::getFace(int index)
{
    return m_faces[index];
}

vector<Face*>& Object::getPrimitives()
{
    return m_faces;
}

void Object::addVertex(Vertex* vertex)
{
    m_vertices.push_back(vertex);
}

Vertex* Object::getVertex(int index)
{
    return m_vertices[index];
}

const Rect& Object::boundingBox()
{
    return m_boundingBox;
}

void Object::setBoundingBox(const Rect& boundingBox)
{
    m_boundingBox = boundingBox;
}

const Material& Object::material() const
{
    return m_material;
}

void Object::toConsole()
{    
    printf( "-----------------------------------------------\n");
    printf( "\nObject\n\n" );
    printf( "-----------------------------------------------\n");

    printf( "   Min: %f %f %f \n",m_boundingBox.min()[0],m_boundingBox.min()[1],m_boundingBox.min()[2]);
    printf( "   Max: %f %f %f \n\n",m_boundingBox.max()[0],m_boundingBox.max()[1],m_boundingBox.max()[2]);

    for(int i=0; i<m_vertices.size(); i++)
    {
        printf("    Vertex %d: \n", i);
        printf("       position: %f %f %f \n",m_vertices[i]->position()[0],m_vertices[i]->position()[1],m_vertices[i]->position()[2]);
        printf("       normal:   %f %f %f \n\n",m_vertices[i]->normal()[0],m_vertices[i]->normal()[1],m_vertices[i]->normal()[2]);
    }

}

