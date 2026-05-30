/*
 * ============================================================================
 * 主题: C++ 字符型详解
 * ============================================================================
 * 字符型 (char) 用于存储单个字符, 占 1 字节 (8 位)。
 * 本文件介绍:
 *   - char 类型的基本使用
 *   - ASCII 编码表
 *   - char 与 int 的互相转换
 *   - 转义字符
 *   - 常见 ASCII 值
 * ============================================================================
 */

#include <iostream>
using namespace std;

int main() {

    // ==================== char 基本使用 ====================
    // char 类型占 1 字节 (8 位), 可以存储一个字符
    // 字符用单引号 '' 括起来 (注意不是双引号 "")
    cout << "===== char 基本使用 =====" << endl;

    char ch1 = 'A';      // 大写字母 A
    char ch2 = 'a';      // 小写字母 a
    char ch3 = '0';      // 数字字符 0
    char ch4 = ' ';      // 空格字符
    char ch5 = '!';      // 标点符号

    cout << "ch1 = " << ch1 << endl;
    cout << "ch2 = " << ch2 << endl;
    cout << "ch3 = " << ch3 << endl;
    cout << "ch4 = [" << ch4 << "]" << endl;   // 用方括号显示空格
    cout << "ch5 = " << ch5 << endl;
    cout << "char 占用字节: " << sizeof(char) << endl;

    // ==================== ASCII 编码 ====================
    // 计算机内部用数字存储字符, ASCII 是最基础的字符编码
    // ASCII 用 0-127 的数字表示字符
    // 例如: 'A' = 65, 'a' = 97, '0' = 48

    cout << "\n===== ASCII 编码 =====" << endl;
    cout << "'A' 的 ASCII 值: " << (int)'A' << endl;  // 65
    cout << "'a' 的 ASCII 值: " << (int)'a' << endl;  // 97
    cout << "'0' 的 ASCII 值: " << (int)'0' << endl;  // 48
    cout << "' ' 的 ASCII 值: " << (int)' ' << endl;  // 32

    // ==================== char 与 int 的转换 ====================
    cout << "\n===== char 与 int 互转 =====" << endl;

    // char -> int: 自动转换 (隐式转换)
    char letter = 'A';
    int asciiValue = letter;   // 自动将 'A' 转换为 65
    cout << "letter = " << letter << ", ASCII = " << asciiValue << endl;

    // int -> char: 自动转换
    int num = 66;
    char ch = num;   // 自动将 66 转换为 'B'
    cout << "num = " << num << ", 对应字符 = " << ch << endl;

    // 用数字初始化 char
    char ch6 = 65;    // 等价于 char ch6 = 'A';
    cout << "char(65) = " << ch6 << endl;

    // 用 char() 强制转换
    cout << "char(97) = " << char(97) << endl;    // 'a'
    cout << "int('Z') = " << int('Z') << endl;   // 90

    // ==================== 字符运算 ====================
    cout << "\n===== 字符运算 =====" << endl;
    // 字符可以参与算术运算, 实际上是用其 ASCII 值运算

    char upperA = 'A';
    char lowerA = upperA + 32;   // 65 + 32 = 97 = 'a'
    cout << "大写 'A' 转小写: " << lowerA << endl;

    char lowerZ = 'z';
    char upperZ = lowerZ - 32;   // 122 - 32 = 90 = 'Z'
    cout << "小写 'z' 转大写: " << upperZ << endl;

    // 大小写转换的通用公式:
    // 大写转小写: 大写字符 + 32
    // 小写转大写: 小写字符 - 32
    // 原因: 'A' = 65, 'a' = 97, 差值为 32

    // 字母表遍历
    cout << "\n大写字母表: ";
    for (char c = 'A'; c <= 'Z'; c++) {
        cout << c << " ";
    }
    cout << endl;

    cout << "小写字母表: ";
    for (char c = 'a'; c <= 'z'; c++) {
        cout << c << " ";
    }
    cout << endl;

    // ==================== 常见 ASCII 值 ====================
    cout << "\n===== 常见 ASCII 值 =====" << endl;

    // 控制字符 (0-31)
    cout << "NUL (空字符):  " << 0 << endl;
    cout << "TAB (制表符):  " << 9 << endl;
    cout << "LF  (换行符):  " << 10 << endl;
    cout << "CR  (回车符):  " << 13 << endl;

    // 可打印字符 (32-126)
    cout << "空格:          " << 32 << endl;
    cout << "'0'-'9':       " << (int)'0' << "-" << (int)'9' << endl;
    cout << "'A'-'Z':       " << (int)'A' << "-" << (int)'Z' << endl;
    cout << "'a'-'z':       " << (int)'a' << "-" << (int)'z' << endl;

    // 打印 ASCII 表 (部分)
    cout << "\nASCII 表 (可打印字符 33-126):" << endl;
    for (int i = 33; i <= 126; i++) {
        cout << (char)i << " ";
        if (i % 16 == 0) cout << endl;  // 每 16 个换行
    }
    cout << endl;

    // ==================== 转义字符 ====================
    cout << "\n===== 转义字符 =====" << endl;
    // 转义字符以反斜杠 \ 开头, 有特殊含义

    // \n: 换行符
    cout << "第一行\n第二行\n";

    // \t: 制表符 (Tab)
    cout << "姓名\t年龄\t城市\n";
    cout << "张三\t20\t北京\n";

    // \\: 反斜杠本身
    cout << "路径: C:\\Users\\Desktop\n";

    // \': 单引号
    cout << "It\'s a test\n";

    // \": 双引号
    cout << "他说: \"Hello!\"\n";

    // \0: 空字符 (字符串结束符)
    cout << "空字符的 ASCII 值: " << (int)'\0' << endl;

    // \r: 回车符 (光标回到行首)
    // 注意: 在不同系统上表现可能不同
    cout << "回车符测试: ABC\rX" << endl;  // 输出 XBC

    // \a: 响铃符 (可能发出提示音)
    // cout << '\a';  // 取消注释可能会发出声音

    // ==================== char 的数值范围 ====================
    cout << "\n===== char 的数值范围 =====" << endl;
    // char 的符号性取决于编译器实现
    // signed char: -128 ~ 127
    // unsigned char: 0 ~ 255

    cout << "char 最小值: " << (int)CHAR_MIN << endl;
    cout << "char 最大值: " << (int)CHAR_MAX << endl;

    // 显式指定 signed 或 unsigned
    signed char sc = -100;     // 有符号, 可以存负数
    unsigned char uc = 200;    // 无符号, 只能存 0-255
    cout << "signed char: " << (int)sc << endl;
    cout << "unsigned char: " << (int)uc << endl;

    // ==================== 常见错误 ====================
    cout << "\n===== 常见错误 =====" << endl;

    // 错误1: 用双引号初始化 char
    // char ch = "A";   // 错误: "A" 是字符串 (const char*), 不是字符
    char correct = 'A';   // 正确: 使用单引号

    // 错误2: 单引号中放多个字符
    // char ch = 'AB';  // 错误 (或警告): 多字符常量

    // 错误3: 混淆字符和数字
    cout << "'5' 的 ASCII 值: " << (int)'5' << endl;  // 53, 不是 5
    cout << "字符 '5' 转数字: " << ('5' - '0') << endl;  // 正确方法: 53 - 48 = 5

    // 错误4: 忘记 char 只能存一个字符
    // 如果需要存储多个字符, 应该使用字符串 (string 或 char[])

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. char 占 1 字节, 存储单个字符, 用单引号 ''
 * 2. ASCII: 'A'=65, 'a'=97, '0'=48
 * 3. char 与 int 可以互相自动转换
 * 4. 大小写转换: 差值为 32
 * 5. 转义字符: \n(换行), \t(制表), \\(反斜杠)等
 * 6. 字符运算实际是 ASCII 值的运算
 * 7. signed char: -128~127, unsigned char: 0~255
 * ===============================================
 */
