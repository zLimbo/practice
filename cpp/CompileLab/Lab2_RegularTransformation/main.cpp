#include "RegularTransformation.h"

using namespace std;

int main(int argc, char** argv) {
	
	
	RegularTransformation rt(RG_TO_RE, "rg3.txt");	// �����ķ�ת����ʽ

	rt.run();
	
//	RegularTransformation rt3(RG_TO_RE, "rg3.txt");	// �����ķ�ת����ʽ
//
//	rt3.run();
	
	cout << endl;
	
	RegularTransformation rt2(RE_TO_RG, "re4.txt");	// ����ʽת�����ķ� 

	rt2.run();

	return 0;
}
