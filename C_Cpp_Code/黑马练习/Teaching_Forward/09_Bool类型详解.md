# 09 Bool 类型详解

## 一、bool 类型基本概念

- `bool`（布尔类型）是 C++ 中用来表示 **真/假** 的数据类型
- 只有两个值：`true`（真）和 `false`（假）
- 在内存中占 **1 个字节**
- 常用于条件判断和逻辑运算

```cpp
#include <iostream>
using namespace std;

int main()
{
    bool isPass = true;     // 及格了
    bool isFail = false;    // 没挂科

    cout << "是否及格: " << isPass << endl;  // 输出: 1
    cout << "是否挂科: " << isFail << endl;  // 输出: 0

    // 查看 bool 类型的大小
    cout << "bool 大小: " << sizeof(bool) << " 字节" << endl;  // 1

    return 0;
}
```

---

## 二、true 和 false 的本质

### 2.1 底层存储

```cpp
// 在 C++ 中：
// true  在内存中存储的是 1
// false 在内存中存储的是 0

// 所以 bool 类型本质上就是一个只能存 0 或 1 的整数

bool a = true;
bool b = false;

cout << "true  = " << a << endl;  // 输出: 1
cout << "false = " << b << endl;  // 输出: 0
```

### 2.2 为什么默认输出 0 和 1

```cpp
// cout 默认把 bool 当整数输出（0 或 1）
bool flag = true;
cout << flag << endl;  // 输出: 1

// 如果想输出 "true" 和 "false"，用 boolalpha
cout << boolalpha << flag << endl;  // 输出: true
cout << boolalpha << false << endl; // 输出: false

// noboolalpha 恢复默认输出
cout << noboolalpha << true << endl; // 输出: 1
```

---

## 三、boolalpha 输出详解

```cpp
#include <iostream>
using namespace std;

int main()
{
    bool isStudent = true;
    bool isTeacher = false;

    // 默认输出 0/1
    cout << "默认输出:" << endl;
    cout << "  是否学生: " << isStudent << endl;   // 1
    cout << "  是否老师: " << isTeacher << endl;   // 0

    // 使用 boolalpha 输出 true/false
    cout << "\nboolalpha 输出:" << endl;
    cout << boolalpha;
    cout << "  是否学生: " << isStudent << endl;   // true
    cout << "  是否老师: " << isTeacher << endl;   // false

    // 恢复默认
    cout << noboolalpha;
    cout << "\n恢复默认: " << isStudent << endl;   // 1

    return 0;
}
```

---

## 四、bool 的本质：非 0 为 true

### 4.1 核心规则

```cpp
// C++ 的规则：0 为 false，非 0 为 true（包括负数！）

bool b1 = 0;      // false
bool b2 = 1;      // true
bool b3 = -1;     // true（非0就是true）
bool b4 = 100;    // true
bool b5 = 0.0;    // false
bool b6 = 3.14;   // true
bool b7 = 'A';    // true（'A' 的 ASCII 码是 65，非0）
bool b8 = '\0';   // false（'\0' 的 ASCII 码是 0）

cout << boolalpha;
cout << "b1(0)   = " << b1 << endl;   // false
cout << "b2(1)   = " << b2 << endl;   // true
cout << "b3(-1)  = " << b3 << endl;   // true
cout << "b4(100) = " << b4 << endl;   // true
cout << "b5(0.0) = " << b5 << endl;   // false
cout << "b6(3.14)= " << b6 << endl;   // true
cout << "b7('A') = " << b7 << endl;   // true
cout << "b8('\\0')= " << b8 << endl;  // false
```

### 4.2 重要特性：bool 只存 0 或 1

```cpp
// 虽然非 0 值都算 true，但 bool 变量内部只会存 0 或 1
bool b = 100;
cout << b << endl;  // 输出: 1（不是 100！）

bool b2 = -999;
cout << b2 << endl; // 输出: 1（不是 -999！）

// 原理：任何非 0 值赋给 bool，都会被转换为 1
```

---

## 五、逻辑运算的结果

