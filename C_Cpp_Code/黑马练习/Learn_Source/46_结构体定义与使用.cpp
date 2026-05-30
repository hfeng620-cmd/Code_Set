/*
 * 46_结构体定义与使用.cpp
 * 本文件详细讲解C++中结构体(struct)的定义和基本使用方法
 * 结构体是自定义数据类型的基础，用于将不同类型的数据组合在一起
 * 编译命令: g++ -o 46_结构体定义与使用 46_结构体定义与使用.cpp -std=c++11
 */

#include <iostream>
#include <cstring>  // for strcpy, strlen
using namespace std;

// ===================== 1. 结构体的定义 =====================
// struct关键字定义结构体类型
// 结构体是一种用户自定义的复合数据类型，可以包含多个不同类型的数据成员
struct Student {
    // 成员变量（属性）
    char name[50];    // 姓名
    int age;          // 年龄
    float score;      // 成绩
    char gender;      // 性别 'M'或'F'
    // 注意：定义结构体时不能直接给成员变量赋初值（C++11可以，但传统写法不行）
};

// 带默认值的结构体（C++11）
struct Point3D {
    double x = 0.0;  // C++11允许在定义时设置默认值
    double y = 0.0;
    double z = 0.0;
};

// ===================== 2. 全局结构体变量的定义方式 =====================
// 方式一：先定义结构体，再声明变量（推荐，最常用）
struct Book {
    char title[100];
    char author[50];
    float price;
    int pages;
};

// 方式二：在定义结构体的同时声明变量
struct Dog {
    char name[30];
    int age;
    char breed[50];  // 品种
} myDog, familyDog;  // myDog和familyDog是Dog类型的全局变量

// 方式三：匿名结构体（省略结构体名称），只能在定义时创建变量
struct {
    int x;
    int y;
} anonymousPoint;  // 匿名结构体变量，后续无法再创建同类型变量

// ===================== 3. typedef结构体 =====================
// typedef可以为结构体类型起别名，简化后续使用
typedef struct {
    char name[50];
    int id;
    float salary;
} Employee;  // Employee现在是一个类型名，使用时不需要再写struct关键字

// C++中struct定义的名称本身就是类型名，typedef不是必须的
// 但在C语言中，typedef可以省去每次写struct关键字的麻烦

// ===================== 4. 函数声明 =====================
void structBasicDemo();
void structSizeofDemo();
void structInitDemo();
void typedefDemo();
void commonMistakes();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 结构体定义与使用详解" << endl;
    cout << "========================================" << endl << endl;

    structBasicDemo();   // 结构体基本使用
    structInitDemo();    // 结构体初始化
    structSizeofDemo();  // 结构体大小与内存对齐
    typedefDemo();       // typedef结构体
    commonMistakes();    // 常见错误

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== 结构体基本使用 =====================
void structBasicDemo() {
    cout << "===== 结构体基本使用 =====" << endl;

    // --- 声明结构体变量（局部变量） ---
    Student stu1;  // 创建一个Student类型的变量

    // --- 访问成员：使用点号(.)运算符 ---
    // 对于字符数组，不能直接用=赋值，需要用strcpy
    strcpy(stu1.name, "张三");
    stu1.age = 20;
    stu1.score = 95.5f;
    stu1.gender = 'M';

    // 输出成员信息
    cout << "学生信息:" << endl;
    cout << "  姓名: " << stu1.name << endl;
    cout << "  年龄: " << stu1.age << endl;
    cout << "  成绩: " << stu1.score << endl;
    cout << "  性别: " << (stu1.gender == 'M' ? "男" : "女") << endl;

    // --- 创建另一个结构体变量 ---
    Student stu2;
    strcpy(stu2.name, "李四");
    stu2.age = 19;
    stu2.score = 88.0f;
    stu2.gender = 'F';

    // --- 结构体变量之间可以直接赋值（整体拷贝） ---
    Student stu3 = stu2;  // 将stu2的所有成员拷贝给stu3
    cout << "\nstu3(拷贝自stu2): " << stu3.name << ", " << stu3.age << "岁" << endl;

    // --- 修改拷贝不影响原变量 ---
    strcpy(stu3.name, "王五");
    stu3.age = 21;
    cout << "修改后stu3: " << stu3.name << ", " << stu3.age << "岁" << endl;
    cout << "stu2不变: " << stu2.name << ", " << stu2.age << "岁" << endl;

    // --- 使用匿名结构体变量 ---
    anonymousPoint.x = 10;
    anonymousPoint.y = 20;
    cout << "\n匿名结构体: (" << anonymousPoint.x << ", " << anonymousPoint.y << ")" << endl;
    // 注意：匿名结构体只能创建这一个变量，无法再声明同类型的新变量

    cout << endl;
}

