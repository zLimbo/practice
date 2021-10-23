#ifndef __LL1GRAMMER_H__ 
#define __LL1GRAMMER_H__

#include <unordered_set> 
#include <unordered_map>
#include <map>		
#include <string>

using namespace std;

//  空类型 
enum EmptyType { 
	TRUE, 		// 能导出空 
	FALSE, 		// 不能导出空 
	UNCERTAIN 	// 未知 
};


class LL1Grammar {
	
	private:
		
		multimap<char, string> P;	// 产生式 
		unordered_set<char> VN;		// 非终结符集合 
		unordered_set<char> VT;		// 终结符集合 
		char S;						// 开始符号 
		
		unordered_map<char, EmptyType> emptyMap;			// 能否导出空映射表 
		unordered_map<char, unordered_set<char>> firstMap;	// FIRST集映射表 
		unordered_map<char, unordered_set<char>> followMap;	// FOLLOW集映射表 
		map<pair<char, string>, unordered_set<char>> selectMap;	// SELECT集映射表 		
		
	private:
		
		// 计算某一非终结符的FIRST集 
		unordered_set<char> calFirst(char ch);
		
	public:
		
		// 构造器 
		LL1Grammar(const string& fileName); 
		
		// 能推倒出空的映射 
		void inferToEmpty();
		
		// 生成FIRST集 
		void generateFirst();
		
		// 生成FOLLOW集 
		void generateFollow();
		
		// 生成SELECT集 
		void generateSelect();
			
		// 是否是非终结符 
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }
		
		// 是否是终结符 
		bool isVT(char ch) const { return !isVN(ch) && ch != '#';}
		
		bool isLL1() const;
		
		// 显示相关信息 
		void showGrammar() const;
};

#endif
