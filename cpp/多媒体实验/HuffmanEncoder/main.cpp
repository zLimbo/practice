#include "HuffmanEncoderCompress.h"

void test();
void test2(int argc, char** argv);

int main(int argc, char** argv) {
	

	test();					// 本地测试
	
//	test2(argc, argv);		// 界面拖动 
	
	return 0;
}


// 本地测试 
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


// 读取输入（可在界面中拖动） 
void test2(int argc, char** argv) {
	if (argc < 2) exit(-1);
	
	HuffmanEncoderCompress* hecp = new HuffmanEncoderCompress(argv[1]);

	hecp->run();
	
	delete hecp;
	getchar();		// 防止运行界面闪退 
}
