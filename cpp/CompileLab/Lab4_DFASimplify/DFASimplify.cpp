#include "DFASimplify.h"

// stlͷ�ļ� 
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>

// stl
using std::ifstream;	
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;
using std::unordered_map;
using std::unordered_set;
using std::queue;
using std::pair;
using std::move;
using std::find_if;
using std::sort;


// ɾ����� 
void DFA::remove(Node* np) {
	nps.erase(find(nps.begin(), nps.end(), np));	// �ӽ�㼯��ɾȥ 
	terminal.erase(np);	// ���յ㼯��ɾȥ(������������) 
	if (!np) return; 
	// ɾ�����е�����ı߼��е���ر� 
	for (Edge* ep: np->outEps) {
		ep->to->inEps.erase(ep);
		eps.erase(ep);
		if (ep) delete ep; 
		
	}
	// ɾ���������Խ��ı߼��е���ر� 
	for (Edge* ep: np->inEps) {
		ep->from->outEps.erase(ep);
		eps.erase(ep);
		if (ep) delete ep;
	}	
}


// �ϲ���㣬���ҽ��ϲ��������� 
void DFA::merge(Node* np1, Node* np2) {
	for (Edge* ep: np2->inEps) { 
		// ���ҽ��������û�еıߣ����������ϸñ� 
		if (find_if(np1->inEps.begin(), np1->inEps.end(),
				[&](Edge* ep2){ return ep2->from == ep->from; })
				== np1->inEps.end()) { 
			Edge* newEp = new Edge(ep->eid, ep->from, np1);
			ep->from->outEps.insert(newEp);
			np1->inEps.insert(newEp);
			eps.insert(newEp);
		}
	}
	// ������С��id 
	if (np2->nid < np1->nid) np1->nid = np2->nid;
	remove(np2);
} 


// ֱ�ӹ�����������Ϊ����DFA��Ϣ���ļ� 
DFASimplify::DFASimplify(const string& fileName) { 
	// �ļ��� 
	ifstream fin(fileName);
	// ������ʼ��� 
	char startID;
	fin >> startID;
	Node* start = _dfa.getNode(startID);
	_dfa.start = start;
	// ������ֹ��� 
	string line;
	fin.get(); // �������з� 
	getline(fin, line); 
	istringstream sin(line);	// �ַ��������� 
	char terminalID;
	while (sin >> terminalID) {
		Node* terminal = _dfa.getNode(terminalID);
		_dfa.terminal.insert(terminal);
	}
	// �����
	char fromID, toID, eid;
	while (fin >> fromID >> toID >> eid) {
		Node* from = _dfa.getNode(fromID);
		Node* to = _dfa.getNode(toID);
		Edge* ep = new Edge(eid, from, to);
		from->outEps.insert(ep);
		to->inEps.insert(ep);
		_dfa.eps.insert(ep);
		_dfa.inputChars.insert(eid);
	} 
}


// �� 
void DFASimplify::simplify() {
	cout << "\n��һ������������״̬" << endl;
	removeRedundantState();
	cout << "\n�ڶ������ϲ��ȼ�״̬:" << endl;
	mergeEquivalentState();
}


 // ��ȥ����״̬
void  DFASimplify::removeRedundantState() {
	// ����״̬��� 
	unordered_set<Node*> reduNps; 	 
	// ���ɵ��� 
	getCannotReach(reduNps);
	// ������ֹ
	getCannotTerminate(reduNps);
	// ɾ������״̬ 
	for (Node* np: reduNps) _dfa.remove(np);	
}


