/*
 * 文件名: 61_自增自减运算符重载.cpp
 * 描述: 自增(++)和自减(--)运算符的重载
 * 知识点:
 *   1. 前置++和后置++的区别与实现
 *   2. 前置--和后置--的区别与实现
 *   3. 返回引用(前置) vs 返回值(后置)
 *   4. int 参数是后置运算符的占位符
 *   5. 副作用的演示
 * 编译: g++ -std=c++11 61_自增自减运算符重载.cpp -o 61_自增自减运算符重载
 */

#include <iostream>
using namespace std;

// ============================================================
// 一、前置 ++ 和后置 ++ 的区别
// ============================================================
// 前置 ++ (++i): 先自增,再返回自增后的值
//   - 返回类型: 引用 (MyClass&)
//   - 无参数
//   - 效率更高(不需要创建临时对象)
//
// 后置 ++ (i++): 先返回当前值,再自增
//   - 返回类型: 值 (MyClass),不是引用
//   - 有一个 int 参数(占位符,编译器自动传入0)
//   - 效率较低(需要创建临时对象保存旧值)
//
// 语法区分:
//   前置: ClassName& operator++()        // 无参数
//   后置: ClassName operator++(int)      // int 参数是占位符
// ============================================================

// 模拟一个迭代器/计数器类
class Counter {
private:
    int count;
    string name;

public:
    // 构造函数
    Counter(string n = "counter", int c = 0) : name(n), count(c) {}

    // ---- 前置 ++ ----
    // 先将 count 加 1,然后返回修改后的自身引用
    // 返回引用可以避免创建临时对象,效率更高
    Counter& operator++() {
        ++count;
        cout << "  [前置++] " << name << " 自增为 " << count << endl;
        return *this;  // 返回自身引用
    }

    // ---- 后置 ++ ----
    // 先保存当前值到临时对象,再将 count 加 1,返回临时对象(旧值)
    // int 参数是占位符,仅用于区分前置和后置,不会被使用
    Counter operator++(int) {
        Counter temp = *this;  // 保存当前值(拷贝)
        ++count;               // 自增
        cout << "  [后置++] " << name << " 自增为 " << count
             << ", 返回旧值 " << temp.count << endl;
        return temp;           // 返回旧值(值返回)
    }

    // ---- 前置 -- ----
    Counter& operator--() {
        --count;
        cout << "  [前置--] " << name << " 自减为 " << count << endl;
        return *this;
    }

    // ---- 后置 -- ----
    Counter operator--(int) {
        Counter temp = *this;
        --count;
        cout << "  [后置--] " << name << " 自减为 " << count
             << ", 返回旧值 " << temp.count << endl;
        return temp;
    }

    // 获取当前值
    int getCount() const { return count; }

    // 重载输出运算符
    friend ostream& operator<<(ostream& os, const Counter& c);
};

ostream& operator<<(ostream& os, const Counter& c) {
    os << c.name << "[" << c.count << "]";
    return os;
}

// ============================================================
// 二、更完整的示例: 时间类
// ============================================================
// 演示 ++ 和 -- 在实际场景中的应用
// ============================================================

class Time {
private:
    int hour;    // 小时 (0-23)
    int minute;  // 分钟 (0-59)

    // 规范化时间(进位/借位处理)
    void normalize() {
        if (minute >= 60) {
            hour += minute / 60;
            minute %= 60;
        } else if (minute < 0) {
            int borrow = (-minute + 59) / 60;
            hour -= borrow;
            minute += borrow * 60;
        }
        if (hour >= 24) hour %= 24;
        while (hour < 0) hour += 24;
    }

public:
    Time(int h = 0, int m = 0) : hour(h), minute(m) {
        normalize();
    }

    // 前置 ++ : 增加1分钟
    Time& operator++() {
        ++minute;
        normalize();
        return *this;
    }

    // 后置 ++ : 增加1分钟,返回旧值
    Time operator++(int) {
        Time temp = *this;
        ++minute;
        normalize();
        return temp;
    }

    // 前置 -- : 减少1分钟
    Time& operator--() {
        --minute;
        normalize();
        return *this;
    }

    // 后置 -- : 减少1分钟,返回旧值
    Time operator--(int) {
        Time temp = *this;
        --minute;
        normalize();
        return temp;
    }

    friend ostream& operator<<(ostream& os, const Time& t);
};

ostream& operator<<(ostream& os, const Time& t) {
    if (t.hour < 10) os << "0";
    os << t.hour << ":";
    if (t.minute < 10) os << "0";
    os << t.minute;
    return os;
}

