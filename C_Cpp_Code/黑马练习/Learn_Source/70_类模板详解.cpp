/*
 ============================================================================
 文件名 : 70_类模板详解.cpp
 描  述 : 详细讲解C++类模板的语法和高级用法
          包括：template<class T> class 语法、类外定义成员函数、
          多模板参数、默认模板参数、全特化、偏特化、模板类继承
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 70_类模板详解.cpp -o 70_类模板详解
 ============================================================================
*/

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// ==================== 1. 基本类模板 ====================
// 一个简单的动态数组类模板
template<class T>
class MyArray {
private:
    T* m_data;       // 指向动态数组的指针
    int m_size;      // 当前元素个数
    int m_capacity;  // 容量

public:
    // 构造函数声明（类内声明，类外定义）
    MyArray(int capacity = 10);
    ~MyArray();

    void push_back(const T& value);
    T& operator[](int index);
    const T& operator[](int index) const;
    int size() const;
    void display() const;
};

// ==================== 2. 类外定义成员函数 ====================
// 语法：template<class T>  返回值  类名<T>::函数名(参数)  { ... }
// 注意：类名后面必须加 <T>

template<class T>
MyArray<T>::MyArray(int capacity) : m_size(0), m_capacity(capacity) {
    m_data = new T[m_capacity];
    cout << "[MyArray] 构造, 容量=" << m_capacity << endl;
}

template<class T>
MyArray<T>::~MyArray() {
    delete[] m_data;
    cout << "[MyArray] 析构" << endl;
}

template<class T>
void MyArray<T>::push_back(const T& value) {
    if (m_size >= m_capacity) {
        // 扩容：容量翻倍
        m_capacity *= 2;
        T* newData = new T[m_capacity];
        for (int i = 0; i < m_size; i++) {
            newData[i] = m_data[i];
        }
        delete[] m_data;
        m_data = newData;
        cout << "  [扩容] 新容量=" << m_capacity << endl;
    }
    m_data[m_size++] = value;
}

template<class T>
T& MyArray<T>::operator[](int index) {
    if (index < 0 || index >= m_size) {
        throw out_of_range("索引越界!");
    }
    return m_data[index];
}

template<class T>
const T& MyArray<T>::operator[](int index) const {
    if (index < 0 || index >= m_size) {
        throw out_of_range("索引越界!");
    }
    return m_data[index];
}

template<class T>
int MyArray<T>::size() const { return m_size; }

