# const 成员与友元

## 一、常函数（const 成员函数）

### 1.1 什么是常函数

在成员函数的**参数列表后面**加上 `const`，就变成了**常函数**。

```cpp
class Person
{
public:
    string name;
    int age;

    // 常函数：参数列表后面加 const
    void show() const
    {
        cout << "name = " << name << ", age = " << age << endl;
    }
};
```

### 1.2 常函数不能修改成员变量

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;

    void setName(string name)
    {
        this->name = name;  // 正确（非常函数）
    }

    void show() const
    {
        cout << name << ", " << age << endl;
        // name = "李四";  // 错误！常函数不能修改成员
        // age = 30;       // 错误！
    }
};

int main()
{
    Person p;
    p.name = "张三";
    p.age = 20;
    p.show();
    return 0;
}
```

### 1.3 常函数的本质

常函数中的 `const` 修饰的是 `this` 指针：

```cpp
// 普通成员函数：
// void show(Person * const this)
// this 指向不能改，但内容可以改

// 常函数：
// void show(const Person * const this)
// this 指向不能改，内容也不能改
```

| 函数类型 | this 的类型 | 能否修改成员 |
|---------|------------|-------------|
| 普通成员函数 | `Person* const` | 能 |
| 常函数 | `const Person* const` | 不能 |

### 1.4 mutable 关键字

用 `mutable` 修饰的成员变量，即使在常函数中也可以修改。

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;
    mutable int accessCount;  // 常函数中也能修改

    Person(string n, int a) : name(n), age(a), accessCount(0) {}

    void show() const
    {
        accessCount++;  // 正确！mutable 可以修改
        cout << name << ", " << age
             << " (访问次数: " << accessCount << ")" << endl;
    }
};

int main()
{
    Person p("张三", 20);

    p.show();  // 张三, 20 (访问次数: 1)
    p.show();  // 张三, 20 (访问次数: 2)
    p.show();  // 张三, 20 (访问次数: 3)

    return 0;
}
```

**mutable 使用场景：** 访问计数、缓存标志、互斥锁等不影响对象逻辑状态的成员。

---

## 二、常对象（const 对象）

### 2.1 什么是常对象

用 `const` 修饰的对象，成员变量不能被修改（mutable 除外）。

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}

    void show() const
    {
        cout << name << ", " << age << endl;
    }

    void setName(string name)
    {
        this->name = name;
    }
};

int main()
{
    const Person p("张三", 20);

    // p.name = "李四";    // 错误！常对象不能修改
    // p.setName("李四");  // 错误！不能调用非常函数

    p.show();  // 正确！常函数可以调用

    return 0;
}
```

### 2.2 常对象只能调用常函数

```cpp
class Person
{
public:
    void func1() {}           // 非常函数
    void func2() const {}     // 常函数
};

int main()
{
    const Person p;

    // p.func1();  // 错误！常对象不能调用非常函数
    p.func2();    // 正确

    return 0;
}
```

### 2.3 常对象中 mutable 的例外

```cpp
class Person
{
public:
    string name;
    mutable int viewCount;

    Person(string n) : name(n), viewCount(0) {}

    void view() const
    {
        viewCount++;  // mutable 可以修改
        cout << "查看: " << name << " (第 " << viewCount << " 次)" << endl;
    }
};

int main()
{
    const Person p("张三");
    p.view();  // 第 1 次
    p.view();  // 第 2 次
    return 0;
}
```

---

## 三、const 总结对比

| 类型 | 能否修改普通成员 | 能否修改 mutable | 能否调用常函数 | 能否调用非常函数 |
|------|----------------|-----------------|--------------|----------------|
| 非常对象 | 能 | 能 | 能 | 能 |
| 常对象 | 不能 | 能 | 能 | 不能 |
| 常函数 | 不能 | 能 | -- | -- |

---

## 四、友元函数

### 4.1 什么是友元

在类中声明某个函数为友元，该函数就可以访问类的私有成员。

```cpp
#include <iostream>
using namespace std;

class Building
{
    friend void goodGay(Building &b);  // 声明友元

private:
    string bedroom;

public:
    string livingRoom;

    Building()
    {
        bedroom = "卧室";
        livingRoom = "客厅";
    }
};

void goodGay(Building &b)
{
    cout << "客厅: " << b.livingRoom << endl;  // 公有，正常访问
    cout << "卧室: " << b.bedroom << endl;     // 私有，友元可以访问
}

int main()
{
    Building b;
    goodGay(b);
    // cout << b.bedroom;  // 错误！非友元不能访问私有
    return 0;
}
```

### 4.2 友元函数重载 << 运算符

友元函数最常见的用途：**重载 `<<` 运算符**。

```cpp
#include <iostream>
using namespace std;

