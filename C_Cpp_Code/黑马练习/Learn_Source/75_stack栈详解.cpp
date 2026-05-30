/*
 * 文件名: 75_stack栈详解.cpp
 * 描述: stack栈适配器的详细用法
 *       包括LIFO原理、基本操作、底层容器、应用实例等
 * 编译: g++ -std=c++11 75_stack栈详解.cpp -o 75_stack栈详解
 */

#include <iostream>
#include <stack>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

// 打印栈内容的辅助函数（会清空栈，仅用于演示）
template<typename T>
void printAndClearStack(const string& title, stack<T> s) {  // 注意：按值传递，不影响原栈
    cout << title << " [size=" << s.size() << "]: ";
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;
}

int main() {
    cout << "========== 一、什么是栈？ ==========" << endl;

    /*
     * 栈 (Stack) 是一种后进先出 (LIFO: Last In, First Out) 的数据结构
     *
     * 想象一摞盘子:
     * - 放盘子只能放在最上面 (push)
     * - 取盘子只能从最上面取 (pop)
     * - 最后放的盘子最先被取走
     *
     * 栈只允许在一端（栈顶）进行操作，另一端（栈底）是封闭的
     *
     * 基本操作:
     * - push(x):  将元素x压入栈顶
     * - pop():    弹出栈顶元素（不返回值！）
     * - top():    返回栈顶元素的引用（不弹出）
     * - empty():  判断栈是否为空
     * - size():   返回栈中元素个数
     *
     * 注意: stack是容器适配器，不是独立的容器
     *       它底层使用其他容器（默认deque）来存储数据
     */

    cout << "stack是LIFO（后进先出）的数据结构" << endl;

    cout << "\n========== 二、stack基本操作 ==========" << endl;

    // 创建空栈
    stack<int> s;

    // 判断是否为空
    cout << "栈是否为空: " << (s.empty() ? "是" : "否") << endl;
    cout << "栈大小: " << s.size() << endl;

    // push: 压入元素到栈顶
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);
    s.push(50);
    printAndClearStack("push 10-50后", s);

    cout << "压入5个元素后:" << endl;
    cout << "栈大小: " << s.size() << endl;

    // top: 查看栈顶元素（不弹出）
    cout << "栈顶元素: " << s.top() << endl;

    // pop: 弹出栈顶元素（注意：pop不返回值！）
    s.pop();
    cout << "pop后栈顶元素: " << s.top() << endl;
    cout << "pop后栈大小: " << s.size() << endl;

    // 修改栈顶元素
    s.top() = 999;
    cout << "修改栈顶为999: " << s.top() << endl;

    // 清空栈
    while (!s.empty()) {
        s.pop();
    }
    cout << "清空后栈大小: " << s.size() << endl;

    cout << "\n========== 三、stack的底层容器 ==========" << endl;

    /*
     * stack是容器适配器，其模板定义为:
     * template<class T, class Container = deque<T>> class stack;
     *
     * 可以选择不同的底层容器:
     * - deque<T> (默认): 双端队列，两端操作都高效
     * - vector<T>: 动态数组，尾部操作高效
     * - list<T>:   双向链表，任意位置操作都高效
     *
     * 不能使用的底层容器:
     * - array: 固定大小，无法动态增长
     * - forward_list: 单向链表，没有push_back
     */

    // 使用deque作为底层容器（默认）
    stack<int> s1;  // 等价于 stack<int, deque<int>>

    // 使用vector作为底层容器
    stack<int, vector<int>> s2;
    s2.push(10);
    s2.push(20);
    cout << "vector栈顶: " << s2.top() << endl;

    // 使用list作为底层容器
    stack<int, list<int>> s3;
    s3.push(10);
    s3.push(20);
    cout << "list栈顶: " << s3.top() << endl;

    // 不同底层容器的性能差异:
    // deque (默认): push/pop都是O(1)，推荐使用
    // vector:       push/pop都是O(1)摊还，但偶尔需要重新分配
    // list:         push/pop都是O(1)，但每次分配一个节点

    cout << "\n========== 四、stack不支持迭代 ==========" << endl;

    /*
     * 重要: stack不提供迭代器！
     * 不能用for循环遍历栈中的元素
     * 这是栈的特性：只能访问栈顶
     *
     * 如果需要遍历，只能通过不断pop来访问
     * 但这样会清空栈！
     */
    stack<int> iter_stack;
    iter_stack.push(1);
    iter_stack.push(2);
    iter_stack.push(3);

    // iter_stack.begin();  // 错误！stack没有迭代器

    // 如果确实需要查看栈中所有元素，可以复制一份
    printAndClearStack("查看栈内容（使用副本）", iter_stack);
    cout << "原栈大小: " << iter_stack.size() << " (未改变)" << endl;

    cout << "\n========== 五、应用1：括号匹配 ==========" << endl;

    // 经典应用：检查括号是否匹配
    // 算法：遇到左括号压栈，遇到右括号弹栈检查是否匹配

    auto isBalanced = [](const string& expr) -> bool {
        stack<char> st;
        for (char ch : expr) {
            if (ch == '(' || ch == '[' || ch == '{') {
                st.push(ch);  // 左括号压栈
            } else if (ch == ')' || ch == ']' || ch == '}') {
                if (st.empty()) return false;  // 没有左括号匹配

                char top = st.top();
                st.pop();

                // 检查括号类型是否匹配
                if ((ch == ')' && top != '(') ||
                    (ch == ']' && top != '[') ||
                    (ch == '}' && top != '{')) {
                    return false;  // 括号类型不匹配
                }
            }
        }
        return st.empty();  // 栈为空说明全部匹配
    };

    // 测试括号匹配
    vector<string> tests = {
        "()",             // 匹配
        "()[]{}",         // 匹配
        "(]",             // 不匹配
        "([)]",           // 不匹配
        "{[]}",           // 匹配
        "((()))",         // 匹配
        "",               // 匹配（空字符串）
        "({[()]})",       // 匹配
        "(",              // 不匹配
        ")",              // 不匹配
    };

    for (const auto& test : tests) {
        cout << "\"" << test << "\" -> "
             << (isBalanced(test) ? "匹配" : "不匹配") << endl;
    }

    cout << "\n========== 六、应用2：字符串反转 ==========" << endl;

    // 使用栈反转字符串
    auto reverseString = [](const string& str) -> string {
        stack<char> st;

        // 将所有字符压入栈
        for (char ch : str) {
            st.push(ch);
        }

        // 依次弹出（后进先出，顺序反转）
        string reversed;
        while (!st.empty()) {
            reversed += st.top();
            st.pop();
        }
        return reversed;
    };

    string original = "Hello, World!";
    string reversed = reverseString(original);
    cout << "原始字符串: " << original << endl;
    cout << "反转字符串: " << reversed << endl;

    // 注意: 实际项目中直接用 std::reverse 更高效
    // 但栈反转是理解LIFO的经典示例

    cout << "\n========== 七、应用3：十进制转二进制 ==========" << endl;

    // 使用栈实现进制转换
    auto toBinary = [](int num) -> string {
        if (num == 0) return "0";

        stack<int> st;
        while (num > 0) {
            st.push(num % 2);  // 取余数压栈
            num /= 2;           // 整除2
        }

        // 弹出栈中元素（顺序反转得到二进制）
        string result;
        while (!st.empty()) {
            result += to_string(st.top());
            st.pop();
        }
        return result;
    };

    vector<int> numbers = {0, 1, 5, 10, 255, 1024};
    for (int n : numbers) {
        cout << n << " 的二进制: " << toBinary(n) << endl;
    }

    cout << "\n========== 八、应用4：表达式求值（简单版） ==========" << endl;

    // 使用栈计算后缀表达式（逆波兰表达式）
    // 后缀表达式: 操作符在操作数之后
    // 例如: "3 4 +" 等价于 "3 + 4"
    auto evalRPN = [](const vector<string>& tokens) -> int {
        stack<int> st;

        for (const string& token : tokens) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                // 遇到操作符，弹出两个操作数
                int b = st.top(); st.pop();
                int a = st.top(); st.pop();

                int result;
                if (token == "+") result = a + b;
                else if (token == "-") result = a - b;
                else if (token == "*") result = a * b;
                else result = a / b;

                st.push(result);
            } else {
                // 遇到操作数，压栈
                st.push(stoi(token));
            }
        }

        return st.top();
    };

    // 测试后缀表达式
    // "3 4 +" -> 7
    // "5 1 2 + 4 * + 3 -" -> 14 (等价于 5 + ((1+2)*4) - 3)
    vector<string> rpn1 = {"3", "4", "+"};
    vector<string> rpn2 = {"5", "1", "2", "+", "4", "*", "+", "3", "-"};

    cout << "后缀表达式 \"3 4 +\" = " << evalRPN(rpn1) << endl;
    cout << "后缀表达式 \"5 1 2 + 4 * + 3 -\" = " << evalRPN(rpn2) << endl;

    cout << "\n========== 九、stack比较操作 ==========" << endl;

    // stack支持关系运算符比较（基于底层容器的比较）
    stack<int> sa, sb;
    sa.push(1); sa.push(2); sa.push(3);
    sb.push(1); sb.push(2); sb.push(4);

    cout << "sa == sb: " << (sa == sb ? "true" : "false") << endl;
    cout << "sa < sb:  " << (sa < sb ? "true" : "false") << endl;
    cout << "sa > sb:  " << (sa > sb ? "true" : "false") << endl;

    cout << "\n========== 十、常见错误和注意事项 ==========" << endl;

    /*
     * 常见错误1: 对空栈调用top()或pop()
     *   这是未定义行为！
     *   正确做法: 先检查empty()
     */
    stack<int> empty_stack;
    // empty_stack.top();  // 未定义行为！
    // empty_stack.pop();  // 未定义行为！

    // 正确做法
    if (!empty_stack.empty()) {
        cout << "栈顶: " << empty_stack.top() << endl;
        empty_stack.pop();
    } else {
        cout << "栈为空，不能访问栈顶" << endl;
    }

    /*
     * 常见错误2: 期望pop()返回值
     *   pop()只删除栈顶元素，不返回值
     *   要获取栈顶值，先用top()再用pop()
     *
     *   错误: int val = s.pop();
     *   正确: int val = s.top(); s.pop();
     */

    /*
     * 常见错误3: 试图遍历栈
     *   stack没有迭代器，不能用for循环遍历
     *   如需遍历，只能复制后逐个pop
     */

    /*
     * 常见错误4: 选择不当的底层容器
     *   - vector: 尾部操作高效，但偶尔需要重新分配
     *   - deque (默认): 两端操作都高效，推荐使用
     *   - list: 每次分配一个节点，内存开销大
     */

    cout << "stack栈详解完毕！" << endl;

    return 0;
}
