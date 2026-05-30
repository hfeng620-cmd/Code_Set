/*
 * 55_初始化列表.cpp
 * 本文件详细讲解C++中构造函数初始化列表的语法和使用场景
 * 初始化列表是C++推荐的成员初始化方式，效率高于函数体内赋值
 * 编译命令: g++ -o 55_初始化列表 55_初始化列表.cpp -std=c++11
 */

#include <iostream>
#include <string>
using namespace std;

// ===================== 1. 初始化列表基本语法 =====================
class Student {
private:
    string name;
    int age;
    float score;

public:
    // 初始化列表语法：
    // 构造函数名(参数) : 成员1(值1), 成员2(值2), 成员3(值3) { 函数体 }
    // 冒号(:)后面，逗号(,)分隔各成员的初始化

    // 使用初始化列表的构造函数
    Student(const string &n, int a, float s)
        : name(n), age(a), score(s)  // 初始化列表
    {
        // 函数体可以为空，因为初始化已在列表中完成
        cout << "  [构造] " << name << ", " << age << "岁" << endl;
    }

    void show() const {
        cout << "  " << name << ", " << age << "岁, " << score << "分" << endl;
    }
};

// ===================== 2. 初始化列表 vs 函数体赋值 =====================
class Comparison {
private:
    string name;
    int value;

public:
    // 方式1：初始化列表（推荐）
    // 直接用参数值初始化成员，一步到位
    Comparison(const string &n, int v) : name(n), value(v) {
        cout << "  [初始化列表] " << name << "=" << value << endl;
    }

    // 方式2：函数体赋值（不推荐）
    // 先调用成员的默认构造函数，再赋值（两步操作，效率低）
    // Comparison(const string &n, int v) {
    //     name = n;  // 先默认构造name(空字符串)，再赋值为n
    //     value = v; // 先默认构造value(随机值)，再赋值为v
    // }

    void show() const {
        cout << "  " << name << "=" << value << endl;
    }
};

// ===================== 3. const成员必须用初始化列表 =====================
class ConstMember {
private:
    const int id;         // const成员：值不可变，必须在初始化时赋值
    const string name;    // const字符串成员
    int normal;           // 普通成员

public:
    // const成员只能在初始化列表中初始化，不能在函数体中赋值
    // 因为const变量一旦初始化就不能修改，赋值是修改操作
    ConstMember(int i, const string &n, int v)
        : id(i), name(n), normal(v)  // const成员必须在这里初始化
    {
        // id = i;      // 错误！const成员不能赋值
        // name = n;    // 错误！const成员不能赋值
        // normal = v;  // 普通成员可以在这里赋值（但不如初始化列表高效）
        cout << "  [构造] ConstMember" << endl;
    }

    void show() const {
        cout << "  id=" << id << ", name=" << name << ", normal=" << normal << endl;
    }
};

// ===================== 4. 引用成员必须用初始化列表 =====================
class RefMember {
private:
    int &ref;        // 引用成员：必须在声明时绑定，不能后赋值
    int value;

public:
    // 引用成员必须在初始化列表中绑定，因为引用一旦创建就必须绑定到某个对象
    RefMember(int &r, int v) : ref(r), value(v) {
        // ref = r;  // 错误！这不是绑定引用，这是赋值给ref绑定的对象
        cout << "  [构造] RefMember, ref指向: " << &ref << endl;
    }

    void show() const {
        cout << "  ref=" << ref << ", value=" << value << endl;
    }
};

// ===================== 5. 初始化顺序与声明顺序 =====================
class OrderDemo {
private:
    // 重要：初始化顺序取决于成员的声明顺序，而不是初始化列表中的顺序
    int a;  // 第1个声明，第1个初始化
    int b;  // 第2个声明，第2个初始化
    int c;  // 第3个声明，第3个初始化

public:
    // 即使初始化列表写的是 c, b, a 的顺序
    // 实际初始化顺序仍然是 a, b, c（按声明顺序）
    OrderDemo(int x, int y, int z)
        : c(z), b(y), a(x)  // 顺序看起来是c->b->a
    {
        cout << "  [构造] 初始化后: a=" << a << ", b=" << b << ", c=" << c << endl;
        // a总是先于b初始化，b总是先于c初始化
    }

    void show() const {
        cout << "  a=" << a << ", b=" << b << ", c=" << c << endl;
    }
};

// ===================== 6. 成员初始化的依赖问题 =====================
class InitDependency {
private:
    int width;
    int height;
    int area;  // 依赖width和height

public:
    // area的初始化依赖于width和height
    // 由于初始化按声明顺序，width和height一定在area之前初始化
    // 所以这里area可以使用width和height的值
    InitDependency(int w, int h)
        : width(w), height(h), area(width * height)
    {
        cout << "  [构造] " << width << "x" << height << "=" << area << endl;
    }

    // 危险示例：如果声明顺序不当
    // int area;        // 先声明area
    // int width;       // 后声明width
    // int height;      // 后声明height
    // InitDependency(int w, int h)
    //     : width(w), height(h), area(width * height)  // area先初始化，但width/height还未初始化！
    // 这会导致area使用未初始化的width和height，是未定义行为

    void show() const {
        cout << "  面积: " << area << endl;
    }
};

