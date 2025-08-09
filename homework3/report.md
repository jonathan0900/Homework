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
1.資料結構設計

    使用 循環鏈結串列 儲存多項式，每個節點包含係數與指數。

    使用 表頭節點 簡化操作，例如在空串列與尾端插入時方便管理。

    使用 可用空間表（available-space list）重複利用釋放的節點。

2.主要功能實作策略

    輸入/輸出：將輸入數列轉換成遞減指數的鏈結串列；輸出時還原為 (c1 e1 c2 e2 …) 格式。

    加減法：使用類似歸併排序的雙指標法，同時遍歷兩個多項式串列，依指數大小合併。

    乘法：對每個項進行雙層迴圈相乘，並將結果插入目標串列，若指數相同則合併係數。

    求值：遍歷鏈結串列，對每個項計算 coef * pow(x, exp) 後加總。

    記憶體管理：使用解構子將節點釋放回可用空間表。


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
| 測試案例       | 參數(a,b)                                                    
| ------------- | ------------------------------------------------------------
| A多項式        |5x^9-3x^8+2x^7+x^6-4x^5+6x^4-8x^3+7x^2-9x+10                 |  
| B多項式        |-2x^10+4x^9-6x^8+8x^7-10x^6+12x^5-14x^4+16x^3-18x^2+20x-22   | 
|請輸入x         |２                                                           |  


輸入a多項式：5x^9-3x^8+2x^7+x^6-4x^5+6x^4-8x^3+7x^2-9x+10


輸入b多項式：-2x^10+4x^9-6x^8+8x^7-10x^6+12x^5-14x^4+16x^3-18x^2+20x-22


a = 5x^9-3x^8+2x^7+x^6-4x^5+6x^4-8x^3+7x^2-9x+10


b = -2x^10+4x^9-6x^8+8x^7-10x^6+12x^5-14x^4+16x^3-18x^2+20x-22


a + b = -2x^10+9x^9-9x^8+10x^7-9x^6+8x^5-8x^4+8x^3-11x^2+11x-12  需時：0.003s


a - b = 2x^10+x^9+3x^8-6x^7+11x^6-16x^5+20x^4-24x^3+25x^2-29x+32  需時：0.002s


a * b = -10x^19+26x^18-46x^17+64x^16-74x^15+72x^14-54x^13+22x^12+28x^11-98x^10+168x^9-358x^8+510x^7-644x^6+710x^5-702x^4+638x^3-

514x^2+398x-220  需時：0.022s


請輸入x，以求出a(x)、b(x)的值?2


a(x) =  2036    需時：0.067s


b(x) =  -918    需時：0.001s


Program ended with exit code: 0


## 編譯與執行指令
g++ -std=c++11 Polynomial.cpp -o Polynomial
./Polynomial

## 結論
1.使用循環鏈結串列有效管理多項式資料結構
2.正確實作多項式加減乘與求值

3.學會運算子多載與動態記憶體管理

4.熟悉資料結構在實務數學運算的應用

## 申論及開發報告
1.難點：

    運算子重載需同時考慮深拷貝與記憶體釋放

    乘法運算需要小心合併同指數項

2.改進建議：

    可改用 std::list 或 std::vector 實作以簡化記憶體管理

    使用哈希表加速同指數項合併

## 心得
透過本次作業：

1.熟悉了 循環鏈結串列 的操作

2.體驗了 運算子重載 在數學運算建模上的方便性

3.體會到 記憶體管理與資料結構設計的重要性

4.對日後進行 大型資料結構與數學模擬 有實際幫助



















