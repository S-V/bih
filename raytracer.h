#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include "scene.h"
#include "image.h"
#include "bihnode.h"
#include "ray.h"
#include "color.h"

class RayTracer
{
public:
    static void rayTrace(const Scene* scene, Image* image, const BihNode* tree, const Ray* rays)
    {
	    //int ray_counter = 0;
        int ray_count   = image->width() * image->height();
        
        for(int ray_counter=0;ray_counter<34172;ray_counter++) //on ray 34173 - memory error
		{
            image->setPixel(ray_counter / image->height(),
                            ray_counter % image->height(),
                            RayTracer::rayCast(&(rays[ray_counter]), scene, tree) 
                            );
        }
	}

    static bool intersectRayAABB(const Vec& origin, const Ray* ray, const Vec& min, const Vec& max, float& t_near, float& t_far)
    {
	    // X - PLANE
		
		    //1. if ray direction is parallel to AABB
		    if(ray->direction()[0] == 0.0f)
		    {
			    //if ray origin is not between min.x and max.x, return false
			    if((origin[0] < min[0]) || (origin[0] > max[0]) )
			    {
				    return false;
			    }
		    }
		    else
		    {
			    //compute intersection distance of planes
			    float T1 = ((min[0] - (origin)[0]))/ ((ray->direction())[0]);
			    float T2 = ((max[0] - (origin)[0]))/ ((ray->direction())[0]);

			    //swap if T1>T2 since T1 intersection with near plane
			    if(T1>T2)
			    {
				    float T_swap = T2;
				    T2=T1;
				    T1=T_swap;
			    }

			    //If T1 > Tnear set Tnear =T1 /* want largest Tnear */
			    if(T1 > (t_near))
				    t_near = T1;
			    //If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
			    if(T2 < (t_far))
				    t_far = T2;
			    //If Tnear > Tfar box is missed so return false
			    if((t_near) > (t_far))
				    return false;
			    //If Tfar < 0 box is behind ray return false end
			    if((t_far) < 0.0f)
				    return false;
		    }

	    // Y - PLANE
		    //1. if ray direction is parallel to AABB
		    if(ray->direction()[1] == 0.0f)
		    {
			    //if ray origin is not between min[1] and max[1], return false
			    if(((origin)[1] < min[1]) || ((origin)[1] > max[1]) )
			    {
				    return false;
			    }
		    }
		    else
		    {
			    //computer intersection distance of planes
			    float T1 = ((min[1] - (origin)[1]))/ ((ray->direction())[1]);
			    float T2 = ((max[1] - (origin)[1]))/ ((ray->direction())[1]);

			    //swap if T1>T2 since T1 intersection with near plane
			    if(T1>T2)
			    {
				    float T_swap = T2;
				    T2=T1;
				    T1=T_swap;
			    }

			    //If T1 > Tnear set Tnear =T1 /* want largest Tnear */
			    if(T1 > (t_near))
				    t_near = T1;
			    //If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
			    if(T2 < (t_far))
				    t_far = T2;
			    //If Tnear > Tfar box is missed so return false
			    if((t_near) > (t_far))
				    return false;
			    //If Tfar < 0 box is behind ray return false end
			    if((t_far) < 0.0f)
				    return false;
		    }

	    // Z - PLANE
		    //1. if ray direction is parallel to AABB
		    if(ray->direction()[2] == 0.0f)
		    {
			    //if ray origin is not between min[2] and max[2], return false
			    if(((origin)[2] < min[2]) || ((origin)[2] > max[2]) )
			    {
				    return false;
			    }
		    }
		    else
		    {
			    //computer intersection distance of planes
			    float T1 = ((min[2] - (origin)[2]))/ ((ray->direction())[2]);
			    float T2 = ((max[2] - (origin)[2]))/ ((ray->direction())[2]);

			    //swap if T1>T2 since T1 intersection with near plane
			    if(T1>T2)
			    {
				    float T_swap = T2;
				    T2=T1;
				    T1=T_swap;
			    }

			    //If T1 > Tnear set Tnear =T1 /* want largest Tnear */
			    if(T1 > (t_near))
				    t_near = T1;
			    //If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
			    if(T2 < (t_far))
				    t_far = T2;
			    //If Tnear > Tfar box is missed so return false
			    if((t_near) > (t_far))
				    return false;
			    //If Tfar < 0 box is behind ray return false end
			    if((t_far) < 0.0f)
				    return false;
		    }

	    //If survive above tests, ray intersects aabb at t_near and t_far, return true;
	    return true;
    }

