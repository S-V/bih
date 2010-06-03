#ifndef _VERTEX_H
#define _VERTEX_H

#include "vec.h"

class Vertex
{
public:
    Vertex(const float& xx, const float& yy, const float& zz, const Vec& nn);
    virtual ~Vertex();

    const Vec& position();
    const Vec& normal();

    Vertex& operator= (const Vertex& another);
    
private:
    Vec m_position;
    Vec m_normal;
};

#endif

