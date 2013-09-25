// Tree.h: interface for the CTree class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__9D3A97DF_1F41_44E6_AE48_AF0C6F1D6371__INCLUDED_)
#define AFX_TREE_H__9D3A97DF_1F41_44E6_AE48_AF0C6F1D6371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <gl/gl.h>

class CTree  
{
public:
	CTree();
	virtual ~CTree(){}
	void draw();
	void setPosition(GLfloat x, GLfloat z){ this->x=x; this->z=z;}
	void getPosition(GLfloat& x, GLfloat& z){ x=this->x; z=this->z;}
private:
	GLfloat x,z;	// (x,z) position of the tree on the ground plane
	// geometry parameters
	int nSlices;
	int nStacks;
	int nLevels;
	GLfloat trunkRadius;
	GLfloat trunkHeight;
	GLfloat treeRadius;
	// material properties
	GLfloat no_shininess[1];
	GLfloat mat_specular[4];
	GLfloat mat_trunk[4];
	GLfloat mat_needles[4];
};

#endif // !defined(AFX_TREE_H__9D3A97DF_1F41_44E6_AE48_AF0C6F1D6371__INCLUDED_)
