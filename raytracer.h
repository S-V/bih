#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include "scene.h"
#include "image.h"
#include "bihnode.h"
#include "bihnodeannex.h"
#include "faceannex.h"
#include "ray.h"
#include "sceneannex.h"
#include "raytracerControl.h"
#include <exception>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <libspe2.h>
#include <pthread.h>
#include <spu_mfcio.h>

extern spe_program_handle_t bihCellSPU;

typedef struct _ppu_thread_ray_generator_data 
{
  spe_context_ptr_t spe_ray_tracer_context;        //spe context
  void* control_block_ray_tracer_ptr;              //pointer to the control block
} ppu_thread_ray_tracer_data_t;

#define SPU_THREADS 1
#define SPU_TREELET_SIZE 8

void *ppu_pthread_function(void *arg) {
	spe_context_ptr_t ctx;
	unsigned int entry= SPE_DEFAULT_ENTRY;
	ctx = *((spe_context_ptr_t *)arg);
	if (spe_context_run(ctx, &entry, 0, NULL, NULL, NULL) < 0) {
		perror("Failed running context");
		exit(1);
	}
	pthread_exit(NULL);
}

using namespace std;

class TreeRelationshipException : public exception {
	virtual const char* what() const throw() {
		return "Tree Relationship Exception";
	}
} treeEx;

