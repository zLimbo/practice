#include <iostream>
#include "NFA_TO_DFA.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {

	cout << "Y11614007 ��ά\n" << endl; 
	NFA_TO_DFA n2d("in.txt");
	
	cout << "��ȷ������NFA" << endl; 
	n2d.showNfa();
	
	cout << endl;
	
	n2d.nfa_to_dfa();
	
	cout << "ȷ�������DFA" << endl;
	n2d.showDfa(); 

	return 0;
}

