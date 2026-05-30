/*
 ============================================================================
 文件名 : 68_多态案例与底层原理.cpp
 描  述 : 详细讲解C++多态的完整案例与底层实现原理
          包括：计算器多态案例、虚函数表(vtable)概念、
          虚表指针(vptr)、虚函数调用过程、对象切片问题、
          dynamic_cast 和 typeid 的初步介绍
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 68_多态案例与底层原理.cpp -o 68_多态案例与底层原理
 ============================================================================
*/

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
using namespace std;

// ==================== 1. 多态计算器案例 ====================
// 利用多态实现一个灵活的计算器框架

// 抽象基类：运算类
class AbstractCalculator {
public:
    double m_a;  // 操作数1
    double m_b;  // 操作数2

    AbstractCalculator(double a = 0, double b = 0)
        : m_a(a), m_b(b) {}

    // 纯虚函数：获取运算结果
    virtual double getResult() const = 0;

    // 纯虚函数：获取运算符号
    virtual string getSymbol() const = 0;

    // 虚析构函数
    virtual ~AbstractCalculator() = default;

    // 普通成员函数
    void showResult() const {
        cout << m_a << " " << getSymbol() << " " << m_b
             << " = " << getResult() << endl;
    }
};

// 加法计算器
class AddCalculator : public AbstractCalculator {
public:
    AddCalculator(double a, double b) : AbstractCalculator(a, b) {}
    double getResult() const override { return m_a + m_b; }
    string getSymbol() const override { return "+"; }
};

// 减法计算器
class SubCalculator : public AbstractCalculator {
public:
    SubCalculator(double a, double b) : AbstractCalculator(a, b) {}
    double getResult() const override { return m_a - m_b; }
    string getSymbol() const override { return "-"; }
};

// 乘法计算器
class MulCalculator : public AbstractCalculator {
public:
    MulCalculator(double a, double b) : AbstractCalculator(a, b) {}
    double getResult() const override { return m_a * m_b; }
    string getSymbol() const override { return "*"; }
};

// 除法计算器
class DivCalculator : public AbstractCalculator {
public:
    DivCalculator(double a, double b) : AbstractCalculator(a, b) {}
    double getResult() const override {
        if (m_b == 0) {
            cout << "错误：除数不能为0！" << endl;
            return 0;
        }
        return m_a / m_b;
    }
    string getSymbol() const override { return "/"; }
};

// ==================== 2. 虚函数表（vtable）原理讲解 ====================
//
// C++多态的底层实现依赖于虚函数表机制：
//
// 【虚函数表 vtable】
// - 每个包含虚函数的类（而非对象）都有一个虚函数表
// - 虚函数表是一个函数指针数组，存放该类所有虚函数的地址
// - 虚函数表在编译时创建，存放在只读数据段（常量区）
//
// 【虚表指针 vptr】
// - 每个包含虚函数的类的对象都有一个隐藏的虚表指针（vptr）
// - vptr 指向该类的虚函数表
// - vptr 在构造函数中被初始化为指向当前类的 vtable
// - vptr 通常是对象的第一个成员（取决于编译器实现）
//
// 【虚函数调用过程】
// 1. 通过基类指针/引用访问对象
// 2. 通过对象的 vptr 找到对应的虚函数表
// 3. 在虚函数表中找到对应偏移位置的函数地址
// 4. 调用该函数
//
// 【vtable 内存布局示意】
//
//   Shape vtable:           Circle vtable:
//   +------------------+    +------------------+
//   | [0] ~Shape()     |    | [0] ~Circle()    |
//   | [1] area()=0     |    | [1] Circle::area |
//   | [2] perimeter()=0|    | [2] Circle::perim|
//   | [3] draw()=0     |    | [3] Circle::draw |
//   +------------------+    +------------------+
//
//   Circle 对象内存:
//   +--------+--------+
//   | vptr   | radius |   vptr -> Circle vtable
//   +--------+--------+
//
//   sizeof 含虚函数的类会多一个 vptr 的大小（通常8字节，64位系统）

// 演示 vtable 的类层次
class Animal {
public:
    virtual void speak() const { cout << "Animal: ..." << endl; }
    virtual void move() const { cout << "Animal: moving..." << endl; }
    virtual ~Animal() = default;

