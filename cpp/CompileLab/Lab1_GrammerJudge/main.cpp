#include "GrammerJudge.h"

int main() {

	GrammerJudge grammerJudge("in2.txt");
	
	cout << "���ߣ�Y11614007 ��ά\n" << endl;
	 
	grammerJudge.printGrammer(); 
	
	cout << endl;
	
	grammerJudge.judgePattern();
	
	return 0;
}
