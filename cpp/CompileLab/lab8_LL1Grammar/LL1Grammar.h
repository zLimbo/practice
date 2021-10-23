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
		
	private:
		
		// �滻���ս������ʽ 
		bool expandProd(const pair<char, string> &p, 
				unordered_set<char> vuSet, multimap<char, string> &P2);
		
		// �ϲ���ͬ�ս������ʽ 
		void mergeProd();
	public:
		
		// ������ 
		LL1Grammar(const string& fileName); 
			
		// �Ƿ��Ƿ��ս�� 
		bool isVN(char ch) const { return ch >= 'A' && ch <= 'Z'; }
		
		// �Ƿ����ս�� 
		bool isVT(char ch) const { return !isVN(ch) && ch != '#';}
		
		// �Ƿ���ll1�ķ� 
		bool isLL1() const;
		
		// ��ʾ�����Ϣ 
		void showGrammar() const;
		
		// ת��ΪLL1�ķ� 
		bool convertToLL1(); 
		
		// ����µķ��ս�� 
		char getNewVn() const;
};

#endif
