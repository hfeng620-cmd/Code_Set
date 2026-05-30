/**
 * 08_堆与优先队列 - LeetCode 经典例题
 *
 * 本文件包含 5 道经典的堆与优先队列相关算法题：
 * 1. 数据流中的第K大元素 (LeetCode 703, Easy)
 * 2. 前K个高频元素 (LeetCode 347, Medium)
 * 3. 数据流的中位数 (LeetCode 295, Hard)
 * 4. 数组中的第K个最大元素 (LeetCode 215, Medium)
 * 5. 合并K个升序链表 (LeetCode 23, Hard)
 *
 * 核心知识：
 * - 堆是一种完全二叉树，分为最大堆和最小堆。
 * - C++ 中 priority_queue 默认是最大堆（大顶堆）。
 * - 堆的基本操作（插入、删除）时间复杂度为 O(log n)。
 * - 建堆的时间复杂度为 O(n)。
 */

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <climits>
using namespace std;

// ============================================================================
// 题目 1: 数据流中的第K大元素 (LeetCode 703, Easy)
// ============================================================================
/**
 * 题目描述：
 * 设计一个找到数据流中第 k 大元素的类。注意是排序后的第 k 大元素，
 * 不是第 k 个不同的元素。
 *
 * 请实现 KthLargest 类：
 * - KthLargest(int k, int[] nums) 使用整数 k 和整数流 nums 初始化对象。
 * - int add(int val) 将 val 插入数据流 nums 后，返回当前数据流中第 k 大的元素。
 *
 * 示例：
 * 输入: ["KthLargest", "add", "add", "add", "add", "add"]
 *        [[3, [4, 5, 8, 2]], [3], [5], [10], [9], [4]]
 * 输出: [null, 4, 5, 5, 8, 8]
 *
 * 核心思想：
 * 为什么用最小堆（小顶堆）找第 K 大？
 * - 维护一个大小为 K 的最小堆，堆顶就是第 K 大的元素。
 * - 堆中只保留最大的 K 个元素，堆顶是这 K 个中最小的。
 * - 当新元素比堆顶大时，弹出堆顶，插入新元素。
 */

// 方法：最小堆（小顶堆），维护大小为 K
// 时间复杂度: 初始化 O(n log k)，add 操作 O(log k)
// 空间复杂度: O(k)
class KthLargest {
private:
    int k;
    priority_queue<int, vector<int>, greater<int>> minHeap;  // 最小堆

public:
    KthLargest(int k, vector<int>& nums) : k(k) {
        // 将所有元素加入堆，维持堆大小不超过 k
        for (int num : nums) {
            add(num);
        }
    }

    int add(int val) {
        minHeap.push(val);

        // 如果堆大小超过 k，弹出堆顶（最小的元素）
        // 这样堆中保留的就是最大的 k 个元素
        if (minHeap.size() > k) {
            minHeap.pop();
        }

        // 堆顶就是第 k 大的元素
        return minHeap.top();
    }
};

void test_problem1() {
    cout << "========================================" << endl;
    cout << "题目 1: 数据流中的第K大元素 (LeetCode 703)" << endl;
    cout << "========================================" << endl;

    // 测试用例
    cout << "初始化: k=3, nums=[4,5,8,2]" << endl;
    vector<int> nums = {4, 5, 8, 2};
    KthLargest kthLargest(3, nums);

    cout << "add(3)  -> 第3大 = " << kthLargest.add(3) << " (期望: 4)" << endl;
    cout << "add(5)  -> 第3大 = " << kthLargest.add(5) << " (期望: 5)" << endl;
    cout << "add(10) -> 第3大 = " << kthLargest.add(10) << " (期望: 5)" << endl;
    cout << "add(9)  -> 第3大 = " << kthLargest.add(9) << " (期望: 8)" << endl;
    cout << "add(4)  -> 第3大 = " << kthLargest.add(4) << " (期望: 8)" << endl;

    // 测试用例 2
    cout << "\n初始化: k=1, nums=[]" << endl;
    vector<int> nums2 = {};
    KthLargest kthLargest2(1, nums2);

    cout << "add(-3) -> 第1大 = " << kthLargest2.add(-3) << " (期望: -3)" << endl;
    cout << "add(-2) -> 第1大 = " << kthLargest2.add(-2) << " (期望: -2)" << endl;
    cout << "add(-4) -> 第1大 = " << kthLargest2.add(-4) << " (期望: -2)" << endl;
    cout << "add(0)  -> 第1大 = " << kthLargest2.add(0) << " (期望: 0)" << endl;
    cout << "add(4)  -> 第1大 = " << kthLargest2.add(4) << " (期望: 4)" << endl;
    cout << endl;
}

