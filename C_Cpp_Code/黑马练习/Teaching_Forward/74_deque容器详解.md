# deque 容器详解

## 一、deque 基本概念

`deque`（Double-Ended Queue，双端队列）是 STL 中的**序列式容器**。
它允许在**头部和尾部**都能高效地插入和删除元素。

**核心特点：**
- 双端操作：头部和尾部插入/删除都是 O(1)
- 随机访问：支持下标访问，但性能略低于 vector
- 动态扩展：不需要像 vector 那样搬移所有数据
- 内存分段：由多段连续内存组成（中控器 + 缓冲区）

```cpp
#include <iostream>
#include <deque>
using namespace std;

int main() {
    deque<int> d;

    // 两端都可以高效操作
    d.push_back(2);    // 尾部添加
    d.push_front(1);   // 头部添加
    d.push_back(3);    // 尾部添加

    // 输出：1 2 3
    for (int x : d) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
```

---

## 二、deque 与 vector 的区别（重要！）

| 特性 | vector | deque |
|------|--------|-------|
| 头部插入/删除 | 效率极低 O(n) | 高效 O(1) |
| 尾部插入 | 高效 O(1) | 高效 O(1) |
| 尾部删除 | 高效 O(1) | 高效 O(1) |
| 随机访问速度 | 非常快（连续内存） | 稍慢（分段内存） |
| `capacity()` | 有 | **无此函数** |
| `reserve()` | 有 | **无此函数** |
| `shrink_to_fit()` | 有 | **无此函数**（C++11有但效果不同） |
| 内存布局 | 连续一块 | 分段连续（中控器+缓冲区） |
| 迭代器失效 | 扩容时全部失效 | 头尾操作不影响中间迭代器 |
| 适用场景 | 尾部操作、随机访问 | 两端频繁操作 |

```cpp
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int main() {
    // vector 在头部插入效率很低
    vector<int> v = {1, 2, 3, 4, 5};
    v.insert(v.begin(), 0);  // O(n)：需要把所有元素后移

    // deque 在头部插入效率很高
    deque<int> d = {1, 2, 3, 4, 5};
    d.push_front(0);  // O(1)：直接在头部缓冲区添加

    // deque 没有 capacity 和 reserve
    // d.capacity();   // 编译错误！deque 没有这个成员函数
    // d.reserve(100); // 编译错误！

    return 0;
}
```

---

## 三、deque 的内部结构

deque 采用**中控器（map）+ 缓冲区（buffer）**的结构：

```
中控器（指针数组）
  |         |         |         |
  v         v         v         v
[缓冲区0] [缓冲区1] [缓冲区2] [缓冲区3]
[1][2][3] [4][5][6] [7][8][9] [10][11][12]
```

- **中控器**：一个指针数组，每个指针指向一个固定大小的缓冲区
- **缓冲区**：存放实际数据的连续内存块（通常每块8个元素）
- **优势**：扩容时只需扩展中控器，不需要搬移已有数据

```
为什么 deque 没有 capacity 和 reserve？
因为 deque 的内存是分段的，不存在"连续空间满了需要整体搬迁"的情况。
中控器满了可以扩容（搬移的是指针，不是数据），缓冲区各自独立。
```

**与 vector 内存对比：**

```
vector：连续内存
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │10 │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘

deque：分段连续内存
┌───┬───┬───┐  ┌───┬───┬───┬───┐  ┌───┬───┬───┐
│ 1 │ 2 │ 3 │  │ 4 │ 5 │ 6 │ 7 │  │ 8 │ 9 │10 │
└───┴───┴───┘  └───┴───┴───┴───┘  └───┴───┴───┘
```

---

## 四、deque 的构造方式

