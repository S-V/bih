#ifndef BIHNODEANNEX_H_
#define BIHNODEANNEX_H_

#include "vec.h"
#include "bihnode.h"

class BihNodeAnnex
{
public:
    BihNodeAnnex():m_originalNode(0),m_axisOrPrimitiveCount(0),m_prevNodeRelation(0),m_leftValue(0),m_rightValue(0),m_isLeaf(0){}
    virtual ~BihNodeAnnex(){}

    int m_isLeaf;				//both 1
    Vec m_min;					//inner 16
    Vec m_max;					//inner 16
    float m_leftValue;			//inner 4
    float m_rightValue;		    //inner 4
    int m_axisOrPrimitiveCount; //both 4
    int m_prevNodeRelation; //4
    
    unsigned long long m_originalNode;	//64 bit EA of original tree node
};

#endif /*BIHNODEANNEX_H_*/
