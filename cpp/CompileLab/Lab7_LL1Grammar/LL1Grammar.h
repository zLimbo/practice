#ifndef __LL1GRAMMER_H__ 
#define __LL1GRAMMER_H__

#include <unordered_set> 
#include <unordered_map>
#include <map>		
#include <string>

using namespace std;

class LL1Grammar {
	
	private:
		
		multimap<char, string> P;	// ����ʽ 
		unordered_set<char> VN;		// ���ս������ 
		unordered_set<char> VT;		// �ս������ 
		char S;						// ��ʼ���� 
		
	public:
		
		// ������ 
		LL1Grammar(const string& fileName); 
			
		// �Ƿ��Ƿ��ս�� 
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }
		
		// �Ƿ����ս�� 
		bool isVT(char ch) const { return !isVN(ch) && ch != '#';}
		
		// ��ʾ�����Ϣ 
		void showGrammar() const; 
		
		// ����µķ��ս�� 
		char getNewVn(unordered_set<char>& VN2) const;
		
		// ����ֱ����ݹ� 
		bool eliminateLeftRecursion();
};

#endif
