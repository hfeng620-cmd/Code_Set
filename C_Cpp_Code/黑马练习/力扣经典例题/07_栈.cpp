/**
 * 07_栈 - LeetCode 经典例题
 *
 * 本文件包含 10 道经典的栈相关算法题：
 * 1. 有效的括号 (LeetCode 20, Easy)
 * 2. 最小栈 (LeetCode 155, Medium)
 * 3. 每日温度 (LeetCode 739, Medium)
 * 4. 柱状图中最大的矩形 (LeetCode 84, Hard)
 * 5. 基本计算器 II (LeetCode 227, Medium)
 * 6. 下一个更大元素 (LeetCode 496, Easy)
 * 7. 股票跨度 (LeetCode 901, Medium)
 * 8. 字符串解码 (LeetCode 394, Medium)
 * 9. 比较含退格的字符串 (LeetCode 844, Easy)
 * 10. 逆波兰表达式求值 (LeetCode 150, Medium)
 *
 * 核心思想：栈是一种后进先出 (LIFO) 的数据结构，
 * 适合处理需要"回溯"或"配对"的问题。
 */

#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <climits>
using namespace std;

// ============================================================================
// 题目 1: 有效的括号 (LeetCode 20, Easy)
// ============================================================================
/**
 * 题目描述：
 * 给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串 s，
 * 判断字符串是否有效。
 *
 * 有效字符串需满足：
 * 1. 左括号必须用相同类型的右括号闭合。
 * 2. 左括号必须以正确的顺序闭合。
 * 3. 每个右括号都有一个对应的相同类型的左括号。
 *
 * 示例：
 * 输入: s = "()"      输出: true
 * 输入: s = "()[]{}"  输出: true
 * 输入: s = "(]"      输出: false
 * 输入: s = "([)]"    输出: false
 * 输入: s = "{[]}"    输出: true
 */

// 方法一：栈 + 哈希表匹配（推荐）
// 思路：遍历字符串，遇到左括号入栈，遇到右括号检查栈顶是否匹配
// 时间复杂度: O(n)  空间复杂度: O(n)
bool isValid(string s) {
    // 奇数长度的字符串一定无效
    if (s.length() % 2 != 0) return false;

    // 使用哈希表存储括号配对关系
    unordered_map<char, char> bracketMap = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };

    stack<char> st;

    for (char c : s) {
        // 如果是右括号
        if (bracketMap.count(c)) {
            // 栈为空或栈顶不匹配，则无效
            if (st.empty() || st.top() != bracketMap[c]) {
                return false;
            }
            st.pop();  // 匹配成功，弹出栈顶
        } else {
            // 左括号入栈
            st.push(c);
        }
    }

    // 栈为空说明所有括号都匹配
    return st.empty();
}

// 方法二：简化版（不用哈希表）
// 思路：遇到左括号时直接将对应的右括号入栈
// 时间复杂度: O(n)  空间复杂度: O(n)
bool isValid_v2(string s) {
    if (s.length() % 2 != 0) return false;

    stack<char> st;

    for (char c : s) {
        if (c == '(') {
            st.push(')');   // 入栈对应的右括号
        } else if (c == '[') {
            st.push(']');
        } else if (c == '{') {
            st.push('}');
        } else {
            // 当前字符是右括号，检查栈顶
            if (st.empty() || st.top() != c) {
                return false;
            }
            st.pop();
        }
    }

    return st.empty();
}

