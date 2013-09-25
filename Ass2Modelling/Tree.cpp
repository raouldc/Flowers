// Tree.cpp: implementation of the CTree class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include "Tree.h"
#include "Utilities.h"
#include <gl/glut.h>

CTree::CTree()
{
	// initialise geometry parameters
	nSlices=24;
	nStacks=1;
	nLevels=randomInt(6,8);
	trunkRadius=(GLfloat) randomDouble(0.05,0.08);
	trunkHeight=(GLfloat) randomDouble(0.7,1.3);
	treeRadius=4.5*trunkRadius;
	// initialise material parameters
	no_shininess[0]=0;
	mat_specular[0]=0;
	mat_specular[1]=0;
	mat_specular[2]=0;
	mat_specular[3]=1;
	mat_trunk[0]=0.5f;
	mat_trunk[1]=0.3f;
	mat_trunk[2]=0.1f;
	mat_trunk[3]=1;
	mat_needles[0]=0.3f;
	mat_needles[1]=0.55f;
	mat_needles[2]=0.35f;
	mat_needles[3]=1;
}

void CTree::draw()
{
	glPushMatrix();
	glTranslatef(x,-1,z);
	glRotatef(-90,1,0,0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_trunk);
	glutSolidCone(trunkRadius, trunkHeight, nSlices, nStacks);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_needles);
	glTranslatef(0, 0, trunkHeight/(GLfloat) (nLevels+2));
	for(int l=1;l<=nLevels;l++)
	{
		glTranslatef(0, 0, trunkHeight/(GLfloat) (nLevels+2));
		glutSolidCone(treeRadius*(nLevels+1-l)/(GLfloat) (nLevels+1), trunkHeight/(GLfloat) (nLevels+1), nSlices, nStacks);
	}
	glPopMatrix();
}
