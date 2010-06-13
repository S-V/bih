#ifndef _OBJECT_H
#define _OBJECT_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include "material.h"
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

    void setMaterial(const Vec& a, const Vec& d, const Vec& s, const double& shine, const double& reflect, const double& opacity, const double& refract_idx);
    const Material& material() const;

    void toConsole();

private:
    vector<Face*> m_faces;
    vector<Vertex*> m_vertices;
    Rect m_boundingBox;
    Material m_material;

};

#endif
