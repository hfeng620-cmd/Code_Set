/*
 ============================================================================
 文件名 : 66_纯虚函数与抽象类.cpp
 描  述 : 详细讲解C++纯虚函数与抽象类的概念和用法
          包括：纯虚函数定义、抽象类特性、接口模式、
          派生类必须实现纯虚函数、虚析构函数的重要性
          以 Shape/Circle/Rectangle 层次结构为例演示
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 ============================================================================
*/

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ==================== 1. 纯虚函数与抽象类基础 ====================
// 纯虚函数语法：virtual 返回值类型 函数名(参数列表) = 0;
// 包含纯虚函数的类称为"抽象类"，抽象类不能实例化对象
// 抽象类的作用：为派生类提供统一的接口规范

// ==================== 2. Shape 抽象基类 ====================
// 这是一个抽象类，因为它包含了纯虚函数
class Shape {
protected:
    string m_name;  // 形状名称

public:
    // 构造函数
    Shape(const string& name) : m_name(name) {
        cout << "[Shape] 构造函数被调用, 形状: " << m_name << endl;
    }

    // ---------- 纯虚函数：计算面积 ----------
    // = 0 表示这是一个纯虚函数，没有函数体
    // 派生类必须重写(实现)这个函数，否则派生类也是抽象类
    virtual double area() const = 0;

    // ---------- 纯虚函数：计算周长 ----------
    virtual double perimeter() const = 0;

    // ---------- 纯虚函数：显示信息 ----------
    virtual void display() const = 0;

    // ---------- 虚析构函数（非常重要！） ----------
    // 抽象类通常作为基类使用，必须提供虚析构函数
    // 否则通过基类指针删除派生类对象时会导致未定义行为
    virtual ~Shape() {
        cout << "[Shape] 析构函数被调用, 形状: " << m_name << endl;
    }

    // ---------- 普通成员函数（非纯虚） ----------
    // 抽象类也可以有普通成员函数的默认实现
    string getName() const { return m_name; }
};

// ==================== 3. Circle 类：继承抽象类 ====================
// 派生类必须实现基类中所有的纯虚函数，否则它仍然是抽象类
class Circle : public Shape {
private:
    double m_radius;  // 半径

public:
    // 构造函数
    Circle(double radius) : Shape("圆形"), m_radius(radius) {
        cout << "[Circle] 构造函数被调用, 半径: " << m_radius << endl;
    }

    // ---------- 实现纯虚函数：计算面积 ----------
    // 必须用 override 关键字（推荐），表明这是重写基类虚函数
    double area() const override {
        return M_PI * m_radius * m_radius;
    }

    // ---------- 实现纯虚函数：计算周长 ----------
    double perimeter() const override {
        return 2 * M_PI * m_radius;
    }

    // ---------- 实现纯虚函数：显示信息 ----------
    void display() const override {
        cout << "=== " << m_name << " ===" << endl;
        cout << "  半径:   " << m_radius << endl;
        cout << "  面积:   " << area() << endl;
        cout << "  周长:   " << perimeter() << endl;
    }

    // 析构函数
    ~Circle() override {
        cout << "[Circle] 析构函数被调用, 半径: " << m_radius << endl;
    }
};

// ==================== 4. Rectangle 类：继承抽象类 ====================
class Rectangle : public Shape {
private:
    double m_width;   // 宽
    double m_height;  // 高

public:
    // 构造函数
    Rectangle(double w, double h)
        : Shape("矩形"), m_width(w), m_height(h) {
        cout << "[Rectangle] 构造函数被调用, 宽=" << m_width
             << " 高=" << m_height << endl;
    }

    // ---------- 实现纯虚函数：计算面积 ----------
    double area() const override {
        return m_width * m_height;
    }

    // ---------- 实现纯虚函数：计算周长 ----------
    double perimeter() const override {
        return 2 * (m_width + m_height);
    }

    // ---------- 实现纯虚函数：显示信息 ----------
    void display() const override {
        cout << "=== " << m_name << " ===" << endl;
        cout << "  宽:     " << m_width << endl;
        cout << "  高:     " << m_height << endl;
        cout << "  面积:   " << area() << endl;
        cout << "  周长:   " << perimeter() << endl;
    }

    // 析构函数
    ~Rectangle() override {
        cout << "[Rectangle] 析构函数被调用" << endl;
    }
};

// ==================== 5. Triangle 类 ====================
class Triangle : public Shape {
private:
    double m_a, m_b, m_c;  // 三条边

public:
    Triangle(double a, double b, double c)
        : Shape("三角形"), m_a(a), m_b(b), m_c(c) {
        cout << "[Triangle] 构造函数被调用" << endl;
    }

    // 海伦公式计算面积
    double area() const override {
        double s = (m_a + m_b + m_c) / 2.0;
        return sqrt(s * (s - m_a) * (s - m_b) * (s - m_c));
    }

    double perimeter() const override {
        return m_a + m_b + m_c;
    }

