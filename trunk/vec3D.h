#ifndef _VEC3D_H_
#define _VEC3D_H_

#include <ppu_intrinsics.h>

typedef union {
	float fVals[4];
	vector float myVec;
} vec3D;

#endif /*VEC3D_H_*/

