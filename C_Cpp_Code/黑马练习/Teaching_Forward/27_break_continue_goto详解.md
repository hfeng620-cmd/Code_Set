# break、continue、goto 详解

## 一、概述

C++ 提供了三种跳转语句，用于改变程序的执行流程：

| 关键字 | 作用 | 使用场景 |
|-------|------|---------|
| `break` | 跳出当前循环或 switch | 循环中提前退出 |
| `continue` | 跳过本次循环，进入下一次 | 循环中跳过某些情况 |
| `goto` | 无条件跳转到标签 | **不推荐使用** |

---

## 二、break 详解

### 2.1 break 跳出一层循环

`break` 只能跳出**当前最内层**的循环，不能一次跳出多层。

```cpp
#include <iostream>
using namespace std;

int main()
{
    // break 跳出 for 循环
    for (int i = 1; i <= 10; i++)
    {
        if (i == 6)
        {
            cout << "遇到6，跳出循环" << endl;
            break;      // 跳出这个 for 循环
        }
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 3 4 5 遇到6，跳出循环

    return 0;
}
```

```cpp
#include <iostream>
using namespace std;

int main()
{
    // break 跳出 while 循环
    int i = 1;
    while (i <= 10)
    {
        if (i == 4)
        {
            cout << "遇到4，跳出循环" << endl;
            break;
        }
        cout << i << " ";
        i++;
    }
    cout << endl;
    // 输出: 1 2 3 遇到4，跳出循环

    return 0;
}
```

```cpp
#include <iostream>
using namespace std;

int main()
{
    // break 跳出 do-while 循环
    int i = 1;
    do
    {
        if (i == 3)
        {
            cout << "遇到3，跳出循环" << endl;
            break;
        }
        cout << i << " ";
        i++;
    } while (i <= 5);
    cout << endl;
    // 输出: 1 2 遇到3，跳出循环

    return 0;
}
```

### 2.2 break 跳出 switch

`break` 在 switch 中的作用是跳出 switch 结构，防止穿透。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int choice = 2;

    switch (choice)
    {
        case 1:
            cout << "选择了1" << endl;
            break;          // 跳出 switch
        case 2:
            cout << "选择了2" << endl;
            break;          // 跳出 switch，不会执行 case 3
        case 3:
            cout << "选择了3" << endl;
            break;
        default:
            cout << "无效选择" << endl;
            break;
    }
    // 输出: 选择了2

    return 0;
}
```

### 2.3 break 在嵌套循环中只跳最内层

```cpp
#include <iostream>
using namespace std;

int main()
{
    for (int i = 1; i <= 3; i++)
    {
        cout << "外层 i=" << i << ": ";

        for (int j = 1; j <= 5; j++)
        {
            if (j == 3)
            {
                break;  // 只跳出内层 for，外层继续
            }
            cout << j << " ";
        }

        cout << "（内层结束）" << endl;
    }

    return 0;
}
```

输出：
```
外层 i=1: 1 2 （内层结束）
外层 i=2: 1 2 （内层结束）
外层 i=3: 1 2 （内层结束）
```

### 2.4 break 的执行流程图

```
for (...) {
    语句1;
    if (条件) {
        break;  ──────────┐
    }                     │
    语句2;   ← 这行被跳过 │
    语句3;   ← 这行也被跳过│
}   ←────────────────────┘
后续代码;  ← break 后从这里继续执行
```

---

## 三、continue 详解

### 3.1 continue 跳过本次循环

`continue` 的作用是**跳过本次循环剩余的代码**，直接进入下一次循环。

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
            continue;   // 跳过本次循环（跳过下面的 cout）
        }
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 4 5 7 8 10

    return 0;
}
```

### 3.2 continue 的执行流程图

```
for (i = 1; i <= 10; i++) {
    if (i % 3 == 0) {
        continue;  ──────────┐
    }                        │
    cout << i;  ← 这行被跳过 │
}   ←────────────────────────┘
    i++;   ← continue 后跳到更新表达式
    判断条件 ← 然后进入下一次判断
```

### 3.3 break vs continue 对比

