#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void drawScene(void);

// Camera
float nearView = -2.0;
float farView = 2.0;

// Cube values
float cubeColor[] = {1.0, 1.0, 1.0, 1.0}; // cube Color RGBA
float cubeShininess = 10;

// Cube Movement
bool isCubeRotating = false;
float cubeAngle[] = {45.0, 45.0, 0.0}; // angle of the cube
float cubeAngleStep = 2;

// Light values
float lightAmbient[4] = {0.3, 0.3, 0.3, 1.0};  // RGBA of the ambient
float lightSpecular[4] = {5.0, 5.0, 5.0, 1.0}; // RGBA light object color

float lightDiffuse[4] = {1.0, 1.0, 1.0, 1.0}; // RGBA light (waves) color
float Noemit[4] = {0.0, 0.0, 0.0, 1.0};		  // change emission (now it's off)

float lightDirI[4] = {1, 0, 0, 0}; // Direction of unit vector I

// Light movement
bool isLightMoving = true;
float lightAngle = 0.0f; // angle in regarts with Ox
float lightSpeed = 0.5f; // speed of the light object

// Menu
void menuInteraction(int num)
{
	switch (num)
	{
	//Cube
	case 10:
		// color white and angles 45 45 0
		cubeColor[0] = 1;
		cubeColor[1] = 1;
		cubeColor[2] = 1;

		cubeAngle[0] = 45;
		cubeAngle[1] = 45;
		cubeAngle[2] = 0;
		break;

	case 11:
		// Red
		cubeColor[0] = 1;
		cubeColor[1] = 0;
		cubeColor[2] = 0;
		break;

	case 12:
		// Blue
		cubeColor[0] = 0;
		cubeColor[1] = 0;
		cubeColor[2] = 1;
		break;

	case 13:
		// Green
		cubeColor[0] = 0;
		cubeColor[1] = 1;
		cubeColor[2] = 0;
		break;

	case 14:
		// White
		cubeColor[0] = 1;
		cubeColor[1] = 1;
		cubeColor[2] = 1;
		break;
	case 0:
		exit(1);
		break;

	// Light
	case 21:
		//High
		for (int i = 0; i < 3; i++)
			lightDiffuse[i] = 1;
		break;
	case 22:
		//Medium
		for (int i = 0; i < 3; i++)
			lightDiffuse[i] = 0.5;
		break;
	case 23:
		//None
		for (int i = 0; i < 3; i++)
			lightDiffuse[i] = 0.1;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void createMenu(void)
{
	auto submenu_cube = glutCreateMenu(menuInteraction);
	glutAddMenuEntry("Red", 11);
	glutAddMenuEntry("Blue", 12);
	glutAddMenuEntry("Green", 13);
	glutAddMenuEntry("White", 14);

	auto submenu_light = glutCreateMenu(menuInteraction);
	glutAddMenuEntry("High", 21);
	glutAddMenuEntry("Medium", 22);
	glutAddMenuEntry("None", 23);

	auto menu_id = glutCreateMenu(menuInteraction);
	glutAddMenuEntry("Reset Cube", 10);
	glutAddSubMenu("Cube Color", submenu_cube);
	glutAddSubMenu("Light Intensity", submenu_light);
	glutAddMenuEntry("Quit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Utils
void checkColorsOverflows()
{
	// check for any overflows
	for (int i = 0; i <= 3; i++)
	{
		// light waves
		if (lightDiffuse[i] > 1)
			lightDiffuse[i] = 1;

		else if (lightDiffuse[i] < 0)
			lightDiffuse[i] = 0;

		// cub color
		if (cubeColor[i] > 1)
			cubeColor[i] = 1;

		else if (cubeColor[i] < 0)
			cubeColor[i] = 0;
	}
}

void checkAnglesOverflows()
{
	// check for any overflows
	for (int i = 0; i <= 3; i++)
		if (cubeAngle[i] > 360)
			lightDiffuse[i] = 0;

	if (lightAngle > 360)
		lightAngle = 0;
}

// Keyboard
void cubeKeyboard(unsigned char key)
{
	switch (key)
	{
	// Cube movement
	case 'w':
		cubeAngle[0] += cubeAngleStep;
		break;

	case 's':
		cubeAngle[0] -= cubeAngleStep;
		break;

	case 'a':
		cubeAngle[1] += cubeAngleStep;
		break;

	case 'd':
		cubeAngle[1] -= cubeAngleStep;
		break;
	case 'q':
		cubeAngle[2] += cubeAngleStep;
		break;

	case 'e':
		cubeAngle[2] -= cubeAngleStep;
		break;

	case 'r':
		// make cube rotate
		isCubeRotating = !isCubeRotating;
		break;

	// Cube color
	case 'z':
		cubeColor[0] += 0.1;
		break;

	case 'Z':
		cubeColor[0] -= 0.1;
		break;

	case 'x':
		cubeColor[1] += 0.1;
		break;

	case 'X':
		cubeColor[1] -= 0.1;
		break;

	case 'c':
		cubeColor[2] += 0.1;
		break;

	case 'C':
		cubeColor[2] -= 0.1;
		break;

	default:
		break;
	}
}

void lightKeyboard(unsigned char key)
{
	switch (key)
	{
	// Light Option
	case 'p':
		isLightMoving = !isLightMoving;
		break;

	case 'o':
		isLightMoving = 1;
		drawScene();
		isLightMoving = 0;
		break;

	// Light movement
	case ']':
		if (lightSpeed < 5.0)
		{
			lightSpeed += 0.5;
		}
		break;
	case '[':
		if (lightSpeed > 0)
		{
			lightSpeed -= 0.5;
		}
		break;
	case 'i':
		lightSpeed = -lightSpeed;
		break;

	// Light intensity
	case '.':
		for (int i = 0; i < 3; i++)
			lightDiffuse[i] += 0.1;
		break;

	case ',':
		for (int i = 0; i < 3; i++)
		{
			if (lightDiffuse[i] < 0.1)
				lightDiffuse[i] = 0.1;
			lightDiffuse[i] -= 0.1;
		}
		break;
	default:
		break;
	}
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	cubeKeyboard(key);
	lightKeyboard(key);

	switch (key)
	{
	case 27:
		exit(1);
	}

	checkColorsOverflows();
	drawScene();
}

// Movement
void dealWithMovement()
{
	if (isLightMoving)
		lightAngle += lightSpeed;

	if (isCubeRotating)
		for (int i = 0; i < 2; i++)
			cubeAngle[i] += cubeAngleStep;
}

// Draw Objects
void drawCube()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cubeColor);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cubeShininess);
	glPushMatrix();

	glRotatef(cubeAngle[0], 1.0, 0.0, 0.0); // Rotate cube x
	glRotatef(cubeAngle[1], 0.0, 1.0, 0.0); // Rotate cube y
	glRotatef(cubeAngle[2], 0.0, 0.0, 1.0); // Rotate cube z

	glutSolidCube(2);
	glFlush();
	glutSwapBuffers();
}

void drawLight()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glPushMatrix();
	glRotatef(lightAngle, 0.0, 0.0, 1.0);
	glTranslatef(5.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, lightSpecular); // Make cube glow (emissive)
	glutSolidSphere(0.3, 5, 5);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit); // Turn off emission

	glLightfv(GL_LIGHT0, GL_POSITION, lightDirI);

	glPopMatrix();
}

void drawScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dealWithMovement();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawLight();

	drawCube();

	glutPostRedisplay();
}

// Resizing
void resizeWindow(int w, int h)
{
	float viewWidth = 7; // keep those the same value so the image does not stretch
	float viewHeight = 7;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h < w)
	{
		viewWidth *= (float)w / (float)h;
	}
	else
	{
		viewHeight *= (float)h / (float)w;
	}
	glOrtho(-viewWidth, viewWidth, -viewHeight, viewHeight, nearView, farView);
}

// Init
void initRendering()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING); // enable the lighting
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}

void init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(10, 60);
	glutInitWindowSize(1260, 720);
	glutCreateWindow("Simple Move Light");
}

// Controls Help
void printControls()
{
	std::cout << "Cube Controls: " << std::endl;
	std::cout << "               - rotation: w/s (X axis), a/d (Y axis), q/e (Z axis), r (start/stop rotation)" << std::endl;
	std::cout << "               - color: z/shift + z (Red), x/shift + x (Green), c/shift + c (Blue)" << std::endl;
	std::cout << "Light Controls: " << std::endl;
	std::cout << "               - movement: p (stop/start rotation), o (run one step), i (change direction)" << std::endl;
	std::cout << "               - speed: ] (increse speed), [ (decrease speed)" << std::endl;
	std::cout << "               - intensity: , (decrease intensity), . (increase intensity)" << std::endl;
	std::cout << "Right Click to Open Menu! " << std::endl;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	init();
	initRendering();

	printControls();
	createMenu();

	glutKeyboardFunc(myKeyboardFunc);

	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(drawScene);

	glutMainLoop();
}
