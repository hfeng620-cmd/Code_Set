# 优先队列 priority_queue 详解

## 一、什么是优先队列？

普通队列是先进先出（FIFO），而**优先队列**是按**优先级**出队，优先级最高的元素先出。

在 C++ STL 中，`priority_queue` 默认是**大顶堆**（最大值优先出队）。

### 核心概念

| 特点 | 说明 |
|------|------|
| 底层结构 | 默认用 `vector` + 堆算法 |
| 默认行为 | **大顶堆**（最大值先出队） |
| 插入 | O(log n) |
| 取最大值 | O(1) |
| 删除最大值 | O(log n) |
| 不支持遍历 | 没有迭代器，不能遍历 |

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    // 创建默认的优先队列（大顶堆）
    priority_queue<int> pq;

    // 入队
    pq.push(30);
    pq.push(10);
    pq.push(50);
    pq.push(20);
    pq.push(40);

    // 出队：按从大到小的顺序
    cout << "出队顺序（大顶堆）: ";
    while (!pq.empty()) {
        cout << pq.top() << " "; // 查看队顶元素
        pq.pop();                 // 弹出队顶元素
    }
    cout << endl;
    // 输出：50 40 30 20 10

    return 0;
}
```

## 二、构造函数

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

int main() {
    // ========== 构造方式1：默认大顶堆 ==========
    priority_queue<int> pq1;
    pq1.push(3);
    pq1.push(1);
    pq1.push(4);
    pq1.push(1);
    pq1.push(5);

    cout << "大顶堆出队: ";
    while (!pq1.empty()) {
        cout << pq1.top() << " "; // 5 4 3 1 1
        pq1.pop();
    }
    cout << endl;

    // ========== 构造方式2：小顶堆 ==========
    // 使用 greater<int> 作为比较器
    priority_queue<int, vector<int>, greater<int>> pq2;
    pq2.push(3);
    pq2.push(1);
    pq2.push(4);
    pq2.push(1);
    pq2.push(5);

    cout << "小顶堆出队: ";
    while (!pq2.empty()) {
        cout << pq2.top() << " "; // 1 1 3 4 5
        pq2.pop();
    }
    cout << endl;

    // ========== 构造方式3：用已有容器构造 ==========
    vector<int> vec = {10, 30, 20, 50, 40};
    priority_queue<int> pq3(vec.begin(), vec.end());
    cout << "从vector构造: ";
    while (!pq3.empty()) {
        cout << pq3.top() << " "; // 50 40 30 20 10
        pq3.pop();
    }
    cout << endl;

    // ========== 构造方式4：拷贝构造 ==========
    priority_queue<int> pq4(pq1); // 假设pq1还有数据

    return 0;
}
```

### 模板参数详解

```cpp
// priority_queue 的完整模板声明：
// template<
//     class T,                              // 元素类型
//     class Container = vector<T>,          // 底层容器，默认vector
//     class Compare = less<typename Container::value_type>  // 比较器
// > class priority_queue;

// 大顶堆（默认）：Compare = less<T>
//   less<T> 表示 a < b 时 a 的优先级更低
//   所以大的优先级高，先出队

// 小顶堆：Compare = greater<T>
//   greater<T> 表示 a > b 时 a 的优先级更低
//   所以小的优先级高，先出队

#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

int main() {
    // 大顶堆的3种等价写法
    priority_queue<int> pq1;
    priority_queue<int, vector<int>, less<int>> pq2;

    // 小顶堆的2种等价写法
    priority_queue<int, vector<int>, greater<int>> pq3;

    cout << "模板参数说明见注释" << endl;
    return 0;
}
```

## 三、基本操作

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    priority_queue<int> pq;

    // ========== push：入队 ==========
    pq.push(10);
    pq.push(30);
    pq.push(20);
    pq.push(50);
    pq.push(40);

    // ========== top：查看队顶元素（最大值） ==========
    cout << "队顶元素: " << pq.top() << endl; // 50

    // ========== size：队列大小 ==========
    cout << "大小: " << pq.size() << endl; // 5

    // ========== empty：判断是否为空 ==========
    cout << "是否为空: " << pq.empty() << endl; // 0(false)

    // ========== pop：弹出队顶元素 ==========
    pq.pop(); // 弹出50
    cout << "弹出后队顶: " << pq.top() << endl; // 40

    // ========== 遍历所有元素 ==========
    // 注意：priority_queue 没有迭代器，只能通过top+pop遍历
    // 遍历会破坏队列！如果需要保留，先拷贝一份
    cout << "所有元素（从大到小）: ";
    priority_queue<int> backup = pq; // 拷贝一份
    while (!backup.empty()) {
        cout << backup.top() << " "; // 40 30 20 10
        backup.pop();
    }
    cout << endl;

    // ========== emplace（C++11） ==========
    // 原地构造，避免拷贝
    pq.emplace(100);
    cout << "emplace后队顶: " << pq.top() << endl; // 100

    return 0;
}
```

## 四、大顶堆 vs 小顶堆

这是面试常考内容，必须搞清楚。

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

int main() {
    vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};

    // ========== 大顶堆：最大的先出 ==========
    // less<int>：a < b 时 a 优先级低（大的优先级高）
    priority_queue<int, vector<int>, less<int>> maxHeap;
    for (int val : data) maxHeap.push(val);

    cout << "大顶堆出队: ";
    while (!maxHeap.empty()) {
        cout << maxHeap.top() << " "; // 9 6 5 4 3 2 1 1
        maxHeap.pop();
    }
    cout << endl;

    // ========== 小顶堆：最小的先出 ==========
    // greater<int>：a > b 时 a 优先级低（小的优先级高）
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int val : data) minHeap.push(val);

    cout << "小顶堆出队: ";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " "; // 1 1 2 3 4 5 6 9
        minHeap.pop();
    }
    cout << endl;

    // ========== 记忆技巧 ==========
    // less<int>    -> "更小的更不重要" -> 大顶堆（大的先出）
    // greater<int> -> "更大的更不重要" -> 小顶堆（小的先出）

    return 0;
}
```

