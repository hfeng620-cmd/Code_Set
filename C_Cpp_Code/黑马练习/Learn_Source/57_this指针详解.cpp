/*
 * 57_this指针详解.cpp
 * 本文件详细讲解C++中this指针的概念和用法
 * this指针是隐含在每个非静态成员函数中的指针，指向调用该函数的对象
 * 编译命令: g++ -o 57_this指针详解 57_this指针详解.cpp -std=c++11
 */

#include <iostream>
#include <string>
using namespace std;

// ===================== 1. this指针基础 =====================
class Person {
private:
    string name;
    int age;

public:
    // 构造函数参数名与成员变量名相同时，需要this区分
    Person(string name, int age) {
        // this->name 是成员变量
        // name 是参数
        this->name = name;
        this->age = age;
        cout << "  [构造] this = " << this << endl;
    }

    void show() const {
        // this指针在const成员函数中是 const Person* 类型
        cout << "  " << name << ", " << age << "岁 (this=" << this << ")" << endl;
    }

    // 返回this指向的对象的引用，实现链式调用
    Person& setName(const string &name) {
        this->name = name;
        return *this;  // 返回当前对象的引用
    }

    Person& setAge(int age) {
        this->age = age;
        return *this;
    }

    // 比较两个对象是否相同
    bool isSame(const Person &other) const {
        // this是指向当前对象的指针，other是另一个对象的引用
        return (this->name == other.name) && (this->age == other.age);
    }

    string getName() const { return name; }
    int getAge() const { return age; }
};

// ===================== 2. 链式调用（Fluent Interface） =====================
class Calculator {
private:
    double result;
    string history;

public:
    Calculator() : result(0.0), history("0") {}

    // 每个方法都返回 *this 的引用，实现链式调用
    Calculator& add(double val) {
        result += val;
        history += " + " + to_string(val);
        return *this;
    }

    Calculator& subtract(double val) {
        result -= val;
        history += " - " + to_string(val);
        return *this;
    }

    Calculator& multiply(double val) {
        result *= val;
        history += " * " + to_string(val);
        return *this;
    }

    Calculator& divide(double val) {
        if (val != 0) {
            result /= val;
            history += " / " + to_string(val);
        }
        return *this;
    }

    Calculator& reset() {
        result = 0.0;
        history = "0";
        return *this;
    }

    void show() const {
        cout << "  计算: " << history << " = " << result << endl;
    }

    double getResult() const { return result; }
};

// ===================== 3. this作为隐式参数 =====================
class Box {
private:
    double length, width, height;

public:
    Box(double l, double w, double h) : length(l), width(w), height(h) {}

    // 编译器将成员函数转换为包含this指针的普通函数：
    // 原型: double volume() const
    // 实际: double volume(const Box* this) const
    double volume() const {
        // this->length, this->width, this->height
        return length * width * height;
    }

    // 比较两个Box的体积
    // 原型: bool isBigger(const Box &other) const
    // 实际: bool isBigger(const Box* this, const Box &other) const
    bool isBigger(const Box &other) const {
        return this->volume() > other.volume();
    }

    void show() const {
        cout << "  Box(" << length << "x" << width << "x" << height
             << ") 体积=" << volume() << endl;
    }
};

// ===================== 4. 空指针调用成员函数的危险 =====================
class Danger {
private:
    int value;

public:
    Danger(int v = 0) : value(v) {}

    // 普通成员函数：内部使用了this
    void show() {
        // 如果通过空指针调用，this是nullptr
        // 访问this->value就是访问nullptr->value，导致崩溃！
        cout << "  value = " << value << endl;  // 等价于 this->value
    }

    // 安全版本：检查this是否为空
    void safeShow() {
        if (this == nullptr) {
            cout << "  警告: 通过空指针调用！" << endl;
            return;
        }
        cout << "  value = " << value << endl;
    }

    // 不访问成员的函数：即使this为空也不会崩溃
    static void staticShow() {
        cout << "  这是静态函数，没有this指针" << endl;
    }

    int getValue() const {
        return value;
    }
};

// ===================== 5. this在运算符重载中的应用 =====================
class Vector2D {
private:
    double x, y;

public:
    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // 运算符重载中使用this
    Vector2D operator+(const Vector2D &other) const {
        // this指向当前对象，other是右操作数
        return Vector2D(this->x + other.x, this->y + other.y);
    }

    Vector2D& operator+=(const Vector2D &other) {
        this->x += other.x;
        this->y += other.y;
        return *this;  // 返回引用以支持链式操作
    }

    bool operator==(const Vector2D &other) const {
        return (this->x == other.x) && (this->y == other.y);
    }

    void show() const {
        cout << "  (" << x << ", " << y << ")" << endl;
    }
};

// ===================== 6. 返回*this的引用 =====================
class Builder {
private:
    string result;

public:
    Builder() {}

    // 每个方法都返回*this的引用
    Builder& append(const string &str) {
        result += str;
        return *this;
    }

    Builder& appendLine(const string &str) {
        result += str + "\n";
        return *this;
    }

    Builder& clear() {
        result.clear();
        return *this;
    }

    string build() const {
        return result;
    }

    void show() const {
        cout << "  \"" << result << "\"" << endl;
    }
};

