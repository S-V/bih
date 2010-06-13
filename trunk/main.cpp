#include <stdio.h>
#include <time.h>
#include "mloader.h"
#include "image.h"
#include "scene.h"
#include "ray.h"
#include "bih.h"
#include "raytracer.h"
#include "vec.h"
//#include "color.h"

using namespace std;

int main( int /*argc*/, const char* /*argv[]*/ )
{
	printf( "\nRay Tracing - BIH\n\n" );

    //Load mesh from file
                                                                                                clock_t startLoad=clock();//printf("%d\n",startLoad); 
    Object* targetObject = new Object();
    
    //g++ linux
    //MLoader::loadM("/home/yzong/Dissertation/Models/models/lion.m",targetObject);
    
    //g++ windows
    //MLoader::loadM("C:\\models\\lion.m",targetObject);
    
    //gcc Cell
    MLoader::loadM("./lion.m",targetObject);
                                                                                                clock_t endLoad=clock()-startLoad;//printf("%d\n",endLoad); 
                                                                                                printf("Load (C.Cycles): %d\n",endLoad);
                                                                                                printf("Load (Time    ): %d\n", endLoad / CLOCKS_PER_SEC);

	
    Vec l_aColor(.2f,.2f,.2f);
    Vec l_dColor(.6f,.6f,.6f);
    Vec l_sColor(.2f,.2f,.2f);
    double opa = 0.4;
    double ref = 0.5;
    double idx = 1.5;
    double shi = 2.0;
    targetObject->setMaterial(l_aColor, l_dColor, l_sColor, shi, ref, opa, idx);
	
    //targetObject->toConsole();
        
                                                                                                clock_t startInitScene=clock();//printf("%d\n",startLoad); 
    //Initialize scene    
    Image* image = new Image(500,300);
    Scene* scene = new Scene();
    scene->addObject(targetObject); //ownership of target object taken by scene
    scene->setCamera(Vec(15,10,5),Vec(-2,2.5,0),Vec(0,1,0),5,(3.1427/3.0f)); //eye,lookat,up,pnear,fovx
    Vec light1Location(2.0f,7.0f,6.0f);
    Vec light1Color(1.0f,1.0f,1.0f);
    scene->setLight(light1Color, light1Location);
                                                                                                clock_t endInitScene=clock()-startInitScene;//printf("%d\n",endLoad); 
                                                                                                printf("Init Scene (C.Cycles): %d\n",endInitScene);
                                                                                                printf("Init Scene (Time    ): %d\n", endInitScene / CLOCKS_PER_SEC);
    

                                                                                                clock_t startGenerate=clock();//printf("%d\n",startLoad); 
    //Generate rays
    Ray* rays = Ray::generateRays(*scene, image);
    /*
    int rayCount = image->width() * image->height();
    for(int i=0; i<rayCount; i++)
    {
        printf("%d ",i);
        rays[i].toConsole();
    }*/
                                                                                                clock_t endGenerate=clock()-startGenerate;//printf("%d\n",endLoad); 
                                                                                                printf("Gen (C.Cycles): %d\n",endGenerate);
                                                                                                printf("Gen (Time    ): %d\n", endGenerate / CLOCKS_PER_SEC);  
    
    
                                                                                                clock_t startBIH=clock();//printf("%d\n",startLoad); 
    //Generate accel struc - bih
    Bih* bih = new Bih();
    bih->buildBIH(scene);
                                                                                                clock_t endBIH=clock()-startBIH;//printf("%d\n",endLoad); 
                                                                                                printf("BIH (C.Cycles): %d\n",endBIH);
                                                                                                printf("BIH (Time    ): %d\n", endBIH / CLOCKS_PER_SEC);  


                                                                                                clock_t startTrace=clock();//printf("%d\n",startLoad); 
    //Ray trace
    RayTracer::rayTrace(scene,image,bih->tree(),rays);
                                                                                                clock_t endTrace=clock()-startTrace;//printf("%d\n",endLoad); 
                                                                                                printf("Trace (C.Cycles): %d\n",endTrace);
                                                                                                printf("Trace (Time    ): %d\n", endTrace / CLOCKS_PER_SEC); 

    //Export image result
    //image->exportImage("result.jpg");
    image->exportImage("./result.bin");
    
    //Clean up
    //delete[] rays;
    //delete scene;
    //delete image;
    //delete bih;
}

