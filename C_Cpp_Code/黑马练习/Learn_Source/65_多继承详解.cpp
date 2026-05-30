/*
 * 文件名: 65_多继承详解.cpp
 * 描述: C++多继承的语法、菱形继承问题和虚继承
 * 知识点:
 *   1. 多继承的语法
 *   2. 菱形继承(Diamond Problem)
 *   3. 虚继承解决菱形问题
 *   4. 多继承中的构造函数调用顺序
 *   5. 何时使用多继承
 * 编译: g++ -std=c++11 65_多继承详解.cpp -o 65_多继承详解
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 一、多继承的基本语法
// ============================================================
// 多继承: 一个类可以同时继承多个基类
// 语法: class Derived : public Base1, public Base2, ...
// 各基类之间用逗号分隔,每个基类前可以指定不同的继承方式
// ============================================================

// 基类: 姓名信息
class NameInfo {
protected:
    string name;
public:
    NameInfo(string n = "未知") : name(n) {
        cout << "  [NameInfo] 构造: " << name << endl;
    }
    ~NameInfo() { cout << "  [NameInfo] 析构: " << name << endl; }

    void showName() const { cout << "姓名: " << name << endl; }
};

// 基类: 年龄信息
class AgeInfo {
protected:
    int age;
public:
    AgeInfo(int a = 0) : age(a) {
        cout << "  [AgeInfo] 构造: age=" << age << endl;
    }
    ~AgeInfo() { cout << "  [AgeInfo] 析构" << endl; }

    void showAge() const { cout << "年龄: " << age << endl; }
};

// 多继承: Person 同时继承 NameInfo 和 AgeInfo
class PersonMulti : public NameInfo, public AgeInfo {
private:
    string city;
public:
    // ============================================================
    // 四、多继承中的构造函数调用顺序
    // ============================================================
    // 构造顺序: 按照继承声明的顺序(不是初始化列表的顺序!)
    // 即 NameInfo -> AgeInfo -> PersonMulti
    // 析构顺序: 与构造相反
    // ============================================================

    PersonMulti(string n, int a, string c)
        : NameInfo(n),     // 先构造
          AgeInfo(a),      // 后构造
          city(c) {
        cout << "  [PersonMulti] 构造: city=" << city << endl;
    }
    ~PersonMulti() { cout << "  [PersonMulti] 析构" << endl; }

    void show() const {
        showName();
        showAge();
        cout << "城市: " << city << endl;
    }
};

// ============================================================
// 二、菱形继承(Diamond Problem)
// ============================================================
//
//        Animal (虚基类)
//        /    \
//     Bird   Horse
//        \    /
//        Pegasus (飞马)
//
// 问题:
//   1. Pegasus 继承了两份 Animal 的数据(来自 Bird 和 Horse)
//   2. 访问 Animal 的成员时会产生二义性(编译错误)
//   3. 数据冗余和不一致
// ============================================================

// ---- 不使用虚继承的菱形继承(有问题) ----

class Animal {
public:
    string species;
    int age;

    Animal(string sp = "动物", int a = 0) : species(sp), age(a) {
        cout << "  [Animal] 构造: " << species << endl;
    }
    ~Animal() { cout << "  [Animal] 析构: " << species << endl; }

    void eat() const {
        cout << "  [" << species << "] 正在吃东西" << endl;
    }
};

class Bird : public Animal {
public:
    Bird(string sp = "鸟") : Animal(sp) {
        cout << "  [Bird] 构造" << endl;
    }
    ~Bird() { cout << "  [Bird] 析构" << endl; }

    void fly() const {
        cout << "  [" << species << "] 正在飞" << endl;
    }
};

class Horse : public Animal {
public:
    Horse(string sp = "马") : Animal(sp) {
        cout << "  [Horse] 构造" << endl;
    }
    ~Horse() { cout << "  [Horse] 析构" << endl; }

    void run() const {
        cout << "  [" << species << "] 正在跑" << endl;
    }
};

// 菱形继承(有问题的版本)
class Pegasus_Bad : public Bird, public Horse {
public:
    Pegasus_Bad() : Bird("飞马(鸟)"), Horse("飞马(马)") {
        cout << "  [Pegasus_Bad] 构造" << endl;
    }
    ~Pegasus_Bad() { cout << "  [Pegasus_Bad] 析构" << endl; }

    void show() const {
        // 错误! 访问 species 有二义性(来自 Bird 还是 Horse?)
        // cout << species << endl;  // 编译错误

        // 忀需要显式指定来源
        cout << "  来自Bird的species: " << Bird::species << endl;
        cout << "  来自Horse的species: " << Horse::species << endl;
        // 注意: 这两个 species 是不同的数据!数据冗余
    }
};

// ============================================================
// 三、虚继承(Virtual Inheritance)解决菱形问题
// ============================================================
// 虚继承: 在继承声明中使用 virtual 关键字
// 语法: class Derived : virtual public Base
//
// 效果:
//   - 虚基类在整个继承体系中只有一份数据
//   - 解决了数据冗余和二义性问题
//   - 最底层的派生类负责调用虚基类的构造函数
//
// 注意:
//   - virtual 应加在"中间层"(Bird 和 Horse),不是最底层
//   - 虚基类的构造由最底层派生类(Pegasus)直接调用
// ============================================================

// 虚基类
class AnimalV {
public:
    string species;
    int age;

    AnimalV(string sp = "动物", int a = 0) : species(sp), age(a) {
        cout << "  [AnimalV] 构造: " << species << endl;
    }
    virtual ~AnimalV() { cout << "  [AnimalV] 析构: " << species << endl; }

    void eat() const {
        cout << "  [" << species << "] 正在吃东西" << endl;
    }
};

// 使用虚继承
class BirdV : virtual public AnimalV {
public:
    BirdV(string sp = "鸟") : AnimalV(sp) {
        cout << "  [BirdV] 构造" << endl;
    }
    ~BirdV() { cout << "  [BirdV] 析构" << endl; }

    void fly() const {
        cout << "  [" << species << "] 正在飞" << endl;
    }
};

class HorseV : virtual public AnimalV {
public:
    HorseV(string sp = "马") : AnimalV(sp) {
        cout << "  [HorseV] 构造" << endl;
    }
    ~HorseV() { cout << "  [HorseV] 析构" << endl; }

    void run() const {
        cout << "  [" << species << "] 正在跑" << endl;
    }
};

// 正确的菱形继承(使用虚继承)
class Pegasus : public BirdV, public HorseV {
public:
    // 注意: 最底层派生类必须直接调用虚基类的构造函数
    // 如果不调用,虚基类将使用默认构造函数
    Pegasus() : AnimalV("飞马"), BirdV("飞马"), HorseV("飞马") {
        cout << "  [Pegasus] 构造" << endl;
    }
    ~Pegasus() { cout << "  [Pegasus] 析构" << endl; }

    void show() const {
        // 使用虚继承后,species 只有一份,没有二义性
        cout << "  物种: " << species << endl;   // OK!
        cout << "  年龄: " << age << endl;       // OK!
        eat();   // OK,只有一份
        fly();   // 来自 BirdV
        run();   // 来自 HorseV
    }
};

// ============================================================
// 五、何时使用多继承
// ============================================================
// 适合使用多继承的场景:
//   1. 组合多个不相关的功能(如: 可序列化 + 可比较)
//   2. 实现接口模式(Java/C#中的多接口实现)
//   3. Mixin 模式(混入功能)
//
// 不建议使用多继承的场景:
//   1. 可以用单一继承+组合解决的问题
//   2. 会导致菱形继承且无法用虚继承解决的情况
//   3. 继承关系过于复杂,难以理解
//
// 替代方案:
//   1. 组合(Composition): 将其他类作为成员
//   2. 接口(Interface): 纯虚基类
//   3. CRTP: 奇异递归模板模式
// ============================================================

// 接口模式示例
class ISerializable {
public:
    virtual ~ISerializable() {}
    virtual string serialize() const = 0;       // 纯虚函数
    virtual void deserialize(const string& data) = 0;
};

class IPrintable {
public:
    virtual ~IPrintable() {}
    virtual void print() const = 0;             // 纯虚函数
};

// 实现多个接口
class Document : public ISerializable, public IPrintable {
private:
    string content;
public:
    Document(string c = "") : content(c) {}

    // 实现 ISerializable
    string serialize() const override {
        return "DOC:" + content;
    }
    void deserialize(const string& data) override {
        if (data.substr(0, 4) == "DOC:") {
            content = data.substr(4);
        }
    }

    // 实现 IPrintable
    void print() const override {
        cout << "  [Document] " << content << endl;
    }
};

// ============================================================
// 六、常见错误
// ============================================================
// 错误1: 忘记使用虚继承导致数据冗余
//   在菱形继承中,如果不用虚继承,会有两份基类数据
//
// 错误2: 虚基类构造函数调用错误
//   虚基类的构造函数由最底层派生类直接调用
//   中间层的构造函数中对虚基类构造函数的调用会被忽略
//
// 错误3: 多继承中的二义性
//   当多个基类有同名成员时,需要显式指定作用域
// ============================================================

int main() {
    cout << "===== 多继承详解演示 =====" << endl;

    // --- 基本多继承 ---
    cout << "\n--- 1. 基本多继承 ---" << endl;
    cout << "创建 PersonMulti 对象(构造顺序):" << endl;
    PersonMulti pm("张三", 25, "北京");
    cout << "\n显示信息:" << endl;
    pm.show();

    // --- 菱形继承(有问题) ---
    cout << "\n--- 2. 菱形继承(有问题的版本) ---" << endl;
    {
        Pegasus_Bad pg;
        pg.show();
        // 注意: Animal 被构造了两次(来自Bird和Horse)
        // 数据冗余!
    }

    // --- 虚继承解决菱形问题 ---
    cout << "\n--- 3. 虚继承(正确版本) ---" << endl;
    {
        Pegasus pg;
        pg.show();
        // 注意: AnimalV 只被构造了一次
        // 数据不冗余,没有二义性!
    }

    // --- 接口模式(多继承的实际应用) ---
    cout << "\n--- 4. 接口模式 ---" << endl;
    Document doc("Hello, World!");
    doc.print();

    string data = doc.serialize();
    cout << "  序列化: " << data << endl;

    Document doc2;
    doc2.deserialize(data);
    cout << "  反序列化后: ";
    doc2.print();

    // 通过接口指针操作
    ISerializable* serPtr = &doc;
    IPrintable* priPtr = &doc;
    cout << "  通过ISerializable: " << serPtr->serialize() << endl;
    cout << "  通过IPrintable: ";
    priPtr->print();

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. 多继承语法: class D : public A, public B" << endl;
    cout << "2. 菱形继承导致数据冗余和二义性" << endl;
    cout << "3. 虚继承(virtual)解决菱形继承问题" << endl;
    cout << "4. 虚基类由最底层派生类负责构造" << endl;
    cout << "5. 多继承的构造顺序按声明顺序" << endl;
    cout << "6. 接口模式是多继承的常见应用" << endl;
    cout << "7. 能用单一继承解决的,尽量不用多继承" << endl;

    return 0;
}
