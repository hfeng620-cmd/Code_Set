/*
 * 56_静态成员详解.cpp
 * 本文件详细讲解C++类中静态成员变量和静态成员函数的用法
 * 静态成员属于类本身，不属于任何特定对象，被所有对象共享
 * 编译命令: g++ -o 56_静态成员详解 56_静态成员详解.cpp -std=c++11
 */

#include <iostream>
#include <string>
using namespace std;

// ===================== 1. 静态成员变量基础 =====================
class Student {
private:
    string name;
    int age;
    // 静态成员变量：所有Student对象共享同一个count
    // 不属于任何一个对象，属于类本身
    static int count;      // 静态成员变量声明（不分配内存）
    static const int MAX;  // 静态const成员

public:
    Student(const string &n, int a) : name(n), age(a) {
        count++;  // 每创建一个对象，计数加1
        cout << "  [构造] " << name << " (第" << count << "个对象)" << endl;
    }

    ~Student() {
        count--;  // 每销毁一个对象，计数减1
        cout << "  [析构] " << name << " (剩余" << count << "个对象)" << endl;
    }

    void show() const {
        cout << "  " << name << ", " << age << "岁" << endl;
    }

    // 静态成员函数：没有this指针，只能访问静态成员
    // 可以通过类名直接调用，不需要对象
    static int getCount() {
        // name = "test";  // 错误！静态函数不能访问非静态成员
        // 因为静态函数没有this指针，不知道访问哪个对象的name
        return count;
    }

    static int getMax() {
        return MAX;
    }
};

// 静态成员变量必须在类外初始化（C++17之前）
// 格式：类型 类名::静态成员名 = 初始值;
int Student::count = 0;
const int Student::MAX = 100;

// ===================== 2. 单例模式 =====================
// 单例模式：确保一个类只有一个实例，并提供全局访问点
class Singleton {
private:
    // 私有静态指针：指向唯一的实例
    static Singleton *instance;

    // 私有数据
    string data;
    int value;

    // 私有构造函数：防止外部创建对象
    Singleton() : data("单例数据"), value(0) {
        cout << "  [构造] Singleton实例被创建" << endl;
    }

    // 私有拷贝构造：防止拷贝
    Singleton(const Singleton &);
    // 私有赋值运算符：防止赋值
    Singleton& operator=(const Singleton &);

public:
    // 公有静态方法：获取唯一实例
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    // 销毁实例
    static void destroyInstance() {
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
            cout << "  Singleton实例被销毁" << endl;
        }
    }

    void setData(const string &d) { data = d; }
    void setValue(int v) { value = v; }
    string getData() const { return data; }
    int getValue() const { return value; }

    void show() const {
        cout << "  Singleton: data=" << data << ", value=" << value << endl;
    }
};

// 静态成员初始化
Singleton* Singleton::instance = nullptr;

// ===================== 3. 计数器示例 =====================
class Product {
private:
    string name;
    double price;
    int id;
    static int nextId;  // 自动分配ID

public:
    Product(const string &n, double p) : name(n), price(p) {
        id = nextId++;  // 分配唯一ID
        cout << "  [创建] 产品#" << id << ": " << name << endl;
    }

    void show() const {
        cout << "  #" << id << " " << name << " - $" << price << endl;
    }

    int getId() const { return id; }
    string getName() const { return name; }

    static int getNextId() { return nextId; }
};

int Product::nextId = 1001;  // 从1001开始编号

// ===================== 4. 静态成员的内存位置 =====================
class MemoryDemo {
public:
    int normalVar;           // 普通成员：在对象内存中
    static int staticVar;    // 静态成员：在全局区（静态区）

    MemoryDemo(int v) : normalVar(v) {}

    void show() const {
        cout << "  普通成员地址: " << &normalVar << endl;
        cout << "  静态成员地址: " << &staticVar << endl;
        cout << "  对象地址:     " << this << endl;
    }
};

int MemoryDemo::staticVar = 0;

// ===================== 5. 静态const成员 =====================
class MathConstants {
public:
    // 静态const整型成员可以在类内初始化（C++11）
    static const int MAX_VALUE = 1000;
    static const int MIN_VALUE = -1000;
    static const double PI;  // 非整型需要类外初始化

    // constexpr static（C++11推荐）
    static constexpr double E = 2.71828;
    static constexpr float GOLDEN_RATIO = 1.618f;
};

const double MathConstants::PI = 3.14159265358979;

