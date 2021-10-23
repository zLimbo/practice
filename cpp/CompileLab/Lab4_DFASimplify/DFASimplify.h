#ifndef __DFA_SIMPLIFY__
#define __DFA_SIMPLIFY__

#include <vector>
#include <string>
#include <unordered_set>
#include <map>

using std::vector;
using std::string; 
using std::unordered_set;
using std::map;
using std::move;


// ��ǰ���� 
struct Node;	
struct SubSet;

// ���� 
struct Edge {
	
	char eid;		// �߱�ʶ 
	
	Node* from;		// ������� 
	
	Node* to;		// ������ 
	
	// ֱ�ӹ����� 
	Edge(char eid_, Node* from_ = nullptr, Node* to_ = nullptr):
		eid(eid_), from(from_), to(to_) 
	{ }
};


// ����� 
struct Node {
	
	char nid;						// ����ʶ 
	
	unordered_set<Edge*> outEps;	// �Ӹý������ıߵļ��� 
	
	unordered_set<Edge*> inEps;		// ����ý��ıߵļ��� 
	
	SubSet* sp;						// �ý������Ӽ� 
	
	// ֱ�ӹ����� 
	Node(char nid_): nid(nid_) { }
	
	// ��ȡ�����Ӽ������� 
	map<char, SubSet*> getAttr() const {
		map<char, SubSet*> attr;
		for (Edge* ep: outEps) attr.emplace(ep->eid, ep->to->sp);
		return attr;
	}
};


// DFA�� 
struct DFA {
	
	vector<Node*> nps;					// ��㼯�� 
	
	unordered_set<Edge*> eps;			// �߼��� 
	
	Node* start;						// ��ʼ״̬ 
	
	unordered_set<Node*> terminal;		// ��ֹ״̬ 
	
	unordered_set<char> inputChars;		// ������� 
	
	//  ������ 
	~DFA() {
		for (Node* np: nps) delete np;
		for (const Edge* ep: eps) delete ep;
	} 
	
	// ����ID��ý�㣬�������򴴽���� 
	Node* getNode(char nid) {
		for (Node* np: nps) if (np->nid == nid) return np;
		Node* np = new Node(nid);
		nps.push_back(np);
		return np;
	}
	
	// ɾ��ĳ��� 
	void remove(Node* np);
	
	// �ϲ���� 
	void merge(Node* np1, Node* np2);
};


// �Ӽ���
struct SubSet {

	unordered_set<Node*> nps;	// ���Ӽ��Ľ�㼯 
	
	map<char, SubSet*> attr;	// ���� 
	
	// Ĭ�Ϲ����� 
	SubSet() = default;	
	
	// ֱ�ӹ����� 		
	SubSet(map<char, SubSet*>& attr_):	
		attr(move(attr_))
	{ } 
}; 


// DFA���� 
class DFASimplify {
	
	private:
		
		DFA _dfa;		// DFA 
		
	private:
	
		// ��ȥ����״̬
		void  removeRedundantState();
		
		// �ϲ��ȼ�״̬
		void  mergeEquivalentState(); 
		
		// ���ɵ��� 
		void getCannotReach(unordered_set<Node*>& reduNps);
		
		// ������ֹ 
		void getCannotTerminate(unordered_set<Node*>& reduNps);
		
		// �����Ӽ� 
		void divide(SubSet* sp, vector<SubSet*>& sps);
		
		// ��ӡ�Ӽ�������Ϣ 
		void printSpS(vector<SubSet*>& sps) const; 
		
	public:
		
		// ֱ�ӹ�����������Ϊ����DFA��Ϣ���ļ� 
		DFASimplify(const string& fileName); 
	
		// �� 
		void simplify();
		 
		// ��ӡ DFA 
		void printDFA() const;
};

#endif 