## 五、自定义排序（仿函数）

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

// ========== 场景1：自定义结构体的优先级 ==========
struct Task {
    string name;
    int priority; // 优先级数值越大越优先
    int time;     // 执行时间

    // 方式1：在结构体内重载 < 运算符
    // priority_queue 默认用 less，所以 < 的含义是"优先级更低"
    bool operator<(const Task& other) const {
        return priority < other.priority; // 优先级低的排后面
    }
};

// 方式2：外部仿函数（更灵活）
struct TaskCompare {
    bool operator()(const Task& a, const Task& b) const {
        // 优先级高的先出队
        if (a.priority != b.priority) {
            return a.priority < b.priority;
        }
        // 优先级相同，执行时间短的先出队
        return a.time > b.time;
    }
};

int main() {
    // 使用方式1：重载 < 运算符
    priority_queue<Task> pq1;
    pq1.push({"低优先级任务", 1, 100});
    pq1.push({"高优先级任务", 5, 50});
    pq1.push({"中优先级任务", 3, 30});

    cout << "按优先级出队:" << endl;
    while (!pq1.empty()) {
        cout << "  " << pq1.top().name
             << " (优先级:" << pq1.top().priority << ")" << endl;
        pq1.pop();
    }
    // 高优先级(5) -> 中优先级(3) -> 低优先级(1)

    // 使用方式2：外部仿函数
    priority_queue<Task, vector<Task>, TaskCompare> pq2;
    pq2.push({"任务A", 3, 100});
    pq2.push({"任务B", 3, 50});  // 同优先级，时间更短
    pq2.push({"任务C", 5, 200});

    cout << "\n按优先级+时间出队:" << endl;
    while (!pq2.empty()) {
        cout << "  " << pq2.top().name
             << " (优先级:" << pq2.top().priority
             << ", 时间:" << pq2.top().time << ")" << endl;
        pq2.pop();
    }
    // 任务C(5) -> 任务B(3,50) -> 任务A(3,100)

    return 0;
}
```

### 字符串按长度排序示例

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct CompareByLength {
    bool operator()(const string& a, const string& b) const {
        // 字符串短的优先级高（小顶堆效果）
        return a.length() > b.length();
    }
};

int main() {
    priority_queue<string, vector<string>, CompareByLength> pq;

    pq.push("hello");
    pq.push("hi");
    pq.push("goodbye");
    pq.push("hey");
    pq.push("cpp");

    cout << "按长度从小到大:" << endl;
    while (!pq.empty()) {
        cout << "  " << pq.top() << " (长度:" << pq.top().length() << ")" << endl;
        pq.pop();
    }
    // hi(2) -> hey/cpp(3) -> hello(5) -> goodbye(7)

    return 0;
}
```

## 六、实际应用

### 应用1：任务调度系统

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct Job {
    string name;
    int urgency; // 紧急程度，数值越大越紧急

    bool operator<(const Job& other) const {
        return urgency < other.urgency; // 紧急的先执行
    }
};

int main() {
    priority_queue<Job> jobQueue;

    // 提交任务
    jobQueue.push({"备份数据库", 2});
    jobQueue.push({"修复线上Bug", 10});
    jobQueue.push({"代码review", 5});
    jobQueue.push({"写文档", 1});
    jobQueue.push({"部署上线", 8});

    cout << "====== 任务调度 ======" << endl;
    int order = 1;
    while (!jobQueue.empty()) {
        auto job = jobQueue.top();
        cout << "第" << order++ << "个执行: " << job.name
             << " (紧急度:" << job.urgency << ")" << endl;
        jobQueue.pop();
    }

    return 0;
}
```

### 应用2：取第K大元素

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 方法：用小顶堆维护前K大元素
// 堆顶就是第K大的元素
int findKthLargest(vector<int>& nums, int k) {
    // 小顶堆
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        if (minHeap.size() > k) {
            minHeap.pop(); // 堆大小超过k就弹出最小的
        }
    }

    return minHeap.top(); // 堆顶就是第K大的
}

int main() {
    vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 3;

    cout << "数组: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    cout << "第" << k << "大的元素: " << findKthLargest(nums, k) << endl;
    // 排序后: 6 5 4 3 2 1，第3大是4

    // 另一个例子
    vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    cout << "第4大的元素: " << findKthLargest(nums2, 4) << endl;
    // 排序后: 6 5 5 4 3 3 2 2 1，第4大是4

    return 0;
}
```