```cpp
#include <iostream>
using namespace std;

int main()
{
    cout << "break 演示：" << endl;
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3) break;
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2（遇到3直接退出循环）

    cout << "continue 演示：" << endl;
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3) continue;
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 4 5（跳过3，继续下一次）

    return 0;
}
```

**区别总结**：
- `break`：终止整个循环，后面都不执行了
- `continue`：只跳过这一次，下一次继续

---

## 四、continue 在 for 和 while 中的区别

这是初学者非常容易搞混的地方。

### 4.1 在 for 循环中（安全）

```cpp
// for 循环中使用 continue
for (int i = 1; i <= 10; i++)     // ← continue 跳到这里（i++）
{
    if (i == 5)
    {
        continue;   // 跳过 cout，但 i++ 会执行
    }
    cout << i << " ";
}
cout << endl;
// 输出: 1 2 3 4 6 7 8 9 10
// i=5 时跳过了 cout，但 i++ 正常执行，不会死循环
```

### 4.2 在 while 循环中（危险！）

```cpp
// while 循环中使用 continue（危险！）
int i = 1;
while (i <= 10)
{
    if (i == 5)
    {
        continue;   // 跳回 while 判断，i++ 被跳过！
        // i 永远是 5，永远 <= 10，死循环！
    }
    cout << i << " ";
    i++;            // 这行被 continue 跳过了！
}
```

### 4.3 正确的 while + continue 写法

```cpp
// 方法1：在 continue 前手动更新
int i = 1;
while (i <= 10)
{
    if (i == 5)
    {
        i++;            // 先更新
        continue;       // 再跳过
    }
    cout << i << " ";
    i++;
}
cout << endl;
// 输出: 1 2 3 4 6 7 8 9 10

// 方法2：把更新放在循环开头
int j = 0;
while (j < 10)
{
    j++;                // 更新放在最前面
    if (j == 5)
    {
        continue;
    }
    cout << j << " ";
}
cout << endl;
// 输出: 1 2 3 4 6 7 8 9 10

// 方法3（推荐）：直接用 for 循环
for (int k = 1; k <= 10; k++)
{
    if (k == 5) continue;
    cout << k << " ";
}
cout << endl;
// 输出: 1 2 3 4 6 7 8 9 10
```

### 4.4 对比总结

| 对比项 | for 循环 | while 循环 |
|-------|---------|-----------|
| continue 跳转到 | 更新表达式（i++） | while 条件判断处 |
| 更新语句是否执行 | **会执行**（安全） | **不会执行**（危险） |
| 是否可能导致死循环 | 不会 | 如果更新在 continue 后面，会 |

---

## 五、continue 的陷阱——while 中忘更新

### 5.1 陷阱演示

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 0;

    // 这个循环看起来没问题，但实际上有隐患
    while (i < 10)
    {
        i++;

        if (i == 5)
        {
            cout << "跳过5" << endl;
            continue;   // 跳过了下面的 cout
        }

        // 这行被跳过了，但 i++ 没被跳过，所以没问题
        cout << i << " ";
    }
    cout << endl;
    // 输出: 1 2 3 4 跳过5 6 7 8 9 10

    return 0;
}
```

### 5.2 真正的陷阱

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 1;

    // 错误！continue 在 i++ 之前，且没有其他更新
    while (i <= 10)
    {
        if (i == 5)
        {
            // continue 跳回 while 判断
            // 但 i++ 在 continue 之后，永远不会执行
            // i 永远是 5，永远 <= 10
            // 死循环！
            continue;
        }

        cout << i << " ";
        i++;    // 当 i==5 时，这行永远不会执行
    }

    return 0;
}
```

### 5.3 如何避免

```cpp
// 规则：在 while 循环中使用 continue 时，
// 确保循环变量的更新在 continue 之前

int i = 1;
while (i <= 10)
{
    i++;            // 更新放在最前面（或 continue 之前）

    if (i == 5)
    {
        continue;   // 安全！i 已经更新了
    }

    cout << i << " ";
}
```

---

## 六、goto 详解

### 6.1 goto 的语法

```cpp
goto 标签名;    // 无条件跳转到标签处
// ...
标签名:         // 标签定义（后面跟冒号）
    语句;
```

