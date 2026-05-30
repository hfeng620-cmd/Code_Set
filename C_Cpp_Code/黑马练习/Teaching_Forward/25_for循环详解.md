# for 循环详解

## 一、什么是 for 循环

`for` 循环是 C++ 中**最常用**的循环结构。它把循环的三个要素（初始化、条件、更新）集中在一行，结构紧凑，代码简洁。

> 生活类比：考试时监考老师说"从第1题开始，做到第10题，每做完一题就翻一页"——这就是 for 循环的思维：起点、终点、步长，一目了然。

---

## 二、for 循环的基本语法

```cpp
for (初始化表达式; 循环条件; 更新表达式)
{
    循环体;
}
```

### 语法要点

| 组成部分 | 说明 |
|---------|------|
| `for` | 关键字 |
| `初始化表达式` | 循环开始前执行一次，通常用来初始化循环变量 |
| `循环条件` | 每次循环前判断，为 `true` 才执行循环体 |
| `更新表达式` | 每次循环体执行完后执行，通常用来更新循环变量 |
| **两个分号** | **必须有，分隔三个表达式，不能省略** |

---

## 三、for 循环的执行流程

```
开始
  |
  v
执行初始化表达式（只执行一次）
  |
  v
判断循环条件 --为 false--> 退出循环 --> 结束
  |
  |为 true
  v
执行循环体
  |
  v
执行更新表达式
  |
  v
回到判断循环条件
```

### 基础示例

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 用 for 循环打印 1 到 5
    //  i=1        → 初始化：从1开始
    //  i<=5       → 条件：只要 i 不超过5就继续
    //  i++        → 更新：每次循环后 i 加1
    for (int i = 1; i <= 5; i++)
    {
        cout << "第 " << i << " 次执行" << endl;
    }

    cout << "循环结束" << endl;
    return 0;
}
```

输出结果：
```
第 1 次执行
第 2 次执行
第 3 次执行
第 4 次执行
第 5 次执行
循环结束
```

### 执行过程详解

```
第1轮: i=1 → 判断 1<=5 ✓ → 执行循环体 → i++ → i=2
第2轮: i=2 → 判断 2<=5 ✓ → 执行循环体 → i++ → i=3
第3轮: i=3 → 判断 3<=5 ✓ → 执行循环体 → i++ → i=4
第4轮: i=4 → 判断 4<=5 ✓ → 执行循环体 → i++ → i=5
第5轮: i=5 → 判断 5<=5 ✓ → 执行循环体 → i++ → i=6
第6轮: i=6 → 判断 6<=5 ✗ → 退出循环
```

---

## 四、for 循环三表达式可省略

for 循环的三个表达式都可以省略（但两个分号不能省）。

### 4.1 省略初始化表达式

```cpp
// 初始化放在 for 前面
int i = 1;
for (; i <= 5; i++)
{
    cout << i << " ";
}
cout << endl;
// 输出: 1 2 3 4 5
```

### 4.2 省略更新表达式

```cpp
// 更新放在循环体里面
for (int i = 1; i <= 5;)
{
    cout << i << " ";
    i++;    // 手动更新
}
cout << endl;
// 输出: 1 2 3 4 5
```

### 4.3 省略循环条件（死循环）

```cpp
// 省略条件 = 条件永远为 true = 死循环
for (int i = 0; ; i++)
{
    cout << i << " ";
    if (i >= 5) break;  // 必须手动 break
}
cout << endl;
// 输出: 0 1 2 3 4 5
```

### 4.4 三个表达式全省略（死循环）

```cpp
// 最简形式的死循环
for (;;)
{
    // 无限循环，需要 break 退出
    break;
}
```

> **建议**：虽然可以省略，但为了代码可读性，**尽量不要省略**，保持三个表达式都写在 for 后面的小括号中。

---

## 五、for 循环 vs while 循环对比

### 5.1 功能上完全等价

```cpp
// for 循环版本
for (int i = 1; i <= 5; i++)
{
    cout << i << " ";
}

// 等价的 while 循环版本
int i = 1;          // 初始化
while (i <= 5)      // 条件
{
    cout << i << " ";
    i++;            // 更新
}
```

### 5.2 选择建议

| 场景 | 推荐使用 | 原因 |
|------|---------|------|
| 已知循环次数 | `for` | 三要素一目了然 |
| 未知循环次数 | `while` | 只需写条件 |
| 遍历数组 | `for` | 索引递增很自然 |
| 用户输入验证 | `while` / `do-while` | 不确定输入几次 |
| 死循环 + break | `while(true)` | 更直观 |

---

## 六、for 循环的各种变体

### 6.1 递增循环（步长为1）

```cpp
// 最常见的形式
for (int i = 0; i < 10; i++)
{
    cout << i << " ";
}
cout << endl;
// 输出: 0 1 2 3 4 5 6 7 8 9
```

### 6.2 递减循环

```cpp
// 倒序打印
for (int i = 10; i >= 1; i--)
{
    cout << i << " ";
}
cout << endl;
// 输出: 10 9 8 7 6 5 4 3 2 1
```

### 6.3 步长不为1

```cpp
// 步长为2：打印所有奇数
for (int i = 1; i <= 20; i += 2)
{
    cout << i << " ";
}
cout << endl;
// 输出: 1 3 5 7 9 11 13 15 17 19

