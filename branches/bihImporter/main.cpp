#include <stdio.h>
#include <time.h>
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
	printf( "\nBIH Ray Trace - Result Convertor\n\n" );

    //Create image , specify width and height
    Image* image = new Image(500,300);
    
    //Import binary image
    image->importImage();

    //Export image result
    image->exportImage("result.jpg");
    
    //Clean up
    //delete[] rays;
    //delete scene;
    //delete image;
    //delete bih;
}