### 6.2 goto 的基本用法

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 1;

start:      // 标签
    cout << i << " ";
    i++;

    if (i <= 5)
    {
        goto start;     // 跳回 start 标签
    }

    cout << endl;
    // 输出: 1 2 3 4 5

    return 0;
}
```

### 6.3 goto 为什么不推荐使用

**原因1：破坏程序的结构化**

```cpp
// goto 可以随意跳转，让代码变成"意大利面条"——难以阅读和维护
// 例如：
goto labelB;
labelA:
    cout << "A" << endl;
    goto labelC;
labelB:
    cout << "B" << endl;
    goto labelA;
labelC:
    cout << "C" << endl;
// 输出: B A C
// 跳来跳去，很难追踪执行顺序
```

**原因2：可以用循环和 break/continue 代替**

```cpp
// 使用 goto 的写法（不推荐）
int i = 1;
loop_start:
    cout << i << " ";
    i++;
    if (i <= 5) goto loop_start;

// 等价的 for 循环写法（推荐）
for (int i = 1; i <= 5; i++)
{
    cout << i << " ";
}
```

**原因3：容易产生 bug**

```cpp
// goto 可能跳过变量初始化
goto skip;
int x = 10;     // 这行被跳过了
skip:
    // cout << x;  // 错误！x 未初始化
    cout << "跳过了变量声明" << endl;
```

**历史背景**：早期的编程语言（如汇编、BASIC）没有结构化控制语句，只能用 `goto`。现代语言有了 `for`、`while`、`break`、`continue`、`return`，基本不需要 `goto`。

### 6.4 goto 的唯一合理场景——多层循环跳出

当需要从多层嵌套循环中一次性跳出时，`goto` 是最简洁的方案（虽然也可以用标志变量）。

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 在一个 5x5 的矩阵中查找数字 42
    int matrix[5][5] = {
        {1,  2,  3,  4,  5},
        {6,  7,  8,  9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    // 使用 goto 跳出多层循环
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (matrix[i][j] == 22)
            {
                cout << "找到 22，在位置 [" << i << "][" << j << "]" << endl;
                goto found_exit;    // 直接跳出两层循环
            }
        }
    }
    found_exit:
    cout << "查找结束" << endl;

    return 0;
}
```

### 6.5 不用 goto 的替代方案

```cpp
#include <iostream>
using namespace std;

int main()
{
    int matrix[5][5] = {
        {1,  2,  3,  4,  5},
        {6,  7,  8,  9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };

    // 方法1：使用标志变量（推荐）
    bool found = false;
    for (int i = 0; i < 5 && !found; i++)
    {
        for (int j = 0; j < 5 && !found; j++)
        {
            if (matrix[i][j] == 22)
            {
                cout << "找到 22，在位置 [" << i << "][" << j << "]" << endl;
                found = true;
            }
        }
    }

    // 方法2：将嵌套循环封装成函数，用 return 退出（最推荐）
    // 在实际项目中，这是最好的做法

    return 0;
}
```

### 6.6 goto 的限制

```cpp
// 限制1：不能跳过变量初始化
// goto label;
// int x = 10;     // 如果 goto 跳到这里，x 的初始化被跳过
// label:

// 限制2：不能跳出函数
// void func() {
//     goto label;  // 不能跳到其他函数的标签
// }

// 限制3：不能跳入循环内部
// goto inner;
// for (...) {
// inner:           // 不允许从外部 goto 跳入循环内部
//     ...
// }
```

---

## 七、break vs continue vs goto 对比

| 特性 | break | continue | goto |
|------|-------|----------|------|
| 作用 | 跳出当前循环/switch | 跳过本次循环 | 无条件跳转到标签 |
| 影响范围 | 只影响最内层循环 | 只影响当前循环 | 可以跳到任意位置 |
| 推荐程度 | 推荐 | 推荐 | **不推荐** |
| 替代方案 | 无（必须用） | 无（必须用） | 用标志变量/函数 return |

### 执行流程对比