    // 非虚函数——不经过虚函数表
    void eat() const { cout << "Animal: eating..." << endl; }
};

class Dog : public Animal {
public:
    void speak() const override { cout << "Dog: 汪汪汪!" << endl; }
    void move() const override { cout << "Dog: 跑跑跑!" << endl; }
};

class Cat : public Animal {
public:
    void speak() const override { cout << "Cat: 喵喵喵!" << endl; }
    void move() const override { cout << "Cat: 走走走!" << endl; }
};

class Bird : public Animal {
public:
    void speak() const override { cout << "Bird: 叽叽叽!" << endl; }
    void move() const override { cout << "Bird: 飞飞飞!" << endl; }
};

// ==================== 3. 对象切片问题 ====================
// 当派生类对象赋值给基类对象（值传递）时，
// 派生类特有的部分会被"切掉"，这就是对象切片（Object Slicing）

class Base {
public:
    int m_base;
    Base(int b = 0) : m_base(b) {}
    virtual void show() const {
        cout << "Base::show(), m_base = " << m_base << endl;
    }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    int m_derived;
    Derived(int b = 0, int d = 0) : Base(b), m_derived(d) {}
    void show() const override {
        cout << "Derived::show(), m_base = " << m_base
             << ", m_derived = " << m_derived << endl;
    }
};

// 值传递——发生对象切片
void callByValue(Base obj) {
    cout << "  值传递: ";
    obj.show();  // 永远调用 Base::show()
}

// 引用传递——不切片，保持多态
void callByRef(const Base& obj) {
    cout << "  引用传递: ";
    obj.show();  // 调用实际类型的 show()
}

// 指针传递——不切片，保持多态
void callByPtr(const Base* obj) {
    cout << "  指针传递: ";
    obj->show();  // 调用实际类型的 show()
}

// ==================== 4. dynamic_cast 和 typeid ====================
// dynamic_cast: 安全的向下转型（运行时检查）
// typeid: 获取对象的类型信息

class Shape {
public:
    virtual ~Shape() = default;
    virtual string getType() const = 0;
};

class Circle : public Shape {
public:
    double radius;
    Circle(double r) : radius(r) {}
    string getType() const override { return "圆形"; }
    double getArea() const { return 3.14159 * radius * radius; }
};

class Rectangle : public Shape {
public:
    double width, height;
    Rectangle(double w, double h) : width(w), height(h) {}
    string getType() const override { return "矩形"; }
    double getArea() const { return width * height; }
};

// ==================== 5. 演示函数 ====================

void demonstrateCalculator() {
    cout << "\n===== 1. 多态计算器演示 =====" << endl;
    double a = 10, b = 3;
    AbstractCalculator* calcs[4];
    calcs[0] = new AddCalculator(a, b);
    calcs[1] = new SubCalculator(a, b);
    calcs[2] = new MulCalculator(a, b);
    calcs[3] = new DivCalculator(a, b);

    for (int i = 0; i < 4; i++) {
        calcs[i]->showResult();  // 多态调用
    }
    for (int i = 0; i < 4; i++) {
        delete calcs[i];
    }
}

void demonstrateVtable() {
    cout << "\n===== 2. 虚函数表(vtable)原理 =====" << endl;
    cout << "【虚函数表机制】" << endl;
    cout << "  1. 每个含虚函数的类有一个虚函数表(vtable)" << endl;
    cout << "  2. 每个对象有一个虚表指针(vptr)指向vtable" << endl;
    cout << "  3. 调用虚函数时: obj -> vptr -> vtable -> 函数地址" << endl;

    // 通过 sizeof 观察 vptr 带来的额外开销
    cout << "\nsizeof(Animal) = " << sizeof(Animal) << " 字节 (含vptr)" << endl;
    cout << "sizeof(Dog)    = " << sizeof(Dog) << " 字节" << endl;

    // 演示动态绑定
    cout << "\n【虚函数调用过程演示】" << endl;
    Dog dog;
    Cat cat;
    Bird bird;
    Animal* p;

    p = &dog;
    cout << "p = &dog:  ";
    p->speak();  // 通过 dog 的 vptr 找到 Dog::speak()

    p = &cat;
    cout << "p = &cat:  ";
    p->speak();  // 通过 cat 的 vptr 找到 Cat::speak()

    p = &bird;
    cout << "p = &bird: ";
    p->speak();  // 通过 bird 的 vptr 找到 Bird::speak()

    // 非虚函数 vs 虚函数
    cout << "\n【非虚函数 vs 虚函数】" << endl;
    Animal* pa = &dog;
    pa->eat();     // 非虚函数：编译时确定调用 Animal::eat()
    pa->speak();   // 虚函数：运行时确定调用 Dog::speak()
}

