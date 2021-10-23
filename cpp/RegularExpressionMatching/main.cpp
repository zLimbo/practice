#include <iostream>
#include <fstream>
using namespace std;

const auto fastIO = []() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    return nullptr;
}();

#include <set>
class Solution {
public:
    bool isMatch(string s, string p) {
        int sLen = s.size();
        int pLen = p.size();
        int sIt = 0;
        int pIt = 0;
        set<int> pc;

        if (pIt+1 < pLen && p[pIt+1] == '*') {
            if (p[pIt] == '.' || p[pIt] == s[sIt]) {
                pc.insert(pIt);
            }
            pc.insert(pIt+2);
            int dIt = pIt + 2;
            while (dIt+1 < pLen && p[dIt+1] == '*') {
                pc.insert(dIt+2);
                dIt += 2;
            }
        } else if (p[pIt] == '.' || p[pIt] == s[sIt]) {
            pc.insert(pIt+1);
            int dIt = pIt + 1;
            while (dIt+1 < pLen && p[dIt+1] == '*') {
                pc.insert(dIt+2);
                dIt += 2;
            }
        } else {
            return false;
        }
        ++sIt;

        cout << "# ";
        for (int v: pc) cout << v << " ";
        cout << endl;

        while (sIt < sLen) {
            set<int> newPc;
            bool isMat = false;
            for (auto it = pc.begin(); it != pc.end(); ++it) {
                pIt = *it;
                if (pIt+1 < pLen && p[pIt+1] == '*') {
                    if (p[pIt] == '.' || p[pIt] == s[sIt]) {
                        newPc.insert(pIt);
                    }
                    newPc.insert(pIt+2);
                    int dIt = pIt + 2;
                    while (dIt+1 < pLen && p[dIt+1] == '*') {
                        newPc.insert(dIt+2);
                        dIt += 2;
                    }
                    isMat = true;
                } else if (p[pIt] == '.' || p[pIt] == s[sIt]) {
                    newPc.insert(pIt+1);
                    int dIt = pIt + 1;
                    while (dIt+1 < pLen && p[dIt+1] == '*') {
                        newPc.insert(dIt+2);
                        dIt += 2;
                    }
                    isMat = true;
                }
            }
            if (!isMat) return false;
            pc = newPc;
            cout << "# ";
            for (int v: pc) cout << v << " ";
            cout << endl;
            ++sIt;
        }
        cout << "## ";
        for (int v: pc) cout << v << " ";
        cout << endl;

        for (int v : pc) if (v == pLen) return true;
        return false;
    }
};

#include <string.h>
bool isMatch(char* s, char* p) {
//    cout << "# " << s << " " << p << endl;
    if (*s == '\0' && *p == '\0') return true;
    int sLen = strlen(s);
    int pLen = strlen(p);
    int sIt = 0;
    int pIt = 0;

    char pCh = p[pIt];
    if (pIt < pLen-1 && p[pIt+1] == '*') {
        if (p[pIt] == '.' || p[pIt] == s[sIt]) {
            if (sIt == sLen-1 && pIt+2 == pLen)
                return true;

            if (sIt < sLen && isMatch(s+sIt+1, p+pIt))
                return true;
            if (isMatch(s+sIt, p+pIt+2))
                return true;

//                cout << "### --->" << "[" << s << ", " << p << "] -> " << (lb || rb) << endl;
//                cout << " ×ó = [" << s+sIt+1 << ", " << p+pIt << "] -> " << lb << endl;
//                cout << " ÓÒ = [" << s+sIt << ", " << p+pIt+2 << "] -> " << rb << endl;
            //return isMatch(s+sIt+1, p+pIt) || isMatch(s+sIt, p+pIt+2);
//                return false;
//                return lb || rb;
        } else {
            return isMatch(s+sIt, p+pIt+2);
        }
    } else if (sIt < sLen && (pCh == '.' || pCh == s[sIt])) {
        return isMatch(s+sIt+1, p+pIt+1);
    }
    return false;
}

int main()
{
    ifstream fin("in.txt");
    string s, p;
    char s2[200], p2[200];
    while (fin >> s2 >> p2) {
        s = s2, p = p2;

        cout << "s=" << s << "\tp=" << p << endl;
        cout << "$1 " << Solution().isMatch(s, p) << endl;
        cout << "$2 " << isMatch(s2, p2) << endl;
        cout << endl;
    }
    return 0;
}


//    bool isMatch(string s, string p) {
//        int sLen = s.size();
//        int pLen = p.size();
//        int sIt = 0;
//        int pIt = 0;
//        while (pIt < pLen || sIt < sLen) {
//            char pCh = p[pIt];
//            char tCh = '0';
//            if (pIt < pLen-1 && p[pIt+1] == '*') {
//                while (sIt < sLen && (pCh == '.' || pCh == s[sIt]))
//                    ++sIt;
//                pIt += 2;
//                while (pCh == p[pIt]) ++pIt;
//            } else if (pCh == '.' || pCh == s[sIt]) {
//                ++pIt;
//                ++sIt;
//            } else {
//                return false;
//            }
//            if (pIt == pLen && sIt == sLen)
//                return true;
//        }
//        return false;
//    }
