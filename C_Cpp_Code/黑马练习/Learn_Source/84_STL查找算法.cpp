/*
 * 84_STL查找算法.cpp
 * 本文件演示 STL 中常用的查找算法
 * 包括: find, find_if, find_if_not, binary_search, lower_bound, upper_bound,
 *       equal_range, search, adjacent_find, min_element, max_element, minmax_element
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <functional>
using namespace std;

// 打印辅助函数
template<typename T>
void printVec(const string& label, const vector<T>& v) {
    cout << label << ": ";
    for (const auto& val : v) cout << val << " ";
    cout << endl;
}

int main() {
    cout << "========== 1. find / find_if / find_if_not ==========" << endl;
    // find: 查找第一个等于指定值的元素，返回迭代器
    vector<int> v = {10, 20, 30, 40, 50, 30, 60};
    auto it = find(v.begin(), v.end(), 30);
    if (it != v.end()) {
        cout << "找到 30, 索引: " << distance(v.begin(), it) << endl;
    }

    // find 只找第一个匹配的元素
    // 如果要找所有匹配，需要循环调用
    cout << "所有 30 的位置: ";
    auto searchStart = v.begin();
    while (true) {
        auto found = find(searchStart, v.end(), 30);
        if (found == v.end()) break;
        cout << distance(v.begin(), found) << " ";
        searchStart = found + 1;  // 从下一个位置继续
    }
    cout << endl;

    // find_if: 查找第一个满足谓词条件的元素
    auto it2 = find_if(v.begin(), v.end(),
                       [](int val) { return val > 35; });
    if (it2 != v.end()) {
        cout << "第一个 >35 的元素: " << *it2
             << " (索引 " << distance(v.begin(), it2) << ")" << endl;
    }

    // find_if_not: 查找第一个不满足条件的元素 (C++11)
    vector<int> v2 = {2, 4, 6, 8, 9, 10, 12};
    auto it3 = find_if_not(v2.begin(), v2.end(),
                           [](int val) { return val % 2 == 0; });
    if (it3 != v2.end()) {
        cout << "第一个非偶数: " << *it3
             << " (索引 " << distance(v2.begin(), it3) << ")" << endl;
    }

    cout << "\n========== 2. binary_search 二分查找 ==========" << endl;
    // binary_search: 在已排序的区间中查找元素是否存在
    // 返回 bool 值，不返回位置
    // 前提: 容器必须已排序！
    vector<int> sorted = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    printVec("已排序数据", sorted);

    bool found = binary_search(sorted.begin(), sorted.end(), 7);
    cout << "binary_search(7): " << (found ? "找到" : "未找到") << endl;

    found = binary_search(sorted.begin(), sorted.end(), 8);
    cout << "binary_search(8): " << (found ? "找到" : "未找到") << endl;

    // binary_search 也可以自定义比较器
    vector<int> desc = {19, 17, 15, 13, 11, 9, 7, 5, 3, 1};
    found = binary_search(desc.begin(), desc.end(), 7, greater<int>());
    cout << "降序 binary_search(7): " << (found ? "找到" : "未找到") << endl;

    // 常见错误: 对未排序的容器使用 binary_search
    // vector<int> unsorted = {5, 3, 1, 4, 2};
    // binary_search(unsorted.begin(), unsorted.end(), 3);  // 结果不可靠！

    cout << "\n========== 3. lower_bound / upper_bound ==========" << endl;
    // lower_bound: 返回第一个不小于(>=) value 的元素的迭代器
    // upper_bound: 返回第一个大于(>) value 的元素的迭代器
    // 前提: 容器必须已排序！
    vector<int> data = {1, 2, 3, 3, 3, 4, 5, 6};
    printVec("数据", data);

    auto lb = lower_bound(data.begin(), data.end(), 3);
    auto ub = upper_bound(data.begin(), data.end(), 3);
    cout << "lower_bound(3): 索引 " << distance(data.begin(), lb)
         << ", 值=" << *lb << endl;
    cout << "upper_bound(3): 索引 " << distance(data.begin(), ub)
         << ", 值=" << *ub << endl;
    cout << "元素 3 的个数: " << distance(lb, ub) << endl;

    // lower_bound 用于查找插入位置
    vector<int> sorted2 = {10, 20, 30, 40, 50};
    auto insertPos = lower_bound(sorted2.begin(), sorted2.end(), 25);
    cout << "25 应插入位置索引: " << distance(sorted2.begin(), insertPos) << endl;

    // 如果元素不存在
    auto lb2 = lower_bound(data.begin(), data.end(), 10);
    if (lb2 == data.end()) {
        cout << "10 不在容器中，lower_bound 返回 end()" << endl;
    }

    cout << "\n========== 4. equal_range ==========" << endl;
    // equal_range: 返回一个 pair，表示等于 value 的元素范围
    // 等价于 {lower_bound, upper_bound}
    // 前提: 容器必须已排序
    vector<int> data2 = {1, 2, 3, 3, 3, 4, 5, 6};
    auto range = equal_range(data2.begin(), data2.end(), 3);

    cout << "equal_range(3):" << endl;
    cout << "  lower_bound 索引: " << distance(data2.begin(), range.first) << endl;
    cout << "  upper_bound 索引: " << distance(data2.begin(), range.second) << endl;
    cout << "  元素 3 的个数: " << distance(range.first, range.second) << endl;

    // 打印范围内的所有元素
    cout << "  范围内元素: ";
    for (auto it = range.first; it != range.second; ++it)
        cout << *it << " ";
    cout << endl;

    // equal_range 用于自定义比较器
    vector<string> names = {"Alice", "Bob", "Bob", "Charlie", "David"};
    auto nameRange = equal_range(names.begin(), names.end(), "Bob");
    cout << "Bob 的个数: " << distance(nameRange.first, nameRange.second) << endl;

    cout << "\n========== 5. search 子序列查找 ==========" << endl;
    // search: 在序列1中查找序列2第一次出现的位置
    // 与 find 不同，search 查找的是子序列（多个连续元素）
    vector<int> text = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3};
    vector<int> pattern = {1, 2, 3};

    auto pos = search(text.begin(), text.end(), pattern.begin(), pattern.end());
    if (pos != text.end()) {
        cout << "子序列 {1,2,3} 首次出现位置: " << distance(text.begin(), pos) << endl;
    }

    // search + 自定义谓词（例如忽略大小写查找字符串）
    string str = "Hello World hello world";
    string sub = "hello";
    auto strPos = search(str.begin(), str.end(), sub.begin(), sub.end(),
                         [](char a, char b) { return tolower(a) == tolower(b); });
    if (strPos != str.end()) {
        cout << "忽略大小写找到 \"hello\" 位置: "
             << distance(str.begin(), strPos) << endl;
    }

    // find_end: 查找子序列最后出现的位置（与 search 相反）
    auto lastPos = find_end(text.begin(), text.end(), pattern.begin(), pattern.end());
    if (lastPos != text.end()) {
        cout << "子序列 {1,2,3} 最后出现位置: " << distance(text.begin(), lastPos) << endl;
    }

    // search_n: 查找连续 n 个等于指定值的元素
    vector<int> v3 = {1, 2, 3, 3, 3, 4, 5, 5, 5, 5, 6};
    auto nPos = search_n(v3.begin(), v3.end(), 3, 3);  // 查找连续3个3
    if (nPos != v3.end()) {
        cout << "连续3个3的位置: " << distance(v3.begin(), nPos) << endl;
    }

    auto nPos2 = search_n(v3.begin(), v3.end(), 4, 5);  // 查找连续4个5
    if (nPos2 != v3.end()) {
        cout << "连续4个5的位置: " << distance(v3.begin(), nPos2) << endl;
    }

    cout << "\n========== 6. adjacent_find 相邻重复查找 ==========" << endl;
    // adjacent_find: 查找第一对相邻且相等的元素
    vector<int> v4 = {1, 2, 3, 3, 4, 5, 5, 6};
    auto adj = adjacent_find(v4.begin(), v4.end());
    if (adj != v4.end()) {
        cout << "第一对相邻重复元素: " << *adj
             << " (索引 " << distance(v4.begin(), adj) << ")" << endl;
    }

    // adjacent_find + 自定义谓词
    vector<int> v5 = {1, 5, 3, 8, 2, 9, 4};
    auto adj2 = adjacent_find(v5.begin(), v5.end(),
                              [](int a, int b) { return a > b; });  // 第一对递减的
    if (adj2 != v5.end()) {
        cout << "第一对递减相邻元素: " << *adj2 << " > " << *(adj2 + 1) << endl;
    }

    cout << "\n========== 7. min_element / max_element / minmax_element ==========" << endl;
    // min_element: 返回指向最小元素的迭代器
    // max_element: 返回指向最大元素的迭代器
    // minmax_element: 返回 pair<最小, 最大> (C++11)
    vector<int> v6 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    printVec("数据", v6);

    auto minIt = min_element(v6.begin(), v6.end());
    auto maxIt = max_element(v6.begin(), v6.end());
    cout << "最小元素: " << *minIt << " (索引 " << distance(v6.begin(), minIt) << ")" << endl;
    cout << "最大元素: " << *maxIt << " (索引 " << distance(v6.begin(), maxIt) << ")" << endl;

    // minmax_element 一次返回最小和最大 (比分别调用 min/max_element 更高效)
    auto [minElem, maxElem] = minmax_element(v6.begin(), v6.end());  // C++17 结构化绑定
    cout << "minmax_element: min=" << *minElem << ", max=" << *maxElem << endl;

    // 自定义比较器：按字符串长度找最短/最长
    vector<string> words = {"hello", "hi", "greetings", "hey", "howdy"};
    auto shortest = min_element(words.begin(), words.end(),
                                [](const string& a, const string& b) {
                                    return a.size() < b.size();
                                });
    auto longest = max_element(words.begin(), words.end(),
                               [](const string& a, const string& b) {
                                   return a.size() < b.size();
                               });
    cout << "最短单词: " << *shortest << " (" << shortest->size() << "字符)" << endl;
    cout << "最长单词: " << *longest << " (" << longest->size() << "字符)" << endl;

    // 注意: min_element/max_element 对空容器返回 end()
    vector<int> empty;
    auto emptyMin = min_element(empty.begin(), empty.end());
    if (emptyMin == empty.end()) {
        cout << "空容器的 min_element 返回 end()" << endl;
    }

    cout << "\n========== 8. 综合应用示例 ==========" << endl;
    // 示例: 在学生成绩中查找
    struct Student {
        string name;
        int score;
    };

    vector<Student> students = {
        {"张三", 85}, {"李四", 92}, {"王五", 78},
        {"赵六", 95}, {"钱七", 88}, {"孙八", 92}
    };

    // 查找第一个成绩 >= 90 的学生
    auto topStudent = find_if(students.begin(), students.end(),
                              [](const Student& s) { return s.score >= 90; });
    if (topStudent != students.end()) {
        cout << "第一个成绩>=90: " << topStudent->name
             << " (" << topStudent->score << ")" << endl;
    }

    // 统计成绩 >= 90 的人数
    int topCount = count_if(students.begin(), students.end(),
                            [](const Student& s) { return s.score >= 90; });
    cout << "成绩>=90的人数: " << topCount << endl;

    // 查找最高分学生
    auto bestIt = max_element(students.begin(), students.end(),
                              [](const Student& a, const Student& b) {
                                  return a.score < b.score;
                              });
    cout << "最高分: " << bestIt->name << " (" << bestIt->score << ")" << endl;

    cout << "\n========== 9. 常见错误 ==========" << endl;
    // 错误1: 对未排序容器使用 binary_search / lower_bound / upper_bound
    // 结果不可靠！

    // 错误2: 未检查返回值就解引用
    // auto it = find(v.begin(), v.end(), 999);
    // cout << *it;  // 如果未找到，*end() 是未定义行为

    // 错误3: 混淆 lower_bound 和 upper_bound
    // lower_bound 返回第一个 >= 的位置
    // upper_bound 返回第一个 > 的位置
    // 对于 {1,2,2,2,3}，lower_bound(2) 指向第一个2，upper_bound(2) 指向3

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
