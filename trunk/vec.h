#ifndef _VEC_H
#define _VEC_H

#include <exception>
#include <stdio.h>
#include "vec3D.h"
#ifdef __SPU__
#include <spu_intrinsics.h>
#else
#include <altivec.h>
#include <ppu_intrinsics.h>
#endif
#include <simdmath/rsqrtf4.h>

using namespace std;

class Vec 
{
public:
    Vec()
    {
#ifdef __SPU__
    	data.myVec = spu_splats(0.0f);
#else
    	data.myVec = vec_splats(0.0f);
#endif
    }
    Vec(const float& splatValue)
    {
#ifdef __SPU__
    	data.myVec = spu_splats(splatValue);
#else
    	data.myVec = vec_splats(splatValue);
#endif
    }
    Vec(const float& xx, const float& yy, const float& zz)
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
    Vec(const float& xx, const float& yy, const float& zz, const float& aa)
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
    Vec(const Vec& another)
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
    Vec(const vec3D& anotherData)
    {
    /*#ifdef __SPU__
    	spu_splat(data.myVec,0);
    	data.myVec = spu_add(data.myVec,anotherData.myVec);
    #else*/
    	//vec_splat(data.myVec,0);
    	//data.myVec = vec_add(data.myVec,anotherData.myVec);
    //#endif
    	data.myVec = anotherData.myVec;
    }
    Vec(const vector float& anotherVec)
    {
    	data.myVec = anotherVec;
    }
    virtual ~Vec(){}

    const float& x()
    {
        //return m_val[0];
    	return data.fVals[0];
    }
    const float& y()
    {
        //return m_val[0];
    	return data.fVals[1];
    }
    const float& z()
    {
        //return m_val[0];
    	return data.fVals[2];
    }

    float mag(void) const // returns magnitude
    {
        //return sqrt(m_val[0]*m_val[0] + m_val[1]*m_val[1] + m_val[2]*m_val[2]);
    	//return sqrt(data.fVals[0]*data.fVals[0] + data.fVals[1]*data.fVals[1] + data.fVals[2]*data.fVals[2]);
    	return (*this)*(*this);
    }

    Vec& operator= (const Vec& v);
    float& operator[] (const int& index){return data.fVals[index];}
    const float& operator[] (const int& index) const{return data.fVals[index];}
    
    Vec operator - (void) const            // Negate
    {
        //return Vec(-m_val[0],-m_val[1],-m_val[2]);
    	return Vec(-data.fVals[0],-data.fVals[1],-data.fVals[2]);
    	
    }
    Vec operator ! (void) const;            // Normalize: returns unit vector

    Vec operator + (const Vec& v) const;    // Add to
    Vec operator - (const Vec& v) const;	// Subtract from
    Vec operator * (const float& num) const;       // Scale
    float operator * (const Vec& v) const;  // DotProd
    Vec operator % (const Vec& v) const;    // CrossProd
    
    Vec operator & (const Vec& v) const;    // Element-wise multiplication
    
private:
    //float m_val[3];
	vec3D data;
};

static __inline float _sum_across_float3(vector float v)
{
	vector float c2, c3;

#ifdef __SPU__
	vector float result;

	c2 = spu_rlqwbyte(v, 4);
	c3 = spu_rlqwbyte(v, 8);
	result = spu_add(v, c2);
	result = spu_add(result, c3);

	return (spu_extract(result, 0));
#else
	union {
		vector float fv;
		float f[4];
	} result;

	c2 = vec_splat(v, 1);
	c3 = vec_splat(v, 2);
	result.fv = vec_add(v, c2);
	result.fv = vec_add(result.fv, c3);

	return (result.f[0]);
#endif
}

inline Vec Vec::operator !(void) const
{
	vector float sum;
	vector float scale;
	vector float x2y2z2, y2z2x2, z2x2y2;
	vector unsigned char shuffle = ((vector unsigned char) {
				0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0A, 0x0B,
				0x00, 0x01, 0x02, 0x03,
				0x00, 0x00, 0x00, 0x00}
	);
#ifdef __SPU__
	x2y2z2 = spu_mul(data.myVec, data.myVec);
	y2z2x2 = spu_shuffle(x2y2z2, x2y2z2, shuffle);
	z2x2y2 = spu_shuffle(y2z2x2, y2z2x2, shuffle);
	sum = spu_add(x2y2z2, y2z2x2);
	sum = spu_add(sum, z2x2y2);
	scale = _rsqrtf4(sum);
	return Vec(spu_mul(scale, data.myVec));
#else
	vector float vzero = ((vector float) {0.0f,0.0f,0.0f,0.0f});

	x2y2z2 = vec_madd(data.myVec, data.myVec, vzero);
	y2z2x2 = vec_perm(x2y2z2, x2y2z2, shuffle);
	z2x2y2 = vec_perm(y2z2x2, y2z2x2, shuffle);
	sum = vec_add(x2y2z2, y2z2x2);
	sum = vec_add(sum, z2x2y2);
	scale = _rsqrtf4(sum);
	return Vec(vec_madd(scale, data.myVec, vzero));
#endif

}

