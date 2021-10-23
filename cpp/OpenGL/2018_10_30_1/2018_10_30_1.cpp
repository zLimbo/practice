// 2018_10_30_1.cpp : Defines the entry point for the console application.
//

#include <GL/glut.h>

void init() { }
void display() { }

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutDisplayFunc(display);

	init();

	glutMainLoop();

	return 0;
}

