#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_范维" << endl;
	 
	LL1Grammar g("in3.txt");
	
	g.inferToEmpty();	// 能推倒出空的映射 
	
	g.generateFirst();	// 生成FIRST集 
	
	g.generateFollow();	// 生成FOLLOW集 
	
	g.generateSelect(); // 生成SELECT集 
	
	g.showGrammar();	// 显示相关信息
	
	cout << "\n" << (g.isLL1() ? "该文法是LL1文法" : "该文法不是LL1文法") << endl; 
	
	return 0;
}
