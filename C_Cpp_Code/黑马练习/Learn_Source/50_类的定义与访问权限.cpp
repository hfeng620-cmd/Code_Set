/*
 * 50_类的定义与访问权限.cpp
 * 本文件详细讲解C++中类(class)的定义和访问权限控制
 * 类是面向对象编程的核心，封装了数据和操作数据的方法
 * 编译命令: g++ -o 50_类的定义与访问权限 50_类的定义与访问权限.cpp -std=c++11
 */

#include <iostream>
#include <string>
using namespace std;

// ===================== 1. 类的基本定义 =====================
// class关键字定义类
// 类是结构体的升级版，默认访问权限是private
class Person {
    // private: 私有成员，只能在类内部访问（默认权限）
private:
    string name;     // 姓名（私有，外部不能直接访问）
    int age;         // 年龄（私有）
    string idCard;   // 身份证号（私有，敏感信息）

    // private: 可以有私有成员函数，只在类内部调用
    void validateAge(int a) {
        if (a < 0 || a > 150) {
            cout << "警告: 年龄 " << a << " 不合理！" << endl;
        }
    }

    // public: 公有成员，类内外都可以访问
public:
    // 构造函数
    Person() : name("未命名"), age(0), idCard("未知") {}

    Person(const string &n, int a, const string &id)
        : name(n), age(a), idCard(id) {
        validateAge(a);  // 调用私有函数
    }

    // 公有成员函数（接口）：通过这些函数访问私有数据
    // getter函数：获取私有成员的值
    string getName() const { return name; }
    int getAge() const { return age; }
    string getIdCard() const { return idCard; }

    // setter函数：设置私有成员的值（可以加验证逻辑）
    void setName(const string &n) { name = n; }
    void setAge(int a) {
        validateAge(a);  // 设置前验证
        age = a;
    }
    void setIdCard(const string &id) { idCard = id; }

    // 显示信息的公有函数
    void showInfo() const {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
        // 注意：showInfo是类的成员函数，可以访问private成员idCard
        // 但这里选择不显示敏感信息
    }

    // protected: 保护成员，类内和子类可以访问，外部不能访问
protected:
    string school;  // 学校（保护成员）

public:
    void setSchool(const string &s) { school = s; }
    void showSchool() const {
        cout << "学校: " << school << endl;
    }
};

// ===================== 2. class vs struct 的区别 =====================
// struct定义的类，默认权限是public
struct StructPoint {
    double x;  // 默认public
    double y;  // 默认public
};

// class定义的类，默认权限是private
class ClassPoint {
    double x;  // 默认private
    double y;  // 默认private
public:
    // 需要显式声明public才能外部访问
    ClassPoint(double x = 0, double y = 0) : x(x), y(y) {}
    double getX() const { return x; }
    double getY() const { return y; }
};

// ===================== 3. 封装的实际应用 =====================
// 一个简单的银行账户类，展示封装的好处
class BankAccount {
private:
    string owner;       // 户主
    double balance;     // 余额（私有，防止外部直接修改）
    string password;    // 密码（私有，绝对不能让外部直接访问）

public:
    // 构造函数
    BankAccount(const string &o, double b, const string &pwd)
        : owner(o), balance(b), password(pwd) {}

    // 存款（公开接口，有验证逻辑）
    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "错误: 存款金额必须为正数！" << endl;
            return false;
        }
        balance += amount;
        cout << "存款成功: +" << amount << ", 余额: " << balance << endl;
        return true;
    }

    // 取款（公开接口，有验证逻辑）
    bool withdraw(double amount, const string &pwd) {
        // 验证密码
        if (pwd != password) {
            cout << "错误: 密码不正确！" << endl;
            return false;
        }
        // 验证金额
        if (amount <= 0) {
            cout << "错误: 取款金额必须为正数！" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "错误: 余额不足！当前余额: " << balance << endl;
            return false;
        }
        balance -= amount;
        cout << "取款成功: -" << amount << ", 余额: " << balance << endl;
        return true;
    }

    // 查询余额（需要密码）
    double getBalance(const string &pwd) const {
        if (pwd != password) {
            cout << "错误: 密码不正确！" << endl;
            return -1;
        }
        return balance;
    }

    // 显示账户信息（不显示密码和余额）
    void showInfo() const {
        cout << "户主: " << owner << endl;
        // 密码和余额不能直接显示
    }
};

// ===================== 4. 类的成员函数定义 =====================
// 可以在类外定义成员函数，使用作用域运算符 ::
class Calculator {
private:
    double result;  // 存储计算结果

public:
    // 类内声明
    Calculator();
    Calculator(double initial);
    double add(double num);
    double subtract(double num);
    double multiply(double num);
    double divide(double num);
    double getResult() const;
    void reset();
};

// 类外定义（使用::作用域运算符）
Calculator::Calculator() : result(0.0) {}
Calculator::Calculator(double initial) : result(initial) {}

double Calculator::add(double num) {
    result += num;
    return result;
}

