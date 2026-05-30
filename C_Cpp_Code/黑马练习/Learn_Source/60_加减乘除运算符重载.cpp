/*
 * 文件名: 60_加减乘除运算符重载.cpp
 * 描述: 加减乘除算术运算符的重载实现
 * 知识点:
 *   1. operator+ operator- operator* operator/ 的重载
 *   2. 成员函数方式实现算术运算符
 *   3. 友元函数方式实现算术运算符
 *   4. 返回 *this 支持链式调用
 *   5. 重载 << 用于输出
 * 编译: g++ -std=c++11 60_加减乘除运算符重载.cpp -o 60_加减乘除运算符重载
 */

#include <iostream>
#include <cmath>
using namespace std;

// ============================================================
// 一、用成员函数方式重载加减乘除
// ============================================================
// 对于二元运算符,成员函数方式:
//   - 左操作数是 *this (当前对象)
//   - 右操作数是函数参数
//   - 返回运算结果(通常是新对象)
// 优点: 可以访问类的私有成员,代码简洁
// 缺点: 左操作数必须是类对象(不能是内置类型)
// ============================================================

class Fraction {
private:
    int numerator;    // 分子
    int denominator;  // 分母

    // 求最大公约数(辅助函数)
    int gcd(int a, int b) const {
        return b == 0 ? a : gcd(b, a % b);
    }

    // 化简分数(辅助函数)
    void simplify() {
        if (denominator < 0) {       // 保证分母为正
            numerator = -numerator;
            denominator = -denominator;
        }
        int g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;
    }

public:
    // 构造函数
    Fraction(int num = 0, int den = 1) : numerator(num), denominator(den) {
        if (denominator == 0) {
            cout << "错误: 分母不能为0,已设为1" << endl;
            denominator = 1;
        }
        simplify();
    }

    // ---- 成员函数方式重载 + ----
    // 语义: a/b + c/d = (a*d + b*c) / (b*d)
    Fraction operator+(const Fraction& other) const {
        int num = numerator * other.denominator + denominator * other.numerator;
        int den = denominator * other.denominator;
        return Fraction(num, den);  // 构造函数中会自动化简
    }

