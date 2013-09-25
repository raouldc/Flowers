// Monument.cpp: implementation of the CMonument class.
// Copyright: (c) Burkhard Wuensche
//Raoul D'Cunha UPI:rdcu001, ID:1845734
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "Monument.h"
#include "Geometry.h"
#include "Utilities.h"
#include <gl/gl.h>
#include <gl/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

CMonument::CMonument()
{
	// initialise material properties
	mat_shininess[0]=0;
	mat_specular[0]=0;
	mat_specular[1]=0;
	mat_specular[2]=0;
	mat_specular[3]=0;
	mat_ambient[0]=0.6f;
	mat_ambient[1]=0.6f;
	mat_ambient[2]=0.6f;
	mat_ambient[3]=1;
	mat_diffuse[0]=1;
	mat_diffuse[1]=1;
	mat_diffuse[2]=1;
	mat_diffuse[3]=1;
	// initialise geometry
	// Definition of a pilllar
	// =============================================================================
	// = Define the surface of revoluting forming a pillar. Compute                =
	// = NUM_HORIZONTAL_VERTICES x NUM_CIRCUMFERENTIAL_VERTICES sample points      =
	// = on the surface and store them and the corresponding normals in the arrays =
	// = "vertices" and "normals" defined in the constructor.                      =
	// =============================================================================
	// Insert missing code

	double  r,h,scale,n1;
	double rVertex=NUM_HORIZONTAL_VERTICES/2;
	for(int i = 0;i<NUM_HORIZONTAL_VERTICES;i++)
	{
		if (i<rVertex) 
		{ 
			n1 = cos(M_PI/2*(( double ) i/(rVertex-1)));
			h = RADIUS_PROFILE_CURVE_PILLAR - cos(M_PI/2*(( double ) i/(rVertex-1)))* RADIUS_PROFILE_CURVE_PILLAR;
			r = RADIUS_PILLAR - sin(M_PI/2*(( double ) i/( rVertex-1)))*RADIUS_PROFILE_CURVE_PILLAR;
		}
		else
		{
			n1 = -sin(M_PI/2*((( double ) i-rVertex)/(rVertex-1)));
			h = HEIGHT_PILLAR-RADIUS_PROFILE_CURVE_PILLAR+sin(M_PI/2*((( double ) i-rVertex)/(rVertex-1)))*RADIUS_PROFILE_CURVE_PILLAR;
			r = RADIUS_PILLAR-cos(M_PI/2*((( double ) i-rVertex)/(rVertex-1)))*RADIUS_PROFILE_CURVE_PILLAR;
		}
		for(int j = 0;j<NUM_CIRCUMFERENTIAL_VERTICES;j++)
		{
			scale = 1-n1*n1;
			vertices[i][j][0] = ( double ) -(cos(2*M_PI*j/( double ) (NUM_CIRCUMFERENTIAL_VERTICES-1))*r);
			vertices[i][j][1] = ( double ) h;
			vertices[i][j][2] = ( double ) (sin(2*M_PI*j/( double ) (NUM_CIRCUMFERENTIAL_VERTICES-1))*r);
			normals[i][j][0] = ( double ) -(cos(2*M_PI*j/( double ) (NUM_CIRCUMFERENTIAL_VERTICES-1))*scale);
			normals[i][j][1] = ( double ) n1;
			normals[i][j][2] = ( double ) (sin(2*M_PI*j/( double ) (NUM_CIRCUMFERENTIAL_VERTICES-1))*scale);
			texture_coordinates[i][j][0] = j/( double ) (NUM_CIRCUMFERENTIAL_VERTICES-1);
			texture_coordinates[i][j][1] = h/HEIGHT_PILLAR;
		}
	}

	// load stone texture
	ifstream textureFile;
	char* fileName="Stone.ppm";
	textureFile.open(fileName, ios::in);
	if (textureFile.fail())
		displayMessage(ERROR_MESSAGE, "CMonument()::CMonument(): could not open file %s",fileName);

	skipLine(textureFile);
	skipLine(textureFile);
	textureFile >> textureWidth;
	textureFile >> textureHeight;
	int numRGBValues;
	textureFile >> numRGBValues;
	texture = new GLubyte[textureWidth*textureHeight*4];

	for(int k=0;k<textureHeight;k++)
		for(int l=0;l<textureWidth;l++)
		{
			int c;
			textureFile >> c;
			texture[(k*textureWidth+l)*4]=(GLubyte) c;
			textureFile >> c;
			texture[(k*textureWidth+l)*4+1]=(GLubyte) c;
			textureFile >> c;
			texture[(k*textureWidth+l)*4+2]=(GLubyte) c;
			texture[(k*textureWidth+l)*4+3]=(GLubyte) 255;
		}
		textureFile.close();
}

void CMonument::draw()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	// ==============================================================================
	// = Draw the pillars of the monument.                                          =
	// = Define the vertices and normals of a surface in the constructor above and  = 
	// = draw 5 pillars fiiting neatly under the stone slap resulting in a picture  = 
	// = as in the assignment handout.                                              =
	// ==============================================================================
	// Insert missing code

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);	
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for(int k = 0;k<NUM_PILLARS; k++)
	{
		glPushMatrix();
		glTranslatef(-2.2, -1, -1.5+(1+(3*k))*RADIUS_PILLAR);
		for(int i = 0;i<NUM_HORIZONTAL_VERTICES-1; i++)
		{
			glBegin(GL_QUAD_STRIP);
			for(int j=0;j<NUM_CIRCUMFERENTIAL_VERTICES;j++)
			{
				glNormal3fv(normals[i][j]);
				glTexCoord2fv( texture_coordinates[i][j] );
				glVertex3fv(vertices[i][j]);
				glNormal3fv(normals[i+1][j]);
				glTexCoord2fv( texture_coordinates[i+1][j] );
				glVertex3fv(vertices[i+1][j]);
			}
			glEnd();
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

	//draw the stone slap on top of the monument

	glPushMatrix();
	glTranslatef(-2.2, 0.55, -0.1);
	glScalef(2*RADIUS_PILLAR,RADIUS_PROFILE_CURVE_PILLAR,14*RADIUS_PILLAR);
	glEnable( GL_NORMALIZE );
	glutSolidCube(1);
	glDisable( GL_NORMALIZE );
	glPopMatrix();
	glFlush();
}
