#include <cstdio>
#include <cstdlib>
#include <memory.h>

const char* FILE_NAME = "test_480x320.yuv";
const char* FILE2_NAME = "out.yuv";
const int NO = 24;
const int FRAME_LEN = 480 * 320 * 3 / 2;
const int UV_POS = 480 * 320;
const int UV_LEN = 480 * 320 / 2;
const int HEIGHT = 320;
const int WIDTH = 480;

inline int getPos(int x, int y) { return x * WIDTH + y; }

// 统计个数 
void getHist(unsigned char* pBuf, int uchNum[]) {
	for (int i = 0; i < 256; ++i) uchNum[i] = 0;
	for (int i = 0; i < UV_POS; ++i) ++uchNum[ pBuf[i] ]; 
}


void histeq(unsigned char* pBuf, unsigned char* pBuf2) {
	
	int uchNum[256];
	
	getHist(pBuf, uchNum);
	
	// 总数 
	int sumNum = 0;
	for (int i = 0; i < 256; ++i) {
		sumNum += uchNum[i];
		printf("%d\t%d\n", i, uchNum[i]);
	}
	printf("\n\n");
	
	unsigned tk[256];
	
	int auxNum = 0;
	for (int i = 0; i < 256; ++i) {
		auxNum += uchNum[i];
		tk[i] = double(auxNum) / double(sumNum) * 256;
		printf("%d\t%d\n", i, tk[i]);
	}
	printf("\n\n");
	
	for (int i = 0; i < UV_POS; ++i) {
		pBuf2[i] = tk[pBuf[i]];
	}
	
	memcpy(pBuf2+UV_POS, pBuf+UV_POS, UV_LEN);
	
	getHist(pBuf2, uchNum);
	
	for (int i = 0; i < 256; ++i)
		printf("%d\t%d\n", i, uchNum[i]);
	
}


int main(int argc, char** argv) {
	
	freopen("out.txt", "w", stdout);
	
	FILE *fp, *fp2;
	if ((fp = fopen(FILE_NAME, "rb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	} 
	if ((fp2 = fopen(FILE2_NAME, "wb")) == NULL) {
		printf("open file error\n");
		exit(-1);
	}
	
	
	unsigned char* pBuf = (unsigned char*)malloc(FRAME_LEN);
	unsigned char* pBuf2 = (unsigned char*)malloc(FRAME_LEN);
	
	fseek(fp, NO * FRAME_LEN, SEEK_SET);
	
	fread(pBuf, 1, FRAME_LEN, fp);
	
	medianFilter(pBuf, pBuf2);
	
	
	fwrite(pBuf2, 1, FRAME_LEN, fp2);
	
	
	free(pBuf);
	free(pBuf2);
	
	fclose(fp);
	fclose(fp2);

	return 0;
}

