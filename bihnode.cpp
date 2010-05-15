#include "bihnode.h"

BihNode::BihNode():m_leftValue(0),
    m_rightValue(0),
    m_leftChild(0),
    m_rightChild(0),
    m_axisOrPrimitiveCount(0),
    m_primitive(0)
{

}

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
