/*
 * ============================================================================
 * 51_封装详解.cpp
 * ============================================================================
 *
 * 本文件详细讲解 C++ 中封装(Encapsulation)的概念和实践。
 * 封装是面向对象编程三大特性之一（封装、继承、多态）。
 *
 * 知识点：
 *   1. 为什么要封装：数据隐藏、接口隔离
 *   2. Getter/Setter 模式的设计
 *   3. Setter 中的验证逻辑
 *   4. 封装的设计原则
 *   5. 封装的实际应用场景
 *
 * 编译命令：g++ -std=c++11 51_封装详解.cpp -o 51_encapsulation
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// ===========================================================================
// 第一部分：为什么要封装 —— 数据隐藏
// ===========================================================================
// 如果不封装，类的数据可以被随意修改，导致数据不一致。
// 封装将数据（属性）和操作数据的方法（行为）绑定在一起，
// 对外只暴露必要的接口，隐藏内部实现细节。

// 不封装的反面教材
struct BadStudent
{
    string name;
    int age;
    double score;
    // 任何外部代码都可以直接修改这些字段
    // 比如 score = -999; age = -5; 都是合法的，但不合理
};

// 封装的正确做法
class Student
{
private:
    // 数据隐藏：外部不能直接访问
    string m_name;
    int m_age;
    double m_score;

public:
    // 构造函数
    Student(const string &name, int age, double score)
    {
        // 构造函数中也可以调用 setter 进行验证
        setName(name);
        setAge(age);
        setScore(score);
    }

    // ---- Getter 模式（获取器） ----
    // 返回私有成员的值，通常用 const 修饰表示只读
    // 命名习惯：get + 成员名
    string getName() const { return m_name; }
    int getAge() const { return m_age; }
    double getScore() const { return m_score; }

    // ---- Setter 模式（设置器） ----
    // 设置私有成员的值，可以在内部加入验证逻辑
    // 命名习惯：set + 成员名
    void setName(const string &name)
    {
        // 验证：名字不能为空
        if (name.empty())
        {
            cout << "[警告] 姓名不能为空，未修改！" << endl;
            return;
        }
        m_name = name;
    }

    void setAge(int age)
    {
        // 验证：年龄在合理范围内
        if (age < 0 || age > 150)
        {
            cout << "[警告] 年龄 " << age << " 不合理（应在0-150之间），未修改！" << endl;
            return;
        }
        m_age = age;
    }

    void setScore(double score)
    {
        // 验证：分数在 0-100 之间
        if (score < 0 || score > 100)
        {
            cout << "[警告] 分数 " << score << " 不合法（应在0-100之间），未修改！" << endl;
            return;
        }
        m_score = score;
    }

    // 展示信息的公有接口
    void showInfo() const
    {
        cout << "姓名：" << m_name << "，年龄：" << m_age
             << "，分数：" << m_score << endl;
    }
};

// ===========================================================================
// 第二部分：只读属性 —— 只有 Getter 没有 Setter
// ===========================================================================
// 某些属性一旦初始化就不应被修改，可以只提供 getter 不提供 setter。

class Circle
{
private:
    double m_radius;               // 半径（可通过 setter 修改）
    const double m_pi = 3.14159;   // 圆周率（常量，不需要 setter）

public:
    Circle(double radius) : m_radius(radius) {}

    // 只读接口：圆周率
    double getPi() const { return m_pi; }

    // 可读可写接口：半径
    double getRadius() const { return m_radius; }
    void setRadius(double r)
    {
        if (r > 0)
            m_radius = r;
        else
            cout << "[警告] 半径必须为正数！" << endl;
    }

    // 派生属性：面积（由半径计算得来，只读）
    double getArea() const { return m_pi * m_radius * m_radius; }

    // 派生属性：周长（只读）
    double getCircumference() const { return 2 * m_pi * m_radius; }
};

// ===========================================================================
// 第三部分：封装的设计模式 —— 银行账户
// ===========================================================================
// 封装不仅能保护数据，还能在接口层面保证业务逻辑的一致性。

class BankAccount
{
private:
    string m_owner;       // 户主
    double m_balance;     // 余额
    string m_password;    // 密码（高度敏感，绝对不能暴露）
    bool m_isFrozen;      // 是否冻结

    // 私有方法：验证密码（内部使用，不对外暴露）
    bool verifyPassword(const string &pwd) const
    {
        return pwd == m_password;
    }

public:
    // 构造函数
    BankAccount(const string &owner, double balance, const string &password)
        : m_owner(owner), m_balance(balance >= 0 ? balance : 0),
          m_password(password), m_isFrozen(false) {}

    // 获取户主名（只读）
    string getOwner() const { return m_owner; }

    // 查询余额（需要密码验证）
    double getBalance(const string &pwd) const
    {
        if (!verifyPassword(pwd))
        {
            cout << "[错误] 密码不正确！" << endl;
            return -1;
        }
        if (m_isFrozen)
        {
            cout << "[错误] 账户已冻结！" << endl;
            return -1;
        }
        return m_balance;
    }

    // 存款
    bool deposit(double amount)
    {
        if (m_isFrozen)
        {
            cout << "[错误] 账户已冻结，无法存款！" << endl;
            return false;
        }
        if (amount <= 0)
        {
            cout << "[错误] 存款金额必须为正数！" << endl;
            return false;
        }
        m_balance += amount;
        cout << "[成功] 存款 " << amount << " 元，余额：" << m_balance << " 元" << endl;
        return true;
    }

    // 取款（需要密码）
    bool withdraw(double amount, const string &pwd)
    {
        if (!verifyPassword(pwd))
        {
            cout << "[错误] 密码不正确！" << endl;
            return false;
        }
        if (m_isFrozen)
        {
            cout << "[错误] 账户已冻结，无法取款！" << endl;
            return false;
        }
        if (amount <= 0)
        {
            cout << "[错误] 取款金额必须为正数！" << endl;
            return false;
        }
        if (amount > m_balance)
        {
            cout << "[错误] 余额不足！当前余额：" << m_balance << " 元" << endl;
            return false;
        }
        m_balance -= amount;
        cout << "[成功] 取款 " << amount << " 元，余额：" << m_balance << " 元" << endl;
        return true;
    }

    // 转账：展示更复杂的业务逻辑封装
    bool transfer(BankAccount &target, double amount, const string &pwd)
    {
        if (!verifyPassword(pwd))
        {
            cout << "[错误] 密码不正确！" << endl;
            return false;
        }
        if (amount <= 0 || amount > m_balance)
        {
            cout << "[错误] 转账金额不合法或余额不足！" << endl;
            return false;
        }
        // 先从自己扣钱，再给对方加钱
        m_balance -= amount;
        target.m_balance += amount;
        // 注意：成员函数可以访问同类对象的 private 成员
        cout << "[成功] 向 " << target.m_owner << " 转账 " << amount << " 元" << endl;
        return true;
    }

    // 展示公开信息（不暴露密码和余额）
    void showPublicInfo() const
    {
        cout << "户主：" << m_owner
             << "，账户状态：" << (m_isFrozen ? "冻结" : "正常") << endl;
    }
};

// ===========================================================================
// 第四部分：封装的设计原则
// ===========================================================================

// 原则1：成员变量全部 private
// 原则2：对外提供 public 接口
// 原则3：接口应该简洁，只暴露必要操作
// 原则4：在 setter 中进行数据验证
// 原则5：不需要修改的属性只提供 getter

// 不需要 setter 的场景示例
class Person
{
private:
    string m_name;          // 可通过 setter 修改
    const string m_bloodType; // 血型：出生就确定，不可修改，只有 getter

public:
    Person(const string &name, const string &blood)
        : m_name(name), m_bloodType(blood) {}

    // name：可读可写
    string getName() const { return m_name; }
    void setName(const string &name)
    {
        if (!name.empty()) m_name = name;
    }

    // bloodType：只读（无 setter）
    string getBloodType() const { return m_bloodType; }
};

// ===========================================================================
// 第五部分：setter 链式调用（返回引用）
// ===========================================================================
// 返回对象引用可以实现链式调用，提高代码可读性。

class Style
{
private:
    string m_color;
    int m_fontSize;
    bool m_bold;

public:
    Style() : m_color("black"), m_fontSize(12), m_bold(false) {}

    // setter 返回 *this 的引用，支持链式调用
    Style& setColor(const string &color)
    {
        m_color = color;
        return *this;   // 返回当前对象的引用
    }

    Style& setFontSize(int size)
    {
        if (size > 0) m_fontSize = size;
        return *this;
    }

    Style& setBold(bool bold)
    {
        m_bold = bold;
        return *this;
    }

    void show() const
    {
        cout << "颜色：" << m_color
             << "，字号：" << m_fontSize
             << "，粗体：" << (m_bold ? "是" : "否") << endl;
    }
};

// ===========================================================================
// 主函数
// ===========================================================================
int main()
{
    cout << "============================================" << endl;
    cout << "              C++ 封装详解" << endl;
    cout << "============================================" << endl;

    // ---- 第一部分：数据隐藏与 getter/setter ----
    cout << "\n===== 第一部分：数据隐藏与 getter/setter =====" << endl;
    Student stu("张三", 20, 85.5);
    stu.showInfo();

    // 通过 setter 修改数据（带验证）
    stu.setAge(21);
    stu.setScore(92.0);
    stu.showInfo();

    // 尝试设置非法数据
    stu.setAge(-5);         // 警告：年龄不合理
    stu.setScore(150);      // 警告：分数不合法
    stu.setName("");         // 警告：姓名为空

    // 通过 getter 获取数据
    cout << "通过 getter 获取：姓名=" << stu.getName()
         << "，年龄=" << stu.getAge()
         << "，分数=" << stu.getScore() << endl;

    // ---- 第二部分：只读属性 ----
    cout << "\n===== 第二部分：只读属性 =====" << endl;
    Circle c(5.0);
    cout << "半径：" << c.getRadius() << endl;
    cout << "面积：" << c.getArea() << endl;
    cout << "周长：" << c.getCircumference() << endl;

    c.setRadius(10);
    cout << "修改半径后面积：" << c.getArea() << endl;
    // c.setPi(3.14);  // 编译错误：没有 setPi 方法

    // ---- 第三部分：银行账户封装 ----
    cout << "\n===== 第三部分：银行账户封装 =====" << endl;
    BankAccount acc1("张三", 10000, "123456");
    BankAccount acc2("李四", 5000, "654321");

    acc1.showPublicInfo();
    acc2.showPublicInfo();

    // 正常操作
    acc1.deposit(2000);
    acc1.withdraw(3000, "123456");
    cout << "查询余额：" << acc1.getBalance("123456") << endl;

    // 转账
    acc1.transfer(acc2, 2000, "123456");
    cout << "张三余额：" << acc1.getBalance("123456") << endl;
    cout << "李四余额：" << acc2.getBalance("654321") << endl;

    // 错误操作（被封装逻辑拦截）
    cout << "\n--- 错误操作测试 ---" << endl;
    acc1.withdraw(99999, "123456");     // 余额不足
    acc1.withdraw(100, "wrong_pwd");    // 密码错误
    acc1.deposit(-100);                 // 金额非法

    // ---- 第四部分：链式调用 ----
    cout << "\n===== 第四部分：setter 链式调用 =====" << endl;
    Style style;
    // 链式调用：一行代码完成多个设置
    style.setColor("red").setFontSize(16).setBold(true);
    style.show();

    // 不使用链式调用（效果一样，但代码更长）
    Style style2;
    style2.setColor("blue");
    style2.setFontSize(20);
    style2.setBold(false);
    style2.show();

    // ---- 总结 ----
    cout << "\n===== 封装总结 =====" << endl;
    cout << "1. 封装 = 数据隐藏 + 接口暴露" << endl;
    cout << "2. 成员变量设为 private，通过 public getter/setter 访问" << endl;
    cout << "3. setter 中加入验证逻辑，保证数据合法性" << endl;
    cout << "4. 不需要修改的属性只提供 getter（只读属性）" << endl;
    cout << "5. 封装的好处：数据安全、接口清晰、易于维护和扩展" << endl;
    cout << "6. 可以在不改变接口的情况下修改内部实现" << endl;

    cout << endl;
    return 0;
}
