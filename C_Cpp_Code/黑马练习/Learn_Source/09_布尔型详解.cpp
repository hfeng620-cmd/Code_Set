/*
 * ============================================================================
 * 主题: C++ 布尔型详解
 * ============================================================================
 * 布尔型 (bool) 是 C++ 中最简单的数据类型, 只有两个值:
 *   - true (真):  内部存储为 1
 *   - false (假): 内部存储为 0
 * 布尔类型常用于条件判断和逻辑运算。
 * ============================================================================
 */

#include <iostream>
#include <iomanip>   // 用于 boolalpha
using namespace std;

int main() {

    // ==================== bool 基本使用 ====================
    // bool 变量只有两个值: true 和 false
    // 占用 1 字节 (虽然只需要 1 位)
    cout << "===== bool 基本使用 =====" << endl;

    bool isStudent = true;     // 是学生
    bool isMarried = false;    // 已婚
    bool isRainy = true;       // 下雨

    cout << "isStudent = " << isStudent << endl;   // 输出 1
    cout << "isMarried = " << isMarried << endl;   // 输出 0
    cout << "isRainy = " << isRainy << endl;       // 输出 1
    cout << "bool 占用字节: " << sizeof(bool) << endl;  // 1 字节

    // ==================== cout 默认输出 0/1 ====================
    // cout 默认将 bool 输出为数字: true->1, false->0
    // 如果要输出 "true"/"false", 需要使用 boolalpha
    cout << "\n===== boolalpha 输出 =====" << endl;

    bool flag = true;
    cout << "默认输出: " << flag << endl;          // 输出 1
    cout << "boolalpha: " << boolalpha << flag << endl;  // 输出 true
    cout << "继续boolalpha: " << false << endl;    // 输出 false (boolalpha 持续生效)

    // 关闭 boolalpha
    cout << noboolalpha;
    cout << "关闭后: " << flag << endl;            // 输出 1

    // ==================== bool 与算术运算 ====================
    // 在算术运算中, true 等于 1, false 等于 0
    cout << "\n===== bool 参与算术运算 =====" << endl;

    bool a = true;   // 1
    bool b = false;  // 0

    cout << "true + true = " << (a + a) << endl;    // 1 + 1 = 2
    cout << "true + false = " << (a + b) << endl;   // 1 + 0 = 1
    cout << "true * 10 = " << (a * 10) << endl;     // 1 * 10 = 10
    cout << "false * 10 = " << (b * 10) << endl;    // 0 * 10 = 0
    cout << "true - true = " << (a - a) << endl;    // 1 - 1 = 0

    // int 转 bool: 0 为 false, 非 0 为 true
    cout << "\n===== int 转 bool =====" << endl;
    int num1 = 0;
    int num2 = 1;
    int num3 = -5;
    int num4 = 100;
    cout << boolalpha;
    cout << "0 -> bool: " << (bool)num1 << endl;    // false
    cout << "1 -> bool: " << (bool)num2 << endl;    // true
    cout << "-5 -> bool: " << (bool)num3 << endl;   // true
    cout << "100 -> bool: " << (bool)num4 << endl;  // true
    cout << noboolalpha;

    // bool 转 int
    cout << "\n===== bool 转 int =====" << endl;
    bool t = true;
    bool f = false;
    cout << "true -> int: " << (int)t << endl;     // 1
    cout << "false -> int: " << (int)f << endl;    // 0

    // ==================== 关系运算符 (返回 bool) ====================
    // 关系运算符的结果是 bool 类型
    cout << "\n===== 关系运算符 =====" << endl;

    int x = 10, y = 20;
    cout << boolalpha;
    cout << "10 == 20: " << (x == y) << endl;   // false (== 判断相等)
    cout << "10 != 20: " << (x != y) << endl;   // true  (!= 判断不等)
    cout << "10 > 20:  " << (x > y) << endl;    // false
    cout << "10 < 20:  " << (x < y) << endl;    // true
    cout << "10 >= 20: " << (x >= y) << endl;   // false
    cout << "10 <= 20: " << (x <= y) << endl;   // true

    // 常见错误: = 和 == 混淆
    // if (x = 5)   // 错误: 这是赋值, 不是比较! 应该用 ==
    // if (x == 5)  // 正确: 比较 x 是否等于 5

    // ==================== 逻辑运算符 (返回 bool) ====================
    cout << "\n===== 逻辑运算符 =====" << endl;

    bool p = true, q = false;

    // && (逻辑与): 两个都为 true 才为 true
    cout << "true && true: " << (true && true) << endl;     // true
    cout << "true && false: " << (true && false) << endl;   // false
    cout << "false && false: " << (false && false) << endl; // false

    // || (逻辑或): 至少一个为 true 就为 true
    cout << "true || false: " << (true || false) << endl;   // true
    cout << "false || false: " << (false || false) << endl; // false
    cout << "true || true: " << (true || true) << endl;     // true

    // ! (逻辑非): 取反
    cout << "!true: " << (!true) << endl;    // false
    cout << "!false: " << (!false) << endl;  // true

    // 组合使用
    int age = 25;
    bool hasTicket = true;
    bool canEnter = (age >= 18) && hasTicket;
    cout << boolalpha;
    cout << "年龄 " << age << ", 有票 " << hasTicket
         << ", 可以进入: " << canEnter << endl;

    // ==================== 条件判断中的 bool ====================
    cout << "\n===== 条件判断 =====" << endl;

    // if 语句的本质就是判断 bool 值
    bool isAdult = true;
    if (isAdult) {   // 等价于 if (isAdult == true)
        cout << "已成年" << endl;
    } else {
        cout << "未成年" << endl;
    }

    // 隐式 bool 转换: 非 0 为 true, 0 为 false
    int score = 85;
    if (score) {   // score 非 0, 所以条件为 true
        cout << "score 非零, 条件为真" << endl;
    }

    int zero = 0;
    if (!zero) {   // !0 = !false = true
        cout << "zero 为零, !zero 为真" << endl;
    }

    // ==================== 三元运算符 (返回值) ====================
    cout << "\n===== 三元运算符 =====" << endl;
    // 语法: 条件 ? 值1 : 值2
    // 条件为 true 返回值1, 否则返回值2

    int score2 = 75;
    string result = (score2 >= 60) ? "及格" : "不及格";
    cout << "成绩 " << score2 << ": " << result << endl;

    int a2 = 10, b2 = 20;
    int maxVal = (a2 > b2) ? a2 : b2;
    cout << a2 << " 和 " << b2 << " 中较大的是: " << maxVal << endl;

    // ==================== bool 的常见应用 ====================
    cout << "\n===== 常见应用 =====" << endl;

    // 应用1: 标记变量 (flag)
    bool found = false;
    int data[] = {3, 7, 12, 5, 9};
    int target = 12;
    for (int i = 0; i < 5; i++) {
        if (data[i] == target) {
            found = true;
            cout << "找到 " << target << " 在索引 " << i << endl;
            break;
        }
    }
    if (!found) {
        cout << "未找到 " << target << endl;
    }

    // 应用2: 状态管理
    bool isLoggedIn = false;
    cout << "登录状态: " << boolalpha << isLoggedIn << endl;
    isLoggedIn = true;   // 用户登录
    cout << "登录后状态: " << isLoggedIn << endl;
    isLoggedIn = false;  // 用户登出
    cout << "登出后状态: " << isLoggedIn << endl;

    // 应用3: 函数返回 bool
    // 例如判断一个数是否为偶数
    auto isEven = [](int n) -> bool { return n % 2 == 0; };
    cout << "4 是偶数? " << boolalpha << isEven(4) << endl;
    cout << "7 是偶数? " << isEven(7) << endl;

    // ==================== 常见错误 ====================
    cout << "\n===== 常见错误 =====" << endl;

    // 错误1: = 和 == 混淆
    bool flag2 = false;
    // if (flag2 = true)   // 赋值! 不是比较! 会进入 if 分支
    if (flag2 == true) {    // 正确的比较写法
        cout << "flag2 为 true" << endl;
    }

    // 错误2: 对 bool 使用 cout << boolalpha 后忘记关闭
    // boolalpha 是持久的, 会影响后续所有 bool 输出
    // 解决方法: 使用 noboolalpha 关闭

    // 错误3: 以为 bool 只占 1 位
    // 实际上 sizeof(bool) = 1 字节 = 8 位
    // 为了内存对齐和访问效率, 编译器用 1 字节存储

    // 错误4: 赋予非 0/1 的值
    // bool b = 42;  // 可以编译, 但 b 的值是 true (1), 不是 42
    bool weird = 42;
    cout << "bool = 42 的值: " << weird << endl;  // 输出 1 (true)

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. bool 只有两个值: true (1) 和 false (0)
 * 2. sizeof(bool) = 1 字节
 * 3. cout 默认输出 0/1, 用 boolalpha 输出 true/false
 * 4. 算术运算中 true=1, false=0
 * 5. int 转 bool: 0 为 false, 非 0 为 true
 * 6. 关系运算符 (==, !=, <, >) 返回 bool
 * 7. 逻辑运算符 (&&, ||, !) 用于组合条件
 * 8. 注意区分 = (赋值) 和 == (比较)
 * ===============================================
 */
