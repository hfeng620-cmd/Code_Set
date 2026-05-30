# do-while 循环详解

## 一、什么是 do-while 循环

`do-while` 循环是 C++ 的第二种循环结构。它与 `while` 循环的核心区别是：**先执行一次循环体，再判断条件**。

这意味着不管条件是否满足，`do-while` 的循环体**至少会执行一次**。

> 生活类比：餐厅先上菜让你吃（执行），吃完再问你"还要不要？"（判断）。而 `while` 是先问"要不要吃？"，你说不要就不吃了。

---

## 二、do-while 的基本语法

```cpp
do
{
    循环体;       // 先执行这里的代码
} while (循环条件);   // 再判断条件，注意最后有分号！
```

### 语法要点

| 组成部分 | 说明 |
|---------|------|
| `do` | 关键字，表示"做" |
| `{}` | 循环体，至少执行一次 |
| `while (条件)` | 判断是否继续循环 |
| **分号 `;`** | **必须有！这是 do-while 的标志，初学者最容易忘** |

> **重要提醒**：`do-while` 末尾的分号是必须的，这是语法规定。忘记写分号会导致编译错误！

---

## 三、do-while 的执行流程

```
开始
  |
  v
执行循环体（至少执行一次）
  |
  v
判断循环条件 --为 false--> 退出循环 --> 结束
  |
  |为 true
  v
回到执行循环体（继续下一轮）
```

**对比 while**：
- `while`：先判断 → 再执行（可能一次都不执行）
- `do-while`：先执行 → 再判断（至少执行一次）

### 基础示例

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 用 do-while 打印 1 到 5
    int i = 1;

    do
    {
        cout << "第 " << i << " 次执行" << endl;
        i++;
    } while (i <= 5);   // 注意分号！

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

## 四、do-while 与 while 的核心区别

### 4.1 至少执行一次

这是最重要的区别。看下面的对比：

```cpp
#include <iostream>
using namespace std;

int main()
{
    int num = 100;  // 条件一开始就为 false

    // while 循环：条件一开始就为 false，循环体一次都不执行
    cout << "--- while 循环 ---" << endl;
    while (num < 10)
    {
        cout << "while: num = " << num << endl;  // 不会执行
        num++;
    }
    cout << "while 结束后 num = " << num << endl;  // num 还是 100

    cout << endl;

    // do-while 循环：即使条件为 false，也会执行一次
    cout << "--- do-while 循环 ---" << endl;
    num = 100;  // 重置
    do
    {
        cout << "do-while: num = " << num << endl;  // 会执行一次！
        num++;
    } while (num < 10);
    cout << "do-while 结束后 num = " << num << endl;  // num 变成 101

    return 0;
}
```

输出结果：
```
--- while 循环 ---
while 结束后 num = 100

--- do-while 循环 ---
do-while: num = 100
do-while 结束后 num = 101
```

### 4.2 对比总结

| 对比项 | while | do-while |
|-------|-------|----------|
| 执行顺序 | 先判断，后执行 | 先执行，后判断 |
| 最少执行次数 | 0 次 | 1 次 |
| 适用场景 | 不确定是否需要执行 | 至少需要执行一次 |
| 末尾标点 | 无分号 | 必须有分号 |

---

## 五、do-while 的使用场景

### 场景1：菜单系统

do-while 最经典的使用场景就是**菜单系统**——菜单至少要显示一次，然后根据用户选择决定是否继续。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int choice;

    do
    {
        // 菜单至少显示一次
        cout << "\n====== 主菜单 ======" << endl;
        cout << "1. 开始游戏" << endl;
        cout << "2. 游戏设置" << endl;
        cout << "3. 查看帮助" << endl;
        cout << "0. 退出程序" << endl;
        cout << "===================" << endl;
        cout << "请输入选项: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                cout << ">> 游戏开始！祝你好运！" << endl;
                break;
            case 2:
                cout << ">> 进入设置界面..." << endl;
                break;
            case 3:
                cout << ">> 使用方向键移动，空格键攻击" << endl;
                break;
            case 0:
                cout << ">> 感谢使用，再见！" << endl;
                break;
            default:
                cout << ">> 无效选项，请重新输入！" << endl;
                break;
        }
    } while (choice != 0);   // 输入 0 时退出循环

    return 0;
}
```

### 场景2：输入验证

do-while 非常适合**输入验证**——先让用户输入，如果不合法就要求重新输入。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int age;

    do
    {
        cout << "请输入您的年龄（1-150）: ";
        cin >> age;

        if (age < 1 || age > 150)
        {
            cout << "年龄输入有误，请重新输入！" << endl;
        }
    } while (age < 1 || age > 150);   // 不合法就继续循环

    cout << "您输入的年龄是: " << age << " 岁" << endl;

    return 0;
}
```

