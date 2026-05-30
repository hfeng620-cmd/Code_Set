/*
 * ============================================================================
 * 主题: C++ 浮点型详解 (详解版)
 * ============================================================================
 * 浮点型 (Floating-Point Types) 用于存储带有小数部分的数。
 * C++ 提供了三种浮点型:
 *   - float (单精度):        通常 4 字节, 约 6-7 位有效数字
 *   - double (双精度):       通常 8 字节, 约 15-16 位有效数字 (最常用)
 *   - long double (扩展精度): 通常 8-16 字节, 精度取决于平台
 *
 * 本文件深入介绍:
 *   - IEEE 754 浮点数表示法 (符号位、指数、尾数)
 *   - 为什么 0.1 + 0.2 != 0.3 (二进制详解)
 *   - 浮点数比较技术 (epsilon 比较)
 *   - 特殊值: NaN, 无穷大, 非规格化数
 *   - float vs double 精度对比
 *   - 科学计数法详解
 *   - 浮点型的选择建议
 *
 * 学习建议:
 *   1. 理解浮点数是"近似"存储, 不是精确存储
 *   2. 永远不要用 == 直接比较浮点数
 *   3. 一般计算推荐使用 double
 * ============================================================================
 */

#include <iostream>
#include <iomanip>    // 用于 setprecision, setw, fixed 等格式控制
#include <cfloat>     // 提供浮点型极限值宏定义 (FLT_MAX, DBL_EPSILON 等)
#include <cmath>      // 提供 isnan, isinf, fabs 等数学函数
#include <limits>     // 提供 numeric_limits 模板
#include <bitset>     // 用于二进制输出演示
using namespace std;

