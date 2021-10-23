#include "DFASimplify.h"

// stl头文件 
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


// 删除结点 
void DFA::remove(Node* np) {
	nps.erase(find(nps.begin(), nps.end(), np));	// 从结点集中删去 
	terminal.erase(np);	// 从终点集中删去(不在则无作用) 
	if (!np) return; 
	// 删除所有到达结点的边集中的相关边 
	for (Edge* ep: np->outEps) {
		ep->to->inEps.erase(ep);
		eps.erase(ep);
		if (ep) delete ep; 
		
	}
	// 删除所有来自结点的边集中的相关边 
	for (Edge* ep: np->inEps) {
		ep->from->outEps.erase(ep);
		eps.erase(ep);
		if (ep) delete ep;
	}	
}


// 合并结点，将右结点合并到左结点上 
void DFA::merge(Node* np1, Node* np2) {
	for (Edge* ep: np2->inEps) { 
		// 若右结点有左结点没有的边，则给左结点加上该边 
		if (find_if(np1->inEps.begin(), np1->inEps.end(),
				[&](Edge* ep2){ return ep2->from == ep->from; })
				== np1->inEps.end()) { 
			Edge* newEp = new Edge(ep->eid, ep->from, np1);
			ep->from->outEps.insert(newEp);
			np1->inEps.insert(newEp);
			eps.insert(newEp);
		}
	}
	// 采用最小的id 
	if (np2->nid < np1->nid) np1->nid = np2->nid;
	remove(np2);
} 


// 直接构造器，参数为包含DFA信息的文件 
DFASimplify::DFASimplify(const string& fileName) { 
	// 文件流 
	ifstream fin(fileName);
	// 读入起始结点 
	char startID;
	fin >> startID;
	Node* start = _dfa.getNode(startID);
	_dfa.start = start;
	// 读入终止结点 
	string line;
	fin.get(); // 抛弃换行符 
	getline(fin, line); 
	istringstream sin(line);	// 字符串输入流 
	char terminalID;
	while (sin >> terminalID) {
		Node* terminal = _dfa.getNode(terminalID);
		_dfa.terminal.insert(terminal);
	}
	// 读入边
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


// 简化 
void DFASimplify::simplify() {
	cout << "\n第一步：消除多余状态" << endl;
	removeRedundantState();
	cout << "\n第二步：合并等价状态:" << endl;
	mergeEquivalentState();
}


 // 消去多余状态
void  DFASimplify::removeRedundantState() {
	// 多余状态结点 
	unordered_set<Node*> reduNps; 	 
	// 不可到达 
	getCannotReach(reduNps);
	// 不可终止
	getCannotTerminate(reduNps);
	// 删除多余状态 
	for (Node* np: reduNps) _dfa.remove(np);	
}


// 不可到达 
void DFASimplify::getCannotReach(unordered_set<Node*>& reduNps) {
	// 创建<结点,是否到达>的映射表，并初始化为未到达 
	unordered_map<Node*, bool> reachMap;
	for (Node* np: _dfa.nps) reachMap.emplace(np, false);	 
	// 创建要扩展的结点队列，起始结点入队 
	queue<Node*> npQ;
	reachMap[_dfa.start] = true;
	npQ.push(_dfa.start);
	// 当队列非空，从队首取出结点，进行边探索（广度优先） 
	while (!npQ.empty()) {
		Node* curNp = npQ.front(); npQ.pop();
		for (Edge* ep: curNp->outEps) {
			Node* to = ep->to;
			// 若到达结点未访问过，将其入队，同时标志为访问过 
			if (reachMap[to] == false) {
				reachMap[to] = true;
				npQ.push(to);
			}
		}
	}
	cout << "不可到达："; 
	// 归入多余状态集 
	for (const pair<Node*, int>& nps: reachMap) {
		if (nps.second == false) {
			cout << nps.first->nid << " ";
			reduNps.insert(nps.first);
		}
	}
	cout << endl;
}


// 不可终止
void DFASimplify::getCannotTerminate(unordered_set<Node*>& reduNps) {
	// 创建<结点,是否到达>的映射表，并初始化为未到达 
	unordered_map<Node*, bool> reachMap;
	for (Node* np: _dfa.nps) reachMap.emplace(np, false);
	// 创建要扩展的结点队列，终止结点入队 
	queue<Node*> npQ;
	for (Node* np: _dfa.terminal) {
		reachMap[np] = true;
		npQ.push(np);
	}
	// 当队列非空，从队首取出结点，进行边探索（广度优先） 
	while (!npQ.empty()) {
		Node* curNp = npQ.front(); npQ.pop();
		for (Edge* ep: curNp->inEps) {
			Node* from = ep->from;
			// 若到达结点未访问过，将其入队，同时标志为访问过
			if (reachMap[from] == false) {
				reachMap[from] = true;
				npQ.push(from);
			}
		}
	}
	cout << "不可终止："; 
	// 归入多余状态集
	for (const pair<Node*, int>& nps: reachMap) {
		if (nps.second == 0) {
			cout << nps.first->nid << " ";
			reduNps.insert(nps.first);
		}
	}
	cout << endl;	
}


// 合并等价状态
void  DFASimplify::mergeEquivalentState() { 
 	// 创建子集集合 
	vector<SubSet*> sps; 
	// 先划分为终态和非终态两个子集 		
	SubSet* sp1 = new SubSet;	
	SubSet* sp2 = new SubSet;	
	for (Node* np: _dfa.nps) {
		if (_dfa.terminal.find(np) == _dfa.terminal.end()) {
			sp1->nps.insert(np);	// 非终态 
			np->sp = sp1;
		} else {
			sp2->nps.insert(np);	// 终态 
			np->sp = sp2;
		}
	}
	sps.push_back(sp1);
	sps.push_back(sp2);
	// 开始划分 
	while (true) {
		size_t oldSize = sps.size();	// 保存划分前大小 
		vector<SubSet*> sps2(sps);	// 拷贝一份 
		// 对每个子集进行划分 
		for (SubSet* sp: sps2) divide(sp, sps);
		// 没有新子集产生，则退出 
		if (sps.size() == oldSize) break; 
	}
	// 打印子集信息 
	printSpS(sps); 
	// 合并等价状态 
	for (SubSet* sp: sps) {
		auto it = sp->nps.begin();
		Node* np = *it;
		for (++it; it != sp->nps.end(); ++it) 
			_dfa.merge(np, *it);
	}
	// 释放资源 	
	for (SubSet* sp: sps) delete sp; 
}


// 划分子集 
void DFASimplify::divide(SubSet* sp, vector<SubSet*>& sps) {
	// 子集的集合，该集合内的子集即为当前子集划分的子集 
	vector<SubSet*> sps3;
	sps3.push_back(sp);
	unordered_set<Node*> nps(sp->nps);	// 拷贝一份 
	// 将当前子集合属性设为开始结点属性
	auto it = nps.begin();
	sp->attr = move((*it)->getAttr());	 
	// 遍历每个结点 
	for ( ; it != nps.end(); ++it) {
		map<char, SubSet*> attr(move((*it)->getAttr()));
		SubSet* sp2 = nullptr;
		// 当前结点是否在已有子集中 
		auto it2 = find_if(sps3.begin(), sps3.end(), 
				[&](SubSet* sp3){ return sp3->attr == attr; });
		if (it2 ==  sps3.end()) { 	// 不在，则创建新子集 
			sp2 = new SubSet(attr); 
			sps3.push_back(sp2); 	// 插入查找集中 
			sps.push_back(sp2);		// 插入总集中 
		} else {
			sp2 = *it2;				// 在，则sp2指向该子集 
		}
		if (sp2 != sp) {
			sp->nps.erase(*it);		// 从旧队列中删除 
			sp2->nps.insert(*it);	// 插入新队列 
			(*it)->sp = sp2;		// 重确定归属子集 
		}
	}
}


// 打印子集 （排序输出） 
void DFASimplify::printSpS(vector<SubSet*>& sps) const {
	cout << "等价状态子集（" << sps.size() << "个）：";
	// 排序 
	sort(sps.begin(), sps.end(), [](SubSet* lhs, SubSet* rhs) { 
			return (*lhs->nps.begin())->nid < (*rhs->nps.begin())->nid;});
	for (SubSet* sp: sps) {
		vector<Node*> v(sp->nps.begin(), sp->nps.end());
		// 排序 
		sort(v.begin(), v.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; });
		auto it = v.begin();
		cout << "{ " << (*it)->nid; 
		for (++it; it != v.end(); ++it) cout << ", " << (*it)->nid; 
		cout << " } ";
	}
	cout << endl;
}


