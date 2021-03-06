/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include "SurfaceB.h"


/// 4x4 grid of points that will define the surface
Point Points[4][4] = {
	{
		{ 0,0,10 },
		{  5,0,10 },
		{ -5,0,10 },
		{-10,0,10 }
	},
	{
		{ 10,0,5 },
		{  5,18,5 },
		{ -5,5,5 },
		{-10,12,5 }
	},
	{
		{ 10,0,-5 },
		{  5,6,-5 },
		{ -5,6,-5 },
		{-10,0,-5 }
	},
	{
		{ 10,0,-10 },
		{  5,0,-10 },
		{ -5,0,-10 },
		{-10,0,-10 }
	}
};

// the level of detail of the surface
unsigned int LOD = 25;

//------------------------------------------------------------	CalculateU()
// Given our 16 control points, we can imagine 4 curves travelling
// in a given surface direction. For example, if the parametric u
// value was 0.5, we could imagine evaluating 4 seperate curves
// at u=0.5.
//
// This function is basically here to perform that very task. each
// row of 4 points in the u-direction is evaluated to provide us
// with 4 new points. These new points then form a curve we can
// evaluate in the v direction to calculate our final output point.
//
void initlights(void)
{
   GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
   GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
   GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat mat_shininess[] = {50.0};

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0, GL_POSITION, position);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
Point CalculateU(float t,int row) {

	// the final point
	Point p;

	// the t value inverted
	float it = 1.0f-t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3*t*t*it;
	float b2 = 3*t*it*it;
	float b3 =  it*it*it;

	// sum the effects of the Points and their respective blending functions
	p.x = b0*Points[row][0].x +
		  b1*Points[row][1].x +
		  b2*Points[row][2].x +
		  b3*Points[row][3].x ;

	p.y = b0*Points[row][0].y +
		  b1*Points[row][1].y +
		  b2*Points[row][2].y +
		  b3*Points[row][3].y ;

	p.z = b0*Points[row][0].z +
		  b1*Points[row][1].z +
		  b2*Points[row][2].z +
		  b3*Points[row][3].z ;

	return p;
}

//------------------------------------------------------------	CalculateV()
// Having generated 4 points in the u direction, we need to
// use those points to generate the final point on the surface
// by calculating a final bezier curve in the v direction.
//     This function takes the temporary points and generates
// the final point for the rendered surface
//
Point CalculateV(float t,Point* pnts) {
	Point p;

	// the t value inverted
	float it = 1.0f-t;

	// calculate blending functions
	float b0 = t*t*t;
	float b1 = 3*t*t*it;
	float b2 = 3*t*it*it;
	float b3 =  it*it*it;

	// sum the effects of the Points and their respective blending functions
	p.x = b0*pnts[0].x +
		  b1*pnts[1].x +
		  b2*pnts[2].x +
		  b3*pnts[3].x ;

	p.y = b0*pnts[0].y +
		  b1*pnts[1].y +
		  b2*pnts[2].y +
		  b3*pnts[3].y ;

	p.z = b0*pnts[0].z +
		  b1*pnts[1].z +
		  b2*pnts[2].z +
		  b3*pnts[3].z ;

	return p;
}

//------------------------------------------------------------	Calculate()
// On our bezier patch, we need to calculate a set of points
// from the u and v parametric range of 0,0 to 1,1. This calculate
// function performs that evaluation by using the specified u
// and v parametric coordinates.
//
Point Calculate(float u,float v) {

	// first of all we will need to evaluate 4 curves in the u
	// direction. The points from those will be stored in this
	// temporary array
	Point temp[4];

	// calculate each point on our final v curve
	temp[0] = CalculateU(u,0);
	temp[1] = CalculateU(u,1);
	temp[2] = CalculateU(u,2);
	temp[3] = CalculateU(u,3);

	// having got 4 points, we can use it as a bezier curve
	// to calculate the v direction. This should give us our
	// final point
	//
	return CalculateV(v,temp);
}

