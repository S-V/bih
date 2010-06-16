#define __SPU__ 1

#include <stdio.h>
#include <spu_mfcio.h>
//#include <simdmath/rsqrtf4.h>
#include <raytracerControl.h>
#include <bihnodeannex.h>
#include <faceannex.h>
#include <sceneannex.h>
#include "vec.h"
#include "ray.h"

control_block_ray_tracer_t control_block __attribute__ ((aligned (64)));

static bool intersectRayAABB(const Vec& origin, Ray& ray, const Vec& min, const Vec& max) {
	float t_near = -10000000000.0f;
	float t_far = 10000000000.0f;

	// X - PLANE
	//Optimizing for SPU
	Vec minMinusOrigin = min - origin;
	Vec maxMinusOrigin = max - origin;

	//1. if ray direction is parallel to AABB
	if (ray.direction()[0] == 0.0f) {
		//if ray origin is not between min.x and max.x, return false
		if ((origin[0] < min[0]) || (origin[0] > max[0])) {
			return false;
		}
	} else {
		//compute intersection distance of planes
		//Optimizing for SPU
		//float T1 = ((min[0] - (origin)[0]))/ ((ray.direction())[0]);
		//float T2 = ((max[0] - (origin)[0]))/ ((ray.direction())[0]);

		float T1 = (minMinusOrigin[0])/ ((ray.direction())[0]);
		float T2 = (maxMinusOrigin[0])/ ((ray.direction())[0]);

		//swap if T1>T2 since T1 intersection with near plane
		if (T1>T2) {
			float T_swap = T2;
			T2=T1;
			T1=T_swap;
		}

		//If T1 > Tnear set Tnear =T1 /* want largest Tnear */
		if (T1 > (t_near))
			t_near = T1;
		//If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
		if (T2 < (t_far))
			t_far = T2;
		//If Tnear > Tfar box is missed so return false
		if ((t_near) > (t_far))
			return false;
		//If Tfar < 0 box is behind ray return false end
		if ((t_far) < 0.0f)
			return false;
	}

	// Y - PLANE
	//1. if ray direction is parallel to AABB
	if (ray.direction()[1] == 0.0f) {
		//if ray origin is not between min[1] and max[1], return false
		if (((origin)[1] < min[1]) || ((origin)[1] > max[1])) {
			return false;
		}
	} else {
		//computer intersection distance of planes
		//Optimizing for SPU
		//float T1 = ((min[1] - (origin)[1]))/ ((ray.direction())[1]);
		//float T2 = ((max[1] - (origin)[1]))/ ((ray.direction())[1]);

		float T1 = (minMinusOrigin[1])/ ((ray.direction())[1]);
		float T2 = (maxMinusOrigin[1])/ ((ray.direction())[1]);

		//swap if T1>T2 since T1 intersection with near plane
		if (T1>T2) {
			float T_swap = T2;
			T2=T1;
			T1=T_swap;
		}

		//If T1 > Tnear set Tnear =T1 /* want largest Tnear */
		if (T1 > (t_near))
			t_near = T1;
		//If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
		if (T2 < (t_far))
			t_far = T2;
		//If Tnear > Tfar box is missed so return false
		if ((t_near) > (t_far))
			return false;
		//If Tfar < 0 box is behind ray return false end
		if ((t_far) < 0.0f)
			return false;
	}

	// Z - PLANE
	//1. if ray direction is parallel to AABB
	if (ray.direction()[2] == 0.0f) {
		//if ray origin is not between min[2] and max[2], return false
		if (((origin)[2] < min[2]) || ((origin)[2] > max[2])) {
			return false;
		}
	} else {
		//computer intersection distance of planes
		//Optimizing for SPU
		//float T1 = ((min[2] - (origin)[2]))/ ((ray.direction())[2]);
		//float T2 = ((max[2] - (origin)[2]))/ ((ray.direction())[2]);

		float T1 = (minMinusOrigin[2])/ ((ray.direction())[2]);
		float T2 = (maxMinusOrigin[2])/ ((ray.direction())[2]);

		//swap if T1>T2 since T1 intersection with near plane
		if (T1>T2) {
			float T_swap = T2;
			T2=T1;
			T1=T_swap;
		}

		//If T1 > Tnear set Tnear =T1 /* want largest Tnear */
		if (T1 > (t_near))
			t_near = T1;
		//If T2 < Tfar set Tfar="T2" /* want smallest Tfar */
		if (T2 < (t_far))
			t_far = T2;
		//If Tnear > Tfar box is missed so return false
		if ((t_near) > (t_far))
			return false;
		//If Tfar < 0 box is behind ray return false end
		if ((t_far) < 0.0f)
			return false;
	}

	//If survive above tests, ray intersects aabb at t_near and t_far, return true;
	return true;
}

