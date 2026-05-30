/*
 * ============================================================================
 * 主题: C++ 数据类型转换详解
 * ============================================================================
 * 数据类型转换是将一种数据类型转换为另一种数据类型的过程。
 * C++ 中有两种转换方式:
 *   - 隐式转换 (Implicit Conversion): 编译器自动完成
 *   - 显式转换 (Explicit Conversion): 程序员手动指定
 * 本文件还介绍了数据丢失和类型提升规则。
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
using namespace std;

int main() {

    // ==================== 隐式转换 (自动转换) ====================
    // 编译器在需要时自动进行类型转换, 不需要程序员干预
    // 规则: 小类型 -> 大类型 (安全, 不丢失数据)
    cout << "===== 隐式转换 (自动) =====" << endl;

    // 示例1: int -> double (安全转换)
    int intVal = 10;
    double doubleVal = intVal;   // int 自动转换为 double
    cout << "int " << intVal << " -> double " << doubleVal << endl;

    // 示例2: char -> int (自动提升)
    char ch = 'A';
    int asciiVal = ch;   // char 自动转换为 int (ASCII 值)
    cout << "char 'A' -> int " << asciiVal << endl;

    // 示例3: bool -> int
    bool flag = true;
    int boolToInt = flag;   // true -> 1
    cout << "bool true -> int " << boolToInt << endl;

    // 示例4: 混合运算中的隐式转换
    // 当不同类型的数据参与运算时, 小类型会自动提升为大类型
    int a = 5;
    double b = 2.0;
    double result = a / b;   // a 先转为 double, 然后进行除法
    cout << "int 5 / double 2.0 = " << result << endl;  // 2.5

    // 示例5: 赋值时的隐式转换
    double pi = 3.14159;
    int truncated = pi;   // double -> int, 小数部分被截断
    cout << "double " << pi << " -> int " << truncated << endl;  // 3

    // ==================== 类型提升规则 ====================
    cout << "\n===== 类型提升规则 =====" << endl;
    // 当不同类型的数据混合运算时, 编译器按以下规则提升:
    // 1. char, short -> int (至少提升到 int)
    // 2. int -> unsigned int (如果有 unsigned 参与)
    // 3. int -> long -> long long (逐步提升)
    // 4. int -> float -> double -> long double (整数转浮点)

    // 示例: char 和 int 运算
    char c1 = 100;
    char c2 = 50;
    int sum = c1 + c2;   // c1 和 c2 先提升为 int, 然后相加
    cout << "char 100 + char 50 = int " << sum << endl;  // 150, 不是截断后的 char

    // 示例: int 和 double 运算
    int i = 7;
    double d = 2.5;
    double mixed = i + d;   // i 提升为 double
    cout << "int 7 + double 2.5 = " << mixed << endl;  // 9.5

    // 示例: unsigned 和 signed 混合
    unsigned int u = 10;
    int s = -5;
    // 注意: signed 会被转换为 unsigned, 可能产生意外结果
    // unsigned int result2 = u + s;  // s 转为 unsigned: 很大的数
    // cout << "unsigned 10 + signed -5 = " << result2 << endl;

    // ==================== 显式转换 (强制类型转换) ====================
    cout << "\n===== 显式转换 (强制) =====" << endl;

    // 方式1: C 风格强制转换
    // 语法: (目标类型) 表达式
    double piVal = 3.14159;
    int intPi = (int)piVal;   // C 风格强制转换
    cout << "(int)3.14159 = " << intPi << endl;  // 3

    // 也可以写成: int(piVal)
    int intPi2 = int(piVal);
    cout << "int(3.14159) = " << intPi2 << endl;  // 3

    // 方式2: C++ 四种类型转换运算符 (推荐)
    // static_cast, dynamic_cast, const_cast, reinterpret_cast
    // 最常用的是 static_cast

    // static_cast<目标类型>(表达式)
    double piVal2 = 3.14159;
    int intPi3 = static_cast<int>(piVal2);
    cout << "static_cast<int>(3.14159) = " << intPi3 << endl;  // 3

    // int -> double
    int num = 42;
    double numD = static_cast<double>(num);
    cout << "static_cast<double>(42) = " << numD << endl;  // 42.0

    // char -> int
    char letter = 'Z';
    int letterInt = static_cast<int>(letter);
    cout << "static_cast<int>('Z') = " << letterInt << endl;  // 90

    // ==================== 数据丢失示例 ====================
    cout << "\n===== 数据丢失示例 =====" << endl;

    // 丢失1: double -> int (小数部分丢失)
    double price = 99.99;
    int intPrice = static_cast<int>(price);
    cout << "double " << price << " -> int " << intPrice << endl;  // 99, 丢失 .99

    // 丢失2: long long -> int (高位丢失)
    long long bigNum = 3000000000LL;  // 超过 int 范围
    int smallNum = static_cast<int>(bigNum);
    cout << "long long " << bigNum << " -> int " << smallNum << endl;  // 溢出!

    // 丢失3: double -> float (精度丢失)
    double precise = 3.141592653589793;
    float lessPrecise = static_cast<float>(precise);
    cout << fixed << setprecision(15);
    cout << "double: " << precise << endl;
    cout << "float:  " << lessPrecise << endl;
    cout << defaultfloat;

    // 丢失4: int -> short (范围溢出)
    int bigInt = 40000;
    short shortVal = static_cast<short>(bigInt);
    cout << "int " << bigInt << " -> short " << shortVal << endl;  // 溢出!

    // 丢失5: double -> bool
    double dTrue = 3.14;
    double dFalse = 0.0;
    cout << boolalpha;
    cout << "double 3.14 -> bool: " << static_cast<bool>(dTrue) << endl;   // true
    cout << "double 0.0 -> bool: " << static_cast<bool>(dFalse) << endl;   // false
    cout << noboolalpha;

    // ==================== 算术运算中的隐式转换陷阱 ====================
    cout << "\n===== 运算陷阱 =====" << endl;

    // 陷阱1: 整数除法
    int numerator = 7;
    int denominator = 2;
    int intDivResult = numerator / denominator;   // 整数除法, 结果是 3
    double doubleDivResult = numerator / denominator;  // 仍然是 3! 先整数除再转 double
    double correctResult = static_cast<double>(numerator) / denominator;  // 正确: 3.5
    cout << "7 / 2 (int) = " << intDivResult << endl;           // 3
    cout << "7 / 2 (转double) = " << doubleDivResult << endl;   // 3.0
    cout << "(double)7 / 2 = " << correctResult << endl;        // 3.5

    // 陷阱2: unsigned 和 signed 混合
    unsigned int big = 100;
    int small = -1;
    // 比较时 small 被转为 unsigned, 变成很大的数
    // 结果可能不符合预期
    if (big > static_cast<unsigned>(small)) {
        cout << "unsigned 100 > (unsigned)(-1)? 可能不符合预期" << endl;
    } else {
        cout << "比较结果取决于 unsigned 转换" << endl;
    }

    // 陷阱3: 赋值时的窄化转换
    // C++11 列表初始化 {} 会检查窄化转换, 更安全
    double value = 3.99;
    // int safe = {value};   // 编译错误! 列表初始化不允许窄化转换
    int unsafe = value;      // 允许, 但会丢失小数部分

    // ==================== C 风格 vs C++ 风格转换 ====================
    cout << "\n===== C 风格 vs C++ 风格 =====" << endl;

    // C 风格: (type)expression
    // 优点: 简洁
    // 缺点: 不够明确, 可能误用, 难以搜索

    // C++ 风格: static_cast<type>(expression)
    // 优点: 意图明确, 编译器检查更严格, 便于搜索
    // 缺点: 写法较长

    // C++ 还有其他三种转换:
    // dynamic_cast:   用于多态类型的向下转换 (运行时检查)
    // const_cast:     用于去除 const 属性
    // reinterpret_cast: 用于底层的位模式重新解释 (最危险)

    // 示例: const_cast
    const int constVal = 100;
    // constVal = 200;  // 错误: 不能修改 const
    int& ref = const_cast<int&>(constVal);  // 去除 const
    ref = 200;   // 技术上可以, 但修改 const 是未定义行为!
    cout << "const_cast 后的值: " << constVal << endl;  // 可能是 200, 但不可靠

    // ==================== 安全转换的建议 ====================
    cout << "\n===== 安全转换建议 =====" << endl;

    // 建议1: 优先使用 static_cast, 而不是 C 风格转换
    double piFinal = 3.14159;
    int piInt = static_cast<int>(piFinal);  // 推荐
    // int piInt = (int)piFinal;            // 不推荐

    // 建议2: 注意数据丢失, 必要时添加检查
    double source = 999.99;
    if (source >= 0 && source <= 100) {
        int safeInt = static_cast<int>(source);
        cout << "安全转换: " << safeInt << endl;
    } else {
        cout << "数值超出安全范围, 转换可能丢失数据" << endl;
    }

    // 建议3: 避免 signed/unsigned 混用
    // 如果需要非负数, 一开始就使用 unsigned

    // 建议4: 使用列表初始化 {} 来防止意外的窄化转换
    double preciseVal = 3.14;
    // int narrow = {preciseVal};  // 编译器会报错, 防止精度丢失

    // 建议5: 整数除法要小心, 至少一个操作数转为浮点型
    int x = 7, y = 2;
    double divResult = static_cast<double>(x) / y;  // 正确
    cout << "7.0 / 2 = " << divResult << endl;      // 3.5

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. 隐式转换: 编译器自动完成, 小类型->大类型
 * 2. 显式转换: (type)expr 或 static_cast<type>(expr)
 * 3. 类型提升: char/short->int->long->long long
 * 4. 数据丢失: double->int 丢失小数, 大范围->小范围溢出
 * 5. 整数除法: 两个 int 相除结果仍是 int
 * 6. 推荐使用 static_cast, 比 C 风格转换更安全
 * 7. 列表初始化 {} 可以防止意外的窄化转换
 * ===============================================
 */