### 5.1 比较运算符返回 bool

```cpp
int a = 10, b = 20;

// 比较运算符的结果是 bool 类型
bool result1 = (a > b);   // false
bool result2 = (a < b);   // true
bool result3 = (a == 10); // true
bool result4 = (a != 10); // false

cout << boolalpha;
cout << "10 > 20  : " << result1 << endl;  // false
cout << "10 < 20  : " << result2 << endl;  // true
cout << "10 == 10 : " << result3 << endl;  // true
cout << "10 != 10 : " << result4 << endl;  // false
```

### 5.2 逻辑运算符

```cpp
bool a = true;
bool b = false;

// &&（逻辑与）：两个都为 true 才是 true
cout << "true  && true  = " << (true && true) << endl;   // 1
cout << "true  && false = " << (true && false) << endl;  // 0
cout << "false && false = " << (false && false) << endl; // 0

// ||（逻辑或）：有一个为 true 就是 true
cout << "true  || true  = " << (true || true) << endl;   // 1
cout << "true  || false = " << (true || false) << endl;  // 1
cout << "false || false = " << (false || false) << endl; // 0

// !（逻辑非）：取反
cout << "!true  = " << (!true) << endl;   // 0
cout << "!false = " << (!false) << endl;  // 1
```

### 5.3 实际应用

```cpp
int age = 25;
int score = 85;

bool isAdult = (age >= 18);           // true
bool isPass = (score >= 60);          // true
bool isGoodStudent = isAdult && isPass;  // true（既是成年人又及格了）

cout << boolalpha;
cout << "是否成年: " << isAdult << endl;        // true
cout << "是否及格: " << isPass << endl;         // true
cout << "是否好学生: " << isGoodStudent << endl; // true
```

---

## 六、bool 与整型的隐式转换

### 6.1 bool -> int（自动转换）

```cpp
bool flag = true;
int num = flag;     // true -> 1
cout << num << endl; // 输出: 1

bool flag2 = false;
int num2 = flag2;    // false -> 0
cout << num2 << endl; // 输出: 0
```

### 6.2 int -> bool（自动转换）

```cpp
int a = 10;
bool b = a;         // 10 -> true（非0为true）
cout << b << endl;   // 输出: 1

int c = 0;
bool d = c;          // 0 -> false
cout << d << endl;   // 输出: 0
```

### 6.3 在条件判断中

```cpp
int num = 42;

// if 语句会自动把表达式结果当作 bool
if (num)  // num 非 0，相当于 if (true)
{
    cout << "num 非零" << endl;
}

if (!num)  // !42 = !true = false
{
    cout << "num 为零" << endl;  // 不会执行
}

// 常见的判断写法
if (num != 0)  // 推荐写法，更清晰
{
    cout << "num 非零" << endl;
}
```

### 6.4 bool 参与算术运算

```cpp
bool a = true;   // 1
bool b = false;  // 0

// bool 可以参与算术运算（先转为 0 或 1）
int sum = a + b;     // 1 + 0 = 1
int product = a * 2; // 1 * 2 = 2

cout << "sum = " << sum << endl;         // 1
cout << "product = " << product << endl; // 2

// 注意：结果不是 bool 类型
bool result = a + b;  // 1 + 0 = 1，非0为true
cout << result << endl; // 1
```

---

## 七、常见用法场景

### 7.1 函数返回值

```cpp
// 函数返回 bool，表示某个条件是否成立
bool isEven(int num)
{
    return (num % 2 == 0);  // 能被2整除就是偶数
}

bool isPositive(int num)
{
    return (num > 0);
}

int main()
{
    int x = 10;
    cout << boolalpha;
    cout << x << " 是偶数? " << isEven(x) << endl;      // true
    cout << x << " 是正数? " << isPositive(x) << endl;  // true
    cout << -5 << " 是偶数? " << isEven(-5) << endl;    // false
    return 0;
}
```

### 7.2 作为标记变量

