#include <cstdio>
#include <cmath>
#include <cstdlib>

const double PI = acos(-1);
const int N = 8;	 


int Q[N][N] = {
	{ 16, 11, 10, 16, 24, 40, 51, 61 },
	{ 12, 12, 14, 19, 26, 58, 60, 55 },
	{ 14, 13, 16, 24, 40, 57, 69, 56 },
	{ 14, 17, 22, 29, 51, 87, 80, 62 },
	{ 18, 22, 37, 56, 68, 109, 102, 77 },
	{ 24, 35, 55, 64, 81, 104, 113, 92 },
	{ 49, 64, 78, 87, 103, 121, 120, 101 },
	{ 72, 92, 95, 98, 112, 100, 103, 99 }	
};


// 打印整型 N x N 数组 
void printI(int arr[][N]) { 
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%4d ", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


// 打印实型 N x N 数组 
void printF(double arr[][N]) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%12f ", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


// DCT 
void DCT(int input[][N], double output[][N]) {
	int u, v, i, j;
	double cu, cv, sum;
	
	for (u = 0; u < N; ++u) {
		for (v = 0; v < N; ++v) {
			
			if (u == 0 && v == 0) cu = cv = sqrt(0.5);
			else cu = cv = 1;
			
			sum = 0;
		
			for (i = 0; i < N; ++i) 
				for (j = 0; j < N; ++j) 
					sum += input[i][j] * cos( (2.0*i+1)*u*PI/16.0 ) * cos( (2.0*j+1)*v*PI/16.0 );
					
			output[u][v] = 2.0 / N * cu * cv * sum;
		}
	} 
}


// 逆 DCT 
void rDCT(double input[][N], int output[][N]) {
	int u, v, i, j;
	double cu, cv, sum;
	
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			
			sum = 0;
			
			for (u = 0; u < N; ++u) {
				for (v = 0; v < N; ++v) {
					
					if (u == 0 && v == 0) cu = cv = sqrt(0.5);
					else cu = cv = 1;
					
					sum += cu * cv * input[u][v] * cos( (2.0*i+1)*u*PI/16.0 ) * cos( (2.0*j+1)*v*PI/16.0 );
				}
			}

			output[i][j] = 0.25 * sum;
		}
	} 
}


// 量化 
void Quantization(double input[][N], int output[][N]) {
	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			output[i][j] = input[i][j] / Q[i][j];
}


// 反量化 
void rQuantization(int input[][N], double output[][N]) {
	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			output[i][j] = input[i][j] * Q[i][j];
}


// DCT->量化->反量化->逆DCT 
int main() {
	
	int a[N][N] = {
		{ 139, 144, 149, 153, 155, 155, 155, 155 },
		{ 144, 151, 153, 156, 159, 156, 156, 156 },
		{ 150, 155, 160, 163, 158, 156, 156, 156 },
		{ 159, 161, 162, 160, 160, 159, 159, 159 },
		{ 159, 160, 161, 162, 162, 155, 155, 155 },
		{ 161, 161, 161, 161, 160, 157, 157, 157 },
		{ 162, 162, 161, 163, 162, 157, 157, 157 },
		{ 162, 162, 161, 161, 163, 158, 158, 158 }
	};
	
	double b[N][N];
	int c[N][N];
	double d[N][N];
	int e[N][N];
	
	DCT(a, b);
	Quantization(b, c);
	rQuantization(c, d);
	rDCT(d, e);
	
	printf("初始: \n"); printI(a);
	printf("DCT: \n"); printF(b);
	printf("量化: \n"); printI(c);
	printf("反量化: \n"); printF(d);
	printf("逆DCT: \n"); printI(e);
	
	return 0;
}

