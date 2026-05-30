/*
 * 90_unique_ptr智能指针.cpp
 * 本文件演示 C++ 智能指针 unique_ptr 的用法
 * 包括: RAII 概念, unique_ptr 独占所有权, make_unique, 移动语义,
 *       自定义删除器, 数组特化, 使用场景
 * 注意: make_unique 是 C++14 特性，但本文件展示其概念
 */

#include <iostream>
#include <memory>       // unique_ptr, make_unique
#include <string>
#include <vector>
#include <functional>   // function
using namespace std;

// 用于演示的类
class MyClass {
public:
    MyClass(const string& n) : name(n) {
        cout << "  [构造] " << name << endl;
    }

    ~MyClass() {
        cout << "  [析构] " << name << endl;
    }

    void greet() const {
        cout << "  你好，我是 " << name << endl;
    }

    string getName() const { return name; }

private:
    string name;
};

// 用于演示自定义删除器
class File {
public:
    File(const string& n) : name(n) {
        cout << "  [打开文件] " << name << endl;
    }
    void write(const string& content) {
        cout << "  [写入] " << name << ": " << content << endl;
    }
    string name;
};

// 文件关闭器（函子）
struct FileCloser {
    void operator()(File* f) const {
        if (f) {
            cout << "  [关闭文件] " << f->name << endl;
            delete f;
        }
    }
};

// 工厂函数示例
unique_ptr<MyClass> createObject(const string& name) {
    return make_unique<MyClass>(name);
}

// 接收 unique_ptr 的函数（转移所有权）
void takeOwnership(unique_ptr<MyClass> ptr) {
    cout << "获取所有权: ";
    ptr->greet();
    // ptr 在函数结束时自动销毁
}

// 接收引用的函数（不转移所有权）
void useObject(const unique_ptr<MyClass>& ptr) {
    cout << "使用对象（不获取所有权）: ";
    ptr->greet();
}

// 接收裸指针的函数（不推荐，但有时需要）
void useRawPointer(MyClass* ptr) {
    if (ptr) {
        cout << "使用裸指针: ";
        ptr->greet();
    }
}

