#ifndef _BIHNODE_H
#define _BIHNODE_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include "vec.h"
#include <vector>

using namespace std;

class BihNode
{
public:
    BihNode();
    virtual ~BihNode();
	//BihNode(const BihNode& another);

    bool m_isLeaf;				//both
    Vec* m_min;					//inner
    Vec* m_max;					//inner
    float* m_leftValue;			//inner
    float* m_rightValue;		//inner
    BihNode* m_leftChild;    	//inner
    BihNode* m_rightChild;		//inner
    BihNode* m_parent;			//inner
    int* m_axisOrPrimitiveCount;//both
    Face* m_primitive;          //leaf pointer owned by class Object
    
    //for debug
    int m_primitiveIndex;
    int m_nodeID;
};

#endif