```cpp
#include <iostream>
#include <deque>
using namespace std;

void printDeque(const deque<int>& d, const string& name) {
    cout << name << " (size=" << d.size() << "): ";
    for (int x : d) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    // 方式1：默认构造（空 deque）
    deque<int> d1;

    // 方式2：指定大小（默认值为0）
    deque<int> d2(5);          // 5 个 0

    // 方式3：指定大小和初始值
    deque<int> d3(5, 100);     // 5 个 100

    // 方式4：拷贝构造
    deque<int> d4(d3);         // 深拷贝

    // 方式5：迭代器范围构造
    deque<int> d5(d3.begin(), d3.end());

    // 方式6：初始化列表（C++11）
    deque<int> d6 = {10, 20, 30, 40, 50};

    printDeque(d2, "d2");   // d2 (size=5): 0 0 0 0 0
    printDeque(d3, "d3");   // d3 (size=5): 100 100 100 100 100
    printDeque(d4, "d4");   // d4 (size=5): 100 100 100 100 100
    printDeque(d5, "d5");   // d5 (size=5): 100 100 100 100 100
    printDeque(d6, "d6");   // d6 (size=5): 10 20 30 40 50

    return 0;
}
```

---

## 五、deque 的赋值操作

```cpp
#include <iostream>
#include <deque>
using namespace std;

int main() {
    deque<int> d1 = {1, 2, 3, 4, 5};

    // = 运算符赋值
    deque<int> d2;
    d2 = d1;  // d2: 1 2 3 4 5

    // assign 赋值（迭代器范围）
    deque<int> d3;
    d3.assign(d1.begin(), d1.begin() + 3);  // d3: 1 2 3

    // assign 赋值（指定数量和值）
    deque<int> d4;
    d4.assign(4, 88);  // d4: 88 88 88 88

    // assign 赋值（初始化列表，C++11）
    deque<int> d5;
    d5.assign({7, 8, 9});  // d5: 7 8 9

    cout << "d2: ";
    for (int x : d2) cout << x << " ";  // 1 2 3 4 5
    cout << endl;

    cout << "d3: ";
    for (int x : d3) cout << x << " ";  // 1 2 3
    cout << endl;

    cout << "d4: ";
    for (int x : d4) cout << x << " ";  // 88 88 88 88
    cout << endl;

    return 0;
}
```

---

## 六、deque 的大小操作

```cpp
#include <iostream>
#include <deque>
using namespace std;

int main() {
    deque<int> d = {10, 20, 30};

    cout << "size: " << d.size() << endl;     // 3
    cout << "empty: " << d.empty() << endl;   // 0 (false)

    // resize：调整大小
    d.resize(5, 99);  // 扩大到5个，新增的用99填充
    // d: 10 20 30 99 99

    d.resize(2);       // 缩小到2个，丢弃后面的
    // d: 10 20

    // 注意：deque 没有 capacity() 和 reserve()
    // cout << d.capacity();  // 编译错误

    cout << "最终: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    return 0;
}
```

---

## 七、deque 的插入与删除操作

### 7.1 常用函数

| 函数 | 说明 |
|------|------|
| `push_back(val)` | 尾部添加元素 |
| `push_front(val)` | 头部添加元素 |
| `pop_back()` | 删除尾部元素 |
| `pop_front()` | 删除头部元素 |
| `insert(pos, val)` | 在迭代器 pos 位置插入 |
| `insert(pos, n, val)` | 插入 n 个 val |
| `erase(pos)` | 删除指定位置 |
| `erase(first, last)` | 删除范围 [first, last) |
| `clear()` | 清空所有元素 |

### 7.2 完整示例

```cpp
#include <iostream>
#include <deque>
using namespace std;

void printDeque(const deque<int>& d) {
    cout << "[size=" << d.size() << "] ";
    for (int x : d) cout << x << " ";
    cout << endl;
}

int main() {
    deque<int> d = {3, 4, 5};

    // push_back 和 push_front（deque 独有的优势）
    d.push_back(6);    // 尾部添加
    d.push_front(2);   // 头部添加
    d.push_back(7);
    d.push_front(1);
    printDeque(d);     // [size=7] 1 2 3 4 5 6 7

    // pop_back 和 pop_front
    d.pop_back();      // 删除尾部 7
    d.pop_front();     // 删除头部 1
    printDeque(d);     // [size=5] 2 3 4 5 6

    // insert：在中间插入
    d.insert(d.begin() + 2, 99);  // 在下标2处插入99
    printDeque(d);     // [size=6] 2 3 99 4 5 6

    // insert：插入多个
    d.insert(d.begin(), 3, 88);   // 在开头插入3个88
    printDeque(d);     // [size=9] 88 88 88 2 3 99 4 5 6

    // erase：删除单个
    d.erase(d.begin() + 3);  // 删除下标3的元素(2)
    printDeque(d);     // [size=8] 88 88 88 3 99 4 5 6

    // erase：删除范围
    d.erase(d.begin(), d.begin() + 3);  // 删除前3个
    printDeque(d);     // [size=5] 3 99 4 5 6

    // clear
    d.clear();
    cout << "clear后: size=" << d.size()
         << ", empty=" << d.empty() << endl;

    return 0;
}
```

