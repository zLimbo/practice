#ifndef __GRAMMERJUDGE__
#define __GRAMMERJUDGE__

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>
#include <vector>
#include <map>

using namespace std;


// 文法类 
struct Grammer {
	unordered_set<char> VN;				// 非终结符集合 
	unordered_set<char> VT;				// 终结符集合 
	vector<pair<string, string>> P;		// 产生式集合 
	string S;							// 开始符号 
	int type;							// 文法类型 
	int linearType;						// 文法线性（仅当文法为3型文法才有意义） 
};


// 文法判断类 
class GrammerJudge {

	private:
		
		Grammer _G;						// 文法 
		
		vector<string> _symbolStrings;	// 待判断的符号串集合 
		
		multimap<string, string> *_auxMap = nullptr;	//辅助映射 
		

		// 判断文法类型 
		void judgeGrammerType();
		
		// 判断是否是句子 
		bool judgeSentence(const string& symbolString_);
		
		// 最左归约 
		bool leftFirstDeduction(const string& symbolString_, string& praise_, unordered_set<string>& errorStrs_);
		
	public:
		
		// 构造器 
		GrammerJudge(const string& fileName_);
		
		// 打印文法 
		void printGrammer();
		
		//  判断句型和句型推导 
		void judgePattern();
};

#endif