// ���ɵ��� 
void DFASimplify::getCannotReach(unordered_set<Node*>& reduNps) {
	// ����<���,�Ƿ񵽴�>��ӳ�������ʼ��Ϊδ���� 
	unordered_map<Node*, bool> reachMap;
	for (Node* np: _dfa.nps) reachMap.emplace(np, false);	 
	// ����Ҫ��չ�Ľ����У���ʼ������ 
	queue<Node*> npQ;
	reachMap[_dfa.start] = true;
	npQ.push(_dfa.start);
	// �����зǿգ��Ӷ���ȡ����㣬���б�̽����������ȣ� 
	while (!npQ.empty()) {
		Node* curNp = npQ.front(); npQ.pop();
		for (Edge* ep: curNp->outEps) {
			Node* to = ep->to;
			// ��������δ���ʹ���������ӣ�ͬʱ��־Ϊ���ʹ� 
			if (reachMap[to] == false) {
				reachMap[to] = true;
				npQ.push(to);
			}
		}
	}
	cout << "���ɵ��"; 
	// �������״̬�� 
	for (const pair<Node*, int>& nps: reachMap) {
		if (nps.second == false) {
			cout << nps.first->nid << " ";
			reduNps.insert(nps.first);
		}
	}
	cout << endl;
}


// ������ֹ
void DFASimplify::getCannotTerminate(unordered_set<Node*>& reduNps) {
	// ����<���,�Ƿ񵽴�>��ӳ�������ʼ��Ϊδ���� 
	unordered_map<Node*, bool> reachMap;
	for (Node* np: _dfa.nps) reachMap.emplace(np, false);
	// ����Ҫ��չ�Ľ����У���ֹ������ 
	queue<Node*> npQ;
	for (Node* np: _dfa.terminal) {
		reachMap[np] = true;
		npQ.push(np);
	}
	// �����зǿգ��Ӷ���ȡ����㣬���б�̽����������ȣ� 
	while (!npQ.empty()) {
		Node* curNp = npQ.front(); npQ.pop();
		for (Edge* ep: curNp->inEps) {
			Node* from = ep->from;
			// ��������δ���ʹ���������ӣ�ͬʱ��־Ϊ���ʹ�
			if (reachMap[from] == false) {
				reachMap[from] = true;
				npQ.push(from);
			}
		}
	}
	cout << "������ֹ��"; 
	// �������״̬��
	for (const pair<Node*, int>& nps: reachMap) {
		if (nps.second == 0) {
			cout << nps.first->nid << " ";
			reduNps.insert(nps.first);
		}
	}
	cout << endl;	
}


// �ϲ��ȼ�״̬
void  DFASimplify::mergeEquivalentState() { 
 	// �����Ӽ����� 
	vector<SubSet*> sps; 
	// �Ȼ���Ϊ��̬�ͷ���̬�����Ӽ� 		
	SubSet* sp1 = new SubSet;	
	SubSet* sp2 = new SubSet;	
	for (Node* np: _dfa.nps) {
		if (_dfa.terminal.find(np) == _dfa.terminal.end()) {
			sp1->nps.insert(np);	// ����̬ 
			np->sp = sp1;
		} else {
			sp2->nps.insert(np);	// ��̬ 
			np->sp = sp2;
		}
	}
	sps.push_back(sp1);
	sps.push_back(sp2);
	// ��ʼ���� 
	while (true) {
		size_t oldSize = sps.size();	// ���滮��ǰ��С 
		vector<SubSet*> sps2(sps);	// ����һ�� 
		// ��ÿ���Ӽ����л��� 
		for (SubSet* sp: sps2) divide(sp, sps);
		// û�����Ӽ����������˳� 
		if (sps.size() == oldSize) break; 
	}
	// ��ӡ�Ӽ���Ϣ 
	printSpS(sps); 
	// �ϲ��ȼ�״̬ 
	for (SubSet* sp: sps) {
		auto it = sp->nps.begin();
		Node* np = *it;
		for (++it; it != sp->nps.end(); ++it) 
			_dfa.merge(np, *it);
	}
	// �ͷ���Դ 	
	for (SubSet* sp: sps) delete sp; 
}


