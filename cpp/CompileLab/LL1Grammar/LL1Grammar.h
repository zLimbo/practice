#ifndef __LL1GRAMMER_H__ 
#define __LL1GRAMMER_H__

#include <unordered_set> 
#include <unordered_map>
#include <map>		
#include <string>

using namespace std;

//  ������ 
enum EmptyType { 
	TRUE, 		// �ܵ����� 
	FALSE, 		// ���ܵ����� 
	UNCERTAIN 	// δ֪ 
};


class LL1Grammar {
	
	private:
		
		multimap<char, string> P;	// ����ʽ 
		unordered_set<char> VN;		// ���ս������ 
		unordered_set<char> VT;		// �ս������ 
		char S;						// ��ʼ���� 
		
		unordered_map<char, EmptyType> emptyMap;			// �ܷ񵼳���ӳ��� 
		unordered_map<char, unordered_set<char>> firstMap;	// FIRST��ӳ��� 
		unordered_map<char, unordered_set<char>> followMap;	// FOLLOW��ӳ��� 
		map<pair<char, string>, unordered_set<char>> selectMap;	// FOLLOW��ӳ��� 
		
	private:
		
		bool expandProd(const pair<char, string> &p, 
				unordered_set<char> vuSet, multimap<char, string> &P2);
		
		void mergeProd();
	public:
		
		// ������ 
		LL1Grammar(const string& fileName); 
		
		// ���Ƶ����յ�ӳ�� 
		void inferToEmpty();
		
		// ����FIRST�� 
		void generateFirst();
		
		// ����FOLLOW�� 
		void generateFollow();
		
		// ����SELECT�� 
		void generateSelect();
			
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