// ===================== 函数声明 =====================
void staticVarDemo();
void staticFuncDemo();
void singletonDemo();
void counterDemo();
void memoryDemo();
void staticConstDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 静态成员详解" << endl;
    cout << "========================================" << endl << endl;

    staticVarDemo();    // 静态成员变量
    staticFuncDemo();   // 静态成员函数
    singletonDemo();    // 单例模式
    counterDemo();      // 计数器示例
    memoryDemo();       // 内存位置
    staticConstDemo();  // 静态const

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== 静态成员变量 =====================
void staticVarDemo() {
    cout << "===== 1. 静态成员变量 =====" << endl;

    // 静态成员变量的特点：
    // 1. 所有对象共享同一份数据
    // 2. 在编译时分配内存（全局区/静态区）
    // 3. 类内声明，类外初始化
    // 4. 可以通过类名或对象访问

    cout << "--- 创建对象前 ---" << endl;
    cout << "  当前学生数: " << Student::getCount() << endl;  // 通过类名访问

    {
        Student s1("张三", 20);
        Student s2("李四", 19);
        Student s3("王五", 21);

        cout << "\n--- 3个对象在作用域内 ---" << endl;
        cout << "  当前学生数: " << Student::getCount() << endl;
        // 也可以通过对象访问静态成员（但不推荐，容易混淆）
        cout << "  通过对象访问: " << s1.getCount() << endl;

        cout << "\n--- 离开作用域 ---" << endl;
    }
    // s3, s2, s1依次析构

    cout << "\n--- 对象全部销毁后 ---" << endl;
    cout << "  当前学生数: " << Student::getCount() << endl;

    // 静态成员变量的本质：
    // 它是类的"全局变量"，被所有对象共享
    // 不占用对象的内存空间

    cout << endl;
}

// ===================== 静态成员函数 =====================
void staticFuncDemo() {
    cout << "===== 2. 静态成员函数 =====" << endl;

    // 静态成员函数的特点：
    // 1. 没有this指针
    // 2. 只能访问静态成员（变量和函数）
    // 3. 可以通过类名直接调用
    // 4. 不能是const（因为没有this指针）

    // 通过类名调用
    cout << "  学生数量: " << Student::getCount() << endl;
    cout << "  最大数量: " << Student::getMax() << endl;

    // 也可以通过对象调用（但不推荐）
    Student s("测试", 18);
    cout << "  通过对象: " << s.getCount() << endl;

    // 什么时候用静态成员函数？
    // 1. 工具函数：不需要访问对象数据
    // 2. 工厂方法：创建对象的静态方法
    // 3. 访问静态成员变量

    cout << endl;
}

// ===================== 单例模式 =====================
void singletonDemo() {
    cout << "===== 3. 单例模式 =====" << endl;

    // 单例模式确保一个类只有一个实例
    // 获取实例的唯一方式是调用getInstance()

    Singleton *s1 = Singleton::getInstance();
    s1->setData("第一次设置");
    s1->setValue(42);
    s1->show();

    Singleton *s2 = Singleton::getInstance();
    s2->show();  // s2和s1指向同一个实例

    // 验证是同一个实例
    cout << "  s1地址: " << s1 << endl;
    cout << "  s2地址: " << s2 << endl;
    cout << "  是否相同: " << (s1 == s2 ? "是" : "否") << endl;

    // 销毁单例
    Singleton::destroyInstance();

    // 单例模式的应用场景：
    // 1. 配置管理器
    // 2. 日志记录器
    // 3. 数据库连接池
    // 4. 线程池

    cout << endl;
}

// ===================== 计数器示例 =====================
void counterDemo() {
    cout << "===== 4. 自动分配ID示例 =====" << endl;

    // 利用静态变量实现自动递增的ID分配
    cout << "下一个可用ID: " << Product::getNextId() << endl;

    Product p1("苹果手机", 999.0);
    Product p2("华为手机", 899.0);
    Product p3("小米手机", 599.0);

    cout << "\n所有产品:" << endl;
    p1.show();
    p2.show();
    p3.show();

    cout << "\n下一个可用ID: " << Product::getNextId() << endl;

    // 静态变量在所有对象间共享，非常适合做计数器、ID分配器等

    cout << endl;
}

// ===================== 内存位置 =====================
void memoryDemo() {
    cout << "===== 5. 静态成员的内存位置 =====" << endl;

    MemoryDemo m1(10);
    MemoryDemo m2(20);

    cout << "--- m1 ---" << endl;
    m1.show();

    cout << "\n--- m2 ---" << endl;
    m2.show();

    // 观察地址：
    // - m1和m2的normalVar地址不同（各自在对象中）
    // - m1和m2的staticVar地址相同（共享同一个）
    // - staticVar的地址与对象地址差异大（在全局区/静态区）

    // 验证静态变量共享
    MemoryDemo::staticVar = 100;
    cout << "\n设置staticVar=100后:" << endl;
    cout << "  m1.staticVar = " << m1.staticVar << endl;
    cout << "  m2.staticVar = " << m2.staticVar << endl;
    // 两个对象看到的staticVar是同一个值

    cout << endl;
}

// ===================== 静态const成员 =====================
void staticConstDemo() {
    cout << "===== 6. 静态const成员 =====" << endl;

    // 静态const整型成员可以在类内初始化（C++11）
    cout << "  MAX_VALUE: " << MathConstants::MAX_VALUE << endl;
    cout << "  MIN_VALUE: " << MathConstants::MIN_VALUE << endl;

    // 非整型静态const需要类外初始化
    cout << "  PI: " << MathConstants::PI << endl;

    // constexpr static可以在类内初始化
    cout << "  E: " << MathConstants::E << endl;
    cout << "  GOLDEN_RATIO: " << MathConstants::GOLDEN_RATIO << endl;

    // 总结：
    // static const int = 类内初始化（推荐）
    // static const double = 类外初始化
    // static constexpr = 类内初始化（C++11推荐）

    cout << endl;
}
