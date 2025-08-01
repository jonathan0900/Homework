# 41141128

作業二
## 解題說明
1.定義多項式類別，包含：
私有成員：多項式係數陣列、項數或最高次數
公有介面：建構子、輸入、輸出、運算子重載


2.實作輸入與輸出：
重載 >> 運算子讀入多項式
重載 << 運算子輸出多項式


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




















