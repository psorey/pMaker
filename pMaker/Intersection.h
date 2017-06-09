#pragma once

class SbVec2f;
class SoMFVec2f;
class SbLine;

class Intersection {

	#define SMALL_NUM   0.00000001 // anything that avoids division overflow
		// dot product (3D) which allows vector operations in arguments
//	#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
//	#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product  (2D)
public:

static	int intersect2D_2Segments(SbVec2f * S1, SbVec2f * S2, SbVec2f &  I0, SbVec2f & I1);
static	int inSegment(SbVec2f P, SbVec2f * S);

static	float _dot(SbVec2f vA, SbVec2f vB);
static	float _perp(SbVec2f vA, SbVec2f vB);   // 0 = perpendicular
};


