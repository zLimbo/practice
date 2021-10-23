#include "LL1Grammar.h"

#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// 构造器，读入产生式，生成文法 
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


// 显示相关信息 
void LL1Grammar::showGrammar() const {
	
	cout << "文法 G" << endl;
	cout << "开始符号：" << S << endl; 
	cout << "非终结符："; 
	for (char ch: set<char>(VN.begin(), VN.end())) cout << ch << " "; cout << endl;
	cout << "终结符："; 
	for (char ch: set<char>(VT.begin(), VT.end())) if (ch != '#') cout << ch << " "; cout << endl; 
	cout << "产生式：" << endl; 
	for (const pair<char, string>& p: P) cout << p.first << "-->" << p.second << endl;
}


// 获得新的非终结符 
char LL1Grammar::getNewVn(unordered_set<char>& VN2) const {
	for (char ch = 'A'; ch <= 'Z'; ++ch) 
		if (VN2.find(ch) == VN2.end()) return ch;
	return '0';
} 


// 消除左递归 
bool LL1Grammar::eliminateLeftRecursion() {
	
	vector<char> VN2(VN.begin(), VN.end());
	sort(VN2.begin(), VN2.end());
	
	do {	// 利用全排列对所有组合情况进行左递归消除操作 
	
		multimap<char, string> P2(P);		// 在副本上操作，保持原产生式不变 
		unordered_set<char> VN3(VN);
		unordered_map<char, int> leftRecursionType;	// 左递归类型 
		
		for (auto iter = VN2.begin(); iter != VN2.end(); ++iter) {
			char vn = *iter;
			auto range = P2.equal_range(vn);
			leftRecursionType[vn] = 0;		// 递归类型初始化为无递归 
			
			// 将当前非终结符为左边的所有产生式的右部入队 
			queue<string> rightQueue;
			for ( ; range.first != range.second; ++range.first) {
				string &right = range.first->second;
				if (right[0] == vn) leftRecursionType[vn] |= 1;  // 将自右往左第一位置为 1，意为出现直接左递归 
				rightQueue.push(right);	
			}
			 
			// rightRecurVec存储右部首字母为 vn 的产生式的右部（即左递归右部）， rightNoRecurVec存储其他 
			vector<string> rightRecurVec, rightNoRecurVec;
			 
			while (!rightQueue.empty()) {		 
				string right = rightQueue.front(); rightQueue.pop();
				
				// 当前产生式右部首字母在当前非终结符前，可以替换 
				if (isVN(right[0]) && find(VN2.begin(), iter, right[0]) != iter) {
					auto range2 = P2.equal_range(right[0]);		// 获得右部首字母为左部的所有产生式 
					for (auto iter = range2.first; iter != range2.second; ++iter) {
						// 进行替换 
						string right2;
						if (iter->second == "#") right2 = right.substr(1);
						else right2 = iter->second + right.substr(1);
						// 为空则设为"#" 
						if (right2.empty()) right2 = "#";

						rightQueue.push(right2); 	// 替换后的新产生式的右部入队 
						if (right2[0] == vn) leftRecursionType[vn] |= 2;	// 将自右往左第二位置为 1，意为出现间接左递归 
					}
				} else {
					if (right[0] ==  vn) rightRecurVec.push_back(right);	// 含左递归进 rightRecurVec 
					else rightNoRecurVec.push_back(right);					// 不含进 rightNoRecurVec 
				}
			}
			// 若 vn 字符出现左递归，进行替换，否则原产生式不做变动 
			if (leftRecursionType[vn] != 0) {	
				// 将 vn 为左部的所有产生式删去 
				auto range3 = P2.equal_range(vn);
				P2.erase(range3.first, range3.second);
				
				// 对每个含左递归的产生式进行消除，产生新的产生式插入 P2 中 
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
		
		//  打印信息 
		cout << "当前顺序："; 
		for (char vn: VN2) cout << vn << " "; cout << endl;
		cout << endl;
		for (char vn: VN2) {
			cout << vn;
			switch (leftRecursionType[vn]) {
				case 0: cout << " 不含左递归"; break;
				case 1: cout << " 含有直接左递归"; break;
				case 2: cout << " 含有间接左递归"; break;
				case 3: cout << " 含有直接左递归和间接左递归"; break;
			}
			cout << endl;
		}
		cout << "\n消除左递归后的产生式：" << endl; 
		for (auto p: P2) cout << p.first << "--->" << p.second << endl;
		cout << "\n" << endl;	
		
	} while ((next_permutation(VN2.begin(), VN2.end())));	// 下一个排列，否则返回 0 跳出 
		
	return true;	
} 
