/*
 ============================================================================
 文件名 : 69_函数模板详解.cpp
 描  述 : 详细讲解C++函数模板的语法和使用技巧
          包括：template<typename T> 语法、类型推导规则、
          显式指定模板参数、多模板参数、模板特化（char*版本）、
          模板与普通函数的优先级、模板在头文件中定义
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 69_函数模板详解.cpp -o 69_函数模板详解
 ============================================================================
*/

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// ==================== 1. 函数模板基础语法 ====================
// template<typename T> 或 template<class T> 都可以
// typename 和 class 在此处完全等价
// T 是类型参数，代表任意类型

// 基本的函数模板：交换两个值
template<typename T>
void mySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// ==================== 2. 类型推导规则 ====================
// 编译器根据传入的实参自动推导模板参数的类型
// 推导规则：
//   规则1：根据实参类型推导 T 的类型
//   规则2：所有 T 必须推导为同一类型（不能冲突）
//   规则3：不会进行隐式类型转换

template<typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}

// ==================== 3. 显式指定模板参数 ====================
// 当编译器无法自动推导时，需要显式指定

// 接收两个不同类型的参数
template<typename T1, typename T2>
void printPair(T1 a, T2 b) {
    cout << "  值1: " << a << " (" << typeid(a).name() << ")" << endl;
    cout << "  值2: " << b << " (" << typeid(b).name() << ")" << endl;
}

// 返回类型无法从参数推导的情况，需要显式指定返回类型
template<typename RetType, typename T1, typename T2>
RetType add(T1 a, T2 b) {
    return static_cast<RetType>(a + b);
}

// ==================== 4. 多模板参数 ====================
template<typename T1, typename T2, typename T3>
T3 calculate(T1 a, T2 b) {
    return static_cast<T3>(a * b + a + b);
}

// 带有非类型模板参数（编译期常量）
template<typename T, int N>
void printArray(const T (&arr)[N]) {
    cout << "  数组大小: " << N << ", 元素: ";
    for (int i = 0; i < N; i++) {
        cout << arr[i];
        if (i < N - 1) cout << ", ";
    }
    cout << endl;
}

// ==================== 5. 模板特化（针对特定类型的特殊版本） ====================
// 通用版本：比较两个值是否相等
template<typename T>
bool isEqual(T a, T b) {
    cout << "[通用模板] ";
    return a == b;
}

// 模板特化：针对 const char* (C风格字符串) 的特殊版本
// 因为通用版本比较的是指针地址，而不是字符串内容
template<>
bool isEqual<const char*>(const char* a, const char* b) {
    cout << "[char*特化] ";
    return strcmp(a, b) == 0;
}

// 针对 double 的近似比较特化
template<>
bool isEqual<double>(double a, double b) {
    cout << "[double特化] ";
    const double epsilon = 1e-9;
    return (a - b < epsilon) && (b - a < epsilon);
}

// ==================== 6. 模板与普通函数的优先级 ====================
// 优先级规则：
//   1. 普通函数完全匹配 > 模板函数
//   2. 模板函数可以产生更好匹配 > 普通函数
//   3. 显式指定模板参数 > 隐式推导

// 普通函数版本
int myAdd(int a, int b) {
    cout << "[普通函数] ";
    return a + b;
}

// 函数模板版本
template<typename T>
T myAdd(T a, T b) {
    cout << "[模板函数] ";
    return a + b;
}

