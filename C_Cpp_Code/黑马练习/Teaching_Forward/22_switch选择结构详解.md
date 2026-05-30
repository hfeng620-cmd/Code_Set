# switch 选择结构详解

## 一、什么是 switch 选择结构

在 C++ 中，`switch` 是一种**多分支选择结构**，专门用于根据一个变量的**不同取值**来执行不同的代码块。

当你要判断的条件是"**是否等于某个值**"时，`switch` 比多个 `if-else if` 更清晰、更易读。

> 生活类比：就像自动售货机——你按下不同的按钮（1号、2号、3号……），就会掉出不同的商品。

---

## 二、switch 的基本语法

```cpp
switch (表达式)
{
    case 常量1:
        语句1;
        break;
    case 常量2:
        语句2;
        break;
    case 常量3:
        语句3;
        break;
    // ... 可以有任意多个 case
    default:
        默认语句;
        break;
}
```

### 语法要点说明

| 组成部分 | 说明 |
|---------|------|
| `switch (表达式)` | 表达式的值会依次与每个 `case` 后的常量比较 |
| `case 常量:` | 每个 `case` 后面必须是**整型或字符型常量**（不能是变量、浮点数、字符串） |
| `break;` | 跳出 `switch` 结构，不写会发生"穿透" |
| `default:` | 所有 `case` 都不匹配时执行，类似 `else`，可以省略 |

---

## 三、switch 的执行流程

```
开始
  |
  v
计算表达式的值
  |
  v
与 case 1 比较 --匹配--> 执行 case 1 语句 --> break? --> 结束
  |不匹配
  v
与 case 2 比较 --匹配--> 执行 case 2 语句 --> break? --> 结束
  |不匹配
  v
与 case 3 比较 --匹配--> 执行 case 3 语句 --> break? --> 结束
  |不匹配
  v
执行 default 语句 --> 结束
```

**关键理解**：switch 并不是"从头到尾每个都判断"，而是找到第一个匹配的 `case`，从那里开始执行，直到遇到 `break` 或 `switch` 结束。

---

## 四、break 的作用——跳出 switch

`break` 的作用是**立即跳出 switch 结构**，不再执行后面的 case。

### 示例：根据数字输出星期

```cpp
#include <iostream>
using namespace std;

int main()
{
    int day = 3;

    switch (day)
    {
        case 1:
            cout << "星期一" << endl;
            break;       // 执行完后跳出 switch
        case 2:
            cout << "星期二" << endl;
            break;
        case 3:
            cout << "星期三" << endl;
            break;       // 匹配到这个 case，执行后跳出
        case 4:
            cout << "星期四" << endl;
            break;
        case 5:
            cout << "星期五" << endl;
            break;
        default:
            cout << "周末" << endl;
            break;
    }

    // 程序继续执行这里的代码
    cout << "switch 结构结束" << endl;
    return 0;
}
```

输出结果：
```
星期三
switch 结构结束
```

---

## 五、default 默认分支

当所有 `case` 都不匹配时，会执行 `default` 分支。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int score;
    cout << "请输入成绩等级（1-5）: ";
    cin >> score;

    switch (score)
    {
        case 1:
            cout << "不及格" << endl;
            break;
        case 2:
            cout << "及格" << endl;
            break;
        case 3:
            cout << "中等" << endl;
            break;
        case 4:
            cout << "良好" << endl;
            break;
        case 5:
            cout << "优秀" << endl;
            break;
        default:
            // 当用户输入的不是 1~5 时，走这里
            cout << "输入有误，请输入1-5之间的数字！" << endl;
            break;
    }

    return 0;
}
```

**注意**：`default` 可以省略，但建议保留，用于处理意外输入。

---

## 六、switch 的穿透特性及利用

### 6.1 什么是穿透（fall-through）

如果 `case` 后面**没有写 `break`**，程序会**继续往下执行**下一个 `case` 的语句，而不再判断是否匹配。这就是"穿透"。

### 6.2 穿透演示（常见错误）

```cpp
#include <iostream>
using namespace std;

