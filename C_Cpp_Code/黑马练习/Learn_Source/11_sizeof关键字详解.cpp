/*
 * ============================================================================
 * 主题: sizeof 关键字详解 (详解版)
 * ============================================================================
 * sizeof 是 C/C++ 中的单目运算符（也可以说是一个关键字），
 * 用于计算 类型 或 变量 所占用的字节数。
 *
 * 基本语法：
 *     sizeof(类型名)    -- 计算某个类型占用的字节数
 *     sizeof(变量名)    -- 计算某个变量占用的字节数
 *     sizeof 变量名     -- 变量名的括号可以省略（不推荐）
 *
 * 本文件深入介绍:
 *   - 基本数据类型的 sizeof
 *   - 指针的 sizeof (与类型无关, 只与系统位数有关)
 *   - 数组的 sizeof 与元素个数计算
 *   - 数组传入函数时退化为指针的问题 (重点!)
 *   - 结构体的 sizeof 与内存对齐/填充详解
 *   - 类的 sizeof (含虚函数表指针)
 *   - 联合体 (union) 的 sizeof
 *   - #pragma pack 对齐控制
 *   - sizeof vs strlen 的区别
 *   - sizeof 是编译期运算符 (不执行表达式)
 *
 * 注意事项：
 *   1. sizeof 的结果类型是 size_t（无符号整型），用 %zu 或 %llu 打印
 *   2. 不同平台、不同编译器下类型大小可能不同（32位 vs 64位）
 *   3. 结构体存在"内存对齐"，实际大小往往大于各成员大小之和
 * ============================================================================
 */

#include <iostream>
#include <cstring>    // 提供 strlen 函数
using namespace std;

// ============================================================================
// 结构体定义 (用于演示内存对齐)
// ============================================================================

// 结构体1: char + int + char (不好的排列顺序)
struct Example {
    char  a;    // 1 字节
    int   b;    // 4 字节
    char  c;    // 1 字节
};

// 结构体2: 调整成员顺序, 减少填充
struct Example2 {
    int   b;    // 4 字节
    char  a;    // 1 字节
    char  c;    // 1 字节
};

// 结构体3: 包含 double, 对齐要求更高
struct Example3 {
    char   a;    // 1 字节
    double b;    // 8 字节
    int    c;    // 4 字节
    char   d;    // 1 字节
};

// 结构体4: 优化排列的 Example3
struct Example4 {
    double b;    // 8 字节
    int    c;    // 4 字节
    char   a;    // 1 字节
    char   d;    // 1 字节
};

// 结构体5: 嵌套结构体
struct Inner {
    char  a;    // 1 字节
    int   b;    // 4 字节
};  // sizeof(Inner) = 8 字节 (含 3 字节填充)

struct Outer {
    char   x;      // 1 字节
    Inner  inner;  // 8 字节 (Inner 的对齐要求是 4)
    double y;      // 8 字节
};

// ============================================================================
// 类定义 (用于演示类的 sizeof)
// ============================================================================

// 普通类 (无虚函数)
class SimpleClass {
public:
    int x;
    int y;
    char z;
    // sizeof(SimpleClass) 与结构体类似, 遵循相同的对齐规则
    // 但要注意访问权限: private 成员也占用空间
private:
    int secret;
};

// 含虚函数的类 (有虚函数表指针)
class VirtualClass {
public:
    int x;
    virtual void func() {}  // 虚函数
    // sizeof(VirtualClass) = 成员大小 + 虚函数表指针 (vptr) 大小
    // vptr 通常占 4 字节 (32位) 或 8 字节 (64位)
};

// 继承中的虚函数
class Base {
public:
    int a;
    virtual void foo() {}
};

class Derived : public Base {
public:
    int b;
    virtual void foo() override {}
    // sizeof(Derived) = Base 的大小 + int b 的大小
    // 派生类共享基类的虚函数表指针, 不会额外增加 vptr
};

// ============================================================================
// 联合体定义 (用于演示 union 的 sizeof)
// ============================================================================

// 联合体 (union): 所有成员共享同一块内存
// sizeof(union) = 最大成员的大小 (考虑对齐)
union MyUnion {
    char   a;    // 1 字节
    int    b;    // 4 字节
    double c;    // 8 字节
};  // sizeof(MyUnion) = 8 字节

// 带数组的联合体
union ArrayUnion {
    char   str[10];  // 10 字节
    int    nums[3];  // 12 字节
};  // sizeof(ArrayUnion) = 12 字节 (取最大)