// ===================== 结构体初始化方式 =====================
void structInitDemo() {
    cout << "===== 结构体初始化方式 =====" << endl;

    // 方式1：按顺序初始化（最常用）
    Book book1 = {"C++ Primer", "Stanley Lippman", 128.0f, 976};
    cout << "方式1 - 按顺序初始化:" << endl;
    cout << "  书名: " << book1.title << ", 作者: " << book1.author << endl;
    cout << "  价格: " << book1.price << ", 页数: " << book1.pages << endl;

    // 方式2：部分初始化（未指定的成员自动初始化为0）
    Book book2 = {"数据结构", "严蔚敏"};
    cout << "\n方式2 - 部分初始化:" << endl;
    cout << "  书名: " << book2.title << ", 价格: " << book2.price << " (自动为0)" << endl;

    // 方式3：逐个赋值（定义后赋值）
    Book book3;
    strcpy(book3.title, "算法导论");
    strcpy(book3.author, "CLRS");
    book3.price = 128.0f;
    book3.pages = 1312;
    cout << "\n方式3 - 逐个赋值:" << endl;
    cout << "  书名: " << book3.title << ", 页数: " << book3.pages << endl;

    // 方式4：C++11统一初始化语法
    Book book4{"深入理解计算机系统", "Bryant", 139.0f, 1000};
    cout << "\n方式4 - C++11统一初始化:" << endl;
    cout << "  书名: " << book4.title << endl;

    // 注意区分：初始化 vs 赋值
    // 初始化：创建变量时给初值
    Book book5 = {"操作系统", "汤小丹", 59.0f, 600};  // 初始化
    // 赋值：变量已经存在，修改其值
    book5.price = 49.0f;  // 赋值（修改已存在的成员）

    // 错误示范：
    // Book bad;  // 未初始化的结构体，成员值是随机的（全局变量才自动为0）
    // cout << bad.price;  // 未定义行为！局部结构体变量不会自动清零

    cout << endl;
}

// ===================== 结构体大小与内存对齐 =====================
void structSizeofDemo() {
    cout << "===== 结构体大小与内存对齐 =====" << endl;

    // 定义一个包含不同大小成员的结构体
    struct Mixed {
        char a;     // 1字节
        int b;      // 4字节
        char c;     // 1字节
        double d;   // 8字节
    };

    struct Mixed m;
    cout << "Mixed结构体大小: " << sizeof(Mixed) << " 字节" << endl;
    // 理论上: 1 + 4 + 1 + 8 = 14字节
    // 实际上可能是24字节（因为内存对齐）

    // 内存对齐规则说明：
    // 1. 每个成员的起始地址必须是其自身大小的整数倍
    // 2. 整个结构体的大小必须是最大成员大小的整数倍
    // 3. 对齐是为了提高CPU访问内存的效率

    // 验证各成员的偏移地址
    cout << "成员偏移量:" << endl;
    cout << "  a的偏移: " << offsetof(Mixed, a) << endl;  // 0
    cout << "  b的偏移: " << offsetof(Mixed, b) << endl;  // 4（前面有3字节填充）
    cout << "  c的偏移: " << offsetof(Mixed, c) << endl;  // 8
    cout << "  d的偏移: " << offsetof(Mixed, d) << endl;  // 16（前面有7字节填充）

    // 对比：调整成员顺序可以改变结构体大小
    struct Optimized {
        double d;   // 8字节，偏移0
        int b;      // 4字节，偏移8
        char a;     // 1字节，偏移12
        char c;     // 1字节，偏移13
        // 总计: 14，对齐到16（8的倍数）
    };
    cout << "\nOptimized结构体大小: " << sizeof(Optimized) << " 字节" << endl;
    // 通过合理排列成员顺序，可以减少内存浪费

    // 基本类型大小参考
    cout << "\n基本类型大小:" << endl;
    cout << "  char: " << sizeof(char) << " 字节" << endl;
    cout << "  int: " << sizeof(int) << " 字节" << endl;
    cout << "  float: " << sizeof(float) << " 字节" << endl;
    cout << "  double: " << sizeof(double) << " 字节" << endl;

    // 结构体变量的大小
    cout << "\n各结构体大小:" << endl;
    cout << "  Student: " << sizeof(Student) << " 字节" << endl;
    cout << "  Book: " << sizeof(Book) << " 字节" << endl;
    cout << "  Dog: " << sizeof(Dog) << " 字节" << endl;
    cout << "  Point3D: " << sizeof(Point3D) << " 字节" << endl;

    cout << endl;
}

