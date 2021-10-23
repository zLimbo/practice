#include "LL1Grammar.h"

#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// ���������������ʽ�������ķ� 
LL1Grammar::LL1Grammar(const string &fileName) {
	
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


// ����µķ��ս�� 
char LL1Grammar::getNewVn(unordered_set<char>& VN2) const {
	for (char ch = 'A'; ch <= 'Z'; ++ch) 
		if (VN2.find(ch) == VN2.end()) return ch;
	return '0';
} 


// ������ݹ� 
bool LL1Grammar::eliminateLeftRecursion() {
	
	vector<char> VN2(VN.begin(), VN.end());
	sort(VN2.begin(), VN2.end());
	
	do {	// ����ȫ���ж�����������������ݹ��������� 
	
		multimap<char, string> P2(P);		// �ڸ����ϲ���������ԭ����ʽ���� 
		unordered_set<char> VN3(VN);
		unordered_map<char, int> leftRecursionType;	// ��ݹ����� 
		
		for (auto iter = VN2.begin(); iter != VN2.end(); ++iter) {
			char vn = *iter;
			auto range = P2.equal_range(vn);
			leftRecursionType[vn] = 0;		// �ݹ����ͳ�ʼ��Ϊ�޵ݹ� 
			
			// ����ǰ���ս��Ϊ��ߵ����в���ʽ���Ҳ���� 
			queue<string> rightQueue;
			for ( ; range.first != range.second; ++range.first) {
				string &right = range.first->second;
				if (right[0] == vn) leftRecursionType[vn] |= 1;  // �����������һλ��Ϊ 1����Ϊ����ֱ����ݹ� 
				rightQueue.push(right);	
			}
			 
			// rightRecurVec�洢�Ҳ�����ĸΪ vn �Ĳ���ʽ���Ҳ�������ݹ��Ҳ����� rightNoRecurVec�洢���� 
			vector<string> rightRecurVec, rightNoRecurVec;
			 
			while (!rightQueue.empty()) {		 
				string right = rightQueue.front(); rightQueue.pop();
				
				// ��ǰ����ʽ�Ҳ�����ĸ�ڵ�ǰ���ս��ǰ�������滻 
				if (isVN(right[0]) && find(VN2.begin(), iter, right[0]) != iter) {
					auto range2 = P2.equal_range(right[0]);		// ����Ҳ�����ĸΪ�󲿵����в���ʽ 
					for (auto iter = range2.first; iter != range2.second; ++iter) {
						// �����滻 
						string right2;
						if (iter->second == "#") right2 = right.substr(1);
						else right2 = iter->second + right.substr(1);
						// Ϊ������Ϊ"#" 
						if (right2.empty()) right2 = "#";

						rightQueue.push(right2); 	// �滻����²���ʽ���Ҳ���� 
						if (right2[0] == vn) leftRecursionType[vn] |= 2;	// ����������ڶ�λ��Ϊ 1����Ϊ���ּ����ݹ� 
					}
				} else {
					if (right[0] ==  vn) rightRecurVec.push_back(right);	// ����ݹ�� rightRecurVec 
					else rightNoRecurVec.push_back(right);					// ������ rightNoRecurVec 
				}
			}
			// �� vn �ַ�������ݹ飬�����滻������ԭ����ʽ�����䶯 
			if (leftRecursionType[vn] != 0) {	
				// �� vn Ϊ�󲿵����в���ʽɾȥ 
				auto range3 = P2.equal_range(vn);
				P2.erase(range3.first, range3.second);
				
				// ��ÿ������ݹ�Ĳ���ʽ���������������µĲ���ʽ���� P2 �� 
				for (string rR: rightRecurVec) {
					char newVn = getNewVn(VN3); 
					VN3.insert(newVn);
					P2.emplace(newVn, rR.substr(1) + newVn);
					P2.emplace(newVn, "#"); 
					for (string nR: rightNoRecurVec) 
						P2.emplace(vn, nR + newVn);
				}
			}
		}
		
		//  ��ӡ��Ϣ 
		cout << "��ǰ˳��"; 
		for (char vn: VN2) cout << vn << " "; cout << endl;
		cout << endl;
		for (char vn: VN2) {
			cout << vn;
			switch (leftRecursionType[vn]) {
				case 0: cout << " ������ݹ�"; break;
				case 1: cout << " ����ֱ����ݹ�"; break;
				case 2: cout << " ���м����ݹ�"; break;
				case 3: cout << " ����ֱ����ݹ�ͼ����ݹ�"; break;
			}
			cout << endl;
		}
		cout << "\n������ݹ��Ĳ���ʽ��" << endl; 
		for (auto p: P2) cout << p.first << "--->" << p.second << endl;
		cout << "\n" << endl;	
		
	} while ((next_permutation(VN2.begin(), VN2.end())));	// ��һ�����У����򷵻� 0 ���� 
		
	return true;	
} 
