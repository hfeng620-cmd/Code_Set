/*
 * 85_STL算术生成与集合算法.cpp
 * 本文件演示 STL 中的算术生成算法和集合算法
 * 包括: accumulate, inner_product, partial_sum, adjacent_difference,
 *       set_union, set_intersection, set_difference, merge, inplace_merge, includes
 * 注意: 部分算法在 <numeric> 头文件中
 */

#include <iostream>
#include <algorithm>
#include <numeric>      // accumulate, inner_product, partial_sum, adjacent_difference
#include <vector>
#include <functional>   // plus, minus
#include <iterator>     // ostream_iterator
using namespace std;

// 打印辅助函数
template<typename T>
void printVec(const string& label, const vector<T>& v) {
    cout << label << ": ";
    for (const auto& val : v) cout << val << " ";
    cout << endl;
}

int main() {
    cout << "========== 1. accumulate 累加算法 ==========" << endl;
    // accumulate: 计算区间内所有元素的累加和
    // 函数签名: accumulate(first, last, init)
    //           accumulate(first, last, init, binary_op)
    // init 是初始值
    vector<int> v = {1, 2, 3, 4, 5};

    // 基本用法: 求和
    int sum = accumulate(v.begin(), v.end(), 0);  // 初始值为0
    cout << "sum = " << sum << endl;  // 0+1+2+3+4+5 = 15

    // 带初始值的累加
    int sumFrom100 = accumulate(v.begin(), v.end(), 100);
    cout << "sum (init=100) = " << sumFrom100 << endl;  // 100+1+2+3+4+5 = 115

    // 自定义操作: 求积
    int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
    cout << "product = " << product << endl;  // 1*1*2*3*4*5 = 120

    // 自定义操作: Lambda 求平方和
    int squareSum = accumulate(v.begin(), v.end(), 0,
                               [](int acc, int val) { return acc + val * val; });
    cout << "平方和 = " << squareSum << endl;  // 1+4+9+16+25 = 55

    // 字符串拼接
    vector<string> words = {"Hello", " ", "World", "!"};
    string sentence = accumulate(words.begin(), words.end(), string(""));
    cout << "拼接结果: " << sentence << endl;

    // 使用 accumulate 求平均值
    vector<double> scores = {85.5, 90.0, 78.5, 92.0, 88.0};
    double avg = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
    cout << "平均分: " << avg << endl;

    cout << "\n========== 2. inner_product 内积算法 ==========" << endl;
    // inner_product: 计算两个序列的内积（点积）
    // 等价于: init + a1*b1 + a2*b2 + ... + an*bn
    vector<int> a = {1, 2, 3, 4, 5};
    vector<int> b = {10, 20, 30, 40, 50};

    // 基本内积: 1*10 + 2*20 + 3*30 + 4*40 + 5*50
    int dot = inner_product(a.begin(), a.end(), b.begin(), 0);
    cout << "内积 = " << dot << endl;  // 10+40+90+160+250 = 550

    // 自定义操作: init (+) a1(*)b1 (+) a2(*)b2 ...
    // 可以替换 + 和 * 为其他操作
    // 例如: 求两个向量对应元素最大值之和
    vector<int> x = {3, 1, 4, 1, 5};
    vector<int> y = {2, 7, 1, 8, 2};
    int customResult = inner_product(x.begin(), x.end(), y.begin(), 0,
                                     plus<int>(),          // 外层操作: 加法
                                     [](int a, int b) { return max(a, b); });  // 内层: 取最大
    cout << "max对应元素之和 = " << customResult << endl;  // 3+7+4+8+5 = 27

    cout << "\n========== 3. partial_sum 部分和算法 ==========" << endl;
    // partial_sum: 计算部分和（前缀和）
    // 结果[i] = 原始[0] + 原始[1] + ... + 原始[i]
    vector<int> v2 = {1, 2, 3, 4, 5};
    vector<int> prefixSum(v2.size());

    partial_sum(v2.begin(), v2.end(), prefixSum.begin());
    printVec("原始数据", v2);
    printVec("前缀和", prefixSum);  // 1, 3, 6, 10, 15

    // 自定义操作: 前缀积
    vector<int> prefixProd(v2.size());
    partial_sum(v2.begin(), v2.end(), prefixProd.begin(), multiplies<int>());
    printVec("前缀积", prefixProd);  // 1, 2, 6, 24, 120

    // 应用: 计算移动平均
    vector<double> temps = {20.0, 22.0, 19.0, 25.0, 23.0, 21.0, 24.0};
    vector<double> prefixTemp(temps.size());
    partial_sum(temps.begin(), temps.end(), prefixTemp.begin());
    cout << "温度前缀和: ";
    for (auto val : prefixTemp) cout << val << " ";
    cout << endl;

    cout << "\n========== 4. adjacent_difference 相邻差算法 ==========" << endl;
    // adjacent_difference: 计算相邻元素的差
    // 结果[0] = 原始[0]
    // 结果[i] = 原始[i] - 原始[i-1]  (i >= 1)
    vector<int> v3 = {1, 3, 6, 10, 15, 21};
    vector<int> diff(v3.size());

    adjacent_difference(v3.begin(), v3.end(), diff.begin());
    printVec("原始数据", v3);
    printVec("相邻差", diff);  // 1, 2, 3, 4, 5, 6

    // 逆操作: 从差分还原原始数据
    vector<int> restored(v3.size());
    partial_sum(diff.begin(), diff.end(), restored.begin());
    printVec("还原数据", restored);  // 1, 3, 6, 10, 15, 21

    // 自定义操作: 相邻比值
    vector<double> v4 = {1.0, 2.0, 6.0, 24.0, 120.0};
    vector<double> ratio(v4.size());
    adjacent_difference(v4.begin(), v4.end(), ratio.begin(), divides<double>());
    cout << "相邻比值: ";
    for (auto val : ratio) cout << val << " ";
    cout << endl;  // 1, 2, 3, 4, 5

    cout << "\n========== 5. set_union 集合并集 ==========" << endl;
    // set_union: 求两个已排序集合的并集
    // 前提: 两个集合必须已排序
    // 结果也已排序，重复元素取出现次数较多的那个
    vector<int> set1 = {1, 2, 3, 4, 5};
    vector<int> set2 = {3, 4, 5, 6, 7};
    vector<int> unionResult(set1.size() + set2.size());  // 足够大的空间

    auto unionEnd = set_union(set1.begin(), set1.end(),
                              set2.begin(), set2.end(),
                              unionResult.begin());
    cout << "set1: ";
    copy(set1.begin(), set1.end(), ostream_iterator<int>(cout, " "));
    cout << "\nset2: ";
    copy(set2.begin(), set2.end(), ostream_iterator<int>(cout, " "));
    cout << "\n并集: ";
    copy(unionResult.begin(), unionEnd, ostream_iterator<int>(cout, " "));
    cout << endl;

    cout << "\n========== 6. set_intersection 集合交集 ==========" << endl;
    // set_intersection: 求两个已排序集合的交集
    vector<int> interResult(min(set1.size(), set2.size()));
    auto interEnd = set_intersection(set1.begin(), set1.end(),
                                     set2.begin(), set2.end(),
                                     interResult.begin());
    cout << "交集: ";
    copy(interResult.begin(), interEnd, ostream_iterator<int>(cout, " "));
    cout << endl;

    cout << "\n========== 7. set_difference 集合差集 ==========" << endl;
    // set_difference: 求在 set1 中但不在 set2 中的元素
    // 注意: 差集不具有交换性，A-B != B-A
    vector<int> diffResult(set1.size());
    auto diffEnd = set_difference(set1.begin(), set1.end(),
                                  set2.begin(), set2.end(),
                                  diffResult.begin());
    cout << "差集 (set1 - set2): ";
    copy(diffResult.begin(), diffEnd, ostream_iterator<int>(cout, " "));
    cout << endl;

    // 反向差集
    vector<int> diffResult2(set2.size());
    auto diffEnd2 = set_difference(set2.begin(), set2.end(),
                                   set1.begin(), set1.end(),
                                   diffResult2.begin());
    cout << "差集 (set2 - set1): ";
    copy(diffResult2.begin(), diffEnd2, ostream_iterator<int>(cout, " "));
    cout << endl;

    // set_symmetric_difference: 对称差集（在A或B中，但不同时在两者中）
    vector<int> symDiff(set1.size() + set2.size());
    auto symEnd = set_symmetric_difference(set1.begin(), set1.end(),
                                           set2.begin(), set2.end(),
                                           symDiff.begin());
    cout << "对称差集: ";
    copy(symDiff.begin(), symEnd, ostream_iterator<int>(cout, " "));
    cout << endl;

    cout << "\n========== 8. merge 合并算法 ==========" << endl;
    // merge: 将两个已排序的序列合并为一个已排序的序列
    // 前提: 两个输入序列必须已排序
    vector<int> m1 = {1, 3, 5, 7, 9};
    vector<int> m2 = {2, 4, 6, 8, 10};
    vector<int> merged(m1.size() + m2.size());

    merge(m1.begin(), m1.end(), m2.begin(), m2.end(), merged.begin());
    printVec("merge 合并", merged);  // 1,2,3,4,5,6,7,8,9,10

    // merge 与 set_union 的区别:
    // - merge: 总是取所有元素（包括重复）
    // - set_union: 重复元素只取一次（取出现次数较多的那个）
    vector<int> dup1 = {1, 2, 2, 3};
    vector<int> dup2 = {2, 2, 3, 4};
    vector<int> mergedDup(dup1.size() + dup2.size());
    vector<int> unionDup(dup1.size() + dup2.size());

    merge(dup1.begin(), dup1.end(), dup2.begin(), dup2.end(), mergedDup.begin());
    auto unionDupEnd = set_union(dup1.begin(), dup1.end(),
                                 dup2.begin(), dup2.end(), unionDup.begin());
    printVec("merge (有重复)", mergedDup);
    cout << "set_union: ";
    copy(unionDup.begin(), unionDupEnd, ostream_iterator<int>(cout, " "));
    cout << endl;

    // inplace_merge: 原地合并一个序列中两个已排序的部分
    vector<int> v5 = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
    // 前半部分 [0,5) 已排序，后半部分 [5,10) 已排序
    inplace_merge(v5.begin(), v5.begin() + 5, v5.end());
    printVec("inplace_merge", v5);

    cout << "\n========== 9. includes 包含判断 ==========" << endl;
    // includes: 判断一个已排序序列是否包含另一个已排序序列
    // 即 set2 是否是 set1 的子集
    vector<int> big = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> small1 = {2, 4, 6, 8, 10};
    vector<int> small2 = {2, 4, 11};

    cout << "big 是否包含 small1: "
         << (includes(big.begin(), big.end(), small1.begin(), small1.end()) ? "是" : "否")
         << endl;
    cout << "big 是否包含 small2: "
         << (includes(big.begin(), big.end(), small2.begin(), small2.end()) ? "是" : "否")
         << endl;

    cout << "\n========== 10. 常见错误 ==========" << endl;
    // 错误1: 对未排序的集合使用集合算法
    // set_union, set_intersection, set_difference, merge, includes 都要求输入已排序
    // vector<int> unsorted = {3, 1, 2};
    // set_union(unsorted.begin(), ...);  // 结果不可靠！

    // 错误2: 目标容器空间不足
    // vector<int> tooSmall(3);
    // merge(m1.begin(), m1.end(), m2.begin(), m2.end(), tooSmall.begin());
    // m1.size()+m2.size() = 10，但 tooSmall 只有3个元素，会越界！

    // 错误3: accumulate 的初始值类型决定了累加类型
    // 如果用 int 初始值累加 double，会丢失精度
    // vector<double> vd = {1.1, 2.2, 3.3};
    // int bad_sum = accumulate(vd.begin(), vd.end(), 0);  // 结果是6，不是6.6
    // 正确: accumulate(vd.begin(), vd.end(), 0.0)

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
