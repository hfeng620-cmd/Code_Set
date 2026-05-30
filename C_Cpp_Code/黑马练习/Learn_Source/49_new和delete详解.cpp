/*
 * 49_new和delete详解.cpp
 * 本文件详细讲解C++中new和delete运算符的使用方法
 * new/delete是C++动态内存管理的核心，替代C语言的malloc/free
 * 编译命令: g++ -o 49_new和delete详解 49_new和delete详解.cpp -std=c++11
 */

#include <iostream>
using namespace std;

// ===================== 1. 基本new/delete用法 =====================
void basicNewDelete() {
    cout << "===== 1. 基本new/delete用法 =====" << endl;

    // --- new一个int ---
    // new int 在堆上分配一个int空间，值未初始化（随机值）
    int *p1 = new int;
    cout << "new int (未初始化): " << *p1 << " (随机值)" << endl;
    delete p1;    // 释放内存
    p1 = nullptr; // 置空，避免悬空指针

    // --- new int(10) 带初始化 ---
    // 在堆上分配一个int，并初始化为10
    int *p2 = new int(10);
    cout << "new int(10): " << *p2 << endl;
    delete p2;
    p2 = nullptr;

    // --- new int[10] 分配数组 ---
    // 在堆上分配10个int的数组
    int *p3 = new int[10];  // 10个int，值未初始化
    // 手动初始化数组
    for (int i = 0; i < 10; i++) {
        p3[i] = i * 10;  // 使用下标访问，和普通数组一样
    }
    cout << "new int[10]: ";
    for (int i = 0; i < 10; i++) {
        cout << p3[i] << " ";
    }
    cout << endl;
    delete[] p3;   // 释放数组必须用delete[]
    p3 = nullptr;

    // --- C++11 带初始化列表的数组new ---
    int *p4 = new int[5]{100, 200, 300, 400, 500};
    cout << "new int[5]{...}: ";
    for (int i = 0; i < 5; i++) {
        cout << p4[i] << " ";
    }
    cout << endl;
    delete[] p4;
    p4 = nullptr;

    cout << endl;
}

// ===================== 2. new/delete匹配规则 =====================
void matchRule() {
    cout << "===== 2. new/delete匹配规则 =====" << endl;

    // 规则1：new 单个变量 用 delete
    int *p1 = new int(42);
    cout << "单个变量: " << *p1 << endl;
    delete p1;     // 正确
    p1 = nullptr;

    // 规则2：new 数组 用 delete[]
    int *p2 = new int[5]{1, 2, 3, 4, 5};
    cout << "数组: ";
    for (int i = 0; i < 5; i++) cout << p2[i] << " ";
    cout << endl;
    delete[] p2;   // 正确：[]告诉编译器释放整个数组
    p2 = nullptr;

    // 常见错误（不注释了，只说明）：
    // 错误1: new[] 配 delete (不带[])
    // int *p = new int[10];
    // delete p;      // 可能只释放第一个元素，导致内存泄漏
    //
    // 错误2: new 配 delete[]
    // int *p = new int(10);
    // delete[] p;    // 未定义行为，可能崩溃
    //
    // 错误3: 忘记delete
    // int *p = new int(10);
    // p = nullptr;   // 原来的内存地址丢失，无法释放 = 内存泄漏

    cout << "规则总结: new配delete, new[]配delete[]" << endl;

    cout << endl;
}

// ===================== 3. 内存泄漏演示 =====================
void memoryLeakDemo() {
    cout << "===== 3. 内存泄漏概念 =====" << endl;

    // 内存泄漏：分配了内存但没有释放，导致内存无法被回收
    // 内存泄漏不会立即导致程序崩溃，但会逐渐消耗系统内存

    // 示例1：忘记delete
    // for (int i = 0; i < 1000; i++) {
    //     int *p = new int(i);  // 每次循环都分配内存
    //     // 忘记 delete p;
    //     // 如果不清空p，下次循环p指向新地址，旧地址就泄漏了
    // }

    // 示例2：中途return导致没执行到delete
    // void riskyFunction() {
    //     int *p = new int(100);
    //     if (someCondition) return;  // 提前return，没执行delete！
    //     delete p;  // 可能执行不到
    // }

    // 示例3：异常导致没执行到delete
    // void exceptionFunction() {
    //     int *p = new int(100);
    //     someFunction();  // 如果这里抛出异常
    //     delete p;        // 不会执行到！
    // }

    // 正确做法：在所有可能的退出路径上都释放内存
    // 或使用智能指针（后续课程讲解）
    cout << "内存泄漏的危害:" << endl;
    cout << "  1. 程序占用内存越来越多" << endl;
    cout << "  2. 最终可能导致系统内存不足" << endl;
    cout << "  3. 长期运行的程序(服务器)尤其危险" << endl;

    // 安全的new/delete模式
    int *p = new int(42);
    try {
        // 使用p做一些操作...
        cout << "安全使用: " << *p << endl;
    } catch (...) {
        delete p;   // 异常时也要释放
        throw;      // 重新抛出异常
    }
    delete p;       // 正常时释放
    p = nullptr;

    cout << endl;
}

// ===================== 4. 动态创建对象 =====================
struct Student {
    char name[50];
    int age;
    float score;

    // 构造函数
    Student() : age(0), score(0.0f) {
        name[0] = '\0';
        cout << "  Student默认构造" << endl;
    }
    Student(const char *n, int a, float s) : age(a), score(s) {
        // 简单复制名字
        int i = 0;
        while (n[i] && i < 49) { name[i] = n[i]; i++; }
        name[i] = '\0';
        cout << "  Student参数构造: " << name << endl;
    }
    ~Student() {
        cout << "  Student析构: " << name << endl;
    }
};

