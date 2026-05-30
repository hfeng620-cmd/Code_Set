/*
 * 39b_函数分文件编写.cpp
 * 本文件演示 C++ 函数分文件编写的完整概念与实践
 * 包括: 头文件(.h)声明、源文件(.cpp)定义、#include 指令、
 *       头文件保护符(#ifndef/#define/#endif)、
 *       多文件项目的组织结构
 *
 * 实际项目中, 代码通常分为多个文件:
 *   - .h 头文件: 函数声明、类声明、宏定义、常量
 *   - .cpp 源文件: 函数实现、类成员实现
 *
 * 由于本文件是单文件演示, 将在注释中详细说明多文件结构
 */

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ============================================================
// 一、为什么要分文件编写?
// ============================================================
// 1. 代码组织: 不同功能放在不同文件中, 便于管理
// 2. 代码复用: 其他项目可以直接引入头文件使用
// 3. 编译效率: 修改一个文件只需重新编译该文件
// 4. 团队协作: 不同人可以同时开发不同文件
// 5. 隐藏实现: 只暴露声明, 隐藏具体实现细节

// ============================================================
// 二、头文件(.h)应该包含什么?
// ============================================================
// 1. 函数声明(函数原型)
// 2. 类的声明
// 3. 宏定义(#define)
// 4. 常量定义
// 5. 类型别名(typedef/using)
// 6. 头文件保护符
//
// 头文件不应该包含:
// 1. 函数的实现(除非是内联函数或模板)
// 2. 全局变量的定义(可以用 extern 声明)
// 3. using namespace (会污染包含它的所有文件)

// ============================================================
// 三、源文件(.cpp)应该包含什么?
// ============================================================
// 1. 对应头文件的 #include
// 2. 函数的实现
// 3. 全局变量的定义
// 4. main() 函数(通常单独一个文件)

// ============================================================
// 四、演示: 一个完整的多文件数学库
// ============================================================
// 假设我们要创建一个数学工具库, 按照分文件原则组织

// ---------- 文件结构示意 ----------
// 项目根目录/
// ├── main.cpp              // 主程序入口
// ├── math_utils.h          // 数学工具头文件(声明)
// ├── math_utils.cpp        // 数学工具实现文件(定义)
// ├── string_utils.h        // 字符串工具头文件
// ├── string_utils.cpp      // 字符串工具实现文件
// └── Makefile / CMakeLists.txt  // 构建文件

// ============================================================
// 五、头文件保护符(Include Guards)
// ============================================================
// 防止同一个头文件被多次包含导致的重复定义错误
//
// 格式:
//   #ifndef MYHEADER_H      // 如果没有定义过 MYHEADER_H
//   #define MYHEADER_H      // 就定义它
//   // ... 头文件内容 ...
//   #endif                  // 结束 #ifndef
//
// 还有一种非标准但广泛支持的方式:
//   #pragma once
//   // ... 头文件内容 ...
//
// #pragma once 更简洁, 但 #ifndef 是 C++ 标准, 兼容性更好

// ============================================================
// 六、完整示例: math_utils.h 的内容(在注释中展示)
// ============================================================

/*
// ===== 文件: math_utils.h =====
// 头文件保护符: 防止重复包含
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// 函数声明(只有声明, 没有实现)
// 声明告诉编译器函数的名称、参数类型和返回类型

// 计算两个数的最大值
int getMax(int a, int b);

// 计算两个数的最小值
int getMin(int a, int b);

// 计算绝对值
int getAbs(int value);

// 计算幂 (base^exponent)
double power(double base, int exponent);

// 判断是否为素数
bool isPrime(int number);

// 计算阶乘
long long factorial(int n);

// 计算圆的面积
double circleArea(double radius);

// 常量定义(放在头文件中, 所有包含者共享)
const double PI = 3.14159265358979;

// 结构体定义(也可以放在头文件中)
struct Point {
    double x;
    double y;
};

// 计算两点距离
double distance(Point p1, Point p2);

#endif // MATH_UTILS_H
*/

// ============================================================
// 七、完整示例: math_utils.cpp 的内容(在注释中展示)
// ============================================================

/*
// ===== 文件: math_utils.cpp =====
#include "math_utils.h"   // 包含自己的头文件(用双引号, 不是尖括号!)
#include <cmath>          // 标准库用尖括号

// 函数实现
int getMax(int a, int b) {
    return (a > b) ? a : b;
}

int getMin(int a, int b) {
    return (a < b) ? a : b;
}

int getAbs(int value) {
    return (value >= 0) ? value : -value;
}

double power(double base, int exponent) {
    double result = 1.0;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

bool isPrime(int number) {
    if (number <= 1) return false;
    if (number <= 3) return true;
    if (number % 2 == 0 || number % 3 == 0) return false;
    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0)
            return false;
    }
    return true;
}

long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

double circleArea(double radius) {
    return PI * radius * radius;
}

double distance(Point p1, Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}
*/

// ============================================================
// 八、完整示例: main.cpp 的内容(在注释中展示)
// ============================================================

/*
// ===== 文件: main.cpp =====
#include <iostream>
#include "math_utils.h"   // 包含自定义头文件
// 如果还有其他头文件:
// #include "string_utils.h"

using namespace std;

int main() {
    cout << "最大值: " << getMax(10, 20) << endl;
    cout << "5的阶乘: " << factorial(5) << endl;
    cout << "7是素数? " << (isPrime(7) ? "是" : "否") << endl;

    Point p1 = {0, 0};
    Point p2 = {3, 4};
    cout << "两点距离: " << distance(p1, p2) << endl;

    return 0;
}
*/