double Calculator::subtract(double num) {
    result -= num;
    return result;
}

double Calculator::multiply(double num) {
    result *= num;
    return result;
}

double Calculator::divide(double num) {
    if (num == 0) {
        cout << "错误: 除数不能为0！" << endl;
        return result;
    }
    result /= num;
    return result;
}

double Calculator::getResult() const {
    return result;
}

void Calculator::reset() {
    result = 0.0;
}

// ===================== 5. 函数声明 =====================
void classVsStructDemo();
void encapsulationDemo();
void accessControlDemo();
void classDefinitionDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 类的定义与访问权限" << endl;
    cout << "========================================" << endl << endl;

    classVsStructDemo();    // class vs struct
    classDefinitionDemo();  // 类的定义方式
    encapsulationDemo();    // 封装应用
    accessControlDemo();    // 访问权限

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== class vs struct =====================
void classVsStructDemo() {
    cout << "===== class vs struct 区别 =====" << endl;

    // struct默认public
    StructPoint sp;
    sp.x = 3.0;   // 可以直接访问，因为默认public
    sp.y = 4.0;
    cout << "StructPoint: (" << sp.x << ", " << sp.y << ")" << endl;

    // class默认private
    ClassPoint cp(3.0, 4.0);
    // cp.x = 3.0;  // 错误！默认private，不能直接访问
    cout << "ClassPoint: (" << cp.getX() << ", " << cp.getY() << ")" << endl;

    // 总结：class和struct在C++中唯一的区别就是默认访问权限
    // class默认private，struct默认public
    // 习惯上：用struct定义纯数据结构，用class定义有行为的对象

    cout << endl;
}

// ===================== 类的定义方式 =====================
void classDefinitionDemo() {
    cout << "===== 类的定义方式 =====" << endl;

    // 使用默认构造函数
    Person p1;
    p1.showInfo();

    // 使用参数化构造函数
    Person p2("张三", 25, "110101199001011234");
    p2.showInfo();

    // 通过公有接口修改私有数据
    p2.setAge(26);
    p2.setName("张三丰");
    p2.showInfo();

    // 类外定义成员函数的方式（Calculator类）
    cout << "\nCalculator示例:" << endl;
    Calculator calc(10);
    cout << "初始值: " << calc.getResult() << endl;
    calc.add(5);
    cout << "+5 = " << calc.getResult() << endl;
    calc.multiply(3);
    cout << "*3 = " << calc.getResult() << endl;
    calc.subtract(10);
    cout << "-10 = " << calc.getResult() << endl;
    calc.divide(5);
    cout << "/5 = " << calc.getResult() << endl;

    cout << endl;
}

// ===================== 封装的实际应用 =====================
void encapsulationDemo() {
    cout << "===== 封装的实际应用 =====" << endl;

    BankAccount account("张三", 10000, "123456");

    account.showInfo();

    // 正常操作
    account.deposit(5000);
    account.withdraw(3000, "123456");
    cout << "余额查询: " << account.getBalance("123456") << endl;

    // 错误操作（被封装的逻辑拦截）
    cout << "\n错误操作测试:" << endl;
    account.deposit(-100);          // 负数存款
    account.withdraw(20000, "123456");  // 余额不足
    account.withdraw(100, "wrong");     // 密码错误
    account.getBalance("wrong");        // 密码错误

    // 封装的好处：
    // 1. 数据被保护，不能被随意修改
    // 2. 可以在setter中加入验证逻辑
    // 3. 外部只需知道接口，不需要了解内部实现
    // 4. 修改内部实现不会影响外部代码

    // 如果balance是public的，任何人都可以：
    // account.balance = -999999;  // 灾难！
    // 有了封装，必须通过deposit/withdraw，确保了数据安全

    cout << endl;
}

// ===================== 访问权限详解 =====================
void accessControlDemo() {
    cout << "===== 访问权限详解 =====" << endl;

    Person p("李四", 30, "110101199501011234");

    // public成员：可以外部访问
    cout << "public - getName(): " << p.getName() << endl;
    cout << "public - getAge(): " << p.getAge() << endl;
    p.showInfo();  // public函数

    // private成员：不能外部访问
    // cout << p.name;     // 错误！name是private
    // cout << p.age;      // 错误！age是private
    // p.validateAge(20);  // 错误！validateAge是private
    // 只能通过public的getter/setter访问

    // protected成员：不能外部访问（但子类可以，后续继承课程讲解）
    // cout << p.school;   // 错误！school是protected
    p.setSchool("清华大学");  // 通过public函数设置
    p.showSchool();           // 通过public函数查看

    // 访问权限总结：
    // public    : 类内、类外、子类都能访问
    // protected : 类内、子类可以访问，类外不能访问
    // private   : 只有类内可以访问（最严格的保护）

    // 设计原则：
    // 1. 成员变量一般设为private
    // 2. 对外提供public的getter/setter
    // 3. 敏感数据不要提供setter
    // 4. 需要被子类访问的设为protected

    cout << endl;
}
