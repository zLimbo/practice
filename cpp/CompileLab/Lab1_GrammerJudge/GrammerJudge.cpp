#include "GrammerJudge.h"


// 构造函数 
GrammerJudge::GrammerJudge(const string& fileName_) {
	// 从文件中读入数据
	ifstream fin(fileName_);	 
	// 读入产生式数目，带判断符号串数目 
	int productionNum, symbolStringNum; 
	fin >> productionNum >> symbolStringNum;	
	// 读入产生式 
	for (int i = 0; i < productionNum; ++i) {
		pair<string, string> p;
		fin >> p.first >> p.second;
		_G.P.push_back(p);
		if (i == 0) _G.S = p.first;
	}
	// 统计终结符、非终结符 
	for (auto it = _G.P.begin(); it != _G.P.end(); ++it) {
		for (char ch: it->first) {
			if (ch >= 'A' && ch <= 'Z') _G.VN.insert(ch);
			else _G.VT.insert(ch);
		}
		for (char ch: it->second) {
			if (ch >= 'A' && ch <= 'Z') _G.VN.insert(ch);
			else _G.VT.insert(ch);
		}
	}
	// 记录待判断的符号串 
	fin.get(); // 排除换行符干扰 
	for (int i = 0; i < symbolStringNum; ++i) {
		string symbolString;
		getline(fin, symbolString);
		_symbolStrings.push_back(move(symbolString));
	}
	// 文件关闭 
	fin.close();
	// 判断文法类型 	
	judgeGrammerType();	
}



// 判断文法类型 
void GrammerJudge::judgeGrammerType() {
	_G.type = 3; 
	_G.linearType = 0; // 1 为左线性，2为右线性 
	for (auto it = _G.P.begin(); it != _G.P.end(); ++it) {	// 对每个产生式进行判断 
		const pair<string, string> &p = *it;	// 产生式 
		
		if (_G.type >= 1) {	 	// 之前的产生式皆满足1型文法的要求 
			if (p.first.size() > p.second.size()) {	// 1型文法判断 
				_G.type = 0;
				break;
			}
		} 
		
		if (_G.type >= 2) {		// 之前的产生式皆满足2型文法的要求 
			if (p.first.size() != 1) {	// 2型文法判断 
				_G.type = 1;	
				continue;
			}
		}
				
		if (_G.type >= 3) {		// 之前的产生式皆满足3型文法的要求 
			if (p.second.size() == 1 && _G.VT.count(p.second[0])) {	// 右边单个字符 ，且为终结符 
				;
			} else if (p.second.size() == 2) {	// 右边两个字符 
				if (_G.VT.count(p.second[0]) && _G.VN.count(p.second[1])) {	//右线性 
					if (_G.linearType == 0) _G.linearType = 2;	//根据第一个判断线性类型为右线性 
					else if (_G.linearType == 1) _G.type = 2; // 非左右线性
				} else if (_G.VT.count(p.second[1]) && _G.VN.count(p.second[0])) {	//左线性 
					if (_G.linearType == 0) _G.linearType = 1;	//根据第一个判断线性类型为左线性  
					else if (_G.linearType == 2) _G.type = 2; // 非左右线性
					
				} else {
					_G.type = 2;
				}	
			} else {
				_G.type = 2;
			}
		}
	}
}




// 最左规约 
bool GrammerJudge::leftFirstDeduction(
		const string& symbolString_, string& praise_, unordered_set<string>& errorStrs_) {
	// 归约终止条件，终于开始符号 
	if (symbolString_ == _G.S) return true;
	int len = symbolString_.size();
	for (int subPos = 0; subPos < len; ++subPos) {
		int subLenCeil = len - subPos;
		for (int subLen = 1; subLen <= subLenCeil; ++subLen) {
			// 获得当前最左直接字串的推导左端集合 
			auto prodStrs = _auxMap->equal_range(symbolString_.substr(subPos, subLen));
			for (; prodStrs.first != prodStrs.second; ++prodStrs.first) {
				string deducStr = symbolString_;
				// 归约替换 
				deducStr.replace(subPos, subLen, prodStrs.first->second); 
				// 错误集合不重入
				if (errorStrs_.find(deducStr) != errorStrs_.end()) continue;	 
				// 对归约替换后的字符串进行递归最左归约 
				if (leftFirstDeduction(deducStr, praise_, errorStrs_)) {
					praise_ += deducStr + "==>";
					return true;
				}
			} // while (prodStrs.first != prodStrs.second)
		} // for (int subLen = 1; subLen <= subLenCeil; ++subLen)
	} // for (int subPos = 0; subPos < len; ++subPos)
	errorStrs_.insert(symbolString_);	// 加入规约错误的符号串集合 
	return false;
}


