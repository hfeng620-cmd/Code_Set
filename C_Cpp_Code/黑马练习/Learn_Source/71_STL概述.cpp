/*
 * 文件名: 71_STL概述.cpp
 * 描述: STL (Standard Template Library) 标准模板库概述
 *       介绍STL的组成、核心概念及基本用法
 * 编译: g++ -std=c++11 71_STL概述.cpp -o 71_STL概述
 */

#include <iostream>
#include <vector>      // 向量容器头文件
#include <algorithm>   // 算法头文件 (sort, find, for_each等)
#include <numeric>     // 数值算法头文件 (accumulate等)
#include <functional>  // 函数对象/仿函数头文件
#include <iterator>    // 迭代器头文件

using namespace std;

/*
 * ============================================================
 * 一、什么是STL？
 * ============================================================
 * STL (Standard Template Library) 标准模板库，是C++标准库的核心组成部分。
 * 它提供了一套通用的模板类和函数，用于实现常见的数据结构和算法。
 *
 * STL的设计理念:
 *   - 数据结构和算法分离，通过迭代器(Iterator)进行连接
 *   - 使用模板技术实现泛型编程，一套代码适用于多种数据类型
 *   - 高度可复用，经过充分优化，性能优异
 */

/*
 * ============================================================
 * 二、STL的六大组件
 * ============================================================
 * 1. 容器(Container)   - 存储数据的数据结构
 *    - 序列容器: vector, deque, list, array, forward_list
 *    - 关联容器: set, multiset, map, multimap
 *    - 无序容器: unordered_set, unordered_map等
 *
 * 2. 算法(Algorithm)   - 操作数据的函数模板
 *    - 非修改算法: find, count, search, for_each等
 *    - 修改算法:   transform, copy, fill, replace等
 *    - 排序算法:   sort, stable_sort, partial_sort等
 *    - 数值算法:   accumulate, inner_product等
 *
 * 3. 迭代器(Iterator)  - 连接容器和算法的桥梁
 *    - 本质是泛化的指针，提供统一的遍历接口
 *    - 类别: 输入迭代器、输出迭代器、前向迭代器、双向迭代器、随机访问迭代器
 *
 * 4. 仿函数(Functor)   - 重载了()运算符的类对象
 *    - 可以像函数一样被调用，常用于算法的自定义比较规则
 *
 * 5. 适配器(Adapter)   - 对已有组件进行接口转换
 *    - 容器适配器: stack, queue, priority_queue
 *    - 迭代器适配器: reverse_iterator, insert_iterator
 *    - 仿函数适配器: bind1st, bind2nd (C++11后用bind替代)
 *
 * 6. 分配器(Allocator) - 负责内存的分配和释放
 *    - 一般使用默认分配器即可，无需手动指定
 */

// ==================== 示例函数定义 ====================

// 仿函数示例: 自定义比较规则
struct Descending {
    // 重载()运算符，实现降序比较
    bool operator()(int a, int b) const {
        return a > b;  // 降序排列
    }
};

// 仿函数示例: 打印元素
struct Printer {
    void operator()(int val) const {
        cout << val << " ";
    }
};

// ==================== 主函数 ====================