class Person
{
    friend ostream& operator<<(ostream &out, const Person &p);

private:
    string name;
    int age;

public:
    Person(string name, int age) : name(name), age(age) {}
};

// 全局函数，重载 <<
ostream& operator<<(ostream &out, const Person &p)
{
    out << "姓名: " << p.name << ", 年龄: " << p.age;
    return out;
}

int main()
{
    Person p("张三", 20);
    cout << p << endl;  // 姓名: 张三, 年龄: 20

    Person p2("李四", 21);
    cout << p << " | " << p2 << endl;

    return 0;
}
```

### 4.3 为什么 << 必须用友元

```cpp
// 如果作为成员函数：
// p.operator<<(cout)  -->  p << cout  语法不对！
// 因为成员函数的 this 是左操作数

// 我们想要：cout << p
// 所以 << 重载必须是全局函数（友元）
```

---

## 五、友元类

### 5.1 什么是友元类

```cpp
#include <iostream>
using namespace std;

class Building
{
    friend class GoodGay;  // 声明友元类

private:
    string bedroom;

public:
    string livingRoom;

    Building()
    {
        bedroom = "卧室";
        livingRoom = "客厅";
    }
};

class GoodGay
{
public:
    Building building;

    void visit()
    {
        cout << "客厅: " << building.livingRoom << endl;
        cout << "卧室: " << building.bedroom << endl;  // 友元可以访问私有
    }
};

int main()
{
    GoodGay gg;
    gg.visit();
    return 0;
}
```

### 5.2 友元的特性

| 特性 | 说明 |
|------|------|
| 单向 | B 是 A 的友元，不代表 A 是 B 的友元 |
| 不传递 | B 是 A 的友元，C 是 B 的友元，不代表 C 是 A 的友元 |
| 不继承 | 父类的友元不是子类的友元 |

---

## 六、友元函数 vs 成员函数

| 对比项 | 友元函数 | 成员函数 |
|--------|---------|---------|
| 是否属于类 | 不是 | 是 |
| this 指针 | 没有 | 有 |
| 访问私有成员 | 可以 | 可以 |
| 调用方式 | `func(obj)` | `obj.func()` |
| 常见用途 | 重载 `<<` `>>` | 常规操作 |

---

## 七、综合案例

```cpp
#include <iostream>
#include <string>
using namespace std;

class Student
{
    friend ostream& operator<<(ostream &out, const Student &s);
    friend istream& operator>>(istream &in, Student &s);

private:
    string name;
    int age;
    float score;

public:
    Student(string n = "", int a = 0, float sc = 0)
        : name(n), age(a), score(sc) {}

    void show() const
    {
        cout << "姓名: " << name
             << ", 年龄: " << age
             << ", 成绩: " << score << endl;
    }

    mutable int showCount = 0;
    void countShow() const
    {
        showCount++;
        cout << "(第 " << showCount << " 次) ";
        show();
    }
};

ostream& operator<<(ostream &out, const Student &s)
{
    out << "[" << s.name << ", " << s.age << "岁, " << s.score << "分]";
    return out;
}

istream& operator>>(istream &in, Student &s)
{
    cout << "请输入姓名: "; in >> s.name;
    cout << "请输入年龄: "; in >> s.age;
    cout << "请输入成绩: "; in >> s.score;
    return in;
}

int main()
{
    const Student s1("张三", 20, 95.5);

    s1.show();       // 常函数
    s1.countShow();  // 常函数（mutable 可修改）

    cout << s1 << endl;  // 友元重载 <<

    return 0;
}
```

---

## 八、mutable 使用场景

```cpp
class DataCache
{
private:
    int data;
    mutable bool isDirty;
    mutable int accessCount;

public:
    DataCache(int d) : data(d), isDirty(false), accessCount(0) {}

    int getData() const
    {
        accessCount++;   // 常函数中修改 mutable
        return data;
    }

    void setData(int d)
    {
        data = d;
        isDirty = true;
    }

    bool needsSave() const { return isDirty; }
    int getAccessCount() const { return accessCount; }
};
```

---

## 总结

| 要点 | 说明 |
|------|------|
| 常函数 | `void func() const`，不能修改普通成员 |
| const 修饰 this | `const Person* const this` |
| mutable | 常函数和常对象中仍可修改 |
| 常对象 | `const Person p`，只能调用常函数 |
| 友元函数 | 可访问私有成员，不是成员函数 |
| 重载 `<<` | 必须用友元（全局函数） |
| 友元类 | 单向、不传递、不继承 |
