# while 循环详解

## 一、什么是循环

循环是编程中最重要的结构之一。它的作用是**让一段代码重复执行多次**。

> 生活类比：操场跑圈——"只要没跑够5圈，就继续跑"，这就是循环思维。

C++ 提供了三种循环结构：`while`、`do-while`、`for`。本文详细讲解 `while` 循环。

---

## 二、while 循环的基本语法

```cpp
while (循环条件)
{
    循环体;   // 条件为 true 时，重复执行这里的代码
}
```

### 语法要点

| 组成部分 | 说明 |
|---------|------|
| `while` | 关键字，表示"当……的时候" |
| `循环条件` | 一个布尔表达式，为 `true` 时执行循环体，为 `false` 时退出循环 |
| `循环体` | 条件为真时重复执行的代码块 |

---

## 三、while 循环的执行流程

```
开始
  |
  v
判断循环条件 --为 false--> 退出循环 --> 结束
  |
  |为 true
  v
执行循环体
  |
  v
回到判断条件（继续下一轮判断）
```

**核心逻辑**：先判断条件，再决定是否执行。如果一开始条件就是 `false`，循环体一次都不会执行。

### 基础示例

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 用 while 循环打印 1 到 5
    int i = 1;           // ① 初始化循环变量

    while (i <= 5)       // ② 循环条件
    {
        cout << "第 " << i << " 次执行" << endl;
        i++;             // ③ 更新循环变量（非常重要！）
    }

    cout << "循环结束，i = " << i << endl;
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
循环结束，i = 6
```

---

## 四、循环三要素（非常重要！）

每个循环都必须有三个要素，缺一不可：

```cpp
int i = 1;            // 要素1：循环变量初始化
while (i <= 5)        // 要素2：循环条件
{
    cout << i << endl;
    i++;              // 要素3：循环变量更新
}
```

| 要素 | 作用 | 忘记的后果 |
|------|------|-----------|
| 循环变量初始化 | 给循环变量一个起始值 | 变量值不确定，行为不可预测 |
| 循环条件 | 决定什么时候继续/停止 | 无法控制循环何时结束 |
| 循环变量更新 | 每轮改变变量值，让条件最终为 false | **死循环！** |

---

## 五、死循环 while(true)

### 5.1 什么是死循环

死循环就是**永远不会结束**的循环，条件永远为 `true`。

```cpp
// 最简单的死循环
while (true)
{
    cout << "我会一直执行下去..." << endl;
}
```

### 5.2 为什么会形成死循环

```cpp
// 原因1：忘记更新循环变量
int i = 1;
while (i <= 5)
{
    cout << i << endl;
    // 忘了写 i++;  → i 永远是 1，条件永远为 true
}

// 原因2：更新方向与条件矛盾
int k = 1;
while (k > 0)
{
    k++;           // k 越来越大，永远 > 0，死循环
}
```

### 5.3 死循环的使用场景

有时候我们**故意**使用死循环，配合 `break` 在合适的时候退出：

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 场景：不断接收用户输入，直到输入 0 退出
    int num;

    while (true)    // 故意写死循环
    {
        cout << "请输入一个数字（输入0退出）: ";
        cin >> num;

        if (num == 0)
        {
            cout << "收到退出指令，再见！" << endl;
            break;  // 用 break 跳出循环
        }

        cout << "你输入的是: " << num << endl;
    }

    return 0;
}
```

> **什么时候用死循环？** 当你**不确定循环要执行多少次**，而是由某个条件在循环体内决定何时结束时，用 `while (true)` + `break` 是常见模式。

### 5.4 如何避免意外的死循环

1. **确保循环变量在循环体内被更新**
2. **确保更新方向能让条件最终变为 false**
3. **调试时如果程序卡住，按 Ctrl+C 强制终止**

---

## 六、break 在 while 循环中的作用

`break` 的作用是**立即跳出当前循环**，不再执行循环体内 `break` 之后的代码，也不再判断循环条件。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 1;

    while (i <= 10)
    {
        if (i == 5)
        {
            cout << "遇到 5，提前退出循环！" << endl;
            break;    // 当 i==5 时跳出整个 while 循环
        }
        cout << "i = " << i << endl;
        i++;
    }

    cout << "循环结束后 i = " << i << endl;
    return 0;
}
```

输出结果：
```
i = 1
i = 2
i = 3
i = 4
遇到 5，提前退出循环！
循环结束后 i = 5
```

---

## 七、continue 在 while 循环中的作用

`continue` 的作用是**跳过本次循环剩余代码，直接进入下一次循环的条件判断**。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 0;

    while (i < 10)
    {
        i++;              // 注意：更新放在前面

        if (i % 3 == 0)   // 如果 i 是 3 的倍数
        {
            continue;     // 跳过本次循环，不执行下面的 cout
        }

        cout << "i = " << i << endl;
    }

    return 0;
}
```

输出结果（跳过了 3、6、9）：
```
i = 1
i = 2
i = 4
i = 5
i = 7
i = 8
i = 10
```

