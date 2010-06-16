#ifndef FACEANNEX_H_
#define FACEANNEX_H_

class FaceAnnex
{
public:
	FaceAnnex(){}
    virtual ~FaceAnnex(){}

    //Vec m_min;
    //Vec m_max;
    Vec m_vertex1;
    Vec m_vertex2;
    Vec m_vertex3;
    Vec m_vertexNormal1;
    Vec m_vertexNormal2;
    Vec m_vertexNormal3;
    Vec m_faceNormal;
};

#endif /*FACEANNEX_H_*/
