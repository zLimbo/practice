#ifndef __GRAMMERJUDGE__
#define __GRAMMERJUDGE__

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>
#include <map>

using namespace std;


// �ķ��� 
struct Grammer {
	unordered_set<char> VN;				// ���ս������ 
	unordered_set<char> VT;				// �ս������ 
	vector<pair<string, string>> P;		// ����ʽ���� 
	string S;							// ��ʼ���� 
	int type;							// �ķ����� 
	int linearType;						// �ķ����ԣ������ķ�Ϊ3���ķ��������壩 
};


// �ķ��ж��� 
class GrammerJudge {

	private:
		
		Grammer _G;						// �ķ� 
		
		vector<string> _symbolStrings;	// ���жϵķ��Ŵ����� 
		
		multimap<string, string> *_auxMap = nullptr;	//����ӳ�� 
		

		// �ж��ķ����� 
		void judgeGrammerType();
		
		// �ж��Ƿ��Ǿ��� 
		bool judgeSentence(const string& symbolString_);
		
		// �����Լ 
		bool leftFirstDeduction(const string& symbolString_, string& praise_, unordered_set<string>& errorStrs_);
		
	public:
		
		// ������ 
		GrammerJudge(const string& fileName_);
		
		// ��ӡ�ķ� 
		void printGrammer();
		
		//  �жϾ��ͺ;����Ƶ� 
		void judgePattern();
};

#endif
