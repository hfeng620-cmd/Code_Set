# unique_ptr 智能指针

## 1. 概述

在C++中，使用 `new` 创建的对象需要手动用 `delete` 释放。如果忘记 `delete`，就会导致**内存泄漏**。

**智能指针**是一种自动管理内存的工具，它会在离开作用域时自动释放所指向的内存，不需要手动 `delete`。

---

## 2. 为什么需要智能指针

### 2.1 裸指针的问题

```cpp
#include <iostream>
using namespace std;

// 问题1：忘记 delete
void problem1() {
    int *ptr = new int(100);
    cout << *ptr << endl;
    // 忘记 delete ptr;  // 内存泄漏！
}

// 问题2：提前 return 导致 delete 不执行
void problem2() {
    int *ptr = new int(100);

    if (*ptr > 0) {
        return;  // 提前返回，delete 不会执行！
    }

    delete ptr;
}

// 问题3：异常导致 delete 不执行
void problem3() {
    int *ptr = new int(100);

    throw "发生错误";  // 异常，delete 不会执行！

    delete ptr;
}

int main() {
    problem1();
    // problem2();
    // problem3();
    return 0;
}
```

### 2.2 智能指针解决问题

```cpp
#include <iostream>
#include <memory>  // 智能指针的头文件
using namespace std;

void solution() {
    // 创建 unique_ptr，自动管理内存
    unique_ptr<int> ptr(new int(100));
    cout << *ptr << endl;

    // 不需要手动 delete
    // 离开作用域时会自动释放内存
}

int main() {
    solution();
    cout << "内存已自动释放" << endl;
    return 0;
}
```

---

## 3. RAII 原理

**RAII**（Resource Acquisition Is Initialization）即"资源获取即初始化"。

核心思想：
- 在构造函数中获取资源（如分配内存、打开文件）
- 在析构函数中释放资源（如释放内存、关闭文件）
- 利用对象的生命周期自动管理资源

```cpp
#include <iostream>
#include <memory>
using namespace std;

class MyResource {
public:
    MyResource() {
        cout << "获取资源" << endl;
    }
    ~MyResource() {
        cout << "释放资源" << endl;
    }
    void doSomething() {
        cout << "使用资源" << endl;
    }
};

int main() {
    {
        // 创建对象时获取资源
        MyResource res;
        res.doSomething();
    }  // 离开作用域时自动释放资源

    cout << "资源已释放" << endl;
    return 0;
}
```

---

## 4. unique_ptr 的特点

`unique_ptr` 是一种**独占所有权**的智能指针：

1. **独占所有权**：一个对象只能被一个 `unique_ptr` 拥有
2. **不能复制**：不能将一个 `unique_ptr` 复制给另一个
3. **可以移动**：可以通过 `move` 转移所有权
4. **自动释放**：离开作用域时自动释放所指向的内存

---

## 5. 创建 unique_ptr

### 5.1 使用 new 创建（C++11）

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 方式1：使用 new 创建
    unique_ptr<int> p1(new int(100));
    cout << "*p1 = " << *p1 << endl;

    // 方式2：创建空的 unique_ptr
    unique_ptr<int> p2;
    cout << "p2 是否为空：" << (p2 == nullptr ? "是" : "否") << endl;

    return 0;
}
```

### 5.2 使用 make_unique 创建（C++14，推荐）

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 使用 make_unique（C++14推荐）
    auto p1 = make_unique<int>(100);
    cout << "*p1 = " << *p1 << endl;

    // 创建数组
    auto p2 = make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) {
        p2[i] = i * 10;
    }

    for (int i = 0; i < 5; i++) {
        cout << "p2[" << i << "] = " << p2[i] << endl;
    }

    return 0;
}
```

```cpp
// 为什么推荐 make_unique？
// 1. 更简洁：不需要写类型名
// 2. 更安全：避免内存泄漏（在某些情况下）
// 3. 更高效：可能有一次优化的机会

// 不推荐（可能在某些情况下有内存泄漏风险）：
// unique_ptr<int> p(new int(100));

// 推荐：
// auto p = make_unique<int>(100);
```

---

## 6. 不能复制

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    unique_ptr<int> p1(new int(100));

    // 错误：unique_ptr 不能复制！
    // unique_ptr<int> p2 = p1;          // 编译错误！
    // unique_ptr<int> p3(p1);           // 编译错误！
    // unique_ptr<int> p4 = unique_ptr<int>(p1);  // 编译错误！

    cout << "*p1 = " << *p1 << endl;

    return 0;
}
```

---

## 7. 可以移动

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    unique_ptr<int> p1(new int(100));
    cout << "移动前：*p1 = " << *p1 << endl;

    // 使用 std::move 转移所有权
    unique_ptr<int> p2 = move(p1);

    // 移动后，p1 变为空，p2 拥有对象
    cout << "移动后：" << endl;
    cout << "p1 是否为空：" << (p1 == nullptr ? "是" : "否") << endl;
    cout << "*p2 = " << *p2 << endl;

    return 0;
}
```

