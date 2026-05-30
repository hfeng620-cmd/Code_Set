/*
 * 81_STL遍历算法.cpp
 * 本文件演示 STL 中常用的遍历与查找算法
 * 包括: for_each, transform, find, find_if, count, count_if, all_of, any_of, none_of
 * 以及 Lambda 表达式在算法中的应用
 */

#include <iostream>
#include <algorithm>   // STL 算法头文件
#include <vector>
#include <list>
#include <numeric>     // accumulate 等数值算法
using namespace std;

// ============ 自定义函数 / 函子，用于 for_each ============

// 普通函数：打印元素
void printFunc(int val) {
    cout << val << " ";
}

// 普通函数：元素翻倍（用于 transform）
int doubleFunc(int val) {
    return val * 2;
}

// 函子（仿函数）：累加器
class Accumulator {
public:
    Accumulator() : sum(0) {}
    void operator()(int val) {
        sum += val;
        cout << val << " ";  // 同时打印
    }
    int sum;  // 保存累加结果
};

// 函子：判断是否为偶数
class IsEven {
public:
    bool operator()(int val) const {
        return val % 2 == 0;
    }
};

int main() {
    cout << "========== 1. for_each 遍历算法 ==========" << endl;
    // for_each: 对容器中每个元素执行指定操作
    // 函数签名: for_each(InputIterator first, InputIterator last, Function fn)
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "使用普通函数打印: ";
    for_each(v.begin(), v.end(), printFunc);
    cout << endl;

    // 使用函子（仿函数）作为 for_each 的操作
    // 注意：for_each 返回的是函数对象的副本，所以可以获取累加结果
    Accumulator acc;
    acc = for_each(v.begin(), v.end(), acc);
    cout << "\n累加结果: " << acc.sum << endl;

    cout << "\n使用 Lambda 表达式打印: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val * 10 << " ";  // 打印每个元素乘以10
    });
    cout << endl;

    cout << "\n========== 2. transform 变换算法 ==========" << endl;
    // transform: 对每个元素执行操作，将结果存入目标容器
    // 函数签名: transform(InputIt first, InputIt last, OutputIt d_first, UnaryOperation op)
    vector<int> source = {1, 2, 3, 4, 5};
    vector<int> dest(source.size());  // 目标容器必须有足够的空间

    // 使用普通函数进行变换
    transform(source.begin(), source.end(), dest.begin(), doubleFunc);
    cout << "原始数据: ";
    for_each(source.begin(), source.end(), printFunc);
    cout << "\n翻倍后:   ";
    for_each(dest.begin(), dest.end(), printFunc);
    cout << endl;

    // 使用 Lambda 进行变换：元素平方
    vector<int> square(dest.size());
    transform(source.begin(), source.end(), square.begin(),
              [](int val) { return val * val; });
    cout << "平方后:   ";
    for_each(square.begin(), square.end(), printFunc);
    cout << endl;

    // 双参数 transform：将两个容器对应元素相加
    vector<int> a = {1, 2, 3, 4, 5};
    vector<int> b = {10, 20, 30, 40, 50};
    vector<int> result(a.size());
    transform(a.begin(), a.end(), b.begin(), result.begin(),
              [](int x, int y) { return x + y; });
    cout << "a + b = ";
    for_each(result.begin(), result.end(), printFunc);
    cout << endl;

    cout << "\n========== 3. find / find_if 查找算法 ==========" << endl;
    // find: 查找第一个等于指定值的元素
    // 返回指向该元素的迭代器，未找到则返回 end()
    vector<int> data = {10, 20, 30, 40, 50, 30, 60};

    auto it = find(data.begin(), data.end(), 30);
    if (it != data.end()) {
        cout << "找到元素 30, 位置索引: " << distance(data.begin(), it) << endl;
    } else {
        cout << "未找到元素 30" << endl;
    }

    // find_if: 查找第一个满足条件的元素
    // 使用 Lambda 作为谓词（判断条件）
    auto it2 = find_if(data.begin(), data.end(),
                       [](int val) { return val > 35; });
    if (it2 != data.end()) {
        cout << "第一个大于35的元素: " << *it2
             << ", 索引: " << distance(data.begin(), it2) << endl;
    }

    // find_if_not: 查找第一个不满足条件的元素（C++11）
    auto it3 = find_if_not(data.begin(), data.end(),
                           [](int val) { return val < 35; });
    if (it3 != data.end()) {
        cout << "第一个不小于35的元素: " << *it3 << endl;
    }

    cout << "\n========== 4. count / count_if 计数算法 ==========" << endl;
    // count: 统计等于指定值的元素个数
    vector<int> nums = {1, 2, 3, 2, 4, 2, 5, 2};
    int cnt = count(nums.begin(), nums.end(), 2);
    cout << "元素 2 出现次数: " << cnt << endl;

    // count_if: 统计满足条件的元素个数
    int evenCnt = count_if(nums.begin(), nums.end(),
                           [](int val) { return val % 2 == 0; });
    cout << "偶数个数: " << evenCnt << endl;

    int largeCnt = count_if(nums.begin(), nums.end(),
                            bind2nd(greater<int>(), 3));  // C++17前的写法
    // C++11 推荐用 Lambda:
    // count_if(nums.begin(), nums.end(), [](int v){ return v > 3; });
    cout << "大于3的元素个数: " << largeCnt << endl;

    cout << "\n========== 5. all_of / any_of / none_of 条件判断 ==========" << endl;
    // all_of: 是否所有元素都满足条件
    // any_of: 是否存在至少一个元素满足条件
    // none_of: 是否没有任何元素满足条件
    vector<int> scores = {85, 90, 78, 92, 88};

    bool allPass = all_of(scores.begin(), scores.end(),
                          [](int s) { return s >= 60; });
    cout << "是否全部及格(>=60): " << (allPass ? "是" : "否") << endl;

    bool anyExcellent = any_of(scores.begin(), scores.end(),
                               [](int s) { return s >= 90; });
    cout << "是否有优秀(>=90): " << (anyExcellent ? "是" : "否") << endl;

    bool noneFail = none_of(scores.begin(), scores.end(),
                            [](int s) { return s < 60; });
    cout << "是否无人不及格: " << (noneFail ? "是" : "否") << endl;

    cout << "\n========== 6. Lambda 表达式进阶用法 ==========" << endl;
    // Lambda 表达式可以捕获外部变量
    int threshold = 100;
    vector<int> prices = {50, 120, 80, 200, 150, 90};

    // 值捕获: [threshold] 捕获 threshold 的值
    int expensiveCnt = count_if(prices.begin(), prices.end(),
                                [threshold](int p) { return p > threshold; });
    cout << "价格超过 " << threshold << " 的商品数: " << expensiveCnt << endl;

    // 引用捕获: [&threshold] 捕获 threshold 的引用
    threshold = 150;  // 修改外部变量
    expensiveCnt = count_if(prices.begin(), prices.end(),
                            [&threshold](int p) { return p > threshold; });
    cout << "价格超过 " << threshold << " 的商品数: " << expensiveCnt << endl;

    // 混合捕获: [=] 值捕获所有, [&] 引用捕获所有
    int base = 10;
    vector<int> vals = {1, 2, 3, 4, 5};
    // [=] 值捕获所有外部变量
    for_each(vals.begin(), vals.end(), [=](int v) {
        // base 是值捕获，不能修改
        cout << v + base << " ";
    });
    cout << endl;

    // [&] 引用捕获所有外部变量
    int total = 0;
    for_each(vals.begin(), vals.end(), [&total](int v) {
        total += v;  // 可以修改外部变量
    });
    cout << "总和: " << total << endl;

    // mutable Lambda: 允许修改值捕获的变量（仅影响 Lambda 内部副本）
    int counter = 0;
    auto incrementer = [counter]() mutable -> int {
        return ++counter;  // mutable 允许修改值捕获的变量
    };
    cout << "mutable Lambda: " << incrementer() << ", " << incrementer() << endl;
    cout << "外部 counter 仍为: " << counter << endl;  // 外部不变

    cout << "\n========== 7. 常见错误示例 ==========" << endl;
    // 错误1: transform 目标容器未分配空间
    // vector<int> bad_dest;
    // transform(source.begin(), source.end(), bad_dest.begin(), doubleFunc);
    // 这会导致未定义行为！bad_dest 为空，begin() == end()

    // 错误2: find 返回值未检查就解引用
    // auto bad_it = find(data.begin(), data.end(), 999);
    // cout << *bad_it;  // 如果未找到，解引用 end() 是未定义行为！

    // 错误3: remove 只是移动元素，不会真正删除
    // 正确做法是 erase-remove 惯用法（见后续文件）

    cout << "请查看源码中的注释了解常见错误" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
