/*
 *  simple2.c
 *  This program draws a white rectangle on a black background.
 */


#include <GL/glut.h>         /* glut.h includes gl.h and glu.h*/

void display(void)

{
/* clear window */

	 glClear(GL_COLOR_BUFFER_BIT); 


/* draw unit square polygon */

	glBegin(GL_POLYGON);
	 	glVertex2f(-0.5, -0.5);
	 	glVertex2f(-0.5, 0.5);
	 	glVertex2f(0.5, 0.5);
	 	glVertex2f(0.5, -0.5);
	glEnd();

/* flush GL buffers */

	glFlush(); 

}


void init()
{

/* set clear color to black */

    glClearColor (0.0, 1.0, 0.0, 0.0); 
/* set fill  color to white */

    glColor3f(1.0, 0.0, 0.0); 

/* set up standard orthogonal view with clipping */
/* box as cube of side 2 centered at origin */
/* This is default view and these statement could be removed */

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-0.0, 1.0, -0.0, 1.0, -1.0, 1.0);  
}

int main(int argc, char** argv)
{

/* Initialize mode and open a window in upper left corner of screen */
/* Window title is name of program (arg[0]) */

/* 初始化，对命令行参数进行处理 */
 	glutInit(&argc,argv); 

/* 设定窗口的显示模式（缓存类型和颜色）为单缓冲和RGB颜色模型 */
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);  

/* 定义窗口宽度为300像素，高度为300像素 */
	glutInitWindowSize(400,400);

/* 定义窗口位置，距离屏幕左边界0像素，距离屏幕上边界0像素 */
	glutInitWindowPosition(100,300); 

/* 创建一个名为“simple”的窗口 */
	glutCreateWindow("我的程序"); 

/* 设置当前窗口的显示回调函数 */
	glutDisplayFunc(display);

/* 完成窗口初始化，设定绘图属性 */
	init();

/* 启动主GLUT事件处理循环 */
	glutMainLoop();

	return 0;
}