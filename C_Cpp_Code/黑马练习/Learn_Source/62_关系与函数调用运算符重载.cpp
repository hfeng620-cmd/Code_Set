/*
 * 文件名: 62_关系与函数调用运算符重载.cpp
 * 描述: 关系运算符和函数调用运算符的重载
 * 知识点:
 *   1. 关系运算符: ==, !=, <, >, <=, >=
 *   2. 函数调用运算符 operator() (仿函数/函子)
 *   3. 仿函数 vs 函数指针
 *   4. Lambda 表达式作为仿函数
 * 编译: g++ -std=c++11 62_关系与函数调用运算符重载.cpp -o 62_关系与函数调用运算符重载
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // sort, for_each
#include <functional> // function
using namespace std;

// ============================================================
// 一、关系运算符重载
// ============================================================
// 关系运算符: ==, !=, <, >, <=, >=
// 通常用于比较两个对象,返回 bool 值
// 一般作为成员函数重载
// 如果重载了 < ,通常也应重载 == 以保持一致性
// ============================================================

class Student {
public:
    string name;
    int score;
    int age;

    Student(string n, int s, int a) : name(n), score(s), age(a) {}

    // ---- 重载 == ----
    bool operator==(const Student& other) const {
        return (name == other.name) && (score == other.score);
    }

    // ---- 重载 != ----
    // C++20中,如果定义了 ==,编译器可以自动生成 !=
    // 在C++11中需要手动定义
    bool operator!=(const Student& other) const {
        return !(*this == other);
    }

    // ---- 重载 < ----
    // 按成绩排序(升序)
    bool operator<(const Student& other) const {
        return score < other.score;
    }

    // ---- 重载 > ----
    bool operator>(const Student& other) const {
        return score > other.score;
    }

    // ---- 重载 <= ----
    bool operator<=(const Student& other) const {
        return !(other < *this);
    }

    // ---- 重载 >= ----
    bool operator>=(const Student& other) const {
        return !(*this < other);
    }

    friend ostream& operator<<(ostream& os, const Student& s);
};

ostream& operator<<(ostream& os, const Student& s) {
    os << s.name << "(分数:" << s.score << ",年龄:" << s.age << ")";
    return os;
}

// ============================================================
// 二、函数调用运算符 operator() (仿函数/函子)
// ============================================================
// 仿函数(Functor): 重载了 operator() 的类的对象
// 仿函数可以像函数一样被调用,但它是对象
// 优点:
//   1. 可以保存状态(成员变量)
//   2. 比函数指针更灵活
//   3. 编译器可以内联优化
//   4. 可以用作模板参数
// ============================================================

// 仿函数1: 加法器
class Adder {
private:
    int base;

public:
    Adder(int b = 0) : base(b) {}

    // 重载 (), 接受一个参数
    int operator()(int x) const {
        return base + x;
    }

    // 重载 (), 接受两个参数(可以重载多个版本)
    int operator()(int x, int y) const {
        return base + x + y;
    }
};

// 仿函数2: 比较器(用于排序)
struct ScoreComparator {
    // 按成绩降序排列
    bool operator()(const Student& a, const Student& b) const {
        return a.score > b.score;
    }
};

// 仿函数3: 带状态的累加器
class Accumulator {
private:
    int total;
    int count;

public:
    Accumulator() : total(0), count(0) {}

    // 每次调用累加一个值
    void operator()(int value) {
        total += value;
        ++count;
    }

    int getTotal() const { return total; }
    int getCount() const { return count; }
    double getAverage() const {
        return count > 0 ? static_cast<double>(total) / count : 0.0;
    }
};

// 仿函数4: 打印器
struct Printer {
    string prefix;

    Printer(const string& p = "") : prefix(p) {}

    // 可以接受任意类型(模板成员函数在C++14后支持auto)
    template<typename T>
    void operator()(const T& item) const {
        cout << prefix << item << " ";
    }
};

// ============================================================
// 三、仿函数 vs 函数指针
// ============================================================
// 函数指针: 指向函数的指针,类型为 返回值(*指针名)(参数类型...)
// 仿函数:   重载了operator()的类的对象
//
// 对比:
// | 特性         | 函数指针       | 仿函数          |
// |-------------|---------------|----------------|
// | 保存状态     | 不能(全局状态) | 可以(成员变量)   |
// | 内联优化     | 较难           | 容易             |
// | 类型安全     | 弱             | 强               |
// | 可读性       | 一般           | 好               |
// | 模板参数     | 麻烦           | 方便             |
// ============================================================

// 函数指针示例
bool compareByAge(const Student& a, const Student& b) {
    return a.age < b.age;
}

// 仿函数示例(带状态)
struct AgeComparator {
    bool ascending;
    AgeComparator(bool asc = true) : ascending(asc) {}
    bool operator()(const Student& a, const Student& b) const {
        return ascending ? (a.age < b.age) : (a.age > b.age);
    }
};

// ============================================================
// 四、Lambda 表达式作为仿函数
// ============================================================
// Lambda 表达式本质上就是编译器自动生成的仿函数
// 语法: [捕获列表](参数列表) -> 返回类型 { 函数体 }
// 最简形式: []() {}
//
// 捕获列表:
//   []        不捕获任何变量
//   [x]       按值捕获变量 x
//   [&x]      按引用捕获变量 x
//   [=]       按值捕获所有局部变量
//   [&]       按引用捕获所有局部变量
//   [this]    按值捕获 this 指针
// ============================================================

// 使用 Lambda 作为比较器
void sortByAgeLambda(vector<Student>& students, bool ascending) {
    sort(students.begin(), students.end(),
         [ascending](const Student& a, const Student& b) -> bool {
             return ascending ? (a.age < b.age) : (a.age > b.age);
         });
}

// ============================================================
// 五、常见错误
// ============================================================
// 错误1: operator== 和 operator< 的语义不一致
//   如果 a == b 为 true,则 a < b 和 b < a 都应为 false
//
// 错误2: operator() 返回值不一致
//   同样的输入应产生同样的输出(纯函数)
//
// 错误3: Lambda 捕获了已销毁的变量
//   [&local_var] 但 local_var 所在函数已返回
//
// 错误4: 仿函数的 operator() 应该是 const
//   如果不修改状态,应标记为 const
// ============================================================

int main() {
    cout << "===== 关系与函数调用运算符重载演示 =====" << endl;

    // --- 关系运算符 ---
    cout << "\n--- 关系运算符 ---" << endl;
    Student s1("张三", 85, 20);
    Student s2("李四", 92, 21);
    Student s3("王五", 78, 19);
    Student s4("张三", 85, 20);  // 与 s1 相同

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;

    cout << "\n比较结果:" << endl;
    cout << s1 << " == " << s4 << " : " << (s1 == s4 ? "true" : "false") << endl;
    cout << s1 << " != " << s2 << " : " << (s1 != s2 ? "true" : "false") << endl;
    cout << s1 << " <  " << s2 << " : " << (s1 < s2 ? "true" : "false") << endl;
    cout << s1 << " >  " << s3 << " : " << (s1 > s3 ? "true" : "false") << endl;

    // 用关系运算符进行排序
    vector<Student> students = {s1, s2, s3};
    sort(students.begin(), students.end());  // 使用 operator<
    cout << "\n按成绩升序排序(使用 operator<):" << endl;
    for (const auto& s : students) {
        cout << "  " << s << endl;
    }

    // --- 仿函数 ---
    cout << "\n--- 仿函数(Functor) ---" << endl;

    // Adder 仿函数
    Adder add5(5);
    Adder add10(10);
    cout << "add5(3) = " << add5(3) << endl;      // 8
    cout << "add10(3) = " << add10(3) << endl;    // 13
    cout << "add5(1, 2) = " << add5(1, 2) << endl; // 8 (5+1+2)

    // 仿函数带状态: 累加器
    cout << "\n--- 累加器仿函数 ---" << endl;
    Accumulator acc;
    vector<int> nums = {10, 20, 30, 40, 50};
    for (int n : nums) {
        acc(n);  // 像函数一样调用
    }
    cout << "累加总和: " << acc.getTotal() << endl;
    cout << "累加次数: " << acc.getCount() << endl;
    cout << "平均值: " << acc.getAverage() << endl;

    // --- 仿函数 vs 函数指针 ---
    cout << "\n--- 仿函数 vs 函数指针 ---" << endl;

    // 使用函数指针排序(按年龄升序)
    vector<Student> students2 = {s1, s2, s3};
    sort(students2.begin(), students2.end(), compareByAge);
    cout << "函数指针排序(按年龄升序):" << endl;
    for (const auto& s : students2) {
        cout << "  " << s << endl;
    }

    // 使用仿函数排序(可控制升降序)
    AgeComparator descComp(false);  // 降序
    sort(students2.begin(), students2.end(), descComp);
    cout << "仿函数排序(按年龄降序):" << endl;
    for (const auto& s : students2) {
        cout << "  " << s << endl;
    }

    // --- Lambda 表达式 ---
    cout << "\n--- Lambda 表达式 ---" << endl;

    // 基本 Lambda
    auto square = [](int x) -> int { return x * x; };
    cout << "square(5) = " << square(5) << endl;  // 25

    // 捕获外部变量的 Lambda
    int threshold = 80;
    auto isPass = [threshold](const Student& s) -> bool {
        return s.score >= threshold;
    };
    cout << "\n及格线: " << threshold << "分" << endl;
    cout << s1 << " 是否及格: " << (isPass(s1) ? "是" : "否") << endl;
    cout << s3 << " 是否及格: " << (isPass(s3) ? "是" : "否") << endl;

    // Lambda 用于 for_each
    cout << "\n使用 Lambda 打印所有学生:" << endl;
    for_each(students.begin(), students.end(),
             [](const Student& s) { cout << "  " << s << endl; });

    // Lambda 作为排序器
    sortByAgeLambda(students, true);
    cout << "Lambda 排序(按年龄升序):" << endl;
    for (const auto& s : students) {
        cout << "  " << s << endl;
    }

    // --- Lambda 与仿函数的等价关系 ---
    cout << "\n--- Lambda 本质是仿函数 ---" << endl;
    cout << "auto fn = [](int x){ return x*2; };" << endl;
    cout << "等价于:" << endl;
    cout << "class __Lambda {" << endl;
    cout << "public:" << endl;
    cout << "    int operator()(int x) const { return x*2; }" << endl;
    cout << "};" << endl;
    cout << "__Lambda fn;" << endl;

    // --- 使用 std::function 包装 ---
    cout << "\n--- std::function 包装 ---" << endl;
    function<int(int)> func;

    func = square;                // 包装 Lambda
    cout << "Lambda: " << func(6) << endl;

    func = Adder(100);            // 包装仿函数(会调用 operator()(int))
    cout << "Functor: " << func(6) << endl;

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. 关系运算符返回 bool,保持语义一致性" << endl;
    cout << "2. operator() 使对象像函数一样调用(仿函数)" << endl;
    cout << "3. 仿函数比函数指针更灵活(可保存状态)" << endl;
    cout << "4. Lambda 是编译器自动生成的仿函数" << endl;
    cout << "5. std::function 可以统一包装各种可调用对象" << endl;

    return 0;
}