    // ---- 成员函数方式重载 - ----
    Fraction operator-(const Fraction& other) const {
        int num = numerator * other.denominator - denominator * other.numerator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    // ---- 成员函数方式重载 * ----
    // 语义: a/b * c/d = (a*c) / (b*d)
    Fraction operator*(const Fraction& other) const {
        int num = numerator * other.numerator;
        int den = denominator * other.denominator;
        return Fraction(num, den);
    }

    // ---- 成员函数方式重载 / ----
    // 语义: (a/b) / (c/d) = (a*d) / (b*c)
    Fraction operator/(const Fraction& other) const {
        int num = numerator * other.denominator;
        int den = denominator * other.numerator;
        return Fraction(num, den);
    }

    // ---- 重载 == 用于比较 ----
    bool operator==(const Fraction& other) const {
        return (numerator == other.numerator) && (denominator == other.denominator);
    }

    // ============================================================
    // 二、返回 *this 引用支持链式赋值
    // ============================================================
    // 对于复合赋值运算符(+=, -=, *=, /=),通常返回 *this 的引用
    // 这样可以支持链式操作: a += b += c;
    // ============================================================

    // 重载 += (返回 *this 引用,支持链式调用)
    Fraction& operator+=(const Fraction& other) {
        // 直接修改当前对象
        numerator = numerator * other.denominator + denominator * other.numerator;
        denominator = denominator * other.denominator;
        simplify();
        return *this;  // 返回自身引用
    }

    // 重载 -=
    Fraction& operator-=(const Fraction& other) {
        numerator = numerator * other.denominator - denominator * other.numerator;
        denominator = denominator * other.denominator;
        simplify();
        return *this;
    }

    // 重载 *=
    Fraction& operator*=(const Fraction& other) {
        numerator *= other.numerator;
        denominator *= other.denominator;
        simplify();
        return *this;
    }

    // 重载 /=
    Fraction& operator/=(const Fraction& other) {
        numerator *= other.denominator;
        denominator *= other.numerator;
        simplify();
        return *this;
    }

    // ============================================================
    // 三、友元函数方式重载 << 运算符
    // ============================================================
    // << 的左操作数是 ostream,不是 Fraction 对象
    // 所以必须用友元函数方式,不能用成员函数方式
    // 返回 ostream& 以支持链式输出: cout << a << b << endl;
    // ============================================================

    friend ostream& operator<<(ostream& os, const Fraction& f);

    // ============================================================
    // 四、友元函数方式重载算术运算符(当左操作数不是类对象时)
    // ============================================================
    // 例如: 2 * Fraction(1,3)  左操作数是 int,不是 Fraction
    // 此时成员函数方式无法处理,需要友元函数
    // ============================================================

    // 重载: int * Fraction (左操作数是内置类型)
    friend Fraction operator*(int num, const Fraction& f);
};

// 实现 << 运算符重载
ostream& operator<<(ostream& os, const Fraction& f) {
    if (f.denominator == 1) {
        os << f.numerator;          // 整数直接输出
    } else {
        os << f.numerator << "/" << f.denominator;
    }
    return os;
}

// 实现 int * Fraction
Fraction operator*(int num, const Fraction& f) {
    return Fraction(num * f.numerator, f.denominator);
}

// ============================================================
// 五、用友元函数方式重载所有算术运算符(完整示例)
// ============================================================
// 当需要支持 左操作数为非类类型 时,全部用友元函数
// ============================================================

class Point3D {
public:
    double x, y, z;

    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // 全部使用友元函数方式重载
    friend Point3D operator+(const Point3D& a, const Point3D& b);
    friend Point3D operator-(const Point3D& a, const Point3D& b);
    // 标量乘法: double * Point3D 和 Point3D * double
    friend Point3D operator*(double s, const Point3D& p);
    friend Point3D operator*(const Point3D& p, double s);
    // 点乘(内积): Point3D * Point3D
    friend double operator*(const Point3D& a, const Point3D& b);
    friend ostream& operator<<(ostream& os, const Point3D& p);
};

// 向量加法
Point3D operator+(const Point3D& a, const Point3D& b) {
    return Point3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

// 向量减法
Point3D operator-(const Point3D& a, const Point3D& b) {
    return Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

// 标量 * 向量 (标量在左)
Point3D operator*(double s, const Point3D& p) {
    return Point3D(s * p.x, s * p.y, s * p.z);
}

// 向量 * 标量 (标量在右)
Point3D operator*(const Point3D& p, double s) {
    return Point3D(p.x * s, p.y * s, p.z * s);
}

// 向量点乘(返回标量)
double operator*(const Point3D& a, const Point3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// 输出
ostream& operator<<(ostream& os, const Point3D& p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}

// ============================================================
// 六、常见错误
// ============================================================
// 错误1: 成员函数重载 << 时,用法变成 obj << cout (反直觉)
// 错误2: 复合赋值运算符忘记返回 *this 引用,导致无法链式调用
// 错误3: 除法运算时忘记检查除数是否为零
// 错误4: 返回临时对象的引用(悬空引用)
// ============================================================

int main() {
    cout << "===== 加减乘除运算符重载演示 =====" << endl;

    // --- 分数运算 ---
    cout << "\n--- 分数(Fraction)运算 ---" << endl;
    Fraction f1(1, 2);   // 1/2
    Fraction f2(1, 3);   // 1/3

    cout << f1 << " + " << f2 << " = " << (f1 + f2) << endl;  // 5/6
    cout << f1 << " - " << f2 << " = " << (f1 - f2) << endl;  // 1/6
    cout << f1 << " * " << f2 << " = " << (f1 * f2) << endl;  // 1/6
    cout << f1 << " / " << f2 << " = " << (f1 / f2) << endl;  // 3/2

    // 链式运算
    Fraction result = f1 + f2 + Fraction(1, 6);
    cout << f1 << " + " << f2 << " + 1/6 = " << result << endl;  // 1

    // 复合赋值链式调用
    Fraction f3(1, 4);
    Fraction f4(1, 4);
    (f3 += f4) += Fraction(1, 2);
    cout << "链式 += 结果: " << f3 << endl;  // 1

    // int * Fraction (友元函数方式)
    cout << "2 * " << f2 << " = " << (2 * f2) << endl;  // 2/3

    // --- 三维向量运算 ---
    cout << "\n--- 三维向量(Point3D)运算 ---" << endl;
    Point3D p1(1, 2, 3), p2(4, 5, 6);

    cout << p1 << " + " << p2 << " = " << (p1 + p2) << endl;    // (5, 7, 9)
    cout << p1 << " - " << p2 << " = " << (p1 - p2) << endl;    // (-3, -3, -3)
    cout << "3 * " << p1 << " = " << (3 * p1) << endl;           // (3, 6, 9)
    cout << p2 << " * 2 = " << (p2 * 2) << endl;                 // (8, 10, 12)
    cout << p1 << " . " << p2 << " = " << (p1 * p2) << endl;    // 32 (点乘)

    // --- 总结 ---
    cout << "\n===== 总结 =====" << endl;
    cout << "1. +, -, *, / 通常返回新对象(值返回)" << endl;
    cout << "2. +=, -=, *=, /= 返回 *this 引用(支持链式调用)" << endl;
    cout << "3. << 和 >> 必须用友元函数重载" << endl;
    cout << "4. 当左操作数不是类对象时,用友元函数" << endl;
    cout << "5. 注意除法运算时检查除数为零的情况" << endl;

    return 0;
}