```cpp
for (int i = 1; i <= 5; i++)
{
    if (i == 3)
    {
        // break:    输出 1 2，然后退出循环
        // continue: 输出 1 2 4 5，跳过3
        // goto end: 输出 1 2，然后跳到 end 标签
    }
    cout << i << " ";
}
end:
cout << "结束" << endl;
```

---

## 八、实战综合案例

### 案例1：查找素数（break 的典型应用）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 找出 2-100 之间所有的素数
    cout << "2-100 之间的素数: " << endl;

    for (int num = 2; num <= 100; num++)
    {
        bool isPrime = true;

        // 检查是否有因子
        for (int i = 2; i * i <= num; i++)
        {
            if (num % i == 0)
            {
                isPrime = false;  // 找到因子，不是素数
                break;            // 提前退出内层循环
            }
        }

        if (isPrime)
        {
            cout << num << " ";
        }
    }
    cout << endl;

    return 0;
}
```

### 案例2：跳过特定数字（continue 的典型应用）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 计算 1-100 中所有不是 7 的倍数且不包含 7 的数的和
    int sum = 0;
    int count = 0;

    for (int i = 1; i <= 100; i++)
    {
        // 跳过 7 的倍数
        if (i % 7 == 0)
        {
            continue;
        }
        // 跳过个位是 7 的数
        if (i % 10 == 7)
        {
            continue;
        }
        // 跳过十位是 7 的数
        if (i / 10 == 7)
        {
            continue;
        }

        sum += i;
        count++;
    }

    cout << "符合条件的数有 " << count << " 个" << endl;
    cout << "它们的和是: " << sum << endl;

    return 0;
}
```

### 案例3：综合练习——break + continue + 无限循环

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 练习：找到前 5 个既能被 3 整除又能被 5 整除的数
    int count = 0;

    for (int i = 1; ; i++)   // 无限循环
    {
        if (i % 3 == 0 && i % 5 == 0)
        {
            cout << i << " ";
            count++;
            if (count == 5) break;   // 找够5个就退出
        }
    }
    cout << endl;
    // 输出: 15 30 45 60 75

    // 练习：输出 1-100 中所有不能被 3 整除且不能被 5 整除的数
    for (int i = 1; i <= 100; i++)
    {
        if (i % 3 == 0) continue;   // 能被3整除，跳过
        if (i % 5 == 0) continue;   // 能被5整除，跳过
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
```

---

## 九、常见错误总结

### 错误1：break 写在 switch 中以为能跳出循环

```cpp
for (int i = 0; i < 10; i++)
{
    switch (i % 3)
    {
        case 0:
            cout << "A";
            break;      // 这个 break 只跳出 switch，不跳出 for！
        case 1:
            cout << "B";
            break;
        case 2:
            cout << "C";
            break;
    }
}
// for 循环会完整执行 10 次
```

### 错误2：continue 在 while 中导致死循环

```cpp
// 已在上面详细讲解，这里再次强调
int i = 1;
while (i <= 10)
{
    if (i == 5) continue;   // 死循环！i++ 被跳过
    cout << i << " ";
    i++;
}
```

### 错误3：goto 跳过变量初始化

```cpp
goto label;
int x = 10;     // 编译错误！goto 跳过了变量初始化
label:
    cout << x << endl;
```

---

## 十、总结

### break 要点

- 跳出**当前最内层**循环或 switch
- 不影响外层循环
- 在 switch 中防止穿透

### continue 要点

- 跳过**本次循环**的剩余代码
- 在 for 中：跳到更新表达式（安全）
- 在 while 中：跳到条件判断处（注意更新变量！）

### goto 要点

- 无条件跳转到标签处
- **不推荐使用**，破坏代码结构
- 唯一合理场景：多层循环跳出

### 使用口诀

```
break 跳出整循环，一层一层往外蹦；
continue 跳过它，本次不做下回轮；
for 中 continue 安全，while 中要小心；
goto 能不用就不用，跳出多层偶尔用。
```

### 选择指南

```
需要提前退出循环？      → break
需要跳过某些情况？      → continue
需要从多层循环中跳出？  → 标志变量（推荐）或 goto（可接受）
需要在 switch 中退出？  → break
```