// ============================================================
// 九、#include 指令详解
// ============================================================
// 两种形式:
//   1. #include <filename>   - 在系统目录中查找(标准库)
//   2. #include "filename"   - 先在当前目录查找, 再到系统目录(自定义)
//
// 搜索路径:
//   尖括号: 编译器的 include 路径(如 /usr/include, 系统配置路径)
//   双引号: 先当前源文件目录, 再按尖括号方式搜索
//
// 实际上, 搜索路径可以由编译器选项(-I)指定

// ============================================================
// 十、在单文件中模拟多文件的效果
// ============================================================
// 以下用实际可运行的代码演示上述数学库的功能

// 模拟头文件中的声明
const double MY_PI = 3.14159265358979;

struct MyPoint {
    double x;
    double y;
};

// 函数声明(原型)
int myGetMax(int a, int b);
int myGetMin(int a, int b);
long long myFactorial(int n);
bool myIsPrime(int number);
double myCircleArea(double radius);
double myDistance(MyPoint p1, MyPoint p2);

// 函数定义(实现)
int myGetMax(int a, int b) {
    return (a > b) ? a : b;
}

int myGetMin(int a, int b) {
    return (a < b) ? a : b;
}

long long myFactorial(int n) {
    if (n <= 1) return 1;
    return n * myFactorial(n - 1);
}

bool myIsPrime(int number) {
    if (number <= 1) return false;
    if (number <= 3) return true;
    if (number % 2 == 0 || number % 3 == 0) return false;
    for (int i = 5; i * i <= number; i += 6) {
        if (number % i == 0 || number % (i + 2) == 0)
            return false;
    }
    return true;
}

double myCircleArea(double radius) {
    return MY_PI * radius * radius;
}

double myDistance(MyPoint p1, MyPoint p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

// main 函数: 模拟使用分文件编写的数学库
int main() {
    cout << "============================================" << endl;
    cout << "  函数分文件编写演示" << endl;
    cout << "============================================" << endl;

    // 使用数学工具函数
    cout << "\n--- 数学工具库演示 ---" << endl;
    cout << "getMax(10, 20) = " << myGetMax(10, 20) << endl;
    cout << "getMin(10, 20) = " << myGetMin(10, 20) << endl;
    cout << "factorial(6) = " << myFactorial(6) << endl;
    cout << "isPrime(17) = " << (myIsPrime(17) ? "是" : "否") << endl;
    cout << "isPrime(15) = " << (myIsPrime(15) ? "是" : "否") << endl;
    cout << "circleArea(5.0) = " << myCircleArea(5.0) << endl;

    MyPoint p1 = {0, 0};
    MyPoint p2 = {3, 4};
    cout << "distance((0,0), (3,4)) = " << myDistance(p1, p2) << endl;

    // ============================================================
    // 十一、编译多文件项目的命令
    // ============================================================
    cout << "\n--- 编译命令示例 ---" << endl;
    cout << "方法1: 分别编译再链接" << endl;
    cout << "  g++ -c math_utils.cpp -o math_utils.o" << endl;
    cout << "  g++ -c main.cpp -o main.o" << endl;
    cout << "  g++ math_utils.o main.o -o myprogram" << endl;
    cout << "\n方法2: 一次性编译" << endl;
    cout << "  g++ main.cpp math_utils.cpp -o myprogram" << endl;
    cout << "\n方法3: 使用 CMake" << endl;
    cout << "  cmake . && make" << endl;

    // ============================================================
    // 十二、分文件编写的最佳实践
    // ============================================================
    cout << "\n--- 最佳实践 ---" << endl;
    cout << "1. 每个 .h 文件都要加头文件保护符" << endl;
    cout << "2. .h 文件中只放声明, 不放定义(模板和内联函数除外)" << endl;
    cout << "3. .cpp 文件首先 #include 自己对应的 .h 文件" << endl;
    cout << "4. 不要在 .h 文件中使用 using namespace" << endl;
    cout << "5. 文件名要有意义, 体现功能" << endl;
    cout << "6. 一个类通常对应一对 .h 和 .cpp 文件" << endl;
    cout << "7. main() 函数单独放在一个文件中" << endl;
    cout << "8. 常量和宏定义放在公共头文件中" << endl;

    // ============================================================
    // 十三、常见错误
    // ============================================================
    cout << "\n--- 常见错误 ---" << endl;
    cout << "错误1: 头文件中定义全局变量(导致重复定义)" << endl;
    cout << "  解决: 用 extern 声明, 在 .cpp 中定义" << endl;
    cout << "错误2: 忘记头文件保护符(导致重复包含)" << endl;
    cout << "  解决: 每个 .h 文件都加 #ifndef/#define/#endif" << endl;
    cout << "错误3: 函数实现放在 .h 中(导致链接错误)" << endl;
    cout << "  解决: 除了模板和内联函数, 实现放在 .cpp 中" << endl;
    cout << "错误4: 使用尖括号包含自定义头文件" << endl;
    cout << "  解决: 自定义头文件用双引号 #include \"xxx.h\"" << endl;

    cout << "\n=== 函数分文件编写演示完成 ===" << endl;

    return 0;
}
