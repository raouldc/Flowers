// Ball.cpp: implementation of the CBall class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <gl/gl.h>
#include "Ball.h"
#include "Geometry.h"

CBall::CBall(GLfloat r, GLfloat g, GLfloat b, float rad):radius(rad)
{
	// initiaise material properties
	mat_ambient_and_diffuse[0]=r;
	mat_ambient_and_diffuse[1]=g;
	mat_ambient_and_diffuse[2]=b;
	mat_ambient_and_diffuse[3]=1;
	mat_specular[0]=0.6f;
	mat_specular[1]=0.6f;
	mat_specular[2]=0.6f;
	mat_specular[3]=1;
	mat_shininess[0]=100;

	// initialise geometry
	numLatitudes = 16;
	numLongitudes = 32;	

	// allocate and compute the mesh vertices
	vertices.setSize(numLatitudes, numLongitudes);
	int lg,lt;
	for(lt=0; lt<numLatitudes; lt++)
	{
		double z = cos(Pi*lt/((double) (numLatitudes-1)));
		double r = sin(Pi*lt/((double) (numLatitudes-1)));
		for(lg=0; lg<numLongitudes; lg++)
		{
			double x = cos(2*Pi*lg/(double) numLongitudes)*r;
			double y = sin(2*Pi*lg/(double) numLongitudes)*r;
			vertices(lt,lg)[X]=(float) x;
			vertices(lt,lg)[Y]=(float) y;
			vertices(lt,lg)[Z]=(float) z;
		}
	}
}

void CBall::draw()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_and_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_NORMALIZE);
	glPushMatrix();
	glScalef(radius,radius,radius);
	int lg,lt;
	for(lt=0; lt<numLatitudes-1; lt++)
	{
		glBegin(GL_QUAD_STRIP);
		for(lg=0; lg<numLongitudes; lg++)
		{
	        glNormal3fv(vertices(lt,lg));
			glVertex3fv(vertices(lt,lg));
	        glNormal3fv(vertices(lt+1,lg));
			glVertex3fv(vertices(lt+1,lg));
		}	
		glNormal3fv(vertices(lt,0));	// close the quadstrip around the longitude
		glVertex3fv(vertices(lt,0));
		glNormal3fv(vertices(lt+1,0));
		glVertex3fv(vertices(lt+1,0));

		glEnd();
	}
	glPopMatrix();
	glDisable( GL_NORMALIZE );
}


const double speedFactor=0.005;
#define COLLISION_TOLERANCE -0.001

CAnimatedBall::CAnimatedBall(GLfloat r, GLfloat g, GLfloat b, float radius):CBall(r,g,b,radius)
{
	timeLastStep=(double) clock();
}

void CAnimatedBall::draw()
{
	glPushMatrix();
	glTranslatef(position[X],position[Y],position[Z]);
	CBall::draw();
	glPopMatrix();
}

void CAnimatedBall::updatePosition()
{
	double time=(double) clock();
	double dt=(time-timeLastStep)/CLOCKS_PER_SEC;
	position+=(float) dt*velocity*speedFactor;
	velocity*=0.998f;				// damping
	velocity[Y]-=(float) 9.81f*dt;	// gravity
	timeLastStep=time;
}

void CAnimatedBall::checkCollisionWithPlane(const CPlane3df& plane)
{
	float d=plane.distance(position);
	if (d-radius<COLLISION_TOLERANCE) 
	{
		// offset position in normal direction so that ball is exactly on plane
		position+=(radius-d)*plane.getNormal();
		// reflect velocity vector on plane
		velocity=reflect(velocity,plane.getNormal());
	}
}

void checkCollisionBetweenBalls(CAnimatedBall& ball1, CAnimatedBall& ball2)
{
	CVec3df n=ball2.position-ball1.position;
	double distance=n.length()-(ball1.radius+ball2.radius);
	if (distance<COLLISION_TOLERANCE)	// collision!
	{
		// offset balls in the direction the collision plane normal so that they are just touching
		ball1.position+=distance*n.normalise();
		ball1.position-=distance*n.normalise();
		n=ball2.position-ball1.position;
		// compute the new velocities after collision
		CVec3df v1Old=ball1.velocity;
		CVec3df v2Old=ball2.velocity;
		CVec3df v1OldNormalDirection, v1OldOrthogonal;
		CVec3df v2OldNormalDirection, v2OldOrthogonal;
		v1Old.decompose(n, v1OldNormalDirection, v1OldOrthogonal);
		n*=-1;	// reverse n
		v2Old.decompose(n, v2OldNormalDirection, v2OldOrthogonal);

		float s1=(ball1.mass-ball2.mass)/(ball1.mass+ball2.mass);
		float t1=2*ball2.mass/(ball1.mass+ball2.mass);
		float s2=2*ball1.mass/(ball1.mass+ball2.mass);
		float t2=(ball2.mass-ball1.mass)/(ball1.mass+ball2.mass);
		CVec3df v1=s1*v1OldNormalDirection+t1*v2OldNormalDirection;
		CVec3df v2=s2*v1OldNormalDirection+t2*v2OldNormalDirection;
		ball1.velocity=v1+v1OldOrthogonal;
		ball2.velocity=v2+v2OldOrthogonal;

		// offset balls again in the direction the collision plane normal so that they are just touching
		distance=length(ball2.position-ball1.position)-(ball1.radius+ball2.radius);
		if (distance<0)
		{
			ball1.position+=distance*n.normalise();
			ball1.position-=distance*n.normalise();
		}
	}
}