class RayTracer {
public:
	static void rayTrace(const Scene* scene, Image* image, const BihNode* tree, Ray* rays) 
	{
		//int ray_counter = 0;
		int ray_count = image->width() * image->height();

		//on ray 34172 - memory error - resolved - error due to missing copy constructor in Vec class, resulting in uninitialized pointers
		/*image->setPixel(34172 / image->height(),
		 34172 % image->height(),
		 RayTracer::rayCast(&(rays[34172]), scene, tree) 
		 );*/

		//on ray 38747 - memory error - cannot access multiple faces in a single leaf node
		/*image->setPixel(38747 / image->height(),
		 38747 % image->height(),
		 RayTracer::rayCast(&(rays[38747]), scene, tree) 
		 );*/

		//for(int ray_counter=77255;ray_counter < 77256;ray_counter++) 
		for (int ray_counter=0; ray_counter < ray_count; ray_counter++) {
			image->setPixel(ray_counter % image->width(), ray_counter
					/ image->width(), RayTracer::rayCastStackless(
					&(rays[ray_counter]), scene, tree)
			//RayTracer::rayCast(&(rays[ray_counter]), scene, tree) 
			);
			//printf("Ray %d traced\n",ray_counter); //for debug
		}
		
		//prepare scene annex
		SceneAnnex sceneAnnex;
		sceneAnnex.Ka = scene->getObject(0)->material().Ka;
		sceneAnnex.Kd = scene->getObject(0)->material().Kd;
		sceneAnnex.Ks = scene->getObject(0)->material().Ks;
		sceneAnnex.m_eye = scene->getEye();
		sceneAnnex.m_lightColor = scene->light().m_color;
		sceneAnnex.m_lightPosition = scene->light().m_position;
		sceneAnnex.shineReflectOpacity = Vec(scene->getObject(0)->material().shine,
				scene->getObject(0)->material().reflect,
				scene->getObject(0)->material().opacity);
				
		//test spu thread launch
		printf("\nSPU thread test start.\n");
		int i;
		spe_context_ptr_t ctxs[SPU_THREADS];
		pthread_t threads[SPU_THREADS];
		control_block_ray_tracer_t control_blocks_ray_tracer[SPU_THREADS];
		ppu_thread_ray_tracer_data_t ppu_thread_ray_tracer_data[SPU_THREADS];
		
		//calculate number of rays assigned to each ray - assume divisible
		int l_perThreadRayCount = ray_count/SPU_THREADS;
		
		BihNodeAnnex treelets[SPU_THREADS][SPU_TREELET_SIZE];
		FaceAnnex facelets[SPU_THREADS][SPU_TREELET_SIZE];
		
		//Write initial treelet to all treelets
		
		/* Create several SPE-threads to execute ’SPU’.*/
		for (i=0; i<SPU_THREADS; i++) 
		{
			/* Create context */
			if ((ctxs[i] = spe_context_create(0, NULL)) == NULL) {
				perror("Failed creating context");
				exit(1);
			}
			/* Load program into context */
			if (spe_program_load(ctxs[i], &bihCellSPU)) {
				perror("Failed loading program");
				exit(1);
			}
			
			//prepare initial treelets and facelets using bih tree root node
			treelets[i][0].m_originalNode = 1;
			setTreelet(&(treelets[i][0]),&(facelets[i][0]),tree);
			
			//prepare control block
			control_blocks_ray_tracer[i].scene_addr = (unsigned long)&sceneAnnex;
			control_blocks_ray_tracer[i].source_addr = (unsigned long)rays;
			control_blocks_ray_tracer[i].out_addr = (unsigned long)image->data();
			control_blocks_ray_tracer[i].rayIndexStart = i*l_perThreadRayCount;
			control_blocks_ray_tracer[i].rayIndexEnd = control_blocks_ray_tracer[i].rayIndexStart + (l_perThreadRayCount-1);
			control_blocks_ray_tracer[i].treelet_addr = (unsigned long)(&(treelets[i][0]));
			control_blocks_ray_tracer[i].faces_addr = (unsigned long)(&(facelets[i][0]));
			control_blocks_ray_tracer[i].imageWidth = image->width();
			
			//Load Thread Data
			ppu_thread_ray_tracer_data[i].spe_ray_tracer_context = ctxs[i];
			ppu_thread_ray_tracer_data[i].control_block_ray_tracer_ptr = &(control_blocks_ray_tracer[i]);
			
			/* Create thread for each SPE context */
			if (pthread_create(&threads[i], NULL, &ppu_pthread_function, &ppu_thread_ray_tracer_data[i]))
			{
				perror("Failed creating thread");
				exit(1);
			}
			
			int threadsCompleted=0;
			while(threadsCompleted!=SPU_THREADS)
			{
				threadsCompleted=0;
				for(int i=0; i<SPU_THREADS; i++)
				{
					if(treelets[i][0].m_originalNode != 2)						//if thread completes, it will write a value 2
					{
						bool refreshed = setTreelet(&(treelets[i][0]), &(facelets[i][0]),tree); //check if treelet needs to be refreshed with new path or reset
						
						//notify SPU if treelet was refreshed
						if(refreshed)
						{
							spe_in_mbox_write(ctxs[i],0,1,SPE_MBOX_ANY_NONBLOCKING); //write to SPU mail inbox to notify tree refreshed
																					//PPU does not wait for mailbox operation to complete
						}
					}
					else
						threadsCompleted++;
				}
			}
		}
		/* Wait for SPU-thread to complete execution.  */
		for (i=0; i<SPU_THREADS; i++) {
			if (pthread_join(threads[i], NULL)) {
				perror("Failed pthread_join");
				exit(1);
			}
			/* Destroy context */
			if (spe_context_destroy (ctxs[i]) != 0) {
				perror("Failed destroying context");
				exit (1);
			}
		}
		printf("\nSPU thread test end.\n");

	}

