#ifndef _SCENE_H
#define _SCENE_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include "object.h"
#include <vector>

using namespace std;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    void addObject(Object* object);
    Object* getObject(int index);

    const Rect& boundingBox();
    void setBoundingBox(const Rect& boundingBox);

    void setCamera(const Vec& eye, const Vec& lookat, const Vec& up, const double& near, const double& fovx);
    const Vec& getEye();
    const Vec& getLookat();
    const Vec& getUp();
    const double& getNear();
    const double& getFovx(); 

private:
    //objects
    vector<Object*> m_objects;

    //camera
    Vec m_eye;      // Viewpoint
    Vec m_lookat;   // Focus
    Vec m_up;       // Orientation
    double m_near;   // distance from viewpoint to image plane
    double m_fovx;    // x-angle of view frustum

    //bounding box
    Rect m_boundingBox;

};

#endif