// ============================================================
// 三、常见错误和注意事项
// ============================================================
// 错误1: 后置++返回引用
//   Counter& operator++(int) { ... return temp; }  // 错误!
//   temp 是局部变量,返回引用会导致悬空引用
//
// 错误2: 前置++返回值
//   Counter operator++() { ... return *this; }  // 可以编译,但效率低
//   应该返回引用以避免不必要的拷贝
//
// 错误3: 忘记后置++的int参数
//   Counter operator++() { ... }   // 这是前置++
//   Counter operator++(int) { ... } // 这才是后置++
//
// 错误4: 前置和后置逻辑不一致
//   应确保前置和后置的自增/自减行为一致
// ============================================================

// 演示错误用法(仅作为注释说明)
/*
class BadCounter {
    int val;
public:
    // 错误: 后置++返回局部变量的引用
    BadCounter& operator++(int) {
        BadCounter temp = *this;
        val++;
        return temp;  // 危险! temp在函数结束后销毁
    }
};
*/

int main() {
    cout << "===== 自增自减运算符重载演示 =====" << endl;

    // --- 基本的前置和后置 ++/-- ---
    cout << "\n--- Counter 类演示 ---" << endl;
    Counter c("cnt", 10);
    cout << "初始值: " << c << endl;

    // 前置 ++: 先自增,返回新值
    cout << "\n执行 ++c:" << endl;
    Counter& ref = ++c;  // ref 是 c 的引用
    cout << "结果: " << c << endl;
    cout << "ref: " << ref << endl;
    cout << "c 和 ref 是同一个对象: " << (&c == &ref ? "是" : "否") << endl;

    // 后置 ++: 先返回旧值,再自增
    cout << "\n执行 c++:" << endl;
    Counter old = c++;  // old 拷贝了 c 的旧值
    cout << "c 的新值: " << c << endl;
    cout << "返回的旧值: " << old << endl;

    // 前置 --
    cout << "\n执行 --c:" << endl;
    --c;
    cout << "结果: " << c << endl;

    // 后置 --
    cout << "\n执行 c--:" << endl;
    Counter old2 = c--;
    cout << "c 的新值: " << c << endl;
    cout << "返回的旧值: " << old2 << endl;

    // ============================================================
    // 四、前置和后置的效率对比
    // ============================================================
    cout << "\n--- 效率对比 ---" << endl;
    cout << "前置 ++i 更高效:" << endl;
    cout << "  1. 不需要创建临时对象" << endl;
    cout << "  2. 返回引用(不拷贝)" << endl;
    cout << "后置 i++ 效率较低:" << endl;
    cout << "  1. 需要创建临时对象保存旧值" << endl;
    cout << "  2. 返回值(需要拷贝)" << endl;
    cout << "建议: 不需要旧值时,优先使用前置 ++i" << endl;

    // --- 时间类演示 ---
    cout << "\n--- Time 类演示 ---" << endl;
    Time t(23, 58);
    cout << "初始时间: " << t << endl;

    // 后置++: 返回旧值,时间前进1分钟
    cout << "\n执行 t++:" << endl;
    Time oldTime = t++;
    cout << "返回的旧时间: " << oldTime << endl;
    cout << "当前时间: " << t << endl;

    // 继续前置++: 跨越0点
    cout << "\n执行 ++t:" << endl;
    ++t;
    cout << "当前时间: " << t << endl;  // 00:00

    // 后置--: 回退1分钟
    cout << "\n执行 t--:" << endl;
    t--;
    cout << "当前时间: " << t << endl;  // 23:59

    // --- 副作用演示 ---
    cout << "\n--- 副作用演示 ---" << endl;
    Counter s("s", 0);
    cout << "s = " << s << endl;

    // 前置 ++ 的副作用: 表达式求值前就已经自增
    cout << "\n前置 ++s 作为表达式的一部分:" << endl;
    cout << "++s 的值: " << ++s << endl;       // s变为1,输出1
    cout << "s 的值: " << s << endl;            // 1

    // 后置 ++ 的副作用: 表达式求值后才自增
    cout << "\ns++ 作为表达式的一部分:" << endl;
    cout << "s++ 的值: " << s++ << endl;        // 返回旧值1,s变为2
    cout << "s 的值: " << s << endl;            // 2

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. 前置 ++: 无参数,返回引用(*this)" << endl;
    cout << "2. 后置 ++: int 参数(占位符),返回值(临时对象)" << endl;
    cout << "3. 前置 -- 和 后置 -- 的规则相同" << endl;
    cout << "4. 优先使用前置 ++i (效率更高)" << endl;
    cout << "5. 后置 ++ 返回的是自增前的值(有副作用)" << endl;
    cout << "6. 不要返回局部变量的引用(悬空引用)" << endl;

    return 0;
}