**注意**：如果 `continue` 放在循环变量更新之前，而更新语句在 `continue` 之后，就会导致变量没更新，形成死循环！

```cpp
// 危险！continue 在 i++ 之前
int i = 1;
while (i <= 10)
{
    if (i == 5)
    {
        continue;   // 跳过了 i++，i 永远是 5，死循环！
    }
    cout << i << endl;
    i++;            // 这行被跳过了
}
```

---

## 八、常见错误

### 错误1：死循环（忘记更新变量）

```cpp
// 错误写法
int i = 1;
while (i <= 5)
{
    cout << i << endl;
    // 忘记写 i++; 导致死循环！
}

// 正确写法
int i = 1;
while (i <= 5)
{
    cout << i << endl;
    i++;    // 别忘了这行！
}
```

### 错误2：循环变量更新方向错误

```cpp
// 错误：条件是 i <= 10，但 i 在减少，永远满足条件
int i = 10;
while (i <= 10)
{
    cout << i << endl;
    i--;    // i 越来越小，永远 <= 10，死循环！
}

// 正确：应该改成递增，或者条件改成 i >= 1
```

### 错误3：while 后面加了分号

```cpp
// 错误写法：while 后面多了分号
int i = 1;
while (i <= 5);      // 这个分号形成了一个空循环体！
{                     // 这个大括号和 while 无关，只是一个普通代码块
    cout << i << endl; // 这行只会执行一次
    i++;
}
// 实际上 while(i<=5) 不断执行空语句，变成死循环

// 正确写法
int i = 1;
while (i <= 5)
{
    cout << i << endl;
    i++;
}
```

### 错误4：continue 导致的死循环

```cpp
int i = 1;

// 错误：continue在更新语句之前，且没有手动更新
// 当i=3时，continue跳过了i++，导致i永远是3
// while (i <= 10)
// {
//     if (i == 3)
//     {
//         continue;   // 跳过了i++，死循环！
//     }
//     cout << i << " ";
//     i++;
// }

// 正确：在continue之前更新循环变量
while (i <= 10)
{
    if (i == 3)
    {
        i++;            // 先更新
        continue;       // 再跳过
    }
    cout << i << " ";
    i++;
}
cout << endl;
// 输出：1 2 4 5 6 7 8 9 10
```

---

## 九、实战案例：猜数字游戏

```cpp
#include <iostream>
#include <cstdlib>   // 用于 rand() 和 srand()
#include <ctime>     // 用于 time()
using namespace std;

int main()
{
    // 猜数字游戏：程序随机生成 1-100 的数字，玩家来猜

    // 用当前时间作为随机数种子
    srand((unsigned int)time(NULL));

    // 生成 1-100 的随机数
    int target = rand() % 100 + 1;
    int guess = 0;        // 玩家猜的数字
    int count = 0;        // 猜的次数

    cout << "=============================" << endl;
    cout << "      猜数字游戏开始！" << endl;
    cout << "  我想了一个 1-100 的数字" << endl;
    cout << "    请来猜猜看吧！" << endl;
    cout << "=============================" << endl;

    while (true)    // 用死循环 + break 的模式
    {
        cout << "\n请输入你猜的数字: ";
        cin >> guess;
        count++;    // 每猜一次，次数+1

        if (guess > target)
        {
            cout << "猜大了！再小一点。" << endl;
        }
        else if (guess < target)
        {
            cout << "猜小了！再大一点。" << endl;
        }
        else
        {
            // 猜对了！
            cout << "\n恭喜你，猜对了！" << endl;
            cout << "答案就是: " << target << endl;
            cout << "你一共猜了 " << count << " 次" << endl;

            // 根据次数给出评价
            if (count <= 3)
            {
                cout << "评价: 天才！运气爆棚！" << endl;
            }
            else if (count <= 7)
            {
                cout << "评价: 不错！水平很高！" << endl;
            }
            else
            {
                cout << "评价: 还需要多练习哦~" << endl;
            }

            break;  // 猜对了，退出循环
        }
    }

    cout << "\n游戏结束，感谢游玩！" << endl;
    return 0;
}
```

---

## 十、while 循环的更多示例

### 示例1：累加求和

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 求 1 + 2 + 3 + ... + 100 的和
    int sum = 0;    // 累加器，初始为 0
    int i = 1;      // 循环变量

    while (i <= 100)
    {
        sum += i;   // 等价于 sum = sum + i
        i++;
    }

    cout << "1+2+3+...+100 = " << sum << endl;
    // 输出: 1+2+3+...+100 = 5050

    return 0;
}
```

### 示例2：求1到100中偶数的和

```cpp
#include <iostream>
using namespace std;

