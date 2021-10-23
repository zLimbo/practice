#include <cstdio>
#include <cmath>
#include <cstdlib>

const double PI = acos(-1);
const int N = 8;
const int M = 320;

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


// ��ӡ���� NxN ����ֿ� 
void printI(int arr[][M], int r = 0, int c = 0) { 
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%d ", arr[r+i][c+j]);
		}
		printf("\n");
	}
	printf("\n");
}


// ��ӡʵ�� NxN ����ֿ� 
void printF(double arr[][M], int r = 0, int c = 0) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%f ", arr[r+i][c+j]);
		}
		printf("\n");
	}
	printf("\n");
}


// DCT�任 
void DCT(int input[][M], double output[][M], int r = 0, int c = 0) {
	int u, v, i, j;
	double cu, cv, sum;
	
	for (u = 0; u < N; ++u) {
		for (v = 0; v < N; ++v) {
			
			if (u == 0 && v == 0) cu = cv = sqrt(0.5);
			else cu = cv = 1;
			
			sum = 0;
		
			for (i = 0; i < N; ++i) 
				for (j = 0; j < N; ++j) 
					sum += input[r+i][c+j] * cos( (2.0*i+1)*u*PI/(2*N) ) * cos( (2.0*j+1)*v*PI/(2*N) );
					
			output[r+u][c+v] = 2.0 / N * cu * cv * sum;
		}
	} 
}


// ��DCT�任 
void rDCT(double input[][M], int output[][M], int r = 0, int c = 0) {
	int u, v, i, j;
	double cu, cv, sum;
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {	
			sum = 0;
			for (u = 0; u < N; ++u) {
				for (v = 0; v < N; ++v) {
					if (u == 0 && v == 0) cu = cv = sqrt(0.5);
					else cu = cv = 1;
					sum += cu * cv * input[r+u][c+v] * cos( (2.0*i+1)*u*PI/(2*N) ) * cos( (2.0*j+1)*v*PI/(2*N) );
				}
			}

			output[r+i][c+j] = 2.0 / N * sum;
		}
	} 
}


// ���� 
void Quantization(double input[][M], int output[][M], int r = 0, int c = 0) {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j) 
			output[r+i][c+j] = input[r+i][c+j] / Q[i][j];

}


// ������ 
void rQuantization(int input[][M], double output[][M], int r = 0, int c = 0) {
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			output[r+i][c+j] = input[r+i][c+j] * Q[i][j];
}


// �������ֲ� 
int main() {
	
	int a1[M][M];		// M = 320
	double a2[M][M];
	int a3[M][M];
	
	for (int i = 0; i < M; ++i)
		for (int j = 0; j < M; ++j)
			a1[i][j] = rand() % 256;	// ���ȡֵ 
	
	int up = M / N;		// 320 / 8 = 40 
	double dis[N][N];		// ͳ�������ֲ� 
	
	// dis��ʼ��Ϊ 0 
	for (int i = 0; i < N; ++i) 
		for (int j = 0; j < N; ++j) 
			dis[i][j] = 0;
			
	// �� 40 x 40 ���ֿ�ֱ����DCT������ 
	for (int i = 0; i < up; ++i) {		
		int r = i * N;
		for (int j =  0; j < up; ++j) {
			int c = j * N;
			
			DCT(a1, a2, r, c);
			Quantization(a2, a3, r, c);
			
			// �ۼӷֲ�ֵ 
			for (int x = 0; x < N; ++x)
				for (int y = 0; y < N; ++y)
					dis[x][y] += a3[r+x][c+y];
		}
	}
	
	// ��ӡ�ۻ�ֵ 
	printf("�ۻ��ֲ���\n"); 
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%10.2f ", dis[i][j]);
		} 
		printf("\n");
	}
	
	// ��ƽ�� 
	int n = up * up;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			dis[i][j] /= n;			// ��ƽ�� 
			
	// ��ӡƽ�� 
	printf("\nƽ���ֲ�:\n"); 
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%10.4f ", dis[i][j]);
		} 
		printf("\n");
	}
	
	return 0;
}