// ==================== 7. 模板的常见应用 ====================
// 打印任意类型数组（重载版本，接受指针和大小）
template<typename T>
void printArray(const T arr[], int size) {
    cout << "  [";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// 冒泡排序模板
template<typename T>
void bubbleSort(T arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                mySwap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ==================== 8. 关于模板在头文件中定义 ====================
// 模板的定义必须在编译器能看到的地方
// 通常将模板定义放在头文件中，而不是 .cpp 文件中
// 原因：模板是在使用时才实例化的，编译器需要看到完整定义
//
// 正确做法：// mytemplate.h
//   template<typename T>
//   T myFunc(T a) { return a * 2; }  // 定义放在头文件
//
// 错误做法：// mytemplate.h 中只有声明，// mytemplate.cpp 中放定义
//   链接时会报 undefined reference 错误
//
// 如果非要在 .cpp 中定义，需要显式实例化：
//   template void mySwap<int>(int&, int&);
//   template void mySwap<double>(double&, double&);

// ==================== 9. 自定义类用于演示 ====================
class Point {
public:
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // 重载 > 以支持 myMax 模板
    bool operator>(const Point& other) const {
        return (x * x + y * y) > (other.x * other.x + other.y * other.y);
    }

    // 重载 << 以支持输出
    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "(" << p.x << "," << p.y << ")";
        return os;
    }
};

// ==================== 10. 主函数 ====================
int main() {
    cout << "========================================" << endl;
    cout << "      函数模板详解 演示" << endl;
    cout << "========================================" << endl;

    // ---- 10.1 基本模板使用 ----
    cout << "\n--- 1. 基本函数模板 ---" << endl;
    int a = 10, b = 20;
    cout << "交换前: a=" << a << ", b=" << b << endl;
    mySwap(a, b);  // 编译器推导 T = int
    cout << "交换后: a=" << a << ", b=" << b << endl;

    double da = 1.5, db = 2.5;
    mySwap(da, db);  // T = double
    cout << "double交换: da=" << da << ", db=" << db << endl;

    string sa = "Hello", sb = "World";
    mySwap(sa, sb);  // T = string
    cout << "string交换: sa=" << sa << ", sb=" << sb << endl;

    // ---- 10.2 类型推导 ----
    cout << "\n--- 2. 类型推导 ---" << endl;
    cout << "myMax(3, 5) = " << myMax(3, 5) << endl;        // T = int
    cout << "myMax(3.14, 2.71) = " << myMax(3.14, 2.71) << endl;
    cout << "myMax('a', 'z') = " << myMax('a', 'z') << endl;

    // 类型推导冲突的解决
    // cout << myMax(3, 3.14) << endl;  // 错误! T 推导冲突
    cout << "myMax<double>(3, 3.14) = " << myMax<double>(3, 3.14) << endl;

    // ---- 10.3 显式指定模板参数 ----
    cout << "\n--- 3. 显式指定模板参数 ---" << endl;
    printPair(42, 3.14);  // 自动推导 T1=int, T2=double
    printPair<string, int>("年龄", 25);  // 显式指定

    auto result1 = add<double>(3, 4.5);
    cout << "add<double>(3, 4.5) = " << result1 << endl;
    auto result2 = add<int>(3.7, 4.5);  // 截断为 int
    cout << "add<int>(3.7, 4.5) = " << result2 << endl;

    // ---- 10.4 多模板参数 ----
    cout << "\n--- 4. 多模板参数 ---" << endl;
    auto r = calculate<int, double, double>(3, 4.5);
    cout << "calculate(3, 4.5) = " << r << endl;

    int arr1[] = {1, 2, 3, 4, 5};
    printArray(arr1);  // 非类型模板参数 N 自动推导为 5

    double arr2[] = {1.1, 2.2, 3.3};
    printArray(arr2);

    // ---- 10.5 模板特化 ----
    cout << "\n--- 5. 模板特化 ---" << endl;
    cout << "isEqual(10, 10): " << isEqual(10, 10) << endl;
    cout << "isEqual(10, 20): " << isEqual(10, 20) << endl;

    const char* s1 = "hello";
    const char* s2 = "hello";
    const char* s3 = "world";
    cout << "isEqual(\"hello\",\"hello\"): " << isEqual(s1, s2) << endl;
    cout << "isEqual(\"hello\",\"world\"): " << isEqual(s1, s3) << endl;

    cout << "isEqual(0.1+0.2, 0.3): " << isEqual(0.1 + 0.2, 0.3) << endl;

    // ---- 10.6 模板与普通函数优先级 ----
    cout << "\n--- 6. 模板与普通函数优先级 ---" << endl;
    int x = 1, y = 2;
    cout << "myAdd(1, 2): " << myAdd(x, y) << endl;
    // 普通函数完全匹配，优先调用

    cout << "myAdd(1.5, 2.5): " << myAdd(1.5, 2.5) << endl;
    // 模板函数更好匹配（不需要类型转换）

    cout << "myAdd<double>(1, 2): " << myAdd<double>(x, y) << endl;
    // 显式指定模板参数，强制调用模板版本

    // ---- 10.7 模板实际应用 ----
    cout << "\n--- 7. 模板的实际应用 ---" << endl;
    int intArr[] = {5, 3, 8, 1, 9, 2, 7};
    int intSize = sizeof(intArr) / sizeof(intArr[0]);
    cout << "排序前: ";
    printArray(intArr, intSize);

    bubbleSort(intArr, intSize);
    cout << "排序后: ";
    printArray(intArr, intSize);

    double dblArr[] = {3.14, 1.41, 2.71, 0.57};
    int dblSize = sizeof(dblArr) / sizeof(dblArr[0]);
    cout << "排序前: ";
    printArray(dblArr, dblSize);

    bubbleSort(dblArr, dblSize);
    cout << "排序后: ";
    printArray(dblArr, dblSize);

    // ---- 10.8 常见错误提醒 ----
    cout << "\n--- 8. 常见错误提醒 ---" << endl;
    cout << "  1. 模板推导不一致时需要显式指定类型" << endl;
    cout << "  2. char* 特化需要注意深拷贝问题" << endl;
    cout << "  3. 模板定义通常放在头文件中" << endl;
    cout << "  4. 模板特化必须在通用模板之后声明" << endl;
    cout << "  5. 模板中的操作必须对所有实例化类型都有效" << endl;

    cout << "\n========================================" << endl;
    cout << "   程序结束" << endl;
    cout << "========================================" << endl;

    return 0;
}
