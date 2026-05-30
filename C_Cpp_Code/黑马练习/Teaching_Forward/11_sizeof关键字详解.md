# 11 sizeof 关键字详解

## 一、sizeof 是什么

- `sizeof` 是 C++ 的一个 **运算符**（不是函数！），用来计算变量或类型所占的 **字节数**
- 语法：`sizeof(类型)` 或 `sizeof(变量)` 或 `sizeof 变量`
- 返回值类型是 `size_t`（无符号整数）

```cpp
#include <iostream>
using namespace std;

int main()
{
    // 用法1：sizeof(类型)
    cout << "int 大小: " << sizeof(int) << " 字节" << endl;

    // 用法2：sizeof(变量)
    int a = 10;
    cout << "变量 a 大小: " << sizeof(a) << " 字节" << endl;

    // 用法3：sizeof 变量（可以省略括号，但不推荐）
    cout << "变量 a 大小: " << sizeof a << " 字节" << endl;

    return 0;
}
```

> **注意：** `sizeof` 是运算符，不是函数。它在 **编译时** 就已经计算好了结果，运行时只是输出一个常量。

---

## 二、所有基本类型的大小

```cpp
#include <iostream>
using namespace std;

int main()
{
    cout << "===== 所有基本类型大小 =====" << endl;

    // 整型
    cout << "bool:          " << sizeof(bool)          << " 字节" << endl;  // 1
    cout << "char:          " << sizeof(char)          << " 字节" << endl;  // 1
    cout << "short:         " << sizeof(short)         << " 字节" << endl;  // 2
    cout << "int:           " << sizeof(int)           << " 字节" << endl;  // 4
    cout << "long:          " << sizeof(long)          << " 字节" << endl;  // 4（Windows）
    cout << "long long:     " << sizeof(long long)     << " 字节" << endl;  // 8

    // 浮点型
    cout << "float:         " << sizeof(float)         << " 字节" << endl;  // 4
    cout << "double:        " << sizeof(double)        << " 字节" << endl;  // 8
    cout << "long double:   " << sizeof(long double)   << " 字节" << endl;  // 8 或 16

    // 无符号版本（大小和有符号版本相同）
    cout << "unsigned int:  " << sizeof(unsigned int)  << " 字节" << endl;  // 4
    cout << "unsigned char: " << sizeof(unsigned char) << " 字节" << endl;  // 1

    return 0;
}
```

### 常见平台的类型大小参考

| 类型 | 32位/64位 Windows (MSVC) | 说明 |
|------|--------------------------|------|
| `bool` | 1 字节 | 真/假 |
| `char` | 1 字节 | 单个字符 |
| `short` | 2 字节 | 短整型 |
| `int` | 4 字节 | 整型（最常用） |
| `long` | 4 字节 | 长整型（Windows） |
| `long long` | 8 字节 | 长长整型 |
| `float` | 4 字节 | 单精度浮点 |
| `double` | 8 字节 | 双精度浮点 |
| `指针` | 4 或 8 字节 | 取决于 32 位或 64 位 |

---

## 三、sizeof 与数组

### 3.1 计算整个数组的大小

```cpp
int arr[] = {1, 2, 3, 4, 5};

// sizeof(arr) 计算的是整个数组占用的字节数
cout << "数组总大小: " << sizeof(arr) << " 字节" << endl;
// 5 个 int，每个 4 字节，总共 20 字节
```

### 3.2 计算数组元素个数（经典技巧）

```cpp
int arr[] = {1, 2, 3, 4, 5};

// 数组总字节数 / 单个元素字节数 = 元素个数
int length = sizeof(arr) / sizeof(arr[0]);
cout << "数组元素个数: " << length << endl;  // 5

// 也可以写成 sizeof(arr) / sizeof(int)
int length2 = sizeof(arr) / sizeof(int);
cout << "数组元素个数: " << length2 << endl;  // 5
```

### 3.3 各种类型的数组

```cpp
// int 数组
int intArr[] = {1, 2, 3};
cout << "int 数组: " << sizeof(intArr) << " 字节, "
     << sizeof(intArr) / sizeof(int) << " 个元素" << endl;
// 输出: int 数组: 12 字节, 3 个元素

// double 数组
double doubleArr[] = {1.1, 2.2, 3.3, 4.4};
cout << "double 数组: " << sizeof(doubleArr) << " 字节, "
     << sizeof(doubleArr) / sizeof(double) << " 个元素" << endl;
// 输出: double 数组: 32 字节, 4 个元素

// char 数组
char charArr[] = "Hello";
cout << "char 数组: " << sizeof(charArr) << " 字节, "
     << sizeof(charArr) / sizeof(char) << " 个元素" << endl;
// 输出: char 数组: 6 字节, 6 个元素（包含 '\0'）

// bool 数组
bool boolArr[] = {true, false, true};
cout << "bool 数组: " << sizeof(boolArr) << " 字节, "
     << sizeof(boolArr) / sizeof(bool) << " 个元素" << endl;
// 输出: bool 数组: 3 字节, 3 个元素
```