    void display() const override {
        cout << "=== " << m_name << " ===" << endl;
        cout << "  边长:   " << m_a << ", " << m_b << ", " << m_c << endl;
        cout << "  面积:   " << area() << endl;
        cout << "  周长:   " << perimeter() << endl;
    }

    ~Triangle() override {
        cout << "[Triangle] 析构函数被调用" << endl;
    }
};

// ==================== 6. 接口模式（Interface Pattern） ====================
// 当一个类中所有的成员函数都是纯虚函数时，这个类就是一个"接口"
// 接口只定义规范，不提供任何实现
class IDrawable {
public:
    // 纯虚函数：绘制
    virtual void draw() const = 0;
    // 纯虚函数：擦除
    virtual void erase() const = 0;
    // 纯虚函数：移动
    virtual void move(double dx, double dy) = 0;

    // 接口类的虚析构函数（必须有！）
    virtual ~IDrawable() = default;
};

class IPrintable {
public:
    virtual void print() const = 0;
    virtual ~IPrintable() = default;
};

// 多接口继承：一个类可以实现多个接口
class TextBox : public IDrawable, public IPrintable {
private:
    string m_text;
    double m_x, m_y;

public:
    TextBox(const string& text, double x, double y)
        : m_text(text), m_x(x), m_y(y) {}

    // 实现 IDrawable 接口
    void draw() const override {
        cout << "在位置(" << m_x << ", " << m_y << ")绘制文本框: "
             << m_text << endl;
    }
    void erase() const override {
        cout << "擦除文本框: " << m_text << endl;
    }
    void move(double dx, double dy) override {
        m_x += dx;
        m_y += dy;
        cout << "移动文本框到(" << m_x << ", " << m_y << ")" << endl;
    }

    // 实现 IPrintable 接口
    void print() const override {
        cout << "[打印] 文本框内容: " << m_text << endl;
    }
};

// ==================== 7. 演示函数 ====================
// 通过基类引用调用多态函数
void printShapeInfo(const Shape& shape) {
    shape.display();  // 根据实际类型调用对应的 display()
}

// 通过基类指针数组管理多个形状
void processShapes(Shape* shapes[], int count) {
    cout << "\n========== 批量处理形状 ==========" << endl;
    double totalArea = 0;
    for (int i = 0; i < count; i++) {
        shapes[i]->display();
        totalArea += shapes[i]->area();
        cout << endl;
    }
    cout << "所有形状的总面积: " << totalArea << endl;
}

// ==================== 8. 主函数 ====================
int main() {
    cout << "========================================" << endl;
    cout << "   纯虚函数与抽象类 详解演示" << endl;
    cout << "========================================\n" << endl;

    // ---- 8.1 抽象类不能实例化 ----
    // Shape s("测试");  // 错误！抽象类不能创建对象
    // Shape* p = new Shape("测试");  // 同样错误！

    cout << "--- 1. 创建派生类对象 ---" << endl;
    Circle circle(5.0);
    Rectangle rect(4.0, 6.0);
    Triangle tri(3.0, 4.0, 5.0);

    cout << "\n--- 2. 通过基类引用实现多态 ---" << endl;
    printShapeInfo(circle);
    printShapeInfo(rect);
    printShapeInfo(tri);

    cout << "\n--- 3. 通过基类指针实现多态 ---" << endl;
    Shape* shapes[3];
    shapes[0] = new Circle(10.0);
    shapes[1] = new Rectangle(5.0, 8.0);
    shapes[2] = new Triangle(6.0, 8.0, 10.0);
    processShapes(shapes, 3);

    // 释放内存（虚析构函数确保正确调用派生类析构函数）
    cout << "\n--- 4. 释放动态对象 ---" << endl;
    for (int i = 0; i < 3; i++) {
        delete shapes[i];  // 会先调用派生类析构，再调用基类析构
    }

    cout << "\n--- 5. 接口模式演示 ---" << endl;
    TextBox tb("Hello World", 10.0, 20.0);
    tb.draw();
    tb.print();
    tb.move(5.0, 5.0);
    tb.erase();

    // 通过接口指针调用
    cout << "\n--- 6. 通过接口指针调用 ---" << endl;
    IDrawable* drawable = &tb;
    drawable->draw();
    IPrintable* printable = &tb;
    printable->print();

    cout << "\n--- 7. 派生类未实现所有纯虚函数的情况 ---" << endl;
    // 如果一个派生类只实现了部分纯虚函数，它仍然是抽象类
    // 例如下面这个类就不能实例化：
    // class IncompleteShape : public Shape {
    // public:
    //     double area() const override { return 0; }
    //     // 没有实现 perimeter() 和 display()，仍然是抽象类！
    // };
    // IncompleteShape is;  // 编译错误！
    cout << "如果派生类没有实现所有纯虚函数，它仍然是抽象类，不能实例化" << endl;

    cout << "\n========================================" << endl;
    cout << "   程序结束" << endl;
    cout << "========================================" << endl;

    return 0;
}
