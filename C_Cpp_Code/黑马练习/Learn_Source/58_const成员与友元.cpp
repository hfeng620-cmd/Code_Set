/*
 * 58_const成员与友元.cpp
 * 本文件详细讲解C++中const成员函数、mutable关键字、const对象以及友元
 * 这些特性是C++封装机制的重要补充
 * 编译命令: g++ -o 58_const成员与友元 58_const成员与友元.cpp -std=c++11
 */

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ===================== 前向声明 =====================
class Box;

// ===================== 1. const成员函数 =====================
class Circle {
private:
    double radius;
    mutable int accessCount;  // mutable成员：即使在const函数中也能修改
    string color;

public:
    Circle(double r, const string &c) : radius(r), accessCount(0), color(c) {}

    // const成员函数：承诺不修改对象的状态
    // 在参数列表后加const修饰
    double getArea() const {
        // radius = 10;  // 错误！const函数不能修改成员变量
        accessCount++;  // mutable成员可以修改
        return 3.14159 * radius * radius;
    }

    double getRadius() const {
        accessCount++;
        return radius;
    }

    // 非const成员函数：可以修改成员变量
    void setRadius(double r) {
        radius = r;
    }

    // 非const函数
    void setColor(const string &c) {
        color = c;
    }

    // const函数可以调用其他const函数
    void show() const {
        // setRadius(5);  // 错误！const函数不能调用非const函数
        cout << "  " << color << "圆: 半径=" << getRadius()
             << ", 面积=" << getArea() << endl;
    }

    // 非const函数可以调用const函数
    void updateAndShow(double r) {
        setRadius(r);
        show();  // 可以调用const函数
    }

    int getAccessCount() const {
        return accessCount;
    }
};

// ===================== 2. const对象 =====================
class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // const成员函数：可以被const对象调用
    double getX() const { return x; }
    double getY() const { return y; }
    double distanceTo(const Point &other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
    void show() const {
        cout << "  (" << x << ", " << y << ")" << endl;
    }

    // 非const成员函数：不能被const对象调用
    void move(double dx, double dy) {
        x += dx;
        y += dy;
    }
    void setX(double val) { x = val; }
    void setY(double val) { y = val; }
};

// ===================== 3. 友元函数 =====================
// 友元函数可以访问类的private和protected成员
class Rectangle {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    // 声明友元函数：printRectInfo可以访问Rectangle的私有成员
    friend void printRectInfo(const Rectangle &r);

    // 友元函数：计算两个矩形的面积和
    friend double totalArea(const Rectangle &r1, const Rectangle &r2);

    // 友元运算符重载：输出运算符 <<
    // 这是友元最常用的应用场景之一
    friend ostream& operator<<(ostream &os, const Rectangle &r);

    void show() const {
        cout << "  矩形: " << width << "x" << height << endl;
    }
};

// 友元函数定义（不是Rectangle的成员函数，但可以访问其私有成员）
void printRectInfo(const Rectangle &r) {
    // 可以直接访问private成员
    cout << "  [友元] 宽=" << r.width << ", 高=" << r.height
         << ", 面积=" << r.width * r.height << endl;
}

double totalArea(const Rectangle &r1, const Rectangle &r2) {
    return r1.width * r1.height + r2.width * r2.height;
}

// 重载输出运算符（友元函数）
// 为什么operator<<必须是友元？
// 因为ostream是左操作数，this是Rectangle，但<<的左操作数应该是ostream
// 所以不能是成员函数，必须是友元函数
ostream& operator<<(ostream &os, const Rectangle &r) {
    os << "Rectangle(" << r.width << "x" << r.height << ")";
    return os;
}

// ===================== 4. 友元类 =====================
class Engine {
private:
    int horsepower;
    string type;

public:
    Engine(int hp, const string &t) : horsepower(hp), type(t) {}

    // 声明Car为友元类：Car可以访问Engine的所有成员
    friend class Car;

    void show() const {
        cout << "  引擎: " << type << ", " << horsepower << "马力" << endl;
    }
};

class Car {
private:
    string brand;
    Engine engine;  // 组合：Car包含Engine

public:
    Car(const string &b, int hp, const string &engineType)
        : brand(b), engine(hp, engineType) {}

    void show() const {
        cout << "  汽车: " << brand << endl;
        // 作为Engine的友元类，可以直接访问Engine的private成员
        cout << "  引擎类型: " << engine.type << endl;      // 直接访问private
        cout << "  马力: " << engine.horsepower << endl;     // 直接访问private
    }

    // 修改引擎参数（友元访问）
    void tuneEngine(int newHp) {
        engine.horsepower = newHp;
        cout << "  引擎调校后: " << engine.horsepower << "马力" << endl;
    }
};

// ===================== 5. 友元的特性 =====================
// 友元的三大特性：
// 1. 单向性：A是B的友元，不代表B是A的友元
// 2. 不传递：A是B的友元，B是C的友元，不代表A是C的友元
// 3. 不继承：A是B的友元，C继承B，不代表A是C的友元

class B;  // 前向声明

class A {
private:
    int secretA = 100;
    friend class B;  // B是A的友元
};

class B {
private:
    int secretB = 200;
    // A不是B的友元，不能访问B的private成员
    // friend class A;  // 如果取消注释，A才能访问B的private

public:
    void accessA(const A &a) {
        cout << "  B访问A的秘密: " << a.secretA << endl;  // 可以，B是A的友元
    }

    // void accessB_from_A(A &a) {
    //     // A不能访问B的private（单向性）
    // }
};

