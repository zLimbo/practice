#include "LL1Grammar.h"

#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

// ���������������ʽ�������ķ� 
LL1Grammar::LL1Grammar(const string& fileName) {
	
	ifstream fin(fileName);
	
	char left;
	string right;
	int n = 0;
	
	while (fin >> left >> right) {
		P.emplace(left, right);
		VN.insert(left);
		for (char ch: right) {
			if (ch >= 'A' && ch <= 'Z') VN.insert(ch);
			else VT.insert(ch);
		}
		if (n++ == 0) S = left;
	} 	
} 


// ��ʾ�����Ϣ 
void LL1Grammar::showGrammar() const {
	
	cout << "�ķ� G" << endl;
	cout << "��ʼ���ţ�" << S << endl; 
	cout << "���ս����"; 
	for (char ch: set<char>(VN.begin(), VN.end())) cout << ch << " "; cout << endl;
	cout << "�ս����"; 
	for (char ch: set<char>(VT.begin(), VT.end())) if (ch != '#') cout << ch << " "; cout << endl; 
	cout << "����ʽ��" << endl; 
	for (const pair<char, string>& p: P) cout << p.first << "-->" << p.second << endl;
}

bool LL1Grammar::expandProd(const pair<char, string> &p, 
		unordered_set<char> vuSet, multimap<char, string> &P2) {
	
	char firstChar = p.second[0];
	
	if (!isVN(firstChar)) {

		P2.insert(p);
		return true;
	}
	
	if (vuSet.find(firstChar) != vuSet.end()) {
		cout << "\nѭ���滻" << endl;
		return false;
	}
	
	vuSet.insert(firstChar);
	
	auto range = P.equal_range(firstChar);
	
	for ( ; range.first != range.second; ++range.first) {
		string& right = range.first->second;
		string newRight;
		
		if (right == "#") newRight = p.second.substr(1);
		else newRight = right + p.second.substr(1);
		
		pair<char, string> newP(p.first, newRight);
		
	//	cout << "### " << newP.first << "--->" << newP.second << endl;
		if (!expandProd(newP, vuSet, P2)) return false;
	}
	return true;
}


// �ϲ���ͬ���ս�� 
void LL1Grammar::mergeProd() {
	
	unordered_set<char> VN2(VN);		// ���ս������
	multimap<char, string> P2;			// ����ʽ 
	
	P.swap(P2);
	
	while (true) {
		multimap<char, string> P3;	// ����ʽ 
	 	unordered_set<char> VN3;		// ���ս������
	 	
		for (char vn: VN2) {
			auto range = P2.equal_range(vn);
			map<char, vector<string>> auxMap;
			for ( ; range.first != range.second; ++range.first) 
				auxMap[range.first->second[0]].push_back(range.first->second);
	
			for (auto auxPs: auxMap) {
				vector<string> &auxVec = auxPs.second;
				if (auxVec.size() < 2) {
					P.emplace(vn, auxVec[0]);
				} else {
					int pos = 0;
					bool isSame = true;
					while (isSame) {
						++pos;
						for (int i = 1; i < auxVec.size(); ++i) {
							if (auxVec[i][pos] != auxVec[0][pos]) {
								isSame = false;
								break;
							}
						}
					} 
					
					char newVn = getNewVn();
					
					P.emplace(vn, auxVec[0].substr(0, pos) + newVn);
					
					for (string &right: auxVec) {
						string newRight = right.substr(pos);
						if (newRight.empty()) P3.emplace(newVn, "#");
						else P3.emplace(newVn, newRight);
					}
					
					VN.insert(newVn);
					VN3.insert(newVn);
				} 
			}
		}
		if (VN3.empty()) break;
		P2.swap(P3);
		VN2.swap(VN3);	
	}
}


// ת��ΪLL1�ķ� 
bool LL1Grammar::convertToLL1() {

	int cnt = 0;
	while (true) {
		
		multimap<char, string> P2;			// ����ʽ 
		
		int RightFirstVnNum = 0;;
		for (const pair<char, string> &p: P)
			if (isVN(p.second[0])) ++RightFirstVnNum;
			
		if (RightFirstVnNum > 0) {
			for (const pair<char, string> &p: P) {
				unordered_set<char> vuSet;
				vuSet.insert(p.first);
				if (!expandProd(p, vuSet, P2)) return false;
			}
			P.swap(P2);
		} 
	
		mergeProd();	// �ϲ���ͬ���ս�� 
		
		int RightFirstVnNum2 = 0;;
		for (const pair<char, string> &p: P)
			if (isVN(p.second[0])) ++RightFirstVnNum2;
		
		// �ж��Ƿ�����滻���ս�� 
		if (RightFirstVnNum2 == 0) break;
		
	
		// ���滻����4�����ϣ���Ϊ�޷�ת��
		if (++cnt == 4) {
			cout << "\n�����滻" << endl;
			return false;
		} 	
	} 
	
	return true;
} 


// ����µķ��ս�� 
char LL1Grammar::getNewVn() const {
	for (char ch = 'A'; ch <= 'Z'; ++ch) 
		if (VN.find(ch) == VN.end()) return ch;
	return '0';
} 

