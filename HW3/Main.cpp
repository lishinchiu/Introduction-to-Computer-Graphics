#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <time.h>
#include <unordered_map>
#include <queue>
#include <conio.h>
#include <vector>

#include "./Library/glut/glut.h"
#include "./Library/SOIL/include/SOIL.h"

#pragma comment( lib, "./Library/glut/glut32.lib" )
#pragma comment( lib, "./Library/SOIL/lib/SOIL.lib" )
using namespace std;

bool move_x, move_y, move_z;
float extend = 0;
int mx, my;
int spinX = 0;
int spinY = 0; 
int spinZ = 0;
int des = 0;
int mov = 0;


void TimerFunc(int value) {
	if (move_z) {
		spinZ += 1;
		if (spinZ > 360)
			spinZ -= 360;
		
	}
	if (move_y) {
		spinY += 1;
		if (spinY > 360)
			spinY -= 360;
	}
	if (move_x) {
		spinX += 1;
		if (spinX > 360)
			spinX -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}


void drawSkewed(double l, double w, double h, double x, double y, double z, int MODE) {
	glPushMatrix();
	glScaled(l, w, h);
	glTranslated(x, y, z);
	if (MODE == 1) {
		glutSolidCube(1);
	}
	else if (MODE == 2) {
		glutWireCube(1);
	}
	glPopMatrix();
}

void init() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat light0_diffuse[] = { 5.5,5.0,1.0,1.0 };
	GLfloat light0_position[] = { 1.0,1.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 10, 10, 10, 10 };
	GLfloat mat_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	GLfloat ambient[] = { 0.5, 0.8, 0.1, 0.1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 5, 5, 5, 5 };
	GLfloat position[] = { -80, 0.0, 25.0, 1.0 };
	GLfloat lmodel_ambient[] = { 1, 1, 1, 2.5 };
	GLfloat local_view[] = { 0.0 };
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	glutTimerFunc(10, TimerFunc, 1);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(1.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();

	glRotated(spinX, 0, 1, 0);
	glRotated(spinY, 1, 0, 0);
	glRotated(spinZ, 0, 0, 1);
	glTranslated(0, 0, des);

	drawSkewed(1, 1, 1, 0, 0, 0, 1);
	
	glutSwapBuffers();
}
void mouseClick(int btn, int state, int x, int y) {
	GLfloat ambient[] = { (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 0.1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
}

void keyPressed(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		spinX -= 2;
		break;
	case 'd':
		spinX += 2;
		break;
	case 'w':
		des += 2;
		break;
	case 's':
		des -= 2;
		break;
	case '1':
		move_x = !move_x;
		break;
	case '2':
		move_y = !move_y;
		break;
	case '3':
		move_z = !move_z;
		break;
	}
	glutPostRedisplay();
}
void mouseMove(int x, int y) {
	int dx = x - mx;
	int dy = y - my;
	spinX += dx;
	spinY += dy;
	glutPostRedisplay();
	mx = x;
	my = y;
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutSetWindowTitle("Mazinger Z");
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove); 
	glutKeyboardFunc(keyPressed);
	glutMainLoop();
	return 0;
}