int main()
{
    int num = 1;

    switch (num)
    {
        case 1:
            cout << "一" << endl;
            // 忘记写 break，发生穿透！
        case 2:
            cout << "二" << endl;
            // 继续穿透！
        case 3:
            cout << "三" << endl;
            break;
    }

    return 0;
}
```

输出结果（注意不是只输出"一"）：
```
一
二
三
```

> **分析**：虽然 `num` 是 1，只匹配 `case 1`，但由于 `case 1` 没有 `break`，程序穿透到了 `case 2`，又穿透到了 `case 3`，直到遇到 `break` 才停止。

### 6.3 利用穿透特性——多个 case 共享代码

穿透并非全是坏事，有时候我们可以**故意利用穿透**让多个 `case` 共享同一段代码。

```cpp
#include <iostream>
using namespace std;

int main()
{
    int month;
    cout << "请输入月份（1-12）: ";
    cin >> month;

    switch (month)
    {
        case 3:
        case 4:
        case 5:
            // 3、4、5 月都会穿透到这里
            cout << "春天" << endl;
            break;
        case 6:
        case 7:
        case 8:
            cout << "夏天" << endl;
            break;
        case 9:
        case 10:
        case 11:
            cout << "秋天" << endl;
            break;
        case 12:
        case 1:
        case 2:
            cout << "冬天" << endl;
            break;
        default:
            cout << "月份输入有误！" << endl;
            break;
    }

    return 0;
}
```

**技巧**：让多个空的 `case` 连续排列，最终共享同一个代码块和 `break`。

---

## 七、switch 与 if 的区别

这是初学者最容易混淆的地方，一定要牢记：

### 7.1 switch 只能判断整型或字符型

```cpp
// 正确：整型
int a = 1;
switch (a) { ... }

// 正确：字符型
char ch = 'A';
switch (ch) { ... }

// 错误！switch 不能用浮点数
// double d = 3.14;
// switch (d) { ... }  // 编译报错

// 错误！switch 不能用字符串
// string s = "hello";
// switch (s) { ... }  // 编译报错
```

### 7.2 switch 只能判断"是否相等"

```cpp
// switch 只能这样用：
switch (a)
{
    case 1: ...  // 判断 a 是否等于 1
    case 2: ...  // 判断 a 是否等于 2
}

// 以下判断 switch 做不到，必须用 if：
// if (a > 10)        → 判断大于
// if (a < 10)        → 判断小于
// if (a >= 10)       → 判断大于等于
// if (a != 10)       → 判断不等于
// if (a >= 1 && a <= 10)  → 判断范围
```

### 7.3 对比总结

| 对比项 | if | switch |
|-------|-----|--------|
| 数据类型 | 任意类型 | 只能整型、字符型 |
| 判断方式 | 大于、小于、等于、不等于、范围等 | 只能判断是否相等 |
| 适用场景 | 条件复杂、范围判断 | 条件是几个固定值 |
| 可读性 | 条件多时嵌套深 | 多分支时更清晰 |
| 效率 | 逐个判断 | 编译器可能优化为跳转表（更快） |

---

## 八、常见错误

### 错误1：忘记写 break

```cpp
// 错误写法——穿透导致逻辑错误
switch (choice)
{
    case 1:
        cout << "功能一" << endl;
        // 漏了 break，会继续执行 case 2 的代码！
    case 2:
        cout << "功能二" << endl;
        break;
}

// 正确写法——每个 case 都加 break
switch (choice)
{
    case 1:
        cout << "功能一" << endl;
        break;
    case 2:
        cout << "功能二" << endl;
        break;
}
```

### 错误2：case 后面不是常量

```cpp
int a = 1;
int b = 2;

// 错误！case 后面不能是变量
// switch (a)
// {
//     case b:    // 编译报错！b 是变量，不是常量
//         break;
// }

// 正确：case 后面必须是常量（字面量或 const 常量）
switch (a)
{
    case 1:        // 正确，1 是字面量
        break;
    case 2:        // 正确，2 是字面量
        break;
}

