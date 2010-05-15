#ifndef _RAY_H
#define _RAY_H

#include "vertex.h"
#include "scene.h"
#include "image.h"
#include <vector>
#include <math.h>
#include <stdio.h>

using namespace std;

class Ray
{
public:
    static Ray* generateRays(const Scene& scene, Image* image)
    {
        //printf("here0");
        int rayCount = image->width() * image->height();
        Ray* allRays = new Ray[rayCount];
        
	    double imageWidth = 2.0f * scene.getNear() * tan(scene.getFovx()/2.0f);  // Width of image in world coords
	    double pixelWidth = imageWidth/image->width();

        //printf("here1");
        
	    // Image axes in world coordinates
	    Vec view = !(scene.getLookat() - scene.getEye());	//view vector
	    Vec u = !(view % scene.getUp());                    //image x-axis (u) = view vector cross prod. with scene up vector
	    Vec v = !(u % view);                                //image y-axis (v) = image x-axis (u) cross prod. with view vector

        //printf("here2");
        
	    // Trace a ray through every pixel
	    for (int i=0; i<image->width(); i++) {

		    for (int j=0; j<image->height(); j++) {

			    // Find position of pixel in world coordinates
			    Vec worldPix = scene.getEye() 
				      + view * (float)(scene.getNear())
				      + u*( (i-(image->width()/2.0f))*(float)pixelWidth )
				      + v*( (j-(image->height()/2.0f))*(float)pixelWidth );

			    // Direction of ray (note: vector is not normalized)
			    Vec direction = worldPix - scene.getEye();

			    direction = !direction; //normalize direction

			    // Construct the ray
                allRays[(j*image->width())+i].set(direction); 
		    }
	    }

	    return allRays;
    }

public:
    Ray();
    Ray(/*const Vec& position,*/ const Vec& direction);
    virtual ~Ray();

    //Vec& position();
    Vec& direction();

    //const Vec& position() const;
    const Vec& direction() const;

    void set(/*const Vec& position,*/ const Vec& direction);

    Ray& operator = (const Ray& another);

    void toConsole();

private:
    //Vec m_position;
    Vec m_direction;

};

#endif
