# 41143263

作業一

## 解題說明

Ackermann 函數是一個著名的遞迴例子，它不屬於原始遞迴範疇，其特點是增長速度非常快，因此適合用來測試遞迴與堆疊的概念。

$$
A(m, n) = \begin{cases}
n + 1 & \text{if } m = 0 \\
A(m - 1, 1) & \text{if } n = 0 \\
A(m - 1, A(m, n - 1)) & \text{otherwise}
\end{cases}
$$

### 解題策略

1.遞迴思維建立遇到 m=0就回傳 n+1，是最底層，遇到 n=0，則往左邊減一層 m，從 n=1 重啟，其他情況會展開成「雙層遞迴」，這是最困難處。  

2.非遞迴策略用堆疊模擬系統呼叫堆疊，每次記錄 (m,n) 當前狀態，根據條件把下一步推入堆疊，特別是 A(m-1, A(m,n-1))，需要等到內部 A(m,n-1) 得出值後，才能處理外層 A(m-1,x)。

## 程式實作

以下為主要程式碼：

遞迴版本:
```cpp
#include <iostream>
using namespace std;

int ackermann(int a, int b) {
    if (a == 0) return b + 1;
    else if (b == 0) return ackermann(a - 1, 1);
    else return ackermann(a - 1, ackermann(a, b - 1));
}

int main() {
    int a, b;
    cin>>a>>b;
    cout << "A(" << a << "," << b << ") = " << ackermann(a, b) << endl;
    return 0;
}
```
非遞迴版本:
```cpp
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
```

## 效能分析

時間複雜度:  

遞迴：非常高，非多項式，屬於超指數級

非遞迴：模擬堆疊行為，但仍接近上述時間複雜度

空間複雜度:

遞迴：最壞可能為 O(n^(m+1))

非遞迴：控制在手動堆疊

## 測試與驗證
| 測試案例       | 參數(a,b)     | 預期輸出       | 實際輸出      |
| ------------- | ------------- | ------------- | ------------- |
| 測試1         | (0,1)         | 2             |2              |
| 測試2         | (1,2)         | 4             | 4             |
| 測試3         | (0,4)         | 5             | 5             |
| 測試4         | (2,3)         | 9             | 9             |
| 測試5         | (3,4)         | 125           | 125           |


### 編譯與執行指令
1.遞迴版本

g++ -o ackermann ackermann.cpp

./ackermann

2.非遞迴版本

g++ -o ackermann.exe ackermann.cpp
### 結論

這題一開始讓我蠻震驚的，因為Ackermann函數看起來很簡單，卻遞迴得非常誇張，動不動就爆炸性成長。寫遞迴版本時，雖然概念很直覺，但實際跑起來很快就會跑到電腦受不了，後來改寫成非遞迴版本，用stack自己控制流程，我才更清楚原本電腦怎麼幫我管理遞迴的呼叫，這題真的幫我加深了對「遞迴怎麼跑」的理解，也訓練我怎麼手動模擬call stack，滿有挑戰性的。

## 申論及開發報告
1.Ackermann函數本身就是用遞迴定義的，所以直接用遞迴來實作最直覺。

2.寫起來程式碼簡潔明瞭，邏輯清楚，符合數學原始定義。

3.這題剛好可以用來練習遞迴的進入與返回流程，很適合加深理解。

4.雖然遞迴版本在m和n太大時會造成Stack Overflow，但對小輸入的測試很方便也看得出結果。

