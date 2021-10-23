#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#pragma comment(lib, "winmm.lib")
#define WIDTH 640      
#define HIGH 480
#define GAME_WIDTH 15    //游戏区横列格子数
#define GAME_HIGH 28   //游戏区竖排格子数
#define BOX_WIDTH 15     //一个格子的宽度
#define BOX_HIGH 15     //一个格子的长度

//函数声明
void startup();  //初始化
void show();     //图像显示
void gameover();  //游戏结束设置
void fourKinds(int four[4][4], int kind); // 新的形状图形
void updateWithInput();   //无输入更新
void updateWithoutInput();  //有输入更新
void transformation();  //变形
void startMenu();    //开始菜单
void pauseStart();    //暂停菜单
void playMusicOnce(CHAR *musicName);  //播放音效一次
void readRecord();  //读档
void writeRecord();  //存档

					 //全局变量
int GAME_CONDITION = 1; // 游戏状态，1为开始菜单， 0为退出， 2 为正式游戏

int board[GAME_WIDTH + 2][GAME_HIGH + 2];  //游戏区
int nowFour[4][4];       //现在图形
int nextFour[4][4];     // 下一个图形
int nowKind;            //现在图形种类
int nextKind;          //下一个图形种类
int four_x, four_y;  //图形位置
int vY;
COLORREF color;        //图形颜色
int pre_x, pre_y;   //预告位置
int score;           //分数（整形形式）
int levelUp;    //等级
int beginWidth, beginHigh;  //游戏区开始
bool isFail;   //游戏失败
bool isPause;  //游戏暂停
bool isMedi;  //游戏中断
bool isArchive;  //存档成功
bool isMusic;  //音乐
IMAGE img_bk;   //背景图片
MOUSEMSG m;    //鼠标控制
int mouse_x, mouse_y;
int choice;  //鼠标选择
int i, j, k;

			 //函数定义
			 // 播放音效一次
void playMusicOnce(CHAR * musicName) {
	CHAR playMusic[50] = "open ";
	strcat(playMusic, musicName);
	strcat(playMusic, " alias tempMusic");
	mciSendString("close tempMusic", NULL, 0, NULL);
	mciSendString(playMusic, NULL, 0, NULL);
	mciSendString("play tempMusic", NULL, 0, NULL);
}

//读档
void readRecord() {
	startup();
	//背景音乐
	mciSendString("close bkMusic1", NULL, 0, NULL);
	mciSendString("open .\\music\\bk2.mp3 alias bkMusic2", NULL, 0, NULL);
	mciSendString("play bkMusic2 repeat", NULL, 0, NULL);
	FILE *fp;
	fp = fopen( "record.txt", "r");
	int n;
	fscanf(fp, "%d", &n);
	if (n == -1) {
		fscanf(fp, "%d %d %d %d %d %d %d %d", &score, &levelUp, &isFail, &isMusic, &four_x, &four_y, &nowKind, &nextKind);
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				fscanf(fp, "%d ", &nowFour[i][j]);
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
				fscanf(fp, "%d ", &nextFour[i][j]);
		for (i = 1; i <= GAME_WIDTH; i++)
			for (j = 1; j <= GAME_HIGH; j++)
				fscanf(fp, "%d ", &board[i][j]);
		GAME_CONDITION = 2;
	}
	fclose(fp);
	if(!isMusic) mciSendString("pause bkMusic2", NULL, 0, NULL);
}

