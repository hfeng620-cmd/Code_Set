/*
 ============================================================================
 文件名 : 67_虚析构函数详解.cpp
 描  述 : 详细讲解C++虚析构函数的原理和使用
          包括：为什么需要虚析构函数、不使用虚析构导致的内存泄漏问题
          基类指针删除派生类对象的问题、virtual ~ClassName() 语法
          何时需要使用虚析构函数
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 ============================================================================
*/

#include <iostream>
#include <string>
using namespace std;

// ==================== 1. 没有虚析构函数的问题演示 ====================
// 这个基类没有虚析构函数
class BaseNoVirtual {
public:
    BaseNoVirtual() {
        cout << "[BaseNoVirtual] 构造函数" << endl;
    }
    // 注意：这里没有 virtual 关键字！
    ~BaseNoVirtual() {
        cout << "[BaseNoVirtual] 析构函数" << endl;
    }
};

class DerivedNoVirtual : public BaseNoVirtual {
private:
    int* m_data;  // 动态分配的资源

public:
    DerivedNoVirtual() {
        cout << "[DerivedNoVirtual] 构造函数" << endl;
        m_data = new int[100];  // 分配内存
        cout << "  -> 分配了 100 个 int 的内存" << endl;
    }

    ~DerivedNoVirtual() {
        cout << "[DerivedNoVirtual] 析构函数" << endl;
        delete[] m_data;  // 释放内存
        cout << "  -> 释放了 100 个 int 的内存" << endl;
    }
};

// ==================== 2. 有虚析构函数的正确版本 ====================
class BaseWithVirtual {
public:
    BaseWithVirtual() {
        cout << "[BaseWithVirtual] 构造函数" << endl;
    }

    // 虚析构函数：在析构函数前加 virtual 关键字
    virtual ~BaseWithVirtual() {
        cout << "[BaseWithVirtual] 析构函数" << endl;
    }
};

class DerivedWithVirtual : public BaseWithVirtual {
private:
    int* m_data;

public:
    DerivedWithVirtual() {
        cout << "[DerivedWithVirtual] 构造函数" << endl;
        m_data = new int[100];
        cout << "  -> 分配了 100 个 int 的内存" << endl;
    }

    ~DerivedWithVirtual() override {
        cout << "[DerivedWithVirtual] 析构函数" << endl;
        delete[] m_data;
        cout << "  -> 释放了 100 个 int 的内存" << endl;
    }
};

// ==================== 3. 实际项目中的例子 ====================
// 模拟一个资源管理的场景

// 资源基类（带虚析构函数）
class Resource {
protected:
    string m_name;

public:
    Resource(const string& name) : m_name(name) {
        cout << "[Resource] 获取资源: " << m_name << endl;
    }

    // 虚析构函数确保资源正确释放
    virtual ~Resource() {
        cout << "[Resource] 释放资源: " << m_name << endl;
    }

    virtual void use() const = 0;  // 纯虚函数
};

// 文件资源
class FileResource : public Resource {
private:
    string m_filename;
    char* m_buffer;  // 模拟文件缓冲区

public:
    FileResource(const string& filename)
        : Resource("文件"), m_filename(filename) {
        m_buffer = new char[1024];  // 分配1KB缓冲区
        cout << "  -> 打开文件: " << m_filename
             << ", 分配1KB缓冲区" << endl;
    }

    ~FileResource() override {
        cout << "  -> 关闭文件: " << m_filename
             << ", 释放缓冲区" << endl;
        delete[] m_buffer;
    }

    void use() const override {
        cout << "正在使用文件: " << m_filename << endl;
    }
};

// 网络资源
class NetworkResource : public Resource {
private:
    string m_url;
    int* m_socket;  // 模拟套接字

public:
    NetworkResource(const string& url)
        : Resource("网络"), m_url(url) {
        m_socket = new int(8080);  // 模拟打开套接字
        cout << "  -> 连接到: " << m_url
             << ", 套接字: " << *m_socket << endl;
    }

    ~NetworkResource() override {
        cout << "  -> 断开连接: " << m_url
             << ", 关闭套接字" << endl;
        delete m_socket;
    }

    void use() const override {
        cout << "正在访问网络: " << m_url << endl;
    }
};

// ==================== 4. 何时需要虚析构函数 ====================
// 规则：只要一个类可能被继承，并且通过基类指针删除派生类对象，
//       就应该将基类的析构函数声明为虚析构函数

// 不需要虚析构函数的情况：类不会被继承
class Utility {
public:
    Utility() = default;
    ~Utility() = default;  // 普通析构函数就够了，不会被继承
};

