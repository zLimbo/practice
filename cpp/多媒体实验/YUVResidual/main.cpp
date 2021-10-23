#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <cmath>

const char* FILE_NAME = "foreman_qcif.yuv";
const char* FILE2_NAME = "foreman_qcif2.yuv";
const char* FILE3_NAME = "foreman_qcif3.yuv";
const char* FILE4_NAME = "foreman_qcif4.yuv";

const int FRAME_LEN = 176 * 144 * 3 / 2;
const int UV_POS = 176 * 144;
const int UV_LEN = 176 * 144 / 2;
const int N = 8;
const int HUNTING_ZONE = 88;
const int HEIGHT = 144;
const int WIDTH = 176;

FILE* fp = NULL;
FILE* fp2 = NULL;
FILE* fp3 = NULL;
FILE* fp4 = NULL;

unsigned char* pBuf1;
unsigned char* pBuf2;
unsigned char* pBuf3;
unsigned char* pBuf4;


inline int getPos(int x, int y) { return x + y * WIDTH; }


// 求 8 x 8 区域的差的平方 
int diff(int x, int y, int x0, int y0) {
	int val = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int v1 = pBuf2[getPos(x+i, y+j)];
			int v2 = pBuf1[getPos(x0+i, y0+j)];
			val += (v1 - v2) * (v1 - v2);
		}
	} 
	return val;
}


// 对 x,y开始的 8x8 区域进行预测 
void predict(int x, int y)  {
	
	int minVal = INT_MAX;
	int minX, minY;
	 
	for (int x0 = x - HUNTING_ZONE; x0 <= x + HUNTING_ZONE; ++x0) {
		for (int y0 = y - HUNTING_ZONE; y0 <= y + HUNTING_ZONE; ++y0) {
			if (x0 < 0 || y0 < 0 || x0 >= WIDTH - N || y0 >= HEIGHT - N) continue;
			int val = diff(x, y, x0, y0);
			if (val < minVal) {
				minVal = val;
				minX = x0;
				minY = y0;
			}
		}
	}
	
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int pos1 = getPos(x+i, y+j);
			int pos2 = getPos(minX+i, minY+j);
			pBuf3[pos1] = pBuf1[pos2];		// 预测帧 
			pBuf4[pos1] = abs(pBuf2[pos1] - pBuf3[pos1]);	// 残差帧 
		}
	}
	
}


// 总体预测 
void predictAll() {
	for (int r = 0; r < WIDTH; r+=8) 
		for (int c = 0; c < HEIGHT; c+=8) 
			predict(r, c);
}


double variance(unsigned char* pBuf) {
	int sum = 0;
	for (int r = 0; r < WIDTH; ++r) 
		for (int c = 0; c < HEIGHT; ++c) 
			sum += pBuf[getPos(r, c)];
	double avg = (double)sum / (WIDTH * HEIGHT);
	double var = 0;
	for (int r = 0; r < WIDTH; ++r) 
		for (int c = 0; c < HEIGHT; ++c) 
			var += (pBuf[getPos(r, c)] - avg) * (pBuf[getPos(r, c)] - avg);
	var = sqrt(var / (WIDTH * HEIGHT));
	return var;
} 

int main(int argc, char** argv) {
	
	if ((fp = fopen(FILE_NAME, "rb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	} 
	if ((fp2 = fopen(FILE2_NAME, "wb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	} 
	if ((fp3 = fopen(FILE3_NAME, "wb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	} 
	if ((fp4 = fopen(FILE4_NAME, "wb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	}
	
	pBuf1 = (unsigned char*)malloc(FRAME_LEN);
	pBuf2 = (unsigned char*)malloc(FRAME_LEN);
	pBuf3 = (unsigned char*)malloc(FRAME_LEN);
	pBuf4 = (unsigned char*)malloc(FRAME_LEN);
	
	fread(pBuf1, 1, FRAME_LEN, fp);
	fread(pBuf2, 1, FRAME_LEN, fp);
	
	predictAll();
	
	memset(pBuf2 + UV_POS, 128, UV_LEN);
	memset(pBuf3 + UV_POS, 128, UV_LEN);
	memset(pBuf4 + UV_POS, 128, UV_LEN);
	
	fwrite(pBuf2, 1, FRAME_LEN, fp2);	// pBuf2 帧 
	fwrite(pBuf3, 1, FRAME_LEN, fp3);	// 预测图形 
	fwrite(pBuf4, 1, FRAME_LEN, fp4);	// 残差 
	
	double var4 = variance(pBuf4);
	double var2 = variance(pBuf2);
	printf("搜索范围 : %d x %d\n", 2 * HUNTING_ZONE, 2 * HUNTING_ZONE); 
	printf("残差能量(方差) = %.2f\n", var4);
	printf("原图能量(方差) = %.2f\n", var2);
	printf("能量比率 = %.2f\n",  var4 / var2);
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	
	free(pBuf1);
	free(pBuf2);
	free(pBuf3);
	free(pBuf4); 
	
	return 0;
}
