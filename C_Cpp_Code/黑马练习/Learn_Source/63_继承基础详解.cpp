/*
 * 文件名: 63_继承基础详解.cpp
 * 描述: C++继承的基础知识详解
 * 知识点:
 *   1. 继承的语法: class Derived : access-specifier Base
 *   2. public, protected, private 继承方式
 *   3. 哪些成员可以被继承
 *   4. 构造函数和析构函数的调用顺序
 *   5. 使用基类构造函数(C++11)
 * 编译: g++ -std=c++11 63_继承基础详解.cpp -o 63_继承基础详解
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 一、继承的基本语法
// ============================================================
// 继承允许我们基于已有类(基类/父类)创建新类(派生类/子类)
// 语法: class 派生类名 : 继承方式 基类名
//
// 继承方式:
//   public    公有继承   - 最常用
//   protected 保护继承
//   private   私有继承   - 默认继承方式
// ============================================================

// ---- 基类: Person ----
class Person {
public:
    string name;
    int age;

    // 构造函数
    Person() : name("未知"), age(0) {
        cout << "  [Person] 默认构造函数被调用" << endl;
    }

    Person(string n, int a) : name(n), age(a) {
        cout << "  [Person] 参数构造函数被调用: " << name << endl;
    }

    // 拷贝构造函数
    Person(const Person& other) : name(other.name), age(other.age) {
        cout << "  [Person] 拷贝构造函数被调用: " << name << endl;
    }

    // 析构函数
    ~Person() {
        cout << "  [Person] 析构函数被调用: " << name << endl;
    }

    void showInfo() const {
        cout << "姓名: " << name << ", 年龄: " << age;
    }
};

// ============================================================
// 二、继承方式详解
// ============================================================
// 继承方式决定了基类成员在派生类中的访问权限:
//
// | 基类成员     | public继承 | protected继承 | private继承 |
// |------------|-----------|--------------|------------|
// | public     | public    | protected    | private    |
// | protected  | protected | protected    | private    |
// | private    | 不可访问   | 不可访问      | 不可访问    |
//
// 记忆口诀: 继承方式取两者中较严格的那个
//   - public 继承: 保持原样
//   - protected 继承: public 降级为 protected
//   - private 继承: 全部变为 private
// ============================================================

// ---- 公有继承(public) ----
// 最常用的继承方式,基类的 public 成员在派生类中仍然是 public
class Student : public Person {
public:
    int studentId;
    string school;

    // ============================================================
    // 三、构造函数的调用顺序
    // ============================================================
    // 构造顺序: 先基类,后派生类
    // 析构顺序: 先派生类,后基类 (与构造相反)
    // 原因: 基类是派生类的一部分,必须先构造好基类部分
    // ============================================================

    // 派生类构造函数
    // 初始化列表中可以调用基类构造函数
    Student() : Person(), studentId(0), school("未知") {
        cout << "  [Student] 默认构造函数被调用" << endl;
    }

    Student(string n, int a, int id, string sch)
        : Person(n, a),           // 调用基类构造函数
          studentId(id),
          school(sch) {
        cout << "  [Student] 参数构造函数被调用: " << name << endl;
    }

    // 析构函数
    ~Student() {
        cout << "  [Student] 析构函数被调用: " << name << endl;
    }

    void showStudent() const {
        showInfo();  // 调用基类的成员函数
        cout << ", 学号: " << studentId << ", 学校: " << school << endl;
    }
};

// ---- 保护继承(protected) ----
class Teacher_protected : protected Person {
public:
    string subject;

    Teacher_protected(string n, int a, string sub)
        : Person(n, a), subject(sub) {}

    void showTeacher() const {
        // 在派生类内部可以访问 protected 成员
        cout << "姓名: " << name << ", 年龄: " << age;  // OK
        cout << ", 科目: " << subject << endl;
    }
};

// ---- 私有继承(private) ----
class Employee_private : private Person {
public:
    string department;

    Employee_private(string n, int a, string dept)
        : Person(n, a), department(dept) {}

    void showEmployee() const {
        // 在派生类内部可以访问 private 继承的成员
        cout << "姓名: " << name << ", 年龄: " << age;  // OK
        cout << ", 部门: " << department << endl;
    }
};

// ============================================================
// 四、哪些成员可以被继承
// ============================================================
// 可以继承的:
//   - 所有非静态成员变量(包括 public, protected, private)
//   - 所有非静态成员函数
//   - 友元关系不能继承(A是B的友元,C继承B,A不一定是C的友元)
//
// 不可以继承的:
//   - 构造函数(但C++11可以用 using 声明继承)
//   - 析构函数
//   - 赋值运算符 operator=
// ============================================================

// ============================================================
// 五、使用基类构造函数(C++11)
// ============================================================
// C++11允许使用 using 声明来继承基类的构造函数
// 语法: using BaseClass::BaseClass;
// 注意: 只继承构造函数,不继承其他成员
// ============================================================

class Animal {
public:
    string species;
    int legs;

    Animal() : species("未知"), legs(0) {
        cout << "  [Animal] 默认构造" << endl;
    }

    Animal(string sp, int l) : species(sp), legs(l) {
        cout << "  [Animal] 参数构造: " << species << endl;
    }

    virtual ~Animal() {
        cout << "  [Animal] 析构: " << species << endl;
    }

    void show() const {
        cout << "物种: " << species << ", 腿数: " << legs;
    }
};

class Dog : public Animal {
public:
    string name;

    // 使用 using 继承基类构造函数
    // 注意: 这只能继承基类构造函数的参数列表
    // 不能同时初始化派生类新增的成员(需要额外定义构造函数)
    using Animal::Animal;  // 继承 Animal 的所有构造函数

    // 额外的构造函数,初始化新增成员
    Dog(string sp, int l, string n) : Animal(sp, l), name(n) {
        cout << "  [Dog] 参数构造: " << name << endl;
    }

    ~Dog() {
        cout << "  [Dog] 析构: " << name << endl;
    }
};

// ============================================================
// 六、继承的层次
// ============================================================
// C++支持多层继承: A -> B -> C
// 但不建议继承层次太深(一般不超过3层)
// ============================================================

class Shape {
public:
    string color;
    Shape(string c = "白色") : color(c) {
        cout << "  [Shape] 构造: " << color << endl;
    }
    ~Shape() { cout << "  [Shape] 析构" << endl; }
};

class Rectangle : public Shape {
public:
    double width, height;
    Rectangle(string c, double w, double h)
        : Shape(c), width(w), height(h) {
        cout << "  [Rectangle] 构造" << endl;
    }
    ~Rectangle() { cout << "  [Rectangle] 析构" << endl; }

    double area() const { return width * height; }
};

class Box : public Rectangle {
public:
    double depth;
    Box(string c, double w, double h, double d)
        : Rectangle(c, w, h), depth(d) {
        cout << "  [Box] 构造" << endl;
    }
    ~Box() { cout << "  [Box] 析构" << endl; }

    double volume() const { return width * height * depth; }
};

// ============================================================
// 七、常见错误
// ============================================================
// 错误1: 派生类构造函数没有调用基类构造函数
//   如果基类没有默认构造函数,派生类必须在初始化列表中显式调用
//
// 错误2: 在派生类中直接访问基类的 private 成员
//   应通过基类的 public 或 protected 接口访问
//
// 错误3: 混淆继承方式对访问权限的影响
//   private 继承会将所有基类成员变为 private
// ============================================================

int main() {
    cout << "===== 继承基础详解演示 =====" << endl;

    // --- 公有继承 ---
    cout << "\n--- 1. 公有继承(public) ---" << endl;
    cout << "创建 Student 对象:" << endl;
    Student stu1("张三", 20, 1001, "清华大学");
    stu1.showStudent();

    // public 继承后,基类的 public 成员仍可直接访问
    cout << "\n直接访问基类 public 成员:" << endl;
    cout << "姓名: " << stu1.name << endl;    // OK, public 继承
    cout << "年龄: " << stu1.age << endl;     // OK

    // --- 保护继承 ---
    cout << "\n--- 2. 保护继承(protected) ---" << endl;
    Teacher_protected teacher("李老师", 35, "数学");
    teacher.showTeacher();
    // cout << teacher.name;  // 错误! protected 继承后,外部不能访问
    cout << "注意: protected 继承后,基类成员在类外不可访问" << endl;

    // --- 私有继承 ---
    cout << "\n--- 3. 私有继承(private) ---" << endl;
    Employee_private emp("王五", 28, "技术部");
    emp.showEmployee();
    // cout << emp.name;  // 错误! private 继承后,外部不能访问
    cout << "注意: private 继承后,基类成员在类外不可访问" << endl;

    // --- 构造和析构顺序 ---
    cout << "\n--- 4. 构造和析构顺序 ---" << endl;
    {
        cout << "创建局部 Student 对象:" << endl;
        Student stu2("赵六", 22, 1002, "北京大学");
        cout << "局部对象即将销毁..." << endl;
    }
    cout << "注意: 构造顺序为 基类->派生类,析构顺序为 派生类->基类" << endl;

    // --- using 继承构造函数 ---
    cout << "\n--- 5. using 继承构造函数(C++11) ---" << endl;
    cout << "创建 Dog 对象(使用继承的构造函数):" << endl;
    Dog dog1("犬科", 4, "旺财");
    dog1.show();
    cout << ", 名字: " << dog1.name << endl;

    // --- 多层继承 ---
    cout << "\n--- 6. 多层继承 ---" << endl;
    cout << "创建 Box 对象(Shape->Rectangle->Box):" << endl;
    Box box("红色", 3.0, 4.0, 5.0);
    cout << "颜色: " << box.color << endl;       // 继承自 Shape
    cout << "面积: " << box.area() << endl;       // 继承自 Rectangle
    cout << "体积: " << box.volume() << endl;     // Box 自己的方法

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. 继承语法: class Derived : access-specifier Base" << endl;
    cout << "2. public 继承最常用,保持基类成员原有访问权限" << endl;
    cout << "3. 构造顺序: 基类 -> 派生类" << endl;
    cout << "4. 析构顺序: 派生类 -> 基类(与构造相反)" << endl;
    cout << "5. C++11可用 using Base::Base 继承基类构造函数" << endl;
    cout << "6. 基类的 private 成员在派生类中不可直接访问" << endl;
    cout << "7. 继承层次不宜过深(建议不超过3层)" << endl;

    return 0;
}
