#include "vec.h"
#include <math.h>

Vec::Vec()
{
    //m_val = new float[3];
    /*m_val[0] = 0;
    m_val[1] = 0;
    m_val[2] = 0;*/
	
	//SIMD
	/*data.fVals[0]=0;
	data.fVals[1]=0;
	data.fVals[2]=0;
	data.fVals[3]=0;*/
	data.myVec = vec_splats(0.0f);
}

Vec::Vec(const float& splatValue)
{
	data.myVec = vec_splats(splatValue);
}

Vec::Vec(const float& xx, const float& yy, const float& zz)
{
    //m_val = new float[3];
    
	/*m_val[0] = xx;
    m_val[1] = yy;
    m_val[2] = zz;*/
	
	//SIMD
    data.fVals[0]=xx;
    data.fVals[1]=yy;
    data.fVals[2]=zz;
    data.fVals[3]=0;
}

Vec::Vec(const float& xx, const float& yy, const float& zz, const float& aa)
{
    //m_val = new float[3];
    
	/*m_val[0] = xx;
    m_val[1] = yy;
    m_val[2] = zz;*/
	
	//SIMD
    data.fVals[0]=xx;
    data.fVals[1]=yy;
    data.fVals[2]=zz;
    data.fVals[3]=aa;
}

Vec::Vec(const Vec& another)
{
    //m_val = new float[3];
    /*m_val[0] = another.m_val[0];
    m_val[1] = another.m_val[1];
    m_val[2] = another.m_val[2];*/
	
	//SIMD
	//data.fVals[0]=another.data.fVals[0];
    //data.fVals[1]=another.data.fVals[1];
    //data.fVals[2]=another.data.fVals[2];
    //data.fVals[3]=another.data.fVals[3];
	data.myVec = another.data.myVec;
}

Vec::Vec(const vec3D& anotherData) {
/*#ifdef __SPU__
	spu_splat(data.myVec,0);
	data.myVec = spu_add(data.myVec,anotherData.myVec);
#else*/
	//vec_splat(data.myVec,0);
	//data.myVec = vec_add(data.myVec,anotherData.myVec);
//#endif
	data.myVec = anotherData.myVec;
}

Vec::Vec(const vector float& anotherVec) {
	data.myVec = anotherVec;
}

Vec::~Vec()
{
    //if(m_val)
    //    delete[] m_val;
}

const float& Vec::x()
{
    //return m_val[0];
	return data.fVals[0];
}

const float& Vec::y()
{
    //return m_val[1];
    return data.fVals[1];
}

const float& Vec::z()
{
    //return m_val[2];
    return data.fVals[2];
}

float Vec::mag(void) const
{
    //return sqrt(m_val[0]*m_val[0] + m_val[1]*m_val[1] + m_val[2]*m_val[2]);
	//return sqrt(data.fVals[0]*data.fVals[0] + data.fVals[1]*data.fVals[1] + data.fVals[2]*data.fVals[2]);
	return (*this)*(*this);
}
/*
Vec& Vec::operator = (const Vec& v)
{
    //m_val[0] = another.m_val[0];
    //m_val[1] = another.m_val[1];
    //m_val[2] = another.m_val[2];

    //return (*this);
	
	data.fVals[0] = v.data.fVals[0];
    data.fVals[1] = v.data.fVals[1];
    data.fVals[2] = v.data.fVals[2];
    //data.fVals[3] = v.data.fVals[3];

    return (*this);
}*/

float& Vec::operator [] (const int& index)
{
    //return m_val[index];
	return data.fVals[index];
}

const float& Vec::operator[] (const int& index) const
{
    //return m_val[index];
	return data.fVals[index];
}

Vec Vec::operator - (void) const
{
    //return Vec(-m_val[0],-m_val[1],-m_val[2]);
	return Vec(-data.fVals[0],-data.fVals[1],-data.fVals[2]);
	
}
/*
Vec Vec::operator ! (void) const
{
    //float li = 1/(mag());
    //return Vec( m_val[0]*li, m_val[1]*li, m_val[2]*li );
	float li = 1/(mag());
	return Vec( data.fVals[0]*li, data.fVals[1]*li, data.fVals[2]*li );
}*/
/*
Vec Vec::operator + (const Vec& v) const
{
    //return Vec( m_val[0]+v.m_val[0], m_val[1]+v.m_val[1], m_val[2]+v.m_val[2] );
    return Vec( data.fVals[0]+v.data.fVals[0], data.fVals[1]+v.data.fVals[1], data.fVals[2]+v.data.fVals[2] );
}
*/
/*
Vec Vec::operator - (const Vec& v) const
{
    //return Vec( m_val[0]-v.m_val[0], m_val[1]-v.m_val[1], m_val[2]-v.m_val[2] );
    return Vec( data.fVals[0]-v.data.fVals[0], data.fVals[1]-v.data.fVals[1], data.fVals[2]-v.data.fVals[2] );
}*/
/*
Vec Vec::operator * (const float& num) const
{
    //return Vec( m_val[0]*num, m_val[1]*num, m_val[2]*num );
	return Vec( data.fVals[0]*num, data.fVals[1]*num, data.fVals[2]*num );
}*/
/*
float Vec::operator * (const Vec& v) const
{
    //return ((m_val[0]*v.m_val[0])+(m_val[1]*v.m_val[1])+(m_val[2]*v.m_val[2]));
	return ((data.fVals[0]*v.data.fVals[0])+(data.fVals[1]*v.data.fVals[1])+(data.fVals[2]*v.data.fVals[2]));
}*/
/*
Vec Vec::operator % (const Vec& v) const
{
    //return Vec( m_val[1]*v.m_val[2]-m_val[2]*v.m_val[1], m_val[2]*v.m_val[0]-m_val[0]*v.m_val[2], m_val[0]*v.m_val[1]-m_val[1]*v.m_val[0] );
	return Vec( data.fVals[1]*v.data.fVals[2]-data.fVals[2]*v.data.fVals[1], data.fVals[2]*v.data.fVals[0]-data.fVals[0]*v.data.fVals[2], data.fVals[0]*v.data.fVals[1]-data.fVals[1]*v.data.fVals[0] );
}*/
/*
Vec Vec::operator & (const Vec& v) const
{
    //return Vec(m_val[0]*v.m_val[0],m_val[1]*v.m_val[1],m_val[2]*v.m_val[2]);
	return Vec(data.fVals[0]*v.data.fVals[0],data.fVals[1]*v.data.fVals[1],data.fVals[2]*v.data.fVals[2]);
}
*/

