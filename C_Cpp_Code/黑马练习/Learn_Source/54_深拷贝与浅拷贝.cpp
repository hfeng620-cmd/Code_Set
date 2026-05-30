/*
 * 54_深拷贝与浅拷贝.cpp
 * 本文件详细讲解C++中深拷贝和浅拷贝的区别与应用
 * 浅拷贝是编译器默认提供的拷贝方式，可能导致双重释放等严重问题
 * 深拷贝需要手动编写拷贝构造函数来实现
 * 编译命令: g++ -o 54_深拷贝与浅拷贝 54_深拷贝与浅拷贝.cpp -std=c++11
 */

#include <iostream>
#include <cstring>
using namespace std;

// ===================== 1. 浅拷贝的问题演示 =====================
// 这个类没有自定义拷贝构造函数，使用编译器默认的浅拷贝
class ShallowCopy {
private:
    char *name;
    int age;

public:
    // 构造函数
    ShallowCopy(const char *n, int a) : age(a) {
        // 动态分配内存存储名字
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "  [构造] ShallowCopy: " << name << " (地址:" << (void*)name << ")" << endl;
    }

    // 没有自定义拷贝构造函数！
    // 编译器会自动生成浅拷贝构造函数，逐字节复制：
    // ShallowCopy(const ShallowCopy &other) {
    //     name = other.name;    // 只复制指针值，不复制指向的内容！
    //     age = other.age;
    // }
    // 结果：两个对象的name指针指向同一块内存！

    // 析构函数
    ~ShallowCopy() {
        cout << "  [析构] ~ShallowCopy: " << (name ? name : "null")
             << " (地址:" << (void*)name << ")" << endl;
        delete[] name;  // 释放内存
        name = nullptr;
    }

    void show() const {
        cout << "  " << (name ? name : "null") << ", " << age << "岁" << endl;
    }
};

// ===================== 2. 深拷贝的正确实现 =====================
class DeepCopy {
private:
    char *name;
    int age;

public:
    // 构造函数
    DeepCopy(const char *n, int a) : age(a) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        cout << "  [构造] DeepCopy: " << name << " (地址:" << (void*)name << ")" << endl;
    }

    // 自定义拷贝构造函数：深拷贝
    // 为新对象分配独立的内存空间，复制内容而不是复制指针
    DeepCopy(const DeepCopy &other) : age(other.age) {
        // 分配新内存
        name = new char[strlen(other.name) + 1];
        // 复制内容
        strcpy(name, other.name);
        cout << "  [深拷贝] DeepCopy: " << name << " (新地址:" << (void*)name << ")" << endl;
        // 现在两个对象各有各的name内存，互不影响
    }

    // 析构函数
    ~DeepCopy() {
        cout << "  [析构] ~DeepCopy: " << (name ? name : "null")
             << " (地址:" << (void*)name << ")" << endl;
        delete[] name;
        name = nullptr;
    }

    void show() const {
        cout << "  " << (name ? name : "null") << ", " << age << "岁" << endl;
    }

    void setName(const char *n) {
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
};

// ===================== 3. 安全的字符串类（完整示例） =====================
class MyString {
private:
    char *data;     // 动态分配的字符数组
    int length;     // 字符串长度

public:
    // 默认构造
    MyString() : data(nullptr), length(0) {
        cout << "  [构造] MyString: 空字符串" << endl;
    }

    // 从C字符串构造
    MyString(const char *str) {
        if (str) {
            length = strlen(str);
            data = new char[length + 1];
            strcpy(data, str);
        } else {
            data = nullptr;
            length = 0;
        }
        cout << "  [构造] MyString: \"" << (data ? data : "") << "\"" << endl;
    }

