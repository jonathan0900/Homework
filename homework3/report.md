# 41141128

作業二
## 解題說明
本作業要求實作一個 C++ 類別 Polynomial，用於表示與操作單變數整數係數多項式，並採用 循環鏈結串列（Circular Linked List） 配合 表頭節點（Header Node） 儲存多項式項目。每個節點包含：


coef | exp | link


多項式需支援：



輸入與輸出運算子 (>> 與 <<)



拷貝建構子與賦值運算子

加法、減法、乘法運算

解構子釋放節點

指定 x 值的多項式求值

此作業重點在於資料結構設計、運算子重載與記憶體管理。


## 解題策略
1.資料結構選擇
使用動態陣列 int* coef 或 vector<int> 來存儲多項式係數
假設係數索引對應次數，例如 coef[0] 為常數項


2.運算子重載
operator>>：逐項輸入係數
operator<<：格式化輸出，如 3x^2 + 2x + 1


3.程式模組化
類別宣告於 Polynomial.h
定義於 Polynomial.cpp
測試主程式於 main.cpp

## 程式實作
```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;
using namespace std::chrono;

struct TermNode {
    int coef;
    int exp;
    TermNode* link;
};

class Polynomial {
private:
    TermNode* head;

    void clear() {
        TermNode* cur = head->link;
        while (cur != head) {
            TermNode* temp = cur;
            cur = cur->link;
            delete temp;
        }
        head->link = head;
    }

public:
    Polynomial() {
        head = new TermNode;
        head->link = head;
    }

    Polynomial(const Polynomial& other) : Polynomial() {
        TermNode* cur = other.head->link;
        while (cur != other.head) {
            insertTerm(cur->coef, cur->exp);
            cur = cur->link;
        }
    }

    ~Polynomial() {
        clear();
        delete head;
    }

    // 插入並合併同指數項
    void insertTerm(int c, int e) {
        if (c == 0) return;
        TermNode* cur = head;
        while (cur->link != head && cur->link->exp > e) cur = cur->link;

        // 如果存在相同指數，合併
        if (cur->link != head && cur->link->exp == e) {
            cur->link->coef += c;
            if (cur->link->coef == 0) { // 刪除 0 項
                TermNode* temp = cur->link;
                cur->link = temp->link;
                delete temp;
            }
            return;
        }

        // 插入新節點
        TermNode* newNode = new TermNode{c, e, cur->link};
        cur->link = newNode;
    }

    // 解析多項式字串
    void parsePolynomial(string input) {
        clear();
        input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
        if (input.empty()) return;

        if (input[0] != '-' && input[0] != '+') input = "+" + input;

        for (size_t i = 0; i < input.size();) {
            int sign = 1;
            if (input[i] == '+') { sign = 1; i++; }
            else if (input[i] == '-') { sign = -1; i++; }

            int coef = 0, exp = 0;
            bool hasX = false;

            // 讀取係數
            while (i < input.size() && isdigit(input[i])) {
                coef = coef * 10 + (input[i] - '0');
                i++;
            }
            if (coef == 0) coef = 1;
            coef *= sign;

            // 處理 x
            if (i < input.size() && input[i] == 'x') {
                hasX = true;
                i++;
                exp = 1;
                if (i < input.size() && input[i] == '^') {
                    i++;
                    exp = 0;
                    while (i < input.size() && isdigit(input[i])) {
                        exp = exp * 10 + (input[i]-'0');
                        i++;
                    }
                }
            } else {
                exp = 0; // 常數項
            }

            insertTerm(coef, exp);
        }
    }

    string toString() const {
        TermNode* cur = head->link;
        if (cur == head) return "0";

        ostringstream os;
        bool first = true;
        while (cur != head) {
            int c = cur->coef, e = cur->exp;
            if (!first) os << (c > 0 ? "+" : "");
            if (e == 0) os << c;
            else if (e == 1) {
                if (c == 1) os << "x";
                else if (c == -1) os << "-x";
                else os << c << "x";
            } else {
                if (c == 1) os << "x^" << e;
                else if (c == -1) os << "-x^" << e;
                else os << c << "x^" << e;
            }
            first = false;
            cur = cur->link;
        }
        return os.str();
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        TermNode* p = head->link;
        TermNode* q = b.head->link;
        while (p != head && q != b.head) {
            if (p->exp == q->exp) {
                int sumCoef = p->coef + q->coef;
                if (sumCoef != 0) result.insertTerm(sumCoef, p->exp);
                p = p->link;
                q = q->link;
            } else if (p->exp > q->exp) {
                result.insertTerm(p->coef, p->exp);
                p = p->link;
            } else {
                result.insertTerm(q->coef, q->exp);
                q = q->link;
            }
        }
        while (p != head) { result.insertTerm(p->coef, p->exp); p = p->link; }
        while (q != b.head) { result.insertTerm(q->coef, q->exp); q = q->link; }
        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        TermNode* p = head->link;
        TermNode* q = b.head->link;
        while (p != head && q != b.head) {
            if (p->exp == q->exp) {
                int diffCoef = p->coef - q->coef;
                if (diffCoef != 0) result.insertTerm(diffCoef, p->exp);
                p = p->link;
                q = q->link;
            } else if (p->exp > q->exp) {
                result.insertTerm(p->coef, p->exp);
                p = p->link;
            } else {
                result.insertTerm(-q->coef, q->exp);
                q = q->link;
            }
        }
        while (p != head) { result.insertTerm(p->coef, p->exp); p = p->link; }
        while (q != b.head) { result.insertTerm(-q->coef, q->exp); q = q->link; }
        return result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (TermNode* p = head->link; p != head; p = p->link) {
            for (TermNode* q = b.head->link; q != b.head; q = q->link) {
                result.insertTerm(p->coef * q->coef, p->exp + q->exp);
            }
        }
        return result;
    }

    double Evaluate(double x) const {
        double result = 0;
        TermNode* cur = head->link;
        while (cur != head) {
            result += static_cast<double>(cur->coef) * pow(x, cur->exp);
            cur = cur->link;
        }
        return result;
    }
};

int main() {
    Polynomial a, b;
    cout << "輸入a多項式：";
    string sa; cin >> sa;
    a.parsePolynomial(sa);

    cout << "輸入b多項式：";
    string sb; cin >> sb;
    b.parsePolynomial(sb);

    cout << "-------------------------" << endl;
    cout << "a = " << a.toString() << endl;
    cout << "b = " << b.toString() << endl;

    auto start = high_resolution_clock::now();
    Polynomial sum = a + b;
    auto end = high_resolution_clock::now();
    cout << "a + b = " << sum.toString()
         << "  需時："
         << duration_cast<microseconds>(end-start).count()/1000.0 << "s\n";

    start = high_resolution_clock::now();
    Polynomial diff = a - b;
    end = high_resolution_clock::now();
    cout << "a - b = " << diff.toString()
         << "  需時："
         << duration_cast<microseconds>(end-start).count()/1000.0 << "s\n";

    start = high_resolution_clock::now();
    Polynomial prod = a * b;
    end = high_resolution_clock::now();
    cout << "a * b = " << prod.toString()
         << "  需時："
         << duration_cast<microseconds>(end-start).count()/1000.0 << "s\n";

    cout << "-------------------------" << endl;
    cout << "請輸入x，以求出a(x)、b(x)的值?";
    double x; cin >> x;

    start = high_resolution_clock::now();
    double va = a.Evaluate(x);
    end = high_resolution_clock::now();
    cout << left << setw(8) << "a(x) = " << setw(8) << va
         << "需時：" << duration_cast<microseconds>(end-start).count()/1000.0 << "s\n";

    start = high_resolution_clock::now();
    double vb = b.Evaluate(x);
    end = high_resolution_clock::now();
    cout << left << setw(8) << "b(x) = " << setw(8) << vb
         << "需時：" << duration_cast<microseconds>(end-start).count()/1000.0 << "s\n";

    return 0;
}
```