//------------------------------------------------------------	OnReshape()
//
void OnReshape(int w, int h)
{
	// prevent a division by zero when setting aspect ratio
	if (h==0)
		h=1;

	// set the drawable region of the window
	glViewport(0,0,w,h);

	// set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// just use a perspective projection
	gluPerspective(45,(float)w/h,0.1,100);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//------------------------------------------------------------	OnDraw()
//
void OnDraw() {

	// clear the screen & depth buffer
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// clear the previous transform
	glLoadIdentity();

	// set the camera position
	gluLookAt(	5,16,10,	//	eye pos
				0,0,0,	//	aim point
				0,1,0);	//	up direction

	glColor3f(1,0,1);
	glPointSize(1.0);

    float t = 1+1.0/LOD;
    float h = 1+1.0/LOD;

	// use the parametric time value 0 to 1
	for(int i=0;i!=LOD;++i) {

		// calculate the parametric u value
		float u = (float)i/(LOD-1);

		for(int j=0;j!=LOD;++j) {

            // calculate the parametric v value
			float v = (float)j/(LOD-1);

			// calculate the point on the surface
			Point p = Calculate(u,v);

			// draw point

			glBegin(GL_QUADS);
                glVertex3f(p.x+t,p.y,p.z);
                glVertex3f(p.x,p.y,p.z);
                glVertex3f(p.x,p.y,p.z+t);
                glVertex3f(p.x+t,p.y,p.z+t);
			glEnd();

			glBegin(GL_QUADS);
                glVertex3f(p.x+t,p.y-h,p.z+t);
                glVertex3f(p.x+t,p.y-h,p.z);
                glVertex3f(p.x+t,p.y,p.z);
                glVertex3f(p.x+t,p.y,p.z+t);
			glEnd();

			glBegin(GL_QUADS);
                glVertex3f(p.x,p.y-h,p.z);
                glVertex3f(p.x+t,p.y-h,p.z);
                glVertex3f(p.x+t,p.y,p.z);
                glVertex3f(p.x,p.y,p.z);
			glEnd();

			glBegin(GL_QUADS);
                glVertex3f(p.x+t,p.y-h,p.z+t);
                glVertex3f(p.x,p.y-h,p.z+t);
                glVertex3f(p.x,p.y,p.z+t);
                glVertex3f(p.x+t,p.y,p.z+t);
			glEnd();

			glBegin(GL_QUADS);
                glVertex3f(p.x,p.y-h,p.z);
                glVertex3f(p.x,p.y-h,p.z+t);
                glVertex3f(p.x,p.y,p.z+t);
                glVertex3f(p.x,p.y,p.z);
			glEnd();
		}
	}



	// currently we've been drawing to the back buffer, we need
	// to swap the back buffer with the front one to make the image visible
	glutSwapBuffers();

	initlights();
}

//------------------------------------------------------------	OnInit()
//
void OnInit() {
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------	OnExit()
//
void OnExit() {
}

//------------------------------------------------------------	OnKeyPress()
//
void OnKeyPress(unsigned char key,int,int) {
	switch(key) {

	// increase the LOD
	case '+':
		++LOD;
		break;

	// decrease the LOD
	case '-':
		--LOD;

		// have a minimum LOD value
		if (LOD<3)
			LOD=3;
		break;
	default:
		break;
	}

	// ask glut to redraw the screen for us...
	glutPostRedisplay();
}

//------------------------------------------------------------	main()
//
int main(int argc,char** argv) {

	// initialise glut
	glutInit(&argc,argv);

	// request a depth buffer, RGBA display mode, and we want double buffering
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);

	// set the initial window size
	glutInitWindowSize(640,480);

	// create the window
	glutCreateWindow("Bezier Curve: +/- to Change Level of Detail");

	// set the function to use to draw our scene
	glutDisplayFunc(OnDraw);

	// set the function to handle changes in screen size
	glutReshapeFunc(OnReshape);

	// set the function for the key presses
	glutKeyboardFunc(OnKeyPress);

	// run our custom initialisation
	OnInit();

	// set the function to be called when we exit
	atexit(OnExit);

	// this function runs a while loop to keep the program running.
	glutMainLoop();
	return 0;
}
