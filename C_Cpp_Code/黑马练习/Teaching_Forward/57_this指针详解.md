# this 指针详解

## 一、什么是 this 指针

`this` 是一个**隐式指针**，它指向**调用当前成员函数的那个对象**。

每个非静态成员函数都有一个隐藏的参数 `this`，
当对象调用成员函数时，编译器会自动把对象的地址传给 `this`。

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;

    void show()
    {
        // this->name 等价于 name
        cout << "name = " << this->name << endl;
        cout << "age  = " << this->age << endl;
    }
};

int main()
{
    Person p;
    p.name = "张三";
    p.age = 20;
    p.show();  // 编译器自动把 &p 传给 this
    return 0;
}
```

---

## 二、this 指针的用途

### 2.1 解决名称冲突

当成员变量和函数参数同名时，用 `this->` 来区分。

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;

    // 参数名和成员变量名相同
    void setInfo(string name, int age)
    {
        this->name = name;  // this->name 是成员变量
        this->age = age;    // age 是参数
    }

    void show()
    {
        cout << "name = " << name << ", age = " << age << endl;
    }
};

int main()
{
    Person p;
    p.setInfo("张三", 20);
    p.show();  // name = 张三, age = 20
    return 0;
}
```

### 2.2 不用 this 会怎样

```cpp
void setInfo(string name, int age)
{
    // 不用 this：
    name = name;  // 参数自己赋给自己，成员变量没变！
    age = age;

    // 正确写法：
    this->name = name;
    this->age = age;
}
```

---

## 三、返回对象本身（链式调用）

### 3.1 什么是链式调用

```cpp
#include <iostream>
using namespace std;

class Calculator
{
private:
    int result;

public:
    Calculator() { result = 0; }

    Calculator& add(int n)
    {
        result += n;
        return *this;  // 返回自身引用
    }

    Calculator& subtract(int n)
    {
        result -= n;
        return *this;
    }

    Calculator& multiply(int n)
    {
        result *= n;
        return *this;
    }

    void show() { cout << "result = " << result << endl; }
};

int main()
{
    Calculator calc;

    // 链式调用
    calc.add(10).subtract(3).multiply(2);
    calc.show();  // result = 14

    return 0;
}
```

### 3.2 返回引用 vs 返回值

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    string name;
    int age;

    // 正确：返回引用
    Person& setName(string name)
    {
        this->name = name;
        return *this;
    }

    // 错误：返回值（拷贝）
    Person setAge(int age)
    {
        this->age = age;
        return *this;
    }
};

int main()
{
    Person p;

    p.setName("张三").setName("李四");
    cout << p.name << endl;  // 李四（正确）

    p.setAge(20).setAge(30);
    cout << p.age << endl;   // 20（不是30！因为返回的是拷贝）

    return 0;
}
```

### 3.3 实际应用

```cpp
#include <iostream>
#include <string>
using namespace std;

class StringBuilder
{
private:
    string content;

public:
    StringBuilder& append(const string &str)
    {
        content += str;
        return *this;
    }

    StringBuilder& appendLine(const string &str)
    {
        content += str + "\n";
        return *this;
    }

    string build() { return content; }
};

int main()
{
    string result = StringBuilder()
        .append("Hello")
        .append(", ")
        .append("World")
        .append("!")
        .build();

    cout << result << endl;  // Hello, World!
    return 0;
}
```

---

## 四、this 指针的本质

### 4.1 隐式参数

```cpp
class Person
{
public:
    string name;
    int age;

    // 你写的代码：
    void show()
    {
        cout << name << ", " << age << endl;
    }

    // 编译器实际处理成：
    // void show(Person * const this)
    // {
    //     cout << this->name << ", " << this->age << endl;
    // }
};
```

### 4.2 this 的类型

```cpp
// this 的类型是 Person* const（指针常量）
// 指针本身不能改（不能指向其他对象）
// 但指向的内容可以改

void func()
{
    // this = nullptr;       // 错误！不能改指向
    this->name = "张三";    // 正确！可以改内容
}
```

### 4.3 隐式传递

```cpp
// 以下两种写法等价：
void func1() { age = 20; }
void func2() { this->age = 20; }
```

---

## 五、空指针调用注意事项

```cpp
#include <iostream>
using namespace std;

class Person
{
public:
    int age;

    void showAge()
    {
        // this 为 nullptr 时会崩溃
        cout << "age = " << age << endl;
    }

    void showText()
    {
        // 不访问成员，不会崩溃
        cout << "成员函数" << endl;
    }
};

int main()
{
    Person *p = nullptr;

    // p->showText();  // 可以运行
    // p->showAge();   // 崩溃！

    return 0;
}
```

---

## 六、this 在运算符重载中的应用

```cpp
#include <iostream>
using namespace std;

class Counter
{
private:
    int value;

public:
    Counter(int v = 0) : value(v) {}

    // 前置 ++：返回引用
    Counter& operator++()
    {
        value++;
        return *this;
    }

    // 后置 ++：返回值
    Counter operator++(int)
    {
        Counter temp = *this;
        value++;
        return temp;
    }

    void show() { cout << "value = " << value << endl; }
};

int main()
{
    Counter c(10);
    (++c).show();  // 11
    (c++).show();  // 11
    c.show();      // 12
    return 0;
}
```

---

## 七、完整示例

```cpp
#include <iostream>
using namespace std;

class Student
{
private:
    string name;
    int age;
    float score;

public:
    Student(string name, int age, float score)
    {
        this->name = name;
        this->age = age;
        this->score = score;
    }

    Student& setName(string name)
    {
        this->name = name;
        return *this;
    }

    Student& setAge(int age)
    {
        this->age = age;
        return *this;
    }

    Student& setScore(float score)
    {
        this->score = score;
        return *this;
    }

    void show()
    {
        cout << name << ", " << age << "岁, " << score << "分" << endl;
    }

    bool isBetterThan(const Student &other)
    {
        return this->score > other.score;
    }

    string getName() const { return name; }
};

int main()
{
    Student s1("张三", 20, 95.5);
    Student s2("李四", 21, 88.0);

    s1.setName("王五").setAge(19).setScore(99.0).show();

    if (s1.isBetterThan(s2))
        cout << s1.getName() << " 成绩更好" << endl;
    else
        cout << s2.getName() << " 成绩更好" << endl;

    return 0;
}
```

---

## 总结

| 要点 | 说明 |
|------|------|
| this 是什么 | 指向调用当前成员函数的对象 |
| 本质 | 隐式参数 `Person* const this` |
| 解决名称冲突 | `this->name = name;` |
| 链式调用 | `return *this;` 返回对象引用 |
| 返回引用 vs 返回值 | 链式调用必须返回引用 |
| 空指针调用 | this 为空时访问成员会崩溃 |
| 静态函数 | 没有 this 指针 |
