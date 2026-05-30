/*
 * 83_STL拷贝替换删除算法.cpp
 * 本文件演示 STL 中常用的拷贝、替换、删除算法
 * 包括: copy, copy_if, replace, replace_if, remove, remove_if (erase-remove),
 *       unique, swap, iter_swap, fill, generate
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <iterator>    // ostream_iterator
#include <numeric>     // iota
using namespace std;

// 打印辅助函数
template<typename T>
void printVec(const string& label, const vector<T>& v) {
    cout << label << ": ";
    for (const auto& val : v) cout << val << " ";
    cout << " (size=" << v.size() << ")" << endl;
}

// 生成器函数：产生斐波那契数列
class Fibonacci {
public:
    Fibonacci() : a(0), b(1) {}
    int operator()() {
        int result = a;
        int temp = a + b;
        a = b;
        b = temp;
        return result;
    }
private:
    int a, b;
};

int main() {
    cout << "========== 1. copy 拷贝算法 ==========" << endl;
    // copy: 将源区间元素拷贝到目标区间
    // 函数签名: copy(InputIt first, InputIt last, OutputIt d_first)
    // 注意: 目标容器必须有足够空间！
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst(src.size());  // 必须预先分配空间

    copy(src.begin(), src.end(), dst.begin());
    printVec("copy 结果", dst);

    // 使用 copy + ostream_iterator 直接输出到 cout
    cout << "copy到输出流: ";
    copy(src.begin(), src.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    // copy_if: 只拷贝满足条件的元素（C++11）
    vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> evenData;  // 空容器

    // 注意: 不能直接 copy_if 到空容器的 begin()，因为没有空间
    // 正确做法: 使用 back_inserter 插入迭代器
    copy_if(data.begin(), data.end(), back_inserter(evenData),
            [](int val) { return val % 2 == 0; });
    printVec("copy_if 偶数", evenData);

    // back_inserter: 自动在容器尾部插入元素，无需预分配空间
    vector<int> largeNums;
    copy_if(data.begin(), data.end(), back_inserter(largeNums),
            [](int val) { return val > 5; });
    printVec("copy_if >5", largeNums);

    cout << "\n========== 2. replace 替换算法 ==========" << endl;
    // replace: 将所有等于 old_value 的元素替换为 new_value
    vector<int> v1 = {1, 2, 3, 2, 4, 2, 5};
    printVec("替换前", v1);

    replace(v1.begin(), v1.end(), 2, 20);  // 将所有2替换为20
    printVec("replace 2->20", v1);

    // replace_if: 只替换满足条件的元素
    vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printVec("替换前", v2);

    replace_if(v2.begin(), v2.end(),
               [](int val) { return val % 2 == 0; },  // 条件: 偶数
               0);                                     // 替换为0
    printVec("replace_if 偶数->0", v2);

    // replace_copy: 不修改原容器，将替换结果存入新容器
    vector<int> v3 = {1, 2, 3, 2, 4, 2, 5};
    vector<int> v3_copy(v3.size());
    replace_copy(v3.begin(), v3.end(), v3_copy.begin(), 2, 99);
    printVec("原容器不变", v3);
    printVec("replace_copy 2->99", v3_copy);

    cout << "\n========== 3. remove / remove_if 删除算法 ==========" << endl;
    // 重要: remove 并不真正删除元素！它只是将不需要删除的元素前移
    // 返回新的逻辑结尾迭代器
    vector<int> v4 = {1, 2, 3, 2, 4, 2, 5};
    printVec("删除前", v4);

    // remove 的工作原理:
    // 原始: 1 2 3 2 4 2 5
    // 结果: 1 3 4 5 [2 2 5]  <- 被"删除"的元素移到末尾
    auto newEnd = remove(v4.begin(), v4.end(), 2);
    cout << "remove 后容器大小: " << v4.size() << " (未改变!)" << endl;
    cout << "remove 到 newEnd 之间的元素(垃圾): ";
    for (auto it = newEnd; it != v4.end(); ++it)
        cout << *it << " ";
    cout << endl;

    // erase-remove 惯用法: 真正删除元素
    // 这是 STL 中最常用的模式之一
    vector<int> v5 = {1, 2, 3, 2, 4, 2, 5};
    printVec("erase-remove 前", v5);
    v5.erase(remove(v5.begin(), v5.end(), 2), v5.end());
    printVec("erase-remove 删除2后", v5);

    // remove_if: 按条件删除
    vector<int> v6 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printVec("删除前", v6);
    v6.erase(remove_if(v6.begin(), v6.end(),
                       [](int val) { return val % 2 == 0; }),  // 删除偶数
             v6.end());
    printVec("erase-remove_if 偶数", v6);

    // remove_copy_if: 条件删除并拷贝（不修改原容器）
    vector<int> v7 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> v7_result;
    remove_copy_if(v7.begin(), v7.end(), back_inserter(v7_result),
                   [](int val) { return val <= 5; });  // 保留 >5 的元素
    printVec("remove_copy_if 保留>5", v7_result);

    cout << "\n========== 4. unique 去重算法 ==========" << endl;
    // unique: 移除相邻的重复元素
    // 重要: 必须先排序再去重！因为 unique 只移除"相邻"的重复元素
    vector<int> v8 = {1, 3, 2, 3, 1, 2, 3, 3, 2, 1};
    printVec("去重前", v8);

    // 错误用法: 没排序直接 unique
    auto badEnd = unique(v8.begin(), v8.end());
    cout << "未排序直接 unique（错误用法）: ";
    for (auto it = v8.begin(); it != badEnd; ++it)
        cout << *it << " ";
    cout << endl;

    // 正确用法: 先排序再去重
    vector<int> v9 = {1, 3, 2, 3, 1, 2, 3, 3, 2, 1};
    sort(v9.begin(), v9.end());
    printVec("排序后", v9);
    v9.erase(unique(v9.begin(), v9.end()), v9.end());  // erase-remove 惯用法
    printVec("sort+unique 去重", v9);

    // unique 也可以用于字符串去重
    string str = "aabbccddaa";
    sort(str.begin(), str.end());
    str.erase(unique(str.begin(), str.end()), str.end());
    cout << "字符串去重: " << str << endl;

    cout << "\n========== 5. swap / iter_swap 交换算法 ==========" << endl;
    // swap: 交换两个变量的值
    int a = 10, b = 20;
    cout << "交换前: a=" << a << ", b=" << b << endl;
    swap(a, b);
    cout << "交换后: a=" << a << ", b=" << b << endl;

    // swap 也可以用于容器（高效，O(1)复杂度）
    vector<int> v10 = {1, 2, 3};
    vector<int> v11 = {4, 5, 6, 7, 8};
    swap(v10, v11);  // 交换整个容器
    printVec("v10", v10);
    printVec("v11", v11);

    // iter_swap: 交换两个迭代器指向的元素
    vector<int> v12 = {1, 2, 3, 4, 5};
    iter_swap(v12.begin(), v12.begin() + 4);  // 交换第一个和第五个元素
    printVec("iter_swap 第1和第5", v12);

    // iter_swap 实现手动选择排序
    vector<int> v13 = {5, 3, 1, 4, 2};
    for (auto i = v13.begin(); i != v13.end(); ++i) {
        auto minIt = min_element(i, v13.end());
        if (minIt != i) iter_swap(i, minIt);
    }
    printVec("手动选择排序", v13);

    cout << "\n========== 6. fill 填充算法 ==========" << endl;
    // fill: 用指定值填充区间
    vector<int> v14(10);
    fill(v14.begin(), v14.end(), 42);
    printVec("fill 42", v14);

    // fill_n: 填充前 n 个元素
    vector<int> v15(10, 0);
    fill_n(v15.begin(), 5, 99);  // 填充前5个为99
    printVec("fill_n 前5个为99", v15);

    // 常见错误: fill_n 的 n 超出容器大小
    // fill_n(v15.begin(), 20, 1);  // 未定义行为！会越界写入

    cout << "\n========== 7. generate 生成算法 ==========" << endl;
    // generate: 用函数对象的返回值填充区间
    // 每次调用函数对象，用其返回值填充下一个元素
    vector<int> v16(10);
    int counter = 1;
    generate(v16.begin(), v16.end(), [&counter]() { return counter++; });
    printVec("generate 递增", v16);

    // 使用斐波那契生成器
    vector<int> fib(10);
    generate(fib.begin(), fib.end(), Fibonacci());
    printVec("generate 斐波那契", fib);

    // generate_n: 生成前 n 个元素
    vector<int> v17;
    generate_n(back_inserter(v17), 8, [n = 0]() mutable { return n * n; });
    printVec("generate_n 平方数", v17);

    cout << "\n========== 8. 常见错误总结 ==========" << endl;
    // 错误1: copy 到未分配空间的容器
    // vector<int> empty;
    // copy(src.begin(), src.end(), empty.begin());  // 未定义行为!
    // 正确: 使用 back_inserter(empty)

    // 错误2: remove 后不 erase
    // vector<int> v = {1,2,3,2,4};
    // remove(v.begin(), v.end(), 2);  // 元素没有真正删除!
    // 正确: v.erase(remove(v.begin(), v.end(), 2), v.end());

    // 错误3: unique 前不排序
    // 只有相邻重复才会被移除

    // 错误4: fill_n 的 n 超出范围
    // 会导致越界写入

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
