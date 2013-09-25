// Mirror.cpp: implementation of the CMirror class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include "Mirror.h"
#include "Algebra.h"
#include <gl/glut.h>

CMirror::CMirror()
{ 
	p0=CVec3df(-2.0,-1.0,-2.0);
	p1=CVec3df(2.0,-1.0,-2.0);
	p2=CVec3df(-2.0,2.0,-2.0);

	showMirror=true;

	p3=p1+p2-p0;
	initReflectionMatrix();
	initWorldToMirrorTransformation();
}

void CMirror::drawMirrorImage(void (*drawScene)(), const CLighting& lighting)
{
	glClear(GL_STENCIL_BUFFER_BIT);
	if (showMirror)
	{
		// Don't update color or depth.
		glDisable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		// Draw 1 into the stencil buffer.
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
		// Now render mirror; mirror pixels just get their stencil set to 1. 
		glEnable(GL_CULL_FACE);
		drawMirror(MIRROR_FRONT);
		glDisable(GL_CULL_FACE);
		// Re-enable update of color and depth. 
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		// Now, only render where stencil is set to 1. 
		glStencilFunc(GL_EQUAL, 1, 0xffffffff);  // draw if ==1 	
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		
		glPushMatrix();
		applyReflectionMatrix();
		lighting.init();		// mirror light sources
		drawScene();			// render the reflected scene
 		glPopMatrix();
		glDisable(GL_STENCIL_TEST);
		lighting.init();		// back to the original light sources
	
		// Back face culling will get used to only draw either the front side or the
		// back side of the mirror.  This let's us get a mirror with two distinct
		// appearances.  The front surface is reflective and kind of grayish.
		// The back surface is not reflective and blue.
		
		// Draw "back" of mirror in blue. 
		glColor4f(0.1f, 0.1f, 0.5f, 1.0f);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		drawMirror(MIRROR_BACK);
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);

		// Draw the mirror with stencil value 3.  
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		// Draw reflective side of mirror.  Use blending to blend in reflection.
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.9, 0.9, 0.9, 0.3);
		glCullFace(GL_BACK);
   		drawMirror(MIRROR_FRONT);
		glDisable(GL_BLEND);
		drawMirror(MIRROR_BOUNDARY);	
	}
}


void CMirror::applyReflectionMatrix()
{ 
	glMultMatrixf((GLfloat*) mirrorToWorldTransformation);
	glMultMatrixf((GLfloat*) reflectionMatrix);
	glMultMatrixf((GLfloat*) worldToMirrorTransformation);
}


// Matrix which reflects at xy-plane
void CMirror::initReflectionMatrix()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			reflectionMatrix[i][j]=0.0;
		reflectionMatrix[i][i]=1.0;
	}
	reflectionMatrix[2][2]=-1.0;
}

void CMirror::initWorldToMirrorTransformation()
{
	// compute u,v,n coordinate system of the mirror 
	CVec3df u=p1-p0;
	CVec3df v=p2-p0;
	n=cross(u,v);
	v=cross(n,u);
	n.normaliseDestructive();
	u.normaliseDestructive();
	v.normaliseDestructive();

	// create matrix m which transforms a point from u,v,n coordinates to x,y,z coordinates
	CMatrix m(4,4);
	m(0,0)=(float) u[0];
	m(1,0)=(float) u[1];
	m(2,0)=(float) u[2];
	m(3,0)=0.0;
	m(0,1)=(float) v[0];
	m(1,1)=(float) v[1];
	m(2,1)=(float) v[2];
	m(3,1)=0.0;
	m(0,2)=(float) n[0];
	m(1,2)=(float) n[1];
	m(2,2)=(float) n[2];
	m(3,2)=0.0;
	m(0,3)=(float) p0[0];
	m(1,3)=(float) p0[1];
	m(2,3)=(float) p0[2];
	m(3,3)=1.0;

	// initialise the OpenGL transformation matrices using the
	// previously computed transformation
	// NOTE: OpenGL matrices are defined columnwise, i.e. we have to
	// use the TRANSPOSE of the matrix.
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			mirrorToWorldTransformation[i][j]=(GLfloat) m(j,i); // store transpose
	CMatrix mInv=m.inverse();
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			worldToMirrorTransformation[i][j]=(GLfloat) mInv(j,i); // store transpose
}

void CMirror::drawMirror(PART_OF_MIRROR part)
{
	switch (part)
	{
	case MIRROR_FRONT:
	{
		glBegin(GL_POLYGON);
		glNormal3fv(n.getArray());
		glVertex3fv(p0.getArray());
		glVertex3fv(p1.getArray());
		glVertex3fv(p3.getArray());
		glVertex3fv(p2.getArray());
		glEnd();
		break;
	}
	case MIRROR_BACK:
	{
		CVec3df nn=-n;
		glBegin(GL_POLYGON);
		glNormal3fv(nn.getArray());
		glVertex3fv(p0.getArray());
		glVertex3fv(p1.getArray());
		glVertex3fv(p3.getArray());
		glVertex3fv(p2.getArray());
		glEnd();
		break;
	}
	case MIRROR_BOUNDARY:
	{
		glLineWidth(2.0);
		glColor3f(0.0,0.0,0.0);
//		glDisable(GL_LIGHTING);
		glBegin(GL_LINE_LOOP);
		glNormal3fv(n.getArray());
		glVertex3fv(p0.getArray());
		glVertex3fv(p1.getArray());
		glVertex3fv(p3.getArray());
		glVertex3fv(p2.getArray());
		glEnd();
//		glEnable(GL_LIGHTING);
		glLineWidth(1.0);
		break;
	}
	}
}