// ===================== 7. 默认参数与初始化列表 =====================
class DefaultParam {
private:
    string name;
    int priority;
    double value;

public:
    // 构造函数参数可以有默认值
    DefaultParam(const string &n = "默认", int p = 0, double v = 0.0)
        : name(n), priority(p), value(v)
    {
        cout << "  [构造] " << name << " (优先级:" << priority << ")" << endl;
    }

    void show() const {
        cout << "  " << name << ", 优先级=" << priority << ", 值=" << value << endl;
    }
};

// ===================== 函数声明 =====================
void basicSyntaxDemo();
void constMemberDemo();
void refMemberDemo();
void orderDemo();
void dependencyDemo();
void defaultParamDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 初始化列表详解" << endl;
    cout << "========================================" << endl << endl;

    basicSyntaxDemo();    // 基本语法
    constMemberDemo();    // const成员
    refMemberDemo();      // 引用成员
    orderDemo();          // 初始化顺序
    dependencyDemo();     // 依赖问题
    defaultParamDemo();   // 默认参数

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== 基本语法 =====================
void basicSyntaxDemo() {
    cout << "===== 1. 初始化列表基本语法 =====" << endl;

    // 初始化列表的语法格式：
    // ClassName(参数) : member1(val1), member2(val2) { ... }

    Student s1("张三", 20, 95.5f);
    s1.show();

    Student s2("李四", 19, 88.0f);
    s2.show();

    // 初始化列表 vs 函数体赋值的效率对比：
    // 对于基本类型(int, float等)：几乎没有区别
    // 对于类类型(string等)：初始化列表效率更高
    //   - 初始化列表：直接调用拷贝构造函数，一步完成
    //   - 函数体赋值：先调用默认构造函数，再调用赋值运算符，两步完成

    // 建议：养成使用初始化列表的习惯

    cout << endl;
}

// ===================== const成员 =====================
void constMemberDemo() {
    cout << "===== 2. const成员必须用初始化列表 =====" << endl;

    // const成员在对象创建时必须有确定的值
    // 初始化列表是唯一能在创建时给const成员赋值的方式

    ConstMember cm1(1, "对象A", 100);
    cm1.show();

    ConstMember cm2(2, "对象B", 200);
    cm2.show();

    // 常见错误：
    // class Bad {
    //     const int x;
    // public:
    //     Bad(int v) { x = v; }  // 错误！不能在函数体中给const赋值
    // };
    // 正确写法：
    // class Good {
    //     const int x;
    // public:
    //     Good(int v) : x(v) {}  // 正确！在初始化列表中初始化
    // };

    cout << endl;
}

// ===================== 引用成员 =====================
void refMemberDemo() {
    cout << "===== 3. 引用成员必须用初始化列表 =====" << endl;

    // 引用在创建时必须绑定到某个对象，之后不能更改绑定
    // 初始化列表是唯一能在创建时绑定引用的方式

    int externalValue = 42;
    RefMember rm1(externalValue, 100);
    rm1.show();

    // 修改外部值会影响引用
    externalValue = 99;
    cout << "  修改externalValue后: ";
    rm1.show();  // ref的值也变了

    // 常见错误：
    // class Bad {
    //     int &ref;
    // public:
    //     Bad(int &r) { ref = r; }  // 错误！这是赋值，不是绑定引用
    // };
    // 正确写法：
    // class Good {
    //     int &ref;
    // public:
    //     Good(int &r) : ref(r) {}  // 正确！在初始化列表中绑定
    // };

    cout << endl;
}

// ===================== 初始化顺序 =====================
void orderDemo() {
    cout << "===== 4. 初始化顺序取决于声明顺序 =====" << endl;

    // 关键规则：成员按声明顺序初始化，不是按初始化列表中的顺序
    // 建议：初始化列表中的顺序与声明顺序保持一致

    OrderDemo od(1, 2, 3);
    od.show();

    // 演示初始化顺序问题
    cout << "\n--- 初始化顺序警告 ---" << endl;
    // 编译器通常会警告初始化列表顺序与声明顺序不一致
    // 这是一个潜在的bug来源

    // 最佳实践：初始化列表的顺序 = 声明顺序
    // 这样可以避免依赖问题

    cout << endl;
}

// ===================== 依赖问题 =====================
void dependencyDemo() {
    cout << "===== 5. 成员初始化的依赖问题 =====" << endl;

    // 如果一个成员的初始化依赖于另一个成员
    // 必须确保被依赖的成员先初始化（即先声明）

    InitDependency id(5, 8);
    id.show();

    // 错误示例（说明）：
    // class Bad {
    //     int area;     // 先声明area
    //     int width;    // 后声明width
    // public:
    //     Bad(int w) : width(w), area(width * 10) {}
    //     // area先初始化，但width还未初始化！area的值是未定义的！
    // };
    // 正确做法：确保width在area之前声明

    cout << endl;
}

// ===================== 默认参数 =====================
void defaultParamDemo() {
    cout << "===== 6. 默认参数与初始化列表 =====" << endl;

    // 构造函数参数可以有默认值，与初始化列表配合使用

    // 使用所有默认参数
    DefaultParam d1;
    d1.show();

    // 只指定第一个参数
    DefaultParam d2("优先任务");
    d2.show();

    // 指定前两个参数
    DefaultParam d3("紧急任务", 10);
    d3.show();

    // 指定所有参数
    DefaultParam d4("特殊任务", 5, 3.14);
    d4.show();

    // 注意：默认参数不要与重载产生二义性
    // DefaultParam() 和 DefaultParam(string="默认") 会冲突

    cout << endl;
}
