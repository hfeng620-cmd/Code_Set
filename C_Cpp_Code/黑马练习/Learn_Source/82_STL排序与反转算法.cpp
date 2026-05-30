/*
 * 82_STL排序与反转算法.cpp
 * 本文件演示 STL 中常用的排序与反转算法
 * 包括: sort, stable_sort, partial_sort, reverse, random_shuffle/shuffle, nth_element
 * 以及比较器的三种形式: 函数、函子、Lambda
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>  // greater, less
#include <random>      // shuffle 需要随机数引擎
#include <ctime>       // time
using namespace std;

// ============ 比较函数 ============

// 自定义升序比较函数
bool ascending(int a, int b) {
    return a < b;
}

// 自定义降序比较函数
bool descending(int a, int b) {
    return a > b;
}

// 按绝对值比较
bool compareByAbs(int a, int b) {
    return abs(a) < abs(b);
}

// ============ 比较函子（仿函数）============

// 按成绩降序排列的函子
struct Student {
    string name;
    int score;
};

class CompareByScore {
public:
    bool operator()(const Student& a, const Student& b) const {
        return a.score > b.score;  // 降序
    }
};

// 多级排序函子：先按成绩降序，成绩相同按姓名升序
class CompareMultiLevel {
public:
    bool operator()(const Student& a, const Student& b) const {
        if (a.score != b.score)
            return a.score > b.score;   // 成绩降序
        return a.name < b.name;         // 姓名升序
    }
};

// 打印辅助函数
void printVec(const string& label, const vector<int>& v) {
    cout << label << ": ";
    for (auto val : v) cout << val << " ";
    cout << endl;
}

void printStudents(const string& label, const vector<Student>& students) {
    cout << label << ":" << endl;
    for (const auto& s : students) {
        cout << "  " << s.name << " -> " << s.score << endl;
    }
}

int main() {
    cout << "========== 1. sort 默认排序 ==========" << endl;
    // sort: 默认使用 < 运算符进行升序排序
    // 函数签名: sort(RandomAccessIt first, RandomAccessIt last)
    // 注意: sort 只能用于随机访问迭代器（vector, deque, 数组），不能用于 list
    vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    printVec("排序前", v);

    sort(v.begin(), v.end());  // 默认升序
    printVec("默认升序", v);

    // 使用 greater<int>() 进行降序排序
    sort(v.begin(), v.end(), greater<int>());
    printVec("greater降序", v);

    cout << "\n========== 2. sort 自定义比较器 ==========" << endl;
    // 比较器的三种形式：函数指针、函子、Lambda

    // 方式一：函数指针
    vector<int> v2 = {5, -2, 8, -1, 9, -3, 7};
    sort(v2.begin(), v2.end(), ascending);
    printVec("函数指针升序", v2);

    sort(v2.begin(), v2.end(), descending);
    printVec("函数指针降序", v2);

    // 按绝对值排序
    sort(v2.begin(), v2.end(), compareByAbs);
    printVec("按绝对值排序", v2);

    // 方式二：函子（仿函数）
    vector<Student> students = {
        {"张三", 85}, {"李四", 92}, {"王五", 78},
        {"赵六", 92}, {"钱七", 85}
    };
    printStudents("排序前", students);

    sort(students.begin(), students.end(), CompareByScore());
    printStudents("按成绩降序（函子）", students);

    // 方式三：Lambda 表达式（最灵活）
    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b) {
             return a.name < b.name;  // 按姓名升序
         });
    printStudents("按姓名升序（Lambda）", students);

    // 多级排序：Lambda
    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b) {
             if (a.score != b.score) return a.score > b.score;
             return a.name < b.name;
         });
    printStudents("成绩降序+姓名升序（Lambda多级）", students);

    cout << "\n========== 3. stable_sort 稳定排序 ==========" << endl;
    // stable_sort: 保持相等元素的相对顺序不变
    // 当相等元素需要保持原有顺序时使用
    vector<Student> students2 = {
        {"张三", 85}, {"李四", 92}, {"王五", 78},
        {"赵六", 92}, {"钱七", 85}
    };

    // sort 不保证相等元素的顺序
    sort(students2.begin(), students2.end(), CompareByScore());
    cout << "sort 结果（相等元素顺序可能改变）:" << endl;
    for (const auto& s : students2)
        cout << "  " << s.name << " -> " << s.score << endl;

    // stable_sort 保持相等元素的原始顺序
    vector<Student> students3 = {
        {"张三", 85}, {"李四", 92}, {"王五", 78},
        {"赵六", 92}, {"钱七", 85}
    };
    stable_sort(students3.begin(), students3.end(), CompareByScore());
    cout << "stable_sort 结果（保持相等元素原始顺序）:" << endl;
    for (const auto& s : students3)
        cout << "  " << s.name << " -> " << s.score << endl;

    cout << "\n========== 4. partial_sort 部分排序 ==========" << endl;
    // partial_sort: 只排序前 N 个最小元素
    // 函数签名: partial_sort(First, Middle, Last)
    // 将 [First, Last) 中最小的 (Middle-First) 个元素放在 [First, Middle)
    vector<int> v3 = {9, 5, 2, 7, 1, 8, 3, 6, 4};
    printVec("排序前", v3);

    // 只排序前 3 个最小元素
    partial_sort(v3.begin(), v3.begin() + 3, v3.end());
    printVec("partial_sort 前3小", v3);
    // 前3个是最小的3个且有序，后面的顺序不确定

    // 取前3个最大元素（降序）
    vector<int> v4 = {9, 5, 2, 7, 1, 8, 3, 6, 4};
    partial_sort(v4.begin(), v4.begin() + 3, v4.end(), greater<int>());
    printVec("partial_sort 前3大", v4);

    cout << "\n========== 5. reverse 反转算法 ==========" << endl;
    // reverse: 反转容器中元素的顺序
    vector<int> v5 = {1, 2, 3, 4, 5};
    printVec("反转前", v5);
    reverse(v5.begin(), v5.end());
    printVec("反转后", v5);

    // 反转字符串
    string str = "Hello World";
    cout << "字符串反转前: " << str << endl;
    reverse(str.begin(), str.end());
    cout << "字符串反转后: " << str << endl;

    // 反转部分区间
    vector<int> v6 = {1, 2, 3, 4, 5, 6, 7, 8};
    reverse(v6.begin() + 2, v6.begin() + 6);  // 反转索引2到5
    printVec("部分反转 [2,6)", v6);

    cout << "\n========== 6. random_shuffle / shuffle 随机洗牌 ==========" << endl;
    // random_shuffle: 随机打乱元素顺序（C++17已弃用）
    // shuffle: C++11 推荐，需要提供随机数引擎
    vector<int> v7 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // C++11 推荐方式: 使用 shuffle + 随机数引擎
    // random_device 产生种子，mt19937 是梅森旋转算法引擎
    random_device rd;
    mt19937 g(rd());
    shuffle(v7.begin(), v7.end(), g);
    printVec("shuffle 结果", v7);

    // 再次 shuffle，结果会不同
    shuffle(v7.begin(), v7.end(), g);
    printVec("再次shuffle", v7);

    cout << "\n========== 7. nth_element 第N个元素 ==========" << endl;
    // nth_element: 使第 N 个位置上的元素就是排序后该位置的元素
    // 且左边的都小于等于它，右边的都大于等于它
    // 但左右两部分内部不保证有序
    vector<int> v8 = {9, 5, 2, 7, 1, 8, 3, 6, 4};
    printVec("排序前", v8);

    // 找第5小的元素（索引4）
    nth_element(v8.begin(), v8.begin() + 4, v8.end());
    cout << "第5小的元素: " << v8[4] << endl;
    printVec("nth_element后", v8);
    // 注意: v8[4] 确实是第5小的，但其他位置不一定有序

    // 找中位数
    vector<int> v9 = {9, 5, 2, 7, 1, 8, 3, 6, 4};
    size_t mid = v9.size() / 2;
    nth_element(v9.begin(), v9.begin() + mid, v9.end());
    cout << "中位数: " << v9[mid] << endl;

    // 找第3大的元素
    vector<int> v10 = {9, 5, 2, 7, 1, 8, 3, 6, 4};
    nth_element(v10.begin(), v10.begin() + 2, v10.end(), greater<int>());
    cout << "第3大的元素: " << v10[2] << endl;

    cout << "\n========== 8. 比较器总结 ==========" << endl;
    cout << "比较器需要满足严格弱序关系:" << endl;
    cout << "  1. 非自反性: comp(a, a) == false" << endl;
    cout << "  2. 非对称性: comp(a,b) == true => comp(b,a) == false" << endl;
    cout << "  3. 传递性: comp(a,b) && comp(b,c) => comp(a,c)" << endl;

    // 常见错误: 比较器写错导致未定义行为
    // 错误示例: 使用 <= 而不是 < （违反非自反性）
    // sort(v.begin(), v.end(), [](int a, int b){ return a <= b; }); // 错误！

    cout << "\n========== 9. 常见错误 ==========" << endl;
    // 错误1: 对 list 使用 sort（list 不支持随机访问）
    // list<int> lst = {3, 1, 2};
    // sort(lst.begin(), lst.end());  // 编译错误！
    // 正确做法: lst.sort();  // list 有自己的 sort 成员函数

    // 错误2: 比较器违反严格弱序
    // sort(v.begin(), v.end(), [](int a, int b){ return a <= b; });
    // 这会导致未定义行为（可能崩溃或死循环）

    // 错误3: 对空容器排序
    // vector<int> empty;
    // sort(empty.begin(), empty.end());  // 技术上没问题，但要小心后续操作

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