int main() {
    cout << "========== 一、STL基本使用：vector示例 ==========" << endl;

    // 创建一个vector容器（动态数组）
    vector<int> vec;

    // 使用push_back向容器尾部添加元素
    vec.push_back(30);
    vec.push_back(10);
    vec.push_back(50);
    vec.push_back(20);
    vec.push_back(40);

    // 使用迭代器遍历容器
    cout << "原始数据: ";
    for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // C++11 新式迭代器写法 (auto自动推导类型)
    cout << "auto迭代器: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // C++11 范围for循环 (最简洁的遍历方式)
    cout << "范围for: ";
    for (const auto& val : vec) {
        cout << val << " ";
    }
    cout << endl;

    cout << "\n========== 二、STL算法示例 ==========" << endl;

    // sort: 排序算法（默认升序）
    sort(vec.begin(), vec.end());
    cout << "升序排序: ";
    for (const auto& val : vec) {
        cout << val << " ";
    }
    cout << endl;

    // 使用仿函数进行降序排序
    sort(vec.begin(), vec.end(), Descending());
    cout << "降序排序: ";
    for (const auto& val : vec) {
        cout << val << " ";
    }
    cout << endl;

    // find: 查找算法
    auto it = find(vec.begin(), vec.end(), 30);
    if (it != vec.end()) {
        cout << "找到元素30，位置索引: " << distance(vec.begin(), it) << endl;
    } else {
        cout << "未找到元素30" << endl;
    }

    // count: 计数算法
    int cnt = count(vec.begin(), vec.end(), 30);
    cout << "元素30出现次数: " << cnt << endl;

    // accumulate: 数值累加算法 (需要#include <numeric>)
    int sum = accumulate(vec.begin(), vec.end(), 0);
    cout << "所有元素之和: " << sum << endl;

    // for_each: 遍历算法，对每个元素执行操作
    cout << "for_each打印: ";
    for_each(vec.begin(), vec.end(), Printer());
    cout << endl;

    cout << "\n========== 三、更多STL容器示例 ==========" << endl;

    // --- map容器示例 ---
    // 注意：此处仅作演示，详细用法在后续课程讲解
    // map是关联容器，存储键值对，按键自动排序
    // 详细内容请参考 79_map和multimap详解.cpp

    // --- set容器示例 ---
    // set是关联容器，存储唯一元素，自动排序
    // 详细内容请参考 78_set和multiset详解.cpp

    cout << "\n========== 四、STL的优势 ==========" << endl;
    /*
     * 为什么使用STL？
     *
     * 1. 代码复用: STL提供了大量现成的数据结构和算法，无需重复造轮子
     *
     * 2. 高性能: STL的实现经过高度优化，性能接近手写C代码
     *    例如vector的push_back在摊还分析下是O(1)的
     *
     * 3. 类型安全: 模板在编译期进行类型检查，比void*指针更安全
     *
     * 4. 跨平台: STL是C++标准的一部分，所有标准编译器都支持
     *
     * 5. 可扩展: 可以自定义容器、算法、仿函数等来扩展STL
     *
     * 6. 易于学习: 统一的接口设计，学会一个容器后容易触类旁通
     */

    cout << "STL是C++程序员必须掌握的核心技术!" << endl;
    cout << "建议学习顺序: vector -> string -> deque -> stack/queue -> list -> set/map -> priority_queue" << endl;

    cout << "\n========== 五、常见错误和注意事项 ==========" << endl;
    /*
     * 常见错误1: 忘记包含头文件
     *   错误: 使用vector但没有 #include <vector>
     *   正确: 每种容器都有对应的头文件，必须包含
     *
     * 常见错误2: 迭代器失效
     *   错误: 在遍历容器时删除元素，导致迭代器失效
     *   正确: 删除元素后，使用erase返回的新迭代器继续遍历
     *   示例:
     *     for (auto it = vec.begin(); it != vec.end(); ) {
     *         if (*it == 目标值)
     *             it = vec.erase(it);  // erase返回下一个有效迭代器
     *         else
     *             ++it;
     *     }
     *
     * 常见错误3: 越界访问
     *   错误: 使用[]访问不存在的索引
     *   正确: 使用at()进行带边界检查的访问，或先检查size()
     *
     * 常见错误4: 混淆容器的end()
     *   注意: end()指向最后一个元素的下一个位置（尾后迭代器）
     *         不能对end()进行解引用操作
     *
     * 常见错误5: 算法与容器不匹配
     *   注意: 某些算法需要特定类型的迭代器
     *         例如sort需要随机访问迭代器，不能用于list
     *         list有自己的sort()成员函数
     */

    cout << "本文件演示完毕，请继续学习后续STL容器详解!" << endl;

    return 0;
}