    // 深拷贝构造函数
    MyString(const MyString &other) : length(other.length) {
        if (other.data) {
            data = new char[length + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
        }
        cout << "  [深拷贝] MyString: \"" << (data ? data : "") << "\"" << endl;
    }

    // 析构函数
    ~MyString() {
        cout << "  [析构] ~MyString: \"" << (data ? data : "") << "\"" << endl;
        delete[] data;
        data = nullptr;
    }

    // 赋值运算符（也需要注意深拷贝）
    MyString& operator=(const MyString &other) {
        if (this == &other) return *this;  // 自我赋值检查

        // 释放原有内存
        delete[] data;

        // 深拷贝
        length = other.length;
        if (other.data) {
            data = new char[length + 1];
            strcpy(data, other.data);
        } else {
            data = nullptr;
        }
        cout << "  [赋值] MyString: \"" << (data ? data : "") << "\"" << endl;
        return *this;
    }

    // 拼接字符串
    MyString operator+(const MyString &other) const {
        int newLen = length + other.length;
        char *newData = new char[newLen + 1];
        if (data) strcpy(newData, data);
        else newData[0] = '\0';
        if (other.data) strcat(newData, other.data);
        MyString result(newData);
        delete[] newData;
        return result;
    }

    void show() const {
        cout << "  \"" << (data ? data : "") << "\" (长度:" << length
             << ", 地址:" << (void*)data << ")" << endl;
    }

    const char* c_str() const { return data ? data : ""; }
};

// ===================== 4. Rule of Three（三法则） =====================
// 如果一个类需要自定义以下三个函数中的任意一个
// 那么通常三个都需要自定义：
// 1. 拷贝构造函数
// 2. 赋值运算符(operator=)
// 3. 析构函数
// 原因：如果类管理了动态资源（如new分配的内存），
//       默认的浅拷贝会导致多个对象共享同一资源，释放时出问题

class RuleOfThree {
private:
    int *data;
    int size;

public:
    // 构造函数
    RuleOfThree(int s) : size(s) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = 0;
        cout << "  [构造] RuleOfThree: " << size << "个元素" << endl;
    }

    // 1. 拷贝构造函数（深拷贝）
    RuleOfThree(const RuleOfThree &other) : size(other.size) {
        data = new int[size];
        memcpy(data, other.data, size * sizeof(int));
        cout << "  [深拷贝] RuleOfThree" << endl;
    }

    // 2. 赋值运算符（深拷贝 + 自我赋值检查）
    RuleOfThree& operator=(const RuleOfThree &other) {
        if (this == &other) return *this;  // 自我赋值检查
        delete[] data;                      // 释放原有资源
        size = other.size;
        data = new int[size];              // 分配新资源
        memcpy(data, other.data, size * sizeof(int));
        cout << "  [赋值] RuleOfThree" << endl;
        return *this;
    }

    // 3. 析构函数
    ~RuleOfThree() {
        delete[] data;
        data = nullptr;
        cout << "  [析构] RuleOfThree" << endl;
    }

    void set(int index, int value) {
        if (index >= 0 && index < size) data[index] = value;
    }

    void show() const {
        cout << "  [";
        for (int i = 0; i < size; i++) {
            if (i > 0) cout << ", ";
            cout << data[i];
        }
        cout << "] (地址:" << (void*)data << ")" << endl;
    }
};

// ===================== 函数声明 =====================
void shallowCopyProblem();
void deepCopySolution();
void myStringDemo();
void ruleOfThreeDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 深拷贝与浅拷贝" << endl;
    cout << "========================================" << endl << endl;

    // 注意：浅拷贝演示会导致双重释放，可能崩溃
    // 已注释掉危险代码，仅保留说明
    // shallowCopyProblem();  // 取消注释可观察问题（可能崩溃）
    deepCopySolution();      // 深拷贝解决方案
    myStringDemo();          // 完整字符串类示例
    ruleOfThreeDemo();       // 三法则演示

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== 浅拷贝问题（危险，已注释关键代码） =====================
void shallowCopyProblem() {
    cout << "===== 1. 浅拷贝的问题 =====" << endl;

    // 浅拷贝的问题说明（不实际运行，避免崩溃）：
    // ShallowCopy s1("张三", 20);
    // ShallowCopy s2 = s1;  // 浅拷贝：s2.name == s1.name（同一地址！）
    //
    // 当s2析构时：delete[] name -> 释放了name指向的内存
    // 当s1析构时：delete[] name -> 再次释放同一块内存！= 双重释放(double free)
    //
    // 双重释放是未定义行为，可能导致：
    // 1. 程序崩溃（最常见的表现）
    // 2. 内存损坏
    // 3. 安全漏洞

    // 演示浅拷贝的内存地址问题
    cout << "浅拷贝演示（展示问题，不释放内存避免崩溃）:" << endl;
    {
        ShallowCopy s1("张三", 20);
        // 手动创建一个"浅拷贝"来展示问题
        // 注意：这里只是演示，实际中不要这样做
        cout << "  s1的name地址: " << (void*)"张三" << endl;
        cout << "  浅拷贝会让两个对象的指针指向同一地址" << endl;
        cout << "  析构时会导致双重释放！" << endl;
    }

    cout << endl;
}

