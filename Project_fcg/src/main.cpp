
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <gl/glut.h>

//openal (sound lib)
#include <al/alut.h>

//bitmap class to load bitmaps for textures
#include "bitmap.h"

//handle for the al.obj model
//#include "ModelAl.h"

//handle generic obj models
#include "mapgame.h"
#include "gameobj.h"
#include "3DObject.h"
#include "Environment.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

#define PI 3.14159265

// sound stuff
#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#define RES_PATH "res\\"
// #define MODELS_PATH "res\\models\\"
#define MODELS_PATH ""

#define GAME_NAME "FCG - TRABALHO FINAL"

void mainInit();
void initSound();
void initTexture();
void initScene();
void initLight();
void enableFog();
void createGLUI();
void mainRender();
void mainCreateMenu();
void onMouseButton(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onGLUIEvent(int id);
void onWindowReshape(int x, int y);
void mainIdle();
int main(int argc, char **argv);
void setWindow();
void setViewport(GLint left, GLint right, GLint bottom, GLint top);
void updateState();
void renderFloor();
void updateCam();
void setTextureToOpengl();

void updateLight();
void initLight1();
void initLight2();

/**
Screen dimensions
*/
int windowWidth = 600;
int windowHeight = 480;

/**
Screen position
*/
int windowXPos = 100;
int windowYPos = 150;

int mainWindowId = 0;

double xOffset = -1.9;
double yOffset = -1.3;
int mouseLastX = 0;
int mouseLastY = 0;

float roty = 0.0f;
float rotx = 90.0f;

bool spacePressed = false;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 0.0f;
float posY = 0.0f;
float posZ = 2.0f;

/*
variavel auxiliar pra dar variação na altura do ponto de vista ao andar.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;

float planeSize = 24.0f;

float lightD    = 0.0f;

// more sound stuff (position, speed and orientation of the listener)
ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,1.0,
						0.0,1.0,0.0};

// now the position and speed of the sound source
ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

// buffers for openal stuff
ALuint  buffer[NUM_BUFFERS];
ALuint  source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
ALsizei size,freq;
ALenum  format;
ALvoid  *data;

CEnvironment *en;

// parte de código extraído de "texture.c" por Michael Sweet (OpenGL SuperBible)
// texture buffers and stuff

GLubyte	    *bits;           /* Bitmap RGB pixels */
GLubyte     temp;            /* Swapping variable */





float jumpSpeed = 0.06;
float gravity = 0.004;
float heightLimit = 0.2;
float posYOffset = 0.2;




void setWindow() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void updateLight(){

    GLfloat light_position1[] = {posX, posY, posZ, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);


    GLfloat light_position2[] = {-35.0, 1.0, 0.0, 10.0 };
    glPushMatrix();
    glRotatef(lightD, 0.0,1.0,0.0);
    lightD++;
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glPopMatrix();

}

//void initLight1(){
//
//    glEnable( GL_LIGHT0 );
//
//    GLfloat light_ambient[] = { backgrundColor[0], backgrundColor[1], backgrundColor[2], backgrundColor[3] };
//	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_position[] = { 1.5,1.0,1.0,0.0 };
//
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
////
////    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
////    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
////    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 200.0);
////
//    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2.0);
//    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
//    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);
//}
//void initLight2(){
//
//    glEnable( GL_LIGHT1 );
//
//    GLfloat light_ambient[] = { backgrundColor[0], backgrundColor[1], backgrundColor[2], backgrundColor[3] };
//	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//	GLfloat light_position[] = {10.0, 1.0, 0.0, 0.0 };
//    GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };
//
//    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
//
//    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
//    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
//    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 100.0);
//}
//
//
//void initLight() {
//
//    glEnable(GL_LIGHTING );
//
//    initLight1();
//    initLight2();
//
//}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}


/**
Initialize
*/
void mainInit() {

	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);

	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

	// habilita remocao de faces ocultas

	glFrontFace(GL_CCW);


	glEnable(GL_CULL_FACE);

	// habilita o z-buffer
	glEnable(GL_DEPTH_TEST);

    //initSound();


    en->InitScene("map5");

    en->Floor->InitTexture("tiled_snow");

	//initLight();

	//enableFog();

	printf("w - andar \n");
	printf("s - ir pra tras \n");
	printf("a - girar para esquerda \n");
	printf("d - girar para direita \n");

	printf("espaco - empurrar blocos \n");
	printf("shift - criar blocos \n");
	printf("v - alternar cameras \n");

}