### 7.3 迭代器失效问题

```cpp
#include <iostream>
#include <deque>
using namespace std;

int main() {
    deque<int> d = {1, 2, 3, 4, 5};

    // deque 的迭代器失效规则：
    // 1. 在头部/尾部 插入/删除：仅使指向被删元素的迭代器失效
    // 2. 在中间 插入/删除：所有迭代器失效
    // 3. resize：所有迭代器失效

    // 安全删除：用返回值更新迭代器
    for (auto it = d.begin(); it != d.end(); /* 不写++ */) {
        if (*it % 2 == 0) {  // 删除偶数
            it = d.erase(it);  // 返回下一个有效位置
        } else {
            it++;
        }
    }
    // d: 1 3 5

    cout << "删除偶数后: ";
    for (int x : d) cout << x << " ";
    cout << endl;

    return 0;
}
```

---

## 八、deque 的元素存取

```cpp
#include <iostream>
#include <deque>
using namespace std;

int main() {
    deque<int> d = {10, 20, 30, 40, 50};

    // [] 运算符：不检查边界
    cout << "d[0] = " << d[0] << endl;   // 10
    cout << "d[2] = " << d[2] << endl;   // 30

    // at()：检查边界，越界抛异常
    cout << "d.at(3) = " << d.at(3) << endl;  // 40
    try {
        d.at(10);  // 越界！
    } catch (const out_of_range& e) {
        cout << "异常: " << e.what() << endl;
    }

    // front() 和 back()
    cout << "front: " << d.front() << endl;  // 10
    cout << "back: " << d.back() << endl;    // 50

    // 修改元素
    d[0] = 100;
    d.at(1) = 200;
    d.front() = 1000;
    d.back() = 5000;

    cout << "修改后: ";
    for (int x : d) cout << x << " ";  // 1000 200 30 40 5000
    cout << endl;

    return 0;
}
```

---

## 九、deque 的排序

deque 支持使用 `sort` 算法进行排序，但因为内存不连续，排序性能比 vector 略低。

```cpp
#include <iostream>
#include <deque>
#include <algorithm>
using namespace std;

int main() {
    deque<int> d = {50, 20, 40, 10, 30};

    // 默认升序排序
    sort(d.begin(), d.end());
    cout << "升序: ";
    for (int x : d) cout << x << " ";  // 10 20 30 40 50
    cout << endl;

    // 降序排序
    sort(d.begin(), d.end(), greater<int>());
    cout << "降序: ";
    for (int x : d) cout << x << " ";  // 50 40 30 20 10
    cout << endl;

    // 自定义排序：按绝对值排序
    deque<int> d2 = {-3, 5, -1, 4, -2};
    sort(d2.begin(), d2.end(), [](int a, int b) {
        return abs(a) < abs(b);
    });
    cout << "按绝对值: ";
    for (int x : d2) cout << x << " ";  // -1 -2 -3 4 5
    cout << endl;

    return 0;
}
```

---

## 十、deque 遍历方式