```cpp
#include <iostream>
#include <memory>
using namespace std;

// 函数参数传递需要移动
void takeOwnership(unique_ptr<int> p) {
    cout << "接收到：*p = " << *p << endl;
    // p 在函数结束时会自动释放
}

unique_ptr<int> createObject() {
    // 返回 unique_ptr 不需要 move，编译器会自动处理
    return make_unique<int>(200);
}

int main() {
    auto p1 = make_unique<int>(100);

    // 传递给函数需要 move
    takeOwnership(move(p1));

    // p1 已经为空
    cout << "p1 是否为空：" << (p1 == nullptr ? "是" : "否") << endl;

    // 接收函数返回的 unique_ptr
    auto p2 = createObject();
    cout << "*p2 = " << *p2 << endl;

    return 0;
}
```

---

## 8. 常用操作

### 8.1 解引用和访问成员

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Person {
public:
    string name;
    int age;

    Person(const string &n, int a) : name(n), age(a) {}

    void introduce() {
        cout << "我是" << name << "，今年" << age << "岁" << endl;
    }
};

int main() {
    auto p = make_unique<Person>("张三", 20);

    // *p 解引用，获取对象
    cout << "姓名：" << p->name << endl;
    cout << "年龄：" << p->age << endl;

    // -> 访问成员
    p->introduce();

    // 也可以用 (*p).name 的方式
    cout << "姓名：" << (*p).name << endl;

    return 0;
}
```

### 8.2 get() 获取裸指针

```cpp
#include <iostream>
#include <memory>
using namespace std;

void legacyFunction(int *ptr) {
    // 旧代码可能需要裸指针
    cout << "值：" << *ptr << endl;
}

int main() {
    auto p = make_unique<int>(100);

    // get() 返回裸指针，但不转移所有权
    int *rawPtr = p.get();
    cout << "通过裸指针：" << *rawPtr << endl;

    // 传递给需要裸指针的旧代码
    legacyFunction(p.get());

    // 注意：不要 delete get() 返回的指针！
    // delete rawPtr;  // 错误！会导致双重释放

    // unique_ptr 仍然拥有对象
    cout << "*p = " << *p << endl;

    return 0;
}
```

### 8.3 reset() 释放并置空

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    auto p = make_unique<int>(100);
    cout << "*p = " << *p << endl;

    // reset() 释放当前对象，并将指针置为空
    p.reset();
    cout << "reset 后 p 是否为空：" << (p == nullptr ? "是" : "否") << endl;

    // reset(new_obj) 释放旧对象，接管新对象
    p.reset(new int(200));
    cout << "*p = " << *p << endl;

    return 0;
}
```

### 8.4 release() 放弃所有权

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    auto p = make_unique<int>(100);
    cout << "*p = " << *p << endl;

    // release() 放弃所有权，返回裸指针
    // 调用者需要负责释放这个指针！
    int *rawPtr = p.release();

    // p 已经为空
    cout << "release 后 p 是否为空：" << (p == nullptr ? "是" : "否") << endl;

    // 裸指针仍然有效
    cout << "裸指针的值：" << *rawPtr << endl;

    // 必须手动释放！
    delete rawPtr;

    return 0;
}
```

```cpp
// release() 的使用场景：
// 1. 需要将所有权转移给旧代码
// 2. 需要将所有权转移给其他智能指针

// 注意：release() 不会释放内存，只是放弃所有权
// 如果不处理返回的指针，就会内存泄漏
```

---

## 9. unique_ptr 管理数组

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 创建管理数组的 unique_ptr
    unique_ptr<int[]> p(new int[5]);

    // 使用数组下标访问
    for (int i = 0; i < 5; i++) {
        p[i] = i * 10;
    }

    for (int i = 0; i < 5; i++) {
        cout << "p[" << i << "] = " << p[i] << endl;
    }

    // 使用 make_unique 创建数组（C++14）
    auto p2 = make_unique<double[]>(3);
    p2[0] = 1.1;
    p2[1] = 2.2;
    p2[2] = 3.3;

    for (int i = 0; i < 3; i++) {
        cout << "p2[" << i << "] = " << p2[i] << endl;
    }

    // 离开作用域时自动调用 delete[]
    return 0;
}
```

---

## 10. 自定义删除器

```cpp
#include <iostream>
#include <memory>
#include <cstdio>  // 用于 FILE
using namespace std;

// 自定义删除器：关闭文件
struct FileDeleter {
    void operator()(FILE *fp) const {
        if (fp) {
            cout << "关闭文件" << endl;
            fclose(fp);
        }
    }
};

int main() {
    // 使用自定义删除器管理文件
    unique_ptr<FILE, FileDeleter> file(fopen("test.txt", "w"));

    if (file) {
        fprintf(file.get(), "Hello, World!\n");
        cout << "写入完成" << endl;
    }

    // 离开作用域时会自动调用 FileDeleter 关闭文件
    return 0;
}
```