//存档
void writeRecord() {
	FILE * fp;
	fp = fopen("record.txt", "w");
	fprintf(fp, "%d\n\n", -1);
	fprintf(fp, "%d %d %d %d %d %d %d %d\n\n", score, levelUp, isFail, isMusic, four_x, four_y, nowKind, nextKind);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			fprintf(fp, "%d ", nowFour[i][j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			fprintf(fp, "%d ", nextFour[i][j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for (i = 1; i <= GAME_WIDTH; i++) {
		for (j = 1; j <= GAME_HIGH; j++)
			fprintf(fp, "%d ", board[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
	isArchive = true;
}

//开始菜单
void startMenu() {
	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);

	settextcolor(GREEN);
	settextstyle(64, 0, _T("华文琥珀"));
	outtextxy(WIDTH * 0.25, HIGH * 0.08, _T("俄罗斯方块"));
	setcolor(WHITE);
	settextstyle(16, 0, _T("楷体"));
	outtextxy(WIDTH * 0.42, HIGH * 0.25, _T("作者：Dora ^-^"));
	/*setcolor(BLUE);
	setfillcolor(RGB(200,200,200));
	fillrectangle(WIDTH * 0.39, HIGH * 0.33, WIDTH * 0.61, HIGH * 0.43);
	fillrectangle(WIDTH * 0.39, HIGH * 0.48, WIDTH * 0.61, HIGH * 0.58);
	fillrectangle(WIDTH * 0.39, HIGH * 0.63, WIDTH * 0.61, HIGH * 0.73);
	fillrectangle(WIDTH * 0.39, HIGH * 0.78, WIDTH * 0.61, HIGH * 0.88);*/

	if (MouseHit()) {
		m = GetMouseMsg();   // 获取鼠标信息
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.uMsg == WM_LBUTTONUP) {
			if (choice == 1) {
				GAME_CONDITION = 2;
				playMusicOnce(".\\music\\mouse.wav");
				Sleep(500);
				mciSendString("close bkMusic1", NULL, 0, NULL);
				mciSendString("open .\\music\\bk2.mp3 alias bkMusic2", NULL, 0, NULL);
				if (isMusic) {
					if (isMedi) mciSendString("resume bkMusic2", NULL, 0, NULL);
					else mciSendString("play bkMusic2 repeat", NULL, 0, NULL);
				}
			}
			else if (choice == 2) { readRecord(); playMusicOnce(".\\music\\mouse.wav"); isPause = true; }
			else if (choice == 3) { writeRecord(); playMusicOnce(".\\music\\mouse.wav"); }
			else if (choice == 4) { playMusicOnce(".\\music\\mouse.wav"); Sleep(100); gameover(); }
		}
	}

	choice = 0;
	settextcolor(WHITE);
	if (mouse_x >= WIDTH * 0.39 && mouse_x <= WIDTH * 0.61 && mouse_y >= HIGH * 0.33 && mouse_y <= HIGH * 0.43) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 1;
	}
	else settextstyle(32, 0, _T("楷体"));
	if (isMedi) outtextxy(WIDTH * 0.4, HIGH * 0.35, _T("继续游戏"));
	else outtextxy(WIDTH * 0.4, HIGH * 0.35, _T("开始游戏"));

	if (mouse_x >= WIDTH * 0.39 && mouse_x <= WIDTH * 0.61 && mouse_y >= HIGH * 0.48 && mouse_y <= HIGH * 0.58) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 2;
	}
	else 	settextstyle(32, 0, _T("楷体"));
	outtextxy(WIDTH * 0.4, HIGH * 0.50, _T("载入进度"));

	if (mouse_x >= WIDTH * 0.39 && mouse_x <= WIDTH * 0.61 && mouse_y >= HIGH * 0.63 && mouse_y <= HIGH * 0.73) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 3;
	}
	else { settextstyle(32, 0, _T("楷体")); isArchive = false; }
	if (isArchive) outtextxy(WIDTH * 0.4, HIGH * 0.65, _T("保存成功"));
	else outtextxy(WIDTH * 0.4, HIGH * 0.65, _T("保存进度"));

	if (mouse_x >= WIDTH * 0.39 && mouse_x <= WIDTH * 0.61 && mouse_y >= HIGH * 0.78 && mouse_y <= HIGH * 0.88) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 4;
	}
	else settextstyle(32, 0, _T("楷体"));
	outtextxy(WIDTH * 0.45, HIGH * 0.80, _T("退出"));

	FlushBatchDraw();
	clearrectangle(0, 0, WIDTH - 1, HIGH - 1);
}

// 获取新的形状图形，各种图形在 4 X 4 表格中的显示
void fourKinds(int four[4][4], int kind) {
	if (kind == 0) { int temp[4][4] = { { 0,1,0,0 },{ 0,1,0,0 },{ 0,1,0,0 },{ 0,1,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 1) { int temp[4][4] = { { 0,0,0,0 },{ 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 2) { int temp[4][4] = { { 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 3) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 1,1,1,1 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 10) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 11) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 12) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 13) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 20) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,0,1,0 },{ 0,0,1,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 21) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,1 },{ 0,1,0,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 22) { int temp[4][4] = { { 0,1,0,0 },{ 0,1,0,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 23) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,1,0 },{ 1,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 30) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,1,0,0 },{ 0,1,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 31) { int temp[4][4] = { { 0,0,0,0 },{ 1,1,1,0 },{ 0,0,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 32) { int temp[4][4] = { { 0,0,1,0 },{ 0,0,1,0 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 33) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,0,0 },{ 0,1,1,1 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 40) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,0,0 },{ 1,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 41) { int temp[4][4] = { { 0,0,1,0 },{ 0,1,1,0 },{ 0,0,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 42) { int temp[4][4] = { { 1,1,1,0 },{ 0,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 43) { int temp[4][4] = { { 1,0,0,0 },{ 1,1,0,0 },{ 1,0,0,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 50) { int temp[4][4] = { { 0,1,0,0 },{ 0,1,1,0 },{ 0,0,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 51) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,1,1 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 52) { int temp[4][4] = { { 0,1,0,0 },{ 0,1,1,0 },{ 0,0,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 53) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,1,1 },{ 0,1,1,0 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }

	else if (kind == 60) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,1,0 },{ 0,1,1,0 },{ 0,1,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 61) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,0,1,1 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 62) { int temp[4][4] = { { 0,0,0,0 },{ 0,0,1,0 },{ 0,1,1,0 },{ 0,1,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
	else if (kind == 63) { int temp[4][4] = { { 0,0,0,0 },{ 0,1,1,0 },{ 0,0,1,1 },{ 0,0,0,0 } }; memcpy(four, temp, sizeof(int) * 16); }
}

//变形
void transformation() {
	int tempKind = nowKind;
	int tempFour[4][4];
	bool isTrans = true;

	if (tempKind % 10 == 3) tempKind = (tempKind / 10) * 10;
	else tempKind++;

	fourKinds(tempFour, tempKind);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (tempFour[i][j] == 1) {
				int trans_i = four_x + i;
				int trans_j = four_y + j;
				if (board[trans_i][trans_j] == 2 || board[trans_i][trans_j] == 3) {
					isTrans = false;
					break;
				}
			}
		}
		if (!isTrans) break;
	}

	if (isTrans) {
		memcpy(nowFour, tempFour, sizeof(int) * 16);
		nowKind = tempKind;
		playMusicOnce(".\\music\\trans.wav");
	}
}

//初始化
void startup() {
	memset(board, 0, sizeof(board));   //游戏区数组初始化为0
	beginWidth = WIDTH / 6;           //游戏区位置
	beginHigh = HIGH / 18;
	score = 0;               //分数
	levelUp = 1;

	srand((unsigned)time(NULL));

	nowKind = (rand() % 7) * 10 + rand() % 4;
	fourKinds(nowFour, nowKind);          //随机开始图形
	nextKind = (rand() % 7) * 10 + rand() % 4;
	fourKinds(nextFour, nextKind);        //随机下一个图形

	four_x = rand() % (GAME_WIDTH - 6) + 2;   //随机图形区初始位置
	four_y = -3;
	vY = 0;
	color = RGB(rand() % 255, rand() % 255, rand() % 255);

	pre_x = beginWidth + (GAME_WIDTH + 4) * BOX_WIDTH;  //"下一个"区域位置
	pre_y = beginHigh + BOX_WIDTH;

	isFail = false;   //失败
	isPause = false;   //暂停
	isMedi = false;    // 中断
	isArchive = false;  //存档
	isMusic = true;  //音乐

	loadimage(&img_bk, ".\\picture\\bk.jpg");  //载入图片

	mouse_x = 0;  //初始鼠标位置为（0，0）
	mouse_y = 0;

	choice = 0;

	initgraph(WIDTH, HIGH);
	BeginBatchDraw();

	mciSendString("open .\\music\\bk1.mp3 alias bkMusic1", NULL, 0, NULL);
	mciSendString("play bkMusic1 repeat", NULL, 0, NULL);
}

//显示
void show() {
	while (GAME_CONDITION == 1) startMenu(); // 进入开始菜单

	putimage(0, 0, &img_bk);
	setbkmode(TRANSPARENT);

	//游戏区显示
	for (i = 0; i < GAME_WIDTH + 2; i++) {
		for (j = 0; j < GAME_HIGH + 2; j++) {
			int begin_i = i * BOX_WIDTH + beginWidth;
			int end_i = (i + 1) * BOX_WIDTH + beginWidth;
			int begin_j = j * BOX_HIGH + beginHigh;
			int end_j = (j + 1) * BOX_HIGH + beginHigh;

			if (i == 0 || i == GAME_WIDTH + 1) {    //左右边界
				board[i][j] = 2;   //左右边界为2
				setcolor(WHITE);
				setfillcolor(GREEN);
				fillrectangle(begin_i - 1, begin_j, end_i - 1, end_j);
			}
			else if (j == 0) {    //上边界
				board[i][j] = 0;    //上边界为4
				setcolor(WHITE);
				setfillcolor(GREEN);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
			else if (j == GAME_HIGH + 1) {   //下边界
				board[i][j] = 3;    //下边界为3
				setcolor(WHITE);
				setfillcolor(GREEN);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
			else if (board[i][j] == 3) {
				setcolor(BLUE);             //沉积区
				setfillcolor(RED);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
			else if (board[i][j] == 1) {             //实区
				setcolor(BLUE);
				setfillcolor(WHITE);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
			else if (board[i][j] == 0) {       //空区
				setcolor(RGB(50, 50, 50));
				setfillcolor(BLACK);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
		}
	}

	// 游戏说明
	settextcolor(WHITE);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(20, 40, _T("W  变形"));
	outtextxy(20, 80, _T("A  左移"));
	outtextxy(20, 120, _T("D  右移"));
	outtextxy(20, 160, _T("S  坠下"));

	//下一个图形预知
	settextcolor(WHITE);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(pre_x, pre_y, _T("下一个:"));

	for (i = 0; i < 4; i++) {             //4 X 4 表格
		for (j = 0; j < 4; j++) {
			if (nextFour[i][j] == 1) {
				int begin_i = i * BOX_WIDTH + pre_x + 100;
				int end_i = (i + 1) * BOX_WIDTH + pre_x + 100;
				int begin_j = j * BOX_HIGH + pre_y;
				int end_j = (j + 1) * BOX_HIGH + pre_y;
				setcolor(BLUE);
				setfillcolor(WHITE);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
		}
	}

	//得分
	settextcolor(WHITE);
	settextstyle(20, 0, _T("黑体"));
	int score_x = pre_x;
	int score_y = pre_y + BOX_HIGH * 8;
	outtextxy(score_x, score_y, _T("得分:"));
	char scoreC[10];   //分数（字符形式）
	sprintf(scoreC, "%5d", score);
	outtextxy(score_x + 60, score_y, scoreC);
	//等级
	int level_x = pre_x;
	int level_y = score_y + BOX_HIGH * 4;
	outtextxy(level_x, level_y, _T("等级："));
	char levelC[10];
	sprintf(levelC, "%5d", levelUp);
	outtextxy(level_x + 60, level_y, levelC);

	//nowFour[4][4]当前图形显示

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (nowFour[i][j] == 1 && four_y + j > 0) {
				int begin_i = (four_x + i) * BOX_WIDTH + beginWidth;
				int end_i = (four_x + i + 1) * BOX_WIDTH + beginWidth;
				int begin_j = (four_y + j) * BOX_HIGH + beginHigh;
				int end_j = (four_y + j + 1) * BOX_HIGH + beginHigh;
				setcolor(BLUE);

				setfillcolor(color);
				fillrectangle(begin_i, begin_j, end_i, end_j);
			}
		}
	}

	//失败显示
	if (isFail) {
		settextcolor(BLUE);
		settextstyle(64, 0, _T("黑体"));
		outtextxy(WIDTH / 4, HIGH / 3, _T("Game over!"));
		FlushBatchDraw();
		Sleep(8000);
		isFail = 0;
		GAME_CONDITION = 1;
		startup();
	}

	//"继续"与"返回主菜单"
	/*setcolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(pre_x + 80, pre_y + 280, pre_x + 160, pre_y + 320);
	fillrectangle(pre_x + 30, pre_y + 360, pre_x + 210, pre_y + 400);*/

	choice = 0;
	settextcolor(WHITE);
	if (mouse_x >= pre_x + 80 && mouse_x <= pre_x + 160 && mouse_y >= pre_y + 280 && mouse_y <= pre_y + 320) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 1;
	}
	else settextstyle(32, 0, _T("楷体"));
	if (isPause) outtextxy(pre_x + 88, pre_y + 285, _T("继续"));
	else outtextxy(pre_x + 88, pre_y + 285, _T("暂停"));

	if (mouse_x >= pre_x + 30 && mouse_x <= pre_x + 210 && mouse_y >= pre_y + 360 && mouse_y <= pre_y + 400) {
		settextstyle(32, 0, _T("华文彩云"));
		choice = 2;
	}
	else 	settextstyle(32, 0, _T("楷体"));
	outtextxy(pre_x + 38, pre_y + 365, _T("返回主菜单"));

	//关闭音乐
	/*setcolor(BLACK);
	setfillcolor(BLACK);
	fillrectangle(5, 235, 90, 265);*/
	settextcolor(WHITE);
	if (mouse_x >= 10 && mouse_x <= 235 && mouse_y >= 90 && mouse_y <= 265) {
		settextstyle(20, 0, _T("华文彩云"));
		choice = 3;
	}
	else settextstyle(20, 0, _T("楷体"));
	if (isMusic)  outtextxy(10, 240, _T("音乐：开"));
	else outtextxy(10, 240, _T("音乐：关"));

	FlushBatchDraw();   //清除页面
	int time = min((150 - levelUp * 5), 40);
	Sleep(time);
	clearrectangle(0, 0, WIDTH - 1, HIGH - 1);
}

//无输入更新
void updateWithoutInput() {
	bool isHit = false;    //判断是否沉积
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (nowFour[i][j] == 1) {
				int next_i = four_x + i;
				int next_j = four_y + j + 1;  //实形下为实形则冻结为沉积区
				if (board[next_i][next_j] == 3 && four_y + j > 0) {
					isHit = true;  //沉积
					break;
				}
			}
		}
		if (isHit) break;
	}

	//沉积
	if (isHit) {
		playMusicOnce(".\\music\\down.wav");
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (nowFour[i][j] == 1) {
					board[four_x + i][four_y + j] = 3;
				}
			}
		}

		memcpy(nowFour, nextFour, sizeof(int) * 16);  //预告图形变为现时图形
		nowKind = nextKind;

		//擦去预告图形
		for (i = 0; i < 4; i++) {             //4 X 4 表格
			for (j = 0; j < 4; j++) {
				if (nextFour[i][j] == 1) {
					int begin_i = i * BOX_WIDTH + pre_x + 100;
					int end_i = (i + 1) * BOX_WIDTH + pre_x + 100;
					int begin_j = j * BOX_HIGH + pre_y;
					int end_j = (j + 1) * BOX_HIGH + pre_y;
					setcolor(BLACK);
					setfillcolor(BLACK);
					fillrectangle(begin_i, begin_j, end_i, end_j);
				}
			}
		}
		nextKind = (rand() % 7) * 10 + rand() % 4;
		fourKinds(nextFour, nextKind);   //随机预告新图形
		four_x = rand() % (GAME_WIDTH - 6) + 2;   //随机图形区初始位置
		four_y = -3;
		color = RGB(rand() % 255, rand() % 255, rand() % 255);
		isHit = false;
	}

	//消去满行,分数增加
	for (j = 1; j <= GAME_HIGH; j++) {
		int tempSum = 0;
		for (i = 1; i <= GAME_WIDTH; i++)
			tempSum += board[i][j];
		if (tempSum == 3 * GAME_WIDTH) {
			score++;
			playMusicOnce(".\\music\\full.wav");
			for (k = j; k > 0; k--)
				for (i = 1; i <= GAME_WIDTH; i++)
					board[i][k] = board[i][k - 1];
		}
	}

	//根据分数计算等级
	int temp = score / 5 + 1;
	if (temp - levelUp == 1) {
		playMusicOnce(".\\music\\level_up.wav");
		levelUp++;
	}

	//堆满失败
	for (i = 1; i <= GAME_WIDTH; i++)
		if (board[i][0] == 3) {
			isFail = true;
			mciSendString("close bkMusic2", NULL, 0, NULL);
			playMusicOnce(".\\music\\fail2.wav");
		}
	// 图形向下移动
	if (vY == 5) {
		four_y += 1;
		vY = 0;
	}
	else vY++;
}

//有输入更新
void updateWithInput() {
	CHAR input;
	if (_kbhit()) {  //有移动操作输入
		bool isLeftBar = false;    //判断是否为边界
		bool isRightBar = false;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (nowFour[i][j] == 1) {
					int left_i = four_x + i - 1;  //左
					int left_j = four_y + j;
					int right_i = four_x + i + 1;  //右
					int right_j = four_y + j;
					if ((board[left_i][left_j] == 2 || board[left_i][left_j] == 3) && four_y + j > 0) {
						isLeftBar = true;  //左有实物
					}
					if ((board[right_i][right_j] == 2 || board[right_i][right_j] == 3) && four_y + j > 0) {
						isRightBar = true;  //右有实物
					}
				}
			}
		}
		input = tolower(_getch());

		if (input == 'a' && !isLeftBar) four_x--;   //左移
		else if (input == 'd' && !isRightBar) four_x++;  //右移
		else if (input == 's') while (four_y != -2) updateWithoutInput();   //一落到地
		else if (input == 'w') transformation();   // 变形
	}

	while (MouseHit()) {  //有鼠标操作输入
		m = GetMouseMsg();  //获得鼠标信息
		mouse_x = m.x;
		mouse_y = m.y;
		if (m.uMsg == WM_LBUTTONUP) {   //左键按下
			if (choice == 2) {
				GAME_CONDITION = 1;   //中断，返回主菜单
				isMedi = true;
				playMusicOnce(".\\music\\mouse.wav");
				mciSendString("pause bkmusic2", NULL, 0, NULL);
			}
			else if (choice == 3) {
				if (isMusic) {  //音乐开关
					mciSendString("pause bkMusic2", NULL, 0, NULL);
					isMusic = false;
				}
				else {
					mciSendString("resume bkMusic2", NULL, 0, NULL);
					isMusic = true;
				}
			}
			if (choice == 1) {    //暂停
				isPause = true;
				playMusicOnce(".\\music\\pause.wav");
			}
		}
	}
	while (isPause) {    //暂停状态，鼠标按"继续"继续游戏
		show();
		while (MouseHit()) {
			m = GetMouseMsg();
			mouse_x = m.x;
			mouse_y = m.y;
			if (m.uMsg == WM_LBUTTONUP) {
				if (choice == 2) {     //中断，返回主菜单
					GAME_CONDITION = 1;
					isMedi = true;
					playMusicOnce(".\\music\\mouse.wav");
					mciSendString("pause bkmusic2", NULL, 0, NULL);
				}
				else if (choice == 3) {
					if (isMusic) {  //音乐开关
						mciSendString("pause bkMusic2", NULL, 0, NULL);
						isMusic = false;
					}
					else {
						mciSendString("resume bkMusic2", NULL, 0, NULL);
						isMusic = true;
					}
				}
				else if (choice == 1) {
					isPause = false;
					playMusicOnce(".\\music\\pause.wav");
				}
			}
		}
	}
}

//游戏结束设置
void gameover() {
	EndBatchDraw();
	closegraph();
	exit(0);
}

//主函数
int main() {
	startup();
	for (;;) {
		show();
		updateWithoutInput();
		updateWithInput();
	}
	gameover();
	return 0;
}
