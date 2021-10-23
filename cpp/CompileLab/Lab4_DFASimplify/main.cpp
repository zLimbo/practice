#include <iostream>
#include "DFASimplify.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	
	cout << "Y11614007 ��ά" << endl; 
	
	DFASimplify dfaSimplify("in.txt");
	
	cout << "\n����ǰ��DFA" << endl; 
	
	dfaSimplify.printDFA();		 
	
	dfaSimplify.simplify();
	
	cout << "\n������DFA" << endl; 
	
	dfaSimplify.printDFA();
	
	return 0;
}