int main()
{
    int sum = 0;
    int i = 1;

    while (i <= 100)
    {
        if (i % 2 == 0)     // 如果i是偶数
        {
            sum += i;       // 累加到sum
        }
        i++;
    }

    cout << "1到100中偶数的和 = " << sum << endl;
    // 输出：1到100中偶数的和 = 2550

    return 0;
}
```

### 示例3：求阶乘

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 求 n! = 1 * 2 * 3 * ... * n
    int n;
    cout << "请输入 n: ";
    cin >> n;

    long long result = 1;  // 阶乘结果可能很大，用 long long
    int i = 1;

    while (i <= n)
    {
        result *= i;    // 等价于 result = result * i
        i++;
    }

    cout << n << "! = " << result << endl;
    // 输入 10，输出: 10! = 3628800

    return 0;
}
```

### 示例4：数字反转

```cpp
#include <iostream>
using namespace std;

int main()
{
    int num;
    cout << "请输入一个整数: ";
    cin >> num;

    int original = num;     // 保存原始值
    int reversed = 0;       // 反转后的数

    // 处理负数
    bool isNegative = false;
    if (num < 0)
    {
        isNegative = true;
        num = -num;         // 转为正数处理
    }

    // 反转数字
    while (num > 0)
    {
        int digit = num % 10;       // 取出最后一位
        reversed = reversed * 10 + digit;  // 拼接到反转结果中
        num /= 10;                  // 去掉最后一位
    }

    if (isNegative)
    {
        reversed = -reversed;
    }

    cout << original << " 反转后为 " << reversed << endl;

    return 0;
}
```

### 示例5：求最大公约数（辗转相除法）

```cpp
#include <iostream>
using namespace std;

int main()
{
    int a, b;
    cout << "请输入两个正整数: ";
    cin >> a >> b;

    // 辗转相除法（欧几里得算法）
    int originalA = a, originalB = b;
    int temp;

    while (b != 0)
    {
        temp = b;
        b = a % b;      // 求余数
        a = temp;        // 更新a
    }

    // 循环结束后，a就是最大公约数
    cout << originalA << " 和 " << originalB
         << " 的最大公约数是: " << a << endl;

    return 0;
}
```

### 示例6：倒计时

```cpp
#include <iostream>
#include <windows.h>  // 用于 Sleep() 函数
using namespace std;

int main()
{
    // 倒计时效果
    int count = 10;

    while (count >= 0)
    {
        cout << "倒计时: " << count << endl;
        Sleep(1000);    // 暂停 1000 毫秒 = 1 秒
        count--;
    }

    cout << "\n时间到！发射！" << endl;
    return 0;
}
```

---

## 十一、while 循环的变体

### 11.1 递减循环

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 倒序打印 10 到 1
    int i = 10;

    while (i >= 1)
    {
        cout << i << " ";
        i--;
    }
    cout << endl;
    // 输出：10 9 8 7 6 5 4 3 2 1

    return 0;
}
```

### 11.2 无限循环配合 break

```cpp
#include <iostream>
using namespace std;

int main()
{
    int num;

    // 持续读取输入，直到输入0
    cout << "输入正整数求和（输入0结束）: " << endl;

    int sum = 0;
    while (true)
    {
        cin >> num;
        if (num == 0)
        {
            break;
        }
        sum += num;
    }

    cout << "总和 = " << sum << endl;

    return 0;
}
```

### 11.3 条件复杂的循环

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
    // 当用户输入有效密码时退出循环
    string password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;

    while (attempts < MAX_ATTEMPTS)
    {
        cout << "请输入密码（第" << attempts + 1 << "次尝试）: ";
        cin >> password;

        if (password == "123456")
        {
            cout << "登录成功！" << endl;
            break;
        }
        else
        {
            attempts++;
            cout << "密码错误！还剩"
                 << MAX_ATTEMPTS - attempts << "次机会" << endl;
        }
    }

    if (attempts >= MAX_ATTEMPTS)
    {
        cout << "错误次数过多，账户已锁定！" << endl;
    }

    return 0;
}
```

---

## 十二、总结

### while 循环要点

| 要点 | 说明 |
|------|------|
| 语法 | `while (条件) { 循环体 }` |
| 执行顺序 | 先判断条件，再执行循环体 |
| 三要素 | 初始条件、循环条件、更新条件 |
| break | 跳出整个循环 |
| continue | 跳过本次循环，进入下一次 |

### 常见错误清单

1. **忘记更新循环变量** → 死循环
2. **循环变量更新方向错误** → 死循环
3. **while后面加分号** → 空循环（死循环）
4. **continue前忘记更新** → 死循环
5. **循环条件写错** → 逻辑错误

### while 使用口诀

```
while 循环要记牢，三要素不能少；
初始化在循环前，条件判断在中间；
更新语句别忘记，否则死循环跑不了；
break 跳出整个圈，continue 跳过这一遍；
while true 常使用，break 退出要记清。
```

### 何时使用 while

- **不确定循环次数**时（如用户输入验证、猜数字）
- **条件驱动**的循环（如读取文件直到末尾）
- **菜单循环**（配合 break 退出）
- 循环次数**已知**时，推荐使用 `for` 循环（下一章讲解）