static double intersectRayTriangle(const Vec& origin, Ray& ray,
	FaceAnnex* polygon, Vec& intersectionNormal) 
{
	float plane_d = polygon->m_faceNormal * (polygon->m_vertex1);
	
	double distance=-1.0;
	double distanceReal=0.0;
	double distancePerUnit=0.0;
	if ((polygon->m_faceNormal * ray.direction()) == 0)
		return distance;
	else {
		distancePerUnit=(polygon->m_faceNormal * ray.direction());
		distanceReal = (plane_d - (polygon->m_faceNormal * origin));
		distance = distanceReal/distancePerUnit;
	}
	
	//if D<0.0, plane is behind ray, return D
	if (distance<0.0) {
		return distance;
	}
	
	//Optimizing for SPU
	//Vec ray_minus_polyv1 = ray_origin-polygon->m_vertex1;
	Vec ray_minus_polyv1 = origin - polygon->m_vertex1;
	
	//double polyNormalTimeAbove = (polygon->m_faceNormal)*ray_minus_polyv1;
	
	//double rayTimesPolyNormal = (ray.direction()) * (polygon->m_faceNormal);
	
	//Substitute D into parametric equation of ray to find point P, where ray intersects plane
	Vec intersect_pt = origin + ((ray.direction())*(float)distance);
	
	//Now we have A/B/C (vertices of polygon) and P
	//In barycentric coord, wA + uB + vC = P
	//Find dominant axis of triangle normal (largest), take away this axis value from all points in triangle
	//i.e. Project along dominant triangle axis, so only u and v barycentric coord are needed
	unsigned int dominant_normal_axis;
	
	if ((polygon->m_faceNormal[0]) >= (polygon->m_faceNormal[1])) {
		if ((polygon->m_faceNormal[0]) >= (polygon->m_faceNormal[2])) {
			dominant_normal_axis=0;
		} else {
			dominant_normal_axis=2;
		}
	} else {
		if ((polygon->m_faceNormal[1]) >= (polygon->m_faceNormal[2])) {
			dominant_normal_axis=1;
		} else {
			dominant_normal_axis=2;
		}
	}
	
	double u=0, v=0;
	double detT = 0;
	
	//was commented
	switch (dominant_normal_axis) {
	case 0:
		detT=(((polygon->m_vertex1[1]-polygon->m_vertex3[1])*(polygon->m_vertex2[2]-polygon->m_vertex3[2]))
				-((polygon->m_vertex1[2]-polygon->m_vertex3[2])*(polygon->m_vertex2[1]
						-polygon->m_vertex3[1])));
		u=(((polygon->m_vertex2[2]-polygon->m_vertex3[2])*(intersect_pt[1]-polygon->m_vertex3[1]))
				-((polygon->m_vertex2[1]-polygon->m_vertex3[1])*(intersect_pt[2]
						-polygon->m_vertex3[2])))/detT;
		v=(-1.0*((polygon->m_vertex1[2]-polygon->m_vertex3[2])*(intersect_pt[1]
				-polygon->m_vertex3[1]))+((polygon->m_vertex1[1]-polygon->m_vertex3[1])
				*(intersect_pt[2]-polygon->m_vertex3[2])))/detT;
	
		if (detT!=0)
			break;
	case 1:
		detT=(((polygon->m_vertex1[0]-polygon->m_vertex3[0])*(polygon->m_vertex2[2]-polygon->m_vertex3[2]))
				-((polygon->m_vertex1[2]-polygon->m_vertex3[2])*(polygon->m_vertex2[0]
						-polygon->m_vertex3[0])));
		u=(((polygon->m_vertex2[2]-polygon->m_vertex3[2])*(intersect_pt[0]-polygon->m_vertex3[0]))
				-((polygon->m_vertex2[0]-polygon->m_vertex3[0])*(intersect_pt[2]
						-polygon->m_vertex3[2])))/detT;
		v=(-1.0*((polygon->m_vertex1[2]-polygon->m_vertex3[2])*(intersect_pt[0]
				-polygon->m_vertex3[0]))+((polygon->m_vertex1[0]-polygon->m_vertex3[0])
				*(intersect_pt[2]-polygon->m_vertex3[2])))/detT;
	
		if (detT!=0)
			break;
	
	case 2:
		detT=(((polygon->m_vertex1[0]-polygon->m_vertex3[0])*(polygon->m_vertex2[1]-polygon->m_vertex3[1]))
				-((polygon->m_vertex1[1]-polygon->m_vertex3[1])*(polygon->m_vertex2[0]
						-polygon->m_vertex3[0])));
		u=(((polygon->m_vertex2[1]-polygon->m_vertex3[1])*(intersect_pt[0]-polygon->m_vertex3[0]))
				-((polygon->m_vertex2[0]-polygon->m_vertex3[0])*(intersect_pt[1]
						-polygon->m_vertex3[1])))/detT;
		v=(-1.0*((polygon->m_vertex1[1]-polygon->m_vertex3[1])*(intersect_pt[0]
				-polygon->m_vertex3[0]))+((polygon->m_vertex1[0]-polygon->m_vertex3[0])
				*(intersect_pt[1]-polygon->m_vertex3[1])))/detT;
	
		break;
	}

	if ((u>=0.0f) && (v>=0.0f) && (u+v<=1.0f)) {
		//w for vertex 1
		double w=1.0-u-v;
	
		//Optimizing for SPU
		/*
		 intersectionNormal = Vec((float)(u*polygon->getVertex(0)->normal()[0] + v*polygon->getVertex(1)->normal()[0] + w*polygon->getVertex(2)->normal()[0]),
		 (float)(u*polygon->getVertex(0)->normal()[1] + v*polygon->getVertex(1)->normal()[1] + w*polygon->getVertex(2)->normal()[1]),
		 (float)(u*polygon->getVertex(0)->normal()[2] + v*polygon->getVertex(1)->normal()[2] + w*polygon->getVertex(2)->normal()[2])
		 );
		 */
		intersectionNormal = (polygon->m_vertexNormal1 * u) + (polygon->m_vertexNormal2 * v) + (polygon->m_vertexNormal3 * w);
		intersectionNormal = !intersectionNormal;
		return distance;
	}
	else
		return -1.0;
}

