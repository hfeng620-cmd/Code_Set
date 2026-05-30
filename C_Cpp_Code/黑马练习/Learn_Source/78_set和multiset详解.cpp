/*
 ============================================================================
 文件名 : 78_set和multiset详解.cpp
 描  述 : 详细讲解C++ STL set 和 multiset 关联容器的用法
          包括：set有序唯一集合、insert/erase/find/count、
          lower_bound/upper_bound/equal_range、multiset允许重复、
          自定义比较器（仿函数/lambda）、O(log n)操作、
          去重和排序应用场景
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 78_set和multiset详解.cpp -o 78_set和multiset详解
 ============================================================================
*/

#include <iostream>
#include <set>
#include <string>
#include <functional>  // greater<>
#include <vector>
#include <algorithm>
using namespace std;

// 打印 set 内容的辅助函数
template<typename T>
void printSet(const string& title, const T& s) {
    cout << title << " [size=" << s.size() << "]: ";
    for (const auto& elem : s) {
        cout << elem << " ";
    }
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "    set 和 multiset 详解 演示" << endl;
    cout << "========================================" << endl;

    // ==================== 1. set 基础介绍 ====================
    /*
     * set 是 STL 中的关联容器，存储唯一且有序的元素
     *
     * 特点:
     *   1. 元素唯一：不允许重复元素
     *   2. 自动排序：默认按升序排列
     *   3. 底层实现：红黑树（平衡二叉搜索树）
     *   4. 查找/插入/删除：O(log n)
     *   5. 不支持随机访问：不能用 [] 或 at()
     *   6. 元素是 const 的，插入后不能修改值
     *
     * set vs unordered_set:
     *   set:           有序, O(log n), 红黑树
     *   unordered_set: 无序, O(1)摊还, 哈希表
     *
     * 适用场景：去重、有序集合、高效的查找/插入/删除
     */

    // ==================== 2. set 构造和插入 ====================
    cout << "\n--- 1. set 构造和插入 ---" << endl;

    set<int> s1;
    // insert 返回 pair<iterator, bool>
    auto r1 = s1.insert(30);
    cout << "插入30: " << (r1.second ? "成功" : "失败") << endl;

    auto r2 = s1.insert(30);  // 重复插入
    cout << "再次插入30: " << (r2.second ? "成功" : "失败") << endl;

    s1.insert(10);
    s1.insert(50);
    s1.insert(20);
    s1.insert(40);
    printSet("s1", s1);  // 自动排序: 10 20 30 40 50

    // 初始化列表
    set<int> s2 = {5, 3, 8, 1, 9, 2, 7};
    printSet("s2 初始化列表", s2);  // 1 2 3 5 7 8 9

    // ==================== 3. set 查找操作 ====================
    cout << "\n--- 2. set 查找操作 ---" << endl;
    set<int> find_set = {10, 20, 30, 40, 50, 60, 70};

    // find: 返回迭代器，O(log n)
    auto it = find_set.find(30);
    if (it != find_set.end()) {
        cout << "find(30): 找到, 值=" << *it << endl;
    }

    it = find_set.find(35);
    cout << "find(35): " << (it != find_set.end() ? "找到" : "未找到") << endl;

    // count: 返回0或1（set中不可能大于1）
    cout << "count(30): " << find_set.count(30) << endl;  // 1
    cout << "count(35): " << find_set.count(35) << endl;  // 0

    // ==================== 4. lower_bound / upper_bound / equal_range ====================
    cout << "\n--- 3. lower_bound / upper_bound / equal_range ---" << endl;
    /*
     * lower_bound(val): 返回第一个 >= val 的元素迭代器
     * upper_bound(val): 返回第一个 >  val 的元素迭代器
     * equal_range(val): 返回 pair(lower_bound, upper_bound)
     */
    set<int> bound_set = {10, 20, 30, 40, 50, 60, 70};

    auto lb = bound_set.lower_bound(35);  // >= 35 的第一个 => 40
    cout << "lower_bound(35): " << *lb << endl;

    auto ub = bound_set.upper_bound(50);  // > 50 的第一个 => 60
    cout << "upper_bound(50): " << *ub << endl;

    // 如果查找的值存在
    auto lb2 = bound_set.lower_bound(50);  // => 50
    auto ub2 = bound_set.upper_bound(50);  // => 60
    cout << "lower_bound(50): " << *lb2 << endl;
    cout << "upper_bound(50): " << *ub2 << endl;

    // equal_range
    auto range = bound_set.equal_range(50);
    cout << "equal_range(50): [" << *range.first << ", " << *range.second << ")" << endl;

    // ==================== 5. set 删除操作 ====================
    cout << "\n--- 4. set 删除操作 ---" << endl;
    set<int> del_set = {10, 20, 30, 40, 50, 60, 70};
    printSet("删除前", del_set);

    // erase(val): 按值删除，返回删除个数
    size_t cnt = del_set.erase(30);
    cout << "erase(30): 删除了" << cnt << "个" << endl;
    printSet("删除30后", del_set);

    // erase(iterator): 按迭代器删除
    auto dit = del_set.find(50);
    if (dit != del_set.end()) del_set.erase(dit);
    printSet("删除50后", del_set);

    // ==================== 6. 自定义比较器 ====================
    cout << "\n--- 5. 自定义比较器 ---" << endl;

    // 方法1: 使用 greater<> 降序排列
    set<int, greater<int>> desc_set = {30, 10, 50, 20, 40};
    cout << "降序set: ";
    for (const auto& v : desc_set) cout << v << " ";
    cout << endl;

    // 方法2: 自定义仿函数（函数对象）
    struct LengthCompare {
        bool operator()(const string& a, const string& b) const {
            if (a.length() != b.length())
                return a.length() < b.length();  // 按长度排序
            return a < b;  // 长度相同按字典序
        }
    };

    set<string, LengthCompare> len_set;
    len_set.insert("banana");
    len_set.insert("apple");
    len_set.insert("hi");
    len_set.insert("cat");
    len_set.insert("a");
    cout << "按长度排序: ";
    for (const auto& s : len_set) cout << s << "(" << s.length() << ") ";
    cout << endl;

    // 方法3: 使用 lambda（需要 decltype）
    auto comp = [](int a, int b) { return a > b; };
    set<int, decltype(comp)> lambda_set(comp);
    lambda_set.insert({30, 10, 50, 20, 40});
    cout << "lambda降序: ";
    for (const auto& v : lambda_set) cout << v << " ";
    cout << endl;

    // ==================== 7. multiset 详解 ====================
    cout << "\n--- 6. multiset 详解 ---" << endl;
    /*
     * multiset 与 set 的区别：
     *   set:       元素唯一，重复插入会被忽略
     *   multiset:  允许重复元素
     * 其他特性相同：有序、红黑树、O(log n)
     */
    multiset<int> mset;
    mset.insert(10);
    mset.insert(20);
    mset.insert(30);
    mset.insert(20);  // 重复，会插入
    mset.insert(20);  // 重复，会插入
    mset.insert(10);  // 重复，会插入
    printSet("multiset", mset);  // 10 10 20 20 20 30

    // count: 可以大于1
    cout << "count(20): " << mset.count(20) << endl;  // 3
    cout << "count(10): " << mset.count(10) << endl;  // 2

    // erase(val): 删除所有等于 val 的元素
    mset.erase(20);
    printSet("erase(20)后", mset);  // 10 10 30

    // 只删除一个
    mset.insert(10);
    mset.insert(10);
    printSet("添加10后", mset);
    auto mit = mset.find(10);
    if (mit != mset.end()) mset.erase(mit);  // 只删除一个
    printSet("删除一个10后", mset);

    // lower_bound / upper_bound 在 multiset 中的应用
    multiset<int> mset2 = {10, 20, 20, 20, 30, 40};
    auto mlb = mset2.lower_bound(20);  // 第一个20
    auto mub = mset2.upper_bound(20);  // 第一个不是20
    cout << "20的个数: " << distance(mlb, mub) << endl;

    // ==================== 8. set 应用场景 ====================
    cout << "\n--- 7. 应用场景 ---" << endl;

    // 应用1: 数组去重
    cout << "【数组去重】" << endl;
    vector<int> nums = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    cout << "去重前: ";
    for (auto n : nums) cout << n << " ";
    cout << endl;

    set<int> unique_nums(nums.begin(), nums.end());
    cout << "去重后: ";
    for (auto n : unique_nums) cout << n << " ";
    cout << endl;

    // 应用2: 判断元素是否存在
    cout << "\n【字典查找】" << endl;
    set<string> dictionary = {"apple", "banana", "cherry", "date"};
    vector<string> words = {"apple", "grape", "banana", "kiwi"};
    for (const auto& word : words) {
        cout << "  " << word << ": "
             << (dictionary.count(word) ? "存在" : "不存在") << endl;
    }

    // 应用3: 集合运算（交集、并集、差集）
    cout << "\n【集合运算】" << endl;
    set<int> set_a = {1, 2, 3, 4, 5};
    set<int> set_b = {3, 4, 5, 6, 7};

    set<int> union_result;
    set_union(set_a.begin(), set_a.end(),
              set_b.begin(), set_b.end(),
              inserter(union_result, union_result.begin()));
    cout << "  并集: ";
    for (auto n : union_result) cout << n << " ";
    cout << endl;

    set<int> inter_result;
    set_intersection(set_a.begin(), set_a.end(),
                     set_b.begin(), set_b.end(),
                     inserter(inter_result, inter_result.begin()));
    cout << "  交集: ";
    for (auto n : inter_result) cout << n << " ";
    cout << endl;

    set<int> diff_result;
    set_difference(set_a.begin(), set_a.end(),
                   set_b.begin(), set_b.end(),
                   inserter(diff_result, diff_result.begin()));
    cout << "  差集(A-B): ";
    for (auto n : diff_result) cout << n << " ";
    cout << endl;

    // ==================== 9. 常见错误 ====================
    cout << "\n--- 8. 常见错误 ---" << endl;
    cout << "  错误1: 试图修改set中的元素 (*it = 100) => 编译错误" << endl;
    cout << "         元素是const的，需要先erase再insert" << endl;
    cout << "  错误2: 混淆set和multiset的insert行为" << endl;
    cout << "  错误3: 混淆lower_bound(>=)和upper_bound(>)" << endl;
    cout << "  错误4: 比较器不满足严格弱序 => 未定义行为" << endl;

    cout << "\n========================================" << endl;
    cout << "   set 和 multiset 详解完毕" << endl;
    cout << "========================================" << endl;

    return 0;
}
