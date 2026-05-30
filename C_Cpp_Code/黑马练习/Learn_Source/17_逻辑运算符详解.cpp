/*
 * ============================================================
 *  逻辑运算符详解（全面扩展版）
 * ============================================================
 *  逻辑运算符用于组合多个条件表达式，返回布尔值 true/false。
 *    &&  逻辑与（AND）—— 两边都为 true 才为 true
 *    ||  逻辑或（OR）——  有一个为 true 就为 true
 *    !   逻辑非（NOT）—— 取反
 *
 *  真值表：
 *    a && b: 仅当 a 和 b 都为 true 时为 true
 *    a || b: 只要 a 或 b 有一个为 true 就为 true
 *    !a: true 变 false，false 变 true
 *
 *  核心特性：
 *    1. 短路求值：&& 左边为 false 时右边不执行；|| 左边为 true 时右边不执行
 *    2. 优先级：! > && > ||
 *    3. C++ 中非零值为 true，零值为 false
 *    4. 德摩根定律：简化复杂逻辑条件
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    cout << boolalpha;  // 输出 true/false 而不是 1/0

    // =========================================================
    // 一、逻辑与 &&：两边都为 true 才为 true
    // =========================================================
    cout << "===== 逻辑与 && =====" << endl;
    cout << "true  && true  = " << (true && true) << endl;    // true
    cout << "true  && false = " << (true && false) << endl;   // false
    cout << "false && true  = " << (false && true) << endl;   // false
    cout << "false && false = " << (false && false) << endl;  // false

    // 实际应用：判断范围
    int age = 25;
    if (age >= 18 && age <= 60) {
        cout << "年龄 " << age << " 在工作年龄范围内" << endl;
    }

    // 实际应用：多重条件验证
    string username = "admin";
    string password = "123456";
    if (username == "admin" && password == "123456") {
        cout << "登录成功！" << endl;
    }
    cout << endl;

    // =========================================================
    // 二、逻辑或 ||：有一个为 true 就为 true
    // =========================================================
    cout << "===== 逻辑或 || =====" << endl;
    cout << "true  || true  = " << (true || true) << endl;    // true
    cout << "true  || false = " << (true || false) << endl;   // true
    cout << "false || true  = " << (false || true) << endl;   // true
    cout << "false || false = " << (false || false) << endl;  // false

    // 实际应用：判断是否为周末
    int day = 6;
    if (day == 6 || day == 7) {
        cout << "今天是周末，可以休息！" << endl;
    }

    // 实际应用：判断是否为元音字母
    char ch = 'e';
    if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
        cout << "'" << ch << "' 是元音字母" << endl;
    }

    // 实际应用：判断是否为特殊字符
    char c = '@';
    if (c == '!' || c == '@' || c == '#' || c == '$') {
        cout << "'" << c << "' 是特殊字符" << endl;
    }
    cout << endl;

    // =========================================================
    // 三、逻辑非 !：取反
    // =========================================================
    cout << "===== 逻辑非 ! =====" << endl;
    cout << "!true  = " << (!true) << endl;    // false
    cout << "!false = " << (!false) << endl;   // true
    cout << "!!true  = " << (!!true) << endl;  // true（双重取反还原）

    // 非零值为 true，零值为 false
    cout << "!0 = " << (!0) << endl;     // true（0 是 false）
    cout << "!5 = " << (!5) << endl;     // false（非零是 true）
    cout << "!-1 = " << (!-1) << endl;   // false（非零是 true）

    // 实际应用
    bool isLoggedIn = false;
    if (!isLoggedIn) {
        cout << "用户未登录，请先登录！" << endl;
    }

    int number = 0;
    if (!number) {  // number 为 0 时 !number 为 true
        cout << "number 是 0（假值）" << endl;
    }

    // 使用 ! 简化条件
    int errorCode = 0;
    if (!errorCode) {  // 等价于 errorCode == 0
        cout << "没有错误" << endl;
    }
    cout << endl;

    // =========================================================
    // 四、完整真值表（所有组合）
    // =========================================================
    cout << "===== 完整真值表 =====" << endl;
    cout << "a\tb\ta&&b\ta||b\t!a\t!b\t!(a&&b)\t!(a||b)" << endl;
    cout << "--------------------------------------------------------------" << endl;
    bool vals[] = {true, false};
    for (bool a : vals) {
        for (bool b : vals) {
            cout << a << "\t" << b << "\t"
                 << (a && b) << "\t"
                 << (a || b) << "\t"
                 << (!a) << "\t"
                 << (!b) << "\t"
                 << (!(a && b)) << "\t"
                 << (!(a || b)) << endl;
        }
    }
    cout << endl;

    // =========================================================
    // 五、短路求值（Short-Circuit Evaluation）详解
    // =========================================================
    cout << "===== 短路求值 =====" << endl;
    // 核心规则：
    //   && 短路：左边为 false 时，右边不执行（结果必定为 false）
    //   || 短路：左边为 true 时，右边不执行（结果必定为 true）

    // --- && 短路示例 ---
    cout << "--- && 短路 ---" << endl;
    int x = 5;
    // x > 10 为 false，所以 ++x 不会执行
    bool r1 = (x > 10) && (++x > 0);
    cout << "&& 短路后 x = " << x << ", r1 = " << r1 << endl;  // x=5, r1=false

    // --- || 短路示例 ---
    cout << "--- || 短路 ---" << endl;
    int y = 5;
    // y < 10 为 true，所以 ++y 不会执行
    bool r2 = (y < 10) || (++y > 0);
    cout << "|| 短路后 y = " << y << ", r2 = " << r2 << endl;  // y=5, r2=true

    // --- 短路求值的实际应用 ---
    cout << "--- 短路保护应用 ---" << endl;

    // 应用1：避免除以零
    int divisor = 0;
    if (divisor != 0 && 10 / divisor > 2) {
        cout << "条件成立" << endl;
    } else {
        cout << "divisor 为 0，短路保护跳过了除法" << endl;
    }

    // 应用2：避免空指针访问
    int* ptr = nullptr;
    // ptr != nullptr 为 false，所以 *ptr 不会被访问
    if (ptr != nullptr && *ptr > 0) {
        cout << "指针指向正数" << endl;
    } else {
        cout << "指针为空，短路保护跳过了解引用" << endl;
    }

    // 应用3：级联条件（从简单到复杂）
    int arr[] = {10, 20, 30};
    int idx = 5;
    // 先检查索引范围，再访问数组（避免越界）
    if (idx >= 0 && idx < 3 && arr[idx] > 15) {
        cout << "arr[" << idx << "] > 15" << endl;
    } else {
        cout << "索引越界或条件不满足" << endl;
    }

    // 应用4：默认值（|| 短路）
    int userInput = 0;  // 用户未输入
    int defaultValue = 42;
    int value = (userInput != 0) ? userInput : defaultValue;
    // 或者用 || 的短路特性：
    int value2 = userInput || defaultValue;  // 注意：结果是 bool（1），不是数值
    cout << "userInput || defaultValue = " << value2 << endl;  // 1（不是 42！）
    cout << "警告：|| 返回 bool，不能用于数值默认值" << endl;
    cout << endl;

    // =========================================================
    // 六、德摩根定律（De Morgan's Laws）
    // =========================================================
    cout << "===== 德摩根定律 =====" << endl;
    // 定律1：!(a && b) ≡ (!a) || (!b)
    // 定律2：!(a || b) ≡ (!a) && (!b)
    // 记忆口诀："非与等于非或非，非或等于非与非"

    int m = 5, n = 10;

    // 验证定律1
    bool left1 = !(m > 3 && n > 8);
    bool right1 = (!(m > 3)) || (!(n > 8));
    cout << "!(m>3 && n>8) = " << left1 << endl;
    cout << "(!m>3 || !n>8) = " << right1 << endl;
    cout << "两者相等: " << (left1 == right1) << endl;

    // 验证定律2
    bool left2 = !(m > 3 || n > 8);
    bool right2 = (!(m > 3)) && (!(n > 8));
    cout << "!(m>3 || n>8) = " << left2 << endl;
    cout << "(!m>3 && !n>8) = " << right2 << endl;
    cout << "两者相等: " << (left2 == right2) << endl;

    // 实际应用：简化复杂条件
    // 原始条件：!(age >= 18 && age <= 60)
    // 应用德摩根定律：age < 18 || age > 60
    int testAge = 15;
    bool cond1 = !(testAge >= 18 && testAge <= 60);
    bool cond2 = (testAge < 18) || (testAge > 60);
    cout << "德摩根定律应用: " << (cond1 == cond2) << endl;
    cout << endl;

    // =========================================================
    // 七、非布尔值的逻辑运算
    // =========================================================
    cout << "===== 非布尔值的逻辑运算 =====" << endl;
    // 规则：非零值为 true，零值为 false
    cout << "5 && 3 = " << (5 && 3) << endl;     // true（都是非零）
    cout << "5 && 0 = " << (5 && 0) << endl;     // false（有一个为零）
    cout << "0 || 0 = " << (0 || 0) << endl;     // false（都是零）
    cout << "0 || 5 = " << (0 || 5) << endl;     // true（有一个非零）
    cout << "!0 = " << (!0) << endl;              // true
    cout << "!5 = " << (!5) << endl;              // false

    // 指针：nullptr 为 false，非空指针为 true
    int* pNull = nullptr;
    int val = 10;
    int* pNotNull = &val;
    cout << "nullptr || &val = " << (pNull || pNotNull) << endl;  // true

    // 注意：逻辑运算符的结果是 bool（0 或 1），不是操作数本身
    int result = 5 && 3;  // result = 1，不是 5 也不是 3
    cout << "5 && 3 = " << result << "（是 bool，不是数值）" << endl;
    cout << endl;

    // =========================================================
    // 八、优先级：! > && > ||
    // =========================================================
    cout << "===== 优先级 =====" << endl;
    bool p = true, q = false, r = true;
    // !p || q && r → (!p) || (q && r) → false || false → false
    cout << "!p || q && r = " << (!p || q && r) << endl;  // false
    // 建议用括号明确意图
    cout << "(!p) || (q && r) = " << ((!p) || (q && r)) << endl;

    // 复杂表达式拆解
    // a || b && c || d → a || (b && c) || d
    // 因为 && 优先级高于 ||
    cout << "true || false && false || true = "
         << (true || false && false || true) << endl;  // true
    cout << "true || (false && false) || true = "
         << (true || (false && false) || true) << endl;  // true（明确）
    cout << endl;

    // =========================================================
    // 九、逻辑运算符与比较运算符组合
    // =========================================================
    cout << "===== 逻辑与比较组合 =====" << endl;
    // 判断一个数是否在某个范围内
    int num = 50;
    // 判断是否在 (30, 70] 区间内
    if (num > 30 && num <= 70) {
        cout << num << " 在 (30, 70] 区间内" << endl;
    }

    // 判断是否为字母或数字
    char testChar = '5';
    if ((testChar >= 'A' && testChar <= 'Z') ||
        (testChar >= 'a' && testChar <= 'z') ||
        (testChar >= '0' && testChar <= '9')) {
        cout << "'" << testChar << "' 是字母或数字" << endl;
    }

    // 判断是否为闰年
    int year = 2024;
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    cout << year << (isLeap ? " 是闰年" : " 不是闰年") << endl;
    cout << endl;

    // =========================================================
    // 十、短路求值与副作用
    // =========================================================
    cout << "===== 短路与副作用 =====" << endl;
    // 短路求值意味着某些表达式可能不被执行
    // 如果表达式有副作用（修改变量），短路会影响结果

    int counter = 0;
    // && 短路：左边为 false，右边的 counter++ 不执行
    bool r3 = (false && ++counter > 0);
    cout << "false && ++counter 后 counter = " << counter << endl;  // 0

    // || 短路：左边为 true，右边的 counter++ 不执行
    counter = 0;
    bool r4 = (true || ++counter > 0);
    cout << "true || ++counter 后 counter = " << counter << endl;  // 0

    // 没有短路的情况：两边都会执行
    counter = 0;
    bool r5 = (true | ++counter > 0);  // | 是位运算，不短路
    cout << "true | ++counter 后 counter = " << counter << endl;  // 1
    cout << "警告：位运算 & | 不短路！逻辑运算 && || 才短路" << endl;
    cout << endl;

    // =========================================================
    // 十一、常见错误
    // =========================================================
    cout << "===== 常见错误 =====" << endl;

    // 错误1：除零错误（条件顺序错误）
    // 错误写法：if (10/x > 1 && x != 0)  → 先除法可能除以零！
    // 正确写法：if (x != 0 && 10/x > 1)  → 短路保护
    int divisor2 = 0;
    if (divisor2 != 0 && 10 / divisor2 > 1) {
        cout << "条件成立" << endl;
    } else {
        cout << "正确顺序：先检查除数不为零" << endl;
    }

    // 错误2：用 & | 代替 && ||
    // & | 是位运算符，不短路，且对 bool 值的结果可能不同
    bool a2 = true, b2 = false;
    cout << "true && false = " << (a2 && b2) << endl;  // false
    cout << "true & false  = " << (a2 & b2) << endl;   // false（结果相同但不短路）

    // 错误3：a || b && c 的优先级
    // 被解析为 a || (b && c)，不是 (a || b) && c
    // 建议加括号明确意图
    cout << "true || false && false = " << (true || false && false) << endl;  // true
    cout << "(true || false) && false = " << ((true || false) && false) << endl;  // false

    // 错误4：逻辑运算返回 bool 而不是数值
    // int val = 5 && 3;  → val = 1，不是 5

    // 错误5：连续比较不能用 && 简化
    // 错误：if (1 < x < 10)  → 被解析为 (1 < x) < 10 → true < 10 → 1 < 10 → true
    // 正确：if (x > 1 && x < 10)

    cout << endl;

    // =========================================================
    // 十二、练习题（TODO）
    // =========================================================
    /*
     * TODO 练习1：编写程序，输入年份和月份，判断该月有多少天
     *   提示：闰年2月29天，平年2月28天
     *   使用逻辑运算符组合多个条件
     *
     * TODO 练习2：编写程序，判断一个字符是否为大写元音字母
     *   使用 || 和 && 组合条件
     *
     * TODO 练习3：用德摩根定律简化以下条件：
     *   !(score >= 60 && score <= 100 && !isAbsent)
     *   写出等价的简化形式
     *
     * TODO 练习4：编写程序，输入三个整数，判断是否能构成三角形
     *   规则：任意两边之和大于第三边
     *
     * TODO 练习5：编写程序，实现一个简单的计算器
     *   输入两个数和运算符（+、-、*、/），输出结果
     *   用逻辑运算符处理除零和无效运算符的情况
     */

    cout << "===== 逻辑运算符演示结束 =====" << endl;
    return 0;
}

/*
 * ============================================================
 * 总结
 * ============================================================
 * 1. && 逻辑与：两边都为 true 才为 true
 * 2. || 逻辑或：有一个为 true 就为 true
 * 3. !  逻辑非：取反
 * 4. 短路求值：&& 左假短路，|| 左真短路
 * 5. 德摩根定律：!(a&&b) = !a||!b，!(a||b) = !a&&!b
 * 6. 优先级：! > && > ||，不确定时加括号
 * 7. 非零值为 true，零值为 false，但结果是 bool
 * 8. 位运算 & | 不短路，逻辑运算 && || 才短路
 * 9. 先检查安全条件（如非零、非空），再进行危险操作
 * ============================================================
 */
