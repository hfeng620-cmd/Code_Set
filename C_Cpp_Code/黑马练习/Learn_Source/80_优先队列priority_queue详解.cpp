/*
 ============================================================================
 文件名 : 80_优先队列priority_queue详解.cpp
 描  述 : 详细讲解C++ STL priority_queue 优先队列（堆）的用法
          包括：默认最大堆、push/pop/top、最小堆写法、
          自定义比较器（仿函数/lambda）、Top K 问题应用、
          合并K个有序列表应用
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 80_优先队列priority_queue详解.cpp -o 80_优先队列priority_queue详解
 ============================================================================
*/

#include <iostream>
#include <queue>
#include <vector>
#include <functional>  // greater<>
#include <string>
using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "  priority_queue 优先队列详解 演示" << endl;
    cout << "========================================" << endl;

    // ==================== 1. 基础介绍 ====================
    /*
     * priority_queue 优先队列，也叫堆（Heap）
     *
     * 特点:
     *   1. 每次取出的元素都是优先级最高的
     *   2. 默认是最大堆（大顶堆）：最大的元素在顶部
     *   3. 底层实现：vector + 堆算法
     *   4. 插入和删除：O(log n)
     *   5. 查看顶部元素：O(1)
     *
     * 与 queue 的区别:
     *   queue:          FIFO，先进先出
     *   priority_queue: 按优先级出队，最高优先级先出
     *
     * 模板参数:
     *   template<
     *       class T,                             // 元素类型
     *       class Container = vector<T>,         // 底层容器
     *       class Compare = less<T>              // 比较器
     *   > class priority_queue;
     */

    // ==================== 2. 最大堆（默认） ====================
    cout << "\n--- 1. 最大堆（默认） ---" << endl;
    priority_queue<int> max_heap;

    // push: 插入元素，O(log n)
    max_heap.push(30);
    max_heap.push(10);
    max_heap.push(50);
    max_heap.push(20);
    max_heap.push(40);

    cout << "堆大小: " << max_heap.size() << endl;
    cout << "堆顶(最大值): " << max_heap.top() << endl;  // O(1)

    // pop: 弹出堆顶元素，O(log n)
    // 注意：pop() 不返回值！需要先 top() 再 pop()
    cout << "依次弹出(降序): ";
    while (!max_heap.empty()) {
        cout << max_heap.top() << " ";
        max_heap.pop();
    }
    cout << endl;

    // ==================== 3. 最小堆 ====================
    cout << "\n--- 2. 最小堆 ---" << endl;
    /*
     * 最小堆（小顶堆）：
     *   最小的元素在顶部
     *   使用 greater<T> 比较器
     *   需要指定所有三个模板参数
     *
     * 常见错误：
     *   priority_queue<int, greater<int>> wrong;  // 错误！缺少Container
     *   priority_queue<int, vector<int>, greater<int>> correct;  // 正确
     */
    priority_queue<int, vector<int>, greater<int>> min_heap;
    min_heap.push(30);
    min_heap.push(10);
    min_heap.push(50);
    min_heap.push(20);
    min_heap.push(40);

    cout << "堆顶(最小值): " << min_heap.top() << endl;
    cout << "依次弹出(升序): ";
    while (!min_heap.empty()) {
        cout << min_heap.top() << " ";
        min_heap.pop();
    }
    cout << endl;

    // ==================== 4. 自定义比较器 ====================
    cout << "\n--- 3. 自定义比较器 ---" << endl;

    // 方法1: 仿函数（函数对象）
    struct CompareGreater {
        bool operator()(int a, int b) const {
            return a > b;  // a > b 时 a 优先级低，实现最小堆
        }
    };
    priority_queue<int, vector<int>, CompareGreater> custom1;
    custom1.push(30); custom1.push(10); custom1.push(50);
    cout << "仿函数最小堆堆顶: " << custom1.top() << endl;

    // 方法2: lambda 表达式（C++11）
    // 使用 lambda 时需要 decltype 获取类型，并将 lambda 传给构造函数
    auto cmp = [](int a, int b) { return a > b; };
    priority_queue<int, vector<int>, decltype(cmp)> custom2(cmp);
    custom2.push(30); custom2.push(10); custom2.push(50);
    cout << "lambda最小堆堆顶: " << custom2.top() << endl;

    // 方法3: 对结构体使用自定义比较器
    struct Task {
        string name;
        int priority;
        string toString() const {
            return name + "(优先级:" + to_string(priority) + ")";
        }
    };

    struct TaskCompare {
        bool operator()(const Task& a, const Task& b) const {
            return a.priority < b.priority;  // 高优先级在前
        }
    };

    priority_queue<Task, vector<Task>, TaskCompare> task_queue;
    task_queue.push({"低优先级任务", 1});
    task_queue.push({"高优先级任务", 10});
    task_queue.push({"中优先级任务", 5});
    task_queue.push({"紧急任务", 100});

    cout << "任务队列:" << endl;
    while (!task_queue.empty()) {
        cout << "  处理: " << task_queue.top().toString() << endl;
        task_queue.pop();
    }

    // ==================== 5. 基本操作详解 ====================
    cout << "\n--- 4. 基本操作详解 ---" << endl;
    priority_queue<int> pq;

    cout << "是否为空: " << (pq.empty() ? "是" : "否") << endl;
    pq.push(10); pq.push(30); pq.push(20); pq.push(50); pq.push(40);
    cout << "插入5个元素后: size=" << pq.size() << ", top=" << pq.top() << endl;

    // top() 返回的是引用，可以修改堆顶
    pq.top() = 100;
    cout << "修改堆顶为100: top=" << pq.top() << endl;

    pq.pop();
    cout << "pop后: top=" << pq.top() << endl;

    // emplace: 原地构造（C++11）
    pq.emplace(60);

    // ==================== 6. priority_queue 不支持迭代 ====================
    cout << "\n--- 5. 不支持迭代 ---" << endl;
    /*
     * 重要：priority_queue 不提供迭代器！
     * 只能访问堆顶(top)，不能遍历内部元素
     * 如需查看所有元素，只能复制后逐个 pop
     */
    priority_queue<int> iter_pq;
    iter_pq.push(1); iter_pq.push(2); iter_pq.push(3);

    // iter_pq.begin();  // 错误！没有迭代器

    priority_queue<int> temp = iter_pq;  // 复制
    cout << "所有元素(降序): ";
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;

    // ==================== 7. 应用1: Top K 问题 ====================
    cout << "\n--- 6. 应用1: Top K 问题 ---" << endl;
    /*
     * Top K 问题：找出数组中最大的 K 个元素
     *
     * 算法：使用最小堆
     *   1. 维护一个大小为 K 的最小堆
     *   2. 遍历数组，如果当前元素大于堆顶，则替换堆顶
     *   3. 最终堆中就是最大的 K 个元素
     *
     * 为什么用最小堆？
     *   堆顶是最小的元素，方便判断是否需要替换
     *   时间复杂度: O(n log k)
     *   空间复杂度: O(k)
     */
    auto topK = [](const vector<int>& nums, int k) -> vector<int> {
        priority_queue<int, vector<int>, greater<int>> min_h;
        for (int num : nums) {
            if ((int)min_h.size() < k) {
                min_h.push(num);
            } else if (num > min_h.top()) {
                min_h.pop();
                min_h.push(num);
            }
        }
        vector<int> result;
        while (!min_h.empty()) {
            result.push_back(min_h.top());
            min_h.pop();
        }
        return result;
    };

    vector<int> data = {3, 1, 5, 12, 2, 11, 7, 9, 4, 8};
    int k = 3;
    cout << "数组: ";
    for (int n : data) cout << n << " ";
    cout << endl;

    vector<int> result = topK(data, k);
    cout << "最大的" << k << "个元素: ";
    for (int n : result) cout << n << " ";
    cout << endl;

    // Top K 最小（使用最大堆）
    auto topKSmallest = [](const vector<int>& nums, int kk) -> vector<int> {
        priority_queue<int> max_h;
        for (int num : nums) {
            if ((int)max_h.size() < kk) {
                max_h.push(num);
            } else if (num < max_h.top()) {
                max_h.pop();
                max_h.push(num);
            }
        }
        vector<int> res;
        while (!max_h.empty()) {
            res.push_back(max_h.top());
            max_h.pop();
        }
        return res;
    };

    result = topKSmallest(data, k);
    cout << "最小的" << k << "个元素: ";
    for (int n : result) cout << n << " ";
    cout << endl;

    // ==================== 8. 应用2: 合并K个有序列表 ====================
    cout << "\n--- 7. 应用2: 合并K个有序数组 ---" << endl;
    /*
     * 算法思路:
     *   1. 将每个数组的第一个元素放入最小堆
     *   2. 每次取出堆顶（最小值），加入结果
     *   3. 如果取出的元素所在数组还有下一个，将下一个加入堆
     *   4. 重复直到堆为空
     *   时间复杂度: O(N log K)
     */
    vector<vector<int>> sorted_arrays = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };

    struct Element {
        int value;
        int array_idx;
        int elem_idx;
        bool operator>(const Element& other) const {
            return value > other.value;
        }
    };

    priority_queue<Element, vector<Element>, greater<Element>> merge_heap;
    for (int i = 0; i < (int)sorted_arrays.size(); ++i) {
        if (!sorted_arrays[i].empty()) {
            merge_heap.push({sorted_arrays[i][0], i, 0});
        }
    }

    cout << "合并结果: ";
    vector<int> merged;
    while (!merge_heap.empty()) {
        Element top = merge_heap.top();
        merge_heap.pop();
        merged.push_back(top.value);

        int next_idx = top.elem_idx + 1;
        if (next_idx < (int)sorted_arrays[top.array_idx].size()) {
            merge_heap.push({
                sorted_arrays[top.array_idx][next_idx],
                top.array_idx,
                next_idx
            });
        }
    }
    for (int val : merged) cout << val << " ";
    cout << endl;

    // ==================== 9. 常见错误 ====================
    cout << "\n--- 8. 常见错误 ---" << endl;
    cout << "  错误1: 最小堆模板参数写法错误" << endl;
    cout << "    错误: priority_queue<int, greater<int>>" << endl;
    cout << "    正确: priority_queue<int, vector<int>, greater<int>>" << endl;
    cout << "  错误2: 对空堆调用 top() 或 pop() => 未定义行为" << endl;
    cout << "         必须先检查 empty()" << endl;
    cout << "  错误3: 期望 pop() 返回值" << endl;
    cout << "         pop() 只删除不返回, 先 top() 再 pop()" << endl;
    cout << "  错误4: 试图遍历 priority_queue" << endl;
    cout << "         没有迭代器, 只能复制后逐个 pop" << endl;
    cout << "  错误5: 比较器方向错误" << endl;
    cout << "         less<T>: 最大堆(默认), greater<T>: 最小堆" << endl;

    cout << "\n========================================" << endl;
    cout << "  priority_queue 详解完毕" << endl;
    cout << "========================================" << endl;

    return 0;
}
