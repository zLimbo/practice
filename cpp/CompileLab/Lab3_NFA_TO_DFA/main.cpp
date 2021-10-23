#include <iostream>
#include "NFA_TO_DFA.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {

	cout << "Y11614007 范维\n" << endl; 
	NFA_TO_DFA n2d("in.txt");
	
	cout << "待确定化的NFA" << endl; 
	n2d.showNfa();
	
	cout << endl;
	
	n2d.nfa_to_dfa();
	
	cout << "确定化后的DFA" << endl;
	n2d.showDfa(); 

	return 0;
}

