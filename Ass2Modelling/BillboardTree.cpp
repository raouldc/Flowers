// BillboardTree.cpp: implementation of the CBillboardTree class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include "BillboardTree.h"
#include "Utilities.h"

bool CBillboardTree::staticVariablesInitialised=false;
float CBillboardTree::texture_coordinates[4][2];
GLuint CBillboardTree::texName;
GLubyte* CBillboardTree::texture;
int CBillboardTree::textureWidth;
int CBillboardTree::textureHeight;

CBillboardTree::CBillboardTree(void)
{
	// initialise geometry parameters
	billboardSizeHalf=0.6f;
	vertices[0][0]=x-billboardSizeHalf;
	vertices[0][1]=0;
	vertices[0][2]=z;
	vertices[1][0]=x+billboardSizeHalf;
	vertices[1][1]=0;
	vertices[1][2]=z;
	vertices[2][0]=x+billboardSizeHalf;
	vertices[2][1]=02*billboardSizeHalf;
	vertices[2][2]=z;
	vertices[3][0]=x-billboardSizeHalf;
	vertices[3][1]=2*billboardSizeHalf;
	vertices[3][2]=z;

	if (!staticVariablesInitialised)
	{
		staticVariablesInitialised=true;
		texture_coordinates[0][0]=1;
		texture_coordinates[0][1]=1;
		texture_coordinates[1][0]=0;
		texture_coordinates[1][1]=1;
		texture_coordinates[2][0]=0;
		texture_coordinates[2][1]=0;
		texture_coordinates[3][0]=1;
		texture_coordinates[3][1]=0;

		// load tree texture
		ifstream textureFile;
		char* fileName="TreeBillboard.ppm";
		textureFile.open(fileName, ios::in);
		if (textureFile.fail())
			displayMessage(ERROR_MESSAGE, "CBillboardTree()::CBillboardTree(): could not open file %s",fileName);

		skipLine(textureFile);
		skipLine(textureFile);
		textureFile >> textureWidth;
		textureFile >> textureHeight;
		int numRGBAValues;
		textureFile >> numRGBAValues;
		texture = new GLubyte[textureWidth*textureHeight*4];	

		int k,l,r,g,b;
		for(k=0;k<textureHeight;k++)
			for(l=0;l<textureWidth;l++)
			{
				textureFile >> r >> g >> b;
				texture[(k*textureWidth+l)*4]=(GLubyte) r;
				texture[(k*textureWidth+l)*4+1]=(GLubyte) g;
				texture[(k*textureWidth+l)*4+2]=(GLubyte) b;
				if ((r==255) && (g==255) && (b=255))
					texture[(k*textureWidth+l)*4+3]=(GLubyte) 0;
				else
					texture[(k*textureWidth+l)*4+3]=(GLubyte) 255;
			}
		textureFile.close();
	}
}

void CBillboardTree::draw()
{
	glPushMatrix();
	glTranslatef(x,-1,z);

	float modelview[16];
	int i,j;
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	// undo all rotations - beware all scaling is lost as well (our billboard is not scaled)
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);

	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, 0.9f);
	glEnable(GL_ALPHA_TEST);

	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 
				0, GL_RGBA, GL_UNSIGNED_BYTE, texture);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	for(int i=0;i<4;i++)
	{
		glTexCoord2fv(texture_coordinates[i]);
		glVertex3fv(vertices[i]);
	}
	glEnd();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glPopMatrix();
}