### 应用3：合并K个有序链表（简化版）

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// 节点结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 用于优先队列的比较器
struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val; // 小顶堆，值小的优先
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    // 小顶堆
    priority_queue<ListNode*, vector<ListNode*>, Compare> pq;

    // 把每个链表的头节点放入堆
    for (ListNode* head : lists) {
        if (head) pq.push(head);
    }

    ListNode dummy(0); // 哨兵节点
    ListNode* tail = &dummy;

    while (!pq.empty()) {
        // 取出最小的节点
        ListNode* minNode = pq.top();
        pq.pop();

        // 接到结果链表
        tail->next = minNode;
        tail = tail->next;

        // 如果该链表还有后续节点，放入堆
        if (minNode->next) {
            pq.push(minNode->next);
        }
    }

    return dummy.next;
}

// 辅助函数：创建链表
ListNode* createList(initializer_list<int> vals) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int v : vals) {
        tail->next = new ListNode(v);
        tail = tail->next;
    }
    return dummy.next;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    // 创建3个有序链表
    vector<ListNode*> lists;
    lists.push_back(createList({1, 4, 7}));
    lists.push_back(createList({2, 5, 8}));
    lists.push_back(createList({3, 6, 9}));

    cout << "链表1: ";
    printList(lists[0]);
    cout << "链表2: ";
    printList(lists[1]);
    cout << "链表3: ";
    printList(lists[2]);

    ListNode* merged = mergeKLists(lists);
    cout << "合并后: ";
    printList(merged);
    // 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9

    return 0;
}
```

### 应用4：数据流的中位数

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class MedianFinder {
private:
    // 大顶堆：存较小的一半
    priority_queue<int> maxHeap;
    // 小顶堆：存较大的一半
    priority_queue<int, vector<int>, greater<int>> minHeap;

public:
    // 添加数字
    void addNum(int num) {
        maxHeap.push(num);

        // 确保maxHeap的堆顶 <= minHeap的堆顶
        minHeap.push(maxHeap.top());
        maxHeap.pop();

        // 确保两个堆大小平衡（maxHeap可以多1个）
        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    // 查找中位数
    double findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top(); // 奇数个元素
        }
        return (maxHeap.top() + minHeap.top()) / 2.0; // 偶数个元素
    }
};

int main() {
    MedianFinder mf;

    vector<int> nums = {5, 15, 1, 3, 8, 7, 9};
    for (int num : nums) {
        mf.addNum(num);
        cout << "添加" << num << "后, 中位数: " << mf.findMedian() << endl;
    }

    return 0;
}
```

## 七、priority_queue 的注意事项

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    // ========== 注意1：不能遍历 ==========
    // priority_queue 没有 begin()/end()，不能用for循环遍历
    priority_queue<int> pq;
    pq.push(1); pq.push(2); pq.push(3);

    // 错误写法：
    // for (auto it = pq.begin(); it != pq.end(); ++it) {} // 编译错误

    // 只能通过 top() + pop() 来"遍历"（会破坏队列）
    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << endl;

    // ========== 注意2：top()返回的是const引用 ==========
    priority_queue<int> pq2;
    pq2.push(10);
    // pq2.top() = 100; // 错误！不能直接修改top()

    // ========== 注意3：底层容器可以查看（但不推荐） ==========
    // priority_queue 有一个 protected 成员 c，可以通过继承访问
    // 但通常不建议这样做

    // ========== 注意4：自定义类型必须提供比较方式 ==========
    struct Point {
        int x, y;
        bool operator<(const Point& other) const {
            return x * x + y * y < other.x * other.x + other.y * other.y;
        }
    };

    priority_queue<Point> points;
    points.push({1, 2});
    points.push({3, 4});
    points.push({0, 1});

    cout << "离原点最远的点: (" << points.top().x
         << "," << points.top().y << ")" << endl; // (3,4)

    return 0;
}
```

## 八、总结

| 要点 | 内容 |
|------|------|
| 本质 | 堆（默认大顶堆） |
| 大顶堆 | `priority_queue<int>` 或 `priority_queue<int, vector<int>, less<int>>` |
| 小顶堆 | `priority_queue<int, vector<int>, greater<int>>` |
| 基本操作 | `push` 入队、`top` 查看队顶、`pop` 弹出、`empty` 判空、`size` 大小 |
| 时间复杂度 | 插入 O(log n)、取顶 O(1)、删除 O(log n) |
| 不能遍历 | 没有迭代器，只能 top+pop |
| 自定义排序 | 仿函数作为第三个模板参数 |
| 应用场景 | 任务调度、Top-K问题、合并有序序列、中位数查找 |
