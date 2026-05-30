/*
 * ============================================================================
 * 主题: C++ 整型详解
 * ============================================================================
 * 整型 (Integer Types) 用于存储整数 (没有小数部分的数)。
 * C++ 提供了四种整型:
 *   - short (短整型):      通常 2 字节
 *   - int (整型):          通常 4 字节 (最常用)
 *   - long (长整型):       4 或 8 字节 (取决于平台)
 *   - long long (长长整型): 至少 8 字节
 * 每种都可以是 signed (有符号) 或 unsigned (无符号)。
 * ============================================================================
 */

#include <iostream>
#include <climits>  // 提供整型的极限值宏定义
using namespace std;

int main() {

    // ==================== short 短整型 ====================
    // short 通常占 2 字节 (16 位)
    // 有符号范围: -32768 ~ 32767
    // 无符号范围: 0 ~ 65535
    cout << "===== short 短整型 =====" << endl;
    short s1 = 32767;       // 最大值
    short s2 = -32768;      // 最小值
    short s3 = 0;
    cout << "short 最大值: " << s1 << endl;
    cout << "short 最小值: " << s2 << endl;
    cout << "short 占用字节: " << sizeof(short) << endl;

    // ==================== int 整型 (最常用) ====================
    // int 通常占 4 字节 (32 位)
    // 有符号范围: -2147483648 ~ 2147483647 (约正负21亿)
    // 无符号范围: 0 ~ 4294967295 (约42亿)
    cout << "\n===== int 整型 =====" << endl;
    int i1 = 2147483647;    // 最大值
    int i2 = -2147483648;   // 最小值
    int i3 = 0;
    int i4 = 42;
    cout << "int 最大值: " << i1 << endl;
    cout << "int 最小值: " << i2 << endl;
    cout << "int 占用字节: " << sizeof(int) << endl;

    // ==================== long 长整型 ====================
    // long 在 Windows 上通常 4 字节, 在 Linux/Mac 64位上通常 8 字节
    // C++ 标准只要求 long 至少 4 字节
    cout << "\n===== long 长整型 =====" << endl;
    long l1 = 2147483647L;    // L 后缀表示 long 类型
    long l2 = -2147483648L;
    cout << "long 值: " << l1 << endl;
    cout << "long 占用字节: " << sizeof(long) << endl;

    // ==================== long long 长长整型 ====================
    // long long 至少占 8 字节 (64 位)
    // 有符号范围: 约 -9.2 * 10^18 ~ 9.2 * 10^18
    // 用于存储非常大的整数
    cout << "\n===== long long 长长整型 =====" << endl;
    long long ll1 = 9223372036854775807LL;   // LL 后缀表示 long long
    long long ll2 = -9223372036854775807LL;
    cout << "long long 最大值: " << ll1 << endl;
    cout << "long long 最小值: " << ll2 << endl;
    cout << "long long 占用字节: " << sizeof(long long) << endl;

    // ==================== signed 与 unsigned ====================
    cout << "\n===== signed 与 unsigned =====" << endl;

    // signed (有符号): 可以存储正数、负数和零 (默认)
    signed int si = -100;     // 等价于 int si = -100;
    cout << "signed int: " << si << endl;

    // unsigned (无符号): 只能存储非负数 (0 和正数)
    // 范围比 signed 大一倍 (因为不需要存储符号位)
    unsigned int ui = 100;
    cout << "unsigned int: " << ui << endl;

    // unsigned 的范围
    unsigned int ui_max = 4294967295;  // unsigned int 的最大值 (2^32 - 1)
    cout << "unsigned int 最大值: " << ui_max << endl;

    // 注意: unsigned 不能存储负数
    unsigned int un = -1;   // 不会报错, 但值会变成一个很大的数 (4294967295)
    cout << "unsigned int = -1 的实际值: " << un << endl;

    // ==================== 使用 climits 查看极限值 ====================
    cout << "\n===== climits 中的极限值 =====" << endl;
    // <climits> 头文件定义了各种整型的极限值宏
    cout << "short   最小值: " << SHRT_MIN  << ", 最大值: " << SHRT_MAX  << endl;
    cout << "int     最小值: " << INT_MIN   << ", 最大值: " << INT_MAX   << endl;
    cout << "long    最小值: " << LONG_MIN  << ", 最大值: " << LONG_MAX  << endl;
    cout << "long long 最小值: " << LLONG_MIN << ", 最大值: " << LLONG_MAX << endl;
    cout << "unsigned int 最大值: " << UINT_MAX << endl;
    cout << "unsigned long 最大值: " << ULONG_MAX << endl;

    // ==================== 整数溢出 ====================
    cout << "\n===== 整数溢出示例 =====" << endl;
    // 溢出: 当数值超过类型的最大值时, 会"回绕"到最小值

    // int 溢出示例
    int maxInt = INT_MAX;     // 2147483647
    cout << "INT_MAX: " << maxInt << endl;
    cout << "INT_MAX + 1: " << maxInt + 1 << endl;  // 溢出! 变成 -2147483648

    // short 溢出示例
    short maxShort = 32767;
    cout << "short 最大值: " << maxShort << endl;
    cout << "short 最大值 + 1: " << (short)(maxShort + 1) << endl;  // 溢出! 变成 -32768

    // unsigned 溢出示例
    unsigned int maxUInt = UINT_MAX;  // 4294967295
    cout << "UINT_MAX: " << maxUInt << endl;
    cout << "UINT_MAX + 1: " << maxUInt + 1 << endl;  // 溢出! 变成 0

    // ==================== 整型字面量 ====================
    cout << "\n===== 整型字面量的进制 =====" << endl;

    // 十进制: 正常写法
    int dec = 42;
    cout << "十进制 42: " << dec << endl;

    // 八进制: 以 0 开头
    int oct = 052;   // 八进制的 52 = 十进制的 42
    cout << "八进制 052: " << oct << " (十进制)" << endl;

    // 十六进制: 以 0x 或 0X 开头
    int hex = 0x2A;  // 十六进制的 2A = 十进制的 42
    cout << "十六进制 0x2A: " << hex << " (十进制)" << endl;

    // 二进制 (C++14): 以 0b 或 0B 开头
    int bin = 0b101010;  // 二进制的 101010 = 十进制的 42
    cout << "二进制 0b101010: " << bin << " (十进制)" << endl;

    // 以不同进制输出
    cout << "\n以不同进制输出 42:" << endl;
    cout << "十进制: " << dec << 42 << endl;
    cout << "八进制: " << oct << 42 << endl;
    cout << "十六进制: " << hex << 42 << endl;

    // ==================== 整型后缀 ====================
    cout << "\n===== 整型后缀 =====" << endl;
    // U/u 后缀: unsigned
    // L/l 后缀: long
    // LL/ll 后缀: long long
    // 可以组合使用

    auto a1 = 42;         // int
    auto a2 = 42U;        // unsigned int
    auto a3 = 42L;        // long
    auto a4 = 42UL;       // unsigned long
    auto a5 = 42LL;       // long long
    auto a6 = 42ULL;      // unsigned long long
    cout << "42   -> sizeof: " << sizeof(a1) << endl;
    cout << "42U  -> sizeof: " << sizeof(a2) << endl;
    cout << "42L  -> sizeof: " << sizeof(a3) << endl;
    cout << "42UL -> sizeof: " << sizeof(a4) << endl;
    cout << "42LL -> sizeof: " << sizeof(a5) << endl;
    cout << "42ULL-> sizeof: " << sizeof(a6) << endl;

    // ==================== 如何选择整型 ====================
    cout << "\n===== 选择建议 =====" << endl;
    // 1. 一般整数运算: 使用 int (最常用, 效率通常最高)
    // 2. 非常大的整数: 使用 long long
    // 3. 节省内存: 使用 short (如嵌入式开发)
    // 4. 不需要负数: 使用 unsigned (如数组索引、大小)
    // 5. 避免使用 unsigned 存储可能为负的值

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. short (2字节), int (4字节), long (4/8字节), long long (8字节)
 * 2. signed 可存负数 (默认), unsigned 只存非负数
 * 3. <climits> 提供各类型的极限值宏
 * 4. 超过最大值会溢出 (回绕到最小值)
 * 5. 字面量后缀: U(无符号), L(long), LL(long long)
 * 6. 八进制以 0 开头, 十六进制以 0x 开头
 * 7. 一般情况下使用 int 即可
 * ===============================================
 */
