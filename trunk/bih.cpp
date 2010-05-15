#include "bih.h"

Bih::Bih()
{

}

Bih::~Bih()
{
    if(!m_tree)
        delete m_tree;
}

void Bih::buildBIH(Scene* scene)
{
    //obtain reference to scene and primitives
    m_primitives = &(scene->getPrimitives());

    //init tree root node
    m_tree = new BihNode();

    //recursive tree construction
    buildBIHRecur(m_tree, 0, m_primitives->size()-1, scene->boundingBox().min(), scene->boundingBox().max());
}

void Bih::buildBIHRecur(BihNode* currentNode, const int& minIndex, const int& maxIndex, const Vec& min, const Vec& max)
{
    // Determine if leaf or inner node
    //A. Leaf Node case: 
    if(minIndex==maxIndex)
	{
		//1. set 'isLeaf' flag
        currentNode->m_isLeaf = true;

		//2. set pointer to polygon
        currentNode->m_primitive = (*m_primitives)[minIndex];

		//3. set number of polygons in leaf
        currentNode->m_axisOrPrimitiveCount = new int();
        *(currentNode->m_axisOrPrimitiveCount) = 1;
	}
    //B. Inner Node case:
    else
    {
        //  1. Determine split axis (mid-point of longest side in current AABB)
        //      Handle exception case - split position equals min or max
        //                              2 or more polygons are so close that float cannot differentiate them
        //                              convert these cases to leaf  

        //  2. Sort list of polygons from minIndex to maxIndex
		//      to either left or right child.
		//      In-place sorting of polygons

        //	3. Set currentNode details
        //      a. Set m_leftValue and m_rightValue

        //      b. Create and Set pointer to children pair - m_leftChild and m_rightChild

        //  4. Calculate new bounding boxes and make recursive call  
    }
}

void toConsole()
{

}