### 3.4 定义宏来简化

```cpp
// 定义一个宏，方便获取数组元素个数
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int arr[] = {10, 20, 30, 40, 50, 60};
int len = ARRAY_SIZE(arr);
cout << "数组长度: " << len << endl;  // 6

// 遍历数组
for (int i = 0; i < ARRAY_SIZE(arr); i++)
{
    cout << arr[i] << " ";
}
cout << endl;
// 输出: 10 20 30 40 50 60
```

---

## 四、sizeof 与指针

### 4.1 指针的大小

```cpp
// 重要：sizeof(指针) 的大小取决于【编译目标是 32 位还是 64 位】
// 和指针指向什么类型无关！

int* p1 = nullptr;
double* p2 = nullptr;
char* p3 = nullptr;

cout << "int* 大小: " << sizeof(p1) << " 字节" << endl;     // 4（32位）或 8（64位）
cout << "double* 大小: " << sizeof(p2) << " 字节" << endl;  // 4（32位）或 8（64位）
cout << "char* 大小: " << sizeof(p3) << " 字节" << endl;    // 4（32位）或 8（64位）

// 所有类型的指针大小都一样！因为指针存的是地址，地址的大小由系统决定
```

### 4.2 指针和数组的区别（重要！）

```cpp
void test(int arr[])
{
    // 在函数参数中，数组会退化为指针！
    // sizeof(arr) 得到的是指针的大小，不是数组的大小！
    cout << "函数内 sizeof(arr): " << sizeof(arr) << endl;  // 4 或 8（指针大小）
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    cout << "main 中 sizeof(arr): " << sizeof(arr) << endl;  // 20（整个数组的大小）

    // 注意：数组作为参数传递时会退化为指针！
    test(arr);  // 在函数内部无法用 sizeof 获取数组大小

    return 0;
}
```

> **重要教训：** 数组传入函数后就变成了指针，在函数内部 `sizeof` 只能得到指针的大小（4 或 8），而不是数组的大小。所以通常需要额外传一个参数表示数组长度。

---

## 五、sizeof 与字符串

```cpp
// C 风格字符串
char str1[] = "Hello";
cout << "sizeof(str1): " << sizeof(str1) << endl;  // 6（包含 '\0'）
cout << "strlen(str1): " << strlen(str1) << endl;   // 5（不包含 '\0'，需要 <cstring>）

char str2[20] = "Hi";
cout << "sizeof(str2): " << sizeof(str2) << endl;  // 20（整个数组的大小，不是字符串长度）
cout << "strlen(str2): " << strlen(str2) << endl;   // 2（实际字符串长度）

// C++ string 类
string s = "Hello";
cout << "sizeof(s): " << sizeof(s) << endl;    // 通常是 32 或 40（string 对象本身的大小）
cout << "s.size(): " << s.size() << endl;       // 5（字符串内容的长度）
cout << "s.length(): " << s.length() << endl;   // 5（和 size() 一样）
```

> **注意：** `sizeof(string)` 得到的是 string **对象本身** 在内存中占用的大小（包含内部指针、长度等管理信息），不是字符串内容的长度。要获取字符串内容长度，用 `.size()` 或 `.length()`。

---

## 六、sizeof 是运算符而非函数

```cpp
// 证据1：sizeof 可以省略括号（对变量而言）
int a = 10;
cout << sizeof a << endl;    // 正确！省略了括号
cout << sizeof(a) << endl;   // 也正确

// 注意：对类型名不能省略括号
// cout << sizeof int << endl;  // 错误！类型名必须加括号

// 证据2：sizeof 在编译时求值，不是运行时
int x = 10;
sizeof(x++);   // x++ 不会真正执行！因为 sizeof 在编译时就算好了
cout << x << endl;  // 10（不是 11！x++ 没有被执行）

// 证据3：sizeof 的结果是编译时常量
int arr[10];
// int arr2[sizeof(int)];  // 可以用 sizeof 来指定数组大小（因为它是常量）
```

