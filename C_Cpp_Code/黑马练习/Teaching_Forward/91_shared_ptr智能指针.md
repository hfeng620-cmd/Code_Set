# shared_ptr 智能指针详解

## 一、为什么需要智能指针？

在 C++ 中，使用 `new` 分配的内存必须手动 `delete`，否则会造成**内存泄漏**。

```cpp
#include <iostream>
using namespace std;

void badFunction() {
    int* p = new int(100);
    // 如果这里发生异常或提前return，delete就不会执行
    // 导致内存泄漏！
    if (*p > 50) {
        return; // 内存泄漏！
    }
    delete p;
}

// 智能指针可以自动管理内存，离开作用域时自动释放
void goodFunction() {
    shared_ptr<int> p = make_shared<int>(100);
    if (*p > 50) {
        return; // 没问题！p离开作用域时自动释放内存
    }
    // 离开作用域自动释放
}

int main() {
    badFunction();  // 内存泄漏
    goodFunction(); // 安全，自动释放
    return 0;
}
```

### 智能指针的分类

| 智能指针 | 说明 | 头文件 |
|---------|------|--------|
| `shared_ptr` | 共享所有权，引用计数 | `<memory>` |
| `unique_ptr` | 独占所有权，不可复制 | `<memory>` |
| `weak_ptr` | 弱引用，不拥有对象 | `<memory>` |

## 二、shared_ptr 的特点

`shared_ptr` 是**共享所有权**的智能指针，多个 `shared_ptr` 可以指向同一个对象。

### 核心机制：引用计数

| 操作 | 引用计数变化 |
|------|-------------|
| 创建/复制 | +1 |
| 销毁/重置 | -1 |
| 计数为0 | 自动释放内存 |

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 创建一个shared_ptr
    shared_ptr<int> p1 = make_shared<int>(42);
    cout << "p1引用计数: " << p1.use_count() << endl; // 1

    {
        // 复制shared_ptr，引用计数+1
        shared_ptr<int> p2 = p1;
        cout << "复制后p1引用计数: " << p1.use_count() << endl; // 2
        cout << "p2引用计数: " << p2.use_count() << endl;       // 2
        cout << "p1的值: " << *p1 << endl;  // 42
        cout << "p2的值: " << *p2 << endl;  // 42（同一块内存）
    } // p2离开作用域，引用计数-1

    cout << "p2销毁后p1引用计数: " << p1.use_count() << endl; // 1

    return 0;
} // p1离开作用域，引用计数变为0，自动释放内存
```

## 三、创建 shared_ptr（两种方式）

### 方式1：make_shared（推荐）

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

int main() {
    // ========== make_shared：推荐方式 ==========

    // 创建 int 类型
    auto p1 = make_shared<int>(42);
    cout << "*p1 = " << *p1 << endl; // 42

    // 创建 string 类型
    auto p2 = make_shared<string>("Hello, World!");
    cout << "*p2 = " << *p2 << endl; // Hello, World!

    // 创建数组（C++20才支持make_shared用于数组）
    // C++11/14/17 中数组要用 new + shared_ptr
    shared_ptr<int[]> p3(new int[5]{1, 2, 3, 4, 5});
    for (int i = 0; i < 5; i++) {
        cout << p3[i] << " "; // 1 2 3 4 5
    }
    cout << endl;

    // make_shared 的优势：
    // 1. 更高效（一次内存分配，对象和控制块一起分配）
    // 2. 更安全（避免异常安全问题）
    // 3. 代码更简洁

    return 0;
}
```