    static double intersectRayTriangle(const Vec& origin, const Ray* ray, Face* polygon, Vec& intersectionNormal)
    {
        //find normal N, of triangle polygon and a point on it (plane of triangle)
	
        //distance D, along ray where ray intersects plane of triangle
        Vec ray_origin;
        ray_origin = Vec(origin[0],origin[1],origin[2]);
 
        Vec polygon_v1;
        polygon_v1 = Vec(polygon->getVertex(0)->position()[0],polygon->getVertex(0)->position()[1],polygon->getVertex(0)->position()[2]);
        
        Vec polygon_v2;
        polygon_v2 = Vec(polygon->getVertex(1)->position()[0],polygon->getVertex(1)->position()[1],polygon->getVertex(1)->position()[2]);
        
        Vec polygon_v3;
        polygon_v3 = Vec(polygon->getVertex(2)->position()[0],polygon->getVertex(2)->position()[1],polygon->getVertex(2)->position()[2]);

        double plane_d = (polygon->normal()[0] * polygon->getVertex(0)->position()[0]) +
			            (polygon->normal()[1] * polygon->getVertex(0)->position()[1]) +
			            (polygon->normal()[2] * polygon->getVertex(0)->position()[2]);
        double distance=-1.0;
        double distanceReal=0.0;
        double distancePerUnit=0.0;
        if((polygon->normal() * ray->direction()) == 0)
            return distance;
        else
        {
            distancePerUnit=(polygon->normal() * ray->direction());
            distanceReal   = (plane_d - (polygon->normal() * origin));
            distance = distanceReal/distancePerUnit;
        }

        //if D<0.0, plane is behind ray, return D
        if(distance<0.0)
            {return distance;} 

        Vec ray_minus_polyv1 = ray_origin-polygon_v1;
        double polyNormalTimeAbove = (polygon->normal())*ray_minus_polyv1;

        double rayTimesPolyNormal = (ray->direction()) * (polygon->normal());

        //Substitute D into parametric equation of ray to find point P, where ray intersects plane
        Vec intersect_pt = ray_origin + ((ray->direction())*(float)distance);

        //Now we have A/B/C (vertices of polygon) and P
        //In barycentric coord, wA + uB + vC = P
        //Find dominant axis of triangle normal (largest), take away this axis value from all points in triangle
        //i.e. Project along dominant triangle axis, so only u and v barycentric coord are needed
        unsigned int dominant_normal_axis;

        if((polygon->normal()[0]) >= (polygon->normal()[1]))
        {
            if((polygon->normal()[0]) >= (polygon->normal()[2]))
            {
	            dominant_normal_axis=0;
            }
            else
            {
	            dominant_normal_axis=2;
            }
        }
        else
        {
            if((polygon->normal()[1]) >= (polygon->normal()[2]))
            {
	            dominant_normal_axis=1;
            }
            else
            {
	            dominant_normal_axis=2;
            }
        }

        double u=0,v=0;
        double detT = 0;

        //was commented
        switch(dominant_normal_axis)
        {
        case 0:
            detT=(((polygon_v1[1]-polygon_v3[1])*(polygon_v2[2]-polygon_v3[2]))-((polygon_v1[2]-polygon_v3[2])*(polygon_v2[1]-polygon_v3[1])));
            u=(((polygon_v2[2]-polygon_v3[2])*(intersect_pt[1]-polygon_v3[1]))-((polygon_v2[1]-polygon_v3[1])*(intersect_pt[2]-polygon_v3[2])))/detT;
            v=(-1.0*((polygon_v1[2]-polygon_v3[2])*(intersect_pt[1]-polygon_v3[1]))+((polygon_v1[1]-polygon_v3[1])*(intersect_pt[2]-polygon_v3[2])))/detT;

            if(detT!=0)
	            break;
        case 1:
            detT=(((polygon_v1[0]-polygon_v3[0])*(polygon_v2[2]-polygon_v3[2]))-((polygon_v1[2]-polygon_v3[2])*(polygon_v2[0]-polygon_v3[0])));
            u=(((polygon_v2[2]-polygon_v3[2])*(intersect_pt[0]-polygon_v3[0]))-((polygon_v2[0]-polygon_v3[0])*(intersect_pt[2]-polygon_v3[2])))/detT;
            v=(-1.0*((polygon_v1[2]-polygon_v3[2])*(intersect_pt[0]-polygon_v3[0]))+((polygon_v1[0]-polygon_v3[0])*(intersect_pt[2]-polygon_v3[2])))/detT;

            if(detT!=0)
	            break;

        case 2:
            detT=(((polygon_v1[0]-polygon_v3[0])*(polygon_v2[1]-polygon_v3[1]))-((polygon_v1[1]-polygon_v3[1])*(polygon_v2[0]-polygon_v3[0])));
            u=(((polygon_v2[1]-polygon_v3[1])*(intersect_pt[0]-polygon_v3[0]))-((polygon_v2[0]-polygon_v3[0])*(intersect_pt[1]-polygon_v3[1])))/detT;
            v=(-1.0*((polygon_v1[1]-polygon_v3[1])*(intersect_pt[0]-polygon_v3[0]))+((polygon_v1[0]-polygon_v3[0])*(intersect_pt[1]-polygon_v3[1])))/detT;

            break;
        }

        if((u>=0.0f) && (v>=0.0f) && (u+v<=1.0f))
        {
            //w for vertex 1
            double w=1.0-u-v;
            intersectionNormal = Vec((float)(u*polygon->getVertex(0)->normal()[0] + v*polygon->getVertex(1)->normal()[0] + w*polygon->getVertex(2)->normal()[0]),
						            (float)(u*polygon->getVertex(0)->normal()[1] + v*polygon->getVertex(1)->normal()[1] + w*polygon->getVertex(2)->normal()[1]),
						            (float)(u*polygon->getVertex(0)->normal()[2] + v*polygon->getVertex(1)->normal()[2] + w*polygon->getVertex(2)->normal()[2])
                                );
            intersectionNormal = !intersectionNormal;
            return distance;
        }

        ////new triangle test
        //(B-A)X(Q-A)
        //Vec baqa = (polygon_v2-polygon_v1)%(intersect_pt-polygon_v1);
        //double baqaN = baqa*polygon->getNormal();
        ////(C-B)X(Q-B)
        //Vec cbqb = (polygon_v3-polygon_v2)%(intersect_pt-polygon_v2);
        //double cbqbN = cbqb*polygon->getNormal();
        ////(A-C)X(Q-C)
        //Vec acqc = (polygon_v1-polygon_v3)%(intersect_pt-polygon_v3);
        //double acqcN = acqc*polygon->getNormal();

        //if((baqaN>=0.0)&&(cbqbN>=0.0)&&(acqcN>=0.0))
        //{
        //	return distance;
        //}

        else return -1.0;
    }

