// Ball.h: interface for the CBall class.
// Copyright: (c) Burkhard Wuensche
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALL_H__D0DFB959_1ECF_444A_8AEC_8E103CB3A984__INCLUDED_)
#define AFX_BALL_H__D0DFB959_1ECF_444A_8AEC_8E103CB3A984__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MY_DEBUG_OPTION		// enables range checking for the array

#include <windows.h>
#include "Geometry.h"
#include <gl/gl.h>
#include <gl/glut.h>

class CBall  
{
public:
	CBall(GLfloat r=1.0f, GLfloat g=0.5f, GLfloat b=0.5f, float rad=1.0f);	// default values
	virtual ~CBall(){}
	virtual void draw();
protected:
	float radius;
	int numLatitudes, numLongitudes;
	CVertexArray2D vertices;	// for a unit sphere the position vectors to
								// the vertices are the normals at those points

	GLfloat mat_specular[4];
	GLfloat mat_ambient_and_diffuse[4];
	GLfloat mat_shininess[1];
};

class CAnimatedBall:public CBall  
{
public:
	friend void checkCollisionBetweenBalls(CAnimatedBall& ball1, CAnimatedBall& ball2);
	CAnimatedBall(GLfloat r=1.0f, GLfloat g=0.5f, GLfloat b=0.5f, float rad=1.0f);
	virtual ~CAnimatedBall(){}
	void draw();
	void setPosition(const CVec3df& p){ position=p;}
	void setVelocity(const CVec3df& v){ velocity=v;}
	void setMass(const float& m){ mass=m;}
	void updatePosition();
	void checkCollisionWithPlane(const CPlane3df& plane);
private:
	CVec3df position;
	CVec3df velocity;
	float mass;
	double timeLastStep;	// time of last animation step
};


#endif // !defined(AFX_BALL_H__D0DFB959_1ECF_444A_8AEC_8E103CB3A984__INCLUDED_)