// ============================================================================
// 题目 2: 前K个高频元素 (LeetCode 347, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给你一个整数数组 nums 和一个整数 k，请你返回其中出现频率前 k 高的元素。
 * 你可以按任意顺序返回答案。
 *
 * 示例：
 * 输入: nums = [1,1,1,2,2,3], k = 2
 * 输出: [1,2]
 *
 * 输入: nums = [1], k = 1
 * 输出: [1]
 */

// 方法一：哈希表 + 最小堆
// 思路：先统计频率，然后用最小堆维护频率最高的 k 个元素
// 时间复杂度: O(n log k)  空间复杂度: O(n)
vector<int> topKFrequent_heap(vector<int>& nums, int k) {
    // 第一步：统计每个元素的出现频率
    unordered_map<int, int> freqMap;
    for (int num : nums) {
        freqMap[num]++;
    }

    // 第二步：使用最小堆，按频率排序
    // pair<频率, 元素值>，优先队列按 first 排序
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;

    for (auto& [num, freq] : freqMap) {
        minHeap.push({freq, num});
        if (minHeap.size() > k) {
            minHeap.pop();  // 弹出频率最小的
        }
    }

    // 第三步：从堆中取出结果
    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }

    return result;
}

// 方法二：哈希表 + 桶排序（最优解）
// 思路：频率作为桶的下标，频率相同的元素放在同一个桶中
// 时间复杂度: O(n)  空间复杂度: O(n)
vector<int> topKFrequent_bucket(vector<int>& nums, int k) {
    // 第一步：统计频率
    unordered_map<int, int> freqMap;
    for (int num : nums) {
        freqMap[num]++;
    }

    // 第二步：创建桶，bucket[i] 存储频率为 i 的所有元素
    // 最大频率不会超过数组长度
    int n = nums.size();
    vector<vector<int>> buckets(n + 1);

    for (auto& [num, freq] : freqMap) {
        buckets[freq].push_back(num);
    }

    // 第三步：从高频到低频收集结果
    vector<int> result;
    for (int i = n; i >= 0 && result.size() < k; i--) {
        for (int num : buckets[i]) {
            result.push_back(num);
            if (result.size() == k) break;
        }
    }

    return result;
}

