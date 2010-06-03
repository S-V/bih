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
        //printf("\n\nGenerate Rays\n\n");
        
        int rayCount = image->width() * image->height();
        Ray* allRays = new Ray[rayCount];
        
	    double imageWidth = 2.0f * scene.getNear() * tan(scene.getFovx()/2.0f);  // Width of image in world coords
	    double pixelWidth = imageWidth/image->width();
        
	    // Image axes in world coordinates
	    Vec view = !(scene.getLookat() - scene.getEye());	//view vector
	    Vec u = !(view % scene.getUp());                    //image x-axis (u) = view vector cross prod. with scene up vector
	    Vec v = !(u % view);                                //image y-axis (v) = image x-axis (u) cross prod. with view vector
	    
	    //Pre calculate scene eye + (view * scene.getNear())
	    Vec l_eyeViewNear = scene.getEye() + view * (float)(scene.getNear());
	    
	    //Pre calculate half image width and height
	    Vec l_halves(image->width(),image->height(),0);
	    l_halves = l_halves * 0.5;
        
	    // Trace a ray through every pixel
	    for (int i=0; i<image->height(); i+=4) { //loop unrolled

	    	Vec l_iMaster(i,i+1,i+2,i+3);
	    	
		    for (int j=0; j<image->width(); j+=4) { //loop unrolled

			    // Find position of pixel in world coordinates
		    	//Rolled
			    /*Vec worldPix = scene.getEye() 
				      + view * (float)(scene.getNear())
				      + u*( (j-(image->width()/2.0f))*(float)pixelWidth )
				      + v*( (i-(image->height()/2.0f))*(float)pixelWidth );
				*/
		    	
		    	//Rolled
		    	/*Vec worldPix = l_eyeViewNear
		    				  + u*( (j-(l_halves.data.fVals[0]))*(float)pixelWidth )
    					      + v*( (i-(l_halves.data.fVals[1]))*(float)pixelWidth );*/
		    	
			    //Rolled
		    	//Vec direction = worldPix - scene.getEye();
		    	
		    	//Rolled
			    //direction = !direction;
		    	
		    	//Rolled
                //allRays[(i*image->width())+j].set(direction); 
		    	
		    	//Unrolled
		    	Vec l_jMaster(j,j+1,j+2,j+3);
		    	
		    	Vec l_halfWidths(l_halves[0]);
		    	Vec l_j = l_jMaster - l_halfWidths;
		    	
		    	Vec l_halfHeights(l_halves[1]);
		    	Vec l_i = l_iMaster - l_halfHeights;
		    	
		    	
		    	//Unrolled
		    	Vec worldPix11 = l_eyeViewNear
		    			    				  + u*( l_j[0] * (float)pixelWidth )
		    	    					      + v*( l_i[0]   *(float)pixelWidth );
		    	Vec worldPix12 = l_eyeViewNear
		    			    				  + u*( l_j[0] * (float)pixelWidth )
		    	    					      + v*( l_i[1]   *(float)pixelWidth );
		    	Vec worldPix13 = l_eyeViewNear
		    			    				  + u*( l_j[0] * (float)pixelWidth )
		    	    					      + v*( l_i[2]   *(float)pixelWidth );
		    	Vec worldPix14 = l_eyeViewNear
		    			    				  + u*( l_j[0] * (float)pixelWidth )
		    	    					      + v*( l_i[3]   *(float)pixelWidth );
		    	
		    	Vec worldPix21 = l_eyeViewNear
		    			    				  + u*( l_j[1] *(float)pixelWidth )
		    	    					      + v*( l_i[0]    *(float)pixelWidth );
		    	Vec worldPix22 = l_eyeViewNear
		    			    				  + u*( l_j[1] *(float)pixelWidth )
		    	    					      + v*( l_i[1]    *(float)pixelWidth );
		    	Vec worldPix23 = l_eyeViewNear
		    			    				  + u*( l_j[1] *(float)pixelWidth )
		    	    					      + v*( l_i[2]    *(float)pixelWidth );
		    	Vec worldPix24 = l_eyeViewNear
		    			    				  + u*( l_j[1] *(float)pixelWidth )
		    	    					      + v*( l_i[3]    *(float)pixelWidth );
		    	
		    	Vec worldPix31 = l_eyeViewNear
		    			    				  + u*( l_j[2] *(float)pixelWidth )
		    	    					      + v*( l_i[0]    *(float)pixelWidth );
		    	Vec worldPix32 = l_eyeViewNear
		    			    				  + u*( l_j[2] *(float)pixelWidth )
		    	    					      + v*( l_i[1]    *(float)pixelWidth );
		    	Vec worldPix33 = l_eyeViewNear
		    			    				  + u*( l_j[2] *(float)pixelWidth )
		    	    					      + v*( l_i[2]    *(float)pixelWidth );
		    	Vec worldPix34 = l_eyeViewNear
		    			    				  + u*( l_j[2] *(float)pixelWidth )
		    	    					      + v*( l_i[3]    *(float)pixelWidth );
		    	
		    	Vec worldPix41 = l_eyeViewNear
		    			    				  + u*( l_j[3] *(float)pixelWidth )
		    	    					      + v*( l_i[0]    *(float)pixelWidth );
		    	Vec worldPix42 = l_eyeViewNear
		    			    				  + u*( l_j[3] *(float)pixelWidth )
		    	    					      + v*( l_i[1]    *(float)pixelWidth );
		    	Vec worldPix43 = l_eyeViewNear
		    			    				  + u*( l_j[3] *(float)pixelWidth )
		    	    					      + v*( l_i[2]    *(float)pixelWidth );
		    	Vec worldPix44 = l_eyeViewNear
		    			    				  + u*( l_j[3] *(float)pixelWidth )
		    	    					      + v*( l_i[3]    *(float)pixelWidth );
			    
		    	// Direction of ray (note: vector is not normalized)
		    	//Unrolled
		    	Vec direction11 = worldPix11 - scene.getEye();
		    	Vec direction12 = worldPix12 - scene.getEye();
		    	Vec direction13 = worldPix13 - scene.getEye();
		    	Vec direction14 = worldPix14 - scene.getEye();
		    	
		    	Vec direction21 = worldPix21 - scene.getEye();
		    	Vec direction22 = worldPix22 - scene.getEye();
		    	Vec direction23 = worldPix23 - scene.getEye();
		    	Vec direction24 = worldPix24 - scene.getEye();
		    	
		    	Vec direction31 = worldPix31 - scene.getEye();
		    	Vec direction32 = worldPix32 - scene.getEye();
		    	Vec direction33 = worldPix33 - scene.getEye();
		    	Vec direction34 = worldPix34 - scene.getEye();
		    	
		    	Vec direction41 = worldPix41 - scene.getEye();
		    	Vec direction42 = worldPix42 - scene.getEye();
		    	Vec direction43 = worldPix43 - scene.getEye();
		    	Vec direction44 = worldPix44 - scene.getEye();

		    	//normalize direction
		    	//Unrolled
		    	direction11 = !direction11;
		    	direction12 = !direction12;
		    	direction13 = !direction13;
		    	direction14 = !direction14;
		    	
		    	direction21 = !direction21;
		    	direction22 = !direction22;
		    	direction23 = !direction23;
		    	direction24 = !direction24;
		    	
		    	direction31 = !direction31;
		    	direction32 = !direction32;
		    	direction33 = !direction33;
		    	direction34 = !direction34;
		    	
		    	direction41 = !direction41;
		    	direction42 = !direction42;
		    	direction43 = !direction43;
		    	direction44 = !direction44;

			    // Construct the ray
		    	//Unrolled
		    	allRays[((int)l_iMaster[0] *image->width())+ (int)l_jMaster[0] ].set(direction11);
		    	allRays[((int)l_iMaster[0] *image->width())+ (int)l_jMaster[1] ].set(direction21);
		    	allRays[((int)l_iMaster[0] *image->width())+ (int)l_jMaster[2] ].set(direction31);
		    	allRays[((int)l_iMaster[0] *image->width())+ (int)l_jMaster[3] ].set(direction41);
		    	
		    	allRays[((int)l_iMaster[1]*image->width())+ (int)l_jMaster[0] ].set(direction12);
		    	allRays[((int)l_iMaster[1]*image->width())+ (int)l_jMaster[1] ].set(direction22);
		    	allRays[((int)l_iMaster[1]*image->width())+ (int)l_jMaster[2] ].set(direction32);
		    	allRays[((int)l_iMaster[1]*image->width())+ (int)l_jMaster[3] ].set(direction42);
		    	
		    	allRays[((int)l_iMaster[2]*image->width())+ (int)l_jMaster[0] ].set(direction13);
		    	allRays[((int)l_iMaster[2]*image->width())+ (int)l_jMaster[1] ].set(direction23);
		    	allRays[((int)l_iMaster[2]*image->width())+ (int)l_jMaster[2] ].set(direction33);
		    	allRays[((int)l_iMaster[2]*image->width())+ (int)l_jMaster[3] ].set(direction43);
		    	
		    	allRays[((int)l_iMaster[3]*image->width())+ (int)l_jMaster[0] ].set(direction14);
		    	allRays[((int)l_iMaster[3]*image->width())+ (int)l_jMaster[1] ].set(direction24);
		    	allRays[((int)l_iMaster[3]*image->width())+ (int)l_jMaster[2] ].set(direction34);
		    	allRays[((int)l_iMaster[3]*image->width())+ (int)l_jMaster[3] ].set(direction44);
                
                //printf("Ray %d created.\n",i*image->width()+j);
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
