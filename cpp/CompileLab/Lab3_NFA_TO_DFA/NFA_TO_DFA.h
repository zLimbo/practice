#ifndef __NFA_TO_DFA__
#define __NFA_TO_DFA__

#include <string>
#include <vector>
#include <set>

using std::string;
using std::vector;
using std::hash;
using std::set;
	

struct Edge;	// 提前声明


// 节点类
struct Node {
	
	char _nId;		// 结点ID
	vector<Edge*> _eps;		// 从该结点出发的边的集合 
	
	Node(int nId): _nId(nId) { }	// 构造器 
	
};


// 边类
struct Edge {

	char _eId;		// 边ID, 本实验中为输入符号  
	Node* _from;	// 出发结点 
	Node* _to;		// 到达结点 
	
	Edge(int eId, Node* from = nullptr, Node* to = nullptr):	// 构造器 
		_eId(eId), _from(from), _to(to) 
	{ }
};


// 图类
struct Graph {
	
	vector<Node*> _nps;		// 点集 
	vector<Edge*> _eps;		//  边集 

	Node* _start;			// 起始结点 
	vector<Node*> _terminal;	// 终止结点 
	
	set<char> _inputCharSet;	// 输入符号集合 

	Node* getNode(char nId) {	// 获得结点 
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


struct NodeSetEdge;	// 提前声明


// 中间多状态节点类
struct NodeSet {
	set<Node*> _nps;
};

// 中间多状态节点的边类
struct NodeSetEdge {
	
	char _eId;		// 边ID, 本实验中为输入符号  
	
	NodeSet* _from;		// 起始结点 
	NodeSet* _to; 		// 终止结点 
	
	NodeSetEdge(char eId, NodeSet* from = nullptr, NodeSet* to = nullptr):
		_eId(eId), _from(from), _to(to) { }
};

// 中间多状态节点的图类
struct SetGraph {
	
	vector<NodeSet*> _nsps;	// 中间多状态节点集 
	
	vector<NodeSetEdge*> _seps;	//中间多状态节点的边的边集 
	
	// 唯一插入
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


// NFA 到 DFA 确定化的转化类
class NFA_TO_DFA {
	
	private:
		
		Graph *_pNfa;	// 待确定化的NFA
		
		Graph *_pDfa;	// 确定化后的DFA 

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