// 步长为3
for (int i = 0; i <= 30; i += 3)
{
    cout << i << " ";
}
cout << endl;
// 输出: 0 3 6 9 12 15 18 21 24 27 30
```

### 6.4 步长为负数（递减变体）

```cpp
// 每次减 3
for (int i = 15; i >= 0; i -= 3)
{
    cout << i << " ";
}
cout << endl;
// 输出: 15 12 9 6 3 0
```

### 6.5 无限循环

```cpp
// for(;;) 是 for 的无限循环写法
for (;;)
{
    // 需要 break 来退出
    break;
}
// 等价于 while(true) { ... }
```

---

## 七、break 和 continue 在 for 循环中

### 7.1 break：跳出循环

```cpp
#include <iostream>
using namespace std;

int main()
{
    for (int i = 1; i <= 10; i++)
    {
        if (i == 6)
        {
            cout << "遇到6，跳出循环" << endl;
            break;      // 立即跳出 for 循环
        }
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 3 4 5 遇到6，跳出循环

    return 0;
}
```

### 7.2 continue：跳过本次

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 打印 1-10 中不是 3 的倍数的数
    for (int i = 1; i <= 10; i++)
    {
        if (i % 3 == 0)
        {
            continue;   // 跳过本次，进入 i++ 然后判断
        }
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 4 5 7 8 10

    return 0;
}
```

### 7.3 continue 在 for vs while 中的区别

```cpp
// 在 for 循环中，continue 跳到"更新表达式"
for (int i = 1; i <= 10; i++)     // ← continue 跳到这里（i++）
{
    if (i == 5) continue;
    cout << i << " ";
}
// 输出: 1 2 3 4 6 7 8 9 10 （安全，i++ 总会执行）

// 在 while 循环中，continue 跳到"条件判断"
int i = 1;
while (i <= 10)
{
    if (i == 5) continue;   // 危险！跳过了 i++，死循环！
    cout << i << " ";
    i++;
}
```

**关键区别**：for 循环中的 `continue` 会自动执行更新表达式，所以不会导致死循环；while 循环中 `continue` 跳过更新语句，可能导致死循环。

---

## 八、常见错误

### 错误1：`<` vs `<=` 差一次

```cpp
// 场景：打印 1 到 10

// 用 <：需要写 i < 11 或 i <= 10
for (int i = 1; i < 11; i++)   // 打印 1-10
{
    cout << i << " ";
}
cout << endl;

// 用 <=：需要写 i <= 10
for (int i = 1; i <= 10; i++)  // 打印 1-10
{
    cout << i << " ";
}
cout << endl;

// 常见错误：想打印 1-10，却写成了 i < 10
for (int i = 1; i < 10; i++)   // 只打印 1-9，少了10！
{
    cout << i << " ";
}
cout << endl;
// 输出: 1 2 3 4 5 6 7 8 9 （少了10！）
```

### 错误2：for 后面加了分号

```cpp
// 错误：for 后面多了分号
for (int i = 0; i < 5; i++);   // 空循环体！
{
    cout << i << endl;           // 这行只执行一次，且 i 的值不确定
}

// 正确
for (int i = 0; i < 5; i++)
{
    cout << i << endl;
}
```

### 错误3：循环变量作用域问题

```cpp
// 在 for 中声明的变量，只能在 for 内部使用
for (int i = 0; i < 5; i++)
{
    cout << i << " ";
}
// cout << i;  // 错误！i 在 for 外面不可用
```

### 错误4：更新方向与条件矛盾

```cpp
// 错误：i++ 让 i 越来越大，i >= 0 永远为 true
// for (int i = 10; i >= 0; i++)   // 死循环！

// 正确：应该是 i--
for (int i = 10; i >= 0; i--)
{
    cout << i << " ";
}
cout << endl;
// 输出: 10 9 8 7 6 5 4 3 2 1 0
```

---

## 九、实战案例

### 案例1：敲桌子（7的倍数或含7的数）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 从1到100：
    // 如果是7的倍数，或者包含数字7，输出"敲桌子"
    // 否则输出该数字

    cout << "敲桌子游戏（1-100）：" << endl;

    for (int i = 1; i <= 100; i++)
    {
        // 判断是否是7的倍数
        bool isMultipleOf7 = (i % 7 == 0);
        // 判断个位是否是7
        bool has7InOnes = (i % 10 == 7);
        // 判断十位是否是7
        bool has7InTens = (i / 10 == 7);

        if (isMultipleOf7 || has7InOnes || has7InTens)
        {
            cout << "敲桌子 ";
        }
        else
        {
            cout << i << " ";
        }

        // 每10个换一行
        if (i % 10 == 0)
        {
            cout << endl;
        }
    }

    return 0;
}
```

### 案例2：求 1 到 100 的和

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 求 1 + 2 + 3 + ... + 100
    int sum = 0;

    for (int i = 1; i <= 100; i++)
    {
        sum += i;   // 等价于 sum = sum + i
    }

    cout << "1+2+3+...+100 = " << sum << endl;
    // 输出: 1+2+3+...+100 = 5050

    return 0;
}
```