// �����Ӽ� 
void DFASimplify::divide(SubSet* sp, vector<SubSet*>& sps) {
	// �Ӽ��ļ��ϣ��ü����ڵ��Ӽ���Ϊ��ǰ�Ӽ����ֵ��Ӽ� 
	vector<SubSet*> sps3;
	sps3.push_back(sp);
	unordered_set<Node*> nps(sp->nps);	// ����һ�� 
	// ����ǰ�Ӽ���������Ϊ��ʼ�������
	auto it = nps.begin();
	sp->attr = move((*it)->getAttr());	 
	// ����ÿ����� 
	for ( ; it != nps.end(); ++it) {
		map<char, SubSet*> attr(move((*it)->getAttr()));
		SubSet* sp2 = nullptr;
		// ��ǰ����Ƿ��������Ӽ��� 
		auto it2 = find_if(sps3.begin(), sps3.end(), 
				[&](SubSet* sp3){ return sp3->attr == attr; });
		if (it2 ==  sps3.end()) { 	// ���ڣ��򴴽����Ӽ� 
			sp2 = new SubSet(attr); 
			sps3.push_back(sp2); 	// ������Ҽ��� 
			sps.push_back(sp2);		// �����ܼ��� 
		} else {
			sp2 = *it2;				// �ڣ���sp2ָ����Ӽ� 
		}
		if (sp2 != sp) {
			sp->nps.erase(*it);		// �Ӿɶ�����ɾ�� 
			sp2->nps.insert(*it);	// �����¶��� 
			(*it)->sp = sp2;		// ��ȷ�������Ӽ� 
		}
	}
}


// ��ӡ�Ӽ� ����������� 
void DFASimplify::printSpS(vector<SubSet*>& sps) const {
	cout << "�ȼ�״̬�Ӽ���" << sps.size() << "������";
	// ���� 
	sort(sps.begin(), sps.end(), [](SubSet* lhs, SubSet* rhs) { 
			return (*lhs->nps.begin())->nid < (*rhs->nps.begin())->nid;});
	for (SubSet* sp: sps) {
		vector<Node*> v(sp->nps.begin(), sp->nps.end());
		// ���� 
		sort(v.begin(), v.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; });
		auto it = v.begin();
		cout << "{ " << (*it)->nid; 
		for (++it; it != v.end(); ++it) cout << ", " << (*it)->nid; 
		cout << " } ";
	}
	cout << endl;
}


// ��ӡ DFA ����������� 
void DFASimplify::printDFA() const {
 
	cout << "��ʼ״̬��" << _dfa.start->nid << endl;
	
	cout << "��ֹ״̬��"; 
	vector<Node*> v1(_dfa.terminal.begin(), _dfa.terminal.end());	// ������
	sort(v1.begin(), v1.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; }); 
	for (Node* np: v1) cout << np->nid << " "; cout << endl;
	
	cout << "״̬: "; 
	vector<Node*> v2(_dfa.nps.begin(), _dfa.nps.end());		// ������
	sort(v2.begin(), v2.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; }); 
	for (Node* np: v2) cout << np->nid << " "; cout << endl;
	
	cout << "������ţ�";
	vector<char> v3(_dfa.inputChars.begin(), _dfa.inputChars.end());	// ������
	sort(v3.begin(), v3.end(), [](char lhs, char rhs) { return lhs < rhs; });
	for (char inputChar: v3) cout << inputChar << " "; cout << endl; 
	
	cout << "��: " << endl;
	vector<Edge*> v4(_dfa.eps.begin(), _dfa.eps.end());		// ������
	sort(v4.begin(), v4.end(), 
			[](Edge* lhs, Edge* rhs) { 
					return lhs->from->nid < rhs->from->nid
							|| (lhs->from->nid == rhs->from->nid && lhs->eid < rhs->eid); });
	for (Edge* ep: v4) cout << ep->from->nid << "--->" << ep->to->nid << "  " << ep->eid << endl; 
}
