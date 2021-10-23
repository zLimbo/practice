#include "RegularTransformation.h"

using namespace std;

int main(int argc, char** argv) {
	
	
	RegularTransformation rt(RG_TO_RE, "rg3.txt");	// 正规文法转正规式

	rt.run();
	
//	RegularTransformation rt3(RG_TO_RE, "rg3.txt");	// 正规文法转正规式
//
//	rt3.run();
	
	cout << endl;
	
	RegularTransformation rt2(RE_TO_RG, "re4.txt");	// 正规式转正规文法 

	rt2.run();

	return 0;
}
