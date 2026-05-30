/*
 * 48_内存四区详解.cpp
 * 本文件详细讲解C++程序运行时的内存四区模型
 * 理解内存分区是掌握C++内存管理的基础
 * 编译命令: g++ -o 48_内存四区详解 48_内存四区详解.cpp -std=c++11
 */

#include <iostream>
#include <string>
using namespace std;

// 前向声明main函数，以便在codeAreaDemo中引用其地址
int main();

// ===================== 全局区变量定义 =====================
// 全局变量：定义在所有函数之外，程序运行期间一直存在
int g_globalVar = 100;           // 全局变量（全局区）
int g_globalVar2 = 200;          // 另一个全局变量

// 全局常量
const int g_constVar = 300;      // 全局const常量（全局区）
const string g_constStr = "全局常量字符串";  // 全局const字符串

// 静态全局变量（作用域限于本文件）
static int g_staticGlobal = 400; // 静态全局变量（全局区）

// 字符串字面量（常量区/全局区）
const char *g_strLiteral = "Hello World";  // 字符串字面量在全局区

// ===================== 1. 演示代码区 =====================
void codeAreaDemo() {
    cout << "===== 1. 代码区(Code Area) =====" << endl;
    // 代码区的特点：
    // 1. 存放程序的机器指令（编译后的二进制代码）
    // 2. 代码区是共享的：多次运行同一程序，内存中只有一份代码
    // 3. 代码区是只读的：防止程序意外修改自身指令

    // 函数名就是函数的地址（代码区的地址）
    cout << "codeAreaDemo函数地址: " << (void*)codeAreaDemo << endl;
    cout << "main函数地址: " << (void*)main << endl;
    // 这些地址都在代码区，是只读的

    // 注意：我们无法直接查看代码区的内容，因为它存储的是机器码
    // 代码区的大小在编译时就确定了

    cout << endl;
}

// ===================== 2. 演示全局区 =====================
void globalAreaDemo() {
    cout << "===== 2. 全局区(Global Area) =====" << endl;
    // 全局区（也叫静态区/数据区）存放：
    // - 全局变量
    // - 静态变量（static）
    // - 全局常量（const修饰的全局变量）
    // - 字符串字面量
    // 全局区的数据在程序启动时分配，程序结束时释放

    // 普通局部变量（不在全局区）
    int localVar = 10;

    // 静态局部变量（在全局区！）
    static int s_localStatic = 50;

    // 打印地址，观察是否在同一区域
    cout << "全局变量地址:" << endl;
    cout << "  g_globalVar:     " << &g_globalVar << endl;
    cout << "  g_globalVar2:    " << &g_globalVar2 << endl;
    cout << "  g_staticGlobal:  " << &g_staticGlobal << endl;

    cout << "\n全局常量地址:" << endl;
    cout << "  g_constVar:      " << &g_constVar << endl;
    cout << "  g_constStr:      " << &g_constStr << endl;
    cout << "  字符串字面量:    " << (void*)g_strLiteral << endl;

    cout << "\n静态局部变量地址:" << endl;
    cout << "  s_localStatic:   " << &s_localStatic << endl;
    // 注意观察：静态局部变量的地址与全局变量相近

    cout << "\n普通局部变量地址（不在全局区）:" << endl;
    cout << "  localVar:        " << &localVar << endl;
    // 普通局部变量的地址与全局变量差距较大（在栈区）

    // 全局区变量的特点：
    // 1. 程序启动时分配，程序结束时释放（生命周期最长）
    // 2. 未初始化的全局变量自动初始化为0
    // 3. 全局变量可以在多个函数中访问

    cout << endl;
}

// ===================== 3. 演示栈区 =====================
void stackAreaDemo() {
    cout << "===== 3. 栈区(Stack Area) =====" << endl;
    // 栈区存放：
    // - 函数的参数值
    // - 局部变量
    // 栈区由编译器自动分配和释放

    int a = 10;     // 局部变量，在栈区
    int b = 20;     // 局部变量，在栈区
    int arr[5] = {1, 2, 3, 4, 5};  // 局部数组，在栈区

    cout << "栈区变量地址（注意：栈向低地址增长）:" << endl;
    cout << "  a:    " << &a << endl;
    cout << "  b:    " << &b << endl;
    cout << "  arr:  " << arr << endl;
    // 栈区变量的地址通常从高到低分配（向低地址方向增长）

    // 栈区的特点：
    // 1. 由编译器自动管理，无需手动释放
    // 2. 栈空间有限（通常几MB），大量数据不要放栈上
    // 3. 函数结束后，栈上的数据立即失效

    // 栈溢出(stack overflow)示例：
    // void recursive() { int arr[1000000]; recursive(); }
    // 无限递归会导致栈空间耗尽，程序崩溃

    cout << endl;
}

// ===================== 4. 返回局部变量地址的危险 =====================
// 危险函数：返回局部变量的地址
// int* dangerousReturn() {
//     int local = 42;      // 局部变量
//     return &local;       // 返回局部变量地址——函数结束后该地址无效！
// }  // local被销毁，返回的地址指向已释放的栈内存

