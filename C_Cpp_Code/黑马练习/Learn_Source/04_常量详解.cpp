/*
 * ============================================================================
 * 主题: C++ 常量详解
 * ============================================================================
 * 常量是程序运行期间值不能被修改的量。
 * C++ 中定义常量有两种方式:
 *   - #define 宏定义 (预处理指令)
 *   - const 关键字 (推荐)
 * 本文件详细比较两者的区别和使用场景。
 * ============================================================================
 */

#include <iostream>
using namespace std;

// ==================== #define 宏定义常量 ====================
// #define 是预处理器指令, 在编译之前进行文本替换
// 语法: #define 常量名 值
// 注意: 不需要分号结尾, 没有类型检查

#define PI 3.14159265358979
#define MAX_SIZE 100
#define SCHOOL_NAME "黑马程序员"
#define GRADE_LIMIT 60

// ==================== const 常量 ====================
// const 是 C++ 的关键字, 用于声明常量
// 语法: const 数据类型 常量名 = 值;
// 有类型检查, 更安全, 推荐使用

const double E = 2.71828182845904;
const int MIN_SCORE = 0;
const int MAX_SCORE = 100;
const string GREETING = "Hello, C++!";

int main() {

    // ==================== 使用 #define 常量 ====================
    cout << "===== #define 常量 =====" << endl;
    cout << "PI = " << PI << endl;
    cout << "学校名称: " << SCHOOL_NAME << endl;

    // 使用宏定义计算圆的面积
    double radius = 5.0;
    double area = PI * radius * radius;  // 预处理后变成 3.14159... * 5.0 * 5.0
    cout << "半径 " << radius << " 的圆面积 = " << area << endl;

    // 使用宏定义作为数组大小
    int arr[MAX_SIZE];  // 预处理后变成 int arr[100];
    cout << "数组大小: " << sizeof(arr) / sizeof(arr[0]) << endl;

    // ==================== 使用 const 常量 ====================
    cout << "\n===== const 常量 =====" << endl;
    cout << "E = " << E << endl;
    cout << "最低分: " << MIN_SCORE << ", 最高分: " << MAX_SCORE << endl;
    cout << GREETING << endl;

    // 使用 const 常量进行判断
    int score = 75;
    if (score >= GRADE_LIMIT) {
        cout << "成绩 " << score << " 及格了!" << endl;
    } else {
        cout << "成绩 " << score << " 不及格!" << endl;
    }

    // ==================== #define 与 const 的区别 ====================
    cout << "\n===== #define vs const 区别 =====" << endl;

    // 区别1: 类型检查
    // #define 没有类型检查, 只是简单的文本替换
    #define NUM 100
    const int num = 100;
    // #define 可以被误用, 例如: #define NUM "hello" 不会报类型错误
    // const 有类型检查, const int num = "hello"; 会编译报错

    // 区别2: 作用域
    // #define 从定义处到文件末尾, 或者到 #undef
    // const 遵循 C++ 的作用域规则
    {
        const int localVar = 50;  // 这个 const 常量只在花括号内有效
        cout << "局部 const: " << localVar << endl;
        // #define 没有块作用域的概念
    }
    // cout << localVar;  // 错误: localVar 已经超出作用域

    // 区别3: 调试
    // #define 在预处理阶段被替换, 调试时看不到宏名
    // const 常量有类型和名称, 调试时可以看到

    // 区别4: 内存
    // #define 不分配内存, 只是文本替换
    // const 常量可能分配内存 (取决于编译器优化)

    // ==================== #define 的注意事项 ====================
    cout << "\n===== #define 注意事项 =====" << endl;

    // 注意1: #define 不需要分号
    // 错误写法: #define VALUE 100;  (分号会被包含在替换中)
    // 例如: int x = VALUE; 会变成 int x = 100;; (两个分号)

    // 注意2: 宏定义的副作用
    // 由于是文本替换, 某些表达式可能产生意外结果
    #define SQUARE(x) ((x) * (x))   // 定义带参数的宏 (函数宏)
    int result1 = SQUARE(5);         // 替换为 ((5) * (5)) = 25
    cout << "SQUARE(5) = " << result1 << endl;

    // 危险的宏定义 (没有加括号):
    // #define BAD_SQUARE(x) x * x
    // BAD_SQUARE(2+3) 会变成 2+3 * 2+3 = 2+6+3 = 11, 而不是 25!

    // 安全的宏定义 (加了括号):
    // #define SQUARE(x) ((x) * (x))
    // SQUARE(2+3) 变成 ((2+3) * (2+3)) = 5*5 = 25

    int a = 2, b = 3;
    int result2 = SQUARE(a + b);
    cout << "SQUARE(2+3) = " << result2 << " (正确: 25)" << endl;

    // ==================== const 的注意事项 ====================
    cout << "\n===== const 注意事项 =====" << endl;

    // 注意1: const 常量必须初始化
    const int MUST_INIT = 100;   // 正确: 声明时初始化
    // const int ERROR;          // 错误: const 变量必须初始化

    // 注意2: const 常量不能被修改
    // MUST_INIT = 200;           // 错误: 不能修改 const 常量

    // 注意3: const 可以用于指针 (进阶内容)
    int value = 42;
    const int* ptr = &value;  // 指向 const int 的指针, 不能通过 ptr 修改值
    // *ptr = 100;             // 错误: 不能通过 const 指针修改值
    cout << "const 指针指向的值: " << *ptr << endl;

    // ==================== 实际应用 ====================
    cout << "\n===== 实际应用 =====" << endl;

    // 应用1: 数学常量
    const double GRAVITY = 9.8;         // 重力加速度
    const double SPEED_OF_LIGHT = 3e8;  // 光速 (米/秒)
    cout << "重力加速度: " << GRAVITY << " m/s^2" << endl;
    cout << "光速: " << SPEED_OF_LIGHT << " m/s" << endl;

    // 应用2: 配置参数
    const int MAX_RETRY = 3;           // 最大重试次数
    const int TIMEOUT_MS = 5000;       // 超时时间 (毫秒)
    const string DEFAULT_LANG = "zh";  // 默认语言
    cout << "最大重试: " << MAX_RETRY << ", 超时: " << TIMEOUT_MS << "ms" << endl;

    // 应用3: 在条件判断中使用常量
    int examScore = 85;
    if (examScore >= MAX_SCORE) {
        cout << "满分!" << endl;
    } else if (examScore >= GRADE_LIMIT) {
        cout << "及格, 分数: " << examScore << endl;
    } else {
        cout << "不及格" << endl;
    }

    // ==================== #undef 取消宏定义 ====================
    // #undef 用于取消之前定义的宏
    #define TEMP_MACRO "临时宏"
    cout << "TEMP_MACRO: " << TEMP_MACRO << endl;
    #undef TEMP_MACRO   // 取消定义, 之后不能再使用
    // cout << TEMP_MACRO;  // 错误: TEMP_MACRO 未定义

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. #define 是预处理指令, 进行文本替换, 无类型检查
 * 2. const 是 C++ 关键字, 有类型检查, 推荐使用
 * 3. #define 常量名一般全大写: #define PI 3.14
 * 4. const 必须在声明时初始化, 之后不能修改
 * 5. 宏函数要注意加括号: #define SQUARE(x) ((x)*(x))
 * 6. #undef 可以取消宏定义
 * 7. 优先使用 const, 更安全、可调试
 * ===============================================
 */
