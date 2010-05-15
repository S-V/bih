#ifndef _OBJECT_H
#define _OBJECT_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include <vector>

using namespace std;

class Object
{
public:
    Object();
    virtual ~Object();

    void addFace(Face* face);
    Face* getFace(int index);
    vector<Face*>& getPrimitives();

    void addVertex(Vertex* vertex);
    Vertex* getVertex(int index);

    const Rect& boundingBox();
    void setBoundingBox(const Rect& boundingBox);

    void toConsole();

private:
    vector<Face*> m_faces;
    vector<Vertex*> m_vertices;
    Rect m_boundingBox;

};

#endif
