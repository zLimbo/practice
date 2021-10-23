#include <windows.h>
#include <GL/glut.h>

void initial() {
	// 背景颜色
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void DrawTriangle() {
	glBegin(GL_TRIANGLES);
		glVertex2f(20.0, 20.0);
		glVertex2f(80.0, 30.0);
		glVertex2f(50.0, 70.0);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0); // 黑色
	DrawTriangle();
	
	/* 平移 */
	glColor3f(1.0, 0.0, 0.0); // 红色
	glTranslatef(40.0, 70.0, 0.0); //平移
	DrawTriangle();

	/* 旋转 */
	glColor3f(0.0, 1.0, 0.0); // 绿色
	glRotatef(30, 0.0, 0.0, 1.0); //旋转
	glTranslatef(80.0, 0.0, 0.0); // 平移
	DrawTriangle();
	glRotatef(-30, 0.0, 0.0, 1.0); //旋转回

	/* 比例 */
	glColor3f(0.0, 0.0, 1.0); // 蓝色
	glScalef(2.0, 2.0, 2.0);	//比例放大
	glTranslatef(0.0, 20.0, 0.0); //平移
	DrawTriangle();

	glFlush();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Triangle");
	initial();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}



/*
    3. 对顶点为V1V2V3的三角形进行
	平移、旋转、比例、错切等基本变换。
    三角形顶点的坐标为：
	V1(20.0,20.0)，V2(80.0,30.0)，V3(50.0,70.0)。
*/