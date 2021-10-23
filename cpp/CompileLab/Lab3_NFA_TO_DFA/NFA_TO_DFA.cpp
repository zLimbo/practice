#include "NFA_TO_DFA.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cstdio>

using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;
using std::unordered_map;

// 构造器 
NFA_TO_DFA::NFA_TO_DFA(const string& fileName):
	_pNfa(new Graph), _pDfa(new Graph)
 {
	
	ifstream fin(fileName);

	char startId;
	fin >> startId;
	Node* start = _pNfa->getNode(startId);
	_pNfa->_start = start;
	fin.get();
	
	string line;
	getline(fin, line);
	istringstream sin(line);
	char terminalId;
	while (sin >> terminalId) {
		Node* terminal = _pNfa->getNode(terminalId); 
		_pNfa->_terminal.push_back(terminal);
	}	
	
	char fromId, toId, eId;
	while (fin >> fromId >> toId >> eId) {
		Node* from = _pNfa->getNode(fromId);
		Node* to = _pNfa->getNode(toId);
		Edge* ep = new Edge(eId, from, to);
		from->_eps.push_back(ep);
		_pNfa->_eps.push_back(ep);
		if (eId != '#') 
			_pNfa->_inputCharSet.insert(eId);
	} 
}


// 析构器 
NFA_TO_DFA::~NFA_TO_DFA() {
	delete _pNfa;
	delete _pDfa;
}


// 转化函数
void NFA_TO_DFA::nfa_to_dfa() {
	
	SetGraph sg;
	NodeSet* nsp = new NodeSet;
	nsp->_nps.insert(_pNfa->_start);
	closure(nsp);
	sg._nsps.push_back(nsp);
	
	printf("子集状态转换表：\n"); 
	printf("出发子集\t");
	for (char inputChar: _pNfa->_inputCharSet) printf("%c\t\t", inputChar);
	printf("\n");
	 
	for (size_t i = 0; i < sg._nsps.size(); ++i) {
		NodeSet* nsp = sg._nsps[i];
		
		for (Node* np2: nsp->_nps) printf("%c ", np2->_nId);
		printf("\t\t");
		
		for (char inputChar: _pNfa->_inputCharSet) {
			NodeSet* newNsp = moveTo(nsp, inputChar);
			if (!newNsp->_nps.empty()) {
				closure(newNsp);
				sg.uniqueInsert(newNsp);	// 不重复插入 
				NodeSetEdge* sep = new NodeSetEdge(inputChar, nsp, newNsp);
				
				for (Node* np2: newNsp->_nps) printf("%c ", np2->_nId);
				printf("\t\t");
				
				sg._seps.push_back(sep);
			} else {
				printf("-\t");
			}
		}
		printf("\n");
	}	
	printf("\n");
	printf("重新编号:\n"); 
	unordered_map<NodeSet*, Node*> ns2nMap;
	char nId = 'A';
	int cnt = 0;
	for (NodeSet* nsp: sg._nsps) {
		++cnt;
		Node* np = new Node(nId++);
		_pDfa->_nps.push_back(np);
		if (cnt == 1) _pDfa->_start = np;
		for (Node* np2: _pNfa->_terminal)
			if (nsp->_nps.count(np2))
				_pDfa->_terminal.push_back(np);
		ns2nMap.emplace(nsp, np);
		
		cout << np->_nId << " : "; 
		for (Node* np2: nsp->_nps)
			cout << np2->_nId << " ";
		cout << endl;
	}
	printf("\n");
	for (NodeSetEdge* sep: sg._seps) 
		_pDfa->_eps.push_back(new Edge(sep->_eId, ns2nMap[sep->_from], ns2nMap[sep->_to]));
} 


// 空
void NFA_TO_DFA::closure(NodeSet* nsp) {
	vector<Node*> tmpNps(nsp->_nps.begin(), nsp->_nps.end());
	for (size_t i = 0; i < tmpNps.size(); ++i) {
		Node* np = tmpNps[i];
		for (Edge* ep: np->_eps) {
			if (ep->_eId == '#' && 
					(find(tmpNps.begin(), tmpNps.end(), ep->_to) == tmpNps.end()))
				tmpNps.push_back(ep->_to);
		}
	}
	for (size_t i = nsp->_nps.size(); i < tmpNps.size(); ++i)
		nsp->_nps.insert(tmpNps[i]);
}


// 输入字符路径
NodeSet* NFA_TO_DFA::moveTo(NodeSet* nsp, char inputChar) {
	NodeSet* retNsp = new NodeSet;	
	for (auto it = nsp->_nps.begin(); it != nsp->_nps.end(); ++it) {
		Node* np = *it;
		for (Edge* ep: np->_eps) 
			if (ep->_eId == inputChar)
				retNsp->_nps.insert(ep->_to);
	}
	return retNsp;
}



// 打印 Graph 
void NFA_TO_DFA::showGraph(Graph* graph) {
	
	cout << "起始结点: " << graph->_start->_nId << "\t";
	cout << "终止结点: ";
	for (Node* np: graph->_terminal) cout << np->_nId << " ";
	cout << endl;
	cout << "  边\t输入符号" << endl; 
	for (Edge* ep: graph->_eps)
		cout << ep->_from->_nId << "--->" << ep->_to->_nId << "\t    " << ep->_eId << endl;
}

// 打印 DFA 
void NFA_TO_DFA::showDfa() { showGraph(_pDfa); }

// 打印 NFA
void NFA_TO_DFA::showNfa() { showGraph(_pNfa); }

