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

void Bih::setLeafNode(BihNode* currentNode, Face* primitive, const int& primitiveCount)
{
    //1. set 'isLeaf' flag
    currentNode->m_isLeaf = true;

	//2. set pointer to polygon
    currentNode->m_primitive = primitive;

	//3. set number of polygons in leaf
    currentNode->m_axisOrPrimitiveCount = new int();
    *(currentNode->m_axisOrPrimitiveCount) = primitiveCount;
}

void Bih::buildBIHRecur(BihNode* currentNode, const int& minIndex, const int& maxIndex, const Vec& min, const Vec& max)
{
    // Determine if leaf or inner node
    //A. Leaf Node case: 
    if(minIndex==maxIndex)
	{
        setLeafNode(currentNode, (*m_primitives)[minIndex], 1);
	}
    //B. Inner Node case:
    else
    {
        //  1. Determine split axis (mid-point of longest side in current AABB)
        //      Handle exception case - split position equals min or max
        //                              2 or more polygons are so close that float cannot differentiate them
        //                              convert these cases to leaf  
        float x_diff = max[0] - min[0];
		float y_diff = max[1] - min[1];
		float z_diff = max[2] - min[2];

        float split_pos;

        if( (x_diff >= y_diff) && (x_diff >= z_diff) )
		{
            split_pos = min[0] + (x_diff/2.0f);
            
            //check exceptional case
            if((split_pos == min[0])||(split_pos == max[0]))
			{
                setLeafNode(currentNode, (*m_primitives)[minIndex], maxIndex-minIndex+1);
                return;
            }
            //norm case
            else
            {
                //1. set axis
                currentNode->m_axisOrPrimitiveCount = new int();
                *(currentNode->m_axisOrPrimitiveCount) = 0;
            }
        }
        else if( (y_diff > x_diff) && ( y_diff >= z_diff) )
        {
            split_pos = min[1] + (y_diff/2.0f);
            
            //check exceptional case
            if((split_pos == min[1])||(split_pos == max[1]))
			{
                setLeafNode(currentNode, (*m_primitives)[minIndex], maxIndex-minIndex+1);
                return;
            }
            //norm case
            else
            {
                //1. set axis
                currentNode->m_axisOrPrimitiveCount = new int();
                *(currentNode->m_axisOrPrimitiveCount) = 1;
            }
        }
        else
		{
            split_pos = min[2] + (z_diff/2.0f);
            
            //check exceptional case
            if((split_pos == min[2])||(split_pos == max[2]))
			{
                setLeafNode(currentNode, (*m_primitives)[minIndex], maxIndex-minIndex+1);
                return;
            }
            //norm case
            else
            {
                //1. set axis
                currentNode->m_axisOrPrimitiveCount = new int();
                *(currentNode->m_axisOrPrimitiveCount) = 2;
            }
        }

        //  2. Sort list of polygons from minIndex to maxIndex
		//      to either left or right child.
		//      In-place sorting of polygons
        int l_count=0;
        int r_count=0;

        float split_max = split_pos;
	    float split_min = split_pos;

        vector<Face*>::iterator curPolyIter;
		curPolyIter = m_primitives->begin();
		curPolyIter = curPolyIter + minIndex;

        vector<Face*>::iterator swapRPolyIter;
		swapRPolyIter=m_primitives->end();
		swapRPolyIter--; //to point to last element in vector
		swapRPolyIter = swapRPolyIter -((m_primitives->size()-1)-maxIndex);

        while( (l_count+r_count) != (maxIndex-minIndex+1) )
		{
            int l_decision=0; // 0 - Left, 1 - Right
            float max_diff;
            float min_diff;

            switch(*(currentNode->m_axisOrPrimitiveCount))
	        {
                case 0:
                    if((*curPolyIter)->boundingBox().max()[0] <= split_pos)
                        l_decision = 0;
                    else if((*curPolyIter)->boundingBox().min()[0] >= split_pos)
                        l_decision = 1;
                    else
                    {
                        max_diff = (*curPolyIter)->boundingBox().max()[0] - split_pos;
						min_diff = split_pos - (*curPolyIter)->boundingBox().min()[0];
                        if(min_diff >= max_diff)
						{
                            l_decision = 0;
                            
                            // Set as split_max if > existing split_max
							if((*curPolyIter)->boundingBox().max()[0] > split_max)
							{
								split_max = (*curPolyIter)->boundingBox().max()[0];
							}
                        }
                        else
                            l_decision = 1;
        
                            // Set as split_min if < existing split_min
							if((*curPolyIter)->boundingBox().min()[0] < split_min)
							{
								split_min = (*curPolyIter)->boundingBox().min()[0];
							}
                    }
                    break;
                case 1:
                    if((*curPolyIter)->boundingBox().max()[1] <= split_pos)
                        l_decision = 0;
                    else if((*curPolyIter)->boundingBox().min()[1] >= split_pos)
                        l_decision = 1;
                    else
                    {
                        max_diff = (*curPolyIter)->boundingBox().max()[1] - split_pos;
						min_diff = split_pos - (*curPolyIter)->boundingBox().min()[1];
                        if(min_diff >= max_diff)
						{
                            l_decision = 0;

                            // Set as split_max if > existing split_max
							if((*curPolyIter)->boundingBox().max()[1] > split_max)
							{
								split_max = (*curPolyIter)->boundingBox().max()[1];
							}
                        }
                        else
                            l_decision = 1;

                            // Set as split_min if < existing split_min
							if((*curPolyIter)->boundingBox().min()[1] < split_min)
							{
								split_min = (*curPolyIter)->boundingBox().min()[1];
							}
                    }
                    break;
                case 2:
                    if((*curPolyIter)->boundingBox().max()[2] <= split_pos)
                        l_decision = 0;
                    else if((*curPolyIter)->boundingBox().min()[2] >= split_pos)
                        l_decision = 1;
                    else
                    {
                        max_diff = (*curPolyIter)->boundingBox().max()[2] - split_pos;
						min_diff = split_pos - (*curPolyIter)->boundingBox().min()[2];
                        if(min_diff >= max_diff)
						{
                            l_decision = 0;
                            // Set as split_max if > existing split_max
							if((*curPolyIter)->boundingBox().max()[2] > split_max)
							{
								split_max = (*curPolyIter)->boundingBox().max()[2];
							}
                        }
                        else
                            l_decision = 1;
                            // Set as split_min if < existing split_min
							if((*curPolyIter)->boundingBox().min()[2] < split_min)
							{
								split_min = (*curPolyIter)->boundingBox().min()[2];
							}
                    }
                    break;
            }

            if(l_decision == 0)
            {
                //Increment l_count
				l_count++;

				//Increment current poly iter. Access Next polygon.
				if(l_count < (maxIndex-minIndex+1))
					curPolyIter++;
            }
            else
            {
                //Retrieve RHS polygon to be swapped over
				Face* temp = (*swapRPolyIter);

				//Perform swap
				(*swapRPolyIter)=(*curPolyIter);
				*curPolyIter=temp;

				//Increment r_count;
				r_count++;

				//Decrement swap iter;
				if(r_count < (maxIndex-minIndex+1) )
					swapRPolyIter--;
            }
        }
        //	3. Set currentNode details
        //      a. Set m_leftValue and m_rightValue
        currentNode->m_leftValue = new float();
        (*currentNode->m_leftValue) = split_max;

        currentNode->m_rightValue = new float();
        (*currentNode->m_rightValue) = split_min;

        //      b. Create and Set pointer to children pair - m_leftChild and m_rightChild
        //         Calculate new bounding boxes and make recursive call
        Vec minNew;
        Vec maxNew;

        if(l_count>0)
        {
            currentNode->m_leftChild = new BihNode();
            
            switch(*(currentNode->m_axisOrPrimitiveCount))
            {
            case 0: //  X - axis
                minNew = Vec(min[0], min[1], min[2]);
				maxNew = Vec(split_pos, max[1], max[2]);
				buildBIHRecur(currentNode->m_leftChild,minIndex,minIndex+(l_count-1), minNew, maxNew);
                break;
            case 1: // Y - axis
                minNew = Vec(min[0], min[1], min[2]);
				maxNew = Vec(max[0], split_pos, max[2]);
				buildBIHRecur(currentNode->m_leftChild,minIndex,minIndex+(l_count-1), minNew, maxNew);
                break;
            case 2: // Z - axis
                minNew = Vec(min[0], min[1], min[2]);
				maxNew = Vec(max[0], max[1], split_pos);
				buildBIHRecur(currentNode->m_leftChild,minIndex,minIndex+(l_count-1), minNew, maxNew);
                break;
            }
        }
        if(r_count>0)
        {
            currentNode->m_rightChild = new BihNode();
        
            switch(*(currentNode->m_axisOrPrimitiveCount))
            {
            case 0: //  X - axis
                minNew = Vec(split_pos, min[1], min[2]);
				maxNew = Vec(max[0], max[1], max[2]);
				buildBIHRecur(currentNode->m_rightChild,minIndex+l_count,maxIndex, minNew, maxNew);
                break;
            case 1: // Y - axis
                minNew = Vec(min[0], split_pos, min[2]);
				maxNew = Vec(max[0], max[1], max[2]);
				buildBIHRecur(currentNode->m_rightChild,minIndex+l_count,maxIndex, minNew, maxNew);
                break;
            case 2: // Z - axis
                minNew = Vec(min[0], min[1], split_pos);
				maxNew = Vec(max[0], max[1], max[2]);
				buildBIHRecur(currentNode->m_rightChild,minIndex+l_count,maxIndex, minNew, maxNew);
                break;
            }
        }
    } //end else inner node case
}

void toConsole()
{

}