```cpp
// 用 bool 做开关/标志
bool isLoggedIn = false;   // 是否已登录
bool hasPermission = false; // 是否有权限

// 模拟登录
isLoggedIn = true;
hasPermission = true;

if (isLoggedIn && hasPermission)
{
    cout << "欢迎进入系统！" << endl;
}
else if (!isLoggedIn)
{
    cout << "请先登录！" << endl;
}
else
{
    cout << "没有权限！" << endl;
}
```

### 7.3 用于循环控制

```cpp
bool running = true;
int count = 0;

while (running)
{
    count++;
    cout << "第 " << count << " 次循环" << endl;

    if (count >= 5)
    {
        running = false;  // 满足条件就停止
    }
}
cout << "循环结束" << endl;
```

---

## 八、常见错误和注意事项

### 8.1 不要写成 0/1 或 true/false 混用

```cpp
// 虽然都能用，但风格要统一
bool flag1 = true;     // 推荐
bool flag2 = false;    // 推荐

bool flag3 = 1;        // 能用但不推荐
bool flag4 = 0;        // 能用但不推荐
```

### 8.2 不要把 == true 写在条件里

```cpp
bool flag = true;

// 不推荐的写法
if (flag == true)
{
    // ...
}

// 推荐的写法
if (flag)
{
    // ...
}

// 不推荐
if (flag == false)
{
    // ...
}

// 推荐
if (!flag)
{
    // ...
}
```

### 8.3 注意隐式转换的陷阱

```cpp
int a = 5;
int b = 3;

// 注意运算优先级！
bool result = a > 3 && b < 10;  // (a > 3) && (b < 10) = true && true = true
cout << result << endl;  // 1

// 加括号更清晰
bool result2 = (a > 3) && (b < 10);
```

---

## 九、综合示例

```cpp
#include <iostream>
using namespace std;

int main()
{
    cout << "===== Bool 类型综合示例 =====" << endl;

    // 1. 基本定义
    bool isSunny = true;
    bool isRainy = false;

    cout << boolalpha;
    cout << "\n天气情况:" << endl;
    cout << "  晴天: " << isSunny << endl;
    cout << "  下雨: " << isRainy << endl;

    // 2. 比较运算
    int score = 85;
    bool isPass = (score >= 60);
    bool isExcellent = (score >= 90);
    bool isGood = (score >= 80) && (score < 90);

    cout << "\n成绩 " << score << " 分:" << endl;
    cout << "  及格: " << isPass << endl;       // true
    cout << "  优秀: " << isExcellent << endl;  // false
    cout << "  良好: " << isGood << endl;       // true

    // 3. 逻辑运算
    int age = 20;
    bool hasID = true;
    bool canVote = (age >= 18) && hasID;

    cout << "\n投票资格:" << endl;
    cout << "  年龄: " << age << endl;
    cout << "  有身份证: " << hasID << endl;
    cout << "  可以投票: " << canVote << endl;  // true

    // 4. bool 参与运算
    cout << "\nbool 参与运算:" << endl;
    cout << "  true + true = " << (true + true) << endl;   // 2
    cout << "  true * false = " << (true * false) << endl; // 0
    cout << "  true + 10 = " << (true + 10) << endl;      // 11

    // 5. 非0为true
    cout << "\n非0为true:" << endl;
    cout << "  bool(0) = " << boolalpha << (bool)0 << endl;      // false
    cout << "  bool(1) = " << (bool)1 << endl;                    // true
    cout << "  bool(-1) = " << (bool)(-1) << endl;                // true
    cout << "  bool(100) = " << (bool)100 << endl;                // true

    return 0;
}
```

---

## 十、总结

| 要点 | 说明 |
|------|------|
| 值 | 只有 `true`(1) 和 `false`(0) |
| 大小 | 1 字节 |
| 输出 | 默认输出 0/1，`boolalpha` 输出 true/false |
| 本质 | 0 为 false，非 0 为 true（但 bool 只存 0 或 1） |
| 转换 | bool 和 int 可以隐式转换 |
| 用途 | 条件判断、逻辑运算、标记变量 |