```cpp
// 使用 lambda 作为删除器
#include <iostream>
#include <memory>
using namespace std;

int main() {
    // 使用 lambda 作为删除器
    auto deleter = [](int *p) {
        cout << "自定义删除：释放内存" << endl;
        delete p;
    };

    unique_ptr<int, decltype(deleter)> p(new int(100), deleter);
    cout << "*p = " << *p << endl;

    // 离开作用域时会调用 lambda 删除器
    return 0;
}
```

---

## 11. 完整的实际应用示例

### 11.1 动态创建对象

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Animal {
public:
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
    string name;
public:
    Dog(const string &n) : name(n) {}
    void speak() override {
        cout << name << "：汪汪汪！" << endl;
    }
};

class Cat : public Animal {
    string name;
public:
    Cat(const string &n) : name(n) {}
    void speak() override {
        cout << name << "：喵喵喵！" << endl;
    }
};

int main() {
    // 使用 unique_ptr 管理多态对象
    unique_ptr<Animal> dog = make_unique<Dog>("旺财");
    unique_ptr<Animal> cat = make_unique<Cat>("咪咪");

    dog->speak();
    cat->speak();

    // 不需要手动 delete，自动释放
    return 0;
}
```

### 11.2 资源管理

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Connection {
    string name;
public:
    Connection(const string &n) : name(n) {
        cout << "建立连接：" << name << endl;
    }
    ~Connection() {
        cout << "断开连接：" << name << endl;
    }
    void query(const string &sql) {
        cout << name << " 执行查询：" << sql << endl;
    }
};

void processRequest() {
    // 自动管理连接的生命周期
    auto conn = make_unique<Connection>("数据库连接");

    conn->query("SELECT * FROM users");
    conn->query("UPDATE users SET age = 21 WHERE name = '张三'");

    // 函数结束时自动断开连接
}

int main() {
    cout << "开始处理请求" << endl;
    processRequest();
    cout << "请求处理完成" << endl;

    return 0;
}
```

### 11.3 返回动态创建的对象

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

class Config {
    string data;
public:
    Config(const string &d) : data(d) {
        cout << "加载配置" << endl;
    }
    ~Config() {
        cout << "释放配置" << endl;
    }
    string get(const string &key) const {
        return "value_of_" + key;
    }
};

// 函数返回 unique_ptr，所有权转移给调用者
unique_ptr<Config> loadConfig(const string &filename) {
    // 创建配置对象
    auto config = make_unique<Config>(filename);

    // 加载配置数据...

    // 返回时所有权转移
    return config;
}

int main() {
    auto config = loadConfig("app.conf");
    cout << "配置值：" << config->get("database") << endl;

    // config 在 main 结束时自动释放
    return 0;
}
```

---

## 12. 常见错误

### 12.1 不要混用裸指针和智能指针

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    int *raw = new int(100);

    // 错误：用同一个裸指针创建多个智能指针
    unique_ptr<int> p1(raw);
    // unique_ptr<int> p2(raw);  // 错误！会导致双重释放

    // 正确做法：只用智能指针管理
    auto p3 = make_unique<int>(200);

    return 0;
}
```

### 12.2 不要 delete get() 返回的指针

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    auto p = make_unique<int>(100);
    int *raw = p.get();

    // 错误：不要 delete get() 返回的指针
    // delete raw;  // 会导致双重释放！

    // 正确：让智能指针自动管理
    return 0;
}
```

### 12.3 注意移动后的状态

```cpp
#include <iostream>
#include <memory>
using namespace std;

int main() {
    auto p1 = make_unique<int>(100);
    auto p2 = move(p1);

    // p1 已经被移动，不要再使用！
    // cout << *p1 << endl;  // 未定义行为！

    // 正确：先检查是否为空
    if (p1) {
        cout << *p1 << endl;
    } else {
        cout << "p1 已经为空" << endl;
    }

    return 0;
}
```

---

## 13. 总结

| 要点 | 说明 |
|------|------|
| 头文件 | `#include <memory>` |
| 创建方式1 | `unique_ptr<int> p(new int(10));` |
| 创建方式2 | `auto p = make_unique<int>(10);`（推荐） |
| 特点 | 独占所有权，不能复制，可以移动 |
| 移动 | `unique_ptr<int> p2 = move(p1);` |
| 解引用 | `*p` 或 `p->member` |
| 获取裸指针 | `p.get()` |
| 释放并置空 | `p.reset()` |
| 接管新对象 | `p.reset(new int(20))` |
| 放弃所有权 | `p.release()` |
| 管理数组 | `unique_ptr<int[]> p(new int[5]);` |

何时使用 unique_ptr：
1. 需要动态分配对象，但只有一个拥有者
2. 需要返回动态创建的对象
3. 需要管理资源（如文件、连接）
4. 作为类成员管理动态资源

记住：**优先使用 unique_ptr，除非需要共享所有权时才用 shared_ptr**