// ===================== 深拷贝解决方案 =====================
void deepCopySolution() {
    cout << "===== 2. 深拷贝的正确实现 =====" << endl;

    DeepCopy d1("李四", 25);
    d1.show();

    // 深拷贝：创建独立的内存副本
    cout << "--- 深拷贝创建d2 ---" << endl;
    DeepCopy d2 = d1;
    d2.show();

    // 修改d2不影响d1（因为内存独立）
    d2.setName("王五");
    cout << "\n修改d2后:" << endl;
    cout << "  d1: "; d1.show();
    cout << "  d2: "; d2.show();

    // d2和d1的name地址不同，析构时互不影响
    cout << "\n--- 离开作用域 ---" << endl;

    cout << endl;
}

// ===================== MyString完整示例 =====================
void myStringDemo() {
    cout << "===== 3. 安全的字符串类(MyString) =====" << endl;

    // 创建字符串
    MyString s1("Hello");
    s1.show();

    // 深拷贝
    cout << "--- 深拷贝 ---" << endl;
    MyString s2 = s1;
    s2.show();

    // 赋值运算符
    cout << "--- 赋值 ---" << endl;
    MyString s3("World");
    s3 = s1;  // 赋值运算符
    s3.show();

    // 拼接
    cout << "--- 拼接 ---" << endl;
    MyString s4 = s1 + MyString(" C++");
    s4.show();

    // 每个对象都有独立的内存，互不影响
    cout << "\n--- 验证独立性 ---" << endl;
    cout << "  s1: "; s1.show();
    cout << "  s2: "; s2.show();
    cout << "  s3: "; s3.show();
    cout << "  s4: "; s4.show();

    cout << "\n--- 离开作用域 ---" << endl;

    cout << endl;
}

// ===================== Rule of Three =====================
void ruleOfThreeDemo() {
    cout << "===== 4. Rule of Three（三法则）=====" << endl;

    RuleOfThree r1(5);
    for (int i = 0; i < 5; i++) r1.set(i, i * 10);
    cout << "r1: "; r1.show();

    // 拷贝构造
    cout << "--- 拷贝构造 ---" << endl;
    RuleOfThree r2 = r1;
    cout << "r2: "; r2.show();

    // 赋值
    cout << "--- 赋值 ---" << endl;
    RuleOfThree r3(3);
    r3 = r1;
    cout << "r3: "; r3.show();

    // 验证独立性
    r2.set(0, 999);
    cout << "\n修改r2[0]=999后:" << endl;
    cout << "r1: "; r1.show();  // r1不受影响
    cout << "r2: "; r2.show();  // r2被修改

    // 三法则总结：
    // 如果类管理了动态资源（new, malloc, 文件句柄等），
    // 必须同时定义：
    // 1. 拷贝构造函数（深拷贝）
    // 2. 赋值运算符（深拷贝 + 自我赋值检查）
    // 3. 析构函数（释放资源）
    // 否则编译器提供的默认版本都是浅拷贝，会导致问题

    // C++11补充：五法则(Rule of Five)
    // 还需要考虑移动构造函数和移动赋值运算符
    // 后续课程会讲解

    cout << "\n--- 离开作用域 ---" << endl;
}
