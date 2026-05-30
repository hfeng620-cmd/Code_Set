/*
 * 91_shared_ptr智能指针.cpp
 * 本文件演示 C++ 智能指针 shared_ptr 和 weak_ptr 的用法
 * 包括: shared_ptr 共享所有权, make_shared, 引用计数,
 *       循环引用问题, weak_ptr 打破循环, 自定义删除器, 线程安全
 */

#include <iostream>
#include <memory>       // shared_ptr, weak_ptr, make_shared
#include <string>
#include <vector>
#include <functional>
using namespace std;

// 用于演示的类
class Resource {
public:
    Resource(const string& n) : name(n) {
        cout << "  [构造] Resource: " << name << endl;
    }

    ~Resource() {
        cout << "  [析构] Resource: " << name << endl;
    }

    string getName() const { return name; }

private:
    string name;
};

// 用于演示循环引用
class Node {
public:
    Node(const string& n) : name(n) {
        cout << "  [构造] Node: " << name << endl;
    }

    ~Node() {
        cout << "  [析构] Node: " << name << endl;
    }

    string name;

    // 错误方式: 使用 shared_ptr 互相引用（会导致循环引用）
    // shared_ptr<Node> next;
    // shared_ptr<Node> prev;

    // 正确方式: 一方使用 weak_ptr 打破循环
    shared_ptr<Node> next;     // 后继节点（拥有所有权）
    weak_ptr<Node> prev;       // 前驱节点（不拥有所有权，使用 weak_ptr）
};

// 用于演示自定义删除器
class Connection {
public:
    Connection(const string& h) : host(h) {
        cout << "  [连接] " << host << endl;
    }
    void send(const string& data) {
        cout << "  [发送] " << host << ": " << data << endl;
    }
    string host;
};