template<class T>
void MyArray<T>::display() const {
    cout << "  [";
    for (int i = 0; i < m_size; i++) {
        cout << m_data[i];
        if (i < m_size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// ==================== 3. 多模板参数 ====================
// 一个键值对类模板，有两个类型参数
template<class Key, class Value>
class Pair {
private:
    Key m_key;
    Value m_value;

public:
    Pair(const Key& k, const Value& v) : m_key(k), m_value(v) {}
    Key getKey() const { return m_key; }
    Value getValue() const { return m_value; }
    void display() const {
        cout << "{" << m_key << ": " << m_value << "}";
    }
};

// ==================== 4. 默认模板参数 ====================
template<class T, int Size = 10, class Container = MyArray<T>>
class Stack {
private:
    Container m_container;
public:
    void push(const T& value) { m_container.push_back(value); }
    void display() const {
        cout << "Stack: ";
        m_container.display();
    }
};

// ==================== 5. 类模板全特化 ====================
// 通用版本
template<class T>
class Comparator {
public:
    static bool isEqual(const T& a, const T& b) {
        cout << "[通用版本] ";
        return a == b;
    }
    static bool isLess(const T& a, const T& b) {
        return a < b;
    }
};

// 全特化：针对 const char* 的特殊版本
template<>
class Comparator<const char*> {
public:
    static bool isEqual(const char* const& a, const char* const& b) {
        cout << "[char*全特化] ";
        return strcmp(a, b) == 0;
    }
    static bool isLess(const char* const& a, const char* const& b) {
        return strcmp(a, b) < 0;
    }
};

// 全特化：针对 double 的近似比较
template<>
class Comparator<double> {
public:
    static bool isEqual(const double& a, const double& b) {
        cout << "[double全特化] ";
        const double eps = 1e-9;
        return (a - b < eps) && (b - a < eps);
    }
    static bool isLess(const double& a, const double& b) {
        return a < b - 1e-9;
    }
};

// ==================== 6. 类模板偏特化 ====================
// 通用版本
template<class T1, class T2>
class DataContainer {
public:
    T1 first;
    T2 second;
    DataContainer(const T1& f, const T2& s) : first(f), second(s) {}
    void show() const {
        cout << "[通用版本] first=" << first << ", second=" << second << endl;
    }
};

// 偏特化：当两个参数类型相同时
template<class T>
class DataContainer<T, T> {
public:
    T first;
    T second;
    DataContainer(const T& f, const T& s) : first(f), second(s) {}
    void show() const {
        cout << "[偏特化-同类型] first=" << first << ", second=" << second << endl;
    }
    T sum() const { return first + second; }
};

// 偏特化：当第二个参数是指针类型时
template<class T1, class T2>
class DataContainer<T1, T2*> {
public:
    T1 first;
    T2* second;
    DataContainer(const T1& f, T2* s) : first(f), second(s) {}
    void show() const {
        cout << "[偏特化-指针] first=" << first << ", *second=" << *second << endl;
    }
};

// ==================== 7. 模板类继承 ====================
// 方式1：派生类也是模板，用派生类的模板参数指定基类参数

// 基类模板
template<class T>
class Animal {
protected:
    string m_name;
    T m_age;
public:
    Animal(const string& name, T age) : m_name(name), m_age(age) {
        cout << "[Animal] 构造: " << m_name << endl;
    }
    virtual ~Animal() = default;
    virtual void speak() const = 0;
    void info() const {
        cout << "  名字: " << m_name << ", 年龄: " << m_age << endl;
    }
};

// 派生类模板继承基类模板
template<class T>
class Dog : public Animal<T> {
private:
    string m_breed;
public:
    Dog(const string& name, T age, const string& breed)
        : Animal<T>(name, age), m_breed(breed) {
        cout << "[Dog] 构造, 品种: " << m_breed << endl;
    }
    void speak() const override {
        cout << this->m_name << ": 汪汪汪!" << endl;
    }
    void showBreed() const {
        cout << this->m_name << " 的品种: " << m_breed << endl;
    }
};

// 方式2：派生类不是模板，直接指定基类的具体类型
class Cat : public Animal<int> {
private:
    string m_color;
public:
    Cat(const string& name, int age, const string& color)
        : Animal<int>(name, age), m_color(color) {
        cout << "[Cat] 构造, 颜色: " << m_color << endl;
    }
    void speak() const override {
        cout << m_name << ": 喵喵喵!" << endl;
    }
};

// ==================== 8. 演示函数 ====================
void demonstrateBasicTemplate() {
    cout << "\n===== 1. 基本类模板与类外定义 =====" << endl;
    MyArray<int> intArr(5);
    intArr.push_back(10);
    intArr.push_back(20);
    intArr.push_back(30);
    intArr.display();
    cout << "  大小: " << intArr.size() << endl;
    cout << "  intArr[1] = " << intArr[1] << endl;

    MyArray<string> strArr(3);
    strArr.push_back("Hello");
    strArr.push_back("World");
    strArr.display();
}

void demonstrateMultiParams() {
    cout << "\n===== 2. 多模板参数 =====" << endl;
    Pair<string, int> p1("年龄", 25);
    p1.display(); cout << endl;

    Pair<int, double> p2(1, 3.14);
    p2.display(); cout << endl;
}

void demonstrateDefaultParams() {
    cout << "\n===== 3. 默认模板参数 =====" << endl;
    Stack<int> s1;  // 使用默认参数
    s1.push(1); s1.push(2); s1.push(3);
    s1.display();

    Stack<string, 20> s2;  // 自定义 Size
    s2.push("Hello"); s2.push("World");
    s2.display();
}

void demonstrateSpecialization() {
    cout << "\n===== 4. 全特化 =====" << endl;
    cout << "Comparator<int>: ";
    cout << Comparator<int>::isEqual(10, 10) << endl;

    cout << "Comparator<const char*>: ";
    cout << Comparator<const char*>::isEqual("hello", "hello") << endl;

    cout << "Comparator<double>: ";
    cout << Comparator<double>::isEqual(0.1 + 0.2, 0.3) << endl;
}

void demonstratePartialSpecialization() {
    cout << "\n===== 5. 偏特化 =====" << endl;
    // 通用版本
    DataContainer<string, int> dc1("张三", 100);
    dc1.show();

    // 偏特化（同类型）
    DataContainer<int, int> dc2(10, 20);
    dc2.show();
    cout << "  sum = " << dc2.sum() << endl;

    // 偏特化（指针）
    int val = 42;
    DataContainer<string, int*> dc3("值", &val);
    dc3.show();
}

void demonstrateTemplateInheritance() {
    cout << "\n===== 6. 模板类继承 =====" << endl;
    Dog<int> dog1("旺财", 3, "金毛");
    dog1.info();
    dog1.speak();
    dog1.showBreed();

    Dog<float> dog2("大黄", 2.5f, "柴犬");
    dog2.info();
    dog2.speak();

    Cat cat1("咪咪", 2, "白色");
    cat1.info();
    cat1.speak();
}

// ==================== 9. 主函数 ====================
int main() {
    cout << "========================================" << endl;
    cout << "       类模板详解 演示" << endl;
    cout << "========================================" << endl;

    demonstrateBasicTemplate();
    demonstrateMultiParams();
    demonstrateDefaultParams();
    demonstrateSpecialization();
    demonstratePartialSpecialization();
    demonstrateTemplateInheritance();

    cout << "\n========================================" << endl;
    cout << "  总结:" << endl;
    cout << "  1. template<class T> 定义类模板" << endl;
    cout << "  2. 类外定义: template<class T> void C<T>::func()" << endl;
    cout << "  3. 全特化: template<> class C<具体类型>" << endl;
    cout << "  4. 偏特化: template<class T> class C<T, T>" << endl;
    cout << "  5. 模板类继承时需要指定基类模板参数" << endl;
    cout << "  6. C++17 支持类模板参数推导(CTAD)" << endl;
    cout << "========================================" << endl;

    return 0;
}
