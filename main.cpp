#include <stdio.h>
#include "mloader.h"
#include "image.h"
#include "scene.h"
#include "ray.h"
#include "bih.h"
#include "raytracer.h"
#include "vec.h"
#include "color.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	printf( "\nRay Tracing - BIH\n\n" );

    //Load mesh from file
    Object* targetObject = new Object();
    MLoader::loadM("/home/yzong/Dissertation/Models/models/lion.m",targetObject);
    
    Color l_aColor(.2f,.2f,.2f);
    Color l_dColor(.6f,.6f,.6f);
    Color l_sColor(.2f,.2f,.2f);
    double opa = 0.4;
    double ref = 0.5;
    double idx = 1.5;
    double shi = 2.0;
    targetObject->setMaterial(l_aColor, l_dColor, l_sColor, shi, ref, opa, idx);
	//MLoader::loadM("C:\\models\\lion.m",targetObject);
    //targetObject->toConsole();

    //Initialize scene    
    Image* image = new Image(500,300);
    Scene* scene = new Scene();
    scene->addObject(targetObject); //ownership of target object taken by scene
    scene->setCamera(Vec(15,10,5),Vec(-2,2.5,0),Vec(0,1,0),5,(3.1427/3.0f)); //eye,lookat,up,pnear,fovx
    Vec light1Location(2.0f,7.0f,6.0f);
    Color light1Color(1.0f,1.0f,1.0f);
    scene->setLight(light1Color, light1Location);
    
    //Generate rays
    Ray* rays = Ray::generateRays(*scene, image);
    /*
    int rayCount = image->width() * image->height();
    for(int i=0; i<rayCount; i++)
    {
        printf("%d ",i);
        rays[i].toConsole();
    }*/
    
    
    //Generate accel struc - bih
    Bih* bih = new Bih();
    bih->buildBIH(scene);

    //Ray trace
    RayTracer::rayTrace(scene,image,bih->tree(),rays);
    
    //Clean up
    delete[] rays;
    delete scene;
    delete image;
    delete bih;
}

