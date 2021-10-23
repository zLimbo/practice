#include <iostream>
#include <string>
#include <vector>
using namespace std;


int n, m;


struct Object {
	string name;
	int kind;
	string value1;
	vector<Object> value2;

	Object(string name_, string value1_):
		name(name_), value1(value1_), kind(0) { }

	Object(string name_): name(name_), kind(1) { }
};


vector<Object> jsonVec;


string getKindString(Object &obj) {
	if (obj.kind == 0) return "STRING";
	else return "OBJECT";
}

inline void readString(string::iterator &it, string &s) {
	while (*(++it) != '\"') {
		if (*it == '\\') ++it;
		s.push_back(*it);
	}
}

Object getObj(string::iterator &it) {
	if (*it == '\"') {
		string name;
		readString(it, name);
		it += 3;
		if (*it == '\"') {
			string value;
			readString(it, value);
			return Object(name, value);
		} else if (*it == '{') {
			Object obj(name);
			do {
				obj.value2.push_back(getObj(++it));
				cout << "value2 : " << obj.value2.back().name << endl;
			} while (*(++it) == ',');
			return obj;
		}
	}
}


void readJSON() {
	string jsonStr;
	for (int i = 0; i < n; ++i) {
		string s;
		getline(cin, s);
		jsonStr.append(s);
	}
	cout << "# " << jsonStr << endl;
	string::iterator it = jsonStr.begin();

	do {
		jsonVec.push_back(getObj(++it));
		cout << "$ " << jsonVec.back().name << endl;
		cout << "$ " << *it << " " << *(it+1) << endl;
	} while (*(++it) == ',');
}

void printObj(Object &obj) {
	if (obj.kind == 0) {
		cout << "\"" << obj.name << "\": \"" << obj.value1 << "\","<< endl;
	} else {
		cout << obj.name << ": {" << endl;
		for (Object &objc: obj.value2) printObj(objc);
		cout << "}," << endl;
	}
}

int main(int argc, char** argv) {

	freopen("in.txt", "r", stdin);

	cin >> n >> m;
	cin.get();

	readJSON();

	cout << "{" << endl;
	for (Object &obj: jsonVec) {
		printObj(obj);
	}
	cout << "}" << endl;

	return 0;
}
