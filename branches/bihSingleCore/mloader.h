#ifndef _MLOADER_H
#define _MLOADER_H

#include "object.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <float.h>
#include <stdio.h>

using namespace std;

class MLoader
{
public:
    static void loadM(const char* filename, Object* object)
    {
	    FILE * pFile;           // pointer to model file
	    //errno_t err;            // error number
	    char type;              // type of data in current line read in model file

	    float x,y,z,nx,ny,nz;   // vertex temporary variables
	    int v1,v2,v3;           // face temporary variables
	    int vertex_id, face_id; // vertex and face temporary id
	    Vec* minimum = new Vec(FLT_MAX,FLT_MAX,FLT_MAX);	            //to hold scene minimum point
	    Vec* maximum = new Vec(FLT_MIN_EXP,FLT_MIN_EXP,FLT_MIN_EXP);    //to hold scene maximum point

	    // open file
	    pFile = fopen(filename, "r" );
        
	    // load vectors
	    while((type=fgetc(pFile)) != EOF)
        {
		    switch(type){
			    case 'V':
				    if(fscanf(pFile,"ertex %d %f %f %f {normal=(%f %f %f)}\n", &vertex_id, &x, &y, &z, &nx, &ny, &nz)==7)
				    {
                        Vec l_normal(nx,ny,nz);
					    Vertex* l_vertex = new Vertex(x,y,z,l_normal);		
					
					    object->addVertex(l_vertex);
				
					    //set bounding vertices
					    if(x<minimum->x()){(*minimum)[0]=x;}
					    if(y<minimum->y()){(*minimum)[1]=y;}
					    if(z<minimum->z()){(*minimum)[2]=z;}
					
					    if(x>maximum->x()){(*maximum)[0]=x;}
					    if(y>maximum->y()){(*maximum)[1]=y;}
					    if(z>maximum->z()){(*maximum)[2]=z;}
					
				    }
				    break;
			    case 'F':
				    if(fscanf(pFile,"ace %d %d %d %d\n", &face_id, &v1, &v2, &v3)==4)
                    {
					    Face* l_face = new Face();

                        //vertex data
                        l_face->addVertex(object->getVertex(v1-1));
                        l_face->addVertex(object->getVertex(v2-1));
                        l_face->addVertex(object->getVertex(v3-1));

                        //face min and max declaration
                        Vec minFace(object->getVertex(v1-1)->position()[0],
                                    object->getVertex(v1-1)->position()[1],
                                    object->getVertex(v1-1)->position()[2]);	    //to hold face minimum point
                        Vec maxFace(object->getVertex(v1-1)->position()[0],
                                    object->getVertex(v1-1)->position()[1],
                                    object->getVertex(v1-1)->position()[2]);      //to hold face maximum point

                        // setting face min
                        if(object->getVertex(v2-1)->position()[0] < minFace[0])
                            minFace[0] = object->getVertex(v2-1)->position()[0];
                        if(object->getVertex(v2-1)->position()[1] < minFace[1])
                            minFace[1] = object->getVertex(v2-1)->position()[1];
                        if(object->getVertex(v2-1)->position()[2] < minFace[2])
                            minFace[2] = object->getVertex(v2-1)->position()[2];

                        if(object->getVertex(v3-1)->position()[0] < minFace[0])
                            minFace[0] = object->getVertex(v3-1)->position()[0];
                        if(object->getVertex(v3-1)->position()[1] < minFace[1])
                            minFace[1] = object->getVertex(v3-1)->position()[1];
                        if(object->getVertex(v3-1)->position()[2] < minFace[2])
                            minFace[2] = object->getVertex(v3-1)->position()[2];

                        // setting face max
                        if(object->getVertex(v2-1)->position()[0] > maxFace[0])
                            maxFace[0] = object->getVertex(v2-1)->position()[0];
                        if(object->getVertex(v2-1)->position()[1] > maxFace[1])
                            maxFace[1] = object->getVertex(v2-1)->position()[1];
                        if(object->getVertex(v2-1)->position()[2] > maxFace[2])
                            maxFace[2] = object->getVertex(v2-1)->position()[2];

                        if(object->getVertex(v3-1)->position()[0] > maxFace[0])
                            maxFace[0] = object->getVertex(v3-1)->position()[0];
                        if(object->getVertex(v3-1)->position()[1] > maxFace[1])
                            maxFace[1] = object->getVertex(v3-1)->position()[1];
                        if(object->getVertex(v3-1)->position()[2] > maxFace[2])
                            maxFace[2] = object->getVertex(v3-1)->position()[2];

                        l_face->setBoundingBox(Rect(minFace,maxFace));

                        //face normal
                        Vec v2_minus_v1(l_face->getVertex(1)->position()[0] - l_face->getVertex(0)->position()[0],
                                        l_face->getVertex(1)->position()[1] - l_face->getVertex(0)->position()[1],
                                        l_face->getVertex(1)->position()[2] - l_face->getVertex(0)->position()[2]);

		                Vec v3_minus_v1(l_face->getVertex(2)->position()[0] - l_face->getVertex(0)->position()[0],
                                        l_face->getVertex(2)->position()[1] - l_face->getVertex(0)->position()[1],
                                        l_face->getVertex(2)->position()[2] - l_face->getVertex(0)->position()[2]);

		                Vec l_normal = v2_minus_v1 % v3_minus_v1;
		                l_face->setNormal(!l_normal);
                        
					    object->addFace(l_face);
				    }
				    break;
			    default:
				    do{
					    type=fgetc(pFile);
				    }while(type != EOF && type != '\n');
				    break;
		    }//end switch
	    }//end while
	    object->setBoundingBox(Rect(*minimum,*maximum));

        // close file
	    if(pFile)
	    {
		    if (fclose( pFile ))
		    {
			    cout<<"mvFileLoader:Fail to close file"<<endl;	
		    }
	    }
    
    }   
 
public:
    MLoader(){}
    virtual ~MLoader(){}

    
};

#endif