void test_problem1() {
    cout << "========================================" << endl;
    cout << "题目 1: 有效的括号 (LeetCode 20)" << endl;
    cout << "========================================" << endl;

    // 测试用例
    vector<pair<string, bool>> testCases = {
        {"()", true},
        {"()[]{}", true},
        {"(]", false},
        {"([)]", false},
        {"{[]}", true},
        {"", true},
        {"((", false},
        {"))", false},
        {"((()))", true},
        {"{[()]}", true}
    };

    for (auto& [s, expected] : testCases) {
        bool result = isValid(s);
        cout << "输入: \"" << s << "\" 期望: " << (expected ? "true" : "false")
             << " 结果: " << (result ? "true" : "false")
             << (result == expected ? " [通过]" : " [失败]") << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目 2: 最小栈 (LeetCode 155, Medium)
// ============================================================================
/**
 * 题目描述：
 * 设计一个支持 push，pop，top 操作，并能在常数时间内检索到最小元素的栈。
 *
 * 实现 MinStack 类:
 * - MinStack() 初始化堆栈对象。
 * - void push(int val) 将元素推入堆栈。
 * - void pop() 删除堆栈顶部的元素。
 * - int top() 获取堆栈顶部的元素。
 * - int getMin() 获取堆栈中的最小元素。
 *
 * 思路分析：
 * 关键难点在于如何在 O(1) 时间内获取最小值。
 * 如果只用一个变量记录最小值，当最小值被弹出后，无法快速知道新的最小值。
 */

// 方法一：辅助栈（两个栈）
// 用一个栈存数据，另一个栈同步存当前的最小值
// 时间复杂度: 所有操作 O(1)  空间复杂度: O(n)
class MinStack_TwoStacks {
private:
    stack<int> dataStack;   // 数据栈
    stack<int> minStack;    // 辅助栈，栈顶始终是当前最小值

public:
    MinStack_TwoStacks() {}

    void push(int val) {
        dataStack.push(val);
        // 辅助栈为空或 val 小于等于当前最小值时入栈
        // 注意：这里用 <= 是为了处理重复的最小值
        if (minStack.empty() || val <= minStack.top()) {
            minStack.push(val);
        }
    }

    void pop() {
        // 如果弹出的值等于最小值，辅助栈也要弹出
        if (dataStack.top() == minStack.top()) {
            minStack.pop();
        }
        dataStack.pop();
    }

    int top() {
        return dataStack.top();
    }

    int getMin() {
        return minStack.top();
    }
};

// 方法二：单栈存储 pair（值, 当前最小值）
// 每个元素存储为 (value, min_so_far)
// 时间复杂度: 所有操作 O(1)  空间复杂度: O(n)
class MinStack_Pairs {
private:
    // pair 的 first 存储值，second 存储当前栈中的最小值
    stack<pair<int, int>> st;

public:
    MinStack_Pairs() {}

    void push(int val) {
        if (st.empty()) {
            st.push({val, val});  // 第一个元素，最小值就是自己
        } else {
            // 当前最小值 = min(val, 之前的最小值)
            int currentMin = min(val, st.top().second);
            st.push({val, currentMin});
        }
    }

    void pop() {
        st.pop();
    }

    int top() {
        return st.top().first;
    }

    int getMin() {
        return st.top().second;
    }
};

void test_problem2() {
    cout << "========================================" << endl;
    cout << "题目 2: 最小栈 (LeetCode 155)" << endl;
    cout << "========================================" << endl;

    // 测试方法一：双栈
    cout << "方法一：辅助栈" << endl;
    MinStack_TwoStacks minStack1;
    minStack1.push(-2);
    minStack1.push(0);
    minStack1.push(-3);
    cout << "push(-2), push(0), push(-3)" << endl;
    cout << "getMin() = " << minStack1.getMin() << " (期望: -3)" << endl;
    minStack1.pop();
    cout << "pop() 后 top() = " << minStack1.top() << " (期望: 0)" << endl;
    cout << "getMin() = " << minStack1.getMin() << " (期望: -2)" << endl;

    cout << endl;

    // 测试方法二：pair 栈
    cout << "方法二：pair 栈" << endl;
    MinStack_Pairs minStack2;
    minStack2.push(5);
    minStack2.push(3);
    minStack2.push(7);
    cout << "push(5), push(3), push(7)" << endl;
    cout << "getMin() = " << minStack2.getMin() << " (期望: 3)" << endl;
    minStack2.pop();
    cout << "pop() 后 getMin() = " << minStack2.getMin() << " (期望: 3)" << endl;
    minStack2.pop();
    cout << "pop() 后 getMin() = " << minStack2.getMin() << " (期望: 5)" << endl;
    cout << endl;
}

// ============================================================================
// 题目 3: 每日温度 (LeetCode 739, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给定一个整数数组 temperatures，表示每天的温度，
 * 返回一个数组 answer，其中 answer[i] 是指对于第 i 天，
 * 下一个更高温度出现在几天后。
 * 如果气温在这之后都不会升高，请在该位置用 0 来代替。
 *
 * 示例：
 * 输入: temperatures = [73,74,75,71,69,72,76,73]
 * 输出: [1,1,4,2,1,1,0,0]
 *
 * 单调栈概念：
 * 单调栈是一种特殊的栈结构，栈中的元素保持单调递增或单调递减的顺序。
 * 常用于解决"下一个更大/更小元素"类问题。
 * 本题使用单调递减栈：栈底到栈顶温度递减。
 */

// 方法一：暴力法
// 对每个元素，向后搜索第一个比它大的元素
// 时间复杂度: O(n²)  空间复杂度: O(1)（不算返回数组）
vector<int> dailyTemperatures_brute(vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> answer(n, 0);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (temperatures[j] > temperatures[i]) {
                answer[i] = j - i;  // 记录天数差
                break;
            }
        }
    }

    return answer;
}

// 方法二：单调栈（推荐）
// 维护一个单调递减栈，栈中存储温度的下标
// 当遇到更高温度时，弹出栈中所有较低温度并记录天数差
// 时间复杂度: O(n)  空间复杂度: O(n)
vector<int> dailyTemperatures(vector<int>& temperatures) {
    int n = temperatures.size();
    vector<int> answer(n, 0);
    stack<int> st;  // 存储下标，栈中下标对应的温度是单调递减的

    for (int i = 0; i < n; i++) {
        // 当前温度高于栈顶对应的温度时，弹出并记录
        while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
            int prevIndex = st.top();
            st.pop();
            answer[prevIndex] = i - prevIndex;  // 天数差
        }
        st.push(i);  // 当前下标入栈
    }

    // 栈中剩余的元素表示后面没有更高温度，保持默认值 0
    return answer;
}

