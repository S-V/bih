#include "scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
    for(int i=0; i<m_objects.size(); i++)
    {
        delete m_objects[i];
    }
}

void Scene::addObject(Object* object)
{
    m_objects.push_back(object);
}

Object* Scene::getObject(int index)
{
    return m_objects[index];
}

const Rect& Scene::boundingBox()
{
    return m_boundingBox;
}

void Scene::setBoundingBox(const Rect& boundingBox)
{
    m_boundingBox = boundingBox;
}

void Scene::setCamera(const Vec& eye, const Vec& lookat, const Vec& up, const double& near, const double& fovx)
{
    m_eye = eye;
    m_lookat = lookat;
    m_up = up;

    m_near = near;
    m_fovx = fovx;
}

const Vec& Scene::getEye()
{
    return m_eye;
}

const Vec& Scene::getLookat()
{
    return m_lookat;
}
 
const Vec& Scene::getUp()
{
    return m_up;
}
 
const double& Scene::getNear()
{
    return m_near;
}

const double& Scene::getFovx()
{
    return m_fovx;
} 
