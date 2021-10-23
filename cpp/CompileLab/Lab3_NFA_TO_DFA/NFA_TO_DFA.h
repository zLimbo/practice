#ifndef __NFA_TO_DFA__
#define __NFA_TO_DFA__

#include <string>
#include <vector>
#include <set>

using std::string;
using std::vector;
using std::hash;
using std::set;
	

struct Edge;	// ��ǰ����


// �ڵ���
struct Node {
	
	char _nId;		// ���ID
	vector<Edge*> _eps;		// �Ӹý������ıߵļ��� 
	
	Node(int nId): _nId(nId) { }	// ������ 
	
};


// ����
struct Edge {

	char _eId;		// ��ID, ��ʵ����Ϊ�������  
	Node* _from;	// ������� 
	Node* _to;		// ������ 
	
	Edge(int eId, Node* from = nullptr, Node* to = nullptr):	// ������ 
		_eId(eId), _from(from), _to(to) 
	{ }
};


// ͼ��
struct Graph {
	
	vector<Node*> _nps;		// �㼯 
	vector<Edge*> _eps;		//  �߼� 

	Node* _start;			// ��ʼ��� 
	vector<Node*> _terminal;	// ��ֹ��� 
	
	set<char> _inputCharSet;	// ������ż��� 

	Node* getNode(char nId) {	// ��ý�� 
		for (Node* np: _nps) if (np->_nId == nId) return np;
		Node* np = new Node(nId);
		_nps.push_back(np); 
		return np;
	}
	
	~Graph() {
		for (Node* np: _nps) delete np;
		for (Edge* ep: _eps) delete ep;
	}
};


struct NodeSetEdge;	// ��ǰ����


// �м��״̬�ڵ���
struct NodeSet {
	set<Node*> _nps;
};

// �м��״̬�ڵ�ı���
struct NodeSetEdge {
	
	char _eId;		// ��ID, ��ʵ����Ϊ�������  
	
	NodeSet* _from;		// ��ʼ��� 
	NodeSet* _to; 		// ��ֹ��� 
	
	NodeSetEdge(char eId, NodeSet* from = nullptr, NodeSet* to = nullptr):
		_eId(eId), _from(from), _to(to) { }
};

// �м��״̬�ڵ��ͼ��
struct SetGraph {
	
	vector<NodeSet*> _nsps;	// �м��״̬�ڵ㼯 
	
	vector<NodeSetEdge*> _seps;	//�м��״̬�ڵ�ıߵı߼� 
	
	// Ψһ����
	bool uniqueInsert(NodeSet* &nsp_) {
		for (NodeSet* nsp: _nsps) 
			if (nsp->_nps == nsp_->_nps) {
				delete nsp_;
				nsp_ = nsp;
				return false;
			}
		_nsps.push_back(nsp_);
		return true;
	}
	
	~SetGraph() {
		for (NodeSet* nsp: _nsps) delete nsp;
		for (NodeSetEdge* sep: _seps) delete sep;
	}
}; 


// NFA �� DFA ȷ������ת����
class NFA_TO_DFA {
	
	private:
		
		Graph *_pNfa;	// ��ȷ������NFA
		
		Graph *_pDfa;	// ȷ�������DFA 

	private:

		void showGraph(Graph* graph);
		
		void closure(NodeSet* nsp);
		
		NodeSet* moveTo(NodeSet* nsp, char inputChar);
		
	public:
		
		NFA_TO_DFA(const string& fileName);

		~NFA_TO_DFA();	
		
		void nfa_to_dfa();
		
		void showNfa();

		void showDfa(); 
}; 

#endif

