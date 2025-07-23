#include <iostream>
using namespace std;

int ackermann(int a, int b) {
    if (a == 0) return b + 1;
    else if (b == 0) return ackermann(a - 1, 1);
    else return ackermann(a - 1, ackermann(a, b - 1));
}

int main() {
    int a = 0, b = 1;
    cout << "A(" << a << "," << b << ") = " << ackermann(a, b) << endl;
    return 0;
}
