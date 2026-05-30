/*
 * ============================================================
 * 文件名: 37_函数默认参数.cpp
 * 主题: C++ 函数默认参数 (Default Arguments)
 * ============================================================
 * 本文件涵盖:
 *   1. 默认参数的基本用法
 *   2. 规则: 默认参数必须从右到左
 *   3. 声明与定义中只能指定一次默认值
 *   4. 默认参数与函数重载的冲突
 *   5. 默认参数的实际应用场景
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 1. 基本默认参数
// ============================================================
// 在声明中指定默认参数值
// 调用时可以省略有默认值的参数

// 函数声明: 带默认参数
void printMessage(string msg, string prefix = "[INFO]", string suffix = "---");

// 带默认参数的计算函数
double calculateArea(double width, double height = 1.0);
double calculateCircleArea(double radius, double pi = 3.14159265);

// ============================================================
// 2. 默认参数必须从右到左
// ============================================================
// 规则: 如果某个参数有默认值, 那么它右边的所有参数都必须有默认值

// 正确示例: 从右到左依次有默认值
void correctExample(int a, int b = 10, int c = 20) {
    cout << "  a=" << a << ", b=" << b << ", c=" << c << endl;
}

// 错误示例 (已注释): 左边有默认值, 右边没有
// void wrongExample(int a = 10, int b, int c = 20) { ... }
// 编译错误! b 没有默认值, 但左边的 a 有默认值

// 错误示例 (已注释): 中间有默认值, 右边没有
// void wrongExample2(int a, int b = 10, int c) { ... }
// 编译错误! c 没有默认值, 但左边的 b 有默认值

// ============================================================
// 3. 声明与定义分离: 默认值只指定一次
// ============================================================
// 规则: 默认参数值只在声明中指定, 定义中不要重复!

// 声明 (在头文件中) - 这里指定默认值
void printMessage(string msg, string prefix, string suffix) {
    // 定义中不要重复默认值!
    cout << "  " << prefix << " " << msg << " " << suffix << endl;
}

// 声明 (在头文件中) - 这里指定默认值
double calculateArea(double width, double height) {
    return width * height;
}

double calculateCircleArea(double radius, double pi) {
    return pi * radius * radius;
}

// ============================================================
// 4. 默认参数与函数重载的冲突
// ============================================================
// 当默认参数导致调用不明确时, 会与重载产生冲突

// 版本1: 一个参数
void display(int x) {
    cout << "  [一个参数] x=" << x << endl;
}

// 版本2: 两个参数, 第二个有默认值
void display(int x, int y = 10) {
    cout << "  [两个参数] x=" << x << ", y=" << y << endl;
}

// 注意: 上面两个函数如果同时存在, 调用 display(5) 时会产生二义性!
// 编译器不知道该调用哪个版本 (已在 main 中注释说明)

// ============================================================
// 5. 实际应用场景: 游戏角色创建
// ============================================================
struct Character {
    string name;
    int level;
    int health;
    int attack;
    string className;
};

// 创建角色函数, 使用默认参数简化调用
Character createCharacter(
    string name,
    string className = "战士",
    int level = 1,
    int health = 100,
    int attack = 10
) {
    Character c;
    c.name = name;
    c.className = className;
    c.level = level;
    c.health = health;
    c.attack = attack;
    return c;
}

// 打印角色信息
void printCharacter(const Character& c) {
    cout << "  ┌─────────────────────────┐" << endl;
    cout << "  │ 姓名: " << c.name << endl;
    cout << "  │ 职业: " << c.className << endl;
    cout << "  │ 等级: " << c.level << endl;
    cout << "  │ 生命: " << c.health << endl;
    cout << "  │ 攻击: " << c.attack << endl;
    cout << "  └─────────────────────────┘" << endl;
}

// ============================================================
// 6. 默认参数的值可以是表达式
// ============================================================
// 默认参数可以是常量、全局变量、甚至函数调用

int getDefaultValue() {
    return 42;
}

void testDefaultExpr(int x = 10 + 20) {  // 表达式作为默认值
    cout << "  x = " << x << endl;
}

void testDefaultFunc(int x = getDefaultValue()) {  // 函数调用作为默认值
    cout << "  x = " << x << endl;
}

// ============================================================
// main 函数
// ============================================================
int main() {

    // ----------------------------------------------------------
    // 1. 基本默认参数使用
    // ----------------------------------------------------------
    cout << "===== 1. 基本默认参数 =====" << endl;

    // 使用全部默认值
    printMessage("Hello");
    // 输出: [INFO] Hello ---

    // 覆盖 prefix
    printMessage("Warning", "[WARN]");
    // 输出: [WARN] Warning ---

    // 覆盖所有参数
    printMessage("Error", "[ERROR]", "!!!");
    // 输出: [ERROR] Error !!!

    // 计算面积
    cout << "\n  矩形面积 (5 x 3) = " << calculateArea(5.0, 3.0) << endl;
    cout << "  矩形面积 (5 x 默认) = " << calculateArea(5.0) << endl;
    cout << "  圆面积 (r=3) = " << calculateCircleArea(3.0) << endl;
    cout << "  圆面积 (r=3, pi精确) = " << calculateCircleArea(3.0, 3.14159265358979) << endl;

    // ----------------------------------------------------------
    // 2. 从右到左的默认值规则
    // ----------------------------------------------------------
    cout << "\n===== 2. 从右到左规则 =====" << endl;

    correctExample(1);          // a=1, b=10, c=20 (使用全部默认值)
    correctExample(1, 2);       // a=1, b=2,  c=20 (使用部分默认值)
    correctExample(1, 2, 3);    // a=1, b=2,  c=3  (不使用默认值)

    // ----------------------------------------------------------
    // 3. 声明与定义分离
    // ----------------------------------------------------------
    cout << "\n===== 3. 声明与定义分离 =====" << endl;
    cout << "  默认参数值在声明中指定, 定义中不重复" << endl;
    cout << "  声明: void printMessage(string msg, string prefix=\"[INFO]\", string suffix=\"---\");" << endl;
    cout << "  定义: void printMessage(string msg, string prefix, string suffix) { ... }" << endl;

    // ----------------------------------------------------------
    // 4. 默认参数与重载冲突
    // ----------------------------------------------------------
    cout << "\n===== 4. 默认参数与重载冲突 =====" << endl;

    // 注意: 以下代码如果取消注释会导致编译错误!
    // display(5);  // 二义性: 调用 display(int) 还是 display(int, int=10)?
    display(5, 20);  // 明确调用两个参数的版本

    cout << "  说明: 当有默认参数的函数与重载函数冲突时," << endl;
    cout << "  编译器可能无法确定调用哪个版本, 导致二义性错误。" << endl;
    cout << "  解决方法: 避免同时使用默认参数和重载。" << endl;

    // ----------------------------------------------------------
    // 5. 实际应用: 游戏角色创建
    // ----------------------------------------------------------
    cout << "\n===== 5. 实际应用: 游戏角色 =====" << endl;

    // 只指定必要参数, 其他使用默认值
    Character hero1 = createCharacter("张三");
    cout << "\n  新建角色 (全部默认):" << endl;
    printCharacter(hero1);

    // 指定部分参数
    Character hero2 = createCharacter("李四", "法师", 5);
    cout << "\n  新建角色 (指定职业和等级):" << endl;
    printCharacter(hero2);

    // 指定全部参数
    Character hero3 = createCharacter("王五", "刺客", 10, 200, 50);
    cout << "\n  新建角色 (全部指定):" << endl;
    printCharacter(hero3);

    // ----------------------------------------------------------
    // 6. 默认参数的值可以是表达式
    // ----------------------------------------------------------
    cout << "\n===== 6. 默认参数的值可以是表达式 =====" << endl;
    testDefaultExpr();        // 使用默认值: 10+20=30
    testDefaultExpr(100);     // 使用指定值: 100
    testDefaultFunc();        // 使用默认值: getDefaultValue()=42
    testDefaultFunc(200);     // 使用指定值: 200

    return 0;
}

/*
 * 总结:
 * 1. 默认参数简化函数调用, 减少重载数量
 * 2. 默认参数必须从右到左连续指定
 * 3. 默认值只在声明中指定, 定义中不重复
 * 4. 默认参数可能导致与重载的冲突 (二义性)
 * 5. 默认参数可以是常量、全局变量或函数调用
 * 建议: 简单场景用默认参数, 复杂场景用重载
 */
