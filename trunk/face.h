#ifndef _FACE_H
#define _FACE_H

#include "vertex.h"
#include "rect.h"
#include "vec.h"
#include <vector>

using namespace std;

class Face
{
public:
    Face();
    ~Face();

    void addVertex(Vertex* vertex);
    Vertex* getVertex(const int& index);

    void setBoundingBox(const Rect& boundingBox);
    const Rect& boundingBox();

    void setNormal(const Vec& normal);
    const Vec& normal();

private:
    vector<Vertex*> m_vertices;
    Rect m_boundingBox;
    Vec m_normal;

};

#endif

