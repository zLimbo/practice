#ifndef __REGULAR_TRANSFORMATION_H__
#define __REGULAR_TRANSFORMATION_H__

#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <iostream>

using std::string;
using std::vector;
using std::multimap;
using std::unordered_set;

static int assignId = 0;	// 自动分配 ID 


// 正规文法类 
struct RegularGrammer {
	unordered_set<char> VN;				// 非终结符集合 
	unordered_set<char> VT;				// 终结符集合 
	multimap<string, string> P;			// 产生式集合 
	string S;							// 开始符号 
};


struct Edge;


// 节点 
struct Node {
	int id;		 
	vector<Edge*> eps;				// 边集合 
	vector<string> starStrs;		// 带 * 的符号串集合 
	Node(int id_ = -1): id(id_) {
		if (id_ == -1) id = assignId++;
	}
};


// 边 
struct Edge {
	Node* from;		 	 
	Node* to;
	string re;		// 当前边的正规式 
	
	Edge(Node* f = nullptr, Node* t = nullptr, const string& r = string()): 
		from(f), to(t), re(r) 
	{ }
};


// 转化类型 
enum TransformType { RG_TO_RE, RE_TO_RG };


// 正规转化类 
class RegularTransformation {
	
	private:
		
		// 正规文法 
		RegularGrammer _RG;
		
		// 正规式 
		string _RE;
		
		// NFA 结点集 
		vector<Node*> _nps;
		
		// NFA 边集 
		vector<Edge*> _eps;
		
		// 转换类型 
		TransformType _transformType;
		
	private:
	 
		// 读入正规文法 
		void readRG(const string& fileName);
		
		// 读入正规式 
		void readRE(const string& fileName);
		
		// 正规文法到正规式 
		void rg_to_re();
	
		// 正规式到正规文法	
		void re_to_rg();
		
		// 根据产生式左边寻找结点 
		Node* findNode(const string& left);
		
		// 从该结点收缩，辅助于 “正规文法到正规式 ”  
		string shrink(Node* np, vector<Edge*> preEdges, Node* zp);
		
		// 从该结点扩展，辅助于 “正规式到正规文法”
		void expand(Edge* ep);
		
	public:
		
		// 构造器，确定转化类型 
		RegularTransformation(TransformType transformType, const string& fileName);
	
		// 析构器，释放动态内存 
		~RegularTransformation() {
			for (Node* np: _nps) delete np;
			for (Edge* ep: _eps) delete ep;
		}
		
		// 运行 
		void run();
		
		// 打印正规文法 
		void printRG();
};

#endif 