```cpp
#include <iostream>
#include <deque>
#include <algorithm>
using namespace std;

int main() {
    deque<int> d = {10, 20, 30, 40, 50};

    // 方式1：下标遍历
    cout << "下标: ";
    for (size_t i = 0; i < d.size(); i++) {
        cout << d[i] << " ";
    }
    cout << endl;

    // 方式2：迭代器遍历
    cout << "迭代器: ";
    for (auto it = d.begin(); it != d.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    // 方式3：范围 for（推荐）
    cout << "范围for: ";
    for (const auto& x : d) {
        cout << x << " ";
    }
    cout << endl;

    // 方式4：反向遍历
    cout << "反向: ";
    for (auto it = d.rbegin(); it != d.rend(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    // 方式5：for_each
    cout << "for_each: ";
    for_each(d.begin(), d.end(), [](int x) {
        cout << x << " ";
    });
    cout << endl;

    return 0;
}
```

---

## 十一、综合实战案例

```cpp
#include <iostream>
#include <deque>
#include <string>
#include <algorithm>
using namespace std;

// 案例：用 deque 实现简单的任务队列（支持首尾操作）
int main() {
    deque<string> taskQueue;

    // 添加任务
    taskQueue.push_back("任务A: 写代码");      // 普通任务加到尾部
    taskQueue.push_back("任务B: 测试");
    taskQueue.push_back("任务C: 提交代码");

    // 紧急任务加到头部
    taskQueue.push_front("紧急任务: 修复Bug");

    cout << "===== 当前任务队列 =====" << endl;
    for (const auto& task : taskQueue) {
        cout << "  - " << task << endl;
    }

    // 按顺序处理任务（从头部开始）
    cout << "\n===== 开始处理任务 =====" << endl;
    while (!taskQueue.empty()) {
        cout << "正在处理: " << taskQueue.front() << endl;
        taskQueue.pop_front();
    }
    cout << "所有任务已完成！" << endl;

    // ====== 另一个案例：滑动窗口最大值的辅助结构 ======
    cout << "\n===== deque 用作滑动窗口 =====" << endl;
    deque<int> window;
    int data[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;  // 窗口大小

    for (int i = 0; i < 8; i++) {
        window.push_back(data[i]);
        if (window.size() > (size_t)k) {
            window.pop_front();  // 移除超出窗口的元素
        }
        if (window.size() == (size_t)k) {
            cout << "窗口 [";
            for (size_t j = 0; j < window.size(); j++) {
                if (j > 0) cout << ",";
                cout << window[j];
            }
            cout << "] 最大值: "
                 << *max_element(window.begin(), window.end())
                 << endl;
        }
    }

    return 0;
}
```

---

## 十二、deque 的头部操作优势演示

```cpp
#include <iostream>
#include <deque>
#include <vector>
#include <ctime>
using namespace std;

int main() {
    // 对比 deque 和 vector 在头部插入的效率
    const int N = 100000;

    // deque 头部插入
    clock_t start = clock();
    deque<int> d;
    for (int i = 0; i < N; i++) {
        d.push_front(i);
    }
    clock_t end = clock();
    cout << "deque push_front " << N << " 次: "
         << (double)(end - start) / CLOCKS_PER_SEC << " 秒" << endl;

    // vector 头部插入（效率极低，大量数据移动）
    start = clock();
    vector<int> v;
    for (int i = 0; i < N; i++) {
        v.insert(v.begin(), i);
    }
    end = clock();
    cout << "vector insert(begin) " << N << " 次: "
         << (double)(end - start) / CLOCKS_PER_SEC << " 秒" << endl;

    // deque 的优势在头部操作中非常明显

    return 0;
}
```

---

## 十三、总结

| 操作 | 函数 | 时间复杂度 |
|------|------|-----------|
| 头部添加 | `push_front()` | O(1) |
| 尾部添加 | `push_back()` | O(1) |
| 头部删除 | `pop_front()` | O(1) |
| 尾部删除 | `pop_back()` | O(1) |
| 随机访问 | `[]` / `at()` | O(1)（略慢于vector） |
| 中间插入 | `insert()` | O(n) |
| 中间删除 | `erase()` | O(n) |
| 排序 | `sort()` | O(n log n) |

**使用建议：**
- 需要在两端频繁插入/删除时，优先选 deque
- 需要频繁随机访问且性能敏感时，选 vector
- deque 是 `stack` 和 `queue` 的默认底层容器
