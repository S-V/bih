#include <stdio.h>
#include "mloader.h"
#include "image.h"
#include "scene.h"
#include "ray.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	printf( "\nRay Tracing - BIH\n\n" );

    //Load mesh from file
    Object* targetObject = new Object();
    MLoader::loadM("/home/yzong/Dissertation/Models/models/lion.m",targetObject);
    //targetObject->toConsole();

    //Initialize scene    
    Image* image = new Image(500,300);
    Scene* scene = new Scene();
    scene->addObject(targetObject);
    scene->setCamera(Vec(15,10,5),Vec(-2,2.5,0),Vec(0,1,0),5,(3.1427/3.0f)); //eye,lookat,up,pnear,fovx    
    
    //Generate rays
    Ray* rays = Ray::generateRays(*scene, image);
    /*
    int rayCount = image->width() * image->height();
    for(int i=0; i<rayCount; i++)
    {
        rays[i].toConsole();
    }
    */
    
    //Generate accel struc - bih

    //Ray trace
    
    delete targetObject;
    //delete[] rays;
}

