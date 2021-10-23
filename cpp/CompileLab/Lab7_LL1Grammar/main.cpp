#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_范维\n" << endl;
	
	LL1Grammar g("in4.txt");
	
	g.showGrammar();	// 显示相关信息
	
	cout << "\n" << endl;
	
	g.eliminateLeftRecursion();	// 消除左递归 
	
	return 0;		//  
}
