/* This program illustrates the use of the glut library for
interfacing with a Window System */

/* The program opens a window, clears it to black,
then draws a box at the location of the mouse each time the
left button is clicked. The right button exits the program

The program also reacts correctly when the window is
moved or resized by clearing the new window to black*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

/* globals */

GLsizei wh = 300, ww = 300; /* initial window size */
GLfloat size = 5.0;   /* half side length of square */
int x1, y1, x2, y2;
int isFirst = 1;


void drawSquare(int x, int y)
{
	  y=wh-y;
	  glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
	  glClear(GL_COLOR_BUFFER_BIT);
	  glBegin(GL_POLYGON);
	 	   glVertex2f(x+size, y+size);
		    glVertex2f(x-size, y+size);
	 	   glVertex2f(x-size, y-size);
	    	glVertex2f(x+size, y-size);
	  glEnd();
	  glFlush();
}

void drawRectangle(int x1, int y1, int x2, int y2) {
	y1 = wh - y1;
	y2 = wh - y2;
	glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
	  glClear(GL_COLOR_BUFFER_BIT);
	  glBegin(GL_QUADS);
	 	   glVertex2i(x1, y1);
		    glVertex2i(x2, y1);
	 	   glVertex2i(x2, y2);
	    	glVertex2i(x1, y2);
	  glEnd();
	  glFlush();
}

/* rehaping routine called whenever window is resized
or moved */

void myReshape(GLsizei w, GLsizei h)
{
  /* adjust clipping box */

   	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity(); 
   	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity(); 

/* adjust viewport and clear */

	glViewport(0,0,w,h);
    glClearColor (0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
   

/* set global size for use by drawing routine */

    ww = w;
   	wh = h; 
}

void myinit(void)
{

	   /* set clear color to black */

    glClearColor (0.0, 0.0, 0.0, 1.0); 


}



/* display callback required by GLUT 3.0 */

void display() {
		int ty1 = wh - y1;
		int ty2 = wh - y2;
		glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_QUADS);
			glVertex2i(x1, ty1);
			glVertex2i(x2, ty1);
			glVertex2i(x2, ty2);
			glVertex2i(x1, ty2);
		glEnd();
		glFlush();
}

void mouse(int btn, int state, int x, int y)
{
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)   
		exit(0);
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!isFirst) {
			x2 = x;
			y2 = y;
			isFirst = 1;
			glutPostRedisplay();
		} else {
			x1 = x;
			y1 = y;
			isFirst = 0;
		}
	}
}



void keyboard(unsigned char key, int x, int y) {
	if (glutGetModifiers() == GLUT_ACTIVE_ALT 
			&&  (key == 'c' || key == 'C')) 	
		exit(0);
}


int main(int argc, char** argv)
{

   	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(300,300);
	glutInitWindowPosition(0,0); 
	glutCreateWindow("square");
//	
//	glutMotionFunc(drawSquare);
//	glutPassiveMotionFunc(drawSquare);
   	glutReshapeFunc (myReshape);
   	glutMouseFunc (mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

   	myinit ();
    glutMainLoop();


}