// ===================== 函数声明 =====================
void thisBasicDemo();
void chainingDemo();
void implicitThisDemo();
void nullPointerDemo();
void operatorDemo();
void builderDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ this指针详解" << endl;
    cout << "========================================" << endl << endl;

    thisBasicDemo();     // this基础
    chainingDemo();      // 链式调用
    implicitThisDemo();  // 隐式参数
    nullPointerDemo();   // 空指针危险
    operatorDemo();      // 运算符重载
    builderDemo();       // Builder模式

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== this基础 =====================
void thisBasicDemo() {
    cout << "===== 1. this指针基础 =====" << endl;

    // this指针的本质：
    // - 是一个隐含的指针，指向调用成员函数的对象
    // - 在成员函数内部可以显式使用
    // - 类型为 ClassName* (非const成员函数)
    // - 类型为 const ClassName* (const成员函数)

    Person p1("张三", 25);
    Person p2("李四", 30);

    // 每个对象的this指针值不同（指向不同的对象）
    cout << "p1的地址: " << &p1 << endl;
    p1.show();  // show内部打印的this应该等于&p1

    cout << "p2的地址: " << &p2 << endl;
    p2.show();  // show内部打印的this应该等于&p2

    // 解决命名冲突
    cout << "\n--- 解决命名冲突 ---" << endl;
    Person p3("王五", 28);
    p3.show();

    cout << endl;
}

// ===================== 链式调用 =====================
void chainingDemo() {
    cout << "===== 2. 链式调用(return *this) =====" << endl;

    // 链式调用：通过返回*this的引用，可以连续调用多个方法

    Person p("张三", 25);
    cout << "修改前: ";
    p.show();

    // 链式调用：每个setter返回对象引用，可以继续调用下一个
    p.setName("李四").setAge(30);
    // 等价于：
    // p.setName("李四");  // 返回p的引用
    // p.setAge(30);       // 继续调用

    cout << "修改后: ";
    p.show();

    // Calculator链式调用
    cout << "\n--- Calculator链式调用 ---" << endl;
    Calculator calc;
    calc.add(10).subtract(3).multiply(2).divide(7);
    calc.show();

    calc.reset().add(100).multiply(0.5);
    calc.show();

    // 链式调用的关键：
    // 1. 每个方法返回 *this 的引用
    // 2. 返回类型是 ClassName& (引用，不是值)
    // 3. 这样返回的是当前对象本身，而不是副本

    cout << endl;
}

// ===================== 隐式this =====================
void implicitThisDemo() {
    cout << "===== 3. this作为隐式参数 =====" << endl;

    // 编译器如何处理成员函数：
    // 源代码: void show() const { cout << name; }
    // 编译器: void show(const Person* this) const { cout << this->name; }

    // 同样，成员函数调用：
    // 源代码: p.show()
    // 编译器: Person::show(&p)  // 把对象地址传给this

    Box b1(3, 4, 5);
    Box b2(2, 3, 4);

    b1.show();
    b2.show();

    // isBigger实际接收两个参数：this和other
    cout << "b1 > b2 ? " << (b1.isBigger(b2) ? "是" : "否") << endl;
    // 等价于: Box::isBigger(&b1, b2)

    cout << endl;
}

// ===================== 空指针调用危险 =====================
void nullPointerDemo() {
    cout << "===== 4. 空指针调用成员函数的危险 =====" << endl;

    // 通过空指针调用成员函数是未定义行为
    // 但如果函数不访问成员变量，可能不会崩溃

    Danger *p = nullptr;

    // 危险：show()内部访问了value，会导致崩溃
    // p->show();  // 取消注释可观察崩溃

    // 安全：不访问成员的函数
    Danger::staticShow();  // 静态函数，没有this

    // 如何防御？
    // 1. 调用前检查指针是否为空
    // 2. 在成员函数中检查this是否为空（不推荐，增加开销）
    // 3. 确保指针在使用前已正确初始化

    // 创建有效对象来演示
    Danger d(42);
    d.show();  // 安全：d不是空指针
    d.safeShow();

    cout << "  空指针调用成员函数是未定义行为，务必避免！" << endl;

    cout << endl;
}

// ===================== 运算符重载中的this =====================
void operatorDemo() {
    cout << "===== 5. 运算符重载中的this =====" << endl;

    Vector2D v1(1.0, 2.0);
    Vector2D v2(3.0, 4.0);

    cout << "v1 = "; v1.show();
    cout << "v2 = "; v2.show();

    // operator+ 中 this 指向 v1
    Vector2D v3 = v1 + v2;
    cout << "v1 + v2 = "; v3.show();

    // operator+= 中 this 指向 v1
    v1 += v2;
    cout << "v1 += v2 后: "; v1.show();

    // operator== 中 this 指向 v1
    cout << "v1 == v3 ? " << (v1 == v3 ? "是" : "否") << endl;

    // 运算符重载中this的作用：
    // - 二元运算符：this指向左操作数，参数是右操作数
    // - 一元运算符：this指向操作对象
    // - 赋值运算符：this指向被赋值的对象

    cout << endl;
}

// ===================== Builder模式 =====================
void builderDemo() {
    cout << "===== 6. Builder模式(return *this) =====" << endl;

    // Builder模式利用链式调用，逐步构建复杂对象

    Builder builder;
    string result = builder
        .append("Hello")
        .append(", ")
        .append("World")
        .append("!")
        .build();

    cout << "构建结果: " << result << endl;

    // 更复杂的Builder
    Builder html;
    html.appendLine("<html>")
        .appendLine("  <body>")
        .append("    <h1>")
        .append("标题")
        .appendLine("</h1>")
        .appendLine("  </body>")
        .appendLine("</html>");

    cout << "\nHTML输出:" << endl;
    html.show();

    // 链式调用的优点：
    // 1. 代码更简洁、可读性更好
    // 2. 减少临时变量
    // 3. API设计更优雅
    // 标准库中的cout就是链式调用的典型例子

    cout << endl;
}