int main() {
    cout << "========== 1. 什么是 RAII ==========" << endl;
    // RAII (Resource Acquisition Is Initialization)
    // 资源获取即初始化
    // 核心思想: 在构造函数中获取资源，在析构函数中释放资源
    // 利用栈对象的自动析构来保证资源不泄漏

    // 传统方式的问题: 容易忘记 delete
    {
        // MyClass* p = new MyClass("传统方式");
        // p->greet();
        // 如果这里抛出异常，delete 就不会执行！
        // delete p;  // 容易忘记
    }

    // RAII 方式: 使用智能指针自动管理
    {
        auto p = make_unique<MyClass>("RAII方式");
        p->greet();
        // 即使抛出异常，p 也会自动析构，释放内存
    }  // p 在这里自动析构

    cout << "\n========== 2. unique_ptr 基本用法 ==========" << endl;
    // unique_ptr: 独占所有权的智能指针
    // 同一时刻只能有一个 unique_ptr 拥有某个对象

    // 创建 unique_ptr
    unique_ptr<MyClass> ptr1(new MyClass("直接构造"));
    ptr1->greet();

    // 推荐: 使用 make_unique (C++14)
    // 优点: 1. 异常安全  2. 代码更简洁  3. 只分配一次内存
    auto ptr2 = make_unique<MyClass>("make_unique构造");
    ptr2->greet();

    // 访问成员
    cout << "名称: " << ptr2->getName() << endl;    // -> 运算符
    cout << "名称: " << (*ptr2).getName() << endl;   // * 解引用

    // 检查是否为空
    unique_ptr<MyClass> ptr3;  // 默认构造，指向 nullptr
    if (ptr3) {
        cout << "ptr3 不为空" << endl;
    } else {
        cout << "ptr3 为空 (nullptr)" << endl;
    }

    if (ptr2 != nullptr) {
        cout << "ptr2 不为空" << endl;
    }

    cout << "\n========== 3. 独占所有权: 不能拷贝，只能移动 ==========" << endl;
    // unique_ptr 不能拷贝（拷贝构造和拷贝赋值被删除）
    // 只能通过 move 转移所有权

    auto ptr4 = make_unique<MyClass>("对象A");
    // unique_ptr<MyClass> ptr5 = ptr4;  // 编译错误! 不能拷贝
    // unique_ptr<MyClass> ptr5(ptr4);   // 编译错误! 不能拷贝

    // 使用 std::move 转移所有权
    unique_ptr<MyClass> ptr5 = std::move(ptr4);
    // ptr4 现在为 nullptr，ptr5 拥有对象
    cout << "move 后 ptr4: " << (ptr4 ? "非空" : "空") << endl;
    cout << "move 后 ptr5: " << (ptr5 ? "非空" : "空") << endl;
    ptr5->greet();

    // reset: 释放当前对象，可以接受新对象
    ptr5.reset(new MyClass("对象B"));
    cout << "reset 后: ";
    ptr5->greet();

    // reset 不带参数: 释放当前对象，变为 nullptr
    ptr5.reset();
    cout << "reset() 后 ptr5: " << (ptr5 ? "非空" : "空") << endl;

    // release: 放弃所有权，返回裸指针（调用者负责释放）
    auto ptr6 = make_unique<MyClass>("对象C");
    MyClass* rawPtr = ptr6.release();  // ptr6 变为 nullptr
    cout << "release 后 ptr6: " << (ptr6 ? "非空" : "空") << endl;
    delete rawPtr;  // 必须手动释放!

    cout << "\n========== 4. make_unique 详解 ==========" << endl;
    // make_unique 是 C++14 引入的工厂函数
    // 语法: make_unique<类型>(构造参数...)

    // 基本用法
    auto p1 = make_unique<MyClass>("make_unique基本");

    // 带多个构造参数（假设 MyClass 有相应构造函数）
    // auto p2 = make_unique<MyClass>("name", 42, 3.14);

    // 创建数组 (C++14)
    auto arr = make_unique<int[]>(5);  // 创建 int[5]，自动初始化为0
    for (int i = 0; i < 5; ++i) {
        arr[i] = i * 10;
    }
    cout << "数组: ";
    for (int i = 0; i < 5; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // make_unique vs new 的优势:
    // 1. 异常安全:
    //    func(unique_ptr<A>(new A()), unique_ptr<B>(new B()));
    //    如果 B 的构造抛出异常，A 可能泄漏
    //    func(make_unique<A>(), make_unique<B>());  // 安全
    // 2. 代码更简洁
    // 3. make_unique 只分配一次内存（包含控制块+对象）

    cout << "\n========== 5. unique_ptr 转移所有权 ==========" << endl;
    // 转移所有权的场景

    // 场景1: 工厂函数返回 unique_ptr
    auto factoryResult = createObject("工厂创建");
    factoryResult->greet();

    // 场景2: 将所有权转移给函数
    auto transferPtr = make_unique<MyClass>("待转移");
    takeOwnership(std::move(transferPtr));
    // transferPtr 现在为 nullptr
    cout << "转移后 transferPtr: " << (transferPtr ? "非空" : "空") << endl;

    // 场景3: 存入容器
    vector<unique_ptr<MyClass>> vec;
    vec.push_back(make_unique<MyClass>("容器对象1"));
    vec.push_back(make_unique<MyClass>("容器对象2"));
    vec.push_back(createObject("容器对象3"));

    cout << "容器中的对象:" << endl;
    for (const auto& item : vec) {
        item->greet();
    }

    // 场景4: 从容器中取出
    auto extracted = std::move(vec[0]);
    cout << "取出后: ";
    extracted->greet();
    cout << "原位置: " << (vec[0] ? "非空" : "空") << endl;

    cout << "\n========== 6. 自定义删除器 ==========" << endl;
    // unique_ptr 可以指定自定义删除器
    // 语法: unique_ptr<类型, 删除器类型>

    // 方式一: 使用函数指针
    {
        auto deleter = [](File* f) {
            cout << "  Lambda 删除器: 关闭 " << f->name << endl;
            delete f;
        };
        unique_ptr<File, decltype(deleter)> filePtr(
            new File("test.txt"), deleter);
        filePtr->write("Hello");
    }  // 自动调用 deleter

    // 方式二: 使用函子
    {
        unique_ptr<File, FileCloser> filePtr(new File("data.bin"));
        filePtr->write("Binary data");
    }  // 自动调用 FileCloser

    // 方式三: 使用 std::function
    {
        function<void(File*)> closer = [](File* f) {
            cout << "  function 删除器: " << f->name << endl;
            delete f;
        };
        unique_ptr<File, function<void(File*)>> filePtr(
            new File("log.txt"), closer);
        filePtr->write("Log entry");
    }

    cout << "\n========== 7. 数组特化 ==========" << endl;
    // unique_ptr<T[]> 数组版本使用 delete[] 而不是 delete

    // 创建动态数组
    unique_ptr<int[]> arrPtr(new int[5]);
    for (int i = 0; i < 5; ++i) {
        arrPtr[i] = i * 100;  // 使用 [] 运算符访问
    }

    cout << "数组内容: ";
    for (int i = 0; i < 5; ++i) {
        cout << arrPtr[i] << " ";
    }
    cout << endl;

    // make_unique 数组版本 (C++14)
    auto arrPtr2 = make_unique<double[]>(3);
    arrPtr2[0] = 1.1;
    arrPtr2[1] = 2.2;
    arrPtr2[2] = 3.3;

    cout << "double 数组: ";
    for (int i = 0; i < 3; ++i) {
        cout << arrPtr2[i] << " ";
    }
    cout << endl;

    // 注意: unique_ptr<T[]> 没有 -> 和 * 运算符
    // 只能用 [] 访问

    cout << "\n========== 8. 常用成员函数 ==========" << endl;
    auto memberPtr = make_unique<MyClass>("测试对象");

    // get(): 获取裸指针（不转移所有权）
    MyClass* raw = memberPtr.get();
    cout << "get() 返回: ";
    raw->greet();

    // operator bool / operator!=: 检查是否为空
    cout << "bool 转换: " << (memberPtr ? "非空" : "空") << endl;

    // reset(): 释放当前对象
    memberPtr.reset(new MyClass("新对象"));
    cout << "reset 后: ";
    memberPtr->greet();

    // swap(): 交换两个 unique_ptr
    auto a = make_unique<MyClass>("A");
    auto b = make_unique<MyClass>("B");
    cout << "交换前: a=" << a->getName() << ", b=" << b->getName() << endl;
    a.swap(b);
    cout << "交换后: a=" << a->getName() << ", b=" << b->getName() << endl;

    cout << "\n========== 9. 使用场景建议 ==========" << endl;
    cout << "unique_ptr 适用场景:" << endl;
    cout << "1. 动态分配的对象需要在作用域结束时自动释放" << endl;
    cout << "2. 工厂函数返回动态创建的对象" << endl;
    cout << "3. 容器中存储多态对象" << endl;
    cout << "4. Pimpl 惯用法（隐藏实现细节）" << endl;
    cout << "5. 管理非内存资源（文件句柄、网络连接等）" << endl;
    cout << endl;
    cout << "何时使用裸指针:" << endl;
    cout << "1. 非拥有的观察者（不负责释放）" << endl;
    cout << "2. 函数参数（不转移所有权时用引用或裸指针）" << endl;
    cout << "3. 性能极度敏感的场景" << endl;

    cout << "\n========== 10. 常见错误 ==========" << endl;
    // 错误1: 对 unique_ptr 使用拷贝
    // auto p1 = make_unique<MyClass>("obj");
    // auto p2 = p1;  // 编译错误!

    // 错误2: 忘记 move
    // void func(unique_ptr<MyClass> p);
    // auto p = make_unique<MyClass>("obj");
    // func(p);  // 编译错误! 应该用 func(std::move(p));

    // 错误3: unique_ptr 和裸指针混用
    // auto p = make_unique<MyClass>("obj");
    // MyClass* raw = p.get();
    // delete raw;  // 错误! 会导致 double delete
    // p 也会在析构时 delete，同一内存被 delete 两次

    // 错误4: 从裸指针创建多个 unique_ptr
    // MyClass* raw = new MyClass("obj");
    // unique_ptr<MyClass> p1(raw);
    // unique_ptr<MyClass> p2(raw);  // 错误! 两个 unique_ptr 管理同一内存

    // 错误5: 返回局部 unique_ptr 的 get()
    // unique_ptr<MyClass> func() { auto p = make_unique<MyClass>("obj"); return p; }
    // MyClass* dangling = func().get();  // 悬空指针! 临时 unique_ptr 已析构

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