// 需要虚析构函数的情况：作为多态基类使用
class Animal {
public:
    Animal() = default;
    virtual ~Animal() = default;  // 虚析构函数
    virtual void speak() const = 0;
};

class Dog : public Animal {
private:
    string m_name;

public:
    Dog(const string& name) : m_name(name) {
        cout << "小狗 " << m_name << " 出生了" << endl;
    }
    ~Dog() override {
        cout << "小狗 " << m_name << " 离开了" << endl;
    }
    void speak() const override {
        cout << m_name << ": 汪汪汪!" << endl;
    }
};

class Cat : public Animal {
private:
    string m_name;

public:
    Cat(const string& name) : m_name(name) {
        cout << "小猫 " << m_name << " 出生了" << endl;
    }
    ~Cat() override {
        cout << "小猫 " << m_name << " 离开了" << endl;
    }
    void speak() const override {
        cout << m_name << ": 喵喵喵!" << endl;
    }
};

// ==================== 5. 演示函数 ====================
void demonstrateNoVirtualDtor() {
    cout << "\n===== 没有虚析构函数的情况 =====" << endl;
    cout << "基类指针指向派生类对象，然后 delete:" << endl;
    BaseNoVirtual* ptr = new DerivedNoVirtual();
    delete ptr;  // 问题：只调用了基类的析构函数！
    // DerivedNoVirtual 的析构函数没有被调用
    // 导致 m_data 分配的 100 个 int 的内存泄漏！
    cout << "*** 注意：DerivedNoVirtual 的析构函数没有被调用！***" << endl;
    cout << "*** 内存泄漏了！***" << endl;
}

void demonstrateWithVirtualDtor() {
    cout << "\n===== 有虚析构函数的情况 =====" << endl;
    cout << "基类指针指向派生类对象，然后 delete:" << endl;
    BaseWithVirtual* ptr = new DerivedWithVirtual();
    delete ptr;  // 正确：先调用派生类析构，再调用基类析构
    cout << "*** 派生类和基类的析构函数都被正确调用了！***" << endl;
}

void demonstrateResourceManagement() {
    cout << "\n===== 资源管理示例 =====" << endl;

    // 基类指针数组管理不同类型的资源
    Resource* resources[3];
    resources[0] = new FileResource("data.txt");
    resources[1] = new NetworkResource("https://example.com");
    resources[2] = new FileResource("config.ini");

    cout << "\n使用资源:" << endl;
    for (int i = 0; i < 3; i++) {
        resources[i]->use();
    }

    cout << "\n释放资源:" << endl;
    for (int i = 0; i < 3; i++) {
        delete resources[i];  // 虚析构确保正确释放
    }
}

void demonstrateAnimalPolymorphism() {
    cout << "\n===== 多态动物示例 =====" << endl;

    Animal* animals[3];
    animals[0] = new Dog("旺财");
    animals[1] = new Cat("咪咪");
    animals[2] = new Dog("大黄");

    cout << "\n动物们说话:" << endl;
    for (int i = 0; i < 3; i++) {
        animals[i]->speak();
    }

    cout << "\n动物们离开了:" << endl;
    for (int i = 0; i < 3; i++) {
        delete animals[i];  // 虚析构确保每个派生类析构函数被调用
    }
}

// ==================== 6. 主函数 ====================
int main() {
    cout << "========================================" << endl;
    cout << "     虚析构函数详解 演示" << endl;
    cout << "========================================" << endl;

    // 6.1 没有虚析构函数 —— 内存泄漏！
    demonstrateNoVirtualDtor();

    // 6.2 有虚析构函数 —— 正确释放
    demonstrateWithVirtualDtor();

    // 6.3 资源管理示例
    demonstrateResourceManagement();

    // 6.4 动物多态示例
    demonstrateAnimalPolymorphism();

    // 6.5 栈上对象不受影响
    cout << "\n===== 栈上对象不受虚析构影响 =====" << endl;
    {
        DerivedNoVirtual d;  // 栈上对象，离开作用域时会正确调用析构函数
        cout << "栈上对象会在离开作用域时正确析构" << endl;
    }
    cout << "栈上对象已正确析构\n" << endl;

    cout << "========================================" << endl;
    cout << "  总结：" << endl;
    cout << "  1. 基类指针 delete 派生类对象时，" << endl;
    cout << "     如果析构函数不是虚函数，只调用基类析构" << endl;
    cout << "  2. 这会导致派生类的资源泄漏" << endl;
    cout << "  3. 解决方案：将基类析构函数声明为 virtual" << endl;
    cout << "  4. 有纯虚函数的类也应该有虚析构函数" << endl;
    cout << "========================================" << endl;

    return 0;
}
