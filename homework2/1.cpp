#include <iostream>
#include <vector>
using namespace std;

struct Term {
    long long coef;
    long long exp;
};

int main() {
    vector<Term> terms;
    long long coef, exp;

    // 讀取所有輸入直到換行或 EOF
    while (cin >> coef >> exp) {
        terms.push_back({coef, exp});
        if (cin.peek() == '\n') break;  // 遇到換行就停止
    }

    // 輸出多項式
    bool first = true;
    for (auto &t : terms) {
        if (t.coef == 0) continue; // 忽略係數為 0 的項

        // 處理符號
        if (!first && t.coef > 0) cout << " + ";
        if (!first && t.coef < 0) cout << " - ";
        if (first && t.coef < 0) cout << "-";

        // 輸出絕對值
        long long absCoef = t.coef < 0 ? -t.coef : t.coef;

        // 根據指數輸出
        if (t.exp == 0) {
            cout << absCoef;
        } else if (t.exp == 1) {
            cout << absCoef << "x";
        } else {
            cout << absCoef << "x^" << t.exp;
        }

        first = false;
    }

    cout << endl;
    return 0;
}
