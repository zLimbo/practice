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


// 提前声明 
struct Node;	
struct SubSet;

// 边类 
struct Edge {
	
	char eid;		// 边标识 
	
	Node* from;		// 出发结点 
	
	Node* to;		// 到达结点 
	
	// 直接构造器 
	Edge(char eid_, Node* from_ = nullptr, Node* to_ = nullptr):
		eid(eid_), from(from_), to(to_) 
	{ }
};


// 结点类 
struct Node {
	
	char nid;						// 结点标识 
	
	unordered_set<Edge*> outEps;	// 从该结点出发的边的集合 
	
	unordered_set<Edge*> inEps;		// 到达该结点的边的集合 
	
	SubSet* sp;						// 该结点归属子集 
	
	// 直接构造器 
	Node(char nid_): nid(nid_) { }
	
	// 获取所属子集的属性 
	map<char, SubSet*> getAttr() const {
		map<char, SubSet*> attr;
		for (Edge* ep: outEps) attr.emplace(ep->eid, ep->to->sp);
		return attr;
	}
};


// DFA类 
struct DFA {
	
	vector<Node*> nps;					// 结点集合 
	
	unordered_set<Edge*> eps;			// 边集合 
	
	Node* start;						// 开始状态 
	
	unordered_set<Node*> terminal;		// 终止状态 
	
	unordered_set<char> inputChars;		// 输入符号 
	
	//  析构器 
	~DFA() {
		for (Node* np: nps) delete np;
		for (const Edge* ep: eps) delete ep;
	} 
	
	// 根据ID获得结点，不存在则创建结点 
	Node* getNode(char nid) {
		for (Node* np: nps) if (np->nid == nid) return np;
		Node* np = new Node(nid);
		nps.push_back(np);
		return np;
	}
	
	// 删除某结点 
	void remove(Node* np);
	
	// 合并结点 
	void merge(Node* np1, Node* np2);
};


// 子集类
struct SubSet {

	unordered_set<Node*> nps;	// 该子集的结点集 
	
	map<char, SubSet*> attr;	// 属性 
	
	// 默认构造器 
	SubSet() = default;	
	
	// 直接构造器 		
	SubSet(map<char, SubSet*>& attr_):	
		attr(move(attr_))
	{ } 
}; 


// DFA简化类 
class DFASimplify {
	
	private:
		
		DFA _dfa;		// DFA 
		
	private:
	
		// 消去多余状态
		void  removeRedundantState();
		
		// 合并等价状态
		void  mergeEquivalentState(); 
		
		// 不可到达 
		void getCannotReach(unordered_set<Node*>& reduNps);
		
		// 不可终止 
		void getCannotTerminate(unordered_set<Node*>& reduNps);
		
		// 划分子集 
		void divide(SubSet* sp, vector<SubSet*>& sps);
		
		// 打印子集集合信息 
		void printSpS(vector<SubSet*>& sps) const; 
		
	public:
		
		// 直接构造器，参数为包含DFA信息的文件 
		DFASimplify(const string& fileName); 
	
		// 简化 
		void simplify();
		 
		// 打印 DFA 
		void printDFA() const;
};

#endif 