// 也可以用 const 常量
const int OPTION_A = 1;
const int OPTION_B = 2;
switch (a)
{
    case OPTION_A:   // 正确，const 常量
        break;
    case OPTION_B:
        break;
}
```

### 错误3：switch 的表达式用了浮点数

```cpp
// 错误！浮点数有精度问题，不能用于 switch
// double price = 9.99;
// switch (price) { ... }  // 编译报错
```

### 错误4：case 值重复

```cpp
// 错误！同一个 switch 中不能有两个相同的 case 值
// switch (x)
// {
//     case 1:
//         cout << "第一个1" << endl;
//         break;
//     case 1:         // 编译错误！case 1 重复
//         cout << "第二个1" << endl;
//         break;
// }
```

---

## 九、实战案例

### 案例1：简易计算器

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 简易计算器：实现 +、-、*、/ 四则运算
    double num1, num2;   // 两个操作数
    char op;             // 运算符

    cout << "========================" << endl;
    cout << "   简易计算器 v1.0" << endl;
    cout << "========================" << endl;

    cout << "请输入表达式（如 3 + 5）: ";
    cin >> num1 >> op >> num2;

    switch (op)
    {
        case '+':
            cout << num1 << " + " << num2 << " = " << num1 + num2 << endl;
            break;
        case '-':
            cout << num1 << " - " << num2 << " = " << num1 - num2 << endl;
            break;
        case '*':
            cout << num1 << " * " << num2 << " = " << num1 * num2 << endl;
            break;
        case '/':
            // 除法要检查除数是否为0
            if (num2 != 0)
            {
                cout << num1 << " / " << num2 << " = " << num1 / num2 << endl;
            }
            else
            {
                cout << "错误：除数不能为0！" << endl;
            }
            break;
        case '%':   // 取模运算（需要整数）
            if ((int)num2 != 0)
            {
                cout << (int)num1 << " % " << (int)num2 << " = "
                     << (int)num1 % (int)num2 << endl;
            }
            else
            {
                cout << "错误：除数不能为0！" << endl;
            }
            break;
        default:
            cout << "错误：不支持的运算符 '" << op << "'" << endl;
            cout << "支持的运算符：+ - * / %" << endl;
            break;
    }

    return 0;
}
```

运行效果：
```
========================
   简易计算器 v1.0
========================
请输入表达式（如 3 + 5）: 10 / 3
10 / 3 = 3.33333
```

### 案例2：根据数字输出星期几

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 根据用户输入的数字（1-7），输出对应的星期几
    int day;

    cout << "请输入一个数字（1-7）: ";
    cin >> day;

    cout << "您输入的是: " << day << endl;
    cout << "对应的是: ";

    switch (day)
    {
        case 1:
            cout << "星期一（Monday）" << endl;
            cout << "今天要开周会，加油！" << endl;
            break;
        case 2:
            cout << "星期二（Tuesday）" << endl;
            break;
        case 3:
            cout << "星期三（Wednesday）" << endl;
            cout << "一周过半了！" << endl;
            break;
        case 4:
            cout << "星期四（Thursday）" << endl;
            break;
        case 5:
            cout << "星期五（Friday）" << endl;
            cout << "明天就周末了，开心！" << endl;
            break;
        case 6:
            cout << "星期六（Saturday）" << endl;
            cout << "周末愉快！" << endl;
            break;
        case 7:
            cout << "星期日（Sunday）" << endl;
            cout << "好好休息，明天要上班/上学！" << endl;
            break;
        default:
            cout << "输入有误！请输入1-7之间的数字。" << endl;
            break;
    }

    // 再用穿透判断是工作日还是周末
    cout << endl;
    cout << "这天是: ";
    switch (day)
    {
        case 1: case 2: case 3: case 4: case 5:
            cout << "工作日" << endl;
            break;
        case 6: case 7:
            cout << "周末" << endl;
            break;
        default:
            cout << "无效日期" << endl;
            break;
    }

    return 0;
}
```

### 案例3：成绩等级转换（利用穿透特性）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 将百分制成绩转换为五级制
    int score;

    cout << "请输入成绩（0-100）: ";
    cin >> score;

    // 先检查成绩是否合法
    if (score < 0 || score > 100)
    {
        cout << "成绩输入有误！" << endl;
        return 1;  // 提前结束程序
    }

    // 利用 switch 和整除来分段
    // 90-100 → 9  → 优秀
    // 80-89  → 8  → 良好
    // 70-79  → 7  → 中等
    // 60-69  → 6  → 及格
    // 0-59   → 0~5 → 不及格
    switch (score / 10)
    {
        case 10:  // 100 分
        case 9:   // 90-99 分，利用穿透
            cout << "成绩: " << score << " → 等级: 优秀（A）" << endl;
            break;
        case 8:
            cout << "成绩: " << score << " → 等级: 良好（B）" << endl;
            break;
        case 7:
            cout << "成绩: " << score << " → 等级: 中等（C）" << endl;
            break;
        case 6:
            cout << "成绩: " << score << " → 等级: 及格（D）" << endl;
            break;
        default:  // 0-5
            cout << "成绩: " << score << " → 等级: 不及格（E）" << endl;
            break;
    }

    return 0;
}
```

