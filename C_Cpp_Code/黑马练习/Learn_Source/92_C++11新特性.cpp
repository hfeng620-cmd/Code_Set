/*
 * ============================================================
 *  C++11 新特性详解
 * ============================================================
 *
 *  C++11 是 C++ 历史上最大的一次更新，引入了大量新特性。
 *  本文件覆盖最常用的 C++11 特性，每个都附带详细注释。
 *
 *  编译命令（需要 C++11 或更高标准）：
 *      g++ -std=c++11 92_C++11新特性.cpp -o demo
 *      或
 *      cl /EHsc /std:c++11 92_C++11新特性.cpp
 *
 *  主要特性：
 *      1. auto 自动类型推导
 *      2. decltype 类型推导
 *      3. nullptr 替代 NULL
 *      4. 范围 for 循环
 *      5. 列表初始化 {}
 *      6. Lambda 表达式
 *      7. 右值引用与移动语义
 *      8. enum class 强类型枚举
 *      9. 原始字符串 R"()"
 *     10. std::function 和 std::bind
 *     11. 初始化列表 initializer_list
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>   // std::function, std::bind
#include <initializer_list>
#include <map>
#include <memory>       // 智能指针
using namespace std;

// ============================================================
// 1. auto 自动类型推导
// ============================================================
// 编译器根据初始化表达式自动推导变量类型
// 避免写又长又复杂的类型名

void demo_auto() {
    cout << "===== auto 自动类型推导 =====" << endl;

    // 基本用法：编译器自动推导类型
    auto a = 10;              // int
    auto b = 3.14;            // double
    auto c = 'A';             // char
    auto d = string("hello"); // string
    auto e = true;            // bool

    cout << "a = " << a << " (int)" << endl;
    cout << "b = " << b << " (double)" << endl;
    cout << "c = " << c << " (char)" << endl;
    cout << "d = " << d << " (string)" << endl;
    cout << "e = " << e << " (bool)" << endl;

    // 最常用于迭代器（避免写又长又复杂的类型）
    vector<int> v = {1, 2, 3, 4, 5};

    // 不用 auto：类型名很长
    // for (vector<int>::iterator it = v.begin(); it != v.end(); it++) { ... }

    // 用 auto：简洁明了
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    // auto 的推导规则
    auto x = 10;          // int
    auto &y = x;          // int&（引用）
    auto *z = &x;         // int*（指针）
    const auto w = 10;    // const int

    cout << "x=" << x << ", y=" << y << ", *z=" << *z << ", w=" << w << endl;

    // 【注意】auto 的限制：
    // 不能用于函数参数：void func(auto a) {}  // C++20 前不行
    // 不能用于类成员变量：class A { auto a = 10; };  // 错误！
    // 不能用于声明（必须初始化）：auto a;  // 错误！编译器无法推导
}

// ============================================================
// 2. decltype 类型推导
// ============================================================
// decltype 根据表达式推导类型，但不求值（不会执行表达式）
// 与 auto 的区别：auto 根据值推导，decltype 根据表达式类型推导

void demo_decltype() {
    cout << "\n===== decltype 类型推导 =====" << endl;

    int a = 10;
    double b = 3.14;

    decltype(a) c = 20;          // int（和 a 同类型）
    decltype(b) d = 2.71;        // double（和 b 同类型）
    decltype(a + b) e = 1.5;     // double（int + double = double）

    cout << "c = " << c << " (decltype(a) -> int)" << endl;
    cout << "d = " << d << " (decltype(b) -> double)" << endl;
    cout << "e = " << e << " (decltype(a+b) -> double)" << endl;

    // auto vs decltype
    auto x = a;      // int（根据值推导）
    decltype(a) y;   // int（根据类型推导，不需要初始化）

    // decltype 的特殊规则
    int &r = a;           // r 是 a 的引用
    decltype(r) b2 = a;   // int&（r 是引用，推导出引用）
    decltype(a + 0) c2;   // int（a+0 是右值，推导出值）
    decltype((a)) d2 = a; // int&（(a) 是左值表达式，推导出引用）

    cout << "b2 = " << b2 << ", c2 = " << c2 << ", d2 = " << d2 << endl;
}

// ============================================================
// 3. nullptr 替代 NULL
// ============================================================
// NULL 实际上是 0（整数），在函数重载时有歧义
// nullptr 的类型是 std::nullptr_t，可以隐式转换为任意指针类型

void func_overload(int) {
    cout << "  调用了 func(int)" << endl;
}

void func_overload(char*) {
    cout << "  调用了 func(char*)" << endl;
}

void demo_nullptr() {
    cout << "\n===== nullptr 替代 NULL =====" << endl;

    // NULL 的问题：实际是 0，会调用 int 版本
    cout << "func(NULL):";
    func_overload(NULL);    // 调用 func(int)！不是你想要的

    // nullptr 正确调用指针版本
    cout << "func(nullptr):";
    func_overload(nullptr); // 调用 func(char*)，正确！

    // nullptr 可以赋值给任意指针类型
    int *p = nullptr;       // 正确
    char *q = nullptr;      // 正确
    // int a = nullptr;     // 错误！不能转换为整型

    // 判断指针是否为空
    if (p == nullptr) {
        cout << "p 是空指针" << endl;
    }

    // 【建议】C++11 以后统一使用 nullptr，不要再用 NULL
}

// ============================================================
// 4. 范围 for 循环
// ============================================================
// 语法：for (元素类型 变量名 : 容器) { ... }
// 要求容器有 begin() 和 end() 方法，或者是数组

void demo_range_for() {
    cout << "\n===== 范围 for 循环 =====" << endl;

    vector<int> v = {1, 2, 3, 4, 5};

    // 方式1：值拷贝（不能修改原容器）
    cout << "值拷贝遍历：";
    for (auto item : v) {
        cout << item << " ";
    }
    cout << endl;

    // 方式2：引用（可以修改原容器）
    for (auto &item : v) {
        item *= 2;  // 每个元素翻倍
    }
    cout << "翻倍后：";
    for (auto item : v) {
        cout << item << " ";
    }
    cout << endl;

    // 方式3：const 引用（只读，避免拷贝，推荐）
    cout << "const引用遍历：";
    for (const auto &item : v) {
        cout << item << " ";
        // item *= 2;  // 错误！const 引用不能修改
    }
    cout << endl;

    // 也可以用于数组
    int arr[] = {10, 20, 30, 40, 50};
    cout << "数组遍历：";
    for (auto x : arr) {
        cout << x << " ";
    }
    cout << endl;

    // 用于 map
    map<string, int> m = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    cout << "map遍历：" << endl;
    for (const auto &pair : m) {
        cout << "  " << pair.first << " -> " << pair.second << endl;
    }
}

// ============================================================
// 5. 列表初始化 {}
// ============================================================
// 用花括号 {} 初始化变量，防止窄化转换（数据丢失）

void demo_list_init() {
    cout << "\n===== 列表初始化 =====" << endl;

    // 基本类型
    int a{10};                // int a = 10
    int b = {20};             // int b = 20
    string s{"hello"};        // string s = "hello"

    cout << "a=" << a << ", b=" << b << ", s=" << s << endl;

    // 容器的列表初始化
    vector<int> v{1, 2, 3, 4, 5};
    map<string, int> m{{"a", 1}, {"b", 2}};
    list<int> l{10, 20, 30};

    cout << "vector: ";
    for (auto x : v) cout << x << " ";
    cout << endl;

    // 防止窄化转换（数据丢失）
    // int x{3.14};          // 错误！double→int 丢失小数部分，编译器报错
    // int y{200};           // 如果 char y{200}，可能溢出，编译器警告

    // 安全的转换
    double d{3.14};           // OK，int→double 不会丢失
    cout << "d=" << d << endl;
}

// ============================================================
// 6. Lambda 表达式
// ============================================================
// Lambda 是匿名函数，可以捕获外部变量
// 语法：[捕获列表](参数列表) mutable -> 返回类型 { 函数体 }
// 各部分都可以省略（除了捕获列表 []）

void demo_lambda() {
    cout << "\n===== Lambda 表达式 =====" << endl;

    // 最简单的 Lambda：无参数，无捕获
    []() {
        cout << "Hello Lambda!" << endl;
    }();  // 立即调用（注意最后的 ()）

    // 带参数的 Lambda
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "add(3, 5) = " << add(3, 5) << endl;

    // ---- 捕获方式 ----
    int x = 10;
    int y = 20;

    // [x] 值捕获 x（拷贝一份，只读）
    auto f1 = [x]() {
        cout << "  值捕获 x = " << x << endl;
        // x++;  // 错误！值捕获默认不能修改
    };
    f1();

    // [&x] 引用捕获 x（可以修改原变量）
    auto f2 = [&x]() {
        x++;  // 修改的是外部的 x
        cout << "  引用捕获后 x = " << x << endl;
    };
    f2();
    cout << "  外部 x = " << x << endl;  // 11

    // [=] 值捕获所有外部变量
    auto f3 = [=]() {
        cout << "  [=] x=" << x << ", y=" << y << endl;
    };
    f3();

    // [&] 引用捕获所有外部变量
    auto f4 = [&]() {
        x += 100;
        y += 100;
    };
    f4();
    cout << "  [&] 后 x=" << x << ", y=" << y << endl;

    // [x, &y] 混合捕获：x 值捕获，y 引用捕获
    auto f5 = [x, &y]() {
        y += x;  // x 是副本，y 是引用
    };
    f5();

    // [=, &x] 默认值捕获，x 引用捕获
    auto f6 = [=, &x]() {
        x += y;  // x 是引用，y 是值
    };
    f6();

    // ---- mutable 关键字 ----
    // 值捕获的变量默认不能修改，加 mutable 可以修改副本
    int counter = 0;
    auto f7 = [counter]() mutable {
        counter++;           // 修改的是副本
        cout << "  mutable 副本 counter = " << counter << endl;
    };
    f7();
    f7();
    cout << "  外部 counter = " << counter << endl;  // 0（原变量不变）

    // ---- Lambda 在 STL 算法中的应用 ----
    vector<int> v = {5, 3, 1, 4, 2};

    // sort 中使用 Lambda 自定义排序（降序）
    sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
    });
    cout << "  降序排序：";
    for (auto val : v) cout << val << " ";
    cout << endl;

    // find_if 中使用 Lambda
    auto it = find_if(v.begin(), v.end(), [](int val) {
        return val > 3;
    });
    if (it != v.end()) {
        cout << "  第一个大于3的元素：" << *it << endl;
    }

    // for_each 中使用 Lambda
    cout << "  for_each：";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val * 10 << " ";
    });
    cout << endl;

    // ---- Lambda 作为变量保存 ----
    // 使用 std::function 保存 Lambda
    function<int(int, int)> multiply = [](int a, int b) {
        return a * b;
    };
    cout << "  multiply(4, 5) = " << multiply(4, 5) << endl;

    // ---- Lambda 的本质 ----
    // Lambda 本质上是编译器生成的匿名类，重载了 () 运算符
    // [x]() { cout << x; } 等价于：
    // class __Lambda {
    //     int x;
    // public:
    //     __Lambda(int x) : x(x) {}
    //     void operator()() const { cout << x; }
    // };
}

// ============================================================
// 7. 右值引用与移动语义
// ============================================================
// 左值：可以取地址的表达式（有名字的变量）
// 右值：不能取地址的表达式（临时值、字面量）
// 右值引用：&&，绑定到右值，延长临时对象生命周期
// 移动语义：窃取临时对象的资源，避免深拷贝

// 简单的字符串类，演示移动语义
class MyString {
public:
    char *m_data;
    int m_size;

    // 普通构造
    MyString(const char *str = "") {
        m_size = strlen(str);
        m_data = new char[m_size + 1];
        strcpy(m_data, str);
        cout << "    构造: " << m_data << endl;
    }

    // 拷贝构造（深拷贝：分配新内存，复制数据）
    MyString(const MyString &other) {
        m_size = other.m_size;
        m_data = new char[m_size + 1];      // 分配新内存
        strcpy(m_data, other.m_data);        // 复制数据
        cout << "    拷贝构造: " << m_data << " (深拷贝，慢)" << endl;
    }

    // 移动构造（窃取资源：不分配新内存，直接拿走指针）
    MyString(MyString &&other) noexcept {
        m_data = other.m_data;      // 窃取指针
        m_size = other.m_size;
        other.m_data = nullptr;     // 原对象置空（防止析构时释放）
        other.m_size = 0;
        cout << "    移动构造 (窃取资源，快)" << endl;
    }

    // 析构函数
    ~MyString() {
        if (m_data) {
            // cout << "    析构: " << m_data << endl;
            delete[] m_data;
        }
    }

    // 拼接操作（返回临时对象 = 右值）
    MyString operator+(const MyString &other) const {
        MyString result;
        delete[] result.m_data;
        result.m_size = m_size + other.m_size;
        result.m_data = new char[result.m_size + 1];
        strcpy(result.m_data, m_data);
        strcat(result.m_data, other.m_data);
        return result;  // 返回临时对象（右值）
    }

    void print() const {
        cout << (m_data ? m_data : "(null)") << " (size=" << m_size << ")" << endl;
    }
};

void demo_move_semantics() {
    cout << "\n===== 右值引用与移动语义 =====" << endl;

    // ---- 左值和右值 ----
    int a = 10;           // a 是左值，10 是右值
    int &r1 = a;          // 左值引用：绑定到左值
    // int &r2 = 10;      // 错误！左值引用不能绑定到右值
    int &&r2 = 10;        // 右值引用：绑定到右值
    // int &&r3 = a;      // 错误！右值引用不能绑定到左值

    cout << "a=" << a << ", r1=" << r1 << ", r2=" << r2 << endl;

    // 右值引用延长了临时对象的生命周期
    string &&s = string("hello");
    cout << "右值引用 string: " << s << endl;  // hello

    // ---- std::move ----
    // move 不移动任何东西，只是将左值转换为右值引用
    // 告诉编译器"我不再需要这个对象了，可以窃取它的资源"
    int mval = 42;
    int &&mref = std::move(mval);  // 将 mval 转换为右值引用
    cout << "move 后 mval=" << mval << ", mref=" << mref << endl;
    // 注意：move 后 mval 的值不确定，不要继续使用

    // ---- 移动构造 vs 拷贝构造 ----
    cout << "\n--- 拷贝构造 ---" << endl;
    MyString s1("hello");
    MyString s2(s1);               // 拷贝构造（s1 是左值）
    cout << "s2: "; s2.print();

    cout << "\n--- 移动构造 ---" << endl;
    MyString s3("world");
    MyString s4(std::move(s3));    // 移动构造（move 将 s3 转为右值引用）
    cout << "s4: "; s4.print();
    // s3 的 m_data 已经被置空！

    // ---- vector 中的移动语义 ----
    cout << "\n--- vector 中的移动语义 ---" << endl;
    vector<MyString> vec;
    vec.push_back(MyString("temp"));  // 移动构造（临时对象是右值）
    cout << "vec[0]: "; vec[0].print();

    MyString s5("keep");
    vec.push_back(std::move(s5));     // 移动构造（move 转换）
    cout << "vec[1]: "; vec[1].print();

    // 【重要】move 后的对象状态不确定，不要继续使用！
    // s5 此时 m_data 为 nullptr
}

// ============================================================
// 8. enum class 强类型枚举
// ============================================================
// 传统枚举的问题：会隐式转换为 int，不同枚举可能有同名值
// enum class：强作用域，不隐式转换，避免命名冲突

enum class Color { Red, Green, Blue };
enum class TrafficLight { Red, Yellow, Green };  // 不会冲突！

// 传统枚举（有隐患）
enum OldColor { OC_Red, OC_Green, OC_Blue };
enum OldTrafficLight { OTL_Red, OTL_Yellow, OTL_Green };
// 如果两个枚举在同一作用域且有同名值，会冲突！

void demo_enum_class() {
    cout << "\n===== enum class 强类型枚举 =====" << endl;

    Color c = Color::Red;          // 必须加作用域
    TrafficLight t = TrafficLight::Red;  // 不会冲突

    // int x = c;                  // 错误！不能隐式转换为 int
    int x = static_cast<int>(c);   // 正确！显式转换
    cout << "Color::Red = " << x << endl;  // 0

    // 用于 switch
    switch (c) {
        case Color::Red:
            cout << "颜色：红色" << endl;
            break;
        case Color::Green:
            cout << "颜色：绿色" << endl;
            break;
        case Color::Blue:
            cout << "颜色：蓝色" << endl;
            break;
    }

    // 【建议】C++11 以后统一使用 enum class，不要再用传统 enum
}

// ============================================================
// 9. 原始字符串 R"()"
// ============================================================
// 普通字符串需要转义反斜杠等字符
// 原始字符串不处理转义，原样输出

void demo_raw_string() {
    cout << "\n===== 原始字符串 =====" << endl;

    // 普通字符串需要转义
    string s1 = "C:\\Users\\test\\file.txt";
    string s2 = "hello\nworld";
    string s3 = "SELECT * FROM users WHERE name = \"张三\"";

    cout << "普通字符串：" << endl;
    cout << "  " << s1 << endl;
    cout << "  " << s2 << endl;
    cout << "  " << s3 << endl;

    // 原始字符串（不处理转义）
    string r1 = R"(C:\Users\test\file.txt)";
    string r2 = R"(hello\nworld)";        // \n 不是换行，是两个字符
    string r3 = R"(SELECT * FROM users WHERE name = "张三")";

    cout << "原始字符串：" << endl;
    cout << "  " << r1 << endl;
    cout << "  " << r2 << endl;
    cout << "  " << r3 << endl;

    // 如果字符串内容包含 )"，可以用自定义分隔符
    string r4 = R"delim(He said "hello)" and left)delim";
    cout << "  " << r4 << endl;

    // 【适用场景】文件路径、正则表达式、SQL 语句、JSON 等含大量特殊字符的字符串
}

// ============================================================
// 10. std::function 和 std::bind
// ============================================================
// std::function：通用函数包装器，可以保存任何可调用对象
// std::bind：绑定参数，生成新的可调用对象

// 普通函数
int add_func(int a, int b) { return a + b; }
int multiply_func(int a, int b) { return a * b; }

// 函数对象（仿函数）
struct Divide {
    int operator()(int a, int b) const {
        return b != 0 ? a / b : 0;
    }
};

void demo_function_bind() {
    cout << "\n===== std::function 和 std::bind =====" << endl;

    // ---- std::function：通用函数包装器 ----
    function<int(int, int)> func;

    // 保存普通函数
    func = add_func;
    cout << "add_func(3, 4) = " << func(3, 4) << endl;  // 7

    // 保存 Lambda
    func = [](int a, int b) { return a * b; };
    cout << "Lambda multiply(3, 4) = " << func(3, 4) << endl;  // 12

    // 保存函数对象
    func = Divide();
    cout << "Divide(10, 3) = " << func(10, 3) << endl;  // 3

    // 保存成员函数需要 bind（见下面）

    // ---- std::bind：绑定参数 ----
    // 绑定第一个参数为 5，生成新函数 add5
    auto add5 = std::bind(add_func, 5, std::placeholders::_1);
    cout << "add5(10) = " << add5(10) << endl;  // 15 (5+10)

    // 绑定两个参数
    auto add_3_4 = std::bind(add_func, 3, 4);
    cout << "add_3_4() = " << add_3_4() << endl;  // 7

    // 改变参数顺序
    auto subtract = [](int a, int b) { return a - b; };
    auto reverse_sub = std::bind(subtract, std::placeholders::_2, std::placeholders::_1);
    cout << "subtract(10, 3) = " << subtract(10, 3) << endl;     // 7
    cout << "reverse_sub(10, 3) = " << reverse_sub(10, 3) << endl; // -3 (3-10)

    // ---- function 用于回调 ----
    // 一个接受回调函数的高阶函数
    auto apply = [](function<int(int, int)> op, int a, int b) {
        return op(a, b);
    };

    cout << "apply(add, 5, 3) = " << apply(add_func, 5, 3) << endl;
    cout << "apply(mul, 5, 3) = " << apply(multiply_func, 5, 3) << endl;
    cout << "apply(lambda, 5, 3) = " << apply([](int a, int b) { return a % b; }, 5, 3) << endl;

    // 【总结】
    // std::function：统一保存各种可调用对象（函数、Lambda、仿函数）
    // std::bind：固定某些参数，生成新的可调用对象
    // C++11 中 Lambda 通常比 bind 更好用、更清晰
}

// ============================================================
// 11. 初始化列表 initializer_list
// ============================================================
// 允许函数或构造函数接受 {} 列表作为参数

class MyVector {
private:
    vector<int> m_data;

public:
    // 接受初始化列表的构造函数
    MyVector(initializer_list<int> list) {
        for (auto it = list.begin(); it != list.end(); it++) {
            m_data.push_back(*it);
        }
        cout << "    MyVector 构造，元素数：" << m_data.size() << endl;
    }

    void print() const {
        cout << "    [";
        for (size_t i = 0; i < m_data.size(); i++) {
            if (i > 0) cout << ", ";
            cout << m_data[i];
        }
        cout << "]" << endl;
    }
};

// 接受可变数量参数的函数
void print_list(initializer_list<string> list) {
    for (auto it = list.begin(); it != list.end(); it++) {
        cout << "  " << *it << endl;
    }
}

void demo_initializer_list() {
    cout << "\n===== 初始化列表 initializer_list =====" << endl;

    // 使用初始化列表构造自定义容器
    MyVector v = {1, 2, 3, 4, 5};
    v.print();

    // 传递初始化列表给函数
    print_list({"hello", "world", "cpp11"});

    // STL 容器都支持初始化列表
    vector<int> vec = {10, 20, 30};
    map<string, int> m = {{"a", 1}, {"b", 2}};
    list<int> l = {100, 200, 300};

    cout << "vector: ";
    for (auto x : vec) cout << x << " ";
    cout << endl;

    cout << "map: ";
    for (auto &p : m) cout << p.first << "=" << p.second << " ";
    cout << endl;
}

// ============================================================
// main 函数：运行所有演示
// ============================================================

int main() {
    cout << "============================================" << endl;
    cout << "       C++11 新特性详解 - 完整演示" << endl;
    cout << "============================================" << endl;

    demo_auto();             // 1. auto 自动类型推导
    demo_decltype();         // 2. decltype 类型推导
    demo_nullptr();          // 3. nullptr 替代 NULL
    demo_range_for();        // 4. 范围 for 循环
    demo_list_init();        // 5. 列表初始化 {}
    demo_lambda();           // 6. Lambda 表达式
    demo_move_semantics();   // 7. 右值引用与移动语义
    demo_enum_class();       // 8. enum class 强类型枚举
    demo_raw_string();       // 9. 原始字符串 R"()"
    demo_function_bind();    // 10. std::function 和 std::bind
    demo_initializer_list(); // 11. 初始化列表

    cout << "\n============================================" << endl;
    cout << "       所有 C++11 特性演示完成！" << endl;
    cout << "============================================" << endl;

    return 0;
}

/*
 * ============================================================
 *  C++11 新特性总结
 * ================================================= *
 *
 *  特性                  | 关键字/语法          | 用途
 *  ---------------------|---------------------|---------------------------
 *  auto                 | auto x = 10;        | 自动类型推导，简化代码
 *  decltype             | decltype(x) y;      | 根据表达式推导类型
 *  nullptr              | int *p = nullptr;   | 替代 NULL，避免歧义
 *  范围 for             | for (auto x : v)    | 简洁遍历容器
 *  列表初始化            | int a{10};          | 防止窄化转换
 *  Lambda               | [](){}              | 匿名函数，简化回调
 *  右值引用             | int &&r = 10;       | 移动语义，避免深拷贝
 *  std::move            | std::move(x)        | 将左值转为右值引用
 *  enum class           | enum class X {};    | 强类型枚举，避免冲突
 *  原始字符串            | R"(text)"           | 不转义的字符串
 *  std::function        | function<int(int)>  | 通用函数包装器
 *  std::bind            | bind(f, 1, _1)      | 绑定参数
 *  initializer_list     | {1, 2, 3}           | 初始化列表
 *
 *  【学习建议】
 *  - auto、范围 for、Lambda 是最常用的，必须熟练掌握
 *  - 移动语义是性能优化的关键，理解原理很重要
 *  - nullptr 和 enum class 是最佳实践，新代码应统一使用
 *  - std::function 在需要回调或策略模式时很有用
 * ============================================================
 */
