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

static int assignId = 0;	// �Զ����� ID 


// �����ķ��� 
struct RegularGrammer {
	unordered_set<char> VN;				// ���ս������ 
	unordered_set<char> VT;				// �ս������ 
	multimap<string, string> P;			// ����ʽ���� 
	string S;							// ��ʼ���� 
};


struct Edge;


// �ڵ� 
struct Node {
	int id;		 
	vector<Edge*> eps;				// �߼��� 
	vector<string> starStrs;		// �� * �ķ��Ŵ����� 
	Node(int id_ = -1): id(id_) {
		if (id_ == -1) id = assignId++;
	}
};


// �� 
struct Edge {
	Node* from;		 	 
	Node* to;
	string re;		// ��ǰ�ߵ�����ʽ 
	
	Edge(Node* f = nullptr, Node* t = nullptr, const string& r = string()): 
		from(f), to(t), re(r) 
	{ }
};


// ת������ 
enum TransformType { RG_TO_RE, RE_TO_RG };


// ����ת���� 
class RegularTransformation {
	
	private:
		
		// �����ķ� 
		RegularGrammer _RG;
		
		// ����ʽ 
		string _RE;
		
		// NFA ��㼯 
		vector<Node*> _nps;
		
		// NFA �߼� 
		vector<Edge*> _eps;
		
		// ת������ 
		TransformType _transformType;
		
	private:
	 
		// ���������ķ� 
		void readRG(const string& fileName);
		
		// ��������ʽ 
		void readRE(const string& fileName);
		
		// �����ķ�������ʽ 
		void rg_to_re();
	
		// ����ʽ�������ķ�	
		void re_to_rg();
		
		// ���ݲ���ʽ���Ѱ�ҽ�� 
		Node* findNode(const string& left);
		
		// �Ӹý�������������� �������ķ�������ʽ ��  
		string shrink(Node* np, vector<Edge*> preEdges, Node* zp);
		
		// �Ӹý����չ�������� ������ʽ�������ķ���
		void expand(Edge* ep);
		
	public:
		
		// ��������ȷ��ת������ 
		RegularTransformation(TransformType transformType, const string& fileName);
	
		// ���������ͷŶ�̬�ڴ� 
		~RegularTransformation() {
			for (Node* np: _nps) delete np;
			for (Edge* ep: _eps) delete ep;
		}
		
		// ���� 
		void run();
		
		// ��ӡ�����ķ� 
		void printRG();
};

#endif 
