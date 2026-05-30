/*
 * ============================================================================
 * 52_构造函数详解.cpp
 * ============================================================================
 *
 * 本文件详细讲解 C++ 中构造函数(Constructor)的各种用法。
 * 构造函数是对象创建时自动调用的特殊成员函数，用于初始化对象。
 *
 * 知识点：
 *   1. 构造函数语法：与类同名，无返回值
 *   2. 默认构造函数（无参数）
 *   3. 参数化构造函数（有参数）
 *   4. 构造函数重载（多个版本）
 *   5. 编译器提供的默认构造函数
 *   6. 拷贝构造函数简介
 *
 * 编译命令：g++ -std=c++11 52_构造函数详解.cpp -o 52_constructor
 * ============================================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ===========================================================================
// 第一部分：构造函数的基本语法
// ===========================================================================
// 构造函数的特点：
//   1. 函数名与类名完全相同
//   2. 没有返回值（连 void 都不写）
//   3. 在对象创建时由编译器自动调用，无需手动调用
//   4. 可以重载（可以有多个不同参数的版本）
//   5. 通常用于初始化成员变量

class Student
{
private:
    string m_name;
    int m_age;
    double m_score;

public:
    // ---- 默认构造函数（无参数） ----
    // 当使用 Student s; 这种不带参数的方式创建对象时调用
    Student()
    {
        m_name = "未命名";
        m_age = 0;
        m_score = 0.0;
        cout << "  [调用] 默认构造函数 -> " << m_name << endl;
    }

    // ---- 参数化构造函数（带参数） ----
    // 当使用 Student s("张三", 20); 创建对象时调用
    Student(const string &name, int age)
    {
        m_name = name;
        m_age = age;
        m_score = 0.0;
        cout << "  [调用] 双参数构造函数 -> " << m_name << endl;
    }

    // ---- 全参数构造函数 ----
    // 当使用 Student s("张三", 20, 95.5); 创建对象时调用
    Student(const string &name, int age, double score)
    {
        m_name = name;
        m_age = age;
        m_score = score;
        cout << "  [调用] 全参数构造函数 -> " << m_name << endl;
    }

    void show() const
    {
        cout << "  " << m_name << "，" << m_age << "岁，"
             << m_score << "分" << endl;
    }
};

// ===========================================================================
// 第二部分：构造函数重载
// ===========================================================================
// 和普通函数一样，构造函数也可以重载。
// 重载条件：参数个数不同，或参数类型不同。

class Rectangle
{
private:
    double m_width;
    double m_height;

public:
    // 重载版本1：无参数，默认 1x1
    Rectangle() : m_width(1.0), m_height(1.0)
    {
        cout << "  [默认] 1x1" << endl;
    }

    // 重载版本2：单参数，正方形
    Rectangle(double side) : m_width(side), m_height(side)
    {
        cout << "  [正方形] " << side << "x" << side << endl;
    }

    // 重载版本3：双参数，长方形
    Rectangle(double w, double h) : m_width(w), m_height(h)
    {
        cout << "  [长方形] " << w << "x" << h << endl;
    }

    double area() const { return m_width * m_height; }
    double perimeter() const { return 2 * (m_width + m_height); }

    void show() const
    {
        cout << "  宽=" << m_width << "，高=" << m_height
             << "，面积=" << area() << "，周长=" << perimeter() << endl;
    }
};

// ===========================================================================
// 第三部分：编译器提供的默认构造函数
// ===========================================================================
// 如果我们不写任何构造函数，编译器会自动提供一个空的默认构造函数。
// 但如果我们自己写了任何一个构造函数，编译器就不再提供了！

class AutoClass
{
public:
    int value;
    string name;
    // 没有自定义任何构造函数
    // 编译器自动生成：AutoClass() {} （空的，什么都不初始化）
};

class ManualClass
{
public:
    int value;
    string name;

    // 自定义了带参数的构造函数
    ManualClass(int v) : value(v) {}

    // 注意：由于自定义了构造函数，编译器不再提供默认构造
    // 如果需要无参创建，必须手动写一个：
    // ManualClass() : value(0) {}
};

// ===========================================================================
// 第四部分：拷贝构造函数简介
// ===========================================================================
// 拷贝构造函数用于从一个已有的对象创建一个新的对象。
// 如果不写，编译器会提供一个默认的（逐字节复制，即浅拷贝）。

class Box
{
private:
    double m_length;
    double m_width;
    double m_height;
    int *m_data;   // 指针成员（用于演示深拷贝）

public:
    // 默认构造
    Box() : m_length(0), m_width(0), m_height(0), m_data(nullptr)
    {
        cout << "  [默认构造] Box" << endl;
    }

    // 参数化构造
    Box(double l, double w, double h) : m_length(l), m_width(w), m_height(h)
    {
        m_data = new int(100);  // 动态分配内存
        cout << "  [参数构造] " << l << "x" << w << "x" << h << endl;
    }

    // 拷贝构造函数（深拷贝）
    // 参数必须是 const 引用，否则会无限递归
    Box(const Box &other)
        : m_length(other.m_length), m_width(other.m_width), m_height(other.m_height)
    {
        // 深拷贝：分配新内存，复制内容
        if (other.m_data)
            m_data = new int(*other.m_data);
        else
            m_data = nullptr;
        cout << "  [拷贝构造] 深拷贝完成" << endl;
    }

    // 析构函数
    ~Box()
    {
        delete m_data;
        m_data = nullptr;
    }

    double volume() const { return m_length * m_width * m_height; }
    void show() const
    {
        cout << "  Box(" << m_length << "x" << m_width << "x" << m_height
             << ")，体积=" << volume() << endl;
    }
};

// ===========================================================================
// 第五部分：匿名对象
// ===========================================================================
// 匿名对象是没有名字的临时对象，创建后立即使用，用完即销毁。

class Temp
{
public:
    Temp() { cout << "  [构造] Temp 默认" << endl; }
    Temp(int x) { cout << "  [构造] Temp(" << x << ")" << endl; }
    ~Temp() { cout << "  [析构] ~Temp" << endl; }
    void show() const { cout << "  Temp::show()" << endl; }
};

// ===========================================================================
// 第六部分：构造函数调用的注意事项
// ===========================================================================

class NoteClass
{
public:
    int value;

    // 默认构造
    NoteClass() : value(0) { cout << "  [默认构造] value=0" << endl; }

    // 带参构造
    NoteClass(int v) : value(v) { cout << "  [带参构造] value=" << v << endl; }

    // 拷贝构造
    NoteClass(const NoteClass &other) : value(other.value)
    {
        cout << "  [拷贝构造] value=" << value << endl;
    }
};

// 注意：下面这种写法不是创建对象，而是函数声明！
// NoteClass obj();  // 这是声明一个返回 NoteClass 的函数，名为 obj
// 这叫做"最令人烦恼的解析"(Most Vexing Parse)

// ===========================================================================
// 主函数
// ===========================================================================
int main()
{
    cout << "============================================" << endl;
    cout << "            C++ 构造函数详解" << endl;
    cout << "============================================" << endl;

    // ---- 第一部分：构造函数基本语法 ----
    cout << "\n===== 第一部分：构造函数基本语法 =====" << endl;

    // 调用默认构造函数（无参数）
    cout << "--- 创建 s1（默认构造） ---" << endl;
    Student s1;
    s1.show();

    // 调用双参数构造函数
    cout << "\n--- 创建 s2（双参数构造） ---" << endl;
    Student s2("张三", 20);
    s2.show();

    // 调用全参数构造函数
    cout << "\n--- 创建 s3（全参数构造） ---" << endl;
    Student s3("李四", 19, 95.5);
    s3.show();

    // 隐式调用构造函数（使用花括号初始化）
    cout << "\n--- 创建 s4（隐式调用） ---" << endl;
    Student s4 = {"王五", 21, 88.0};
    s4.show();

    // ---- 第二部分：构造函数重载 ----
    cout << "\n===== 第二部分：构造函数重载 =====" << endl;

    Rectangle r1;          // 调用默认构造
    r1.show();

    Rectangle r2(5.0);     // 调用单参数构造（正方形）
    r2.show();

    Rectangle r3(3.0, 4.0); // 调用双参数构造（长方形）
    r3.show();

    // ---- 第三部分：编译器提供的默认构造 ----
    cout << "\n===== 第三部分：编译器默认构造 =====" << endl;

    // AutoClass 没有自定义构造函数，编译器提供空的默认构造
    AutoClass ac;
    ac.value = 42;
    ac.name = "自动的";
    cout << "AutoClass: value=" << ac.value << ", name=" << ac.name << endl;

    // ManualClass 自定义了带参构造
    ManualClass mc(100);
    cout << "ManualClass: value=" << mc.value << endl;

    // ManualClass mc2;  // 编译错误！没有默认构造函数可用

    // 规则总结：
    // 1. 不写任何构造函数 -> 编译器提供默认构造（空）+ 拷贝构造（浅拷贝）+ 析构
    // 2. 写了任何构造函数 -> 编译器不再提供默认构造
    // 3. 编译器的拷贝构造是浅拷贝（逐字节复制指针值）

    // ---- 第四部分：拷贝构造函数 ----
    cout << "\n===== 第四部分：拷贝构造函数 =====" << endl;

    Box b1(3, 4, 5);
    b1.show();

    // 方式1：显式调用拷贝构造
    cout << "--- 拷贝构造 b2 ---" << endl;
    Box b2(b1);
    b2.show();

    // 方式2：等价写法
    cout << "--- 拷贝构造 b3 ---" << endl;
    Box b3 = b1;
    b3.show();

    // 拷贝构造的触发时机：
    // 1. 用一个对象初始化另一个对象（如上）
    // 2. 函数参数按值传递
    // 3. 函数返回值按值返回

    // ---- 第五部分：匿名对象 ----
    cout << "\n===== 第五部分：匿名对象 =====" << endl;

    // 匿名对象：创建后立即使用，当前行结束后立即销毁
    Temp(42).show();
    cout << "--- 匿名对象已析构 ---" << endl;

    // 对比有名对象
    Temp t(100);
    t.show();
    // t 在 main 函数结束时才析构

    // ---- 第六部分：注意事项 ----
    cout << "\n===== 第六部分：常见错误与注意事项 =====" << endl;

    // 错误1：创建对象时加括号会被解析为函数声明
    // NoteClass obj();   // 这是函数声明，不是创建对象！
    // 正确写法：
    NoteClass obj;       // 不加括号
    NoteClass obj2(10);  // 加参数

    // 错误2：忘记默认构造函数
    // 如果类有带参构造但没有默认构造，就不能用无参方式创建

    // 错误3：构造函数写了返回值类型
    // void Student() {}  // 这不是构造函数，是普通函数！

    cout << endl;
    return 0;
}
