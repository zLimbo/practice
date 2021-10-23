#include <iostream>
#include "DFASimplify.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
	
	cout << "Y11614007 范维" << endl; 
	
	DFASimplify dfaSimplify("in.txt");
	
	cout << "\n化简前的DFA" << endl; 
	
	dfaSimplify.printDFA();		 
	
	dfaSimplify.simplify();
	
	cout << "\n化简后的DFA" << endl; 
	
	dfaSimplify.printDFA();
	
	return 0;
}
