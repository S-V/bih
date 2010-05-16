#ifndef _BIH_H
#define _BIH_H

#include "scene.h"
#include "vec.h"
#include "face.h"
#include "bihnode.h"
#include <vector>

using namespace std;

class Bih
{
public:
    Bih();
    virtual ~Bih();

    void buildBIH(Scene* scene);
	void buildBIHRecur(BihNode* currentNode, const int& minIndex, const int& maxIndex, const Vec& min, const Vec& max);
	
	const BihNode* tree();

    void toConsole();

private:
    void setLeafNode(BihNode* currentNode, Face* primitive, const int& primitiveCount);

private:
    vector<Face*>* m_primitives; //pointer owned by class Object (if multiple objects, ownership belongs to class Scene)
    BihNode* m_tree;            
};

#endif
