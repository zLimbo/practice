#include "LR0Grammar.h"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

bool operator<(const Project &lhs, const Project &rhs) {
	return lhs.prodNo < rhs.prodNo || (lhs.prodNo == rhs.prodNo && lhs.pointPos < rhs.pointPos);
}

bool operator==(const Project &lhs, const Project &rhs) {
	return lhs.prodNo == rhs.prodNo && lhs.pointPos == rhs.pointPos;
}

bool operator==(const Node &lhs, const Node &rhs) {
	return lhs.projects == rhs.projects;
}

void LR0Grammar::printProject(const Project &project) {
    string right = _g.P[project.prodNo].second;
    if (project.pointPos < _g.P[project.prodNo].second.size())
        right.insert(project.pointPos, ".");
    else
        right.push_back('.');
    cout << _g.P[project.prodNo].first << "->" << right << endl;
};

// 构造器，读入产生式，生成文法
LR0Grammar::LR0Grammar(const string& fileName) {

	ifstream fin(fileName);

	char left;
	string right;
	int n = 0;

	while (fin >> left >> right) {
        pair<char, string> p(left, right);
		_g.P.push_back(p);
		_g.VN.insert(left);
		for (char ch: right) {
			if (ch >= 'A' && ch <= 'Z') _g.VN.insert(ch);
			else _g.VT.insert(ch);
		}
		if (n++ == 0) _g.S = left;
	}
}


// 显示相关信息
void LR0Grammar::showGrammar() const {

	cout << "文法 G" << endl;
	cout << "开始符号：" << _g.S << endl;
	cout << "非终结符：";
	for (char ch: set<char>(_g.VN.begin(), _g.VN.end())) cout << ch << " "; cout << endl;
	cout << "终结符：";
	for (char ch: set<char>(_g.VT.begin(), _g.VT.end())) if (ch != '#') cout << ch << " "; cout << endl;
	cout << "产生式：" << endl;
	for (const pair<char, string>& p: _g.P) cout << p.first << "-->" << p.second << endl;
}


// 拓广文法
void LR0Grammar::extendGrammar() {
	char vn = getNewVn();
	pair<char, string> p(vn, string(1, _g.S));
    _g.P.insert(_g.P.begin(), p);
	_g.S = vn;
	_g.VN.insert(vn);
}




// 生成分析表
void LR0Grammar::generateAnalysisTable() {

	extendGrammar();

	_dfa.start = new Node;
	_dfa.start->status = statusAssign++;
	_dfa.start->projects.insert(Project(0));
    closure(_dfa.start);
	_dfa.nps.insert(_dfa.start);

	queue<Node*> q;
	q.push(_dfa.start);

	while (!q.empty()) {
        Node* np = q.front(); q.pop();
        expand(np, q);
	}
}





// 扩展结点
void LR0Grammar::expand(Node *np, queue<Node*> &q) {

	multimap<char, const Project*> projAuxMap;
	set<char> outs;

	for (const Project &project: np->projects) {
		if (project.pointPos < _g.P[project.prodNo].second.size()) {
			projAuxMap.emplace(_g.P[project.prodNo].second[project.pointPos], &project);
			outs.insert(_g.P[project.prodNo].second[project.pointPos]);
		}
	}

    if (outs.empty()) {
        int prodNo = np->projects.begin()->prodNo;
        int ch = _g.P[prodNo].second.back();
        if (_g.P[prodNo].first == _g.S) {
            _acction.emplace(pair<char, int>('#', np->status), pair<char, int>('a', 0));
        } else {
            _acction.emplace(pair<char, int>('#', np->status), pair<char, int>('r', prodNo));
            for (char ch2: _g.VT) {
                _acction.emplace(pair<char, int>(ch2, np->status), pair<char, int>('r', prodNo));
            }
        }
    }

	for (char ch: outs) {
		auto range = projAuxMap.equal_range(ch);

		Node *newNp = new Node;
		for ( ; range.first != range.second; ++range.first) {
			const Project *project = range.first->second;
			newNp->projects.insert(Project(project->prodNo, project->pointPos+1));
		}

		closure(newNp);

        auto np2 = find_if(_dfa.nps.begin(), _dfa.nps.end(),
                    [&](Node *np3){ return *np3 == *newNp;});
		if (np2 == _dfa.nps.end()) {
			newNp->status = statusAssign++;
			np->nexts.emplace(ch, newNp);
			_dfa.nps.insert(newNp);

			if (isVT(ch)) {
                _acction.emplace(pair<char, int>(ch, np->status), pair<char, int>('S', newNp->status));
			} else {
                _goto.emplace(pair<char, int>(ch, np->status), newNp->status);
			}

			q.push(newNp);
		} else {
            //np->nexts.insert(ch, &*np2);
		}
	}
}

// Closure 闭包
void LR0Grammar::closure(Node *np) {

	while (true) {
		set<Project> projects2(np->projects);
		for (const Project &project: np->projects) {
			if (project.pointPos < _g.P[project.prodNo].second.size()) {
				char ch = _g.P[project.prodNo].second[project.pointPos];
				if (isVN(ch)) {
					for (int i = 0; i < _g.P.size(); ++i) {
                        if (_g.P[i].first == ch)
                            projects2.insert(Project(i));
					}
				}
			}
		}
		if (np->projects.size() == projects2.size()) break;
		np->projects.swap(projects2);
	}
}

void LR0Grammar::printAnalysisTable() {
    set<char> V(_g.VT.begin(), _g.VT.end());
	V.insert('#');
	int statusNum = _dfa.nps.size();
	cout << "\t";
	for (char ch: V) cout << ch << "\t";
	for (char ch: _g.VN) cout << ch << "\t";
	cout << endl;
    for (int i = 0; i < statusNum; ++i) {
        cout << i << "\t";
        for (char ch: V) {
            pair<char, int> first(ch, i);
            if (_acction.find(first) != _acction.end()) {
                cout << _acction[first].first << _acction[first].second << "\t";
            } else {
                cout << "--" << "\t";
            }
        }
        for (char ch: _g.VN) {
            pair<char, int> first(ch, i);
            if (_goto.find(first) != _goto.end()) {
                cout << _goto[first] << "\t";
            } else {
                cout << "--" << "\t";
            }
        }
        cout << endl;
    }

    vector<Node*> auxVec(_dfa.nps.begin(), _dfa.nps.end());
    sort(auxVec.begin(), auxVec.end(), [](Node *lhs, Node *rhs) { return lhs->status < rhs->status; });
    for (Node *np: auxVec) {
        cout << "\n状态：" << np->status  << endl;
        for (const Project &project: np->projects) {
            printProject(project);
        }
    }
}
