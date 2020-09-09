#include <iostream>
#include <stdint.h>
#include <zl/debug.h>

using namespace std;

int main() {

	cout << "Hello World !" << endl;

	int32_t i32 = ((int64_t)1 << 31) - 1;
	int64_t i64 = ((int64_t)1 << 62) - 1;

	print(GREEN, sizeof(int32_t), sizeof(int64_t));

	print(BLUE, "i32 =", i32);
	print(YELLO, "i64 =", i64);
	print(RED, "ABC");

	cout << "\033[1;31mabc" << endl;
	cout << 2 << endl;
	cout << "\033[0m" << endl;

	return 0;
}