**为什么用 do-while 而不是 while？** 因为年龄**至少要输入一次**才能验证，这正是 do-while 的特点。

### 场景3：密码验证

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
    string password;
    int attempts = 0;

    do
    {
        cout << "请输入密码: ";
        cin >> password;
        attempts++;

        if (password != "admin123")
        {
            cout << "密码错误！请重试。" << endl;
        }

    } while (password != "admin123" && attempts < 3);

    if (password == "admin123")
    {
        cout << "登录成功！" << endl;
    }
    else
    {
        cout << "错误次数过多，账户已锁定！" << endl;
    }

    return 0;
}
```

---

## 六、break 在 do-while 中的作用

`break` 同样可以立即跳出 do-while 循环。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int i = 1;

    do
    {
        if (i == 3)
        {
            cout << "遇到 3，提前退出！" << endl;
            break;    // 立即跳出循环
        }
        cout << "i = " << i << endl;
        i++;
    } while (i <= 5);

    cout << "循环结束" << endl;
    return 0;
}
```

输出结果：
```
i = 1
i = 2
遇到 3，提前退出！
循环结束
```

---

## 七、continue 在 do-while 中的作用

`continue` 跳过本次循环剩余代码，直接跳到 `while (条件)` 处进行判断。

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 打印 1-10 中不是 3 的倍数的数
    int i = 0;

    do
    {
        i++;                  // 更新变量

        if (i % 3 == 0)      // 如果是 3 的倍数
        {
            continue;         // 跳过下面的 cout，去判断条件
        }

        cout << i << " ";
    } while (i <= 10);

    cout << endl;
    // 输出: 1 2 4 5 7 8 10

    return 0;
}
```

**与 while 中 continue 的区别**：在 do-while 中，`continue` 跳到 `while (条件)` 处；在 while 中，`continue` 跳回 `while (条件)` 处。行为基本相同，但在 do-while 中要注意**变量更新的位置**，避免死循环。

---

## 八、实战案例

### 案例1：找出所有水仙花数

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 水仙花数：一个三位数，各位数字的立方和等于该数本身
    // 例如：153 = 1^3 + 5^3 + 3^3 = 1 + 125 + 27 = 153

    cout << "所有的水仙花数：" << endl;

    int num = 100;  // 从 100 开始（最小的三位数）

    do
    {
        // 分离出百位、十位、个位
        int bai = num / 100;            // 百位
        int shi = num / 10 % 10;        // 十位
        int ge  = num % 10;             // 个位

        // 计算各位数字的立方和
        int sum = bai * bai * bai
                + shi * shi * shi
                + ge  * ge  * ge;

        // 判断是否为水仙花数
        if (sum == num)
        {
            cout << num << " = "
                 << bai << "^3 + "
                 << shi << "^3 + "
                 << ge  << "^3" << endl;
        }

        num++;
    } while (num <= 999);   // 到 999 结束（最大的三位数）

    return 0;
}
```

输出结果：
```
所有的水仙花数：
153 = 1^3 + 5^3 + 3^3
370 = 3^3 + 7^3 + 0^3
371 = 3^3 + 7^3 + 1^3
407 = 4^3 + 0^3 + 7^3
```

### 案例2：菜单系统（完整版——银行账户管理）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 一个完整的银行账户管理菜单系统
    int choice;
    double balance = 10000.0;   // 初始余额
    double amount;

    do
    {
        // 显示菜单
        cout << endl;
        cout << "========================================" << endl;
        cout << "         银行账户管理系统" << endl;
        cout << "========================================" << endl;
        cout << "  当前余额: " << balance << " 元" << endl;
        cout << "========================================" << endl;
        cout << "  1. 存款" << endl;
        cout << "  2. 取款" << endl;
        cout << "  3. 查询余额" << endl;
        cout << "  0. 退出系统" << endl;
        cout << "========================================" << endl;
        cout << "请选择操作: ";
        cin >> choice;

        // 处理用户选择
        switch (choice)
        {
            case 1:     // 存款
                cout << "请输入存款金额: ";
                cin >> amount;
                if (amount > 0)
                {
                    balance += amount;
                    cout << "存款成功！存入 " << amount << " 元" << endl;
                    cout << "当前余额: " << balance << " 元" << endl;
                }
                else
                {
                    cout << "存款金额必须大于0！" << endl;
                }
                break;

            case 2:     // 取款
                cout << "请输入取款金额: ";
                cin >> amount;
                if (amount <= 0)
                {
                    cout << "取款金额必须大于0！" << endl;
                }
                else if (amount > balance)
                {
                    cout << "余额不足！当前余额: " << balance << " 元" << endl;
                }
                else
                {
                    balance -= amount;
                    cout << "取款成功！取出 " << amount << " 元" << endl;
                    cout << "当前余额: " << balance << " 元" << endl;
                }
                break;

            case 3:     // 查询余额
                cout << "当前余额: " << balance << " 元" << endl;
                break;

            case 0:     // 退出
                cout << "========================================" << endl;
                cout << "  最终余额: " << balance << " 元" << endl;
                cout << "  感谢使用银行账户管理系统！" << endl;
                cout << "========================================" << endl;
                break;

            default:    // 无效选择
                cout << "无效选择，请重新输入！" << endl;
                break;
        }

    } while (choice != 0);

    return 0;
}
```

### 案例3：猜数字游戏（do-while 版）

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    srand((unsigned int)time(0));
    int target = rand() % 100 + 1;
    int guess;
    int attempts = 0;

    cout << "============================" << endl;
    cout << "    猜数字游戏" << endl;
    cout << "============================" << endl;
    cout << "我想了一个 1-100 之间的数字" << endl;
    cout << endl;

    do
    {
        cout << "请输入你的猜测: ";
        cin >> guess;
        attempts++;

        if (guess > target)
        {
            cout << "太大了！" << endl;
        }
        else if (guess < target)
        {
            cout << "太小了！" << endl;
        }
        else
        {
            cout << "恭喜你，猜对了！" << endl;
            cout << "你一共猜了 " << attempts << " 次" << endl;
        }

    } while (guess != target);  // 猜对了就退出

    return 0;
}
```

