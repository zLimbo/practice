#include "LR0Grammar.h"
#include <iostream>


int main(int argc, char** argv) {

	LR0Grammar g("in.txt");

	g.showGrammar();        // ��ʾ�ķ�

	g.generateAnalysisTable();  // ���ɷ�����

	g.printAnalysisTable();    //��ӡ������

	return 0;
}
