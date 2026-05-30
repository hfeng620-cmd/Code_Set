/*
 * 文件名: 64_继承中的同名成员.cpp
 * 描述: 继承中同名成员的处理规则
 * 知识点:
 *   1. 派生类中定义与基类同名的成员
 *   2. 派生类隐藏基类成员(成员隐藏/遮蔽)
 *   3. 使用 Base::member 显式访问基类版本
 *   4. 函数隐藏规则
 *   5. 静态成员的隐藏
 * 编译: g++ -std=c++11 64_继承中的同名成员.cpp -o 64_继承中的同名成员
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 一、同名成员变量
// ============================================================
// 当派生类和基类有同名成员变量时:
//   - 派生类对象中同时存在两个同名变量(基类的和派生类的)
//   - 默认情况下,派生类中的同名成员会"隐藏"基类的同名成员
//   - 使用 基类名::成员名 可以显式访问基类的成员
// ============================================================

class Base1 {
public:
    int value;
    Base1(int v = 10) : value(v) {
        cout << "  [Base1] 构造, value = " << value << endl;
    }
};

class Derived1 : public Base1 {
public:
    int value;  // 与基类同名的成员变量

    Derived1(int v1, int v2) : Base1(v1), value(v2) {
        cout << "  [Derived1] 构造, value = " << value << endl;
    }

    void showValues() const {
        // 直接访问 value,得到的是派生类的 value
        cout << "  派生类 value = " << value << endl;
        // 使用作用域运算符访问基类的 value
        cout << "  基类 Base1::value = " << Base1::value << endl;
    }
};

// ============================================================
// 二、同名成员函数 - 函数隐藏
// ============================================================
// 当派生类定义了与基类同名的函数时,会发生"函数隐藏":
//   - 只要函数名相同,不管参数列表是否相同,都会隐藏基类函数
//   - 这与函数重载不同!函数重载要求在同一作用域
//   - 使用 Base::func() 可以显式调用基类版本
// ============================================================

class Printer {
public:
    // 基类的 print 函数(无参数)
    void print() const {
        cout << "  [Printer::print()] 无参数版本" << endl;
    }

    // 基类的 print 函数(带一个参数)
    void print(const string& msg) const {
        cout << "  [Printer::print(string)] " << msg << endl;
    }

    // 基类的 print 函数(带两个参数)
    void print(const string& msg, int count) const {
        cout << "  [Printer::print(string, int)] ";
        for (int i = 0; i < count; ++i) {
            cout << msg << " ";
        }
        cout << endl;
    }
};

class ColorPrinter : public Printer {
public:
    // 派生类定义了同名函数 print(隐藏了基类的所有 print)
    void print(const string& msg) const {
        cout << "  [ColorPrinter::print(string)] 彩色打印: " << msg << endl;
    }

    // 如果想使用基类的 print,需要显式调用
    void printAll(const string& msg) const {
        // 调用基类的无参版本
        Printer::print();
        // 调用基类的带参版本
        Printer::print(msg);
        // 调用派生类自己的版本
        print(msg);
    }
};

// ============================================================
// 三、函数隐藏的详细规则
// ============================================================
// 规则1: 只要函数名相同,就会隐藏(不管参数列表)
// 规则2: 隐藏不是删除,基类函数仍然存在,只是不能直接访问
// 规则3: 使用 using 声明可以把基类函数引入派生类作用域
// 规则4: 重载只发生在同一作用域中
// ============================================================

class MathBase {
public:
    // 基类有三个重载版本
    int calculate(int a, int b) {
        cout << "  [MathBase] int 版本: ";
        return a + b;
    }

    double calculate(double a, double b) {
        cout << "  [MathBase] double 版本: ";
        return a + b;
    }

    int calculate(int a, int b, int c) {
        cout << "  [MathBase] 三参数版本: ";
        return a + b + c;
    }
};

class MathDerived : public MathBase {
public:
    // 定义同名函数,隐藏了基类的所有 calculate
    int calculate(int a, int b) {
        cout << "  [MathDerived] 重写版本: ";
        return a * b;  // 改为乘法
    }

    // 使用 using 声明,将基类的 calculate 引入派生类
    // 这样基类的其他重载版本就不会被隐藏了
    using MathBase::calculate;
};

// ============================================================
// 四、同名静态成员
// ============================================================
// 静态成员的隐藏规则与非静态成员相同
// 可以通过 类名::成员 或 对象.成员 访问
// ============================================================

class CounterBase {
public:
    static int count;
    CounterBase() { ++count; }
    static void showCount() {
        cout << "  [CounterBase] count = " << count << endl;
    }
};
int CounterBase::count = 0;

class CounterDerived : public CounterBase {
public:
    static int count;  // 同名静态成员,隐藏基类的
    CounterDerived() { ++count; }
    static void showCount() {
        cout << "  [CounterDerived] count = " << count << endl;
        // 访问基类的静态成员
        cout << "  [CounterBase] count = " << CounterBase::count << endl;
    }
};
int CounterDerived::count = 0;

// ============================================================
// 五、指针和引用的特殊情况
// ============================================================
// 当使用基类指针/引用指向派生类对象时:
//   - 对于普通函数(非虚函数),调用的是基类版本
//   - 对于虚函数(virtual),调用的是派生类版本
//   - 这就是多态的基础(将在下一节详细介绍)
// ============================================================

class Animal2 {
public:
    string type;
    Animal2(string t = "动物") : type(t) {}

    // 普通函数(非虚函数)
    void speak() const {
        cout << "  [" << type << "]: 普通叫声" << endl;
    }

    // 虚函数
    virtual void move() const {
        cout << "  [" << type << "]: 普通移动" << endl;
    }
};

class Cat2 : public Animal2 {
public:
    Cat2() : Animal2("猫") {}

    // 隐藏基类的 speak (非虚函数)
    void speak() const {
        cout << "  [猫]: 喵喵喵~" << endl;
    }

    // 重写基类的 move (虚函数)
    void move() const override {
        cout << "  [猫]: 轻轻地走" << endl;
    }
};

// ============================================================
// 六、常见错误
// ============================================================
// 错误1: 以为同名函数是重载
//   派生类和基类的同名函数不是重载关系,而是隐藏关系
//
// 错误2: 忘记用 Base:: 访问基类版本
//   在派生类中直接调用同名函数,只会调用派生类版本
//
// 错误3: 以为派生类隐藏了基类成员后,基类成员被"删除"了
//   基类成员仍然存在,只是在派生类作用域中被遮蔽
// ============================================================

int main() {
    cout << "===== 继承中的同名成员演示 =====" << endl;

    // --- 同名成员变量 ---
    cout << "\n--- 1. 同名成员变量 ---" << endl;
    Derived1 d1(100, 200);
    d1.showValues();

    // 直接访问 value,得到派生类的
    cout << "  对象 d1.value = " << d1.value << endl;        // 200
    // 使用作用域运算符访问基类的
    cout << "  对象 d1.Base1::value = " << d1.Base1::value << endl; // 100

    // --- 同名成员函数(函数隐藏) ---
    cout << "\n--- 2. 同名成员函数(函数隐藏) ---" << endl;
    ColorPrinter cp;

    // 调用派生类的 print(string)
    cp.print("Hello");

    // 试图调用基类的 print() 会报错,因为被隐藏了
    // cp.print();  // 错误! 基类的无参版本被隐藏了

    // 必须显式调用基类版本
    cout << "\n显式调用基类版本:" << endl;
    cp.printAll("测试消息");

    // --- using 声明解除隐藏 ---
    cout << "\n--- 3. using 声明解除隐藏 ---" << endl;
    MathDerived md;

    // 调用派生类的 calculate(int, int)
    cout << md.calculate(3, 4) << endl;      // 12 (乘法)

    // 使用 using 后,基类的其他版本也可以调用
    cout << md.calculate(3.0, 4.0) << endl;  // 7 (基类的 double 版本)
    cout << md.calculate(1, 2, 3) << endl;    // 6 (基类的三参数版本)

    // 显式调用基类版本
    cout << md.MathBase::calculate(3, 4) << endl;  // 7 (基类的 int 版本)

    // --- 同名静态成员 ---
    cout << "\n--- 4. 同名静态成员 ---" << endl;
    CounterBase cb1, cb2;
    CounterDerived cd1, cd2, cd3;

    CounterBase::showCount();     // 2 (基类创建了2个对象)
    CounterDerived::showCount();  // 3 (派生类创建了3个对象)

    // 通过对象访问
    cout << "  cd1.CounterBase::count = " << cd1.CounterBase::count << endl;    // 2
    cout << "  cd1.CounterDerived::count = " << cd1.CounterDerived::count << endl; // 3

    // --- 指针和引用 ---
    cout << "\n--- 5. 指针和引用 ---" << endl;
    Cat2 cat;
    Animal2* ptr = &cat;      // 基类指针指向派生类对象
    Animal2& ref = cat;       // 基类引用引用派生类对象

    cout << "通过基类指针调用:" << endl;
    ptr->speak();  // 调用基类的 speak(非虚函数,静态绑定)
    ptr->move();   // 调用派生类的 move(虚函数,动态绑定)

    cout << "通过基类引用调用:" << endl;
    ref.speak();   // 调用基类的 speak(非虚函数)
    ref.move();    // 调用派生类的 move(虚函数)

    cout << "通过派生类对象调用:" << endl;
    cat.speak();   // 调用派生类的 speak
    cat.move();    // 调用派生类的 move

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. 派生类同名成员隐藏(hide)基类同名成员" << endl;
    cout << "2. 隐藏不是删除,基类成员仍然存在" << endl;
    cout << "3. 使用 Base::member 显式访问基类成员" << endl;
    cout << "4. using Base::func 可以解除函数隐藏" << endl;
    cout << "5. 函数隐藏与函数重载不同(重载要求同一作用域)" << endl;
    cout << "6. 基类指针调用非虚函数时,调用基类版本" << endl;
    cout << "7. 基类指针调用虚函数时,调用派生类版本(多态)" << endl;

    return 0;
}
