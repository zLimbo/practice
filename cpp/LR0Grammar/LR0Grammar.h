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

// �ķ�
struct Grammar {
	vector<pair<char, string>> P;
	unordered_set<char> VN;		// ���ս������
	unordered_set<char> VT;		// �ս������
	char S;
};

// ��Ŀ
struct Project {
	int prodNo;
	int pointPos;
	Project(int prodNo_, int pointPos_ = 0):
		prodNo(prodNo_), pointPos(pointPos_)
	{ }
};


// ���
struct Node {
	int status;
	set<Project> projects;
	map<char, Node*> nexts;
};


// ͼ
struct Graph {
	unordered_set<Node*> nps;

	Node *start;

};


// LR0�ķ�
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

		// ������
		LR0Grammar(const string& fileName);

		// �Ƿ��Ƿ��ս��
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }

		// �Ƿ����ս��
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
