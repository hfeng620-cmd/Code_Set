/*
 * 42_const修饰指针详解.cpp
 * 本文件详细演示 C++ 中 const 修饰指针的各种情况
 * 包括: const int*, int* const, const int* const
 *       以及每种情况的读法、含义和使用场景
 *
 * 核心记忆技巧: 从右向左读!
 *   const int* p   -> p is a pointer to const int (指向常量的指针)
 *   int* const p   -> p is a const pointer to int (常量指针)
 *   const int* const p -> p is a const pointer to const int
 */

#include <iostream>
#include <string>
using namespace std;

int main() {

    // ============================================================
    // 一、基本概念: const 与指针的组合
    // ============================================================
    // const 和指针有三种组合方式:
    //   1. const int* p   - 指向常量的指针(指针可变, 指向的值不可变)
    //   2. int* const p   - 常量指针(指针不可变, 指向的值可变)
    //   3. const int* const p - 指向常量的常量指针(都不可变)
    //
    // 记忆方法: 看 const 在 * 的左边还是右边
    //   const 在 * 左边 -> 修饰数据(数据不可变)
    //   const 在 * 右边 -> 修饰指针(指针不可变)

    // ============================================================
    // 二、const int* p - 指向常量的指针(Pointer to Const)
    // ============================================================
    // 含义: 指针指向的数据不能通过该指针修改
    //        但指针本身可以改变指向
    // 别名: 底层const (low-level const)

    cout << "=== const int* p - 指向常量的指针 ===" << endl;

    int val1 = 10;
    int val2 = 20;

    const int* p1 = &val1;  // p1 指向一个 const int
    // 从右向左读: p1 is a pointer to const int

    // 可以读取数据
    cout << "*p1 = " << *p1 << endl;   // 输出 10

    // 不可以通过 p1 修改数据
    // *p1 = 100;  // 编译错误! const int* 不允许通过指针修改值

    // 但可以通过原变量修改数据
    val1 = 100;
    cout << "val1 修改后, *p1 = " << *p1 << endl;  // 输出 100

    // 指针本身可以改变指向
    p1 = &val2;  // 正确! p1 可以指向其他变量
    cout << "p1 改变指向后, *p1 = " << *p1 << endl;  // 输出 20

    // 可以用 const 变量的地址来初始化
    const int constVal = 50;
    const int* p1b = &constVal;  // 正确! const int* 可以指向 const int
    cout << "*p1b = " << *p1b << endl;

    // 不能用普通指针指向 const 变量(除非强制转换)
    // int* badPtr = &constVal;  // 编译错误! 安全性问题

    // ============================================================
    // 三、int* const p - 常量指针(Const Pointer)
    // ============================================================
    // 含义: 指针本身不可改变(必须在声明时初始化)
    //        但可以通过指针修改指向的数据
    // 别名: 顶层const (top-level const)

    cout << "\n=== int* const p - 常量指针 ===" << endl;

    int val3 = 30;
    int val4 = 40;

    int* const p2 = &val3;  // p2 是常量指针, 必须初始化
    // 从右向左读: p2 is a const pointer to int

    // 可以通过指针修改数据
    *p2 = 300;  // 正确! int* const 允许修改指向的值
    cout << "*p2 = " << *p2 << endl;   // 输出 300
    cout << "val3 = " << val3 << endl; // val3 也被修改为 300

    // 不可以改变指针的指向
    // p2 = &val4;  // 编译错误! const 指针不能改变指向
    // p2 = nullptr; // 编译错误!

    // 读取数据当然也可以
    cout << "*p2 = " << *p2 << endl;

    // ============================================================
    // 四、const int* const p - 指向常量的常量指针
    // ============================================================
    // 含义: 指针不可改变, 指向的数据也不可通过该指针修改
    //        最严格的约束, 两边都不能动

    cout << "\n=== const int* const p - 双重const ===" << endl;

    int val5 = 50;
    int val6 = 60;

    const int* const p3 = &val5;
    // 从右向左读: p3 is a const pointer to const int

    // 读取数据
    cout << "*p3 = " << *p3 << endl;   // 输出 50

    // 不可以通过指针修改数据
    // *p3 = 500;  // 编译错误! const int* 部分

    // 不可以改变指针指向
    // p3 = &val6;  // 编译错误! * const 部分

    // 但可以通过原变量修改
    val5 = 500;
    cout << "val5 修改后, *p3 = " << *p3 << endl;  // 输出 500

    // ============================================================
    // 五、记忆技巧总结
    // ============================================================
    cout << "\n=== 记忆技巧 ===" << endl;
    cout << "方法1: 从右向左读" << endl;
    cout << "  const int* p  -> p is a pointer to const int (指向常量的指针)" << endl;
    cout << "  int* const p  -> p is a const pointer to int (常量指针)" << endl;
    cout << endl;
    cout << "方法2: const 在 * 的位置" << endl;
    cout << "  const 在 * 左边 -> 数据不可变(底层const)" << endl;
    cout << "  const 在 * 右边 -> 指针不可变(顶层const)" << endl;
    cout << endl;
    cout << "方法3: 简化记忆" << endl;
    cout << "  const int* p  = 数据被锁, 指针自由" << endl;
    cout << "  int* const p  = 指针被锁, 数据自由" << endl;
    cout << "  const int* const p = 全部被锁" << endl;

    // ============================================================
    // 六、const 指针的赋值规则
    // ============================================================
    cout << "\n=== 赋值规则 ===" << endl;

    // 规则: 只能用 const 来加强限制, 不能放松
    int ordinary = 10;
    const int cValue = 20;

    // 普通指针 -> 指向常量的指针: 正确(加强限制)
    const int* cp1 = &ordinary;  // 正确
    const int* cp2 = &cValue;    // 正确

    // 指向常量的指针 -> 普通指针: 错误(放松限制)
    // int* bad1 = cp1;  // 编译错误!

    // 普通指针 -> 常量指针: 需要初始化时确定
    int* const cp3 = &ordinary;  // 正确
    // int* const cp4 = &cValue;  // 编译错误! 普通指针不能指向 const

    cout << "const 指针赋值规则已演示" << endl;

    // ============================================================
    // 七、函数参数中的 const 指针
    // ============================================================
    cout << "\n=== 函数参数中的 const 指针 ===" << endl;

    // 使用 const int* 作为函数参数: 防止函数修改传入的数据
    // 这是一种"只读"语义, 告诉调用者: 我不会修改你的数据

    // 打印数组(不会修改数组内容)
    auto printArray = [](const int* arr, int size) {
        // arr[0] = 100;  // 编译错误! const int* 不允许修改
        for (int i = 0; i < size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    };

    // 修改数组(需要修改数组内容)
    auto doubleArray = [](int* arr, int size) {
        for (int i = 0; i < size; i++) {
            arr[i] *= 2;  // 正确! 普通指针允许修改
        }
    };

    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;

    cout << "原始数组: ";
    printArray(arr, size);

    doubleArray(arr, size);
    cout << "翻倍后: ";
    printArray(arr, size);

    // ============================================================
    // 八、返回值中的 const 指针
    // ============================================================
    cout << "\n=== 返回值中的 const 指针 ===" << endl;

    // 返回 const 指针: 调用者不能修改返回的数据
    static int staticVal = 42;
    auto getReadOnlyPtr = []() -> const int* {
        return &staticVal;
    };

    const int* retPtr = getReadOnlyPtr();
    cout << "返回的值: " << *retPtr << endl;
    // *retPtr = 100;  // 编译错误! 不允许修改

    // ============================================================
    // 九、const 与多级指针
    // ============================================================
    cout << "\n=== const 与多级指针 ===" << endl;

    int value = 100;

    // 二级指针 + const
    int* pOrdinary = &value;
    int** pp1 = &pOrdinary;           // 普通二级指针

    // const int** pp2 = &pOrdinary;  // 错误! 类型不匹配
    int* const* pp3 = &pOrdinary;     // 指向常量指针的指针
    int** const pp4 = &pOrdinary;     // 常量二级指针

    cout << "**pp1 = " << **pp1 << endl;
    cout << "**pp3 = " << **pp3 << endl;
    cout << "**pp4 = " << **pp4 << endl;

    // ============================================================
    // 十、常见错误与陷阱
    // ============================================================
    cout << "\n=== 常见错误 ===" << endl;

    // --- 错误1: 混淆 const int* 和 int* const ---
    // const int* p;    -> 数据不可变, 指针可变
    // int* const p;    -> 指针不可变, 数据可变
    // 它们的含义完全不同!

    // --- 错误2: 声明常量指针时忘记初始化 ---
    // int* const p;    // 编译错误! 常量指针必须在声明时初始化
    int* const mustInit = &val1;  // 正确

    // --- 错误3: 试图通过 const_cast 绕过 const ---
    const int safeVal = 42;
    const int* safePtr = &safeVal;
    // 以下行为是未定义的(虽然编译可能通过):
    // int* unsafe = const_cast<int*>(safePtr);
    // *unsafe = 100;  // 未定义行为! 不要这样做!

    // --- 错误4: 函数参数忘记加 const ---
    // 如果函数不修改指针指向的数据, 应该加 const
    // 这样调用者可以传入 const 数据, 也更安全
    // void func(int* p)    -> 不能传入 const int*
    // void func(const int* p) -> 可以传入 int* 和 const int*

    // --- 错误5: 返回局部变量的指针 ---
    // int* badFunc() {
    //     int local = 10;
    //     return &local;  // 错误! 局部变量在函数结束后销毁
    // }

    // ============================================================
    // 十一、实际应用建议
    // ============================================================
    cout << "\n=== 实际应用建议 ===" << endl;
    cout << "1. 函数参数如果不修改数据, 用 const int* (或 const int&)" << endl;
    cout << "2. 如果指针本身不需要改变指向, 用 int* const" << endl;
    cout << "3. 优先使用 const 引用而不是 const 指针(更安全)" << endl;
    cout << "4. const 是一种契约: 告诉编译器和其他程序员你的意图" << endl;
    cout << "5. 能用 const 就用 const (const 正确性)" << endl;

    cout << "\n=== const 修饰指针详解完成 ===" << endl;

    return 0;
}
