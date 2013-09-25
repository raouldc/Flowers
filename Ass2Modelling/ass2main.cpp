//=============================================================
//= Assignment 2                                              =
//= An scene with grass, trees, 2 balls,                      =
//= a Greek monument and some flowers                         =
//=============================================================

#include <windows.h>
#include "Trackball.h"
#include "Utilities.h"
#include "Geometry.h"
#include "Mirror.h"
#include "GroundWithGrass.h"
#include "Ball.h"
#include "BillboardTree.h"
#include "Monument.h"
#include "Flower.h"
#include "Lighting.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

const int windowWidth=600;
const int windowHeight=600;
			  	 	
CTrackball trackball;
CMirror mirror;
CLighting lighting;
CAnimatedBall ball1(1.0,0.5,0.8,0.15f);
CAnimatedBall ball2(0.2,0.2,1.4,0.10f);
CGroundWithGrass ground;
const int numTrees=12;
const float minDistanceTrees=0.45f;
CBillboardTree trees[numTrees];
CMonument monument;
const int numFlowers=20;
const float minDistanceFlowers=0.3f;
CFlower flowers[numFlowers];


static GLuint texName[4];

void handleMouseMotion(int x, int y)
{	
	trackball.tbMotion(x, y);
}

void handleMouseClick(int button, int state, int x, int y)
{
	trackball.tbMouse(button, state, x, y);
}

void handleKeyboardEvent(unsigned char key, int x, int y)
{
	trackball.tbKeyboard(key);
}


void drawScene()
{
	int i;
	ground.draw();
	for(i=0;i<numTrees;i++) trees[i].draw();

	glShadeModel(GL_SMOOTH);
	lighting.enable();
	for(i=0;i<numFlowers;i++) flowers[i].draw();
	monument.draw();
	ball1.draw();
	ball2.draw();
	lighting.disable();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode( GL_MODELVIEW );	// Set the view matrix ...
	glLoadIdentity();				// ... to identity.
	trackball.tbMatrix();

	glEnable(GL_DEPTH_TEST);

	drawScene();

	mirror.drawMirrorImage(drawScene,lighting);

	glFlush ();
	glutSwapBuffers();
}

void animate()
{
	checkCollisionBetweenBalls(ball1,ball2);
	ball1.checkCollisionWithPlane(ground.getPlane());
	ball1.updatePosition();
	ball2.checkCollisionWithPlane(ground.getPlane());
	ball2.updatePosition();
}

void init(void) 
{
	// select clearing color (for glClear)
	glClearColor (1.0, 1.0, 1.0, 0.0);	// RGB-value for white

	// initialize view (simple perspective projection)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(6.0, (GLfloat) windowWidth/(GLfloat) windowHeight, 5.0, 55.0);
	gluLookAt(0,0,45, -0.8,0.2,0, 0,1,0);
	trackball.tbInit(GLUT_LEFT_BUTTON);

	lighting.init();

	// Initialise balls
	ball1.setPosition(CVec3df(-2,0,-1));
	ball1.setVelocity(CVec3df(25,0,0));
	ball2.setPosition(CVec3df(2,0.20,-1));
	ball2.setVelocity(CVec3df(-33,30,0));
	ball1.setMass(2.25);
	ball2.setMass(1);

	// Distribute trees randomly over the front half of the ground plane such
	// that the minimum distance between any two trees is at least minDistanceTrees
	GLfloat x,z,xx,zz; // (x,z) positions of trees
	bool validPosition;
	int i;
	for(i=0;i<numTrees;i++)
	{
		do {
			validPosition=true;
			x=(GLfloat) randomDouble(-1.95,1.95);
			z=(GLfloat) randomDouble(0,1.95);
			for(int j=0;j<i;j++)
			{	// position is valid if the distance between the trunks of each tree is at least minDistanceTrees
				trees[j].getPosition(xx,zz);
				if (sqrt((xx-x)*(xx-x)+(zz-z)*(zz-z))<minDistanceTrees) { validPosition=false; break;}
			}
		} while (validPosition==false);
		trees[i].setPosition(x,z);
	}

	// Distribute flowers randomly over the back half of the ground plane such
	// that the minimum distance between any two flowers is at least minDistanceFlowers
	// x,z,xx,zz; // (x,z) positions of trees
	for(i=0;i<numFlowers;i++)
	{
		do {
			validPosition=true;
			x=(GLfloat) randomDouble(-1.95,1.95);
			z=(GLfloat) randomDouble(-1.8,-1.1);
			for(int j=0;j<i;j++)
			{	// position is valid if the distance between the stems of each tree is at least minDistanceFlowers
				flowers[j].getPosition(xx,zz);
				if (sqrt((xx-x)*(xx-x)+(zz-z)*(zz-z))<minDistanceFlowers) { validPosition=false; break;}
			}
		} while (validPosition==false);
		flowers[i].setPosition(x,z);
	}

	// initialise three texture names
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(3, texName);
	ground.setTexture(texName[0]);
	monument.setTexture(texName[1]);
	trees[0].setTexture(texName[2]);
}

void reshape(int width, int height ) {
	// Called at start, and whenever user resizes component
	int size = min(width, height);
	glViewport(0, 0, size, size);  // Largest possible square
	trackball.tbReshape(width, height);
}

void myTimer(int value){
	animate();
	glutPostRedisplay();
	glutTimerFunc(1000/30, myTimer, 1);
}

// create a single buffered colour window
int main(int argc, char** argv)
{
	glutInit(&argc, argv);	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(windowWidth, windowHeight); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment 2");
	init ();						// initialise view
	glutMouseFunc(handleMouseClick);		// Set function to handle mouse clicks
	glutMotionFunc(handleMouseMotion);		// Set function to handle mouse motion
	glutKeyboardFunc(handleKeyboardEvent);	// Set function to handle keyboard input
	glutDisplayFunc(display);		// Set function to draw scene
	glutReshapeFunc(reshape);		// Set function called if window gets resized
	glutTimerFunc(1000/30, myTimer,1);
	glutMainLoop();
	return 0;
}
