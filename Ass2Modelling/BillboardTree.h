// BillboardTree.h: interface of the CBillboardTree class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BILLBOARDTREE)
#define AFX_BILLBOARDTREE

#pragma once

#include <windows.h>
#include <gl/gl.h>

class CBillboardTree
{
public:
	CBillboardTree();
	virtual ~CBillboardTree(){}
	void draw();
	static void setTexture(GLuint i){ texName=i;}
	void setPosition(GLfloat x, GLfloat z){ this->x=x; this->z=z;}
	void getPosition(GLfloat& x, GLfloat& z){ x=this->x; z=this->z;}
private:
	GLfloat x,z;	// (x,z) position of the tree on the ground plane
	// geometry parameters
	float vertices[4][3];
	GLfloat billboardSizeHalf;
	// texture parameters
	static float texture_coordinates[4][2];
	static bool staticVariablesInitialised;
	static GLuint texName;
	static GLubyte *texture;
	static int textureWidth, textureHeight;
};

#endif // !defined(AFX_BILLBOARDTREE)