---

## 七、常见用途

### 7.1 跨平台兼容性检查

```cpp
// 不同平台上类型的大小可能不同，用 sizeof 检查
cout << "当前平台上 int 的大小: " << sizeof(int) << " 字节" << endl;

// 编译时断言（确保类型大小符合预期）
static_assert(sizeof(int) == 4, "int 必须是 4 字节");
static_assert(sizeof(double) == 8, "double 必须是 8 字节");
```

### 7.2 计算数组长度

```cpp
int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int len = sizeof(arr) / sizeof(arr[0]);
cout << "数组长度: " << len << endl;  // 10
```

### 7.3 动态内存分配

```cpp
// 计算需要分配多少字节
int count = 100;
int* p = new int[count];  // 分配 100 个 int 的空间
cout << "分配了 " << count * sizeof(int) << " 字节" << endl;
delete[] p;
```

### 7.4 结构体/类的大小分析

```cpp
struct Student
{
    char name[20];  // 20 字节
    int age;         // 4 字节
    double score;    // 8 字节
};

cout << "Student 大小: " << sizeof(Student) << " 字节" << endl;
// 可能是 32 字节（不是简单的 20+4+8=32，因为有内存对齐）

struct Data
{
    char a;    // 1 字节
    // 3 字节 填充（对齐到 4 字节边界）
    int b;     // 4 字节
    char c;    // 1 字节
    // 3 字节 填充（整体大小是最大成员的倍数）
};

cout << "Data 大小: " << sizeof(Data) << " 字节" << endl;
// 实际是 12 字节，不是 6 字节（因为内存对齐）
```

---

## 八、综合示例

```cpp
#include <iostream>
#include <string>
using namespace std;

int main()
{
    cout << "===== sizeof 综合示例 =====" << endl;

    // 1. 基本类型大小
    cout << "\n--- 基本类型大小 ---" << endl;
    cout << "bool:      " << sizeof(bool)      << " 字节" << endl;
    cout << "char:      " << sizeof(char)      << " 字节" << endl;
    cout << "short:     " << sizeof(short)     << " 字节" << endl;
    cout << "int:       " << sizeof(int)       << " 字节" << endl;
    cout << "long long: " << sizeof(long long) << " 字节" << endl;
    cout << "float:     " << sizeof(float)     << " 字节" << endl;
    cout << "double:    " << sizeof(double)    << " 字节" << endl;

    // 2. 数组大小和元素个数
    cout << "\n--- 数组相关 ---" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    cout << "arr 总大小: " << sizeof(arr) << " 字节" << endl;
    cout << "单个元素: " << sizeof(arr[0]) << " 字节" << endl;
    cout << "元素个数: " << sizeof(arr) / sizeof(arr[0]) << endl;

    // 3. 字符串
    cout << "\n--- 字符串相关 ---" << endl;
    char cstr[] = "Hello";
    cout << "char[] sizeof: " << sizeof(cstr) << " 字节（含\\0）" << endl;

    string str = "Hello";
    cout << "string sizeof: " << sizeof(str) << " 字节（对象大小）" << endl;
    cout << "string size: " << str.size() << "（内容长度）" << endl;

    // 4. 指针
    cout << "\n--- 指针大小 ---" << endl;
    int* p1 = nullptr;
    double* p2 = nullptr;
    cout << "int*:    " << sizeof(p1) << " 字节" << endl;
    cout << "double*: " << sizeof(p2) << " 字节" << endl;

    // 5. 结构体（内存对齐）
    cout << "\n--- 结构体大小 ---" << endl;
    struct Simple
    {
        int a;      // 4
        double b;   // 8
        char c;     // 1
    };
    cout << "Simple: " << sizeof(Simple) << " 字节（可能因对齐而大于 13）" << endl;

    return 0;
}
```

---

## 九、总结

| 要点 | 说明 |
|------|------|
| 本质 | 编译时运算符，不是函数 |
| 语法 | `sizeof(类型)` 或 `sizeof 变量` |
| 数组 | `sizeof(arr)/sizeof(arr[0])` 可以求元素个数 |
| 指针 | 所有指针大小相同，32位系统4字节，64位系统8字节 |
| string | `sizeof(string)` 是对象大小，`.size()` 才是内容长度 |
| 数组传参 | 数组传入函数会退化为指针，sizeof 不再是数组大小 |
| 内存对齐 | 结构体大小可能大于成员大小之和 |
| 用途 | 跨平台检查、求元素个数、内存分配、结构体分析 |
