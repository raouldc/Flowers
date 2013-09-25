//Raoul D'Cunha, UPI:rdcu001 ID:1845734
// Flower.cpp: implementation of the CTree class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include "Flower.h"
#include "Utilities.h"
#include "Geometry.h"
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

CFlower::CFlower()
{
	// initialise geometry parameters
	nSlices=16;
	nStacks=16;
	angleWithXAxis=(GLfloat) randomDouble(0,90);	// in degree (!)
	stemRadius=0.02;
	stemHeight=(GLfloat) randomDouble(0.3,0.5);
	leafLength=stemHeight/2;
	leafAngle=(GLfloat) randomDouble(30,70);		// in degree (!)
	leafHeight1=stemHeight/3.5;
	leafHeight2=stemHeight/2.5;
	blossomCentreRadius=stemHeight/10;
	petalLength=4*blossomCentreRadius;
	numPetals=37;
	// initialise material parameters
	no_shininess[0]=50;
	mat_specular[0]=0.3;
	mat_specular[1]=0.3;
	mat_specular[2]=0.3;
	mat_specular[3]=1;
	mat_stem[0]=0.2f;
	mat_stem[1]=0.5f;
	mat_stem[2]=0.1f;
	mat_stem[3]=1;
	mat_blossom_centre[0]=0.7f;
	mat_blossom_centre[1]=0.65f;
	mat_blossom_centre[2]=0.15f;
	mat_blossom_centre[3]=1;
	mat_petal[0]=0.65f;
	mat_petal[1]=0.25f;
	mat_petal[2]=0.15f;
	mat_petal[3]=1;
}

void CFlower::draw()
{
	glEnable(GL_NORMALIZE);
	//Stem
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_stem);
	glPushMatrix();
	glTranslatef(x,-1,z);					// move flower onto the ground plane
	glRotatef(-90,1,0,0);
	glutSolidCone(stemRadius,stemHeight,nSlices,nStacks);	
	glPopMatrix();
	
	//Leaf
	//Leaf 1
	glPushMatrix();
	glTranslated(x+leafLength/2.5,leafHeight1-0.9,z);
	glRotated(leafAngle,0.0,0.0,1.0);
	glScalef(1.0,0.4,0.1);
	glutSolidSphere(leafLength/2,nSlices,nStacks);
	glPopMatrix();
	
	//Leaf 2
	glPushMatrix();
	glTranslated(x-leafLength/2.5,leafHeight2-0.9,z);
	glRotated(-leafAngle,0.0,0.0,1.0);
	glScalef(1.0,0.4,0.1);
	glutSolidSphere(leafLength/2,nSlices,nStacks);
	glPopMatrix();

	//Blossom
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_blossom_centre);
	glPushMatrix();
	glTranslatef(x,stemHeight-1,z);
	glScalef(blossomCentreRadius,blossomCentreRadius*0.5,blossomCentreRadius);
	glutSolidSphere(1,nSlices,nStacks);
	glPopMatrix();

	//Petals
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_petal);
	double h, theta,phi;
	for(int k =1;k<=numPetals;k++)
	{
		if (k == 1)
		{
			theta = 0;
			phi = 0;
		}
		else{
			glPushMatrix();
			h =-1.0 + ((2.0*(k-1.0))/(numPetals-1.0));
			theta = acos(h);
			phi = phi + 3.6/sqrt((double)numPetals)*(1.0/sqrt((double)1-pow(h,2.0)));
			phi= fmod((double)phi,(double)(2*M_PI));
			glTranslated(x,stemHeight-1,z);
			glRotatef(270, 1, 0, 0); 
			glRotatef(-90, 0, 1, 0); 
			glRotatef(phi/(2.0*M_PI)*360.0f, 1, 0, 0); 
			glRotatef(theta/(2.0*M_PI)*360.0f, 0, 1, 0); 
			glTranslatef(0, 0, petalLength/2); 
			glScalef(0.2, 0.5, 1); 
			glutSolidSphere(petalLength/2, nSlices, nStacks); 
			glPopMatrix();
		}
	}
	glDisable(GL_NORMALIZE);
}