    static Color shadeLocal(const Vec& origin, const Ray* ray, const Vec& normal, const Face* polygon, const double& parameter, const Scene* scene)
    {
	    //point of intersection
	    Vec p = origin + ((ray->direction())*(float)parameter);

	    //result color components
	    double r=0,g=0,b=0; 

	    //normalized vector from p to viewer (normalize opposite ray direction)
	    Vec v = (ray->direction())*-1.0;
	    v = !v;
	
	    //get material
	    const Material& l_material = scene->getObject(0)->material();

	    //for each light
	    //for(unsigned int i=0; i<lights.size(); i++)
	    {
		    //normalized vector from p to light source
		    Vec l = scene->light().m_position - p;
		    l = !l;

		    //normalized vector of reflected light at p
		    Vec rVec = (normal * 2.0 * (normal*l))-l;

		    {
			    double specularFactor = pow((double)(rVec*v),l_material.shine);
			    double diffuseFactor = (l*normal);
			    if(diffuseFactor<0.0)
			    {
				    r=l_material.Ka[0] * scene->light().m_color[0];
				    g=l_material.Ka[1] * scene->light().m_color[1];
				    b=l_material.Ka[2] * scene->light().m_color[2];
			    }
                else
                {
			        r = (l_material.Ka[0]  * scene->light().m_color[0] + //ambient
						        l_material.Kd[0]  * diffuseFactor * scene->light().m_color[0] + //diffuse
						        l_material.Ks[0]  * specularFactor * scene->light().m_color[0]); //specular

			        g = (l_material.Ka[1]  * scene->light().m_color[1] + //ambient
				        l_material.Kd[1]  * diffuseFactor * scene->light().m_color[1] + //diffuse
				        l_material.Ks[1]  * specularFactor * scene->light().m_color[1]); //specular

			        b = (l_material.Ka[2]  * scene->light().m_color[2] + //ambient
				        l_material.Kd[2]  * diffuseFactor * scene->light().m_color[2] + //diffuse
				        l_material.Ks[2]  * specularFactor * scene->light().m_color[2]); //specular
                }
		    }
	    }

	    return Color((float)r,(float)g,(float)b);
    }