int main() {
    cout << "========== 1. shared_ptr 基本概念 ==========" << endl;
    // shared_ptr: 共享所有权的智能指针
    // 多个 shared_ptr 可以管理同一个对象
    // 使用引用计数: 当最后一个 shared_ptr 销毁时，对象才被释放

    // 创建 shared_ptr
    shared_ptr<Resource> sp1(new Resource("对象A"));
    cout << "sp1 引用计数: " << sp1.use_count() << endl;  // 1

    // 推荐: 使用 make_shared (更高效，只分配一次内存)
    auto sp2 = make_shared<Resource>("对象B");
    cout << "sp2 引用计数: " << sp2.use_count() << endl;  // 1

    cout << "\n========== 2. 引用计数机制 ==========" << endl;
    // 每次拷贝 shared_ptr，引用计数+1
    // 每次销毁 shared_ptr，引用计数-1
    // 引用计数为0时，对象被自动删除

    auto sp3 = make_shared<Resource>("共享对象");
    cout << "创建后引用计数: " << sp3.use_count() << endl;  // 1

    {
        // 拷贝: 引用计数 +1
        shared_ptr<Resource> sp4 = sp3;
        cout << "拷贝后引用计数: " << sp3.use_count() << endl;  // 2

        // 另一个拷贝
        shared_ptr<Resource> sp5 = sp3;
        cout << "再次拷贝后引用计数: " << sp3.use_count() << endl;  // 3

        // sp5 离开作用域，引用计数 -1
    }  // sp5 析构
    cout << "sp5 销毁后引用计数: " << sp3.use_count() << endl;  // 2

    // 赋值也会改变引用计数
    auto sp6 = make_shared<Resource>("另一个对象");
    cout << "sp3 引用计数: " << sp3.use_count() << endl;  // 2
    cout << "sp6 引用计数: " << sp6.use_count() << endl;  // 1

    shared_ptr<Resource> sp7 = sp3;
    cout << "sp3 引用计数: " << sp3.use_count() << endl;  // 3

    sp7 = sp6;  // sp7 不再指向 sp3 的对象，指向 sp6 的对象
    cout << "sp7 重新赋值后:" << endl;
    cout << "  sp3 引用计数: " << sp3.use_count() << endl;  // 2
    cout << "  sp6 引用计数: " << sp6.use_count() << endl;  // 2

    cout << "\n========== 3. make_shared 详解 ==========" << endl;
    // make_shared 比 shared_ptr<T>(new T()) 更高效
    // 因为 make_shared 只分配一次内存（对象+控制块）
    // 而 new + shared_ptr 会分配两次内存

    // make_shared 基本用法
    auto ms1 = make_shared<string>("Hello");
    auto ms2 = make_shared<int>(42);
    auto ms3 = make_shared<vector<int>>(10, 5);  // 10个元素，每个值为5

    cout << "string: " << *ms1 << endl;
    cout << "int: " << *ms2 << endl;
    cout << "vector: ";
    for (auto v : *ms3) cout << v << " ";
    cout << endl;

    // make_shared 的优点:
    // 1. 异常安全: 避免在参数求值时的内存泄漏
    //    func(shared_ptr<A>(new A()), compute());  // 如果 compute() 抛异常，A 可能泄漏
    //    func(make_shared<A>(), compute());         // 安全
    // 2. 性能: 只分配一次内存
    // 3. 减少内存碎片

    // make_shared 的缺点:
    // 无法指定自定义删除器（需要用 shared_ptr 构造函数）

    cout << "\n========== 4. shared_ptr 常用操作 ==========" << endl;
    auto sp = make_shared<Resource>("操作演示");

    // get(): 获取裸指针
    Resource* raw = sp.get();
    cout << "get() 返回: " << raw->getName() << endl;

    // -> 和 * 运算符
    cout << "-> 访问: " << sp->getName() << endl;
    cout << "* 访问: " << (*sp).getName() << endl;

    // use_count(): 引用计数
    cout << "use_count(): " << sp.use_count() << endl;

    // unique(): 是否独占（C++17 已弃用，建议用 use_count() == 1）
    cout << "是否独占: " << (sp.use_count() == 1 ? "是" : "否") << endl;

    // reset(): 释放当前对象
    auto sp8 = make_shared<Resource>("待重置");
    cout << "reset 前引用计数: " << sp8.use_count() << endl;
    sp8.reset();  // 释放对象
    cout << "reset 后: " << (sp8 ? "非空" : "空") << endl;

    // reset 带参数: 释放旧对象，管理新对象
    auto sp9 = make_shared<Resource>("旧对象");
    sp9.reset(new Resource("新对象"));
    cout << "reset 后: " << sp9->getName() << endl;

    cout << "\n========== 5. shared_ptr 赋值和拷贝 ==========" << endl;
    // shared_ptr 支持拷贝和赋值（与 unique_ptr 不同）

    auto p1 = make_shared<Resource>("P1");
    auto p2 = make_shared<Resource>("P2");

    cout << "p1: " << p1->getName() << " (count=" << p1.use_count() << ")" << endl;
    cout << "p2: " << p2->getName() << " (count=" << p2.use_count() << ")" << endl;

    // 拷贝
    auto p3 = p1;  // p1 和 p3 指向同一对象
    cout << "p3 = p1 后:" << endl;
    cout << "  p1 count: " << p1.use_count() << endl;  // 2
    cout << "  p3 count: " << p3.use_count() << endl;  // 2

    // 赋值
    p3 = p2;  // p3 不再指向 P1，改为指向 P2
    cout << "p3 = p2 后:" << endl;
    cout << "  p1 count: " << p1.use_count() << endl;  // 1
    cout << "  p2 count: " << p2.use_count() << endl;  // 2
    cout << "  p3 count: " << p3.use_count() << endl;  // 2

    cout << "\n========== 6. 循环引用问题 ==========" << endl;
    // 循环引用是 shared_ptr 的经典问题
    // 当两个或多个对象互相持有对方的 shared_ptr 时，
    // 引用计数永远不会为0，导致内存泄漏

    cout << "--- 错误示例（循环引用，已注释）---" << endl;
    // 如果 Node 的 prev 也是 shared_ptr:
    // shared_ptr<Node> nodeA(new Node("A"));
    // shared_ptr<Node> nodeB(new Node("B"));
    // nodeA->next = nodeB;  // A -> B
    // nodeB->prev = nodeA;  // B -> A (如果用 shared_ptr)
    // nodeA 引用计数: 2 (自身 + nodeB->prev)
    // nodeB 引用计数: 2 (自身 + nodeA->next)
    // 两者离开作用域后引用计数仍为1，永远不会析构!

    cout << "\n--- 正确示例（使用 weak_ptr 打破循环）---" << endl;
    {
        auto nodeA = make_shared<Node>("A");
        auto nodeB = make_shared<Node>("B");

        // 建立双向链接
        nodeA->next = nodeB;   // A 拥有 B
        nodeB->prev = nodeA;   // B 观察 A（不拥有）

        cout << "nodeA 引用计数: " << nodeA.use_count() << endl;  // 1
        cout << "nodeB 引用计数: " << nodeB.use_count() << endl;  // 2 (nodeA->next)

        // 通过 weak_ptr 访问对象
        if (auto prevNode = nodeB->prev.lock()) {
            cout << "B 的前驱: " << prevNode->name << endl;
        }
    }  // nodeA 和 nodeB 在这里正常析构

    cout << "\n========== 7. weak_ptr 详解 ==========" << endl;
    // weak_ptr: 弱引用，不增加引用计数
    // 用于观察 shared_ptr 管理的对象，但不拥有它
    // 不能直接访问对象，必须先 lock() 转换为 shared_ptr

    weak_ptr<Resource> wp;
    {
        auto spInner = make_shared<Resource>("临时对象");
        wp = spInner;  // weak_ptr 观察对象

        cout << "作用域内:" << endl;
        cout << "  spInner 引用计数: " << spInner.use_count() << endl;  // 1 (weak_ptr 不计入)
        cout << "  wp expired: " << wp.expired() << endl;  // false

        // lock(): 尝试获取 shared_ptr
        if (auto locked = wp.lock()) {
            cout << "  lock 成功: " << locked->getName() << endl;
            cout << "  lock 后引用计数: " << locked.use_count() << endl;  // 2
        }

        // use_count(): weak_ptr 也能查看引用计数
        cout << "  wp.use_count(): " << wp.use_count() << endl;  // 1
    }  // spInner 销毁，对象被释放

    cout << "作用域外:" << endl;
    cout << "  wp expired: " << wp.expired() << endl;  // true，对象已销毁

    // lock 一个已过期的 weak_ptr
    if (auto locked = wp.lock()) {
        cout << "  对象仍然存在" << endl;
    } else {
        cout << "  对象已不存在，lock 返回空 shared_ptr" << endl;
    }

    // reset: 释放 weak_ptr 的观察
    wp.reset();
    cout << "  wp reset 后 expired: " << wp.expired() << endl;

    cout << "\n========== 8. 自定义删除器 ==========" << endl;
    // shared_ptr 可以指定自定义删除器
    // 注意: 删除器类型不包含在 shared_ptr 类型中（与 unique_ptr 不同）

    // Lambda 删除器
    {
        shared_ptr<Connection> conn(
            new Connection("数据库"),
            [](Connection* c) {
                cout << "  [断开连接] " << c->host << endl;
                delete c;
            }
        );
        conn->send("SELECT * FROM users");
    }  // 自动调用删除器

    // 函数指针删除器
    {
        auto deleter = [](Connection* c) {
            cout << "  [关闭连接] " << c->host << endl;
            delete c;
        };
        shared_ptr<Connection> conn(new Connection("Redis"), deleter);
        conn->send("GET key");
    }

    // shared_ptr 管理数组（C++17 之前需要自定义删除器）
    {
        // C++11/14 方式: 自定义删除器
        shared_ptr<int> arr(new int[10], [](int* p) {
            cout << "  [删除数组]" << endl;
            delete[] p;
        });

        // C++17 方式: 直接支持数组
        // shared_ptr<int[]> arr2(new int[10]);  // C++17
    }

    // make_shared 无法指定自定义删除器
    // 如果需要自定义删除器，必须使用 shared_ptr 构造函数

    cout << "\n========== 9. shared_ptr 的陷阱 ==========" << endl;
    // 陷阱1: 不要从裸指针创建多个 shared_ptr
    // Resource* rawPtr = new Resource("危险");
    // shared_ptr<Resource> sp_a(rawPtr);
    // shared_ptr<Resource> sp_b(rawPtr);  // 错误! 两个独立的引用计数
    // 两者都会尝试 delete 同一对象 -> double delete

    // 陷阱2: 不要用 get() 返回的裸指针创建 shared_ptr
    // auto sp = make_shared<Resource>("对象");
    // shared_ptr<Resource> bad(sp.get());  // 错误!

    // 陷阱3: 不要在函数中返回 make_shared 对象的裸指针
    // Resource* bad_func() { auto sp = make_shared<Resource>("temp"); return sp.get(); }
    // 返回的指针会悬空!

    cout << "陷阱已注释说明，请查看源码" << endl;

    cout << "\n========== 10. 线程安全说明 ==========" << endl;
    // shared_ptr 的引用计数操作是线程安全的（原子操作）
    // 但对象本身的访问不是线程安全的

    // 线程安全的操作:
    // - 多线程同时拷贝/销毁同一个 shared_ptr（引用计数安全）
    // - 多线程同时读取同一个 shared_ptr（不修改引用计数）

    // 不是线程安全的操作:
    // - 多线程同时读写同一个 shared_ptr 对象本身
    //   （例如一个线程 reset，另一个线程读取）
    // - 多线程同时读写 shared_ptr 管理的资源
    //   （需要额外的互斥锁保护）

    cout << "shared_ptr 引用计数是线程安全的（原子操作）" << endl;
    cout << "但对象本身不是线程安全的，需要额外加锁" << endl;

    cout << "\n========== 11. enable_shared_from_this ==========" << endl;
    // 当类的成员函数需要返回指向自身的 shared_ptr 时
    // 使用 enable_shared_from_this 避免重复创建控制块

    class MyClass : public enable_shared_from_this<MyClass> {
    public:
        MyClass(const string& n) : name(n) {}

        // 安全地获取指向自身的 shared_ptr
        shared_ptr<MyClass> getSelf() {
            return shared_from_this();
        }

        string name;
    };

    {
        auto obj = make_shared<MyClass>("自引用");
        auto self = obj->getSelf();
        cout << "对象名: " << self->name << endl;
        cout << "引用计数: " << obj.use_count() << endl;  // 2
    }

    cout << "\n========== 12. 使用场景建议 ==========" << endl;
    cout << "shared_ptr 适用场景:" << endl;
    cout << "1. 多个持有者共享同一资源（如缓存、观察者模式）" << endl;
    cout << "2. 所有权关系不确定（无法确定谁最后释放）" << endl;
    cout << "3. 需要在容器和函数间共享对象" << endl;
    cout << endl;
    cout << "优先使用 unique_ptr 的场景:" << endl;
    cout << "1. 明确的单一所有者" << endl;
    cout << "2. 工厂函数返回对象" << endl;
    cout << "3. Pimpl 惯用法" << endl;
    cout << "4. 性能敏感场景（unique_ptr 无额外开销）" << endl;
    cout << endl;
    cout << "使用 weak_ptr 的场景:" << endl;
    cout << "1. 打破循环引用" << endl;
    cout << "2. 缓存（不阻止对象释放）" << endl;
    cout << "3. 观察者（不拥有被观察对象）" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