// ============================================================================
// 辅助函数 (用于演示数组传参退化为指针)
// ============================================================================

// 错误示范: 试图用 sizeof 计算数组大小 (失败!)
void printArraySizeWrong(int arr[]) {
    // arr 实际上是指针 int*, 不是数组!
    // sizeof(arr) 返回的是指针的大小, 不是数组的大小
    cout << "函数内 sizeof(arr) = " << sizeof(arr) << " 字节" << endl;
    cout << "函数内 sizeof(arr)/sizeof(arr[0]) = " << sizeof(arr) / sizeof(arr[0]) << endl;
    // 结果错误! 在 64 位系统上通常输出 2 (8/4=2), 而不是实际元素个数
}

// 正确做法1: 传递数组大小作为参数
void printArraySizeCorrect(int arr[], int size) {
    cout << "数组元素个数 (由参数传入) = " << size << endl;
}

// 正确做法2: 使用模板 (编译期推导数组大小)
template <size_t N>
void printArraySizeTemplate(int (&arr)[N]) {
    // N 就是数组的元素个数, 编译器自动推导
    cout << "模板推导的数组元素个数 = " << N << endl;
}

// 正确做法3: 使用 C++17 的 std::size
// 需要 #include <iterator>
// cout << std::size(arr) << endl;

// ============================================================================
// 主函数
// ============================================================================

