#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cmath>

#define WINDOW_WIDTH		500
#define WINDOW_HEIGHT		500
#define WINDOW_POS_X		400
#define WINDOW_POS_Y		100

#define OPTION_LINE			00
#define OPTION_TRIANGLE		01
#define	OPTION_RECTANGLE	02
#define OPTION_HEXAGON		03

#define OPTION_RED		10
#define OPTION_GREEN	11
#define OPTION_BLUE		12
#define OPTION_WHITE	13

#define OPTION_ADD		20
#define	OPTION_DEC		21	

#define OPTION_MOVE_UP		30
#define OPTION_MOVE_DOWN	31
#define OPTION_MOVE_LEFT	32
#define OPTION_MOVE_RIGHT	33	

#define OPTION_CLOCKWISE		40
#define OPTION_ANTICLOCKWISE	41

#define DEGREES_TO_RADIANS (3.1415926 / 180.0f);

int TriangleCount = 0;

GLfloat theta = 0.0f;
GLfloat Red = 1.0f;
GLfloat Green = 1.0f;
GLfloat Blue = 1.0f;
GLfloat Size = 1.0f;
GLfloat X1 = -0.5, Y1 = -0.5, X2 = 0.5, Y2 = 0.5;
GLfloat X3 = -0.5, Y3 = 0.5;

void (*drawGraph)();	// 函数指针

/**画六边形
 *
 */
void drawHexagon() {
	int t = 0;
	glBegin(GL_POLYGON); 
	while (t < 6) {  // 六边
		GLfloat radian = (theta+t*60) * DEGREES_TO_RADIANS;
		glVertex2f( cos(radian)*Size, sin(radian)*Size );
		++t;
	}
	glEnd();
}

/**画线
 *
 */
void drawLine() {
	glBegin(GL_LINES);
		glVertex2f(X1*Size, Y1*Size);
		glVertex2f(X2*Size, Y2*Size);
	glEnd();
}

/**画三角形
 *
 */
void drawTriangle() {
	glBegin(GL_TRIANGLES);
		glVertex2f(X1*Size, Y1*Size);
		glVertex2f(X2*Size, Y2*Size);
		glVertex2f(X3*Size, Y3*Size);
	glEnd();
}

/**画矩形
 *
 */
void drawRectangle() {
	glBegin(GL_QUADS);
		glVertex2f(X1*Size, Y1*Size);
		glVertex2f(X2*Size, Y1*Size);
		glVertex2f(X2*Size, Y2*Size);
		glVertex2f(X1*Size, Y2*Size);
	glEnd();
}

/**显示函数
 *
 */
void myDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT);
	(*drawGraph)();
	glutSwapBuffers ();
}


/**图形菜单事件
 *
 */
void pocessGraphMenuEvent(int graph_) {
	switch (graph_) {
	case OPTION_LINE:			// 画线
		drawGraph = &drawLine; break;
	case OPTION_TRIANGLE:		// 画三角线
		drawGraph = &drawTriangle; break;
	case OPTION_RECTANGLE:		// 画矩形
		drawGraph = &drawRectangle; break;
	case OPTION_HEXAGON:
		drawGraph = &drawHexagon;
	default: break;
	}
	if (drawGraph == &drawTriangle) {
		TriangleCount = 1;
	} else {
		TriangleCount = 0;
	}
	glLoadIdentity();
	glutPostRedisplay();		//立即重画
}

/**大小菜单事件
 *
 */
void pocessSizeMenuEvent(int size_) {
	if (size_ == OPTION_ADD) {
		Size += 0.1;
	} else if (size_ == OPTION_DEC && Size > 0.1) {
		Size -= 0.1;
	}
	glutPostRedisplay();		//立即重画
}

/**颜色菜单事件
 *
 */
void pocessColorMenuEvent(int color_) {
	switch (color_) {
	case OPTION_RED:			// 红色
		glColor3f(1.0f, 0.0f, 0.0f); break;
	case OPTION_GREEN:			// 绿色 
		glColor3f(0.0f, 1.0f, 0.0f); break;
	case OPTION_BLUE: 			// 蓝色
		glColor3f(0.0f, 0.0f, 1.0f); break;
	case OPTION_WHITE: 			// 白色
		glColor3f(1.0f, 1.0f, 1.0f); break;
	default: break;
	}
	glutPostRedisplay();		//立即重画
}

/**移动菜单事件
 *
 */
void pocessMoveMenuEvent(int move_) {
	switch (move_) {
	case OPTION_MOVE_UP:		//上移
		glTranslatef(0.0f, 0.1f, 0.0f); 
		break;
	case OPTION_MOVE_DOWN: 		//下移
		glTranslatef(0.0f, -0.1f, 0.0f); 
		break;
	case OPTION_MOVE_LEFT:		//左移
		glTranslatef(-0.1f, 00.0f, 0.0f); 
		break;
	case OPTION_MOVE_RIGHT:		//右移 
		glTranslatef(0.1f, 00.0f, 0.0f); 
		break;
	default: break;
	}
	glutPostRedisplay();		//立即重画
}

