#ifndef _BIHNODE_H
#define _BIHNODE_H

#include "face.h"
#include "vertex.h"
#include "rect.h"
#include <vector>

using namespace std;

class BihNode
{
public:
    BihNode();
    virtual ~BihNode();
	//BihNode(const BihNode& another);

//TODO: make member variables private
    bool m_isLeaf;
    float* m_leftValue;
    float* m_rightValue;
    BihNode* m_leftChild;    
    BihNode* m_rightChild;
    int* m_axisOrPrimitiveCount;
    Face* m_primitive;          //pointer owned by class Object
};

#endif
