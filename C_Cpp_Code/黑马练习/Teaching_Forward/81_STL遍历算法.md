# STL 遍历算法

## 一、概述

遍历算法用于对容器中的每一个元素执行某种操作。STL 中最常用的两个遍历算法是：

| 算法 | 作用 |
|------|------|
| `for_each` | 对容器中每个元素执行某个操作（就地修改或打印） |
| `transform` | 将容器中每个元素经过函数处理后，输出到另一个容器 |

**头文件：** `#include <algorithm>`

---

## 二、for_each 算法

### 2.1 基本语法

```cpp
for_each(iterator_begin, iterator_end, 函数/仿函数/Lambda);
```

- `iterator_begin`：容器的起始迭代器
- `iterator_end`：容器的结束迭代器（不包含该位置）
- 第三个参数：可以是普通函数、仿函数（函数对象）、或 Lambda 表达式

**功能：** 对 `[begin, end)` 范围内的每一个元素，调用第三个参数所指定的函数。

---

### 2.2 用普通函数作为回调

```cpp
#include <iostream>
#include <vector>
#include <algorithm>  // for_each 在这个头文件中
using namespace std;

// 普通函数：打印一个整数
void printInt(int val)
{
    cout << val << " ";
}

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // for_each 会对 v 中的每一个元素调用 printInt 函数
    // v.begin() 指向第一个元素
    // v.end()   指向最后一个元素的下一个位置（不包含）
    for_each(v.begin(), v.end(), printInt);
    // 输出：1 2 3 4 5

    cout << endl;
    return 0;
}
```

**执行流程：**
1. 取出 v 的第一个元素 `1`，调用 `printInt(1)`，打印 `1`
2. 取出 v 的第二个元素 `2`，调用 `printInt(2)`，打印 `2`
3. ... 依次处理所有元素
4. 当迭代器到达 `v.end()` 时停止

---

### 2.3 用仿函数（函数对象）作为回调

仿函数就是一个重载了 `()` 运算符的类，可以像函数一样使用。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 仿函数（函数对象）
class MyPrint
{
public:
    // 重载 () 运算符，使得对象可以像函数一样被调用
    void operator()(int val)
    {
        cout << val << " ";
    }
};

int main()
{
    vector<int> v = {10, 20, 30, 40, 50};

    // 创建仿函数对象，传给 for_each
    for_each(v.begin(), v.end(), MyPrint());
    // 输出：10 20 30 40 50

    cout << endl;
    return 0;
}
```

**仿函数的优势 —— 可以有状态（成员变量）：**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 带状态的仿函数：每次调用时计数
class MyPrintWithCount
{
public:
    int count;  // 记录调用次数

    MyPrintWithCount() : count(0) {}  // 构造函数，初始化计数为0

    void operator()(int val)
    {
        cout << "第" << ++count << "个元素: " << val << endl;
    }
};

int main()
{
    vector<int> v = {100, 200, 300};

    // 注意：for_each 的第三个参数是值传递，所以返回的是仿函数的副本
    // 要获取最终状态，需要用变量接收 for_each 的返回值
    MyPrintWithCount result = for_each(v.begin(), v.end(), MyPrintWithCount());

    cout << "总共处理了 " << result.count << " 个元素" << endl;
    // 输出：
    // 第1个元素: 100
    // 第2个元素: 200
    // 第3个元素: 300
    // 总共处理了 3 个元素

    return 0;
}
```

---

### 2.4 用 Lambda 表达式作为回调（C++11）

Lambda 表达式是一种匿名函数，可以在调用 `for_each` 时直接定义操作。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // Lambda 表达式语法：[捕获列表](参数列表) { 函数体 }
    // [] 不捕获外部变量
    // (int val) 接收一个整数参数
    // { } 函数体，执行打印操作
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 2 3 4 5

    cout << endl;

    // Lambda 也可以捕获外部变量
    int sum = 0;
    for_each(v.begin(), v.end(), [&sum](int val) {
        sum += val;  // [&sum] 表示以引用方式捕获 sum，可以修改它
    });
    cout << "累加和: " << sum << endl;  // 输出：累加和: 15

    return 0;
}
```

**Lambda 表达式说明：**

```
[捕获列表](参数列表) -> 返回类型 { 函数体 }
```

| 部分 | 说明 | 示例 |
|------|------|------|
| `[]` | 捕获列表，指定如何使用外部变量 | `[]` 不捕获, `[&]` 引用捕获全部, `[=]` 值捕获全部 |
| `(参数)` | 参数列表，和普通函数参数一样 | `(int val)` |
| `-> 返回类型` | 可选，通常可省略让编译器自动推导 | `-> void` |
| `{ }` | 函数体 | `{ cout << val; }` |

---

### 2.5 for_each 修改容器元素

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 用 Lambda 加上引用 & 来修改元素
    // 注意：参数要写成 int& 类型（引用），否则修改的是副本
    for_each(v.begin(), v.end(), [](int& val) {
        val *= 2;  // 每个元素乘以2
    });

    // 验证修改结果
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：2 4 6 8 10

    cout << endl;
    return 0;
}
```

**常见错误：忘记加引用**