// 正确做法：如果需要返回数据，使用动态内存（堆区）或静态变量
int* safeReturn() {
    static int safe = 42;  // 静态变量，在全局区，不会随函数结束而销毁
    return &safe;          // 安全：静态变量的生命周期是整个程序
}

void stackDangerDemo() {
    cout << "===== 栈区的注意事项 =====" << endl;

    // 危险用法（已注释，防止程序崩溃）
    // int *p = dangerousReturn();
    // cout << *p;  // 未定义行为！可能输出42，也可能输出垃圾值

    // 安全用法
    int *p = safeReturn();
    cout << "安全返回静态变量: " << *p << endl;

    // 建议：
    // 1. 不要返回局部变量的指针或引用
    // 2. 如果必须返回，使用static、全局变量或new分配的内存
    // 3. 使用引用返回时也要注意同样的问题

    cout << endl;
}

// ===================== 5. 演示堆区 =====================
void heapAreaDemo() {
    cout << "===== 4. 堆区(Heap Area) =====" << endl;
    // 堆区存放：
    // - 程序员手动分配的内存（new/malloc）
    // - 需要程序员手动释放（delete/free）
    // 堆区空间大，但需要自己管理

    // 在堆上分配一个int
    int *pHeap = new int(42);  // 分配一个int，值为42
    cout << "堆区变量: " << *pHeap << ", 地址: " << pHeap << endl;

    // 在堆上分配数组
    int *pArr = new int[5]{10, 20, 30, 40, 50};
    cout << "堆区数组: ";
    for (int i = 0; i < 5; i++) {
        cout << pArr[i] << " ";
    }
    cout << endl;

    // 堆区地址 vs 栈区地址
    int stackVar = 100;
    cout << "\n地址对比:" << endl;
    cout << "  堆区地址(单个): " << pHeap << endl;
    cout << "  堆区地址(数组): " << pArr << endl;
    cout << "  栈区地址:       " << &stackVar << endl;
    // 通常堆区地址和栈区地址有明显差异

    // 释放堆区内存
    delete pHeap;      // 释放单个变量
    delete[] pArr;     // 释放数组
    pHeap = nullptr;   // 置空指针，避免悬空指针
    pArr = nullptr;

    // 堆区的特点：
    // 1. 空间大，可以动态分配
    // 2. 由程序员手动管理（new/delete, malloc/free）
    // 3. 如果不释放，会造成内存泄漏
    // 4. 堆区的分配和释放速度比栈慢

    cout << endl;
}

// ===================== 6. 内存四区地址范围总结 =====================
void addressRangeDemo() {
    cout << "===== 内存地址范围总结 =====" << endl;

    // 各区域变量
    int globalAddr = 1;          // 栈区
    static int staticAddr = 2;   // 全局区（静态区）
    int *heapAddr = new int(3);  // 堆区

    cout << "各区域典型地址:" << endl;
    cout << "  代码区(函数地址):  " << (void*)main << endl;
    cout << "  全局区(全局变量):  " << &g_globalVar << endl;
    cout << "  全局区(静态变量):  " << &staticAddr << endl;
    cout << "  全局区(全局常量):  " << &g_constVar << endl;
    cout << "  堆区(new分配):     " << heapAddr << endl;
    cout << "  栈区(局部变量):    " << &globalAddr << endl;

    // 地址大小关系（典型情况，不一定在所有系统上都成立）：
    // 代码区 < 全局区 < 堆区 < ... < 栈区
    // 堆区从低向高增长，栈区从高向低增长

    delete heapAddr;
    heapAddr = nullptr;

    cout << endl;
}

// ===================== 7. 静态变量的特殊性 =====================
void staticVarDemo() {
    cout << "===== 静态变量的特殊性 =====" << endl;

    // 静态局部变量：在全局区，但只在本函数内可见
    static int count = 0;  // 只在第一次调用时初始化
    count++;
    cout << "函数调用次数: " << count << endl;

    // 对比普通局部变量
    int normal = 0;
    normal++;
    cout << "普通变量值: " << normal << " (每次调用都重置)" << endl;

    // 静态局部变量的初始化只执行一次
    // 普通局部变量每次进入函数都会重新初始化
}

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 内存四区详解" << endl;
    cout << "========================================" << endl << endl;

    codeAreaDemo();      // 代码区
    globalAreaDemo();    // 全局区
    stackAreaDemo();     // 栈区
    stackDangerDemo();   // 栈区注意事项
    heapAreaDemo();      // 堆区
    addressRangeDemo();  // 地址范围总结

    // 演示静态变量的持续调用
    cout << "===== 静态变量多次调用演示 =====" << endl;
    for (int i = 0; i < 3; i++) {
        staticVarDemo();
    }

    // 总结：
    // 1. 代码区：存放可执行代码，只读、共享
    // 2. 全局区：全局变量、静态变量、全局常量、字符串字面量
    // 3. 栈区：局部变量、函数参数，自动管理，空间小
    // 4. 堆区：new分配的内存，手动管理，空间大
    // 理解内存四区对于避免内存错误至关重要

    cout << "\n程序执行完毕！" << endl;
    return 0;
}