### 方式2：new + shared_ptr

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // ========== 用 new 创建 ==========

    // 方式2a：直接构造
    shared_ptr<int> p1(new int(100));
    cout << "*p1 = " << *p1 << endl; // 100

    // 方式2b：用已有指针构造
    int* raw = new int(200);
    shared_ptr<int> p2(raw);
    cout << "*p2 = " << *p2 << endl; // 200
    // 注意：之后不要手动 delete raw，否则会双重释放！

    // ========== 危险！不能这样做 ==========
    // int x = 42;
    // shared_ptr<int> p3(&x); // 错误！不能用栈上的地址

    // ========== 危险！不能用同一个裸指针创建多个shared_ptr ==========
    // int* raw2 = new int(300);
    // shared_ptr<int> p4(raw2);
    // shared_ptr<int> p5(raw2); // 严重错误！两个独立的引用计数
    // 离开作用域时会 double free！

    // ========== 推荐：优先使用 make_shared ==========
    auto p6 = make_shared<int>(42); // 最佳方式

    return 0;
}
```

## 四、引用计数详解

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 创建
    auto p1 = make_shared<int>(10);
    cout << "创建p1: use_count=" << p1.use_count() << endl; // 1

    // 复制（引用计数+1）
    shared_ptr<int> p2 = p1;
    cout << "p2=p1: use_count=" << p1.use_count() << endl; // 2

    shared_ptr<int> p3 = p1;
    cout << "p3=p1: use_count=" << p1.use_count() << endl; // 3

    // 移动（引用计数不变，p1变为空）
    shared_ptr<int> p4 = move(p1);
    cout << "move后p1: " << (p1 ? "有效" : "空") << endl; // 空
    cout << "move后p4: use_count=" << p4.use_count() << endl; // 3

    // reset：释放所有权（引用计数-1）
    p2.reset();
    cout << "p2.reset后: use_count=" << p4.use_count() << endl; // 2

    // reset并指向新对象
    p3.reset(new int(20));
    cout << "p3.reset(new)后p4: use_count=" << p4.use_count() << endl; // 1
    cout << "*p3 = " << *p3 << endl; // 20

    // 引用计数为0时自动释放
    cout << "p4: use_count=" << p4.use_count() << endl; // 1
    p4.reset(); // 引用计数变为0，自动释放内存
    cout << "p4.reset后: " << (p4 ? "有效" : "空") << endl; // 空

    return 0;
}
```

## 五、常用操作

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

int main() {
    auto p = make_shared<string>("Hello");

    // ========== * 解引用 ==========
    cout << "值: " << *p << endl; // Hello

    // ========== -> 访问成员 ==========
    cout << "长度: " << p->length() << endl; // 5

    // ========== get() 获取裸指针 ==========
    // get返回内部管理的裸指针，注意不要delete这个指针！
    string* raw = p.get();
    cout << "裸指针值: " << *raw << endl; // Hello

    // ========== unique() 判断是否独占（C++17已弃用） ==========
    // C++17前：如果use_count()==1返回true
    // cout << "是否独占: " << p.unique() << endl;

    // ========== use_count() 引用计数 ==========
    cout << "引用计数: " << p.use_count() << endl; // 1

    // ========== reset() 重置 ==========
    auto p2 = p;
    cout << "复制后引用计数: " << p.use_count() << endl; // 2

    p.reset(); // 释放p的所有权
    cout << "p.reset后: " << (p ? "有效" : "空") << endl; // 空
    cout << "p2引用计数: " << p2.use_count() << endl; // 1

    // reset并指向新对象
    p.reset(new string("World"));
    cout << "*p = " << *p << endl; // World

    // ========== bool 转换 ==========
    if (p) {
        cout << "p有效" << endl;
    }

    shared_ptr<int> nullPtr;
    if (!nullPtr) {
        cout << "nullPtr为空" << endl;
    }

    // ========== swap 交换 ==========
    auto a = make_shared<int>(1);
    auto b = make_shared<int>(2);
    a.swap(b);
    cout << "*a=" << *a << ", *b=" << *b << endl; // *a=2, *b=1

    return 0;
}
```

## 六、循环引用问题（重点！）

循环引用是 `shared_ptr` 最大的陷阱，会导致内存泄漏。

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// 前向声明
struct B;

struct A {
    string name;
    shared_ptr<B> b_ptr; // A 指向 B

    A(const string& n) : name(n) {
        cout << "A构造: " << name << endl;
    }
    ~A() {
        cout << "A析构: " << name << endl;
    }
};

struct B {
    string name;
    shared_ptr<A> a_ptr; // B 指向 A（问题所在！）

    B(const string& n) : name(n) {
        cout << "B构造: " << name << endl;
    }
    ~B() {
        cout << "B析构: " << name << endl;
    }
};

int main() {
    {
        auto a = make_shared<A>("对象A");
        auto b = make_shared<B>("对象B");

        // 互相引用
        a->b_ptr = b;  // A -> B，b的引用计数变为2
        b->a_ptr = a;  // B -> A，a的引用计数变为2

        cout << "a引用计数: " << a.use_count() << endl; // 2
        cout << "b引用计数: " << b.use_count() << endl; // 2

    } // 离开作用域
    // a和b的栈上引用消失
    // 但 a->b_ptr 和 b->a_ptr 仍然互相引用
    // a的引用计数: 2-1=1（b->a_ptr还在）
    // b的引用计数: 2-1=1（a->b_ptr还在）
    // 引用计数永远不会变为0，内存泄漏！

    cout << "程序结束（但A和B的析构函数没有被调用！内存泄漏！）" << endl;

    return 0;
}
```

