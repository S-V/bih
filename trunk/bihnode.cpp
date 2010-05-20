#include "bihnode.h"

BihNode::BihNode():
    m_isLeaf(false),
    m_leftValue(0),
    m_rightValue(0),
    m_leftChild(0),
    m_rightChild(0),
    m_axisOrPrimitiveCount(0),
    m_primitive(0)
{

}

/*BihNode::BihNode(const BihNode& another)
{
	m_isLeaf = another.m_isLeaf;
    m_leftValue = another.m_leftValue;
    m_rightValue = another.m_rightValue;
    m_leftChild = another.m_leftChild;    
    m_rightChild = another.m_rightChild;
    m_axisOrPrimitiveCount = another.m_axisOrPrimitiveCount;
    m_primitive = another.m_primitive;         
}*/

BihNode::~BihNode()
{
    if(!m_leftValue)
        delete m_leftValue;
    if(!m_rightValue)
        delete m_rightValue;
    if(!m_axisOrPrimitiveCount)
        delete m_axisOrPrimitiveCount;
    if(!m_leftChild)
        delete m_leftChild;
    if(!m_rightChild)
        delete m_rightChild;
}
