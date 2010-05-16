#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include "scene.h"
#include "image.h"
#include "bihnode.h"
#include "ray.h"

class RayTracer
{
public:
    static void rayTrace(const Scene* scene, Image* image, const BihNode* tree, const Ray* rays)
    {
	
	}
public:
    RayTracer();
    virtual ~RayTracer();
};

#endif
