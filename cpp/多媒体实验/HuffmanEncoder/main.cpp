#include "HuffmanEncoderCompress.h"

void test();
void test2(int argc, char** argv);

int main(int argc, char** argv) {
	

	test();					// ���ز���
	
//	test2(argc, argv);		// �����϶� 
	
	return 0;
}


// ���ز��� 
void test() {
	
	HuffmanEncoderCompress* hecp = new HuffmanEncoderCompress("test\\pic.png.zLzip");
	
	hecp->run();
//	hecp->printInfo();

	int n = 4;
	for (int i = 0; i < n; ++i) {
		HuffmanEncoderCompress* hecp2 = new HuffmanEncoderCompress(hecp->getOutputFileName(), true);
		hecp2->run();
		delete hecp;
		hecp = hecp2;
	}
	
	delete hecp;
}


// ��ȡ���루���ڽ������϶��� 
void test2(int argc, char** argv) {
	if (argc < 2) exit(-1);
	
	HuffmanEncoderCompress* hecp = new HuffmanEncoderCompress(argv[1]);

	hecp->run();
	
	delete hecp;
	getchar();		// ��ֹ���н������� 
}
