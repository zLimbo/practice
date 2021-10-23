#ifndef __LR0GRAMMAR_H__
#define __LR0GRAMMAR_H__

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <queue>

using namespace std;

static int statusAssign = 0;

// 文法
struct Grammar {
	vector<pair<char, string>> P;
	unordered_set<char> VN;		// 非终结符集合
	unordered_set<char> VT;		// 终结符集合
	char S;
};

// 项目
struct Project {
	int prodNo;
	int pointPos;
	Project(int prodNo_, int pointPos_ = 0):
		prodNo(prodNo_), pointPos(pointPos_)
	{ }
};


// 结点
struct Node {
	int status;
	set<Project> projects;
	map<char, Node*> nexts;
};


// 图
struct Graph {
	unordered_set<Node*> nps;

	Node *start;

};


// LR0文法
class LR0Grammar {

	private:

		Grammar _g;

		Graph _dfa;


		map<pair<char, int>, pair<char, int>> _acction;
		map<pair<char, int>, int> _goto;

	private:

		void extendGrammar();

		void closure(Node *node);

		void expand(Node *node, queue<Node*> &q);

	public:

		// 构造器
		LR0Grammar(const string& fileName);

		// 是否是非终结符
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }

		// 是否是终结符
		bool isVT(char ch) const { return !isVN(ch) && ch != '#';}

		//
		char getNewVn() const {
			for (char ch = 'A'; ch <= 'Z'; ++ch)
				if (!_g.VN.count(ch))
					return ch;
		}


		void showGrammar() const;

        void printProject(const Project &project);

		void generateAnalysisTable();

		void printAnalysisTable();
};



#endif
