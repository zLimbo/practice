#include "GrammerJudge.h"


// ���캯�� 
GrammerJudge::GrammerJudge(const string& fileName_) {
	// ���ļ��ж�������
	ifstream fin(fileName_);	 
	// �������ʽ��Ŀ�����жϷ��Ŵ���Ŀ 
	int productionNum, symbolStringNum; 
	fin >> productionNum >> symbolStringNum;	
	// �������ʽ 
	for (int i = 0; i < productionNum; ++i) {
		pair<string, string> p;
		fin >> p.first >> p.second;
		_G.P.push_back(p);
		if (i == 0) _G.S = p.first;
	}
	// ͳ���ս�������ս�� 
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
	// ��¼���жϵķ��Ŵ� 
	fin.get(); // �ų����з����� 
	for (int i = 0; i < symbolStringNum; ++i) {
		string symbolString;
		getline(fin, symbolString);
		_symbolStrings.push_back(move(symbolString));
	}
	// �ļ��ر� 
	fin.close();
	// �ж��ķ����� 	
	judgeGrammerType();	
}



// �ж��ķ����� 
void GrammerJudge::judgeGrammerType() {
	_G.type = 3; 
	_G.linearType = 0; // 1 Ϊ�����ԣ�2Ϊ������ 
	for (auto it = _G.P.begin(); it != _G.P.end(); ++it) {	// ��ÿ������ʽ�����ж� 
		const pair<string, string> &p = *it;	// ����ʽ 
		
		if (_G.type >= 1) {	 	// ֮ǰ�Ĳ���ʽ������1���ķ���Ҫ�� 
			if (p.first.size() > p.second.size()) {	// 1���ķ��ж� 
				_G.type = 0;
				break;
			}
		} 
		
		if (_G.type >= 2) {		// ֮ǰ�Ĳ���ʽ������2���ķ���Ҫ�� 
			if (p.first.size() != 1) {	// 2���ķ��ж� 
				_G.type = 1;	
				continue;
			}
		}
				
		if (_G.type >= 3) {		// ֮ǰ�Ĳ���ʽ������3���ķ���Ҫ�� 
			if (p.second.size() == 1 && _G.VT.count(p.second[0])) {	// �ұߵ����ַ� ����Ϊ�ս�� 
				;
			} else if (p.second.size() == 2) {	// �ұ������ַ� 
				if (_G.VT.count(p.second[0]) && _G.VN.count(p.second[1])) {	//������ 
					if (_G.linearType == 0) _G.linearType = 2;	//���ݵ�һ���ж���������Ϊ������ 
					else if (_G.linearType == 1) _G.type = 2; // ����������
				} else if (_G.VT.count(p.second[1]) && _G.VN.count(p.second[0])) {	//������ 
					if (_G.linearType == 0) _G.linearType = 1;	//���ݵ�һ���ж���������Ϊ������  
					else if (_G.linearType == 2) _G.type = 2; // ����������
					
				} else {
					_G.type = 2;
				}	
			} else {
				_G.type = 2;
			}
		}
	}
}




// �����Լ 
bool GrammerJudge::leftFirstDeduction(
		const string& symbolString_, string& praise_, unordered_set<string>& errorStrs_) {
	// ��Լ��ֹ���������ڿ�ʼ���� 
	if (symbolString_ == _G.S) return true;
	int len = symbolString_.size();
	for (int subPos = 0; subPos < len; ++subPos) {
		int subLenCeil = len - subPos;
		for (int subLen = 1; subLen <= subLenCeil; ++subLen) {
			// ��õ�ǰ����ֱ���ִ����Ƶ���˼��� 
			auto prodStrs = _auxMap->equal_range(symbolString_.substr(subPos, subLen));
			for (; prodStrs.first != prodStrs.second; ++prodStrs.first) {
				string deducStr = symbolString_;
				// ��Լ�滻 
				deducStr.replace(subPos, subLen, prodStrs.first->second); 
				// ���󼯺ϲ�����
				if (errorStrs_.find(deducStr) != errorStrs_.end()) continue;	 
				// �Թ�Լ�滻����ַ������еݹ������Լ 
				if (leftFirstDeduction(deducStr, praise_, errorStrs_)) {
					praise_ += deducStr + "==>";
					return true;
				}
			} // while (prodStrs.first != prodStrs.second)
		} // for (int subLen = 1; subLen <= subLenCeil; ++subLen)
	} // for (int subPos = 0; subPos < len; ++subPos)
	errorStrs_.insert(symbolString_);	// �����Լ����ķ��Ŵ����� 
	return false;
}


