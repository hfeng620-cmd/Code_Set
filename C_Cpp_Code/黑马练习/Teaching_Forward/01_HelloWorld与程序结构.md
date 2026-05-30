# 01 HelloWorld 与程序结构详解

## 一、最简单的 C++ 程序

```cpp
#include <iostream>
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
```

> 这是每一个 C++ 学习者写的第一个程序，下面我们逐行拆解。

---

## 二、#include 预处理指令

### 2.1 什么是预处理

- 以 `#` 开头的行叫做 **预处理指令**
- 它在 **编译之前** 就会被处理（所以叫"预"处理）
- `#include` 的作用是：把指定文件的 **全部内容复制粘贴** 到当前位置

### 2.2 尖括号 <> 和双引号 "" 的区别

```cpp
// 尖括号：去【系统目录】（编译器自带的头文件目录）中查找
#include <iostream>    // 系统提供的头文件用 <>

// 双引号：先在【当前项目目录】中找，找不到再去系统目录中找
#include "myheader.h"  // 自己写的头文件用 ""
```

| 写法 | 查找顺序 | 典型用途 |
|------|----------|----------|
| `<xxx>` | 系统目录 | 标准库头文件，如 `<iostream>` `<string>` |
| `"xxx"` | 当前目录 -> 系统目录 | 自定义头文件，如 `"myheader.h"` |

### 2.3 常见头文件

```cpp
#include <iostream>   // 输入输出（cout、cin）
#include <string>     // 字符串类 string
#include <cmath>      // 数学函数（sqrt、pow 等）
#include <cstdio>     // C 风格输入输出（printf、scanf）
```

---

## 三、using namespace std;（命名空间）

### 3.1 为什么需要命名空间

假设两个公司都写了一个叫 `print` 的函数，编译器就不知道该用谁的了。**命名空间** 就是给标识符加一个"姓氏"来区分。

```cpp
// 公司 A 的命名空间
namespace CompanyA
{
    void print() { cout << "A 的 print" << endl; }
}

// 公司 B 的命名空间
namespace CompanyB
{
    void print() { cout << "B 的 print" << endl; }
}

// 使用时通过 命名空间::函数名 来区分
CompanyA::print();  // 调用 A 的
CompanyB::print();  // 调用 B 的
```

### 3.2 std 是什么

- `std` 是 C++ **标准库** 使用的命名空间
- `cout`、`endl`、`string` 等标准库中的东西都在 `std` 里面
- `using namespace std;` 的意思是：**展开** std 命名空间，以后写 `cout` 就不用写 `std::cout` 了

```cpp
// 不写 using namespace std; 的话，每次都要加 std::
std::cout << "Hello" << std::endl;

// 写了 using namespace std; 之后，可以省略 std::
cout << "Hello" << endl;
```

### 3.3 更推荐的写法（按需引入）

```cpp
// 不要一口气展开整个命名空间，按需引入更安全
using std::cout;
using std::endl;
using std::string;
```

---

## 四、main 函数（程序入口）

### 4.1 main 是程序的入口

- 每个 C++ 程序 **必须有且只有一个** `main` 函数
- 程序从 `main` 函数的 **第一行** 开始执行
- `main` 函数执行结束 = 程序结束

### 4.2 main 的标准写法

```cpp
// 标准写法一：无参数
int main()
{
    // 程序代码
    return 0;
}

// 标准写法二：有参数（命令行参数，了解即可）
int main(int argc, char* argv[])
{
    return 0;
}
```

### 4.3 返回值含义

- `return 0;` 表示程序 **正常结束**
- `return 非0值;` 表示程序 **异常结束**（不同的非0值可以表示不同的错误类型）
- 操作系统可以接收到这个返回值，判断程序是否运行成功

```cpp
int main()
{
    // 如果一切正常
    return 0;

    // 如果出现错误
    // return 1;  // 表示出错了
}
```

### 4.4 常见错误

```cpp
// 错误1：没有 main 函数 -> 编译报错（程序不知道从哪里开始）
// 错误2：有两个 main 函数 -> 链接报错（不知道该执行哪个）
// 错误3：main 写成了 Main -> 不认，C++ 区分大小写
```

