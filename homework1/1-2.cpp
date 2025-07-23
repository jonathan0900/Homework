#include <iostream>
#include <stack>
using namespace std;

int P1(int a, int b) {
    stack<int> stk;

    stk.push(a);
    while (!stk.empty()) {
        a = stk.top();
        stk.pop();

        if (a == 0) {
            b = b + 1;
        } else if (b == 0) {
            stk.push(a - 1);
            b = 1;
        } else {
            stk.push(a - 1);
            stk.push(a);
            b = b - 1;
        }
    }

    return b;
}

int main() {
    int a , b ;
    cin>>a>>b;
    int result = P1(a, b);
    cout << "P2(" << a << "," << b << ") = " << result << endl;

    return 0;
}