void resetTreelet(unsigned long lastNode, bool forceUpTraverse, BihNodeAnnex* treeNodes, FaceAnnex* faces, unsigned int* tags)
{
	//send message to PPU to write new traversal nodes
						
	//wait for message from PPU to read new traversal nodes
	
	//read new traversal nodes and faces
		// Get tree nodes
		mfc_get (treeNodes, control_block.treelet_addr, sizeof(BihNodeAnnex) * control_block.treelet_size, (tags)[0], 0, 0);
		mfc_write_tag_mask (1 << tags[0]);
		mfc_read_tag_status_all ();
		
		// Get face primitives
		mfc_get (faces, control_block.faces_addr, sizeof(FaceAnnex) * control_block.faces_size, (tags)[0], 0, 0);
		mfc_write_tag_mask (1 << tags[0]);
		mfc_read_tag_status_all ();
}

int main(unsigned long long speid __attribute__ ((unused)), unsigned long long argp,
		unsigned long long envp __attribute__ ((unused)))
{
    //prof_clear();
    //prof_start();
    printf("Cell (0x%llx) begin...\n", speid);
    //prof_stop();
  
    //MFC transfer tags
    unsigned int tags[2];
    tags[0] = mfc_tag_reserve();
    tags[1] = mfc_tag_reserve();
    if ((tags[0] == MFC_TAG_INVALID) || (tags[1] == MFC_TAG_INVALID)) {
    	printf("SPU ERROR, unable to reserve tag\n");
    	return 1;
    }
    
    Ray currentRay;
	BihNodeAnnex* treeNodes = new BihNodeAnnex[control_block.treelet_size];
	FaceAnnex* faces = new FaceAnnex[control_block.faces_size];
	SceneAnnex scene;
	
	// issue DMA transfer to get the control block information from system memory
	mfc_get (&control_block, argp, sizeof (control_block_ray_tracer_t), tags[0], 0, 0);
	// wait for the control block transfer to complete
	mfc_write_tag_mask (1 << tags[0]);
	mfc_read_tag_status_all ();
	
	// Scene
	mfc_get (&scene, control_block.scene_addr, sizeof (SceneAnnex), tags[0], 0, 0);
	mfc_write_tag_mask (1 << tags[0]);
	mfc_read_tag_status_all ();
	
	// Intersection variables
	FaceAnnex nearestFace;
	Vec minIntersectNormal;
	double lineIntersectMin=-1.0;
	BihNodeAnnex* currentNode = treeNodes;
	int traversalCount=0;
	
	for(int i=control_block.rayIndexStart; i<=control_block.rayIndexEnd; i++)
	{
		// Get direction of current ray
		mfc_get (&currentRay, control_block.source_addr+(i*sizeof(Ray)), sizeof(Ray), tags[0], 0, 0);
		mfc_write_tag_mask (1 << tags[0]);
		mfc_read_tag_status_all ();
		
		// Get tree nodes
		mfc_get (treeNodes, control_block.treelet_addr, sizeof(BihNodeAnnex) * control_block.treelet_size, tags[0], 0, 0);
		mfc_write_tag_mask (1 << tags[0]);
		mfc_read_tag_status_all ();
		
		// Get face primitives
		mfc_get (faces, control_block.faces_addr, sizeof(FaceAnnex) * control_block.faces_size, tags[0], 0, 0);
		mfc_write_tag_mask (1 << tags[0]);
		mfc_read_tag_status_all ();
		
		// Traversal variables
		currentNode = treeNodes;
		lineIntersectMin=-1.0;
		traversalCount=0;
		
		// While bih tree traversal has not end
		while (currentNode!=0) {
			//Leaf
			if (currentNode->m_isLeaf) 
			{
				Vec intersectNormal;
				double intersectPt = intersectRayTriangle(scene.m_eye,
						currentRay, &faces[currentNode->m_primitiveIndex], intersectNormal);
				
				//if line intersection point >= 0.0 && < lineIntersectMin,
				if (intersectPt>=0.0) {
					//if((lineIntersectMin<=0.0)||(intersectPt<lineIntersectMin))
					if ((lineIntersectMin==-1.0)||(intersectPt<lineIntersectMin))
					{
						//replace lineIntersectMin and nearestPolygon pointer
						lineIntersectMin=intersectPt;
						nearestFace.m_vertex1 = faces[currentNode->m_primitiveIndex].m_vertex1;
						nearestFace.m_vertex1 = faces[currentNode->m_primitiveIndex].m_vertex2;
						nearestFace.m_vertex1 = faces[currentNode->m_primitiveIndex].m_vertex3;
						nearestFace.m_vertexNormal1 = faces[currentNode->m_primitiveIndex].m_vertexNormal1;
						nearestFace.m_vertexNormal2 = faces[currentNode->m_primitiveIndex].m_vertexNormal2;
						nearestFace.m_vertexNormal3 = faces[currentNode->m_primitiveIndex].m_vertexNormal3;
						nearestFace.m_faceNormal = faces[currentNode->m_primitiveIndex].m_faceNormal;
						minIntersectNormal=intersectNormal;
					} //end closest intersection
				} //end intersection with triangle
				traversalCount++;
				if(traversalCount==control_block.treelet_size)
				{
					resetTreelet(currentNode->m_originalNode, false, treeNodes, faces, &tags[0]);
					//reset traversal count
						traversalCount=0;
					//move current node pointer to front of treelet
						currentNode=treeNodes;
				}
				else
					currentNode++; //move forward current node pointer
			}//end leaf case
			//Inner
			else
			{
				if (intersectRayAABB(scene.m_eye, currentRay, (currentNode->m_min), (currentNode->m_max)))
				{
					traversalCount++;
					if(traversalCount==control_block.treelet_size)
					{
						resetTreelet(currentNode->m_originalNode, false, treeNodes, faces, &tags[0]);
						//reset traversal count
							traversalCount=0;
						//move current node pointer to front of treelet
							currentNode=treeNodes;
					}
					else
						currentNode++; //move forward current node pointer
				}
				// else terminate traversal of sub-tree, move on to next subtree
				else {
					resetTreelet(currentNode->m_originalNode, true, treeNodes, faces, &tags[0]);
					//reset traversal count
						traversalCount=0;
					//move current node pointer to front of treelet
						currentNode=treeNodes;
				}
			}
		}
		
		//set pixel color using nearest polygon if lineIntersect is +ve (intersection occured)
		if (lineIntersectMin>=0.0)
		{
			//Shading
			//l_color = shadeLocal(scene->getEye(), ray, minIntersectNormal,nearestPolygon, lineIntersectMin, scene);
		}
		
		//write to rgb image array (output)
	}
    		
    return 0;
}
