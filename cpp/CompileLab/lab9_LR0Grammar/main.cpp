#include "LR0Grammar.h"
#include <iostream>


int main(int argc, char** argv) {

	LR0Grammar g("in.txt");

	g.showGrammar();        // 显示文法

	g.generateAnalysisTable();  // 生成分析表

	g.printAnalysisTable();    //打印分析表

	return 0;
}
