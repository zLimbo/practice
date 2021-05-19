#include <iostream>
#include <thread>
#include <vector>
#include <time.h>

using namespace std;
constexpr int MOD = 1e9 - 7;
int N = 12;
int M = 4;

int fib_n = 45;


int fib(int n) {
	return n <= 1 ? n : fib(n - 2) + fib(n - 1);
}

void foo(int k) {
	std::thread::id this_id = std::this_thread::get_id();
	int loop_num = N / M;
	cout << this_id << ": " << loop_num << endl;
	while (loop_num--) {
		fib(fib_n);
	}
}

double test_thread(int m) {
	M = m;
	vector<thread> ths;
	clock_t start = clock();
	for (int i = 0; i < m; ++i) {
		ths.emplace_back(foo, i);
	}
	for (thread& th: ths) {
		th.join();
	}
	clock_t end = clock();
	double time = double(end - start) / CLOCKS_PER_SEC;
	return time;
}


int main() {

	for (int i = 0; i <= fib_n; ++i) {
		clock_t start = clock();
		int y = fib(i);
		clock_t end = clock();
		cout << i << ": " << y << " time: " << double(end - start) / CLOCKS_PER_SEC << "s " << endl;
	}

	vector<int> vm = {1, 2, 4, 6, 12};
	vector<double> vt;
	
	for (int m: vm) {
		cout << "thread num: " << m << endl;
		double time = test_thread(m);
		vt.push_back(time);	
	}	

	cout << endl;
	for (int i = 0; i < vm.size(); ++i) {
		cout << "thread: num: " << vm[i] << ", speed time: " << vt[i] << "s" << endl;
	}
	return 0;
}