int main() {

    // =========================================================
    // 一、float 单精度浮点型
    // =========================================================
    cout << "===== 一、float 单精度浮点型 =====" << endl;

    // float 占 4 字节 (32 位)
    // 精度: 约 6-7 位有效数字
    // 存储范围: 约 1.2e-38 ~ 3.4e+38
    // 字面量需要加 f 后缀, 否则编译器默认为 double
    float f1 = 3.14f;         // f 后缀表示 float 类型
    float f2 = 1.23e4f;       // 科学计数法: 1.23 * 10^4 = 12300
    float f3 = -0.0056f;
    float f4 = 1.0f;
    float f5 = .5f;           // 省略前导 0: 0.5
    float f6 = 2.f;           // 省略小数部分: 2.0

    cout << "f1 = " << f1 << endl;
    cout << "f2 = " << f2 << endl;
    cout << "f3 = " << f3 << endl;
    cout << "float 占用字节: " << sizeof(float) << endl;
    cout << "float 有效数字约 6-7 位" << endl;
    cout << "float 最大值: " << FLT_MAX << endl;
    cout << "float 最小正值: " << FLT_MIN << endl;
    cout << "float 精度 (epsilon): " << FLT_EPSILON << endl;

    // =========================================================
    // 二、double 双精度浮点型
    // =========================================================
    cout << "\n===== 二、double 双精度浮点型 =====" << endl;

    // double 占 8 字节 (64 位), 是最常用的浮点型
    // 精度: 约 15-16 位有效数字
    // 存储范围: 约 2.2e-308 ~ 1.8e+308
    // 编译器默认的小数字面量就是 double 类型
    double d1 = 3.141592653589793;
    double d2 = 123456.789;
    double d3 = -0.000123;
    double d4 = 1.0;          // 1.0 是 double, 不是 int
    double d5 = 1e10;         // 1 * 10^10

    cout << "d1 = " << d1 << endl;
    cout << "d2 = " << d2 << endl;
    cout << "d3 = " << d3 << endl;
    cout << "double 占用字节: " << sizeof(double) << endl;
    cout << "double 最大值: " << DBL_MAX << endl;
    cout << "double 最小正值: " << DBL_MIN << endl;
    cout << "double 精度 (epsilon): " << DBL_EPSILON << endl;

    // =========================================================
    // 三、long double 扩展精度
    // =========================================================
    cout << "\n===== 三、long double 扩展精度 =====" << endl;

    // long double 至少 8 字节, 可能是 12 或 16 字节 (取决于编译器和平台)
    // MSVC: 8 字节 (与 double 相同)
    // GCC/Linux x86: 12 或 16 字节 (80 位扩展精度)
    // 精度更高, 但不一定比 double 更精确 (取决于平台)
    // 字面量加 L 后缀
    long double ld1 = 3.141592653589793238L;
    cout << "ld1 = " << ld1 << endl;
    cout << "long double 占用字节: " << sizeof(long double) << endl;

    // =========================================================
    // 四、IEEE 754 浮点数表示法 (重点!)
    // =========================================================
    cout << "\n===== 四、IEEE 754 浮点数表示法 =====" << endl;
    // 计算机用二进制存储浮点数, 遵循 IEEE 754 标准
    //
    // float (32 位) 的结构:
    //   ┌─────┬──────────┬───────────────────────────┐
    //   │ 符号 │  指数     │        尾数 (小数部分)      │
    //   │ 1位  │  8位     │         23位              │
    //   └─────┴──────────┴───────────────────────────┘
    //   位:  31    30~23           22~0
    //
    // double (64 位) 的结构:
    //   ┌─────┬──────────┬───────────────────────────────────────────┐
    //   │ �符号 │  指数     │              尾数 (小数部分)               │
    //   │ 1位  │  11位    │                 52位                      │
    //   └─────┴──────────┴───────────────────────────────────────────┘
    //   位:  63    62~52                    51~0
    //
    // 计算公式: 值 = (-1)^符号位 * 1.尾数 * 2^(指数-偏移量)
    //   偏移量: float = 127, double = 1023
    //
    // 示例: 将 6.75 转换为 float 的二进制表示
    //   6.75 = 110.11 (二进制)
    //        = 1.1011 * 2^2 (规范化)
    //   符号位 = 0 (正数)
    //   指数 = 2 + 127 = 129 = 10000001 (二进制)
    //   尾数 = 10110000000000000000000 (去掉隐含的 1)
    //   结果: 0 10000001 10110000000000000000000

    // 用代码验证 6.75 的 float 存储
    float testVal = 6.75f;
    cout << "float 6.75 的 IEEE 754 表示:" << endl;

    // 将 float 的二进制模式重新解释为 unsigned int 来查看位模式
    unsigned int bits;
    memcpy(&bits, &testVal, sizeof(bits));
    cout << "二进制: ";
    for (int i = 31; i >= 0; i--) {
        cout << ((bits >> i) & 1);
        if (i == 31 || i == 23) cout << " ";  // 分隔符
    }
    cout << endl;
    cout << "符号位: " << ((bits >> 31) & 1) << " (0=正)" << endl;
    cout << "指数:   " << ((bits >> 23) & 0xFF) << " (实际指数: " << ((int)((bits >> 23) & 0xFF) - 127) << ")" << endl;
    cout << "尾数:   " << (bits & 0x7FFFFF) << endl;

    // =========================================================
    // 五、为什么 0.1 + 0.2 != 0.3 (经典问题!)
    // =========================================================
    cout << "\n===== 五、0.1 + 0.2 != 0.3 详解 =====" << endl;
    // 计算机用二进制存储浮点数, 某些十进制小数无法精确表示
    // 就像 1/3 无法用有限位十进制小数表示一样
    //
    // 0.1 的二进制表示 (无限循环):
    //   0.1 (十进制) = 0.00011001100110011... (二进制)
    //                  ↑ 无限循环 0011
    //
    // 0.2 的二进制表示 (无限循环):
    //   0.2 (十进制) = 0.0011001100110011... (二进制)
    //                  ↑ 无限循环 0011
    //
    // 由于 float/double 只有有限的位数 (23/52 位尾数),
    // 这些无限循环小数必须被截断, 导致精度丢失
    //
    // 存储过程 (以 double 为例):
    //   0.1 -> 存储为 0.1000000000000000055511151231257827021181583404541015625
    //   0.2 -> 存储为 0.200000000000000011102230246251565404236316680908203125
    //   0.1 + 0.2 -> 0.3000000000000000444089209850062616169452667236328125
    //   而 0.3 -> 存储为 0.299999999999999988897769753748434595763683319091796875

    double a = 0.1;
    double b = 0.2;
    double c = a + b;

    cout << fixed << setprecision(20);  // 显示 20 位小数来观察精度差异
    cout << "0.1      = " << a << endl;
    cout << "0.2      = " << b << endl;
    cout << "0.1 + 0.2 = " << c << endl;
    cout << "0.3      = " << 0.3 << endl;
    cout << "差值      = " << (c - 0.3) << endl;
    cout << defaultfloat;  // 恢复默认输出格式

    cout << "\n0.1 + 0.2 == 0.3 ? " << (c == 0.3 ? "true" : "false") << endl;
    // 输出 false! 因为 0.1 + 0.2 的实际结果是 0.30000000000000004

    // =========================================================
    // 六、浮点数比较技术 (epsilon 比较)
    // =========================================================
    cout << "\n===== 六、浮点数比较技术 =====" << endl;

    // 方法1: 绝对误差比较 (适用于值的范围已知且较小)
    // 检查两个数的差的绝对值是否小于一个很小的阈值
    double epsilon = 1e-9;  // 误差范围
    bool isEqual1 = fabs(c - 0.3) < epsilon;  // fabs 是浮点数的绝对值
    cout << "绝对误差比较 (epsilon=" << epsilon << "): "
         << (isEqual1 ? "相等" : "不相等") << endl;

    // 方法2: 相对误差比较 (适用于值的范围不确定)
    // 检查差值相对于较大值的比例是否小于阈值
    double x = 1000000.0;
    double y = 1000000.0000001;
    double relEpsilon = 1e-9;
    bool isEqual2 = fabs(x - y) / fmax(fabs(x), fabs(y)) < relEpsilon;
    cout << "相对误差比较: " << (isEqual2 ? "相等" : "不相等") << endl;

    // 方法3: 使用 ULP (Units in the Last Place) 比较
    // 这是最精确的浮点数比较方法, 但实现较复杂
    // ULP 表示两个相邻浮点数之间的距离
    // 基本思想: 检查两个数之间有多少个 ULP

    // 方法4: 使用 std::nextafter (C++11)
    // 可以获取给定浮点数在某个方向上的下一个可表示值
    double val = 1.0;
    double nextVal = nextafter(val, 2.0);  // 1.0 向 2.0 方向的下一个值
    cout << "1.0 的下一个 double: " << setprecision(20) << nextVal << endl;
    cout << defaultfloat;
    cout << "ULP 大小: " << (nextVal - val) << endl;

    // 实用建议:
    //   - 一般情况: fabs(a - b) < 1e-9 即可
    //   - 高精度需求: 使用 ULP 比较或专业的数学库
    //   - 永远不要用 == 直接比较浮点数!

    // =========================================================
    // 七、特殊浮点值: NaN, 无穷大, 非规格化数
    // =========================================================
    cout << "\n===== 七、特殊浮点值 =====" << endl;

    // --- 正无穷和负无穷 ---
    // 产生原因: 正数除以 0, 或运算结果超出最大可表示值
    double posInf = 1.0 / 0.0;    // 正无穷 (不会报错!)
    double negInf = -1.0 / 0.0;   // 负无穷
    double bigInf = 1e308 * 10.0; // 溢出为无穷大

    cout << "正无穷: " << posInf << endl;
    cout << "负无穷: " << negInf << endl;
    cout << "溢出:   " << bigInf << endl;

    // 无穷大的性质:
    cout << "Inf + 1 = " << (posInf + 1) << endl;      // 仍然是 Inf
    cout << "Inf * 2 = " << (posInf * 2) << endl;      // 仍然是 Inf
    cout << "Inf > 任何有限数? " << (posInf > 1e308 ? "true" : "false") << endl;
    cout << "-Inf < 任何有限数? " << (negInf < -1e308 ? "true" : "false") << endl;
    cout << "Inf - Inf = " << (posInf - posInf) << endl;  // NaN!
    cout << "Inf / Inf = " << (posInf / posInf) << endl;  // NaN!

    // 使用 isinf() 判断是否为无穷大 (需要 #include <cmath>)
    cout << "isinf(1.0/0.0) = " << (isinf(posInf) ? "true" : "false") << endl;

    // --- NaN (Not a Number, 非数字) ---
    // 产生原因: 0/0, Inf-Inf, Inf/Inf, sqrt(-1) 等无效运算
    double nanVal = 0.0 / 0.0;       // NaN
    double nanVal2 = sqrt(-1.0);     // NaN (负数的平方根)
    double nanVal3 = posInf - posInf; // NaN (无穷减无穷)

    cout << "\nNaN: " << nanVal << endl;

    // NaN 的特殊性质: NaN 不等于任何值, 包括它自己!
    cout << "NaN == NaN ? " << (nanVal == nanVal ? "true" : "false") << endl;  // false!
    cout << "NaN != NaN ? " << (nanVal != nanVal ? "true" : "false") << endl;  // true!

    // NaN 具有"传染性": 任何涉及 NaN 的运算结果都是 NaN
    cout << "NaN + 1 = " << (nanVal + 1) << endl;      // NaN
    cout << "NaN * 0 = " << (nanVal * 0) << endl;      // NaN
    cout << "NaN == 0 ? " << (nanVal == 0 ? "true" : "false") << endl;  // false

    // 使用 isnan() 判断是否为 NaN (需要 #include <cmath>)
    // 正确的 NaN 判断方法:
    cout << "isnan(NaN) = " << (isnan(nanVal) ? "true" : "false") << endl;

    // 错误的 NaN 判断方法 (不要这样做):
    // if (nanVal == nanVal) -- 永远为 false, 无法检测 NaN
    // if (nanVal != nanVal) -- 永远为 true, 这可以检测 NaN (但不推荐)

    // --- 非规格化数 (Denormalized Numbers / Subnormal Numbers) ---
    // 当数值非常接近 0, 小于最小规格化数时, 使用非规格化表示
    // 非规格化数的精度低于规格化数, 但可以表示更接近 0 的值
    double denormMin = DBL_MIN;  // 最小规格化正数
    double tinyNum = 1e-320;     // 非规格化数 (小于 DBL_MIN)
    cout << "\n最小规格化正数 (DBL_MIN): " << denormMin << endl;
    cout << "非规格化数 (1e-320): " << tinyNum << endl;

    // =========================================================
    // 八、float vs double 精度对比
    // =========================================================
    cout << "\n===== 八、float vs double 精度对比 =====" << endl;

    // 精度差异演示
    float  floatPi  = 3.14159265358979f;
    double doublePi = 3.14159265358979;

    cout << fixed << setprecision(15);
    cout << "float  存储的 pi: " << floatPi << endl;
    cout << "double 存储的 pi: " << doublePi << endl;
    cout << "实际 pi 的值:      3.141592653589793" << endl;
    cout << defaultfloat;

    // 累积误差演示
    cout << "\n累积误差演示 (1000000 次加法):" << endl;
    float  floatSum  = 0.0f;
    double doubleSum = 0.0;
    int iterations = 1000000;

    for (int i = 0; i < iterations; i++) {
        floatSum  += 0.000001f;
        doubleSum += 0.000001;
    }

    cout << fixed << setprecision(10);
    cout << "float  累积结果: " << floatSum << " (理论值: 1.0)" << endl;
    cout << "double 累积结果: " << doubleSum << " (理论值: 1.0)" << endl;
    cout << "float  误差: " << fabs(floatSum - 1.0) << endl;
    cout << "double 误差: " << fabs(doubleSum - 1.0) << endl;
    cout << defaultfloat;

    // 精度位数对比
    cout << "\n精度位数对比:" << endl;
    float  f = 1.23456789012345f;
    double d = 1.23456789012345;
    cout << fixed << setprecision(20);
    cout << "float:  " << f << endl;
    cout << "double: " << d << endl;
    cout << defaultfloat;
    cout << "float 有效位数约 6-7 位, 之后的数字不可靠" << endl;
    cout << "double 有效位数约 15-16 位" << endl;

    // =========================================================
    // 九、科学计数法详解
    // =========================================================
    cout << "\n===== 九、科学计数法详解 =====" << endl;
    // 格式: 数值e指数 或 数值E指数
    // 表示 数值 * 10^指数
    //
    // 规则:
    //   - e/E 前面必须有数字 (不能是 .5e2, 但可以是 0.5e2)
    //   - e/E 后面必须是整数 (不能是 e1.5)
    //   - 正号可以省略 (e2 和 e+2 相同)

    double sci1 = 3.14e2;      // 3.14 * 10^2 = 314
    double sci2 = 3.14E-2;     // 3.14 * 10^-2 = 0.0314
    double sci3 = 1.5e10;      // 1.5 * 10^10 = 15000000000
    double sci4 = 2.5e-3;      // 2.5 * 10^-3 = 0.0025
    double sci5 = 1e6;         // 1 * 10^6 = 1000000
    double sci6 = 5.0e+3;      // 5.0 * 10^3 = 5000 (正号可省略)

    cout << "3.14e2  = " << sci1 << endl;
    cout << "3.14E-2 = " << sci2 << endl;
    cout << "1.5e10  = " << sci3 << endl;
    cout << "2.5e-3  = " << sci4 << endl;
    cout << "1e6     = " << sci5 << endl;
    cout << "5.0e+3  = " << sci6 << endl;

    // 科学计数法的实际应用
    double speedOfLight = 3.0e8;       // 光速: 约 3 * 10^8 米/秒
    double planckConst  = 6.626e-34;   // 普朗克常数: 6.626 * 10^-34 J·s
    double avogadroNum  = 6.022e23;    // 阿伏伽德罗常数: 6.022 * 10^23
    double earthMass    = 5.972e24;    // 地球质量: 5.972 * 10^24 kg

    cout << "\n科学常数:" << endl;
    cout << "光速:           " << speedOfLight << " m/s" << endl;
    cout << "普朗克常数:     " << planckConst << " J*s" << endl;
    cout << "阿伏伽德罗常数: " << avogadroNum << endl;
    cout << "地球质量:       " << earthMass << " kg" << endl;

    // =========================================================
    // 十、浮点数与整数的混合运算
    // =========================================================
    cout << "\n===== 十、混合运算 =====" << endl;
    // 整数和浮点数运算时, 整数会被自动转换为浮点数 (隐式类型转换)

    int intNum = 5;
    double doubleNum = 2.0;
    double result = intNum / doubleNum;  // int 被提升为 double
    cout << "5 / 2.0 = " << result << endl;  // 输出 2.5

    // 注意: 两个整数相除, 结果仍是整数 (截断小数部分, 不是四舍五入!)
    int result2 = 5 / 2;
    cout << "5 / 2 (整数除法) = " << result2 << endl;  // 输出 2, 不是 2.5

    // 要得到浮点结果, 至少一个操作数必须是浮点型
    double result3 = 5.0 / 2;           // 5.0 是 double, 结果为 2.5
    double result4 = (double)5 / 2;     // 强制类型转换
    double result5 = static_cast<double>(5) / 2;  // C++ 风格强制转换 (推荐)
    cout << "5.0 / 2 = " << result3 << endl;
    cout << "(double)5 / 2 = " << result4 << endl;
    cout << "static_cast<double>(5) / 2 = " << result5 << endl;

    // 整数溢出 vs 浮点数溢出
    cout << "\n溢出行为对比:" << endl;
    int maxInt = INT_MAX;
    cout << "INT_MAX = " << maxInt << endl;
    cout << "INT_MAX + 1 = " << (maxInt + 1) << " (整数溢出, 结果未定义!)" << endl;

    double maxDouble = DBL_MAX;
    cout << "DBL_MAX = " << maxDouble << endl;
    cout << "DBL_MAX * 2 = " << (maxDouble * 2) << " (浮点溢出, 结果为 Inf)" << endl;

    // =========================================================
    // 十一、浮点数的输出格式控制
    // =========================================================
    cout << "\n===== 十一、输出格式控制 =====" << endl;

    double pi = 3.141592653589793;

    // 默认输出
    cout << "默认: " << pi << endl;

    // fixed: 定点表示法 (小数点后固定位数)
    cout << fixed;
    cout << "fixed(2):  " << setprecision(2) << pi << endl;
    cout << "fixed(5):  " << setprecision(5) << pi << endl;
    cout << "fixed(10): " << setprecision(10) << pi << endl;

    // scientific: 科学计数法
    cout << scientific;
    cout << "scientific(4): " << setprecision(4) << pi << endl;
    cout << "scientific(8): " << setprecision(8) << pi << endl;

    // 恢复默认
    cout << defaultfloat << setprecision(6);

    // =========================================================
    // 十二、浮点型选择建议
    // =========================================================
    cout << "\n===== 十二、浮点型选择建议 =====" << endl;
    // 1. 一般计算: 使用 double (最常用, 精度足够, 性能与 float 相当)
    // 2. 节省内存: 使用 float (如图形处理、大量数据存储)
    // 3. 高精度需求: 使用 long double (如科学计算, 但平台依赖性强)
    // 4. 金融计算: 不建议用浮点型! 应使用定点数库或整数运算 (以分为单位)
    // 5. 比较浮点数: 不要用 ==, 用误差范围比较
    // 6. 避免浮点数与整数的混合运算导致的精度问题

    // 金融计算的正确做法 (以分为单位):
    int price = 1999;    // 19.99 元, 存为 1999 分
    int quantity = 3;
    int total = price * quantity;  // 5997 分 = 59.97 元
    cout << "\n金融计算示例 (以分为单位):" << endl;
    cout << "单价: " << price / 100 << "." << setw(2) << setfill('0') << price % 100 << " 元" << endl;
    cout << "数量: " << quantity << endl;
    cout << "总价: " << total / 100 << "." << setw(2) << setfill('0') << total % 100 << " 元" << endl;
    cout << setfill(' ');  // 恢复默认填充字符

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. float (4字节, ~7位精度), double (8字节, ~16位精度), long double (8-16字节)
 * 2. IEEE 754: 符号位 + 指数 + 尾数, 值 = (-1)^符号 * 1.尾数 * 2^(指数-偏移)
 * 3. 0.1 + 0.2 != 0.3 的原因: 二进制无法精确表示某些十进制小数
 * 4. 浮点数比较: 使用 fabs(a - b) < epsilon, 不要用 ==
 * 5. 特殊值: NaN (isnan 判断), Inf (isinf 判断), 非规格化数
 * 6. NaN 不等于任何值, 包括自己; NaN 有"传染性"
 * 7. 科学计数法: 3.14e2 = 314, 1.5e-3 = 0.0015
 * 8. 整数/浮点混合运算: 整数自动提升为浮点数
 * 9. 一般计算推荐使用 double, 金融计算用整数 (以分为单位)
 * 10. float 的累积误差比 double 大得多, 长期计算优先选 double
 * ===============================================
 *
 * ==================== 练习 ====================
 * TODO 1: 写一个函数, 判断两个 double 是否"相等" (使用 epsilon 比较)
 * TODO 2: 计算并输出 1/3 在 float 和 double 中的精度差异
 * TODO 3: 写一个程序, 演示 float 的累积误差 (累加 0.1 一百万次)
 * TODO 4: 将十进制数 0.5 转换为二进制, 验证它能被精确表示
 * TODO 5: 写一个程序, 用 ULP 方法比较两个浮点数是否相等
 * TODO 6: 研究为什么金融系统不用浮点数, 用整数以分为单位有什么好处
 * ===============================================
 */
