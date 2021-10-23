// 20181016_OpenGL_1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <GL/glut.h>

void display() {
	
	glClear(GL_COLOR_BUFFER_BIT); 

	glBegin(GL_POLYGON);
		glVertex2f(-0.5,  -0.5);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, -0.5);
	glEnd();

	glFlush();
}

void init() {

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("simple");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}