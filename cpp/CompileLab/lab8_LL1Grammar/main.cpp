#include <iostream>
#include "LL1Grammar.h"

using namespace std;

int main(int argc, char** argv) {
	
	cout << "Y11614007_��ά\n" << endl;
	
	LL1Grammar g("in6.txt");
	
	g.showGrammar();	// ��ʾ�����Ϣ
	
	if (!g.convertToLL1()) cout << "\n�޷�ת��" << endl;
	else {
		cout << "\nת��֮��\n" << endl;
		g.showGrammar();	// ��ʾ�����Ϣ	
	} 
	
	return 0;		//  
}