### 案例3：求 1 到 100 中偶数的和

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 方法1：用 if 判断
    int sum1 = 0;
    for (int i = 1; i <= 100; i++)
    {
        if (i % 2 == 0)     // 如果是偶数
        {
            sum1 += i;
        }
    }
    cout << "方法1 - 偶数和: " << sum1 << endl;

    // 方法2：直接从2开始，步长为2（更高效）
    int sum2 = 0;
    for (int i = 2; i <= 100; i += 2)
    {
        sum2 += i;
    }
    cout << "方法2 - 偶数和: " << sum2 << endl;

    // 方法3：用 continue 跳过奇数
    int sum3 = 0;
    for (int i = 1; i <= 100; i++)
    {
        if (i % 2 != 0)
        {
            continue;   // 跳过奇数
        }
        sum3 += i;
    }
    cout << "方法3 - 偶数和: " << sum3 << endl;

    // 三种方法结果相同: 2550

    return 0;
}
```

### 案例4：求阶乘

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 求 n! = 1 * 2 * 3 * ... * n
    int n;
    cout << "请输入 n: ";
    cin >> n;

    long long factorial = 1;  // 用 long long 防止溢出

    for (int i = 1; i <= n; i++)
    {
        factorial *= i;
    }

    cout << n << "! = " << factorial << endl;
    // 5! = 120
    // 10! = 3628800

    return 0;
}
```

### 案例5：判断素数

```cpp
#include <iostream>
using namespace std;

int main()
{
    int num;
    cout << "请输入一个大于1的整数: ";
    cin >> num;

    bool isPrime = true;  // 假设是素数

    // 从2到 sqrt(num) 检查是否有因子
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
        {
            isPrime = false;  // 找到因子，不是素数
            break;            // 提前退出
        }
    }

    if (isPrime && num > 1)
    {
        cout << num << " 是素数" << endl;
    }
    else
    {
        cout << num << " 不是素数" << endl;
    }

    return 0;
}
```

### 案例6：打印九九乘法表

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 打印九九乘法表
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= i; j++)
        {
            cout << j << "x" << i << "=" << i * j << "\t";
        }
        cout << endl;
    }

    return 0;
}
```

---

## 十、for 循环的更多变体

### 10.1 多个循环变量

```cpp
#include <iostream>
using namespace std;

int main()
{
    // for 中可以有多个初始化/更新表达式（用逗号分隔）
    for (int i = 0, j = 10; i < j; i++, j--)
    {
        cout << "i=" << i << ", j=" << j << endl;
    }
    // 输出:
    // i=0, j=10
    // i=1, j=9
    // i=2, j=8
    // i=3, j=7
    // i=4, j=6

    return 0;
}
```

### 10.2 循环变量可以是字符型

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 循环变量可以是字符型
    for (char ch = 'A'; ch <= 'Z'; ch++)
    {
        cout << ch << " ";
    }
    cout << endl;
    // 输出: A B C D ... Z

    return 0;
}
```

---

## 十一、总结

### for 循环要点

| 要点 | 说明 |
|------|------|
| 语法 | `for (初始化; 条件; 更新) { 循环体 }` |
| 执行顺序 | 初始化 → 条件判断 → 循环体 → 更新 → 条件判断 ... |
| 三表达式 | 都可省略，但分号不能省 |
| break | 跳出整个循环 |
| continue | 跳过本次，进入更新表达式 |

### for vs while 对比

| 对比项 | for | while |
|-------|-----|-------|
| 结构 | 三要素集中一行 | 三要素分散 |
| 适用 | 已知循环次数 | 未知循环次数 |
| continue | 安全（自动更新） | 可能死循环（跳过更新） |
| 可读性 | 计数循环更清晰 | 条件循环更清晰 |

### for 循环使用口诀

```
for 循环三部分，分号隔开不能混；
初始化在最前面，只执行那一遍；
条件判断在中间，为假退出为真转；
更新放在最后面，每次循环都来一遍；
break 跳出整个圈，continue 跳过进更新。
```