// ===================== typedef结构体 =====================
void typedefDemo() {
    cout << "===== typedef结构体 =====" << endl;

    // 使用typedef定义的Employee类型
    Employee emp1;
    strcpy(emp1.name, "赵六");
    emp1.id = 1001;
    emp1.salary = 15000.0f;

    cout << "员工信息:" << endl;
    cout << "  姓名: " << emp1.name << endl;
    cout << "  工号: " << emp1.id << endl;
    cout << "  薪资: " << emp1.salary << endl;

    // 在C语言中，没有typedef的话每次都要写 struct Employee
    // 在C++中，struct定义的名称直接就是类型名，typedef不是必须的
    // 但typedef在某些场景下仍然有用（如兼容C代码）

    // typedef还可以为基本类型起别名
    typedef int Score;
    typedef const char* CString;

    Score s1 = 95;
    CString str = "Hello";
    cout << "\ntypedef别名: Score = " << s1 << ", CString = " << str << endl;

    // 注意区分typedef和using（C++11推荐）
    // typedef int MyInt;      // 传统写法
    // using MyInt = int;      // C++11新写法，更直观

    cout << endl;
}

// ===================== 常见错误 =====================
void commonMistakes() {
    cout << "===== 常见错误和注意事项 =====" << endl;

    // 错误1：忘记初始化就使用
    // Student badStu;
    // cout << badStu.name;  // 未定义行为！name数组内容随机
    // 正确做法：声明后立刻初始化
    Student goodStu = {};  // 所有成员初始化为0/空
    cout << "正确初始化后的age: " << goodStu.age << " (应为0)" << endl;

    // 错误2：字符数组成员用=赋值
    Student stu;
    // stu.name = "张三";  // 错误！数组名是常量指针，不能赋值
    strcpy(stu.name, "张三");  // 正确：使用strcpy

    // 如果用string类型就简单多了
    struct ModernStudent {
        string name;  // 使用string而非char数组
        int age;
        float score;
    };
    ModernStudent ms;
    ms.name = "李四";  // string支持直接赋值
    ms.age = 20;
    ms.score = 95.5f;
    cout << "ModernStudent: " << ms.name << ", " << ms.age << "岁" << endl;

    // 错误3：结构体比较
    Student s1 = {"A", 18, 90.0f, 'M'};
    Student s2 = {"A", 18, 90.0f, 'M'};
    // if (s1 == s2) {}  // 错误！结构体不能直接用==比较
    // 正确做法：逐个成员比较
    bool equal = (strcmp(s1.name, s2.name) == 0) &&
                 (s1.age == s2.age) &&
                 (s1.score == s2.score);
    cout << "s1和s2" << (equal ? "相等" : "不等") << endl;

    // 错误4：两个不同类型的结构体变量不能互相赋值
    // Book b;
    // Student s;
    // b = s;  // 错误！类型不同

    // 小技巧：使用花括号清零整个结构体
    Student zeroed = {};  // 所有成员归零
    cout << "清零后的age: " << zeroed.age << endl;

    cout << endl;
}