---

## 十、switch 的高级用法（了解）

### 10.1 switch 中声明变量——用大括号限制作用域

```cpp
#include <iostream>
using namespace std;

int main()
{
    int x = 2;

    // 更安全的做法：用大括号限制变量作用域
    switch (x)
    {
        case 1:
        {
            int b = 10;     // 变量b只在这个大括号内有效
            cout << "b = " << b << endl;
            break;
        }
        case 2:
        {
            int c = 20;     // 变量c只在这个大括号内有效
            cout << "c = " << c << endl;
            break;
        }
    }

    return 0;
}
```

### 10.2 嵌套 switch

```cpp
#include <iostream>
using namespace std;

int main()
{
    char department = 'A';  // 部门
    int level = 2;          // 级别

    switch (department)
    {
        case 'A':
            cout << "技术部 - ";
            switch (level)      // 嵌套 switch
            {
                case 1:
                    cout << "初级工程师" << endl;
                    break;
                case 2:
                    cout << "中级工程师" << endl;
                    break;
                case 3:
                    cout << "高级工程师" << endl;
                    break;
                default:
                    cout << "未知级别" << endl;
                    break;
            }
            break;
        case 'B':
            cout << "市场部 - ";
            switch (level)
            {
                case 1:
                    cout << "初级专员" << endl;
                    break;
                case 2:
                    cout << "中级专员" << endl;
                    break;
                default:
                    cout << "未知级别" << endl;
                    break;
            }
            break;
        default:
            cout << "未知部门" << endl;
            break;
    }

    return 0;
}
```

---

## 十一、总结

### switch 使用口诀

```
switch 括号放变量，case 后面跟常量；
每个 case 加 break，忘记就会往下泄；
default 是兜底用，所有不中它来接；
只能判断相不等，大于小于用 if 别；
整型字符可以用，浮点字符串不行。
```

### switch vs if 对比表

| 特性 | switch | if...else if...else |
|------|--------|---------------------|
| 判断方式 | 只能判断相等 | 可以判断任意条件 |
| 表达式类型 | 整型、字符型 | 任意类型 |
| 范围判断 | 不支持 | 支持 |
| 多值匹配 | 利用穿透很方便 | 需要多个条件 |
| 执行效率 | 值较多时效率高 | 条件复杂时更灵活 |
| 可读性 | 值匹配时更清晰 | 条件复杂时更清晰 |

### 选择建议

- 当判断条件是**几个固定值**时，优先用 `switch`（更清晰）
- 当判断条件涉及**范围、不等式**时，必须用 `if`
- 当只有 **2~3 个简单条件**时，用 `if` 更简洁
- 当有 **5 个以上固定值**时，用 `switch` 可读性更好
- 多个 `case` 执行同一段代码时，可以利用**穿透特性**
