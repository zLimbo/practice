#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_��ά\n" << endl;
	
	LL1Grammar g("in4.txt");
	
	g.showGrammar();	// ��ʾ�����Ϣ
	
	cout << "\n" << endl;
	
	g.eliminateLeftRecursion();	// ������ݹ� 
	
	return 0;		//  
}