/**旋转菜单事件
 *
 */
void pocessRotateMenuEvent(int rotate_) {
	switch (rotate_) {
	case OPTION_CLOCKWISE:		// 顺时针
		glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
		break;
	case OPTION_ANTICLOCKWISE:	// 逆时针
		glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
		break;
	default: break;
	}
	glutPostRedisplay();
}

/**创建菜单
 *
 */
void createMyMenu() {
	// 菜单
	int mainMenu;
	int graphMenu;
	int colorMenu;
	int sizeMenu;
	int moveMenu;
	int rotateMenu;

	// 图形菜单
	graphMenu = glutCreateMenu(pocessGraphMenuEvent);
	glutAddMenuEntry("线段", OPTION_LINE);
	glutAddMenuEntry("三角形", OPTION_TRIANGLE);
	glutAddMenuEntry("矩形", OPTION_RECTANGLE);
	glutAddMenuEntry("六边形", OPTION_HEXAGON);


	// 颜色菜单
	colorMenu = glutCreateMenu(pocessColorMenuEvent);
	glutAddMenuEntry("红", OPTION_RED);
	glutAddMenuEntry("绿", OPTION_GREEN);
	glutAddMenuEntry("蓝", OPTION_BLUE);
	glutAddMenuEntry("白", OPTION_WHITE);

	// 大小菜单
	sizeMenu = glutCreateMenu(pocessSizeMenuEvent);
	glutAddMenuEntry("增大", OPTION_ADD);
	glutAddMenuEntry("减小", OPTION_DEC);

	// 移动菜单
	moveMenu = glutCreateMenu(pocessMoveMenuEvent);
	glutAddMenuEntry("上移", OPTION_MOVE_UP);
	glutAddMenuEntry("下移", OPTION_MOVE_DOWN);
	glutAddMenuEntry("左移", OPTION_MOVE_LEFT);
	glutAddMenuEntry("右移", OPTION_MOVE_RIGHT);

	// 旋转菜单
	rotateMenu = glutCreateMenu(pocessRotateMenuEvent);
	glutAddMenuEntry("顺时针", OPTION_CLOCKWISE);
	glutAddMenuEntry("逆时针", OPTION_ANTICLOCKWISE);
	
	// 主菜单
	mainMenu = glutCreateMenu(NULL);
	glutAddSubMenu("图形", graphMenu);
	glutAddSubMenu("颜色", colorMenu);
	glutAddSubMenu("大小", sizeMenu);
	glutAddSubMenu("移动", moveMenu);
	glutAddSubMenu("旋转", rotateMenu);

	// 右键触发
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void idle() {
	
}

/**鼠标事件响应
 *
 */
void myMouseFunc(int btn_, int state_, int x_, int y_) {
	if (btn_ == GLUT_LEFT_BUTTON) {
		if (TriangleCount == 0) {			// 非三角形
			if (state_ == GLUT_DOWN) {
				X1 = (GLfloat)x_ / WINDOW_WIDTH * 2 - 1.0f;
				Y1 = 1.0f - (GLfloat)y_ / WINDOW_HEIGHT * 2;
			}
			if (state_ == GLUT_UP) {
				X2 = (GLfloat)x_ / WINDOW_WIDTH * 2 - 1.0f;
				Y2 = 1.0f - (GLfloat)y_ / WINDOW_HEIGHT * 2;
				glutPostRedisplay();		//立即重画
			}
		} else if (state_ == GLUT_DOWN) {	// 三角形
			if (TriangleCount == 1) {
				X1 = (GLfloat)x_ / WINDOW_WIDTH * 2 - 1.0f;
				Y1 = 1.0f - (GLfloat)y_ / WINDOW_HEIGHT * 2;
				TriangleCount = 2;
			} else if (TriangleCount == 2) {	// 三角形的第二个顶多
				X2 = (GLfloat)x_ / WINDOW_WIDTH * 2 - 1.0f;
				Y2 = 1.0f - (GLfloat)y_ / WINDOW_HEIGHT * 2; 
				TriangleCount = 3;
			} else if (TriangleCount == 3) {	// 三角形的第三个顶点
				X3 = (GLfloat)x_ / WINDOW_WIDTH * 2 - 1.0f;
				Y3 = 1.0f - (GLfloat)y_ / WINDOW_HEIGHT * 2;
				glutPostRedisplay();			//立即重画
				TriangleCount = 1;
			}
		}
	}
}

/**初始化
 *
 */
void myInitial() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glShadeModel(GL_FLAT);
	drawGraph = &drawHexagon;
}

/**主函数
 *
 */
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// 窗口
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("MyDrawing");
	myInitial();

	glutDisplayFunc(&myDisplayFunc);
	glutMouseFunc(&myMouseFunc);
	createMyMenu();
	// 事件循环回调
	glutMainLoop();

	return 0;
}