### 循环引用示意图

```
栈上变量:     a -----> A对象 <---- b
              |                  |
              +----> B对象 <-----+

A.b_ptr ---> B  (B的引用计数: 2)
B.a_ptr ---> A  (A的引用计数: 2)

离开作用域后：
  a销毁，A的引用计数: 2-1=1（B.a_ptr还在）
  b销毁，B的引用计数: 2-1=1（A.b_ptr还在）
  永远无法释放！
```

## 七、weak_ptr 解决循环引用

`weak_ptr` 是 `shared_ptr` 的弱引用，它**不增加引用计数**。

### weak_ptr 的特点

| 特点 | 说明 |
|------|------|
| 不拥有对象 | 不影响引用计数 |
| 不能直接访问 | 必须先 `lock()` 转为 `shared_ptr` |
| 用于打破循环 | 解决 `shared_ptr` 的循环引用问题 |
| `expired()` | 检查对象是否已被释放 |

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// 使用 weak_ptr 打破循环引用
struct B;

struct A {
    string name;
    shared_ptr<B> b_ptr; // A 强引用 B

    A(const string& n) : name(n) {
        cout << "A构造: " << name << endl;
    }
    ~A() {
        cout << "A析构: " << name << endl;
    }
};

struct B {
    string name;
    weak_ptr<A> a_ptr; // B 弱引用 A（关键！改成weak_ptr）

    B(const string& n) : name(n) {
        cout << "B构造: " << name << endl;
    }
    ~B() {
        cout << "B析构: " << name << endl;
    }
};

int main() {
    {
        auto a = make_shared<A>("对象A");
        auto b = make_shared<B>("对象B");

        a->b_ptr = b;  // A -> B，b的引用计数变为2
        b->a_ptr = a;  // B ->weak A，a的引用计数仍然是1！

        cout << "a引用计数: " << a.use_count() << endl; // 1
        cout << "b引用计数: " << b.use_count() << endl; // 2

    } // 离开作用域
    // a销毁，A的引用计数: 1-1=0 -> A被释放
    // A释放后，A.b_ptr销毁，B的引用计数: 2-1=1 -> B也被释放
    // 完美！没有内存泄漏

    cout << "程序正常结束" << endl;
    return 0;
}
```

### weak_ptr 的使用方法

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // ========== 创建 weak_ptr ==========
    shared_ptr<int> sp = make_shared<int>(42);
    weak_ptr<int> wp = sp; // 从shared_ptr创建

    cout << "sp引用计数: " << sp.use_count() << endl; // 1（wp不影响计数）

    // ========== expired() 检查对象是否已释放 ==========
    cout << "是否过期: " << wp.expired() << endl; // 0(false，还有效)

    // ========== lock() 获取 shared_ptr ==========
    // lock() 返回一个 shared_ptr
    // 如果对象已释放，返回空的 shared_ptr
    if (auto locked = wp.lock()) {
        cout << "lock成功, 值: " << *locked << endl; // 42
        cout << "引用计数: " << sp.use_count() << endl; // 2（locked增加计数）
    } else {
        cout << "对象已释放" << endl;
    }

    // ========== 对象释放后 ==========
    sp.reset(); // 释放对象
    cout << "sp.reset后是否过期: " << wp.expired() << endl; // 1(true)

    if (auto locked = wp.lock()) {
        cout << "值: " << *locked << endl;
    } else {
        cout << "对象已释放，无法访问" << endl;
    }

    return 0;
}
```

### weak_ptr 实际应用：观察者模式

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
using namespace std;

// 被观察者
class Subject {
public:
    string name;
    Subject(const string& n) : name(n) {}
    void notify() {
        cout << name << " 发出通知" << endl;
    }
};

// 观察者：使用 weak_ptr 避免阻止 Subject 的释放
class Observer {
public:
    string name;
    weak_ptr<Subject> subject; // 弱引用，不阻止Subject释放

    Observer(const string& n, shared_ptr<Subject> s)
        : name(n), subject(s) {}

    void check() {
        if (auto s = subject.lock()) {
            cout << name << " 观察到 " << s->name << endl;
        } else {
            cout << name << ": 观察目标已消失" << endl;
        }
    }
};