	static bool intersectRayAABB(const Vec& origin, Ray* ray,
			const Vec& min, const Vec& max/*, float& t_near, float& t_far*/) {
		float t_near = FLT_MIN;
		float t_far = FLT_MAX;

		// X - PLANE
		//Optimizing for SPU
		Vec minMinusOrigin = min - origin;
		Vec maxMinusOrigin = max - origin;

		//1. if ray direction is parallel to AABB
		if (ray->direction()[0] == 0.0f) {
			//if ray origin is not between min.x and max.x, return false
			if ((origin[0] < min[0]) || (origin[0] > max[0])) {
				return false;
			}
		} else {
			//compute intersection distance of planes
			//Optimizing for SPU
			//float T1 = ((min[0] - (origin)[0]))/ ((ray->direction())[0]);
			//float T2 = ((max[0] - (origin)[0]))/ ((ray->direction())[0]);

			float T1 = (minMinusOrigin[0])/ ((ray->direction())[0]);
			float T2 = (maxMinusOrigin[0])/ ((ray->direction())[0]);

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
		if (ray->direction()[1] == 0.0f) {
			//if ray origin is not between min[1] and max[1], return false
			if (((origin)[1] < min[1]) || ((origin)[1] > max[1])) {
				return false;
			}
		} else {
			//computer intersection distance of planes
			//Optimizing for SPU
			//float T1 = ((min[1] - (origin)[1]))/ ((ray->direction())[1]);
			//float T2 = ((max[1] - (origin)[1]))/ ((ray->direction())[1]);

			float T1 = (minMinusOrigin[1])/ ((ray->direction())[1]);
			float T2 = (maxMinusOrigin[1])/ ((ray->direction())[1]);

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
		if (ray->direction()[2] == 0.0f) {
			//if ray origin is not between min[2] and max[2], return false
			if (((origin)[2] < min[2]) || ((origin)[2] > max[2])) {
				return false;
			}
		} else {
			//computer intersection distance of planes
			//Optimizing for SPU
			//float T1 = ((min[2] - (origin)[2]))/ ((ray->direction())[2]);
			//float T2 = ((max[2] - (origin)[2]))/ ((ray->direction())[2]);

			float T1 = (minMinusOrigin[2])/ ((ray->direction())[2]);
			float T2 = (maxMinusOrigin[2])/ ((ray->direction())[2]);

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

	static double intersectRayTriangle(const Vec& origin, Ray* ray,
			Face* polygon, Vec& intersectionNormal) {
		//find normal N, of triangle polygon and a point on it (plane of triangle)

		//distance D, along ray where ray intersects plane of triangle
		//Optimizing for SPU
		//Vec ray_origin;
		//ray_origin = Vec(origin[0],origin[1],origin[2]);

		//Optimizing for SPU
		//Vec polygon_v1;
		//polygon_v1 = Vec(polygon->getVertex(0)->position()[0],polygon->getVertex(0)->position()[1],polygon->getVertex(0)->position()[2]);
		//Vec polygon_v2;
		//polygon_v2 = Vec(polygon->getVertex(1)->position()[0],polygon->getVertex(1)->position()[1],polygon->getVertex(1)->position()[2]);
		//Vec polygon_v3;
		//polygon_v3 = Vec(polygon->getVertex(2)->position()[0],polygon->getVertex(2)->position()[1],polygon->getVertex(2)->position()[2]);
		Vec polygon_v1 = polygon->getVertex(0)->position();
		Vec polygon_v2 = polygon->getVertex(1)->position();
		Vec polygon_v3 = polygon->getVertex(2)->position();

		//Optimizing for SPU
		/*double plane_d = (polygon->normal()[0] * polygon->getVertex(0)->position()[0]) +
		 (polygon->normal()[1] * polygon->getVertex(0)->position()[1]) +
		 (polygon->normal()[2] * polygon->getVertex(0)->position()[2]);*/
		float plane_d = polygon->normal() * (polygon->getVertex(0)->position());

		double distance=-1.0;
		double distanceReal=0.0;
		double distancePerUnit=0.0;
		if ((polygon->normal() * ray->direction()) == 0)
			return distance;
		else {
			distancePerUnit=(polygon->normal() * ray->direction());
			distanceReal = (plane_d - (polygon->normal() * origin));
			distance = distanceReal/distancePerUnit;
		}

		//if D<0.0, plane is behind ray, return D
		if (distance<0.0) {
			return distance;
		}

		//Optimizing for SPU
		//Vec ray_minus_polyv1 = ray_origin-polygon_v1;
		Vec ray_minus_polyv1 = origin - polygon->getVertex(0)->position();

		//double polyNormalTimeAbove = (polygon->normal())*ray_minus_polyv1;

		//double rayTimesPolyNormal = (ray->direction()) * (polygon->normal());

		//Substitute D into parametric equation of ray to find point P, where ray intersects plane
		Vec intersect_pt = origin + ((ray->direction())*(float)distance);

		//Now we have A/B/C (vertices of polygon) and P
		//In barycentric coord, wA + uB + vC = P
		//Find dominant axis of triangle normal (largest), take away this axis value from all points in triangle
		//i.e. Project along dominant triangle axis, so only u and v barycentric coord are needed
		unsigned int dominant_normal_axis;

		if ((polygon->normal()[0]) >= (polygon->normal()[1])) {
			if ((polygon->normal()[0]) >= (polygon->normal()[2])) {
				dominant_normal_axis=0;
			} else {
				dominant_normal_axis=2;
			}
		} else {
			if ((polygon->normal()[1]) >= (polygon->normal()[2])) {
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
			detT=(((polygon_v1[1]-polygon_v3[1])*(polygon_v2[2]-polygon_v3[2]))
					-((polygon_v1[2]-polygon_v3[2])*(polygon_v2[1]
							-polygon_v3[1])));
			u=(((polygon_v2[2]-polygon_v3[2])*(intersect_pt[1]-polygon_v3[1]))
					-((polygon_v2[1]-polygon_v3[1])*(intersect_pt[2]
							-polygon_v3[2])))/detT;
			v=(-1.0*((polygon_v1[2]-polygon_v3[2])*(intersect_pt[1]
					-polygon_v3[1]))+((polygon_v1[1]-polygon_v3[1])
					*(intersect_pt[2]-polygon_v3[2])))/detT;

			if (detT!=0)
				break;
		case 1:
			detT=(((polygon_v1[0]-polygon_v3[0])*(polygon_v2[2]-polygon_v3[2]))
					-((polygon_v1[2]-polygon_v3[2])*(polygon_v2[0]
							-polygon_v3[0])));
			u=(((polygon_v2[2]-polygon_v3[2])*(intersect_pt[0]-polygon_v3[0]))
					-((polygon_v2[0]-polygon_v3[0])*(intersect_pt[2]
							-polygon_v3[2])))/detT;
			v=(-1.0*((polygon_v1[2]-polygon_v3[2])*(intersect_pt[0]
					-polygon_v3[0]))+((polygon_v1[0]-polygon_v3[0])
					*(intersect_pt[2]-polygon_v3[2])))/detT;

			if (detT!=0)
				break;

		case 2:
			detT=(((polygon_v1[0]-polygon_v3[0])*(polygon_v2[1]-polygon_v3[1]))
					-((polygon_v1[1]-polygon_v3[1])*(polygon_v2[0]
							-polygon_v3[0])));
			u=(((polygon_v2[1]-polygon_v3[1])*(intersect_pt[0]-polygon_v3[0]))
					-((polygon_v2[0]-polygon_v3[0])*(intersect_pt[1]
							-polygon_v3[1])))/detT;
			v=(-1.0*((polygon_v1[1]-polygon_v3[1])*(intersect_pt[0]
					-polygon_v3[0]))+((polygon_v1[0]-polygon_v3[0])
					*(intersect_pt[1]-polygon_v3[1])))/detT;

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
			intersectionNormal = (polygon->getVertex(0)->normal() * u) + (polygon->getVertex(1)->normal() * v) + (polygon->getVertex(2)->normal() * w);
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

		else
			return -1.0;
	}

	static Vec shadeLocal(const Vec& origin, Ray* ray, const Vec& normal,
			const Face* /*polygon*/, const double& parameter, const Scene* scene) {
		//point of intersection
		Vec p = origin + ((ray->direction())*(float)parameter);

		//result color components
		//Optimizing for SPU
		//double r=0,g=0,b=0;
		Vec l_rgb(0);

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
				double specularFactor = pow((double)(rVec*v), l_material.shine);
				double diffuseFactor = (l*normal);
				if (diffuseFactor<0.0) {
					//r=l_material.Ka[0] * scene->light().m_color[0];
					//g=l_material.Ka[1] * scene->light().m_color[1];
					//b=l_material.Ka[2] * scene->light().m_color[2];

					l_rgb = l_material.Ka & scene->light().m_color;
				} else {
					/*
					 Vec a(l_material.Ka[0],l_material.Kd[0],l_material.Ks[0]);
					 Vec b(l_material.Ka[1],l_material.Kd[1],l_material.Ks[1]);
					 Vec c(l_material.Ka[2],l_material.Kd[2],l_material.Ks[2]);*/

					l_rgb = (l_material.Ka & scene->light().m_color ) + (l_material.Kd * diffuseFactor
							& scene->light().m_color) + (l_material.Ks
							* specularFactor & scene->light().m_color);
					/*
					 r = (l_material.Ka[0]  * scene->light().m_color[0] + //ambient
					 l_material.Kd[0]  * diffuseFactor * scene->light().m_color[0] + //diffuse
					 l_material.Ks[0]  * specularFactor * scene->light().m_color[0]); //specular

					 g = (l_material.Ka[1]  * scene->light().m_color[1] + //ambient
					 l_material.Kd[1]  * diffuseFactor * scene->light().m_color[1] + //diffuse
					 l_material.Ks[1]  * specularFactor * scene->light().m_color[1]); //specular

					 b = (l_material.Ka[2]  * scene->light().m_color[2] + //ambient
					 l_material.Kd[2]  * diffuseFactor * scene->light().m_color[2] + //diffuse
					 l_material.Ks[2]  * specularFactor * scene->light().m_color[2]); //specular
					 */
				}
			}
		}

		//return Color((float)r,(float)g,(float)b);
		return l_rgb;
	}
	
	static void setNodeAnnexToNull(BihNodeAnnex& nodeAnnex)
	{
		nodeAnnex.m_axisOrPrimitiveCount = -1;
	}
	
	static void setNodeAnnex(BihNodeAnnex& nodeAnnex, const BihNode* node, const int& prevRelationship)
	{
		nodeAnnex.m_originalNode = (unsigned long)node;
		nodeAnnex.m_prevNodeRelation = prevRelationship;
		nodeAnnex.m_isLeaf = node->m_isLeaf;	
		nodeAnnex.m_min = *(node->m_min);
		nodeAnnex.m_max = *(node->m_max);
		nodeAnnex.m_leftValue = *(node->m_leftValue);
		nodeAnnex.m_rightValue = *(node->m_rightValue);
		nodeAnnex.m_axisOrPrimitiveCount = *(node->m_axisOrPrimitiveCount);
	}

	static void setFaceAnnex(FaceAnnex& faceAnnex, const BihNode* node)
	{
		faceAnnex.m_faceNormal = node->m_primitive->normal();
		faceAnnex.m_vertex1 = node->m_primitive->getVertex(0)->position();
		faceAnnex.m_vertex2 = node->m_primitive->getVertex(1)->position();
		faceAnnex.m_vertex3 = node->m_primitive->getVertex(2)->position();
		faceAnnex.m_vertexNormal1 = node->m_primitive->getVertex(0)->normal();
		faceAnnex.m_vertexNormal2 = node->m_primitive->getVertex(1)->normal();
		faceAnnex.m_vertexNormal3 = node->m_primitive->getVertex(2)->normal();
	}
	
	static bool setTreelet(BihNodeAnnex* nodeAnnex, FaceAnnex* faceAnnex, const BihNode* tree)
	{
		int nextNodeRelationship=-1;
		int indexToSetNullOnwards=-1;
		
		//Reset to initial tree
		if(nodeAnnex->m_originalNode == 1)
		{
			setNodeAnnex(nodeAnnex[0], tree,0); //assume tree root is definitely not a leaf node
			const BihNode* prevNode = tree;
			int prevRelationship = 0;
			
			for(int i=1; i<SPU_TREELET_SIZE; i++)
			{
				const BihNode* nextNode = getNextNodeInPath(prevNode,
						prevRelationship,
						nextNodeRelationship);
				
				if(nextNode==0)
				{
					indexToSetNullOnwards = i;
					break;
				}
				
				setNodeAnnex(nodeAnnex[i],nextNode,nextNodeRelationship);
				if(nextNode->m_isLeaf)
				{
					setFaceAnnex(faceAnnex[0],nextNode);
				}
				
				prevRelationship = nextNodeRelationship;
				prevNode = nextNode;
			}
			return true;
		}
		//Continue traversal
		else if(nodeAnnex->m_originalNode == 0)
		{	
			const BihNode* prevNode = (BihNode*)(nodeAnnex[1].m_originalNode);
			int prevRelationship = nodeAnnex[2].m_axisOrPrimitiveCount;
						
			for(int i=0; i<SPU_TREELET_SIZE; i++)
			{
				const BihNode* nextNode = getNextNodeInPath(prevNode,
						prevRelationship,
						nextNodeRelationship);
				
				if(nextNode==0)
				{
					indexToSetNullOnwards = i;
					break;
				}
				
				setNodeAnnex(nodeAnnex[i],nextNode,nextNodeRelationship);
				if(nextNode->m_isLeaf)
				{
					setFaceAnnex(faceAnnex[0],nextNode);
				}
				
				prevRelationship = nextNodeRelationship;
				prevNode = nextNode;
			}
			return true;
		}
		
		if(indexToSetNullOnwards!=-1)
		{
			for(int i=indexToSetNullOnwards; i<SPU_TREELET_SIZE; i++)
			{
				setNodeAnnexToNull(nodeAnnex[i]);
			}
			return true;
		}
		else
			return false;
	}

	/* Get the next node to move to in tree traversal
	 * 
	 * prevNodeRelation/ - 0 - parent
	 * nextNodeRelation    1 - left child
	 *                     2 - right child
	 *         
	 */
	static const BihNode* getNextNodeInPath(const BihNode* currentNode,
			const int& prevNodeRelation, int& nextNodeRelation) {
		bool returnParent=false;

		if (prevNodeRelation == 0) //parent
		{
			if (currentNode->m_leftChild != 0) {
				nextNodeRelation = 0;
				return currentNode->m_leftChild;
			} else if (currentNode->m_rightChild != 0) {
				nextNodeRelation = 0;
				return currentNode->m_rightChild;
			} else {
				returnParent=true;
			}
		} else if (prevNodeRelation == 1) //left child
		{
			if (currentNode->m_rightChild!=0) {
				nextNodeRelation = 0;
				return currentNode->m_rightChild;
			} else {
				returnParent=true;
			}
		} else if (prevNodeRelation == 2) //right child
		{
			returnParent=true;
		}

		if (returnParent) {
			if (currentNode->m_parent) {
				if (currentNode == (currentNode->m_parent->m_leftChild)) {
					nextNodeRelation = 1;
					return currentNode->m_parent;
				} else if (currentNode == (currentNode->m_parent->m_rightChild)) {
					nextNodeRelation = 2;
					return currentNode->m_parent;
				} else
					throw treeEx;
			} else
				return 0;
		}
	}

	static Vec rayCastStackless(Ray* ray, const Scene* scene,
			const BihNode* tree) {
		Vec l_color(0, 0, 0);

		// Traversal variables
		const BihNode* currentNode = tree;
		int prevNodeRelation = 0; //first traversal step is downwards, relationship to next node is parent
		int nextNodeRelation = -1;

		// Intersection variables
		Face* nearestPolygon=0;
		Vec minIntersectNormal;
		double lineIntersectMin=-1.0;
		int minPrimitiveIndex=-1; //for debug
		int minPrimitiveNodeIndex=-1; //for debug

		// While bih tree traversal has not end
		while (currentNode!=0) {
			//printf("node %d\n",currentNode->m_nodeID);

			if (currentNode->m_isLeaf) {
				//printf("Hit Leaf\n");

				// Get pointer to Face
				Face* currentPolygon = currentNode->m_primitive;

				// Loop through each face
				//for(int polyCount=0; polyCount < *(currentNode->m_axisOrPrimitiveCount); polyCount++)
				{
					//call to intersection test function, returns line intersection point
					Vec intersectNormal;
					double intersectPt = intersectRayTriangle(scene->getEye(),
							ray, currentPolygon, intersectNormal);

					//if line intersection point >= 0.0 && < lineIntersectMin,
					if (intersectPt>=0.0) {
						//if((lineIntersectMin<=0.0)||(intersectPt<lineIntersectMin))
						if ((lineIntersectMin==-1.0)||(intersectPt
								<lineIntersectMin)) {
							//replace lineIntersectMin and nearestPolygon pointer
							lineIntersectMin=intersectPt;
							nearestPolygon=currentPolygon;
							minIntersectNormal=intersectNormal;

							//for debug
							minPrimitiveIndex = currentNode->m_primitiveIndex;
							minPrimitiveNodeIndex = currentNode->m_nodeID;
						}
					}
					//currentPolygon++; //go to next polygon in leave node
				}

				currentNode = getNextNodeInPath(currentNode, prevNodeRelation,
						nextNodeRelation);
				prevNodeRelation = nextNodeRelation;
			}//end if node isLeaf
			//else if is inner node
			else {
				//printf("Hit Inner\n");

				//	- if ray intersects current aabb
				//float t_near = FLT_MIN;
				//float t_far = FLT_MAX;

				//if(currentNode->m_nodeID == 3)
				//{
				//	printf("node3: %.2f %.2f %.2f\n",currentNode->m_min->x(),currentNode->m_min->y(),currentNode->m_min->z());
				//	printf("node3: %.2f %.2f %.2f\n",currentNode->m_max->x(),currentNode->m_max->y(),currentNode->m_max->z());
				//}

				// if intersect current inner node, continue traversal of sub-tree
				if (intersectRayAABB(scene->getEye(), ray,
						*(currentNode->m_min), *(currentNode->m_max)/*,t_near,t_far*/)) {
					//printf("    Continue Sub-tree Traverse\n");
					currentNode = getNextNodeInPath(currentNode,
							prevNodeRelation, nextNodeRelation);
					prevNodeRelation = nextNodeRelation;
				}
				// else terminate traversal of sub-tree, move on to next subtree
				else {
					//printf("    Terminate Sub-tree Traverse\n");
					currentNode = getNextNodeInPath(currentNode, 2,
							nextNodeRelation); //prev node relation set as right child, force upward traversal
					prevNodeRelation = nextNodeRelation;
				}
			}//end else if inner node
		}

		//set pixel color using nearest polygon if lineIntersect is +ve (intersection occured)
		//if(lineIntersectMin>=0.0)
		if ((nearestPolygon!=0)&&(lineIntersectMin>=0.0)) {
			//Call to shading function to get color
			//ray - (*ray_iterator)
			//interpolated normal at intersection point - minIntersectNormal
			//intersection point parameter - lineIntersectMin
			//lights - scene
			//Reflection/Refraction ray generation

			l_color = shadeLocal(scene->getEye(), ray, minIntersectNormal,
					nearestPolygon, lineIntersectMin, scene);

			//printf("min primitive index: %d\n", minPrimitiveIndex);//for debug
			//printf("min primitive node index: %d\n", minPrimitiveNodeIndex);//for debug
		}

		return l_color;
	}

	static Vec rayCast(Ray* ray, const Scene* scene, const BihNode* tree) {
		Vec l_color(0, 0, 0);

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

		int minPrimitiveIndex=-1; //for debug
		int minPrimitiveNodeIndex=-1; //for debug

		while (!nodeStack.empty()) {
			// d.	Pop next element in stack (node)
			const BihNode* currentNode = nodeStack.at(0);
			//printf("node %d\n",currentNode->m_nodeID);
			/*
			 if(currentNode->m_isLeaf)
			 {
			 Face* currentPolygon = currentNode->m_primitive;
			 printf("        V1 position: %f %f %f \n",currentPolygon->getVertex(0)->position()[0],currentPolygon->getVertex(0)->position()[1],currentPolygon->getVertex(0)->position()[2]);
			 printf("        V1 normal:   %f %f %f \n\n",currentPolygon->getVertex(0)->normal()[0],currentPolygon->getVertex(0)->normal()[1],currentPolygon->getVertex(0)->normal()[2]);
			 printf("        V2 position: %f %f %f \n",currentPolygon->getVertex(1)->position()[0],currentPolygon->getVertex(1)->position()[1],currentPolygon->getVertex(1)->position()[2]);
			 printf("        V2 normal:   %f %f %f \n\n",currentPolygon->getVertex(1)->normal()[0],currentPolygon->getVertex(1)->normal()[1],currentPolygon->getVertex(1)->normal()[2]);
			 printf("        V3 position: %f %f %f \n",currentPolygon->getVertex(2)->position()[0],currentPolygon->getVertex(2)->position()[1],currentPolygon->getVertex(2)->position()[2]);
			 printf("        V3 normal:   %f %f %f \n\n",currentPolygon->getVertex(2)->normal()[0],currentPolygon->getVertex(2)->normal()[1],currentPolygon->getVertex(2)->normal()[2]);
			 }
			 */
			nodeStack.erase(nodeStack.begin());

			//  get current node AABB min/max
			currentMin = minStack.at(0);
			currentMax = maxStack.at(0);
			minStack.erase(minStack.begin());
			maxStack.erase(maxStack.begin());

			// ei.  If element is invalid node
			// Obsolete - was in un-optimized version

			if (currentNode->m_isLeaf) {
				// Get pointer to Face
				Face* currentPolygon = currentNode->m_primitive;

				// Loop through each face
				//for(int polyCount=0; polyCount < *(currentNode->m_axisOrPrimitiveCount); polyCount++)
				{
					//call to intersection test function, returns line intersection point
					Vec intersectNormal;
					double intersectPt = intersectRayTriangle(scene->getEye(),
							ray, currentPolygon, intersectNormal);

					//if line intersection point >= 0.0 && < lineIntersectMin,
					if (intersectPt>=0.0) {
						//if((lineIntersectMin<=0.0)||(intersectPt<lineIntersectMin))
						if ((lineIntersectMin==-1.0)||(intersectPt
								<lineIntersectMin)) {
							//replace lineIntersectMin and nearestPolygon pointer
							lineIntersectMin=intersectPt;
							nearestPolygon=currentPolygon;
							minIntersectNormal=intersectNormal;

							//for debug
							minPrimitiveIndex = currentNode->m_primitiveIndex;
							minPrimitiveNodeIndex = currentNode->m_nodeID;
						}
					}
					//currentPolygon++; //go to next polygon in leave node
				}

			}//end if node isLeaf
			//else if is inner node
			else {
				//	- if ray intersects current aabb
				//float t_near = FLT_MIN;
				//float t_far = FLT_MAX;

				//if(currentNode->m_nodeID == 3)
				//{
				//	printf("node3: %.2f %.2f %.2f\n",currentMin.x(),currentMin.y(),currentMin.z());
				//	printf("node3: %.2f %.2f %.2f\n",currentMax.x(),currentMax.y(),currentMax.z());
				//}

				if (intersectRayAABB(scene->getEye(), ray, currentMin,
						currentMax/*,t_near,t_far*/)) {
					//	- get split axis
					int split_axis = *(currentNode->m_axisOrPrimitiveCount);

					//	- get pointers to children
					/*unsigned int childrenAddress = ((currentNode->index)& 4294967292) >> 2;
					 mvBIHNode* childrenNode = reinterpret_cast<mvBIHNode*>(childrenAddress);*/

					//	- use split axis and L/R split values, push into call/min/max stack
					//	Left child
					//mvBIHNode* leftChild = childrenNode[0];

					if (currentNode->m_leftChild != 0) {
						nodeStack.push_back(currentNode->m_leftChild);
						//set new max value
						Vec newMax;
						switch (split_axis) {
						case 0:
							newMax = Vec(*(currentNode->m_leftValue),
									currentMax[1], currentMax[2]);
							break;
						case 1:
							newMax = Vec(currentMax[0],
									*(currentNode->m_leftValue), currentMax[2]);
							break;
						case 2:
							newMax = Vec(currentMax[0], currentMax[1],
									*(currentNode->m_leftValue));
							break;
						}
						minStack.push_back(currentMin);
						maxStack.push_back(newMax);
					}
					//	Right child
					if (currentNode->m_rightChild != 0) {
						nodeStack.push_back(currentNode->m_rightChild);

						//set new min value
						Vec newMin;
						switch (split_axis) {
						case 0:
							newMin = Vec(*(currentNode->m_rightValue),
									currentMin[1], currentMin[2]);
							break;
						case 1:
							newMin
									= Vec(currentMin[0],
											*(currentNode->m_rightValue),
											currentMin[2]);
							break;
						case 2:
							newMin = Vec(currentMin[0], currentMin[1],
									*(currentNode->m_rightValue));
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
		if ((nearestPolygon!=0)&&(lineIntersectMin>=0.0)) {
			//Call to shading function to get color
			//ray - (*ray_iterator)
			//interpolated normal at intersection point - minIntersectNormal
			//intersection point parameter - lineIntersectMin
			//lights - scene
			//Reflection/Refraction ray generation

			l_color = shadeLocal(scene->getEye(), ray, minIntersectNormal,
					nearestPolygon, lineIntersectMin, scene);

			//printf("min primitive index: %d\n", minPrimitiveIndex);//for debug
			//printf("min primitive node index: %d\n", minPrimitiveNodeIndex);//for debug
		}

		return l_color;
	}
public:
	RayTracer();
	virtual ~RayTracer();

private:

};

#endif
