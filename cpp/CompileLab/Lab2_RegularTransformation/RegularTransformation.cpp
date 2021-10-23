#include "RegularTransformation.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::ifstream;
using std::cout;
using std::endl;
using std::pair;


// ������ 
RegularTransformation::RegularTransformation(TransformType transformType, const string& fileName):
	_transformType(transformType)
{ 
	if (_transformType == RG_TO_RE) {
		readRG(fileName);
	} else if (_transformType == RE_TO_RG) {
		readRE(fileName);
	}
	assignId = 0;
} 


// ���������ķ� 
void RegularTransformation::readRG(const string& fileName) {
	
	ifstream fin(fileName);
	// �������ʽ
	pair<string, string> p;
	int cnt = 0;
	while (fin >> p.first >> p.second) {	
		_RG.P.insert(p);
		if (cnt++ == 0) _RG.S = p.first;
	}
	// ͳ���ս�������ս�� 
	for (auto it = _RG.P.begin(); it != _RG.P.end(); ++it) {
		for (char ch: it->first) {
			if (ch >= 'A' && ch <= 'Z') _RG.VN.insert(ch);
			else _RG.VT.insert(ch);
		}
		for (char ch: it->second) {
			if (ch >= 'A' && ch <= 'Z') _RG.VN.insert(ch);
			else _RG.VT.insert(ch);
		}
	}
}


// ��������ʽ 
void RegularTransformation::readRE(const string& fileName) {
	ifstream fin(fileName);
	fin >> _RE;
} 


// ���� 
void RegularTransformation::run() {
	
	if (_transformType == RG_TO_RE) {
		cout << "\n�����ķ� ===> ����ʽ��\n" << endl; 
		rg_to_re();
	} else if (_transformType == RE_TO_RG) {
		cout << "\n����ʽ ===> �����ķ���\n" << endl; 
		re_to_rg();
	}
}


// ���ҽڵ� 
Node* RegularTransformation::findNode(const string& left) {
	
	int id = left[0] - 'A';
	for (auto it = _nps.begin(); it != _nps.end(); ++it) {
		if ((*it)->id == id) return *it;
	}
	Node* np = np = new Node(left[0]-'A');
	_nps.push_back(np);
	return np;
}


// �����ķ�������ʽ 
void RegularTransformation::rg_to_re() {
	// ��ֹ��� 
	Node* zp = new Node('Z'-'A');
	// ���������ķ����� NFA 
	for (const pair<string, string> &p: _RG.P) {
		Node* np = findNode(p.first);
		const string& right = p.second; 
		if (right.size() == 1) {
			if (_RG.VT.count(right[0])) {
				Edge* ep = new Edge(np, zp, right.substr(0,1)); 
				(np->eps).push_back(ep);
				_eps.push_back(ep);	
			} else {
				Edge* ep = new Edge(np, findNode(right.substr(0,1)), "");
				(np->eps).push_back(ep);
				_eps.push_back(ep);
			}
				
		} else {
			Edge* ep = new Edge(np, findNode(right.substr(1,1)), right.substr(0,1));
			(np->eps).push_back(ep);
			_eps.push_back(ep);
		}
	}
	// ��ӡ�����ķ� 
	printRG();		
	cout << "\nת���ɵã�\n" << endl;
	string re = shrink(findNode(_RG.S), vector<Edge*>(), zp);
	cout << "����ʽ��" << re << endl; 
}


// �Ӹý�������������� �������ķ�������ʽ ��
string RegularTransformation::shrink(Node* np, vector<Edge*> preEdges, Node* zp) {
	string orRe;
	int cnt = 0;
	vector<pair<Edge*, string>> preNodePaths;
	for (Edge* ep: np->eps) {
		// ��ָ 
		if (ep->to->id == np->id) {
			np->starStrs.push_back(ep->re);
			continue;
		}
		// �Ƿ�ָ�� 
		bool pointPre = false;
		for (auto it =preEdges.begin(); it != preEdges.end(); ++it) {
			Edge* preEp = *it;
			if (preEp->from->id == ep->to->id) {
				// ָ�أ����ɱհ��� 
				string starRe = preEp->re;
				for (++it; it != preEdges.end(); ++it) 
					starRe.append((*it)->re);
				preNodePaths.emplace_back(ep, starRe);
				starRe.append(ep->re);
				// ����հ��� 
				preEp->from->starStrs.push_back(starRe);
				pointPre = true;
				break;
			}
		}
		if (pointPre) continue;
		// �������̽�� 
		preEdges.push_back(ep); 
		string tmpStr = shrink(ep->to, preEdges, zp);
		if (tmpStr != "NO_END") {	
			orRe.append(ep->re + tmpStr + "|"); 
			++cnt;
		}
		preEdges.pop_back();
	}
	
	if (!np->starStrs.empty() && !preNodePaths.empty()) {
		for (pair<Edge*, string>& p: preNodePaths) {
			for (string& starStr: np->starStrs) {
				p.first->to->starStrs.push_back(p.second + starStr + "(" +  starStr + ")*" + p.first->re);
			}
		}
	}
	
	
	// �����ǳ��յ���Ŀմ� 
	if (np->id != zp->id && orRe.empty()) return "NO_END";
	if (!orRe.empty()) orRe.pop_back();
	
	// ���غϲ��������ʽ 
	string retStr;
	if (!np->starStrs.empty()) {
		retStr.push_back('(');
		for (auto it = np->starStrs.begin(); it != np->starStrs.end(); ++it)
			retStr.append(*it + "|");
		retStr.pop_back();
		retStr.append(")*");
	}
	if (cnt > 1) retStr.append("(" + orRe + ")");
	else retStr.append(orRe);
	np->starStrs.clear();
	return retStr;
}