void test_problem3() {
    cout << "========================================" << endl;
    cout << "题目 3: 每日温度 (LeetCode 739)" << endl;
    cout << "========================================" << endl;

    vector<int> temps1 = {73, 74, 75, 71, 69, 72, 76, 73};
    vector<int> expected1 = {1, 1, 4, 2, 1, 1, 0, 0};

    cout << "输入: [73,74,75,71,69,72,76,73]" << endl;

    auto result1 = dailyTemperatures_brute(temps1);
    cout << "暴力法:  [";
    for (int i = 0; i < result1.size(); i++) {
        cout << result1[i] << (i < result1.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    auto result2 = dailyTemperatures(temps1);
    cout << "单调栈:  [";
    for (int i = 0; i < result2.size(); i++) {
        cout << result2[i] << (i < result2.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    // 测试用例 2
    vector<int> temps2 = {30, 40, 50, 60};
    cout << "\n输入: [30,40,50,60]" << endl;
    auto result3 = dailyTemperatures(temps2);
    cout << "单调栈:  [";
    for (int i = 0; i < result3.size(); i++) {
        cout << result3[i] << (i < result3.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    // 测试用例 3
    vector<int> temps3 = {30, 60, 90};
    cout << "\n输入: [30,60,90]" << endl;
    auto result4 = dailyTemperatures(temps3);
    cout << "单调栈:  [";
    for (int i = 0; i < result4.size(); i++) {
        cout << result4[i] << (i < result4.size() - 1 ? "," : "");
    }
    cout << "]" << endl;
    cout << endl;
}

// ============================================================================
// 题目 4: 柱状图中最大的矩形 (LeetCode 84, Hard)
// ============================================================================
/**
 * 题目描述：
 * 给定 n 个非负整数，用来表示柱状图中各个柱子的高度。
 * 每个柱子彼此相邻，且宽度为 1。
 * 求在该柱状图中，能够勾勒出来的矩形的最大面积。
 *
 * 示例：
 * 输入: heights = [2,1,5,6,2,3]
 * 输出: 10
 * 解释: 最大矩形是高度 [5,6] 对应的区域，面积 = 5 * 2 = 10
 *
 * 核心思想：
 * 对于每根柱子，找到左右两边第一个比它矮的柱子，
 * 则以该柱子高度为矩形高度的最大宽度 = 右边界 - 左边界 - 1
 */

// 方法一：暴力法
// 对每根柱子，向左右扩展找到最大宽度
// 时间复杂度: O(n²)  空间复杂度: O(1)
int largestRectangleArea_brute(vector<int>& heights) {
    int n = heights.size();
    int maxArea = 0;

    for (int i = 0; i < n; i++) {
        int height = heights[i];

        // 向左扩展
        int left = i;
        while (left > 0 && heights[left - 1] >= height) {
            left--;
        }

        // 向右扩展
        int right = i;
        while (right < n - 1 && heights[right + 1] >= height) {
            right++;
        }

        int width = right - left + 1;
        maxArea = max(maxArea, height * width);
    }

    return maxArea;
}

// 方法二：单调栈（推荐）
// 维护一个单调递增栈，栈中存储柱子的下标
// 当遇到更矮的柱子时，计算栈中柱子的最大矩形面积
// 时间复杂度: O(n)  空间复杂度: O(n)
int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    stack<int> st;  // 单调递增栈，存储下标
    int maxArea = 0;

    // 在末尾添加一个高度为 0 的哨兵，确保所有柱子都能被处理
    heights.push_back(0);

    for (int i = 0; i <= n; i++) {
        // 当前柱子比栈顶柱子矮时，弹出并计算面积
        while (!st.empty() && heights[i] < heights[st.top()]) {
            int height = heights[st.top()];
            st.pop();

            // 计算宽度：右边界是 i，左边界是新的栈顶
            int width = st.empty() ? i : (i - st.top() - 1);
            maxArea = max(maxArea, height * width);
        }
        st.push(i);
    }

    // 恢复原数组（移除哨兵）
    heights.pop_back();

    return maxArea;
}

void test_problem4() {
    cout << "========================================" << endl;
    cout << "题目 4: 柱状图中最大的矩形 (LeetCode 84)" << endl;
    cout << "========================================" << endl;

    // 测试用例 1
    vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    cout << "输入: [2,1,5,6,2,3]" << endl;
    cout << "暴力法结果: " << largestRectangleArea_brute(heights1)
         << " (期望: 10)" << endl;
    cout << "单调栈结果: " << largestRectangleArea(heights1)
         << " (期望: 10)" << endl;

    // 测试用例 2
    vector<int> heights2 = {2, 4};
    cout << "\n输入: [2,4]" << endl;
    cout << "暴力法结果: " << largestRectangleArea_brute(heights2)
         << " (期望: 4)" << endl;
    cout << "单调栈结果: " << largestRectangleArea(heights2)
         << " (期望: 4)" << endl;

    // 测试用例 3
    vector<int> heights3 = {1, 1, 1, 1, 1};
    cout << "\n输入: [1,1,1,1,1]" << endl;
    cout << "暴力法结果: " << largestRectangleArea_brute(heights3)
         << " (期望: 5)" << endl;
    cout << "单调栈结果: " << largestRectangleArea(heights3)
         << " (期望: 5)" << endl;

    // 测试用例 4
    vector<int> heights4 = {3, 5, 1, 7, 5, 9};
    cout << "\n输入: [3,5,1,7,5,9]" << endl;
    cout << "暴力法结果: " << largestRectangleArea_brute(heights4)
         << " (期望: 15)" << endl;
    cout << "单调栈结果: " << largestRectangleArea(heights4)
         << " (期望: 15)" << endl;
    cout << endl;
}

// ============================================================================
// 题目 5: 基本计算器 II (LeetCode 227, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。
 * 整数除法仅保留整数部分。
 *
 * 你可以假设给定的表达式总是有效的。所有中间结果将在 [-2^31, 2^31 - 1] 范围内。
 *
 * 注意：不允许使用任何将字符串作为数学表达式计算的内置函数。
 *
 * 示例：
 * 输入: s = "3+2*2"     输出: 7
 * 输入: s = " 3/2 "     输出: 1
 * 输入: s = " 3+5 / 2 " 输出: 5
 *
 * 核心思想：
 * 使用栈处理运算符优先级。遇到 + 或 - 将数字入栈，
 * 遇到 * 或 / 立即计算（因为优先级高）。
 */

// 方法一：栈处理优先级（推荐）
// 时间复杂度: O(n)  空间复杂度: O(n)
int calculate(string s) {
    stack<int> st;       // 存储数字
    int num = 0;         // 当前正在解析的数字
    char op = '+';       // 上一个运算符，初始化为 '+'

    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        // 解析数字（可能是多位数）
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
        }

        // 遇到运算符或到达字符串末尾时处理
        if ((!isdigit(c) && c != ' ') || i == s.length() - 1) {
            // 根据上一个运算符决定如何处理数字
            switch (op) {
                case '+':
                    st.push(num);    // 正数直接入栈
                    break;
                case '-':
                    st.push(-num);   // 负数取反后入栈
                    break;
                case '*':
                    // 乘法：弹出栈顶，与当前数相乘后入栈
                    {
                        int top = st.top();
                        st.pop();
                        st.push(top * num);
                    }
                    break;
                case '/':
                    // 除法：弹出栈顶，与当前数相除后入栈
                    {
                        int top = st.top();
                        st.pop();
                        st.push(top / num);
                    }
                    break;
            }

            // 更新运算符，重置数字
            op = c;
            num = 0;
        }
    }

    // 累加栈中所有数字
    int result = 0;
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }

    return result;
}

// 方法二：不用栈，用变量直接计算
// 记录上一个数字 prevNum，遇到 * 或 / 立即计算
// 时间复杂度: O(n)  空间复杂度: O(1)
int calculate_v2(string s) {
    int result = 0;    // 最终结果
    int prevNum = 0;   // 上一个数字（用于 * 和 /）
    int curNum = 0;    // 当前数字
    char op = '+';     // 上一个运算符

    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        if (isdigit(c)) {
            curNum = curNum * 10 + (c - '0');
        }

        if ((!isdigit(c) && c != ' ') || i == s.length() - 1) {
            switch (op) {
                case '+':
                    result += prevNum;   // 把之前的数字加入结果
                    prevNum = curNum;    // 当前数字成为新的 prevNum
                    break;
                case '-':
                    result += prevNum;
                    prevNum = -curNum;   // 负数
                    break;
                case '*':
                    prevNum = prevNum * curNum;  // 立即计算
                    break;
                case '/':
                    prevNum = prevNum / curNum;  // 立即计算
                    break;
            }

            op = c;
            curNum = 0;
        }
    }

    result += prevNum;  // 别忘了最后一个数字
    return result;
}

void test_problem5() {
    cout << "========================================" << endl;
    cout << "题目 5: 基本计算器 II (LeetCode 227)" << endl;
    cout << "========================================" << endl;

    // 测试用例
    vector<pair<string, int>> testCases = {
        {"3+2*2", 7},
        {" 3/2 ", 1},
        {" 3+5 / 2 ", 5},
        {"42", 42},
        {"1+1+1", 3},
        {"2*3+4", 10},
        {"2+3*4", 14},
        {"10-3*2", 4},
        {"14/3*2", 8},
        {" 100 + 200 * 3 / 2 ", 400}
    };

    for (auto& [expr, expected] : testCases) {
        int result1 = calculate(expr);
        int result2 = calculate_v2(expr);
        cout << "输入: \"" << expr << "\" 期望: " << expected
             << " 栈方法: " << result1
             << " 变量方法: " << result2
             << (result1 == expected && result2 == expected ? " [通过]" : " [失败]")
             << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目 6: 下一个更大元素 (LeetCode 496, Easy)
// ============================================================================
/**
 * 题目描述：
 * 给你两个没有重复元素的数组 nums1 和 nums2，其中 nums1 是 nums2 的子集。
 * 请你找出 nums1 中每个元素在 nums2 中的下一个比它的更大元素。
 * 如果不存在，返回 -1。
 *
 * "下一个更大元素"定义：对于 nums1 中的数字 x，在 nums2 中找到第一个比 x 大的数字。
 *
 * 示例：
 * 输入: nums1 = [4,1,2], nums2 = [1,3,4,2]
 * 输出: [-1,3,-1]
 * 解释: nums1 中 4 在 nums2 中找不到更大的; 1 的下一个是 3; 2 找不到更大的
 *
 * 输入: nums1 = [2,4], nums2 = [1,2,3,4]
 * 输出: [3,-1]
 *
 * 核心思想：
 * 单调栈 + 哈希表：从右到左遍历 nums2，维护单调递减栈。
 * 对于每个元素，栈中第一个比它大的元素就是"下一个更大元素"。
 */

// 方法一：单调栈（从右到左遍历）
// 思路：遍历 nums2 从右到左，维护一个单调递减栈
//       每个元素出栈时，记录它的"下一个更大元素"
// 时间复杂度: O(n)  空间复杂度: O(n)
vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
    unordered_map<int, int> nextGreater;  // 记录每个元素的下一个更大元素
    stack<int> st;  // 单调递减栈

    // 从右到左遍历 nums2
    for (int i = nums2.size() - 1; i >= 0; i--) {
        // 弹出所有小于等于当前元素的栈中元素
        while (!st.empty() && st.top() <= nums2[i]) {
            st.pop();
        }

        // 栈顶元素就是当前元素的下一个更大元素
        // 如果栈为空，说明右边没有更大的元素
        nextGreater[nums2[i]] = st.empty() ? -1 : st.top();

        // 当前元素入栈
        st.push(nums2[i]);
    }

    // 根据 nums1 查找结果
    vector<int> result;
    for (int num : nums1) {
        result.push_back(nextGreater[num]);
    }

    return result;
}

// 方法二：暴力法（对比用）
// 对每个 nums1 中的元素，在 nums2 中找到该元素位置，然后向右搜索更大的
// 时间复杂度: O(m*n)  空间复杂度: O(1)
vector<int> nextGreaterElement_brute(vector<int>& nums1, vector<int>& nums2) {
    vector<int> result;

    for (int num : nums1) {
        int next = -1;
        bool found = false;
        for (int j = 0; j < nums2.size(); j++) {
            if (nums2[j] == num) {
                found = true;
            } else if (found && nums2[j] > num) {
                next = nums2[j];
                break;
            }
        }
        result.push_back(next);
    }

    return result;
}

void test_problem6() {
    cout << "========================================" << endl;
    cout << "题目 6: 下一个更大元素 (LeetCode 496)" << endl;
    cout << "========================================" << endl;

    // 测试用例 1
    vector<int> nums1 = {4, 1, 2};
    vector<int> nums2 = {1, 3, 4, 2};
    auto result = nextGreaterElement(nums1, nums2);
    cout << "nums1=[4,1,2], nums2=[1,3,4,2]" << endl;
    cout << "单调栈: [";
    for (int i = 0; i < result.size(); i++)
        cout << result[i] << (i < result.size() - 1 ? "," : "");
    cout << "] (期望: [-1,3,-1])" << endl;

    // 测试用例 2
    vector<int> nums3 = {2, 4};
    vector<int> nums4 = {1, 2, 3, 4};
    auto result2 = nextGreaterElement(nums3, nums4);
    cout << "\nnums1=[2,4], nums2=[1,2,3,4]" << endl;
    cout << "单调栈: [";
    for (int i = 0; i < result2.size(); i++)
        cout << result2[i] << (i < result2.size() - 1 ? "," : "");
    cout << "] (期望: [3,-1])" << endl;

    // 测试用例 3：全部递减
    vector<int> nums5 = {3, 2, 1};
    vector<int> nums6 = {3, 2, 1};
    auto result3 = nextGreaterElement(nums5, nums6);
    cout << "\nnums1=[3,2,1], nums2=[3,2,1]" << endl;
    cout << "单调栈: [";
    for (int i = 0; i < result3.size(); i++)
        cout << result3[i] << (i < result3.size() - 1 ? "," : "");
    cout << "] (期望: [-1,-1,-1])" << endl;

    // 测试用例 4
    vector<int> nums7 = {1, 3, 5, 2, 4};
    vector<int> nums8 = {6, 5, 4, 3, 2, 1, 7};
    auto result4 = nextGreaterElement(nums7, nums8);
    cout << "\nnums1=[1,3,5,2,4], nums2=[6,5,4,3,2,1,7]" << endl;
    cout << "单调栈: [";
    for (int i = 0; i < result4.size(); i++)
        cout << result4[i] << (i < result4.size() - 1 ? "," : "");
    cout << "] (期望: [7,7,7,7,7])" << endl;
    cout << endl;
}

// ============================================================================
// 题目 7: 股票跨度 (LeetCode 901, Medium)
// ============================================================================
/**
 * 题目描述：
 * 编写一个 StockSpanner 类，它收集某些股票的每日报价，并返回该股票当日价格的跨度。
 * "跨度"定义为：今天（含今天）之前连续多少天的价格不超过今天的价格。
 *
 * 例如，如果未来7天股票的价格是 [100, 80, 60, 70, 60, 75, 85]，
 * 那么股票跨度将是 [1, 1, 1, 2, 1, 4, 6]。
 *
 * 示例：
 * 输入: ["StockSpanner","next","next","next","next","next","next","next"],
 *       [[],[100],[80],[60],[70],[60],[75],[85]]
 * 输出: [null,1,1,1,2,1,4,6]
 *
 * 核心思想：
 * 单调递减栈，存储 (price, span) 对。
 * 对于每次 next(price)，弹出所有小于等于 price 的栈元素，
 * 将它们的 span 累加到当前 span 上。
 */

// StockSpanner 类：单调栈存储 (价格, 跨度) 对
// 时间复杂度: 均摊 O(1) per next call  空间复杂度: O(n)
class StockSpanner {
private:
    // 栈中存储 (price, span) 对，价格从栈底到栈顶单调递减
    stack<pair<int, int>> st;

public:
    StockSpanner() {}

    int next(int price) {
        int span = 1;  // 至少包含今天自己

        // 弹出所有价格 <= 当前价格的栈元素，累加它们的跨度
        while (!st.empty() && st.top().first <= price) {
            span += st.top().second;
            st.pop();
        }

        // 当前价格和跨度入栈
        st.push({price, span});

        return span;
    }
};

// 暴力法（对比用）：对每次 next，向后遍历已存储的价格
// 时间复杂度: O(n) per call
class StockSpanner_Brute {
private:
    vector<int> prices;

public:
    StockSpanner_Brute() {}

    int next(int price) {
        prices.push_back(price);
        int span = 0;

        // 从当前位置向前遍历，统计连续不超过当前价格的天数
        for (int i = prices.size() - 1; i >= 0; i--) {
            if (prices[i] <= price) {
                span++;
            } else {
                break;  // 遇到更大的价格就停止
            }
        }

        return span;
    }
};

void test_problem7() {
    cout << "========================================" << endl;
    cout << "题目 7: 股票跨度 (LeetCode 901)" << endl;
    cout << "========================================" << endl;

    // 测试单调栈方法
    cout << "单调栈方法:" << endl;
    StockSpanner spanner;
    vector<int> prices = {100, 80, 60, 70, 60, 75, 85};
    vector<int> expected = {1, 1, 1, 2, 1, 4, 6};

    for (int i = 0; i < prices.size(); i++) {
        int result = spanner.next(prices[i]);
        cout << "next(" << prices[i] << ") = " << result
             << " (期望: " << expected[i] << ")"
             << (result == expected[i] ? " [通过]" : " [失败]") << endl;
    }

    // 测试暴力法
    cout << "\n暴力法:" << endl;
    StockSpanner_Brute spanner2;
    for (int i = 0; i < prices.size(); i++) {
        int result = spanner2.next(prices[i]);
        cout << "next(" << prices[i] << ") = " << result
             << " (期望: " << expected[i] << ")"
             << (result == expected[i] ? " [通过]" : " [失败]") << endl;
    }

    // 测试单调递增序列
    cout << "\n单调递增序列: [30, 40, 50, 60]" << endl;
    StockSpanner spanner3;
    vector<int> prices2 = {30, 40, 50, 60};
    vector<int> expected2 = {1, 2, 3, 4};
    for (int i = 0; i < prices2.size(); i++) {
        int result = spanner3.next(prices2[i]);
        cout << "next(" << prices2[i] << ") = " << result
             << " (期望: " << expected2[i] << ")"
             << (result == expected2[i] ? " [通过]" : " [失败]") << endl;
    }

    // 测试单调递减序列
    cout << "\n单调递减序列: [60, 50, 40, 30]" << endl;
    StockSpanner spanner4;
    vector<int> prices3 = {60, 50, 40, 30};
    vector<int> expected3 = {1, 1, 1, 1};
    for (int i = 0; i < prices3.size(); i++) {
        int result = spanner4.next(prices3[i]);
        cout << "next(" << prices3[i] << ") = " << result
             << " (期望: " << expected3[i] << ")"
             << (result == expected3[i] ? " [通过]" : " [失败]") << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目 8: 字符串解码 (LeetCode 394, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给定一个经过编码的字符串，返回它解码后的字符串。
 * 编码规则为: k[encoded_string]，表示方括号内的字符串重复 k 次。
 *
 * 注意：k 保证为正整数，输入字符串中没有额外的空格。
 *
 * 示例：
 * 输入: s = "3[a]2[bc]"    输出: "aaabcbc"
 * 输入: s = "3[a2[c]]"     输出: "accaccacc"
 * 输入: s = "2[abc]3[cd]ef" 输出: "abcabccdcdcdef"
 *
 * 核心思想：
 * 使用栈处理嵌套结构。遇到 ']' 时弹出直到 '['，得到字符串和重复次数。
 * 遇到数字时继续解析多位数。
 */

// 方法一：栈处理嵌套字符串（推荐）
// 思路：遍历字符串，遇到 ']' 时弹出栈中内容直到 '['，组装重复字符串后压回
// 时间复杂度: O(n * maxK) 其中 maxK 为最大重复次数  空间复杂度: O(n)
string decodeString(string s) {
    stack<char> st;

    for (char c : s) {
        if (c != ']') {
            // 非右括号，直接入栈（包括字母、数字、左括号）
            st.push(c);
        } else {
            // 遇到 ']'：弹出直到 '['，收集编码字符串
            string encoded = "";
            while (!st.empty() && st.top() != '[') {
                encoded = st.top() + encoded;
                st.pop();
            }
            st.pop();  // 弹出 '['

            // 弹出数字（可能是多位数）
            string numStr = "";
            while (!st.empty() && isdigit(st.top())) {
                numStr = st.top() + numStr;
                st.pop();
            }
            int k = stoi(numStr);

            // 将 encoded 重复 k 次后压回栈中
            string decoded = "";
            for (int i = 0; i < k; i++) {
                decoded += encoded;
            }
            for (char ch : decoded) {
                st.push(ch);
            }
        }
    }

    // 将栈中所有字符组装成结果字符串
    string result = "";
    while (!st.empty()) {
        result = st.top() + result;
        st.pop();
    }

    return result;
}

// 方法二：递归法
// 思路：遇到数字时解析重复次数，遇到 '[' 递归解析内部字符串
// 时间复杂度: O(n * maxK)  空间复杂度: O(n)
class DecodeString_Recursive {
public:
    string decodeString(string s) {
        int index = 0;
        return helper(s, index);
    }

private:
    string helper(const string& s, int& index) {
        string result = "";

        while (index < s.length() && s[index] != ']') {
            if (isdigit(s[index])) {
                // 解析数字
                int num = 0;
                while (index < s.length() && isdigit(s[index])) {
                    num = num * 10 + (s[index] - '0');
                    index++;
                }
                index++;  // 跳过 '['

                // 递归解码内部字符串
                string decoded = helper(s, index);
                index++;  // 跳过 ']'

                // 重复 num 次
                for (int i = 0; i < num; i++) {
                    result += decoded;
                }
            } else {
                // 普通字母，直接拼接
                result += s[index];
                index++;
            }
        }

        return result;
    }
};

void test_problem8() {
    cout << "========================================" << endl;
    cout << "题目 8: 字符串解码 (LeetCode 394)" << endl;
    cout << "========================================" << endl;

    DecodeString_Recursive recursiveSolver;

    vector<pair<string, string>> testCases = {
        {"3[a]2[bc]", "aaabcbc"},
        {"3[a2[c]]", "accaccacc"},
        {"2[abc]3[cd]ef", "abcabccdcdcdef"},
        {"abc3[cd]xyz", "abccdcdcdxyz"},
        {"10[a]", "aaaaaaaaaa"},
        {"3[a]2[b4[F]c]", "aaabFFFFcbFFFFc"}
    };

    for (auto& [input, expected] : testCases) {
        string result1 = decodeString(input);
        string result2 = recursiveSolver.decodeString(input);
        cout << "输入: \"" << input << "\"" << endl;
        cout << "  栈方法:   \"" << result1 << "\""
             << (result1 == expected ? " [通过]" : " [失败]") << endl;
        cout << "  递归方法: \"" << result2 << "\""
             << (result2 == expected ? " [通过]" : " [失败]") << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目 9: 比较含退格的字符串 (LeetCode 844, Easy)
// ============================================================================
/**
 * 题目描述：
 * 给定 s 和 t 两个字符串，当它们分别被输入到空白的文本编辑器后，
 * 如果两者相等，返回 true。# 代表退格字符。
 *
 * 注意：如果对空文本输入退格字符，文本继续为空。
 *
 * 示例：
 * 输入: s = "ab#c", t = "ad#c"    输出: true  (都变为 "ac")
 * 输入: s = "ab##", t = "c#d#"    输出: true  (都变为 "")
 * 输入: s = "a#c", t = "b"        输出: false ("c" vs "b")
 *
 * 核心思想：
 * 用栈模拟退格操作：遇到 '#' 弹出栈顶，否则压入字符。
 * 最后比较两个栈的内容是否相同。
 */

// 方法一：栈模拟退格（推荐）
// 思路：遍历字符串，遇到 '#' 弹栈，否则压栈
// 时间复杂度: O(n+m)  空间复杂度: O(n+m)
bool backspaceCompare(string s, string t) {
    // 用栈处理字符串 s
    stack<char> st1;
    for (char c : s) {
        if (c == '#') {
            if (!st1.empty()) st1.pop();  // 退格：弹出栈顶
        } else {
            st1.push(c);  // 普通字符：入栈
        }
    }

    // 用栈处理字符串 t
    stack<char> st2;
    for (char c : t) {
        if (c == '#') {
            if (!st2.empty()) st2.pop();
        } else {
            st2.push(c);
        }
    }

    // 比较两个栈的内容
    if (st1.size() != st2.size()) return false;

    while (!st1.empty()) {
        if (st1.top() != st2.top()) return false;
        st1.pop();
        st2.pop();
    }

    return true;
}

// 方法二：双指针（O(1) 空间）
// 思路：从后向前遍退，用 skip 记录需要跳过的字符数
// 时间复杂度: O(n+m)  空间复杂度: O(1)
bool backspaceCompare_twoPointer(string s, string t) {
    int i = s.length() - 1;  // s 的指针
    int j = t.length() - 1;  // t 的指针
    int skipS = 0, skipT = 0;  // 需要跳过的字符数

    while (i >= 0 || j >= 0) {
        // 找到 s 中下一个实际字符
        while (i >= 0) {
            if (s[i] == '#') {
                skipS++;  // 遇到退格符
                i--;
            } else if (skipS > 0) {
                skipS--;  // 跳过一个字符
                i--;
            } else {
                break;  // 找到实际字符
            }
        }

        // 找到 t 中下一个实际字符
        while (j >= 0) {
            if (t[j] == '#') {
                skipT++;
                j--;
            } else if (skipT > 0) {
                skipT--;
                j--;
            } else {
                break;
            }
        }

        // 比较两个实际字符
        if (i >= 0 && j >= 0) {
            if (s[i] != t[j]) return false;
        } else if (i >= 0 || j >= 0) {
            // 一个有字符，另一个没有
            return false;
        }

        i--;
        j--;
    }

    return true;
}

void test_problem9() {
    cout << "========================================" << endl;
    cout << "题目 9: 比较含退格的字符串 (LeetCode 844)" << endl;
    cout << "========================================" << endl;

    vector<tuple<string, string, bool>> testCases = {
        {"ab#c", "ad#c", true},      // "ac" == "ac"
        {"ab##", "c#d#", true},      // "" == ""
        {"a#c", "b", false},         // "c" != "b"
        {"a##c", "#a#c", true},      // "c" == "c"
        {"bxj##tw", "bxo#j##tw", true},  // "btw" == "btw"
        {"xywrrmp", "xywrrmu#p", true},  // "xywrrmp" == "xywrrmp"
        {"", "", true},              // 空串
        {"a", "a", true}             // 单字符
    };

    for (auto& [s, t, expected] : testCases) {
        bool result1 = backspaceCompare(s, t);
        bool result2 = backspaceCompare_twoPointer(s, t);
        cout << "s=\"" << s << "\", t=\"" << t << "\""
             << " 期望: " << (expected ? "true" : "false")
             << " 栈方法: " << (result1 ? "true" : "false")
             << " 双指针: " << (result2 ? "true" : "false")
             << (result1 == expected && result2 == expected ? " [通过]" : " [失败]")
             << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目 10: 逆波兰表达式求值 (LeetCode 150, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给你一个字符串数组 tokens，表示一个逆波兰表达式（后缀表达式），
 * 返回该表达式的计算结果。
 *
 * 有效运算符: +, -, *, /
 * 每个操作数可以是整数或另一个逆波兰表达式。
 * 整数除法只保留整数部分。
 *
 * 示例：
 * 输入: tokens = ["2","1","+","3","*"]  输出: 9  ((2+1)*3)
 * 输入: tokens = ["4","13","5","/","+"]  输出: 6  (4+(13/5))
 * 输入: tokens = ["10","6","9","3","+","-11","*","/","*","17","+","5","+"]
 * 输出: 22  ((10*(6/((9+3)*-11)))+17)+5
 *
 * 核心思想：
 * 逆波兰表达式天然适合用栈求值：
 * - 遇到数字入栈
 * - 遇到运算符，弹出两个操作数，计算后将结果入栈
 */

// 方法一：栈求值（推荐）
// 时间复杂度: O(n)  空间复杂度: O(n)
int evalRPN(vector<string>& tokens) {
    stack<int> st;

    for (const string& token : tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // 遇到运算符，弹出两个操作数
            // 注意：先弹出的是右操作数，后弹出的是左操作数
            int right = st.top();
            st.pop();
            int left = st.top();
            st.pop();

            if (token == "+") {
                st.push(left + right);
            } else if (token == "-") {
                st.push(left - right);
            } else if (token == "*") {
                st.push(left * right);
            } else if (token == "/") {
                st.push(left / right);  // C++ 整数除法向零截断
            }
        } else {
            // 数字：转为整数后入栈
            st.push(stoi(token));
        }
    }

    // 栈中最后一个元素就是结果
    return st.top();
}

// 方法二：递归法
// 思路：逆波兰表达式可以从右向左递归解析
// 时间复杂度: O(n)  空间复杂度: O(n)
class EvalRPN_Recursive {
public:
    int evalRPN(vector<string>& tokens) {
        int index = tokens.size() - 1;
        return helper(tokens, index);
    }

private:
    int helper(vector<string>& tokens, int& index) {
        string token = tokens[index];
        index--;

        if (token == "+" || token == "-" || token == "*" || token == "/") {
            // 递归解析右操作数和左操作数
            int right = helper(tokens, index);
            int left = helper(tokens, index);

            if (token == "+") return left + right;
            if (token == "-") return left - right;
            if (token == "*") return left * right;
            return left / right;
        }

        return stoi(token);  // 数字直接返回
    }
};

void test_problem10() {
    cout << "========================================" << endl;
    cout << "题目 10: 逆波兰表达式求值 (LeetCode 150)" << endl;
    cout << "========================================" << endl;

    EvalRPN_Recursive recursiveSolver;

    // 测试用例 1
    vector<string> tokens1 = {"2", "1", "+", "3", "*"};
    cout << "输入: [\"2\",\"1\",\"+\",\"3\",\"*\"]" << endl;
    cout << "  栈方法: " << evalRPN(tokens1) << " (期望: 9)" << endl;
    tokens1 = {"2", "1", "+", "3", "*"};
    cout << "  递归法: " << recursiveSolver.evalRPN(tokens1) << " (期望: 9)" << endl;

    // 测试用例 2
    vector<string> tokens2 = {"4", "13", "5", "/", "+"};
    cout << "\n输入: [\"4\",\"13\",\"5\",\"/\",\"+\"]" << endl;
    cout << "  栈方法: " << evalRPN(tokens2) << " (期望: 6)" << endl;
    tokens2 = {"4", "13", "5", "/", "+"};
    cout << "  递归法: " << recursiveSolver.evalRPN(tokens2) << " (期望: 6)" << endl;

    // 测试用例 3
    vector<string> tokens3 = {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"};
    cout << "\n输入: 复杂逆波兰表达式" << endl;
    cout << "  栈方法: " << evalRPN(tokens3) << " (期望: 22)" << endl;
    tokens3 = {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"};
    cout << "  递归法: " << recursiveSolver.evalRPN(tokens3) << " (期望: 22)" << endl;

    // 测试用例 4：单个数字
    vector<string> tokens4 = {"42"};
    cout << "\n输入: [\"42\"]" << endl;
    cout << "  栈方法: " << evalRPN(tokens4) << " (期望: 42)" << endl;

    // 测试用例 5：负数除法
    vector<string> tokens5 = {"-1", "2", "/"};
    cout << "\n输入: [\"-1\",\"2\",\"/\"]" << endl;
    cout << "  栈方法: " << evalRPN(tokens5) << " (期望: 0, 向零截断)" << endl;
    cout << endl;
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    cout << "============================================" << endl;
    cout << "    07_栈 - LeetCode 经典例题" << endl;
    cout << "============================================" << endl << endl;

    test_problem1();   // 有效的括号
    test_problem2();   // 最小栈
    test_problem3();   // 每日温度
    test_problem4();   // 柱状图中最大的矩形
    test_problem5();   // 基本计算器 II
    test_problem6();   // 下一个更大元素
    test_problem7();   // 股票跨度
    test_problem8();   // 字符串解码
    test_problem9();   // 比较含退格的字符串
    test_problem10();  // 逆波兰表达式求值

    cout << "============================================" << endl;
    cout << "    所有题目测试完成！" << endl;
    cout << "============================================" << endl;

    return 0;
}
