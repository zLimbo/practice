#ifndef __LL1GRAMMER_H__ 
#define __LL1GRAMMER_H__

#include <unordered_set> 
#include <unordered_map>
#include <map>		
#include <string>

using namespace std;

class LL1Grammar {
	
	private:
		
		multimap<char, string> P;	// 产生式 
		unordered_set<char> VN;		// 非终结符集合 
		unordered_set<char> VT;		// 终结符集合 
		char S;						// 开始符号 
		
	public:
		
		// 构造器 
		LL1Grammar(const string& fileName); 
			
		// 是否是非终结符 
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }
		
		// 是否是终结符 
		bool isVT(char ch) const { return !isVN(ch) && ch != '#';}
		
		// 显示相关信息 
		void showGrammar() const; 
		
		// 获得新的非终结符 
		char getNewVn(unordered_set<char>& VN2) const;
		
		// 消除直接左递归 
		bool eliminateLeftRecursion();
};

#endif