int main() {
    auto subject = make_shared<Subject>("服务器");

    Observer obs1("监控A", subject);
    Observer obs2("监控B", subject);

    obs1.check(); // 监控A 观察到 服务器
    obs2.check(); // 监控B 观察到 服务器

    // 释放Subject
    subject.reset();
    cout << "服务器已关闭" << endl;

    obs1.check(); // 监控A: 观察目标已消失
    obs2.check(); // 监控B: 观察目标已消失

    return 0;
}
```

## 八、完整对比与选择指南

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // ========== shared_ptr 适用场景 ==========
    // 多个地方需要共享同一个对象的所有权
    cout << "=== shared_ptr ===" << endl;
    {
        auto p1 = make_shared<int>(100);
        auto p2 = p1; // 共享所有权
        auto p3 = p1; // 共享所有权
        cout << "引用计数: " << p1.use_count() << endl; // 3
    }

    // ========== unique_ptr 适用场景 ==========
    // 只有一个地方拥有对象的所有权
    cout << "\n=== unique_ptr ===" << endl;
    {
        unique_ptr<int> p1 = make_unique<int>(200); // C++14
        // unique_ptr<int> p2 = p1; // 错误！不能复制
        unique_ptr<int> p2 = move(p1); // 只能移动
        cout << "*p2 = " << *p2 << endl; // 200
        // p1此时为空
    }

    // ========== weak_ptr 适用场景 ==========
    // 打破循环引用、缓存、观察者模式
    cout << "\n=== weak_ptr ===" << endl;
    {
        shared_ptr<int> sp = make_shared<int>(300);
        weak_ptr<int> wp = sp; // 弱引用

        if (auto p = wp.lock()) {
            cout << "值: " << *p << endl; // 300
        }
    }

    return 0;
}
```

### 如何选择智能指针

| 场景 | 选择 | 原因 |
|------|------|------|
| 单一所有者 | `unique_ptr` | 最轻量，零开销 |
| 多个所有者 | `shared_ptr` | 引用计数管理 |
| 打破循环引用 | `weak_ptr` | 不增加引用计数 |
| 观察/缓存 | `weak_ptr` | 不阻止对象释放 |
| 不确定 | `unique_ptr` | 需要时可转为 `shared_ptr` |

## 九、注意事项与最佳实践

```cpp
#include <iostream>
#include <memory>
using namespace std;

// ========== 最佳实践 ==========

class Widget {
public:
    Widget() { cout << "Widget构造" << endl; }
    ~Widget() { cout << "Widget析构" << endl; }
};

int main() {
    // 1. 优先使用 make_shared / make_unique
    auto p1 = make_shared<int>(42);    // 好
    shared_ptr<int> p2(new int(42));   // 可以但不推荐

    // 2. 不要混用裸指针和智能指针
    // int* raw = new int(10);
    // shared_ptr<int> sp1(raw);
    // shared_ptr<int> sp2(raw); // 严重错误！

    // 3. 不要用 get() 的返回值来创建另一个智能指针
    auto sp = make_shared<int>(100);
    // shared_ptr<int> sp2(sp.get()); // 严重错误！双重释放

    // 4. 使用 reset() 而不是 new 来重新赋值
    sp.reset(new int(200)); // 正确但不推荐
    sp = make_shared<int>(300); // 更好

    // 5. 注意 shared_ptr 的线程安全
    // 引用计数的增减是线程安全的
    // 但对对象的读写不是线程安全的，需要额外加锁

    // 6. 数组要用对应的删除器
    // shared_ptr<int> arr(new int[10]); // 错误！默认用delete而不是delete[]
    shared_ptr<int> arr(new int[10], default_delete<int[]>()); // 正确
    // 或者 C++17+: shared_ptr<int[]> arr(new int[10]);

    return 0;
}
```

## 十、总结

| 要点 | 内容 |
|------|------|
| shared_ptr | 共享所有权，引用计数自动管理内存 |
| 创建 | `make_shared<T>(args...)` 推荐 |
| 引用计数 | 复制+1，销毁-1，为0自动释放 |
| 常用操作 | `*` 解引用、`->` 访问成员、`get()` 裸指针、`reset()` 重置、`use_count()` 计数 |
| 循环引用 | 两个shared_ptr互相引用导致内存泄漏 |
| weak_ptr | 弱引用，不增加计数，`lock()` 获取 shared_ptr |
| 选择 | 单一所有者用 unique_ptr，共享用 shared_ptr，观察用 weak_ptr |
