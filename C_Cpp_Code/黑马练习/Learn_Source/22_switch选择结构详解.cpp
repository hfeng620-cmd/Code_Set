/*
 * ============================================================
 *  主题: switch 选择结构详解
 * ============================================================
 *  switch 语句用于多分支选择，适合对同一个变量进行多个
 *  固定值的判断。本文件涵盖:
 *    1. switch 基本语法: case, break, default
 *    2. 穿透 (fall-through) 行为
 *    3. 为什么 case 必须是常量表达式
 *    4. switch 与 if-else 的对比
 *    5. 枚举类型与 switch 的配合
 * ============================================================
 */

#include <iostream>
using namespace std;

int main()
{
    // ========================================
    // 1. switch 基本语法
    // ========================================
    // switch (表达式) { case 常量1: 语句; break; ... default: 语句; }
    // 表达式的值与每个 case 后的常量进行匹配
    // break 用于跳出 switch，否则会继续执行下一个 case
    // default 处理所有未匹配的情况 (类似 else)

    int day = 3;

    switch (day)
    {
        case 1:
            cout << "星期一" << endl;
            break;
        case 2:
            cout << "星期二" << endl;
            break;
        case 3:
            cout << "星期三" << endl;   // 匹配这个 case
            break;
        case 4:
            cout << "星期四" << endl;
            break;
        case 5:
            cout << "星期五" << endl;
            break;
        case 6:
            cout << "星期六" << endl;
            break;
        case 7:
            cout << "星期日" << endl;
            break;
        default:
            cout << "无效的日期" << endl;
            break;
    }

    // ========================================
    // 2. 穿透 (fall-through) 行为
    // ========================================
    // 如果 case 后没有 break，程序会继续执行下一个 case 的代码
    // 这叫做"穿透"或"贯穿"，有时是有意为之，有时是 bug

    int month = 8;
    int days = 0;

    switch (month)
    {
        case 1: case 3: case 5: case 7:
        case 8: case 10: case 12:       // 利用穿透: 多个 case 共用同一段代码
            days = 31;
            break;
        case 4: case 6: case 9: case 11:
            days = 30;
            break;
        case 2:
            days = 28;  // 简化处理，不考虑闰年
            break;
        default:
            days = -1;  // 无效月份
            break;
    }
    cout << month << " 月有 " << days << " 天" << endl;

    // 故意利用穿透的示例 (没有 break):
    int level = 2;
    int bonus = 0;

    switch (level)
    {
        case 3:
            bonus += 1000;   // level 3 额外奖励 1000 (穿透继续)
        case 2:
            bonus += 500;    // level 2 额外奖励 500  (穿透继续)
        case 1:
            bonus += 200;    // level 1 基础奖励 200
            break;
        default:
            bonus = 0;
            break;
    }
    // level=2 时，bonus = 500 + 200 = 700 (穿透了 case 2 和 case 1)
    cout << "level " << level << " 的奖金: " << bonus << endl;

    // ========================================
    // 3. 为什么 case 必须是常量表达式
    // ========================================
    // case 标签必须是编译时就能确定的整型常量 (int, char, enum 等)
    // 不能使用变量、浮点数或字符串

    const int CONST_A = 10;    // const 修饰的整型常量可以用
    // int var = 20;           // 变量不能用作 case 标签
    // double d = 1.5;         // 浮点数不能用作 case 标签

    int choice = 10;
    switch (choice)
    {
        case CONST_A:          // 合法: const int
            cout << "匹配 CONST_A" << endl;
            break;
        case 20:               // 合法: 字面量常量
            cout << "匹配 20" << endl;
            break;
        // case var:           // 非法! 编译错误
        //     break;
        // case 1.5:           // 非法! 浮点数不能用作 case
        //     break;
        default:
            cout << "其他值" << endl;
            break;
    }

    // ========================================
    // 4. switch 与 if-else 的对比
    // ========================================
    // switch 适合: 对单个变量进行多个离散值的判断
    // if-else 适合: 范围判断、复杂条件表达式

    // switch 不能直接用于范围判断，以下写法非法:
    // switch (score) {
    //     case >= 90:  // 非法语法!
    //         break;
    // }

    // 使用 if-else 处理范围:
    int score = 75;
    if (score >= 90)
        cout << "优秀" << endl;
    else if (score >= 60)
        cout << "及格" << endl;
    else
        cout << "不及格" << endl;

    // switch 的表达式结果必须是整型 (int, char, enum, short 等)
    // 不能是 float, double, string 等类型

    // ========================================
    // 5. 枚举类型与 switch 的配合
    // ========================================
    // 枚举值本质上是整数常量，非常适合与 switch 配合使用
    // 编译器还能检查 switch 是否覆盖了所有枚举值

    enum Color { RED = 0, GREEN = 1, BLUE = 2, YELLOW = 3 };
    Color color = GREEN;

    switch (color)
    {
        case RED:
            cout << "颜色: 红色" << endl;
            break;
        case GREEN:
            cout << "颜色: 绿色" << endl;  // 输出这个
            break;
        case BLUE:
            cout << "颜色: 蓝色" << endl;
            break;
        case YELLOW:
            cout << "颜色: 黄色" << endl;
            break;
        // 如果省略某个 case，编译器可能警告未处理的枚举值
    }

    // ========================================
    // 6. switch 中的变量声明问题
    // ========================================
    // 在 switch 的某个 case 中声明变量可能有问题
    // 如果跳过了变量的初始化，编译器可能报错

    int option = 2;
    switch (option)
    {
        case 1:
        {
            // 在 case 中声明变量，建议用花括号限定作用域
            int localVar = 100;
            cout << "case 1, localVar = " << localVar << endl;
            break;
        }
        case 2:
        {
            int localVar = 200;   // 不同的 case 用不同的花括号，避免冲突
            cout << "case 2, localVar = " << localVar << endl;
            break;
        }
        default:
            cout << "default case" << endl;
            break;
    }

    // ========================================
    // 7. 实际应用: 简易计算器
    // ========================================
    double num1 = 10.0, num2 = 3.0;
    char op = '+';

    switch (op)
    {
        case '+':
            cout << num1 << " + " << num2 << " = " << (num1 + num2) << endl;
            break;
        case '-':
            cout << num1 << " - " << num2 << " = " << (num1 - num2) << endl;
            break;
        case '*':
            cout << num1 << " * " << num2 << " = " << (num1 * num2) << endl;
            break;
        case '/':
            if (num2 != 0)
                cout << num1 << " / " << num2 << " = " << (num1 / num2) << endl;
            else
                cout << "错误: 除数不能为零!" << endl;
            break;
        default:
            cout << "不支持的运算符: " << op << endl;
            break;
    }

    cout << endl << "=== switch 选择结构详解完毕 ===" << endl;
    return 0;
}
