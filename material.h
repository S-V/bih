#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "vec.h"

class Material
{
public:
    Material(){}
    ~Material(){}

    Vec Ka, Kd, Ks;    // Ambient, Diffuse, Specular constants
    double shine;        // Shininess
    double reflect;      // Reflectiveness: 1=mirror, 0=Lambertian
    double opacity;      // Translucency: 1=opaque, 0=transparent
    double refract_idx;  // Index of refraction (air=1.0, water=1.3, glass=1.5, diamond=2.47)
};

#endif