inline Vec Vec::operator + (const Vec& v) const
{
	
#ifdef __SPU__
	return Vec(spu_add(data.myVec,v.data.myVec));
#else
	return Vec(vec_add(data.myVec,v.data.myVec));
#endif

	//Vec result;
	//result.data.myVec = vec_add(data.myVec,v.data.myVec);
	/*
	printf("origin %.3f, %.3f, %.3f\n",data.fVals[0],data.fVals[1],data.fVals[2]);
	printf("target %.3f, %.3f, %.3f\n",v.data.fVals[0],v.data.fVals[1],v.data.fVals[2]);
	printf("result %.3f, %.3f, %.3f\n",result.data.fVals[0],result.data.fVals[1],result.data.fVals[2]);
	
	if(result.data.fVals[0] != data.fVals[0] + v.data.fVals[0])
		throw exception();
	if(result.data.fVals[1] != data.fVals[1] + v.data.fVals[1])
		throw exception();
	if(result.data.fVals[2] != data.fVals[2] + v.data.fVals[2])
		throw exception();
	*/
	//return Vec(result.data.fVals[0],result.data.fVals[1], result.data.fVals[2]);
	//return Vec(vec_add(data.myVec, v.data.myVec));
}

inline Vec Vec::operator - (const Vec& v) const
{
#ifdef __SPU__
	return Vec(spu_sub(data.myVec,v.data.myVec));
#else
	return Vec(vec_sub(data.myVec,v.data.myVec));
#endif
}

inline Vec& Vec::operator = (const Vec& v)
{
	data.myVec = v.data.myVec;
	return (*this);
}

inline Vec Vec::operator * (const float& num) const
{
	vector float scaleVec = (vector float) {num, num, num};
#ifdef __SPU__
	return Vec(spu_mul(data.myVec,scaleVec));
#else
	vector float vzero = ((vector float) {0.0f,0.0f,0.0f,0.0f});
	return Vec(vec_madd(data.myVec,scaleVec,vzero));
#endif
}

inline float Vec::operator * (const Vec &v) const
{
	vector float product;
#ifdef __SPU__
	product = spu_mul(data.myVec, v.data.myVec);
	return (_sum_across_float3(product));
#else
	union {
		float f[4];
		vector float v;
	}result;
	vector float y, z;

	product = vec_madd(data.myVec, v.data.myVec, ((vector float) {0.0,0.0,0.0,0.0}));
	y = vec_sld(product, product, 4);
	z = vec_sld(product, product, 8);
	result.v = vec_add(product, y);
	result.v = vec_add(result.v, z);
	return (result.f[0]);
#endif

}


inline Vec Vec::operator % (const Vec &v) const
{
	vector float yzxw1, yzxw2;
	vector float result;
	vector unsigned char shuffle_yzxw = ((vector unsigned char) {
				0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
				0x00, 0x01, 0x02, 0x03, 0x0C, 0x0D, 0x0E, 0x0F});

#ifdef __SPU__
	yzxw1 = spu_shuffle(data.myVec, data.myVec, shuffle_yzxw);
	yzxw2 = spu_shuffle(v.data.myVec, v.data.myVec, shuffle_yzxw);
	result = spu_mul(data.myVec, yzxw2);
	result = spu_nmsub(v.data.myVec, yzxw1, result);
	result = spu_shuffle(result, result, shuffle_yzxw);
#else
	yzxw1 = vec_perm(data.myVec, data.myVec, shuffle_yzxw);
	yzxw2 = vec_perm(v.data.myVec, v.data.myVec, shuffle_yzxw);
	result = vec_madd(data.myVec, yzxw2, ((vector float) {0.0,0.0,0.0,0.0}));
	result = vec_nmsub(v.data.myVec, yzxw1, result);
	result = vec_perm(result, result, shuffle_yzxw);
#endif
	return (Vec(result));
}

inline Vec Vec::operator & (const Vec& v) const
{
#ifdef __SPU__
	return Vec(spu_mul(data.myVec,v.data.myVec));
#else
	Vec zero(0);
	return Vec(vec_madd(data.myVec, v.data.myVec,zero.data.myVec));
#endif
}

#endif