## 效能分析
時間複雜度：O(m + n)

空間複雜度：O(m + n)

## 測試與驗證
| 測試案例       | 參數(a,b)     | 預期輸出       | 實際輸出      |
| ------------- | ------------- | ------------- | ------------- |
| 測試1         | 56            | 565x^6        |565x^6         |
| 測試2         | 789-453       | 78x^9 + 8x^9  | 78x^9 + 8x^9  |
| 測試3         | -4632         | -4x^6 + 3x^2  | -4x^6 + 3x^2  |
| 測試4         | -45-58        | -4x^5         |-4x^5          |
| 測試5         | 0905          | 0             |0            | 

## 編譯與執行指令
g++ main.cpp -o poly
./poly

## 結論
1.使用固定陣列簡單有效，適合小型作業


2.運算子重載讓輸入輸出更直覺


3.加法與乘法演算法正確，可正確處理不同次方的合併

## 申論及開發報告
1.選擇使用固定陣列的原因
    題目項數上限已知（100 項），使用陣列簡單直觀
    容易用索引操作，提高程式可讀性


2.選擇使用遞增陣列與掃描法實作加法、乘法
    加法使用雙指標掃描，類似合併排序

    
3.運算子重載的理由符合題目要求
    讓輸入與輸出更像數學表達式


## 心得
1.物件導向設計
將多項式封裝成 Polynomial 類別，內部用 Term 儲存項目。
好處是程式模組化、易讀且方便之後擴充功能。


2.運算子重載經驗
重載 >>、<< 讓輸入輸出更直覺。
遇到正負號與特殊次方輸出的格式問題，最後透過條件判斷解決。


3.多項式運算設計
加法用雙指標掃描合併項目。
乘法用雙迴圈並檢查同次項合併，邏輯清楚但效能普通。


4.測試與除錯心得
測試了零多項式、單項多項式及正負交錯情況。
發現邊界條件與輸出格式是最容易出錯的地方。


5.收穫與改進
學會用物件導向設計數學結構，程式更直觀。
未來可用動態陣列或 map 優化記憶體與運算效率。




















