// Monument.h: interface for the CMonument class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONUMENT_H__CFA6100C_622B_478B_B2F1_E5DDF89FE34D__INCLUDED_)
#define AFX_MONUMENT_H__CFA6100C_622B_478B_B2F1_E5DDF89FE34D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>

const int NUM_PILLARS=5;
const double HEIGHT_PILLAR=1.5;
const double RADIUS_PROFILE_CURVE_PILLAR=0.1;
const double RADIUS_PILLAR=0.2;
const int NUM_HORIZONTAL_VERTICES=16;
const int NUM_CIRCUMFERENTIAL_VERTICES=16;

class CMonument  
{
public:
	CMonument();
	virtual ~CMonument(){}
	void draw();
	void setTexture(GLuint i){ texName=i;}
private:
	float vertices[NUM_HORIZONTAL_VERTICES][NUM_CIRCUMFERENTIAL_VERTICES][3];
	float normals[NUM_HORIZONTAL_VERTICES][NUM_CIRCUMFERENTIAL_VERTICES][3];
	// texture parameters
	float texture_coordinates[NUM_HORIZONTAL_VERTICES][NUM_CIRCUMFERENTIAL_VERTICES][2];
	GLuint texName;
	GLubyte *texture;
	int textureWidth, textureHeight;
	// material properties
	GLfloat mat_shininess[1];
	GLfloat mat_specular[4];
	GLfloat mat_ambient[4];
	GLfloat mat_diffuse[4];
};

#endif // !defined(AFX_MONUMENT_H__CFA6100C_622B_478B_B2F1_E5DDF89FE34D__INCLUDED_)