### 案例4：累加器程序

```cpp
#include <iostream>
using namespace std;

int main()
{
    double number;
    double sum = 0;
    int count = 0;
    char continueInput;

    do
    {
        cout << "请输入一个数字: ";
        cin >> number;

        sum += number;
        count++;

        cout << "是否继续输入？(y/n): ";
        cin >> continueInput;

    } while (continueInput == 'y' || continueInput == 'Y');

    cout << endl;
    cout << "========== 统计结果 ==========" << endl;
    cout << "输入了 " << count << " 个数字" << endl;
    cout << "总和 = " << sum << endl;
    cout << "平均值 = " << sum / count << endl;

    return 0;
}
```

---

## 九、do-while 的嵌套使用

do-while 也可以嵌套使用：

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 外层：控制行数
    int row = 1;
    do
    {
        // 内层：控制每行的列数
        int col = 1;
        do
        {
            cout << "* ";
            col++;
        } while (col <= row);

        cout << endl;   // 每行结束后换行
        row++;
    } while (row <= 5);

    return 0;
}
```

输出结果：
```
* 
* * 
* * * 
* * * * 
* * * * * 
```

---

## 十、常见错误

### 错误1：忘记末尾分号

```cpp
// 错误写法
do
{
    cout << i << endl;
    i++;
} while (i <= 5)    // 缺少分号！编译错误！

// 正确写法
do
{
    cout << i << endl;
    i++;
} while (i <= 5);   // 别忘了分号！
```

### 错误2：continue 导致变量未更新

```cpp
// 错误：continue 跳过了 i++
int i = 1;
do
{
    if (i == 3)
    {
        continue;   // i 永远是 3，死循环！
    }
    cout << i << endl;
    i++;            // 这行被 continue 跳过了
} while (i <= 5);

// 正确：把更新放在 continue 之前
int i = 0;
do
{
    i++;            // 先更新
    if (i == 3)
    {
        continue;   // 跳过的是 cout，不影响 i 的更新
    }
    cout << i << endl;
} while (i <= 5);
```

### 错误3：条件写反

```cpp
// 注意理解 do-while 至少执行一次的特性
int i = 10;
do
{
    cout << "至少执行一次" << endl;
    i++;
} while (i < 0);    // 条件为假，但已经执行了一次
// 输出：至少执行一次
```

---

## 十一、总结

### do-while 使用口诀

```
do-while 很特别，先做后判断是规则；
至少执行那一次，while 做不到这区别；
末尾分号不能忘，否则编译就报错；
菜单输入验证好，先做后判正合适；
break 跳出整循环，continue 跳过到判断。
```

### do-while 要点

| 要点 | 说明 |
|------|------|
| 语法 | `do { 循环体 } while (条件);` |
| 执行顺序 | 先执行循环体，再判断条件 |
| 最少执行次数 | **1次**（与 while 的区别） |
| 分号 | while 后面**必须有分号** |
| 适用场景 | 菜单显示、输入验证、至少执行一次 |

### 何时使用 do-while

| 场景 | 为什么适合 do-while |
|------|-------------------|
| 菜单系统 | 菜单至少要显示一次 |
| 输入验证 | 至少要输入一次才能验证 |
| 游戏循环 | 游戏至少要运行一局 |
| 密码验证 | 至少要输入一次密码 |
| 水仙花数等遍历 | 需要从头到尾遍历所有数 |

### while vs do-while 选择指南

```
问自己：循环体是否至少需要执行一次？
  |
  +-- 是 → 用 do-while
  |
  +-- 否 → 用 while
```
