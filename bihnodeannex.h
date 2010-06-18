#ifndef BIHNODEANNEX_H_
#define BIHNODEANNEX_H_

#include "vec.h"
#include "bihnode.h"

class BihNodeAnnex
{
public:
    BihNodeAnnex(){}
    virtual ~BihNodeAnnex(){}

    bool m_isLeaf;				//both
    Vec m_min;					//inner
    Vec m_max;					//inner
    float m_leftValue;			//inner
    float m_rightValue;		    //inner
    int m_axisOrPrimitiveCount; //both
    int m_prevNodeRelation;
    
    unsigned long m_originalNode;	//64 bit EA of original tree node
};

#endif /*BIHNODEANNEX_H_*/