// 打印 DFA （排序输出） 
void DFASimplify::printDFA() const {
 
	cout << "起始状态：" << _dfa.start->nid << endl;
	
	cout << "终止状态："; 
	vector<Node*> v1(_dfa.terminal.begin(), _dfa.terminal.end());	// 排序辅助
	sort(v1.begin(), v1.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; }); 
	for (Node* np: v1) cout << np->nid << " "; cout << endl;
	
	cout << "状态: "; 
	vector<Node*> v2(_dfa.nps.begin(), _dfa.nps.end());		// 排序辅助
	sort(v2.begin(), v2.end(), [](Node* lhs, Node* rhs) { return lhs->nid < rhs->nid; }); 
	for (Node* np: v2) cout << np->nid << " "; cout << endl;
	
	cout << "输入符号：";
	vector<char> v3(_dfa.inputChars.begin(), _dfa.inputChars.end());	// 排序辅助
	sort(v3.begin(), v3.end(), [](char lhs, char rhs) { return lhs < rhs; });
	for (char inputChar: v3) cout << inputChar << " "; cout << endl; 
	
	cout << "边: " << endl;
	vector<Edge*> v4(_dfa.eps.begin(), _dfa.eps.end());		// 排序辅助
	sort(v4.begin(), v4.end(), 
			[](Edge* lhs, Edge* rhs) { 
					return lhs->from->nid < rhs->from->nid
							|| (lhs->from->nid == rhs->from->nid && lhs->eid < rhs->eid); });
	for (Edge* ep: v4) cout << ep->from->nid << "--->" << ep->to->nid << "  " << ep->eid << endl; 
}
