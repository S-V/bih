#ifndef _SCENE_H
#define _SCENE_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include "object.h"
#include "light.h"
#include <vector>

using namespace std;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    void addObject(Object* object);
    Object* getObject(int index);
    const Object* getObject(int index) const;
    vector<Face*>& getPrimitives();

    Rect& boundingBox();
    const Rect& boundingBox() const;
    void setBoundingBox(const Rect& boundingBox);

    void setCamera(const Vec& eye, const Vec& lookat, const Vec& up, const double& near, const double& fovx);
    const Vec& getEye() const;
    const Vec& getLookat() const;
    const Vec& getUp() const;
    const double& getNear() const;
    const double& getFovx() const; 

    void setLight(const Vec& color, const Vec& position);
    const Light& light() const;

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

    //light
    Light m_light;
};

#endif