// 判断是否是句子（前提是symbolString_已经是句型） 
bool GrammerJudge::judgeSentence(const string& symbolString_) {
	for (char ch: symbolString_) if (_G.VN.count(ch)) return false;
	return true;
}


// 判断待定的符号串是否是句型 
void GrammerJudge::judgePattern() {
	_auxMap = new multimap<string, string>();
	for (pair<string, string> p: _G.P) 
		_auxMap->insert(make_pair(p.second, p.first));
	
	for (string &symbolString: _symbolStrings) {
		string praise;	// 推导式 
		int symbolStringType = 0;
		unordered_set<string> errorStrs;
		if (leftFirstDeduction(symbolString, praise, errorStrs)) {
			praise += symbolString;
			symbolStringType = 1;
			if (judgeSentence(symbolString)) symbolStringType = 2;
		} 
		
		if (symbolStringType >= 1) {
			cout << "符号串 " << symbolString << " 是该文法的句型";
			if (symbolStringType == 2) cout << ",还是该文法的句子";
			cout << "\n    其推导式为：" << praise << endl; 
		} else {
			cout << "符号串 " << symbolString << " 不是该文法的句型" << endl; 
		}
		cout << endl; 
	}
	
	delete _auxMap;	// 释放容器资源 
	_auxMap = nullptr; 
}


// 输出文法 
void GrammerJudge::printGrammer() {
	cout << "文法 G(VN, VT, P, S)" << endl;
	{
		cout << "VN = { ";
		auto it = _G.VN.begin();
		cout << *it;
		for (++it; it != _G.VN.end(); ++it) cout << ", " << *it;
		cout << " }" << endl;
	}	
	{
		cout << "VT = { ";
		auto it =  _G.VT.begin();
		cout << *it;
		for (++it; it != _G.VT.end(); ++it) cout << ", " << *it;
		cout << " }" << endl;	
	}
	{
		cout << "P = {" << endl;
		auto it = _G.P.begin();
		cout << "    " << it->first << "-->" << it->second;
		for (++it; it != _G.P.end(); ++it) {
			cout << ",\n    " << it->first << "-->" << it->second;
		}
		cout << "\n}" << endl;
		
	}
	cout << "S = " << _G.S << endl;
	cout << "\n该文法是0";
	for (int i = 1; i <= _G.type; ++i) {
		cout << "、 " << i;
	}
	cout << "型文法" << endl; 
}


//// 判断一个符号串是否符合三型文法
//// p: 该符号串， lineType: 该文法线性类型(1为左线性，2为右线性） 
//bool GrammerJudge::judge3Grammer(const pair<string, string>& p_) {
//	if (p_.second.size() == 1 && _G.VT.count(p_.second[0])) {	// 右边单个字符 ，且为终结符 
//		return true;
//	}
//	if (p_.second.size() == 2) {	// 右边两个字符 
//		if (_G.VT.count(p_.second[0]) && _G.VN.count(p_.second[1])) {	//右线性 
//			if (_G.linearType == 1) return false; // 非左右线性
//			if (_G.linearType == 0) _G.linearType = 2;	//根据第一个判断线性类型为右线性 
//			return true; 
//		} else if (_G.VT.count(p_.second[1]) && _G.VN.count(p_.second[0])) {	//左线性 
//			if (_G.linearType == 2) return false; // 非左右线性
//			if (_G.linearType == 0) _G.linearType = 1;	//根据第一个判断线性类型为左线性  
//			return true;
//		}	
//	} 
//	return false; // 都不满足 
//}
//
//
//// 判断文法类型 
//void GrammerJudge::judgeGrammerType() {
//	bool is1G = true, is2G = true, is3G = true;	// 文法类型 
//	_G.linearType = 0; // 1 为左线性，2为右线性 
//	for (auto it = _G.P.begin(); it != _G.P.end(); ++it) {	// 对每个产生式进行判断 
//		const pair<string, string> &p = *it;	// 产生式 
//		if (is1G) {
//			if (p.first.size() <= p.second.size()) {	// 1型文法判断 
//			 	if (is2G) {
//				 	if (p.first.size() == 1) {	// 2型文法判断 
//						if (is3G) 	 
//							is3G = judge3Grammer(p);	// 3型文法判断
//					} else {
//						is2G = is3G = false;
//					}
//				}
//			} else {
//				is1G = is2G = is3G = false;
//				break; 
//			}
//		}
//	}
//	if (is3G) _G.type = 3;
//	else if (is2G) _G.type = 2;
//	else if (is1G) _G.type = 1;
//	else _G.type = 0;
//}