void test_problem2() {
    cout << "========================================" << endl;
    cout << "题目 2: 前K个高频元素 (LeetCode 347)" << endl;
    cout << "========================================" << endl;

    // 测试用例 1
    vector<int> nums1 = {1, 1, 1, 2, 2, 3};
    int k1 = 2;
    cout << "输入: nums=[1,1,1,2,2,3], k=2" << endl;

    auto result1_heap = topKFrequent_heap(nums1, k1);
    cout << "最小堆: [";
    for (int i = 0; i < result1_heap.size(); i++) {
        cout << result1_heap[i] << (i < result1_heap.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    auto result1_bucket = topKFrequent_bucket(nums1, k1);
    cout << "桶排序: [";
    for (int i = 0; i < result1_bucket.size(); i++) {
        cout << result1_bucket[i] << (i < result1_bucket.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    // 测试用例 2
    vector<int> nums2 = {1};
    int k2 = 1;
    cout << "\n输入: nums=[1], k=1" << endl;

    auto result2_heap = topKFrequent_heap(nums2, k2);
    cout << "最小堆: [";
    for (int i = 0; i < result2_heap.size(); i++) {
        cout << result2_heap[i] << (i < result2_heap.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    auto result2_bucket = topKFrequent_bucket(nums2, k2);
    cout << "桶排序: [";
    for (int i = 0; i < result2_bucket.size(); i++) {
        cout << result2_bucket[i] << (i < result2_bucket.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    // 测试用例 3
    vector<int> nums3 = {4, 1, -1, 2, -1, 2, 3};
    int k3 = 2;
    cout << "\n输入: nums=[4,1,-1,2,-1,2,3], k=2" << endl;

    auto result3_heap = topKFrequent_heap(nums3, k3);
    cout << "最小堆: [";
    for (int i = 0; i < result3_heap.size(); i++) {
        cout << result3_heap[i] << (i < result3_heap.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    auto result3_bucket = topKFrequent_bucket(nums3, k3);
    cout << "桶排序: [";
    for (int i = 0; i < result3_bucket.size(); i++) {
        cout << result3_bucket[i] << (i < result3_bucket.size() - 1 ? "," : "");
    }
    cout << "]" << endl;
    cout << endl;
}

// ============================================================================
// 题目 3: 数据流的中位数 (LeetCode 295, Hard)
// ============================================================================
/**
 * 题目描述：
 * 中位数是有序整数列表中间的数。如果列表长度是偶数，中位数则是中间两个数的平均值。
 *
 * 实现 MedianFinder 类：
 * - MedianFinder() 初始化 MedianFinder 对象。
 * - void addNum(int num) 将数据流中的整数 num 添加到数据结构中。
 * - double findMedian() 返回目前所有元素的中位数。
 *
 * 示例：
 * 输入: ["MedianFinder", "addNum", "addNum", "findMedian", "addNum", "findMedian"]
 *        [[], [1], [2], [], [3], []]
 * 输出: [null, null, null, 1.5, null, 2.0]
 *
 * 核心思想：双堆法（大顶堆 + 小顶堆）
 * 这是一道非常经典的面试题！
 *
 * 设计思路：
 * 1. 用大顶堆存储较小的一半数据（堆顶是较小部分的最大值）
 * 2. 用小顶堆存储较大的一半数据（堆顶是较大部分的最小值）
 * 3. 维护两个堆的大小平衡：要么相等，要么大顶堆比小顶堆多一个
 * 4. 中位数 = 堆顶元素（奇数个）或两个堆顶的平均值（偶数个）
 */

class MedianFinder {
private:
    // 大顶堆：存储较小的一半，堆顶是最大值
    priority_queue<int> maxHeap;

    // 小顶堆：存储较大的一半，堆顶是最小值
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:
    MedianFinder() {}

    void addNum(int num) {
        // 策略：先加入大顶堆，再平衡
        // 这样可以保证大顶堆中的元素都 <= 小顶堆中的元素

        // 第一步：将新元素加入大顶堆
        maxHeap.push(num);

        // 第二步：将大顶堆的最大值移到小顶堆
        // 这确保小顶堆中的所有元素 >= 大顶堆中的所有元素
        minHeap.push(maxHeap.top());
        maxHeap.pop();

        // 第三步：保持平衡，大顶堆的大小可以 >= 小顶堆
        // 如果小顶堆比大顶堆大，将小顶堆的最小值移到大顶堆
        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        // 奇数个元素：中位数是大顶堆的堆顶
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        }
        // 偶数个元素：中位数是两个堆顶的平均值
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};

void test_problem3() {
    cout << "========================================" << endl;
    cout << "题目 3: 数据流的中位数 (LeetCode 295)" << endl;
    cout << "========================================" << endl;

    cout << "这是非常经典的面试题！" << endl << endl;

    // 测试用例 1
    cout << "测试用例 1:" << endl;
    MedianFinder mf1;
    mf1.addNum(1);
    mf1.addNum(2);
    cout << "add(1), add(2) -> findMedian = " << mf1.findMedian()
         << " (期望: 1.5)" << endl;
    mf1.addNum(3);
    cout << "add(3) -> findMedian = " << mf1.findMedian()
         << " (期望: 2.0)" << endl;

    cout << endl;

    // 测试用例 2：连续添加
    cout << "测试用例 2：连续添加 [5,15,1,3,8,7,9]" << endl;
    MedianFinder mf2;
    vector<int> nums = {5, 15, 1, 3, 8, 7, 9};
    for (int num : nums) {
        mf2.addNum(num);
        cout << "add(" << num << ") -> findMedian = " << mf2.findMedian() << endl;
    }

    cout << endl;

    // 测试用例 3：单个元素
    cout << "测试用例 3：单个元素" << endl;
    MedianFinder mf3;
    mf3.addNum(42);
    cout << "add(42) -> findMedian = " << mf3.findMedian()
         << " (期望: 42)" << endl;
    cout << endl;
}

// ============================================================================
// 题目 4: 数组中的第K个最大元素 (LeetCode 215, Medium)
// ============================================================================
/**
 * 题目描述：
 * 给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
 * 请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。
 * 你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。
 *
 * 示例：
 * 输入: [3,2,1,5,6,4], k = 2
 * 输出: 5
 *
 * 输入: [3,2,3,1,2,4,5,5,6], k = 4
 * 输出: 4
 */

// 方法一：排序
// 思路：直接排序后取第 k 大
// 时间复杂度: O(n log n)  空间复杂度: O(1) 或 O(n)
int findKthLargest_sort(vector<int>& nums, int k) {
    sort(nums.begin(), nums.end(), greater<int>());  // 降序排序
    return nums[k - 1];
}

// 方法二：最小堆
// 思路：维护大小为 k 的最小堆，堆顶就是第 k 大
// 时间复杂度: O(n log k)  空间复杂度: O(k)
int findKthLargest_heap(vector<int>& nums, int k) {
    // 最小堆（小顶堆）
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop();  // 保持堆大小为 k
        }
    }

    return minHeap.top();  // 堆顶就是第 k 大
}

// 方法三：快速选择算法（Quickselect）
// 思路：基于快速排序的 partition，平均 O(n)
// 时间复杂度: 平均 O(n)，最坏 O(n²)  空间复杂度: O(1)
int findKthLargest_quickselect(vector<int>& nums, int k) {
    int n = nums.size();
    int target = k - 1;  // 第 k 大在降序排序后的下标

    // partition 函数：将大于 pivot 的放左边，小于的放右边
    auto partition = [&](int left, int right) -> int {
        int pivot = nums[left];
        int i = left + 1, j = right;

        while (i <= j) {
            while (i <= right && nums[i] >= pivot) i++;
            while (j >= left && nums[j] < pivot) j--;
            if (i < j) {
                swap(nums[i], nums[j]);
            }
        }

        swap(nums[left], nums[j]);
        return j;
    };

    int left = 0, right = n - 1;

    while (left <= right) {
        int pivotIndex = partition(left, right);

        if (pivotIndex == target) {
            return nums[pivotIndex];  // 找到第 k 大
        } else if (pivotIndex < target) {
            left = pivotIndex + 1;    // 在右半部分继续找
        } else {
            right = pivotIndex - 1;   // 在左半部分继续找
        }
    }

    return -1;  // 不会执行到这里
}

void test_problem4() {
    cout << "========================================" << endl;
    cout << "题目 4: 数组中的第K个最大元素 (LeetCode 215)" << endl;
    cout << "========================================" << endl;

    // 测试用例 1
    vector<int> nums1 = {3, 2, 1, 5, 6, 4};
    int k1 = 2;
    cout << "输入: [3,2,1,5,6,4], k=2" << endl;
    cout << "排序法:     " << findKthLargest_sort(nums1, k1)
         << " (期望: 5)" << endl;
    cout << "最小堆:     " << findKthLargest_heap(nums1, k1)
         << " (期望: 5)" << endl;
    cout << "快速选择:   " << findKthLargest_quickselect(nums1, k1)
         << " (期望: 5)" << endl;

    cout << endl;

    // 测试用例 2
    vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int k2 = 4;
    cout << "输入: [3,2,3,1,2,4,5,5,6], k=4" << endl;
    cout << "排序法:     " << findKthLargest_sort(nums2, k2)
         << " (期望: 4)" << endl;
    cout << "最小堆:     " << findKthLargest_heap(nums2, k2)
         << " (期望: 4)" << endl;
    cout << "快速选择:   " << findKthLargest_quickselect(nums2, k2)
         << " (期望: 4)" << endl;

    cout << endl;

    // 测试用例 3
    vector<int> nums3 = {7, 6, 5, 4, 3, 2, 1};
    int k3 = 5;
    cout << "输入: [7,6,5,4,3,2,1], k=5" << endl;
    cout << "排序法:     " << findKthLargest_sort(nums3, k3)
         << " (期望: 3)" << endl;
    cout << "最小堆:     " << findKthLargest_heap(nums3, k3)
         << " (期望: 3)" << endl;
    cout << "快速选择:   " << findKthLargest_quickselect(nums3, k3)
         << " (期望: 3)" << endl;
    cout << endl;
}

// ============================================================================
// 题目 5: 合并K个升序链表 (LeetCode 23, Hard)
// ============================================================================
/**
 * 题目描述：
 * 给你一个链表数组，每个链表都已经按升序排列。
 * 请你将所有链表合并到一个升序链表中，返回合并后的链表。
 *
 * 示例：
 * 输入: lists = [[1,4,5],[1,3,4],[2,6]]
 * 输出: [1,1,2,3,4,4,5,6]
 *
 * 输入: lists = []
 * 输出: []
 *
 * 输入: lists = [[]]
 * 输出: []
 */

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 方法一：暴力法
// 思路：将所有节点值放入数组，排序后重建链表
// 时间复杂度: O(N log N)，N 是所有节点总数  空间复杂度: O(N)
ListNode* mergeKLists_brute(vector<ListNode*>& lists) {
    vector<int> values;

    // 收集所有节点的值
    for (ListNode* head : lists) {
        while (head) {
            values.push_back(head->val);
            head = head->next;
        }
    }

    // 排序
    sort(values.begin(), values.end());

    // 重建链表
    ListNode dummy(0);
    ListNode* curr = &dummy;
    for (int val : values) {
        curr->next = new ListNode(val);
        curr = curr->next;
    }

    return dummy.next;
}

// 方法二：最小堆（优先队列）（推荐）
// 思路：将每个链表的头节点放入最小堆，每次取出最小的节点，
//       然后将该节点的下一个节点加入堆
// 时间复杂度: O(N log k)，k 是链表数量  空间复杂度: O(k)
ListNode* mergeKLists_heap(vector<ListNode*>& lists) {
    // 自定义比较函数，最小堆
    auto cmp = [](ListNode* a, ListNode* b) {
        return a->val > b->val;  // 注意：> 是最小堆
    };

    priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> minHeap(cmp);

    // 将每个链表的头节点加入堆
    for (ListNode* head : lists) {
        if (head) {
            minHeap.push(head);
        }
    }

    ListNode dummy(0);
    ListNode* curr = &dummy;

    while (!minHeap.empty()) {
        // 取出堆顶（最小节点）
        ListNode* node = minHeap.top();
        minHeap.pop();

        // 加入结果链表
        curr->next = node;
        curr = curr->next;

        // 如果该节点有下一个节点，加入堆
        if (node->next) {
            minHeap.push(node->next);
        }
    }

    return dummy.next;
}

// 方法三：分治法
// 思路：类似归并排序，两两合并链表
// 时间复杂度: O(N log k)  空间复杂度: O(log k) 递归栈
// 前向声明
ListNode* divideAndConquer(vector<ListNode*>& lists, int left, int right);

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* curr = &dummy;

    while (l1 && l2) {
        if (l1->val <= l2->val) {
            curr->next = l1;
            l1 = l1->next;
        } else {
            curr->next = l2;
            l2 = l2->next;
        }
        curr = curr->next;
    }

    curr->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* mergeKLists_divide(vector<ListNode*>& lists) {
    if (lists.empty()) return nullptr;
    return divideAndConquer(lists, 0, lists.size() - 1);
}

ListNode* divideAndConquer(vector<ListNode*>& lists, int left, int right) {
    if (left == right) return lists[left];

    int mid = left + (right - left) / 2;
    ListNode* l1 = divideAndConquer(lists, left, mid);
    ListNode* l2 = divideAndConquer(lists, mid + 1, right);

    return mergeTwoLists(l1, l2);
}

// 辅助函数：从 vector 创建链表
ListNode* createList(const vector<int>& nums) {
    ListNode dummy(0);
    ListNode* curr = &dummy;
    for (int num : nums) {
        curr->next = new ListNode(num);
        curr = curr->next;
    }
    return dummy.next;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    cout << "[";
    while (head) {
        cout << head->val;
        if (head->next) cout << ",";
        head = head->next;
    }
    cout << "]";
}

// 辅助函数：释放链表内存
void deleteList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void test_problem5() {
    cout << "========================================" << endl;
    cout << "题目 5: 合并K个升序链表 (LeetCode 23)" << endl;
    cout << "========================================" << endl;

    // 测试用例 1
    cout << "测试用例 1: [[1,4,5],[1,3,4],[2,6]]" << endl;
    vector<ListNode*> lists1 = {
        createList({1, 4, 5}),
        createList({1, 3, 4}),
        createList({2, 6})
    };

    ListNode* result1_brute = mergeKLists_brute(lists1);
    cout << "暴力法:   ";
    printList(result1_brute);
    cout << endl;

    // 重新创建链表（暴力法修改了原链表）
    lists1 = {
        createList({1, 4, 5}),
        createList({1, 3, 4}),
        createList({2, 6})
    };
    ListNode* result1_heap = mergeKLists_heap(lists1);
    cout << "最小堆:   ";
    printList(result1_heap);
    cout << endl;

    lists1 = {
        createList({1, 4, 5}),
        createList({1, 3, 4}),
        createList({2, 6})
    };
    ListNode* result1_divide = mergeKLists_divide(lists1);
    cout << "分治法:   ";
    printList(result1_divide);
    cout << endl;

    // 释放内存
    deleteList(result1_brute);
    deleteList(result1_heap);
    deleteList(result1_divide);

    // 测试用例 2
    cout << "\n测试用例 2: []" << endl;
    vector<ListNode*> lists2 = {};
    ListNode* result2 = mergeKLists_heap(lists2);
    cout << "结果: ";
    printList(result2);
    cout << endl;

    // 测试用例 3
    cout << "\n测试用例 3: [[]]" << endl;
    vector<ListNode*> lists3 = {nullptr};
    ListNode* result3 = mergeKLists_heap(lists3);
    cout << "结果: ";
    printList(result3);
    cout << endl;

    // 测试用例 4
    cout << "\n测试用例 4: [[1],[0]]" << endl;
    vector<ListNode*> lists4 = {
        createList({1}),
        createList({0})
    };
    ListNode* result4 = mergeKLists_heap(lists4);
    cout << "结果: ";
    printList(result4);
    cout << endl;
    deleteList(result4);
    cout << endl;
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    cout << "============================================" << endl;
    cout << "    08_堆与优先队列 - LeetCode 经典例题" << endl;
    cout << "============================================" << endl << endl;

    test_problem1();  // 数据流中的第K大元素
    test_problem2();  // 前K个高频元素
    test_problem3();  // 数据流的中位数
    test_problem4();  // 数组中的第K个最大元素
    test_problem5();  // 合并K个升序链表

    cout << "============================================" << endl;
    cout << "    所有题目测试完成！" << endl;
    cout << "============================================" << endl;

    return 0;
}
