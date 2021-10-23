#include "GrammerJudge.h"

int main() {

	GrammerJudge grammerJudge("in2.txt");
	
	cout << "×÷Õß£ºY11614007 ·¶Î¬\n" << endl;
	 
	grammerJudge.printGrammer(); 
	
	cout << endl;
	
	grammerJudge.judgePattern();
	
	return 0;
}
