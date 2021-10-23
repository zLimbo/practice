#include <cstdio>

int main(int argc, char** argv) {
	
	
	unsigned long long key = 0x1234567890123456;
	unsigned long long key2 = key >> 32;
	printf("%d\n", sizeof(key));
	printf("%#x\n", key2);
	printf("%#x\n", key);
	
	return 0;
}
