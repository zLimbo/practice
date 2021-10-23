#include <iostream>

using namespace std;

struct Node {
    int a;
    string b;
};

int main() {

    cout << sizeof(((Node*)0)->a) << endl;

}