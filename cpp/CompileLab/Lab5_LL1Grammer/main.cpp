#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_��ά" << endl;
	 
	LL1Grammar g("in3.txt");
	
	g.inferToEmpty();	// ���Ƶ����յ�ӳ�� 
	
	g.generateFirst();	// ����FIRST�� 
	
	g.generateFollow();	// ����FOLLOW�� 
	
	g.generateSelect(); // ����SELECT�� 
	
	g.showGrammar();	// ��ʾ�����Ϣ
	
	cout << "\n" << (g.isLL1() ? "���ķ���LL1�ķ�" : "���ķ�����LL1�ķ�") << endl; 
	
	return 0;
}
