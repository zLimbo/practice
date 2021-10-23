#include "LL1Grammar.h"

#include <fstream>
#include <iostream>
#include <set>

using namespace std;

// 构造器，读入产生式，生成文法 
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


// 得到非终结符是否能推出空的映射表 
void LL1Grammar::inferToEmpty() {
	
	for (char ch: VN) emptyMap.emplace(ch, UNCERTAIN);
	multimap<char, string> auxP;
	
	for (const pair<char, string>& p: P) {
		if (emptyMap[p.first] == TRUE) continue;
		if (p.second == "#") {
			emptyMap[p.first] = TRUE;	// 能推出空 
			continue;
		}
		bool hasVT = false;
		for (char ch: p.second) {
			if (isVT(ch)) {
				hasVT = true;
				break;
			}
		}
		if (!hasVT) auxP.insert(p);
	}
	
	for (char ch: VN) 
		if (emptyMap[ch] == UNCERTAIN && auxP.find(ch) == auxP.end()) 
			emptyMap[ch] = FALSE;

	while (!auxP.empty()) {
		
		multimap<char, string> auxP2;
		unordered_set<char> auxVN;
		
		for (const pair<char, string>& p: auxP) {
			if (emptyMap[p.first] == TRUE) continue;
			EmptyType empty = TRUE;
			for (char ch: p.second) {
				if (emptyMap[ch] != TRUE) {
					empty = emptyMap[ch];
					break;
				} 
			}
			if (empty == TRUE) emptyMap[p.first] = TRUE;
			else if (empty == FALSE) auxVN.insert(p.first);
			else auxP2.insert(p);
		}
		for (char ch: auxVN) 
			if (auxP2.find(ch) == auxP2.end()) 
				emptyMap[ch] = FALSE;
		auxP.swap(auxP2);
	}
}


// 生成FIRST集 
void LL1Grammar::generateFirst() {
	
	for (char ch: VN) firstMap.emplace(ch, unordered_set<char>());
	bool isEnd = false;
	while (!isEnd) {
		isEnd = true;
		for (const pair<char, string>& p: P) {
			unordered_set<char>& s = firstMap[p.first];
			int oldSize = s.size(); 
			bool allEmpty = true;
			for (char ch1: p.second) {
				if (!isVN(ch1)) {
					s.insert(ch1);
					allEmpty = false;
					break;
				} else {
					for (char ch2: firstMap[ch1]) if (ch2 != '#') s.insert(ch2);
					if (emptyMap[ch1] == FALSE) {
						allEmpty = false;
						break;
					}
				}	
			}
			if (allEmpty) s.insert('#');
			if (oldSize != s.size()) isEnd = false;
		}
	}
} 


// 生成FOLLOW集 
void LL1Grammar::generateFollow() {
	
	for (char ch: VN) followMap.emplace(ch, unordered_set<char>());
	followMap[S].insert('#');
	bool isEnd = false;
	while (!isEnd) {
		isEnd = true;
		for (const pair<char, string>& p: P) {
			int len = p.second.size();
			for (int i = 0; i < len; ++i) {
				char ch = p.second[i];
				if (isVN(ch)) {
					int oldSize = followMap[ch].size();
					if (i == len-1) {
						followMap[ch].insert(followMap[p.first].begin(), followMap[p.first].end());
					} else {
						char nextCh = p.second[i+1];
						if (isVT(nextCh)) followMap[ch].insert(nextCh);
						else followMap[ch].insert(firstMap[nextCh].begin(), firstMap[nextCh].end());
					}
					if (oldSize != followMap[ch].size()) isEnd = false;
				}
			}
		}	
	}
}


// 生成SELECT集 
void LL1Grammar::generateSelect() {
	
	for (const pair<char, string>& p: P) {
		unordered_set<char> s;
		
		bool allEmpty = true;
		for (char ch1: p.second) {
			if (ch1 == '#') break;
			if (isVT(ch1)) {
				s.insert(ch1);
				allEmpty = false;
				break;
			} else {
				for (char ch2: firstMap[ch1]) if (ch2 != '#') s.insert(ch2);
				if (emptyMap[ch1] == FALSE) {
					allEmpty = false;
					break;
				}
			}
		}
		if (allEmpty) 
			for (char ch2: followMap[p.first]) s.insert(ch2);
		selectMap.emplace(p, s);
	}
}


// 判断是否是LL1文法
bool LL1Grammar::isLL1() const {
	
	for (char ch: VN) {
		auto range = P.equal_range(ch);
		for (auto it1 = range.first; it1 != range.second; ++it1) {
			const unordered_set<char>& s1 = selectMap.find(*it1)->second; 
			auto it2 = it1; ++it2;
			for ( ; it2 != range.second; ++it2) {
				const unordered_set<char>& s2 = selectMap.find(*it2)->second; 
				for (char ch: s2)
					if (s1.find(ch) != s1.end())
						return false;
			}
		}	
	}
	return true;
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

	cout << "\nFIRST集：" << endl;
	for (auto f: map<char, unordered_set<char>>(firstMap.begin(), firstMap.end())) {
		cout << f.first << ": ";
		for (char ch: set<char>(f.second.begin(), f.second.end())) 
			cout << ch << " "; 
		cout << endl;
	}
	
	cout << "\nFOLLOW集：" << endl;
	for (auto f: map<char, unordered_set<char>>(followMap.begin(), followMap.end())) {
		cout << f.first << ": ";
		for (char ch: set<char>(f.second.begin(), f.second.end()))
			cout << ch << " "; 
		cout << endl;
	}
	
	cout << "\nSELECT集：" << endl;
	for (auto f: selectMap) {
		cout << f.first.first << "-->" << f.first.second << "\t: ";
		for (char ch: set<char>(f.second.begin(), f.second.end()))
			cout << ch << " "; 
		cout << endl;
	}

}