void demonstrateObjectSlicing() {
    cout << "\n===== 3. 对象切片(Object Slicing)问题 =====" << endl;
    Derived d(10, 20);
    cout << "原始派生类对象: ";
    d.show();

    // 赋值给基类对象——切片发生
    Base b = d;
    cout << "赋值给基类对象（切片）: ";
    b.show();  // m_derived 丢失

    // 使用引用/指针避免切片
    cout << "\n引用传递（无切片）:" << endl;
    callByRef(d);
    cout << "指针传递（无切片）:" << endl;
    callByPtr(&d);
    cout << "值传递（切片）:" << endl;
    callByValue(d);

    cout << "\n【总结】" << endl;
    cout << "  赋值给基类对象 => 切片，丢失派生类数据" << endl;
    cout << "  使用基类指针/引用 => 无切片，保持完整" << endl;
}

void demonstrateCastAndTypeid() {
    cout << "\n===== 4. dynamic_cast 和 typeid =====" << endl;
    Shape* shapes[3];
    shapes[0] = new Circle(5.0);
    shapes[1] = new Rectangle(4.0, 6.0);
    shapes[2] = new Circle(3.0);

    for (int i = 0; i < 3; i++) {
        cout << "\nshapes[" << i << "]:" << endl;
        // typeid 获取类型信息
        cout << "  typeid: " << typeid(*shapes[i]).name() << endl;
        cout << "  类型: " << shapes[i]->getType() << endl;

        // dynamic_cast 安全向下转型
        Circle* cp = dynamic_cast<Circle*>(shapes[i]);
        if (cp != nullptr) {
            cout << "  这是一个圆形, 面积 = " << cp->getArea() << endl;
        }
        Rectangle* rp = dynamic_cast<Rectangle*>(shapes[i]);
        if (rp != nullptr) {
            cout << "  这是一个矩形, 面积 = " << rp->getArea() << endl;
        }
    }

    for (int i = 0; i < 3; i++) {
        delete shapes[i];
    }

    // dynamic_cast 转型失败示例
    cout << "\n【dynamic_cast 转型失败】" << endl;
    Shape* s = new Circle(1.0);
    Rectangle* rp = dynamic_cast<Rectangle*>(s);
    if (rp == nullptr) {
        cout << "  将圆形转为矩形: 失败, 返回 nullptr" << endl;
    }
    delete s;

    cout << "\n【dynamic_cast 使用注意】" << endl;
    cout << "  - 只能用于含有虚函数的类（多态类型）" << endl;
    cout << "  - 转型失败时: 指针返回 nullptr, 引用抛出 bad_cast" << endl;
    cout << "  - 比 static_cast 更安全，但有运行时开销" << endl;
}

// ==================== 6. 主函数 ====================
int main() {
    cout << "========================================" << endl;
    cout << "   多态案例与底层原理 详解演示" << endl;
    cout << "========================================" << endl;

    // 6.1 多态计算器
    demonstrateCalculator();

    // 6.2 虚函数表原理
    demonstrateVtable();

    // 6.3 对象切片问题
    demonstrateObjectSlicing();

    // 6.4 dynamic_cast 和 typeid
    demonstrateCastAndTypeid();

    cout << "\n========================================" << endl;
    cout << "  总结:" << endl;
    cout << "  1. 多态通过 vtable/vptr 实现动态绑定" << endl;
    cout << "  2. 每个含虚函数的类有一个 vtable" << endl;
    cout << "  3. 每个对象有一个 vptr 指向 vtable" << endl;
    cout << "  4. 值传递导致对象切片, 用指针/引用避免" << endl;
    cout << "  5. dynamic_cast 提供安全的向下转型" << endl;
    cout << "  6. typeid 获取运行时类型信息" << endl;
    cout << "========================================" << endl;

    return 0;
}