/**
Initialize openal and check for errors
*/
void initSound() {

	printf("Initializing OpenAl \n");

	// Init openAL
	alutInit(0, NULL);

	alGetError(); // clear any error messages

    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.\n");
    }

	alutLoadWAVFile("Footsteps.wav",&format,&data,&size,&freq,false);
    alBufferData(buffer[0],format,data,size,freq);

	alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR)
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;

	alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);

	alSourcef(source[0], AL_PITCH, 1.0f);
    alSourcef(source[0], AL_GAIN, 1.0f);
    alSourcefv(source[0], AL_POSITION, source0Pos);
    alSourcefv(source[0], AL_VELOCITY, source0Vel);
    alSourcei(source[0], AL_BUFFER,buffer[0]);
    alSourcei(source[0], AL_LOOPING, AL_TRUE);

	printf("Sound ok! \n\n");
}






void enableFog(void)
{
    glEnable(GL_FOG); // habilita fog
}




/**
Render scene
*/
void mainRender() {

	en->UpdateState();
	en->RenderScene();
	glFlush();
	glutPostRedisplay();
	Sleep(30);

}

/**
Handles events from the mouse right button menu
*/
void mainHandleMouseRightButtonMenuEvent(int option) {
	switch (option) {
		case 1 :
			exit(0);
			break;
		default:
			break;
	}
}

/**
Create mouse button menu
*/
void mainCreateMenu() {
	glutCreateMenu(mainHandleMouseRightButtonMenuEvent);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**
Mouse button event handler
*/
void onMouseButton(int button, int state, int x, int y) {
	//printf("onMouseButton button: %d \n", button);
	glutPostRedisplay();
}

/**
Mouse move while button pressed event handler
*/
void onMouseMove(int x, int y) {

	/*mouseLastX = x;
	mouseLastY = y;*/

	glutPostRedisplay();
}

/**
Mouse move with no button pressed event handler
*/
void onMousePassiveMove(int x, int y) {

	roty += (x - mouseLastX);

	rotx -= (y - mouseLastY);

	if (rotx < -128.0) {
		rotx = -128.0;
	}

	if (rotx > -45.0) {
		rotx = -45.0;
	}

	mouseLastX = x;
	mouseLastY = y;

	//glutPostRedisplay();
}

/**
Key press event handler
*/
void onKeyDown(unsigned char key, int x, int y) {
	//printf("%d \n", key);
	switch (key) {
		case 32: //space
			spacePressed = true;
			break;
		case 119: //w
			en->SetUp(true);
			break;
		case 115: //s
			en->SetDown(true);
			break;
		case 97: //a
			en->SetLeft(true);
			break;
		case 100: //d
			en->SetRight(true);
			break;
		case 118: //v
			en->SetView(true);
			break;
		default:
			break;
	}

	//glutPostRedisplay();
}

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y) {
	switch (key) {
		case 32: //space
			spacePressed = false;
			break;
		case 119: //w
			en->SetUp(false);
			break;
		case 115: //s
			en->SetDown(false);
			break;
		case 97: //a
			en->SetLeft(false);
			break;
		case 100: //d
			en->SetRight(false);
			break;
        case 118: //v
			en->SetView(false);
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}

	//glutPostRedisplay();
}

void onWindowReshape(int x, int y) {
	windowWidth = x;
	windowHeight = y;
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);
}

/**
Glut idle funtion
*/
void mainIdle() {
	/**
	Set the active window before send an glutPostRedisplay call
	so it wont be accidently sent to the glui window
	*/

	glutSetWindow(mainWindowId);
	glutPostRedisplay();
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);

    en = new CEnvironment();

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow(GAME_NAME);

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);

	/**
	Register mouse events handlers
	*/
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMousePassiveMove);

	/**
	Register keyboard events handlers
	*/
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);

	mainInit();

	glutMainLoop();

    return 0;
}
