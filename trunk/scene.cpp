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

    if(m_objects.size()==1)
        m_boundingBox = m_objects[0]->boundingBox();
    //else
        //TODO: multiple objects
}

Object* Scene::getObject(int index)
{
    return m_objects[index];
}

vector<Face*>& Scene::getPrimitives()
{
    if(m_objects.size()==1)
        return m_objects[0]->getPrimitives();
    //else
        //TODO: multiple objects
}

Rect& Scene::boundingBox()
{
    return m_boundingBox;
}

const Rect& Scene::boundingBox() const
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

const Vec& Scene::getEye() const
{
    return m_eye;
}

const Vec& Scene::getLookat() const
{
    return m_lookat;
}
 
const Vec& Scene::getUp() const
{
    return m_up;
}
 
const double& Scene::getNear() const
{
    return m_near;
}

const double& Scene::getFovx() const
{
    return m_fovx;
} 
