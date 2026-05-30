/*
 * ============================================================
 *  主题: 输入输出详解 (I/O Formatting)
 * ============================================================
 *  C++ 提供了丰富的输入输出功能。本文件涵盖:
 *    1. cout 格式化输出 (精度、宽度、填充)
 *    2. cin 输入及错误处理
 *    3. C 风格 printf/scanf
 *    4. stringstream 进行类型转换
 *    5. 文件输入输出简介
 * ============================================================
 */

#include <iostream>
#include <iomanip>       // 格式化输出头文件
#include <sstream>       // stringstream
#include <cstdio>        // printf, scanf
#include <string>
using namespace std;

int main()
{
    // ========================================
    // 1. cout 格式化输出
    // ========================================
    // <iomanip> 提供了多种格式化操控符 (manipulators)

    cout << "========== cout 格式化 ==========" << endl;

    // (a) 设置浮点数精度
    double pi = 3.14159265358979;
    cout << "默认精度 (6位): " << pi << endl;         // 3.14159
    cout << setprecision(3) << "精度3: " << pi << endl;    // 3.14
    cout << setprecision(10) << "精度10: " << pi << endl;  // 3.141592654

    // fixed + setprecision: 控制小数位数
    cout << fixed;
    cout << setprecision(2) << "固定2位小数: " << pi << endl;  // 3.14
    cout << setprecision(4) << "固定4位小数: " << pi << endl;  // 3.1416 (四舍五入)
    cout << defaultfloat;   // 恢复默认格式

    // (b) 设置输出宽度
    cout << "--- 宽度设置 ---" << endl;
    int num = 42;
    cout << "[" << setw(10) << num << "]" << endl;      // [        42]
    cout << "[" << setw(2) << num << "]" << endl;       // [42] (宽度不够时无效)

    // (c) 设置填充字符
    cout << "--- 填充字符 ---" << endl;
    cout << setfill('*');
    cout << "[" << setw(10) << num << "]" << endl;      // [********42]
    cout << setfill(' ');   // 恢复空格填充

    // (d) 对齐方式
    cout << "--- 对齐方式 ---" << endl;
    cout << left;   // 左对齐
    cout << "[" << setw(10) << num << "]" << endl;      // [42        ]

    cout << right;  // 右对齐 (默认)
    cout << "[" << setw(10) << num << "]" << endl;      // [        42]

    // (e) 进制输出
    cout << "--- 进制输出 ---" << endl;
    int value = 255;
    cout << "十进制:   " << dec << value << endl;       // 255
    cout << "八进制:   " << oct << value << endl;       // 377
    cout << "十六进制: " << hex << value << endl;       // ff
    cout << "十六进制(大写): " << uppercase << hex << value << endl;  // FF
    cout << nouppercase << dec;  // 恢复默认

    // (f) bool 值输出
    cout << "--- bool 输出 ---" << endl;
    bool flag = true;
    cout << "默认: " << flag << endl;              // 1
    cout << boolalpha << "文本: " << flag << endl; // true
    cout << noboolalpha;  // 恢复默认

    // ========================================
    // 2. cin 输入及错误处理
    // ========================================
    cout << endl << "========== cin 输入 ==========" << endl;

    // 基本输入
    // int inputVal;
    // cout << "请输入一个整数: ";
    // cin >> inputVal;
    // cout << "你输入了: " << inputVal << endl;

    // 多个值输入
    // int a, b;
    // cout << "请输入两个整数 (空格分隔): ";
    // cin >> a >> b;
    // cout << "a=" << a << ", b=" << b << endl;

    // cin 的错误处理
    cout << "--- cin 错误处理 ---" << endl;
    // 模拟输入错误的处理方式
    int testInput = 0;
    // cin >> testInput;
    // if (cin.fail())
    // {
    //     cout << "输入错误! 输入不是整数" << endl;
    //     cin.clear();              // 清除错误标志
    //     cin.ignore(1000, '\n');   // 忽略缓冲区中的无效输入
    // }

    // cin 读取字符串
    // 注意: cin >> 遇到空格会停止
    // getline(cin, str) 可以读取整行

    string name = "test";
    // cout << "请输入姓名: ";
    // cin >> name;          // 只读到空格前
    // cout << "你好, " << name << endl;

    // cout << "请输入完整姓名: ";
    // cin.ignore();          // 忽略之前的换行符
    // getline(cin, name);    // 读取整行
    // cout << "你好, " << name << endl;

    // ========================================
    // 3. C 风格 printf/scanf
    // ========================================
    // printf 和 scanf 是 C 语言的输入输出函数
    // 在 C++ 中仍然可以使用，需要 #include <cstdio>

    cout << endl << "========== printf/scanf ==========" << endl;

    // printf 格式化输出
    int intVal = 42;
    double dblVal = 3.14159;
    char ch = 'A';
    const char* str = "Hello";

    printf("整数: %d\n", intVal);              // %d: 整数
    printf("浮点数: %f\n", dblVal);            // %f: 浮点数 (默认6位小数)
    printf("浮点数(2位): %.2f\n", dblVal);     // %.2f: 2位小数
    printf("字符: %c\n", ch);                   // %c: 字符
    printf("字符串: %s\n", str);                // %s: 字符串
    printf("八进制: %o\n", intVal);             // %o: 八进制
    printf("十六进制: %x\n", intVal);           // %x: 十六进制
    printf("十六进制(大写): %X\n", intVal);     // %X: 十六进制大写

    // printf 宽度和对齐
    printf("右对齐: [%10d]\n", intVal);         // [        42]
    printf("左对齐: [%-10d]\n", intVal);        // [42        ]
    printf("前导零: [%010d]\n", intVal);        // [0000000042]

    // scanf 输入 (注释掉，避免阻塞)
    // int scanVal;
    // printf("请输入一个整数: ");
    // scanf("%d", &scanVal);    // 注意: 需要传地址 &
    // printf("你输入了: %d\n", scanVal);

    // ========================================
    // 4. stringstream 进行类型转换
    // ========================================
    // stringstream 可以在字符串和其他类型之间转换
    // 需要 #include <sstream>

    cout << endl << "========== stringstream ==========" << endl;

    // (a) 数字转字符串
    int number = 12345;
    double decimal = 3.14;
    string strNum, strDec;

    stringstream ss1;
    ss1 << number;          // 将 int 写入流
    ss1 >> strNum;          // 从流中读取 string
    cout << "int -> string: " << strNum << endl;

    // 更简洁的写法
    stringstream ss2;
    ss2 << decimal;
    ss2 >> strDec;
    cout << "double -> string: " << strDec << endl;

    // (b) 字符串转数字
    string s1 = "67890";
    string s2 = "2.718";
    int convInt;
    double convDouble;

    stringstream ss3(s1);
    ss3 >> convInt;
    cout << "string -> int: " << convInt << endl;

    stringstream ss4(s2);
    ss4 >> convDouble;
    cout << "string -> double: " << convDouble << endl;

    // (c) 使用 to_string 和 stoi/stod (C++11)
    cout << "--- C++11 简便方法 ---" << endl;
    string fromInt = to_string(999);           // int -> string
    string fromDouble = to_string(3.14159);    // double -> string
    cout << "to_string(999): " << fromInt << endl;
    cout << "to_string(3.14159): " << fromDouble << endl;

    int backToInt = stoi("12345");             // string -> int
    double backToDouble = stod("3.14");        // string -> double
    cout << "stoi(\"12345\"): " << backToInt << endl;
    cout << "stod(\"3.14\"): " << backToDouble << endl;

    // (d) 拼接字符串和数字
    cout << "--- 拼接示例 ---" << endl;
    stringstream ss5;
    ss5 << "学号: " << 2024001 << ", 姓名: 张三, GPA: " << fixed << setprecision(2) << 3.85;
    string info = ss5.str();
    cout << info << endl;

    // ========================================
    // 5. 输出格式对比
    // ========================================
    cout << endl << "========== 格式对比 ==========" << endl;

    double val = 1234.56789;

    cout << "--- cout 方式 ---" << endl;
    cout << "默认: " << val << endl;
    cout << fixed << setprecision(2) << "fixed(2): " << val << endl;
    cout << scientific << "scientific: " << val << endl;
    cout << defaultfloat;

    printf("--- printf 方式 ---\n");
    printf("默认: %f\n", val);
    printf("2位小数: %.2f\n", val);
    printf("科学计数法: %e\n", val);
    printf("宽度15,右对齐: [%15.2f]\n", val);
    printf("宽度15,左对齐:  [%-15.2f]\n", val);

    // ========================================
    // 6. 常见错误
    // ========================================
    // 错误 1: printf 格式不匹配
    // printf("%d", 3.14);      // 格式 %d 和 double 不匹配，输出垃圾值

    // 错误 2: scanf 忘记取地址
    // int x;
    // scanf("%d", x);          // 应该是 &x，不是 x

    // 错误 3: 混用 cin/cout 和 scanf/printf
    // 混用时可能出现缓冲区问题，建议统一使用一种

    // 错误 4: stringstream 重复使用前忘记清空
    stringstream ss;
    ss << "abc";
    string result1;
    ss >> result1;
    // ss << "xyz";   // 错误! 流处于 eof 状态
    ss.clear();        // 必须先清除状态标志
    ss.str("");        // 清空内容
    ss << "xyz";       // 现在可以了
    string result2;
    ss >> result2;
    cout << "result1=" << result1 << ", result2=" << result2 << endl;

    cout << endl << "=== 输入输出详解完毕 ===" << endl;
    return 0;
}