class C {
    // C不是A的友元，即使C继承了其他类
    // 不能访问A的private成员
};

// ===================== 函数声明 =====================
void constMemberDemo();
void constObjectDemo();
void friendFuncDemo();
void friendClassDemo();
void friendPropertiesDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ const成员与友元" << endl;
    cout << "========================================" << endl << endl;

    constMemberDemo();     // const成员函数
    constObjectDemo();     // const对象
    friendFuncDemo();      // 友元函数
    friendClassDemo();     // 友元类
    friendPropertiesDemo();// 友元特性

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== const成员函数 =====================
void constMemberDemo() {
    cout << "===== 1. const成员函数 =====" << endl;

    // const成员函数的特点：
    // 1. 不能修改非mutable的成员变量
    // 2. 不能调用非const的成员函数
    // 3. const对象只能调用const成员函数

    Circle c(5.0, "红色");

    // 调用const函数
    cout << "面积: " << c.getArea() << endl;
    cout << "半径: " << c.getRadius() << endl;
    c.show();

    // 调用非const函数
    c.setRadius(10.0);
    cout << "\n修改半径后:" << endl;
    c.show();

    // mutable成员的作用：
    // 有些成员即使在const函数中也需要修改
    // 比如：访问计数器、缓存标志、互斥锁等
    cout << "\n访问次数: " << c.getAccessCount() << endl;

    // 什么时候用const？
    // 1. 不修改对象状态的函数都应该声明为const
    // 2. 这样const对象也能调用这些函数
    // 3. 提高代码的安全性和可读性

    cout << endl;
}

// ===================== const对象 =====================
void constObjectDemo() {
    cout << "===== 2. const对象 =====" << endl;

    // const对象：一旦创建就不能修改其成员变量（mutable除外）
    const Point origin(0, 0);
    const Point p1(3, 4);

    // const对象只能调用const成员函数
    cout << "原点: ";
    origin.show();  // show()是const函数

    cout << "p1: ";
    p1.show();

    // const对象不能调用非const函数
    // origin.move(1, 1);  // 错误！move不是const函数
    // origin.setX(5);     // 错误！setX不是const函数

    // const对象可以调用const函数
    double dist = origin.distanceTo(p1);
    cout << "原点到p1的距离: " << dist << endl;

    // const引用也是类似的效果
    const Point &ref = p1;
    ref.show();         // 可以调用const函数
    // ref.move(1,1);   // 错误！const引用不能调用非const函数

    // 什么时候用const对象？
    // 1. 函数参数使用const引用：避免拷贝 + 保护数据
    // 2. 不需要修改的局部变量声明为const
    // 3. 全局常量对象

    cout << endl;
}

// ===================== 友元函数 =====================
void friendFuncDemo() {
    cout << "===== 3. 友元函数 =====" << endl;

    // 友元函数可以访问类的private成员
    // 但它不是类的成员函数

    Rectangle r1(3, 4);
    Rectangle r2(5, 6);

    // 普通成员函数访问
    r1.show();

    // 友元函数访问私有成员
    cout << "\n--- 友元函数 ---" << endl;
    printRectInfo(r1);
    printRectInfo(r2);

    // 友元函数计算面积和
    double total = totalArea(r1, r2);
    cout << "总面积: " << total << endl;

    // 友元重载运算符 <<
    cout << "\n--- 运算符重载 ---" << endl;
    cout << r1 << endl;  // 使用重载的<<运算符
    cout << r2 << endl;

    // 为什么需要友元函数？
    // 1. 某些函数需要访问类的私有成员，但不适合做成员函数
    // 2. 运算符重载（如<<）的左操作数不是当前类的对象
    // 3. 两个类之间需要互相访问私有成员

    cout << endl;
}

// ===================== 友元类 =====================
void friendClassDemo() {
    cout << "===== 4. 友元类 =====" << endl;

    // 友元类可以访问被友元类的所有成员

    Car car("宝马", 300, "V8");
    car.show();

    cout << "\n--- 修改引擎 ---" << endl;
    car.tuneEngine(350);
    car.show();

    // 友元类的特点：
    // 1. Car是Engine的友元，Car可以访问Engine的所有成员
    // 2. 但Engine不能访问Car的private成员（单向性）
    // 3. Car的友元关系不会传递给其他类

    cout << endl;
}

// ===================== 友元的特性 =====================
void friendPropertiesDemo() {
    cout << "===== 5. 友元的三大特性 =====" << endl;

    A a;
    B b;

    // 1. 单向性：B是A的友元，但A不是B的友元
    cout << "--- 单向性 ---" << endl;
    b.accessA(a);  // B可以访问A的private
    // a不能访问B的private（因为A不是B的友元）

    // 2. 不传递性
    cout << "\n--- 不传递性 ---" << endl;
    // 假设A是B的友元，B是C的友元
    // A不能访问C的private（友元关系不传递）
    cout << "  友元关系不传递: A友元B, B友元C ≠ A友元C" << endl;

    // 3. 不继承性
    cout << "\n--- 不继承性 ---" << endl;
    // 假设D继承B，B是A的友元
    // D不能访问A的private（友元关系不继承）
    cout << "  友元关系不继承: B友元A, D继承B ≠ D友元A" << endl;

    // 总结：
    // 1. 友元是封装的例外，谨慎使用
    // 2. 友元关系是单向的、不传递的、不继承的
    // 3. 最常用场景：重载<<运算符
    // 4. 友元破坏了封装性，能不用就不用
    // 5. 有些场景下友元是必要的（如两个类需要紧密协作）

    cout << endl;
}