// ����ʽ�������ķ�	
void RegularTransformation::re_to_rg() {
	cout << "����ʽ: " << _RE << endl;
	Node* sp = new Node('S'-'A');
	Node* zp = new Node('#'-'A');
	Edge* ep = new Edge(sp, zp, _RE);
	_nps.push_back(sp);
	_nps.push_back(zp);

	expand(ep);
	
	int cnt = 0;
	for (Edge* ep: _eps) {
		pair<string, string> p;
		p.first.push_back((char)(ep->from->id+'A'));
		p.second.append(ep->re + (char)(ep->to->id+'A'));
		_RG.P.insert(p);
		if (cnt++ == 0) _RG.S = p.first;
	}
	// ͳ���ս�������ս�� 
	for (auto it = _RG.P.begin(); it != _RG.P.end(); ++it) {
		for (char ch: it->first) {
			if (ch >= 'A' && ch <= 'Z') _RG.VN.insert(ch);
			else _RG.VT.insert(ch);
		}
		for (char ch: it->second) {
			if (ch >= 'A' && ch <= 'Z') _RG.VN.insert(ch);
			else _RG.VT.insert(ch);
		}
	}
	cout << "\nת���ɵã�\n" << endl; 
	printRG();
} 


// �Ӹý����չ�������� ������ʽ�������ķ���
void RegularTransformation::expand(Edge* ep) {
	string& re = ep->re;
	int reLen = re.size();
	// ��ֹ���� 
	if (reLen <= 1) {
		if (ep->from != ep->to || !(ep->re).empty()) _eps.push_back(ep);
		return;
	}
	// RE1 | RE2 ��ʽ 
	for (int brkNum = 0, brkPos = 0; brkPos < reLen; ++brkPos) {
		if (re[brkPos] == '(') ++brkNum;
		else if (re[brkPos] == ')') --brkNum;
		if (re[brkPos] == '|' && brkNum == 0) {
			expand(new Edge(ep->from, ep->to, re.substr(0, brkPos)));
			expand(new Edge(ep->from, ep->to, re.substr(brkPos+1)));
			delete ep;
			return;
		}
	} 
	
	int pos, len, nextPos;
	if (re[0] != '(') {
		pos = 0;
		len = 1;
		nextPos = 1;
	} else {
		int brkNum = 1;
		int brkPos = 0;
		while (brkNum != 0) {
			++brkPos;
			if (re[brkPos] == '(') ++brkNum;
			else if (re[brkPos] == ')') --brkNum;
		}
		pos = 1;
		len = brkPos - pos;
		nextPos = brkPos + 1;
	}
	// RE1 * RE2 ��ʽ 
	if (re[nextPos] == '*') {
		++nextPos;	
		Node* np = new Node();
		_nps.push_back(np);
		expand(new Edge(ep->from, np, ""));
		expand(new Edge(np, np, re.substr(pos, len)));	
		expand(new Edge(np, ep->to, re.substr(nextPos)));
	} else {	// RE1 . RE2 ��ʽ 	
		Node* np = new Node();
		expand(new Edge(ep->from, np, re.substr(pos, len)));
		expand(new Edge(np, ep->to, re.substr(nextPos)));
	}
	delete ep;
}


// ��������ķ� 
void RegularTransformation::printRG() {
	cout << "�����ķ�: RG(VN, VT, P, S)" << endl;
	{
		cout << "VN = { ";
		auto it = _RG.VN.begin();
		cout << *it;
		for (++it; it != _RG.VN.end(); ++it) cout << ", " << *it;
		cout << " }" << endl;
	}	
	{
		cout << "VT = { ";
		auto it =  _RG.VT.begin();
		cout << *it;
		for (++it; it != _RG.VT.end(); ++it) cout << ", " << *it;
		cout << " }" << endl;	
	}
	{
		cout << "P = {" << endl;
		auto it = _RG.P.begin();
		cout << "    " << it->first << "-->" << it->second;
		for (++it; it != _RG.P.end(); ++it) {
			cout << ",\n    " << it->first << "-->" << it->second;
		}
		cout << "\n}" << endl;
		
	}
	cout << "S = " << _RG.S << endl;
}