```cpp
// 错误写法！不会修改原容器
for_each(v.begin(), v.end(), [](int val) {  // val 是副本，修改不影响原值
    val *= 2;
});

// 正确写法！使用引用
for_each(v.begin(), v.end(), [](int& val) {  // val 是引用，修改会反映到原值
    val *= 2;
});
```

---

## 三、transform 算法

### 3.1 基本语法

```cpp
transform(src_begin, src_end, dst_begin, 函数);
```

- `src_begin, src_end`：源容器的起始和结束迭代器
- `dst_begin`：目标容器的起始迭代器
- `函数`：对每个元素执行的变换操作

**功能：** 将源容器中的每个元素经过函数处理后，存放到目标容器中。

---

### 3.2 基本用法 —— 每个元素加10

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 变换函数：将值加10
int addTen(int val)
{
    return val + 10;
}

int main()
{
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst;

    // 【重要】目标容器必须提前开辟足够空间！
    // resize 不仅设置大小，还会初始化元素
    dst.resize(src.size());

    // transform 将 src 中每个元素经过 addTen 处理后存入 dst
    transform(src.begin(), src.end(), dst.begin(), addTen);

    // 打印目标容器
    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：11 12 13 14 15

    cout << endl;
    return 0;
}
```

**常见错误：忘记给目标容器分配空间**

```cpp
vector<int> dst;  // 空容器，size = 0
// 错误！dst 没有空间，写入会导致未定义行为（崩溃）
transform(src.begin(), src.end(), dst.begin(), addTen);

// 正确做法：先分配空间
dst.resize(src.size());  // 或者 dst.resize(5);
transform(src.begin(), src.end(), dst.begin(), addTen);
```

---

### 3.3 用 Lambda 简化 transform

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst;
    dst.resize(src.size());

    // 用 Lambda 直接定义变换规则：每个元素乘以3
    transform(src.begin(), src.end(), dst.begin(), [](int val) {
        return val * 3;
    });

    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：3 6 9 12 15

    cout << endl;
    return 0;
}
```

---

### 3.4 transform 原地变换（源和目标是同一个容器）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 源和目标可以是同一个容器
    transform(v.begin(), v.end(), v.begin(), [](int val) {
        return val + 100;
    });

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：101 102 103 104 105

    cout << endl;
    return 0;
}
```

---

### 3.5 transform 处理自定义类型

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Student
{
public:
    string name;
    int score;

    Student(string n, int s) : name(n), score(s) {}
};

int main()
{
    vector<Student> students = {
        Student("张三", 80),
        Student("李四", 90),
        Student("王五", 70)
    };

    vector<int> scores;
    scores.resize(students.size());

    // 将每个学生的成绩提取出来存入 scores 容器
    transform(students.begin(), students.end(), scores.begin(), [](const Student& s) {
        return s.score;
    });

    for_each(scores.begin(), scores.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：80 90 70

    cout << endl;
    return 0;
}
```

---

## 四、for_each 与 transform 的区别

| 比较项 | for_each | transform |
|--------|----------|-----------|
| **作用** | 对每个元素执行操作（通常是就地修改或打印） | 将元素变换后输出到另一个容器 |
| **返回值** | 返回函数对象（仿函数）的副本 | 返回目标容器的结束迭代器 |
| **是否产生新容器** | 不产生，就地操作 | 可以输出到新容器 |
| **函数要求** | 函数可以没有返回值 | 函数必须有返回值 |
| **典型用途** | 打印、就地修改 | 数据变换、类型转换 |

**简单记忆：**
- `for_each` = "对每个元素做点什么"（关注过程）
- `transform` = "把元素变换成新的"（关注结果）

---

## 五、综合示例

### 示例1：打印 vector 中所有元素

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // 方法1：普通函数
    cout << "方法1（普通函数）: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;

    // 方法2：手动遍历（对比理解）
    cout << "方法2（手动遍历）: ";
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;

    // 方法3：范围for（C++11）
    cout << "方法3（范围for）: ";
    for (int val : v)
    {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

### 示例2：将字符串容器中所有字符串转为大写

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    vector<string> src = {"hello", "world", "cpp"};
    vector<string> dst;
    dst.resize(src.size());

    // transform 将每个字符串转为大写
    transform(src.begin(), src.end(), dst.begin(), [](string s) {
        // 对每个字符调用 toupper
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    });

    for_each(dst.begin(), dst.end(), [](const string& s) {
        cout << s << " ";
    });
    // 输出：HELLO WORLD CPP

    cout << endl;
    return 0;
}
```

---

## 六、注意事项总结

1. **for_each 的参数是值传递的**，如果想修改原容器，函数参数要使用引用 `int&`
2. **transform 的目标容器必须提前分配空间**，用 `resize()` 而不是 `reserve()`
   - `resize()` 改变容器大小并初始化元素
   - `reserve()` 只分配内存，不改变大小
3. **transform 的函数必须有返回值**，for_each 的函数可以没有返回值
4. **Lambda 表达式中 `[&]` 表示引用捕获所有外部变量，`[=]` 表示值捕获所有外部变量**
5. **遍历算法需要包含头文件：`#include <algorithm>`**