// �ж��Ƿ��Ǿ��ӣ�ǰ����symbolString_�Ѿ��Ǿ��ͣ� 
bool GrammerJudge::judgeSentence(const string& symbolString_) {
	for (char ch: symbolString_) if (_G.VN.count(ch)) return false;
	return true;
}


// �жϴ����ķ��Ŵ��Ƿ��Ǿ��� 
void GrammerJudge::judgePattern() {
	_auxMap = new multimap<string, string>();
	for (pair<string, string> p: _G.P) 
		_auxMap->insert(make_pair(p.second, p.first));
	
	for (string &symbolString: _symbolStrings) {
		string praise;	// �Ƶ�ʽ 
		int symbolStringType = 0;
		unordered_set<string> errorStrs;
		if (leftFirstDeduction(symbolString, praise, errorStrs)) {
			praise += symbolString;
			symbolStringType = 1;
			if (judgeSentence(symbolString)) symbolStringType = 2;
		} 
		
		if (symbolStringType >= 1) {
			cout << "���Ŵ� " << symbolString << " �Ǹ��ķ��ľ���";
			if (symbolStringType == 2) cout << ",���Ǹ��ķ��ľ���";
			cout << "\n    ���Ƶ�ʽΪ��" << praise << endl; 
		} else {
			cout << "���Ŵ� " << symbolString << " ���Ǹ��ķ��ľ���" << endl; 
		}
		cout << endl; 
	}
	
	delete _auxMap;	// �ͷ�������Դ 
	_auxMap = nullptr; 
}


// ����ķ� 
void GrammerJudge::printGrammer() {
	cout << "�ķ� G(VN, VT, P, S)" << endl;
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
	cout << "\n���ķ���0";
	for (int i = 1; i <= _G.type; ++i) {
		cout << "�� " << i;
	}
	cout << "���ķ�" << endl; 
}


//// �ж�һ�����Ŵ��Ƿ���������ķ�
//// p: �÷��Ŵ��� lineType: ���ķ���������(1Ϊ�����ԣ�2Ϊ�����ԣ� 
//bool GrammerJudge::judge3Grammer(const pair<string, string>& p_) {
//	if (p_.second.size() == 1 && _G.VT.count(p_.second[0])) {	// �ұߵ����ַ� ����Ϊ�ս�� 
//		return true;
//	}
//	if (p_.second.size() == 2) {	// �ұ������ַ� 
//		if (_G.VT.count(p_.second[0]) && _G.VN.count(p_.second[1])) {	//������ 
//			if (_G.linearType == 1) return false; // ����������
//			if (_G.linearType == 0) _G.linearType = 2;	//���ݵ�һ���ж���������Ϊ������ 
//			return true; 
//		} else if (_G.VT.count(p_.second[1]) && _G.VN.count(p_.second[0])) {	//������ 
//			if (_G.linearType == 2) return false; // ����������
//			if (_G.linearType == 0) _G.linearType = 1;	//���ݵ�һ���ж���������Ϊ������  
//			return true;
//		}	
//	} 
//	return false; // �������� 
//}
//
//
//// �ж��ķ����� 
//void GrammerJudge::judgeGrammerType() {
//	bool is1G = true, is2G = true, is3G = true;	// �ķ����� 
//	_G.linearType = 0; // 1 Ϊ�����ԣ�2Ϊ������ 
//	for (auto it = _G.P.begin(); it != _G.P.end(); ++it) {	// ��ÿ������ʽ�����ж� 
//		const pair<string, string> &p = *it;	// ����ʽ 
//		if (is1G) {
//			if (p.first.size() <= p.second.size()) {	// 1���ķ��ж� 
//			 	if (is2G) {
//				 	if (p.first.size() == 1) {	// 2���ķ��ж� 
//						if (is3G) 	 
//							is3G = judge3Grammer(p);	// 3���ķ��ж�
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
