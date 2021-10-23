#include <iostream>
#include <string>
using namespace std;


static const auto fastIO = []() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	return nullptr;
}();

int* buildNext(const string &sx) {
	int len = sx.size();
	int *next = new int[len];
	next[0] = -1;
	int i = -1, j = 0;
	
	while (j < len - 1) {
		if (i < 0 || sx[i] == sx[j]) {
			++i; ++j;
			next[j] = (sx[i] == sx[j] ? next[i] : i);
		} else {
			i = next[i];
		}
	}
	return next;
}

bool kmpMatch(const string &sx, const string &sy) {
	int xLen = sx.size(), yLen = sy.size();
	int xi = 0, yi = 0;
	int *next = buildNext(sx);
	while (xi < xLen && yi < yLen) {
		if (xi < 0 || sx[xi] == sy[yi]) {
			++xi; ++yi;
		} else {
			xi = next[xi];
		}
	}
	delete[] next;
	if (xi == xLen) return true;
	return false;
}

int main() {
	
}