---

## 五、cout 输出详解

### 5.1 cout 是什么

- `cout` 是 **标准输出流对象**（Console OUTput，控制台输出）
- `<<` 是 **输出运算符**（也叫插入运算符）
- `endl` 是 **换行并刷新缓冲区**

### 5.2 基本输出

```cpp
// 输出字符串
cout << "Hello World!" << endl;

// 输出数字
cout << 100 << endl;

// 输出变量
int age = 18;
cout << age << endl;
```

### 5.3 链式输出（连续使用 <<）

```cpp
// 可以连续使用 << 输出多个内容
cout << "姓名: " << "张三" << ", 年龄: " << 18 << endl;
// 输出: 姓名: 张三, 年龄: 18

int a = 10, b = 20;
cout << "a = " << a << ", b = " << b << endl;
// 输出: a = 10, b = 20
```

### 5.4 endl 和 "\n" 的区别

```cpp
// 方式一：endl（end line）
cout << "第一行" << endl;
cout << "第二行" << endl;

// 方式二：\n（转义字符，换行）
cout << "第一行\n";
cout << "第二行\n";

// 方式三：直接拼接
cout << "第一行\n第二行\n";
```

| 区别 | endl | \n |
|------|------|-----|
| 效果 | 换行 + 刷新缓冲区 | 只换行 |
| 速度 | 稍慢（频繁刷新） | 更快 |
| 推荐 | 需要立即显示时使用 | 日常使用推荐 |

---

## 六、return 0 详解

```cpp
int main()
{
    // ... 程序代码 ...
    return 0;  // 返回给操作系统，0 = 正常
}
```

- 在 `main` 函数中，`return 0;` 可以省略（C++11 起，编译器会自动加）
- 但 **强烈建议写上**，养成好习惯
- 在其他函数中，`return` 用于返回值给调用者

---

## 七、C++ 编译过程

```
源代码(.cpp)  -->  预处理  -->  编译  -->  汇编  -->  链接  -->  可执行文件(.exe)
```

### 7.1 四个阶段

| 阶段 | 做什么 | 产物 |
|------|--------|------|
| 预处理 | 处理 `#include`、`#define` 等 | 展开后的源代码 |
| 编译 | 将 C++ 代码翻译成汇编代码 | 汇编文件(.s) |
| 汇编 | 将汇编代码翻译成机器码 | 目标文件(.obj/.o) |
| 链接 | 将多个目标文件和库文件合并 | 可执行文件(.exe) |

### 7.2 在命令行手动编译（以 g++ 为例）

```bash
# 一步到位：源文件 -> 可执行文件
g++ hello.cpp -o hello.exe

# 分步编译（理解原理）
g++ -E hello.cpp -o hello.i     # 预处理
g++ -S hello.i -o hello.s       # 编译
g++ -c hello.s -o hello.o       # 汇编
g++ hello.o -o hello.exe        # 链接
```

---

## 八、完整程序结构总结

```cpp
// ========== 第一部分：头文件包含 ==========
#include <iostream>   // 引入输入输出功能
#include <string>     // 引入字符串功能（如果需要的话）

// ========== 第二部分：命名空间 ==========
using namespace std;  // 使用标准命名空间（初学阶段先这样写）

// ========== 第三部分：主函数 ==========
int main()
{
    // ========== 第四部分：程序逻辑 ==========
    cout << "=== 我的第一个 C++ 程序 ===" << endl;
    cout << "Hello World!" << endl;
    cout << "我正在学习 C++！" << endl;

    // ========== 第五部分：返回值 ==========
    return 0;  // 程序正常结束
}
```

---

## 九、常见错误速查

| 错误现象 | 原因 |
|----------|------|
| `error: iostream: No such file` | 头文件名写错 |
| `error: 'cout' was not declared` | 没写 `using namespace std;` 或 `std::` |
| `error: expected ';' before '}'` | 某行末尾少了分号 `;` |
| `error: multiple definition of 'main'` | 有两个 main 函数 |
| 中文乱码 | 源文件编码问题，保存为 UTF-8 |
