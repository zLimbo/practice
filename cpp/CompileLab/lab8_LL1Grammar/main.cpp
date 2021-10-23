#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_范维\n" << endl;
	
	LL1Grammar g("in6.txt");
	
	g.showGrammar();	// 显示相关信息
	
	if (!g.convertToLL1()) cout << "\n无法转换" << endl;
	else {
		cout << "\n转换之后\n" << endl;
		g.showGrammar();	// 显示相关信息	
	} 
	
	return 0;		//  
}