void dynamicObjectDemo() {
    cout << "===== 4. 动态创建/销毁对象 =====" << endl;

    // new一个对象：调用构造函数
    cout << "--- new单个对象 ---" << endl;
    Student *ps1 = new Student("张三", 20, 95.5f);
    cout << "创建: " << ps1->name << ", " << ps1->age << "岁" << endl;
    delete ps1;   // 调用析构函数，然后释放内存
    ps1 = nullptr;

    // new对象数组：每个元素都调用构造函数
    cout << "\n--- new对象数组 ---" << endl;
    Student *ps2 = new Student[3]{
        Student("李四", 19, 88.0f),
        Student("王五", 21, 76.0f),
        Student("赵六", 20, 92.0f)
    };
    cout << "数组创建完成" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  " << ps2[i].name << " " << ps2[i].score << "分" << endl;
    }
    delete[] ps2;  // 每个元素都调用析构函数，注意用delete[]
    ps2 = nullptr;

    cout << endl;
}

// ===================== 5. 二维数组的动态创建 =====================
void twoDArrayDemo() {
    cout << "===== 5. 动态创建二维数组 =====" << endl;

    int rows = 3, cols = 4;

    // 方法1：使用指针数组
    // 先创建一个指针数组（存放每行的首地址）
    int **matrix = new int*[rows];
    // 为每一行分配内存
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    // 初始化并打印
    int count = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = count++;
        }
    }

    cout << "动态二维数组 (" << rows << "x" << cols << "):" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }

    // 释放二维数组（注意顺序：先释放每行，再释放指针数组）
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];  // 释放每一行
    }
    delete[] matrix;         // 释放指针数组
    matrix = nullptr;

    // 方法2：使用一维数组模拟二维
    cout << "\n--- 一维数组模拟二维 ---" << endl;
    int *flat = new int[rows * cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            flat[i * cols + j] = (i + 1) * 10 + j;  // 手动计算索引
        }
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << flat[i * cols + j] << "\t";
        }
        cout << endl;
    }
    delete[] flat;
    flat = nullptr;

    cout << endl;
}

// ===================== 6. new与malloc对比 =====================
void newVsMalloc() {
    cout << "===== 6. new vs malloc 对比 =====" << endl;

    // C++的new
    int *p1 = new int(42);     // 分配+初始化
    cout << "new: " << *p1 << endl;
    delete p1;
    p1 = nullptr;

    // C语言的malloc（C++也兼容）
    int *p2 = (int*)malloc(sizeof(int));  // 只分配，不初始化
    if (p2 != nullptr) {
        *p2 = 42;  // 手动赋值
        cout << "malloc: " << *p2 << endl;
        free(p2);
        p2 = nullptr;
    }

    // new的优势：
    // 1. 自动计算大小（不需要sizeof）
    // 2. 自动类型转换（不需要强制转换）
    // 3. 可以调用构造函数（malloc不能）
    // 4. 可以重载（自定义内存分配策略）
    // 5. 是运算符，不是函数

    // 对于类对象，new/delete会调用构造/析构函数
    // malloc/free不会，所以C++中应使用new/delete

    cout << endl;
}

// ===================== 7. 最佳实践 =====================
void bestPractices() {
    cout << "===== 7. 最佳实践 =====" << endl;

    // 实践1：delete后置空指针
    int *p = new int(100);
    cout << "使用: " << *p << endl;
    delete p;
    p = nullptr;  // 防止悬空指针
    // delete nullptr;  // delete空指针是安全的（什么都不做）

    // 实践2：避免重复delete
    int *p2 = new int(200);
    delete p2;
    p2 = nullptr;
    // delete p2;  // 如果p2已置空，重复delete是安全的
    // 如果不置空，重复delete是未定义行为！

    // 实践3：使用RAII思想（资源获取即初始化）
    // 把资源封装在类中，利用构造/析构函数自动管理
    // 这就是智能指针的基本原理（后续课程）

    // 实践4：检查new是否成功（C++默认抛异常）
    try {
        // C++中new失败会抛出std::bad_alloc异常
        // int *bigArray = new int[9999999999];  // 可能失败
        int *safe = new int(42);
        cout << "安全分配: " << *safe << endl;
        delete safe;
        safe = nullptr;
    } catch (const bad_alloc &e) {
        cout << "内存分配失败: " << e.what() << endl;
    }

    // 实践5：nothrow版本（不抛异常，返回nullptr）
    int *p3 = new(nothrow) int(300);
    if (p3 != nullptr) {
        cout << "nothrow分配: " << *p3 << endl;
        delete p3;
        p3 = nullptr;
    }

    cout << endl;
}

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ new和delete详解" << endl;
    cout << "========================================" << endl << endl;

    basicNewDelete();   // 基本用法
    matchRule();        // 匹配规则
    memoryLeakDemo();   // 内存泄漏
    dynamicObjectDemo();// 动态对象
    twoDArrayDemo();    // 二维数组
    newVsMalloc();      // new vs malloc
    bestPractices();    // 最佳实践

    // 总结：
    // 1. new/delete 是C++动态内存管理的核心
    // 2. new分配内存并调用构造函数，delete调用析构函数并释放内存
    // 3. new配delete，new[]配delete[]，不可混用
    // 4. delete后要置空指针，防止悬空指针和重复delete
    // 5. 内存泄漏是C++最常见的bug，务必确保每个new都有对应的delete
    // 6. 后续学习智能指针可以更安全地管理动态内存

    cout << "程序执行完毕！" << endl;
    return 0;
}
