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
bool walk_x, walk_y, walk_z;
bool walk,shoot=0;
float extend=0;
int mx, my;
int spinX = 0;
int spinY = 0;
int des = 0;
int mov = 0;
float leftArmAngle = 0.0;
float rightArmAngle = 0.0;

int theta_x = 0, theta_y = 0, theta_z = 0;
int angle_x = 0, angle_y = 0, angle_z = 0;


void TimerFunc(int value) {
	if (walk_x) {
		if (angle_x > 45) {
			walk = 1;
		}
		else if (angle_x < -45) {
			walk = 0;
		}
		if (walk) {
			angle_x -= 1;
		}
		else {
			angle_x += 1;
		}
	}
	if (move_z) {
		theta_z += 1;
		if (theta_z > 360)
			theta_z -= 360;
	}
	if (move_y) {
		theta_y += 1;
		if (theta_y > 360)
			theta_y -= 360;
	}
	if (move_x) {
		theta_x += 1;
		if (theta_x > 360)
			theta_x -= 360;
	}
	if (walk_z) {
		angle_z += 1;
		if (angle_z > 360)
			angle_z -= 360;
	}
	if (walk_y) {
		angle_y += 1;
		if (angle_y > 360)
			angle_y -= 360;
	}
	if (shoot == 1) {
		extend -= 0.1;
	}
	else {
		extend=0;
	}
		
	
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}
void drawBall(double R, double x, double y, double z, int MODE) {
	glPushMatrix();
	glTranslated(x, y, z);
	if (MODE ==1) {
		glutSolidSphere(R, 20, 20);
	}
	else if (MODE == 2) {
		glutWireSphere(R, 20, 20);
	}
	glPopMatrix();
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

	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };  
	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 100.0 };     

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	GLfloat ambient[] = { 0.5, 0.8, 0.1, 0.1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { -80.0, 50.0, 25.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
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

	gluLookAt(-2.0, -5.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	//glTranslatef(0, 0, 10);
	glRotated(spinX, 0, 1, 0);
	glRotated(spinY, 1, 0, 0);
	glTranslated(0, 0, des);
	//body
	drawSkewed(5, 2, 2.5, 0, -0.75, 4, 1);
	drawSkewed(4, 3.5, 2, 0, -1.2, 5, 1);
	//head
	drawBall(2, 0, 1, 10, 1);
	//soulder
	drawBall(1.3, 3.5, -1.3, 10, 1);
	drawBall(1.3, -3.5, -1.3, 10, 1);
	//right-arm
	glPushMatrix();
	glTranslatef(3.5, -1.3, 10);
	if (shoot == 0) {
		glRotatef(-theta_z, 0, 0, 1);
		glRotatef(-theta_y, 0, 1, 0);
		glRotatef(-theta_x, 1, 0, 0);
	}
	else {
		glRotatef(0, 0, 0, 1);
		glRotatef(0, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
	}
	
	glTranslatef(-3.5, 1.3, -10);
	glTranslatef(3.5, -3.8+extend, 10);
	
	glScalef(1.3, 3.5, 1.3);
	glutSolidCube(1);
	glPopMatrix(); 
	//left-arm
	glPushMatrix();
	glTranslatef(-3.5, -1.3, 10);
	if (shoot == 0) {
		glRotatef(theta_z, 0, 0, 1);
		glRotatef(theta_y, 0, 1, 0);
		glRotatef(theta_x, 1, 0, 0);
	}
	else {
		glRotatef(0, 0, 0, 1);
		glRotatef(0, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
	}
	glTranslatef(3.5, 1.3, -10);
	glTranslatef(-3.5, -3.8+extend, 10);
	glScalef(1.3, 3.5, 1.3);
	glutSolidCube(1);
	glPopMatrix();

	//left-hand
	glPushMatrix();
	glTranslatef(-3.5, -1.3, 10);
	if (shoot == 0) {
		glRotatef(theta_z, 0, 0, 1);
		glRotatef(theta_y, 0, 1, 0);
		glRotatef(theta_x, 1, 0, 0);
	}
	else {
		glRotatef(0, 0, 0, 1);
		glRotatef(0, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
	}
	
	glTranslatef(3.5, 1.3, -10);
	drawBall(1.2, -3.5, -6.4 + extend, 10, 1);
	glPopMatrix();
	//right-hand
	glPushMatrix();
	glTranslatef(3.5, -1.3, 10);
	if (shoot == 0) {
		glRotatef(-theta_z, 0, 0, 1);
		glRotatef(-theta_y, 0, 1, 0);
		glRotatef(-theta_x, 1, 0, 0);
	}
	else {
		glRotatef(0, 0, 0, 1);
		glRotatef(0, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);
	}
	
	glTranslatef(-3.5, 1.3, -10);
	drawBall(1.2, 3.5, -6.4 + extend, 10, 1);
	glPopMatrix();

	//right leg
	glPushMatrix();
	glTranslatef(-1, -5.5, 10);
	glRotatef(angle_z, 0, 0, 1);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(angle_x, 1, 0, 0);
	glTranslatef(1, 5.5, -10);
	glTranslatef(-1, -7.0, 10);
	glScalef(1.2, 3.0, 2.0);
	glutSolidCube(1);
	glPopMatrix();
	// left leg
	glPushMatrix();
	glTranslatef(1, -5.5, 10);
	glRotatef(angle_z, 0, 0, 1);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(-angle_x, 1, 0, 0);
	glTranslatef(-1, 5.5, 10);
	glTranslatef(1, -7.0, -10);
	glScalef(1.2, 3.0, 2.0);
	glutSolidCube(1);
	glPopMatrix();
	//left feet
	glPushMatrix();
	glTranslatef(1, -5.5, 10);
	glRotatef(angle_z, 0, 0, 1);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(-angle_x, 1, 0, 0);
	glTranslatef(-1, 5.5, -10);
	glTranslatef(1.2, -9.0, 10);
	glScalef(2, 1, 3);
	glutSolidCube(1);
	glPopMatrix();
	//right feet
	glPushMatrix();
	glTranslatef(-1, -5.5, 10);
	glRotatef(angle_z, 0, 0, 1);
	glRotatef(angle_y, 0, 1, 0);
	glRotatef(angle_x, 1, 0, 0);
	glTranslatef(1, 5.5, -10);
	glTranslatef(-1.2, -9.0, 10);
	glScalef(2, 1, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}
void mouseClick(int btn, int state, int x, int y) {
	mx = x;
	my = y;
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

	case '4':
		move_x = !move_x;
		break;
	case '5':
		move_y = !move_y;
		break;
	case '6':
		move_z = !move_z;
		break;
	case '1':
		walk_x = !walk_x;
		break;
	case '2':
		walk_y = !walk_y;
		break;
	case '3':
		walk_z = !walk_z;
		break;
	case'o':
			shoot = !shoot;
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