	static Color rayCast(const Ray* ray, const Scene* scene, const BihNode* tree)
	{
        Color l_color(0,0,0);

        // a.	Initialize recursive call stacks
	    vector<const BihNode*> nodeStack;
	    vector<Vec> minStack;
	    vector<Vec> maxStack;

        // b.	Put 1st call into call stacks
        nodeStack.push_back(tree);
        minStack.push_back(scene->boundingBox().min());
        maxStack.push_back(scene->boundingBox().max());

        // c.	Initialize nearest polygon, line intersection point
	    //		Initialize pointer to current node,AABB min and AABB max
	    Face* nearestPolygon=0;
	    Vec minIntersectNormal;
	    double lineIntersectMin=-1.0;
        Vec currentMin;
        Vec currentMax;

        while (!nodeStack.empty())
        {
		    // d.	Pop next element in stack (node)
            const BihNode* currentNode = nodeStack.at(0);
            nodeStack.erase(nodeStack.begin());
            
            //  get current node AABB min/max
            currentMin = minStack.at(0);
            currentMax = maxStack.at(0);
            minStack.erase(minStack.begin());
            maxStack.erase(maxStack.begin());

            // ei.  If element is invalid node
            // Obsolete - was in un-optimized version

            if(currentNode->m_isLeaf)
            {
                // Get pointer to Face
                Face* currentPolygon = currentNode->m_primitive;

                // Loop through each face
                for(int polyCount=0; polyCount < *(currentNode->m_axisOrPrimitiveCount); polyCount++)
			    {
                    //call to intersection test function, returns line intersection point
                    Vec intersectNormal;
                    double intersectPt = intersectRayTriangle(scene->getEye(), ray, currentPolygon, intersectNormal);

                    //if line intersection point >= 0.0 && < lineIntersectMin,
                    if(intersectPt>=0.0)
                    {
	                    //if((lineIntersectMin<=0.0)||(intersectPt<lineIntersectMin))
	                    if((lineIntersectMin==-1.0)||(intersectPt<lineIntersectMin))
	                    {
		                    //replace lineIntersectMin and nearestPolygon pointer
		                    lineIntersectMin=intersectPt;
		                    nearestPolygon=currentPolygon;
		                    minIntersectNormal=intersectNormal;
	                    }
                    }
                    currentPolygon++; //go to next polygon in leave node
                }
                
            }//end if node isLeaf
            //else if is inner node
            else
            {
                //	- if ray intersects current aabb
                float t_near = FLT_MIN;
                float t_far = FLT_MAX;

                if(intersectRayAABB(scene->getEye(),ray,currentMin,currentMax,t_near,t_far))
                {
	                //	- get split axis
	                int split_axis = *(currentNode->m_axisOrPrimitiveCount);

	                //	- get pointers to children
	                /*unsigned int childrenAddress = ((currentNode->index)& 4294967292) >> 2;
	                mvBIHNode* childrenNode = reinterpret_cast<mvBIHNode*>(childrenAddress);*/

	                //	- use split axis and L/R split values, push into call/min/max stack
	                //	Left child
		                //mvBIHNode* leftChild = childrenNode[0];

                        if(currentNode->m_leftChild != 0)
                        {
		                    nodeStack.push_back(currentNode->m_leftChild);
		                    //set new max value
		                    Vec newMax;
		                    switch(split_axis)
		                    {
		                    case 0:
			                    newMax = Vec(*(currentNode->m_leftValue),currentMax[1],currentMax[2]);
			                    break;
		                    case 1:
			                    newMax = Vec(currentMax[0],*(currentNode->m_leftValue),currentMax[2]);
			                    break;
		                    case 2:
			                    newMax = Vec(currentMax[0],currentMax[1],*(currentNode->m_leftValue));
			                    break;
		                    }
		                    minStack.push_back(currentMin);
		                    maxStack.push_back(newMax);
                        }
	                //	Right child
                        if(currentNode->m_rightChild != 0)
                        {
		                    nodeStack.push_back(currentNode->m_rightChild);

		                    //set new min value
		                    Vec newMin;
		                    switch(split_axis)
		                    {
		                    case 0:
			                    newMin = Vec(*(currentNode->m_rightValue),currentMin[1],currentMin[2]);
			                    break;
		                    case 1:
			                    newMin = Vec(currentMin[0],*(currentNode->m_rightValue),currentMin[2]);
			                    break;
		                    case 2:
			                    newMin = Vec(currentMin[0],currentMin[1],*(currentNode->m_rightValue));
			                    break;
		                    }
		                    minStack.push_back(newMin);
		                    maxStack.push_back(currentMax);
                        }
                }
            }//end else if inner node

            
        }//end while nodeStack !empty

        //set pixel color using nearest polygon if lineIntersect is +ve (intersection occured)
	    //if(lineIntersectMin>=0.0)
	    if((nearestPolygon!=0)&&(lineIntersectMin>=0.0))
	    {
		    //Call to shading function to get color
			    //ray - (*ray_iterator)
			    //interpolated normal at intersection point - minIntersectNormal
			    //intersection point parameter - lineIntersectMin
			    //lights - scene
			    //Reflection/Refraction ray generation

		    l_color = shadeLocal(scene->getEye(),ray,minIntersectNormal,nearestPolygon,lineIntersectMin,scene);
	    }
      
        return l_color;
	}
public:
    RayTracer();
    virtual ~RayTracer();

private:
    
};

#endif
