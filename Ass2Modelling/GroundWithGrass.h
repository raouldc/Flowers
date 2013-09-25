// GroundWithGrass.h: interface for the CGroundWithGrass class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUNDWITHGRASS_H__CFE7798E_FFB3_4EEB_875C_34E9608F55C3__INCLUDED_)
#define AFX_GROUNDWITHGRASS_H__CFE7798E_FFB3_4EEB_875C_34E9608F55C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <gl/gl.h>
#include "Geometry.h"

#define NUM_GRASS_BLADES 20000

class CGroundWithGrass  
{
public:
	CGroundWithGrass();
	virtual ~CGroundWithGrass();
	void draw();
	const CPlane3df& getPlane(){ return plane;} 
	void setTexture(GLuint i){ texName=i;}
private:
	void init();
	CPlane3df plane;
	CVec3df p0,p1,p2,p3;	// vertices of ground
	CVec2df t0,t1,t2,t3;	// texture coordinates of ground 
	float groundY; 
	float groundX1, groundX2;
	float groundZ1, groundZ2;
	float grassBlade3DPositions[NUM_GRASS_BLADES][2][3];
	// texture parameters
	float grassBlade3DTextureCoordinates[NUM_GRASS_BLADES][2][2];
	GLuint texName;
	GLubyte *texture;
	int textureWidth, textureHeight;
};

#endif // !defined(AFX_GROUNDWITHGRASS_H__CFE7798E_FFB3_4EEB_875C_34E9608F55C3__INCLUDED_)