int main() {

    // =========================================================
    // 一、基本数据类型的 sizeof
    // =========================================================
    cout << "===== 基本数据类型的大小 =====" << endl;

    // sizeof(类型名) 可以直接计算类型的字节数
    cout << "char       的大小: " << sizeof(char)       << " 字节" << endl;  // 1
    cout << "short      的大小: " << sizeof(short)      << " 字节" << endl;  // 2
    cout << "int        的大小: " << sizeof(int)        << " 字节" << endl;  // 4
    cout << "long       的大小: " << sizeof(long)       << " 字节" << endl;  // 4 (Windows 64位)
    cout << "long long  的大小: " << sizeof(long long)  << " 字节" << endl;  // 8
    cout << "float      的大小: " << sizeof(float)      << " 字节" << endl;  // 4
    cout << "double     的大小: " << sizeof(double)     << " 字节" << endl;  // 8
    cout << "long double的大小: " << sizeof(long double)<< " 字节" << endl;  // 8-16
    cout << "bool       的大小: " << sizeof(bool)       << " 字节" << endl;  // 1
    cout << "wchar_t    的大小: " << sizeof(wchar_t)    << " 字节" << endl;  // 2 (Windows)
    cout << "char16_t   的大小: " << sizeof(char16_t)   << " 字节" << endl;  // 2
    cout << "char32_t   的大小: " << sizeof(char32_t)   << " 字节" << endl;  // 4
    cout << endl;

    // =========================================================
    // 二、变量的 sizeof
    // =========================================================
    cout << "===== 变量的 sizeof =====" << endl;

    int    numInt    = 100;
    double numDouble = 3.14;
    char   ch        = 'A';
    bool   flag      = true;

    // 对变量使用 sizeof, 括号可省略, 写成 sizeof numInt
    // 但推荐始终加括号, 因为 sizeof int 这种写法容易混淆
    cout << "numInt    占 " << sizeof(numInt)    << " 字节" << endl;
    cout << "numDouble 占 " << sizeof(numDouble) << " 字节" << endl;
    cout << "ch        占 " << sizeof(ch)        << " 字节" << endl;
    cout << "flag      占 " << sizeof(flag)      << " 字节" << endl;
    cout << endl;

    // =========================================================
    // 三、指针的 sizeof
    // =========================================================
    cout << "===== 指针的大小 =====" << endl;

    // 在 64 位系统中, 所有类型的指针大小都是 8 字节
    // 在 32 位系统中, 所有类型的指针大小都是 4 字节
    // 指针大小只跟系统位数有关, 与所指向的类型无关
    int*    pInt    = &numInt;
    double* pDouble = &numDouble;
    char*   pChar   = &ch;
    void*   pVoid   = nullptr;

    cout << "int*    指针大小: " << sizeof(pInt)    << " 字节" << endl;
    cout << "double* 指针大小: " << sizeof(pDouble) << " 字节" << endl;
    cout << "char*   指针大小: " << sizeof(pChar)   << " 字节" << endl;
    cout << "void*   指针大小: " << sizeof(pVoid)   << " 字节" << endl;
    cout << "函数指针大小:     " << sizeof(&main)    << " 字节" << endl;

    // 为什么指针大小相同?
    // 因为指针存储的是内存地址, 在 64 位系统上地址是 64 位 (8 字节)
    // 无论指向 char (1字节) 还是 double (8字节), 地址本身都是 8 字节
    cout << endl;

    // =========================================================
    // 四、数组的 sizeof 与元素个数计算
    // =========================================================
    cout << "===== 数组的大小 =====" << endl;

    int arr[] = {10, 20, 30, 40, 50};
    double darr[] = {1.1, 2.2, 3.3};
    char carr[] = "Hello";  // 包含 '\0', 共 6 个字节

    // sizeof(数组名) 返回整个数组占用的字节数
    cout << "int arr[]    总大小: " << sizeof(arr)  << " 字节" << endl;
    cout << "double darr[]总大小: " << sizeof(darr) << " 字节" << endl;
    cout << "char carr[]  总大小: " << sizeof(carr) << " 字节" << endl;

    // sizeof(arr[0]) 返回单个元素占用的字节数
    cout << "int 单个元素大小:   " << sizeof(arr[0])  << " 字节" << endl;
    cout << "double 单个元素大小:" << sizeof(darr[0]) << " 字节" << endl;

    // 经典技巧: 计算数组元素个数
    //   数组元素个数 = 整个数组大小 / 单个元素大小
    int len = sizeof(arr) / sizeof(arr[0]);
    cout << "int arr 元素个数:   " << len << endl;
    cout << "double darr 元素个数: " << sizeof(darr) / sizeof(darr[0]) << endl;

    // 注意: 字符数组要小心 '\0'
    cout << "char carr 元素个数: " << sizeof(carr) / sizeof(carr[0]) << endl;
    // 输出 6 (包含 '\0'), 如果想要实际字符数, 用 strlen
    cout << "carr 的 strlen:     " << strlen(carr) << endl;  // 输出 5 (不含 '\0')
    cout << endl;

    // =========================================================
    // 五、数组传入函数时退化为指针 (重要!)
    // =========================================================
    cout << "===== 数组传参退化问题 =====" << endl;

    // 当数组作为函数参数传递时, 会"退化"为指向首元素的指针
    // 这意味着在函数内部, sizeof 无法获取数组的真实大小

    cout << "main 中 sizeof(arr) = " << sizeof(arr) << " 字节" << endl;
    cout << "main 中元素个数 = " << sizeof(arr) / sizeof(arr[0]) << endl;

    // 传入函数后, sizeof 结果不同!
    printArraySizeWrong(arr);  // 结果错误!

    // 正确做法: 显式传递数组大小
    int arrSize = sizeof(arr) / sizeof(arr[0]);
    printArraySizeCorrect(arr, arrSize);

    // 正确做法: 使用模板 (编译期推导)
    printArraySizeTemplate(arr);

    // 内存布局图解:
    //
    // main 中的 arr:
    // ┌─────┬─────┬─────┬─────┬─────┐
    // │ 10  │ 20  │ 30  │ 40  │ 50  │  5 个 int, sizeof = 20 字节
    // └─────┴─────┴─────┴─────┴─────┘
    //
    // 传入函数后退化为指针:
    // ┌─────────────────────┐
    // │ 指向 arr[0] 的地址   │  1 个指针, sizeof = 8 字节 (64位)
    // └─────────────────────┘
    //
    // 这是 C/C++ 的一个重要特性, 也是常见的 bug 来源!
    cout << endl;

    // =========================================================
    // 六、结构体的 sizeof 与内存对齐详解
    // =========================================================
    cout << "===== 结构体的大小（内存对齐）=====" << endl;

    // 按照直观理解: Example 应该是 1 + 4 + 1 = 6 字节
    // 但实际结果往往是 12 字节, 原因是"内存对齐"
    cout << "Example  大小: " << sizeof(Example)  << " 字节 (成员: char, int, char)" << endl;
    cout << "Example2 大小: " << sizeof(Example2) << " 字节 (成员: int, char, char)" << endl;
    cout << "Example3 大小: " << sizeof(Example3) << " 字节 (成员: char, double, int, char)" << endl;
    cout << "Example4 大小: " << sizeof(Example4) << " 字节 (成员: double, int, char, char)" << endl;

    // 内存对齐规则 (详细说明):
    //   规则1: 每个成员的起始地址必须是其自身大小的整数倍
    //          char 从任意地址开始, short 从 2 的倍数, int 从 4 的倍数...
    //   规则2: 结构体总大小必须是最大成员大小的整数倍
    //   规则3: 编译器会在成员之间插入"填充字节" (padding) 来满足对齐要求

    // Example 布局分析 (假设在 64 位系统):
    //
    // 偏移:  0     1     2     3     4     5     6     7     8     9    10    11
    //       ┌─────┬─────────────────────┬─────┬─────────────────────┐
    //       │  a  │    填充 (padding)    │  b  (4字节)              │  c  │ 填充 │
    //       │ 1B  │      3 字节          │       4 字节             │ 1B  │ 3字节│
    //       └─────┴─────────────────────┴─────────────────────┴─────┴─────┘
    //       char a  : 偏移 0，占 1 字节
    //       [填充 3 字节]  → 为了让 int b 对齐到 4 的倍数 (偏移 4)
    //       int  b  : 偏移 4，占 4 字节
    //       char c  : 偏移 8，占 1 字节
    //       [填充 3 字节]  → 让总大小对齐到 4 的倍数 (最大成员 int = 4)
    //       总计：12 字节

    // Example2 布局分析:
    //
    // 偏移:  0     1     2     3     4     5     6     7
    //       ┌─────────────────────┬─────┬─────┬─────────┐
    //       │  b  (4字节)          │  a  │  c  │  填充    │
    //       │       4 字节         │ 1B  │ 1B  │  2 字节  │
    //       └─────────────────────┴─────┴─────┴─────────┘
    //       int  b  : 偏移 0，占 4 字节
    //       char a  : 偏移 4，占 1 字节
    //       char c  : 偏移 5，占 1 字节
    //       [填充 2 字节]  → 让总大小对齐到 4 的倍数
    //       总计：8 字节
    //
    // 经验：把大的成员放在前面，可以减少填充，节省内存。

    // Example3 vs Example4 对比:
    // Example3 (char, double, int, char): 需要更多填充, 总计 24 字节
    // Example4 (double, int, char, char): 更紧凑, 总计 16 字节
    // 相同的成员, 不同的顺序, 大小差了 8 字节!

    // 嵌套结构体的对齐
    cout << "\n嵌套结构体:" << endl;
    cout << "Inner 大小: " << sizeof(Inner) << " 字节" << endl;
    cout << "Outer 大小: " << sizeof(Outer) << " 字节" << endl;
    // Inner 的对齐要求是 4 (最大成员 int), 所以 Outer 中 Inner 从 4 的倍数开始

    cout << endl;

    // =========================================================
    // 七、类的 sizeof
    // =========================================================
    cout << "===== 类的大小 =====" << endl;

    // 普通类: 与结构体类似, 遵循相同的对齐规则
    // private 成员也占用空间
    cout << "SimpleClass 大小: " << sizeof(SimpleClass) << " 字节" << endl;
    // 成员: int x(4) + int y(4) + char z(1) + 填充(3) + int secret(4) = 16 字节

    // 含虚函数的类: 额外有一个虚函数表指针 (vptr)
    cout << "VirtualClass 大小: " << sizeof(VirtualClass) << " 字节" << endl;
    // 成员: vptr(8) + int x(4) + 填充(4) = 16 字节 (64位系统)

    // 虚函数表指针 (vptr) 说明:
    //   - 每个含虚函数的类, 编译器会添加一个隐藏的 vptr
    //   - vptr 指向虚函数表 (vtable), 用于实现动态多态
    //   - 64 位系统上 vptr 占 8 字节, 32 位系统上占 4 字节
    //
    // VirtualClass 内存布局:
    // ┌──────────────────┬──────────────┬─────────┐
    // │  vptr (8 字节)    │  int x (4B)  │ 填充 4B  │
    // └──────────────────┴──────────────┴─────────┘
    // 总计: 16 字节

    // 继承中的 sizeof
    cout << "Base 大小:    " << sizeof(Base)    << " 字节" << endl;
    cout << "Derived 大小: " << sizeof(Derived) << " 字节" << endl;
    // Base: vptr(8) + int a(4) + 填充(4) = 16
    // Derived: Base(16) + int b(4) + 填充(4) = 20
    // 派生类共享基类的 vptr, 不会额外增加

    // 空类的大小
    class EmptyClass {};
    cout << "空类大小: " << sizeof(EmptyClass) << " 字节" << endl;
    // 空类占 1 字节! 为了保证不同对象有不同的地址
    cout << endl;

    // =========================================================
    // 八、联合体 (union) 的 sizeof
    // =========================================================
    cout << "===== 联合体的大小 =====" << endl;

    // 联合体: 所有成员共享同一块内存
    // sizeof(union) = 最大成员的大小 (考虑对齐)
    cout << "MyUnion 大小: " << sizeof(MyUnion) << " 字节" << endl;
    // 最大成员 double c = 8 字节, 所以 sizeof(MyUnion) = 8

    cout << "ArrayUnion 大小: " << sizeof(ArrayUnion) << " 字节" << endl;
    // char str[10] = 10 字节, int nums[3] = 12 字节
    // 最大 = 12, 且 12 是 4 的倍数 (int 的对齐要求), 所以 = 12

    // 联合体的用途:
    //   1. 节省内存: 同一时间只使用一个成员
    //   2. 类型双关: 查看同一数据的不同表示形式
    //   3. 实现变体类型 (variant)

    // 类型双关示例: 查看 float 的二进制表示
    union FloatBits {
        float f;
        unsigned int bits;
    };
    FloatBits fb;
    fb.f = 3.14f;
    cout << "float 3.14 的二进制表示: 0x" << hex << fb.bits << dec << endl;
    cout << endl;

    // =========================================================
    // 九、#pragma pack 对齐控制
    // =========================================================
    cout << "===== #pragma pack 对齐控制 =====" << endl;

    // #pragma pack(n) 可以设置最大对齐边界为 n 字节
    // n 必须是 1, 2, 4, 8, 16 等 2 的幂

    // 默认对齐 (编译器决定)
    cout << "默认对齐: Example = " << sizeof(Example) << " 字节" << endl;

    // 设置对齐为 1 字节 (无填充)
    #pragma pack(push, 1)  // 保存当前对齐设置, 设置为 1
    struct PackedExample {
        char  a;    // 1 字节
        int   b;    // 4 字节
        char  c;    // 1 字节
    };
    #pragma pack(pop)  // 恢复之前的对齐设置

    cout << "#pragma pack(1): PackedExample = " << sizeof(PackedExample) << " 字节" << endl;
    // 没有填充, 总大小 = 1 + 4 + 1 = 6 字节

    // 设置对齐为 2 字节
    #pragma pack(push, 2)
    struct PackedExample2 {
        char  a;    // 1 字节
        int   b;    // 4 字节
        char  c;    // 1 字节
    };
    #pragma pack(pop)

    cout << "#pragma pack(2): PackedExample2 = " << sizeof(PackedExample2) << " 字节" << endl;
    // 对齐边界为 2, 总大小 = 1 + 1(填充) + 4 + 1 + 1(填充) = 8 字节

    // 注意: 修改对齐可能影响性能!
    // CPU 读取未对齐的数据可能需要多次内存访问
    // 但在网络协议或文件格式中, 有时需要紧凑的内存布局
    cout << endl;

    // =========================================================
    // 十、sizeof vs strlen 对比
    // =========================================================
    cout << "===== sizeof vs strlen =====" << endl;

    // sizeof: 计算占用的字节数 (包括 '\0'), 编译期求值
    // strlen: 计算字符串的实际长度 (不包括 '\0'), 运行时求值

    char str1[] = "Hello";
    char str2[] = "Hi";
    char str3[10] = "Hello";  // 有 10 字节, 但只用了 6 字节 (含 '\0')

    cout << "\"Hello\" 的 sizeof: " << sizeof(str1) << " (5字符 + '\\0' = 6)" << endl;
    cout << "\"Hello\" 的 strlen: " << strlen(str1) << " (不含 '\\0')" << endl;
    cout << "\"Hi\"    的 sizeof: " << sizeof(str2) << " (2字符 + '\\0' = 3)" << endl;
    cout << "\"Hi\"    的 strlen: " << strlen(str2) << endl;
    cout << "str3[10] 的 sizeof: " << sizeof(str3) << " (整个数组 10 字节)" << endl;
    cout << "str3     的 strlen: " << strlen(str3) << " (实际字符串长度 5)" << endl;

    // 字符串常量的 sizeof
    cout << "\n字符串常量:" << endl;
    cout << "\"Hello\"   的 sizeof: " << sizeof("Hello") << " 字节" << endl;
    cout << "\"\"        的 sizeof: " << sizeof("")      << " 字节 (只有 '\\0')" << endl;
    cout << "\"A\"       的 sizeof: " << sizeof("A")     << " 字节 ('A' + '\\0')" << endl;

    // sizeof 是编译期运算符, strlen 是运行时函数
    // sizeof 的结果在编译时就确定了, strlen 需要在运行时遍历字符串
    cout << endl;

    // =========================================================
    // 十一、sizeof 的特殊行为
    // =========================================================
    cout << "===== sizeof 的特殊行为 =====" << endl;

    // 1. sizeof 是编译期运算, 不会实际执行表达式
    int x = 10;
    cout << "sizeof(x = 20) = " << sizeof(x = 20) << endl;
    cout << "x 的值仍然是: " << x << endl;
    // x 的值没有被改变! 因为 sizeof 在编译期求值, x = 20 不会真正执行

    // 2. sizeof 的结果类型是 size_t (无符号整数)
    cout << "sizeof(size_t) = " << sizeof(size_t) << " 字节" << endl;

    // 3. sizeof 不能用于函数类型、不完整类型、位域
    // sizeof(void)    // 错误: void 是不完整类型
    // sizeof(int())   // 错误: 函数类型 (但 sizeof(int(*)()) 可以, 这是指针)

    // 4. 条件表达式的 sizeof
    cout << "sizeof(true ? 1.0 : 1) = " << sizeof(true ? 1.0 : 1) << endl;
    // 结果是 8 (double), 因为条件表达式的类型是 double

    // 5. sizeof 与模板
    // 模板可以根据 sizeof 在编译期做条件判断 (SFINAE, type traits 等)
    // 这是高级用法, 后续学习模板时会深入
    cout << endl;

    // =========================================================
    // 十二、常见误区汇总
    // =========================================================
    cout << "===== 常见误区 =====" << endl;

    // 误区1: sizeof 不是函数, 它是运算符
    //        所以 sizeof int 也是合法的 (但不推荐, 建议写成 sizeof(int))

    // 误区2: 以为数组传入函数后 sizeof 仍然有效
    //        数组会退化为指针, sizeof 返回指针大小

    // 误区3: 以为 sizeof 是运行时计算
    //        sizeof 是编译期运算, 不会执行表达式

    // 误区4: 忘记字符串的 '\0'
    //        char str[] = "Hello"; sizeof = 6, 不是 5

    // 误区5: 以为结构体大小等于成员大小之和
    //        结构体有内存对齐, 实际大小通常更大

    // 误区6: 以为类和结构体的 sizeof 规则不同
    //        它们的对齐规则相同, 唯一区别是默认访问权限

    // 误区7: 以为所有指针大小相同 (在某些嵌入式系统上可能不同)
    //        在主流桌面系统 (x86/x64) 上确实相同

    cout << endl;
    cout << "===== sizeof 演示结束 =====" << endl;

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. sizeof(类型) 或 sizeof(变量) 计算字节数, 结果是 size_t
 * 2. 指针大小只与系统位数有关: 64位=8字节, 32位=4字节
 * 3. 数组 sizeof = 元素个数 * 单个元素大小
 * 4. 数组传入函数退化为指针, sizeof 不再返回数组大小!
 * 5. 结构体有内存对齐, 实际大小 >= 成员大小之和
 * 6. 大成员放前面可以减少填充, 节省内存
 * 7. 含虚函数的类有额外的 vptr (64位=8字节)
 * 8. 联合体 sizeof = 最大成员大小
 * 9. #pragma pack(n) 可以修改对齐边界
 * 10. sizeof 是编译期运算, 不会执行表达式
 * 11. sizeof(字符串) 包含 '\0', strlen 不包含
 * 12. 空类大小为 1 字节, 保证不同对象有不同地址
 * ===============================================
 *
 * ==================== 练习 ====================
 * TODO 1: 定义一个包含 char, int, short, double 的结构体,
 *          尝试不同排列顺序, 观察 sizeof 的变化
 * TODO 2: 写一个宏或模板函数, 安全地计算数组元素个数
 * TODO 3: 用 #pragma pack(1) 定义一个结构体, 对比默认对齐的大小差异
 * TODO 4: 定义一个含虚函数的类, 输出其 sizeof, 解释为什么比成员总和大
 * TODO 5: 写一个程序, 用 union 查看 int 的每个字节 (类型双关)
 * TODO 6: 解释为什么 sizeof(空类) = 1, 如果为 0 会有什么问题
 * ===============================================
 */
