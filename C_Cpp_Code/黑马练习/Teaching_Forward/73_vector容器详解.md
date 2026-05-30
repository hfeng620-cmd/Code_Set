# vector 容器详解

## 一、vector 基本概念

`vector` 是 STL 中最常用的**序列式容器**，被称为**单端数组**（动态数组）。
它能够在运行时动态扩展容量，不需要手动管理内存。

**核心特点：**
- 连续存储：元素在内存中连续排列，支持随机访问（按下标访问）
- 动态扩展：容量不够时自动扩容，无需手动 realloc
- 尾部操作高效：在尾部插入/删除元素时间复杂度 O(1)
- 中间操作低效：在中间插入/删除需要移动元素，时间复杂度 O(n)

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 最简单的创建方式：创建一个空的 vector
    vector<int> v;

    // 向尾部添加元素
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // 输出：10 20 30
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}
```

---

## 二、vector 的五种构造方式

### 2.1 默认构造（无参构造）

```cpp
vector<int> v1;  // 创建一个空的 vector，size=0，capacity=0
```

### 2.2 构造并指定初始大小

```cpp
vector<int> v2(10);      // 创建包含 10 个元素的 vector，每个元素默认初始化为 0
vector<int> v3(10, 99);  // 创建包含 10 个元素的 vector，每个元素初始化为 99
```

### 2.3 拷贝构造

```cpp
vector<int> v4(v3);       // 用 v3 的所有元素构造 v4（深拷贝）
vector<int> v5 = v3;      // 等价写法
```

### 2.4 使用迭代器范围构造

```cpp
vector<int> v6(v3.begin(), v3.end());       // 用 v3 的全部元素构造 v6
vector<int> v7(v3.begin(), v3.begin() + 5); // 只用前 5 个元素构造
```

### 2.5 初始化列表构造（C++11）

```cpp
vector<int> v8 = {1, 2, 3, 4, 5};  // 直接用花括号初始化
vector<int> v9{10, 20, 30};         // 等价写法
```

### 完整示例

```cpp
#include <iostream>
#include <vector>
using namespace std;

void printVec(const vector<int>& v, const string& name) {
    cout << name << " (size=" << v.size() << "): ";
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    // 方式1：默认构造
    vector<int> v1;

    // 方式2：指定大小和初始值
    vector<int> v2(5);          // 5 个 0
    vector<int> v3(5, 100);     // 5 个 100

    // 方式3：拷贝构造
    vector<int> v4(v3);         // 深拷贝 v3

    // 方式4：迭代器范围构造
    vector<int> v5(v3.begin(), v3.end());

    // 方式5：初始化列表（C++11）
    vector<int> v6 = {1, 2, 3, 4, 5};

    printVec(v2, "v2");   // v2 (size=5): 0 0 0 0 0
    printVec(v3, "v3");   // v3 (size=5): 100 100 100 100 100
    printVec(v4, "v4");   // v4 (size=5): 100 100 100 100 100
    printVec(v5, "v5");   // v5 (size=5): 100 100 100 100 100
    printVec(v6, "v6");   // v6 (size=5): 1 2 3 4 5

    return 0;
}
```

---

## 三、vector 的赋值操作

### 3.1 使用 `=` 运算符赋值

```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2;
v2 = v1;  // 将 v1 的所有元素赋值给 v2（深拷贝）
```

### 3.2 使用 `assign` 方法赋值

```cpp
vector<int> v3;
v3.assign(v1.begin(), v1.end());   // 用迭代器范围赋值

vector<int> v4;
v4.assign(5, 88);                  // 赋值 5 个 88

vector<int> v5;
v5.assign({10, 20, 30});           // 用初始化列表赋值（C++11）
```

### 3.3 assign 与 = 的区别

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v1 = {1, 2, 3, 4, 5};

    // = 运算符：整体替换
    vector<int> v2;
    v2 = v1;  // v2: 1 2 3 4 5

    // assign：可以选择部分赋值
    vector<int> v3;
    v3.assign(v1.begin() + 1, v1.begin() + 4);  // v3: 2 3 4

    // assign：赋值指定数量的相同值
    vector<int> v4;
    v4.assign(3, 99);  // v4: 99 99 99

    cout << "v2: ";
    for (int x : v2) cout << x << " ";  // 1 2 3 4 5
    cout << endl;

    cout << "v3: ";
    for (int x : v3) cout << x << " ";  // 2 3 4
    cout << endl;

    cout << "v4: ";
    for (int x : v4) cout << x << " ";  // 99 99 99
    cout << endl;

    return 0;
}
```

---

## 四、容量与大小操作

### 4.1 常用函数

| 函数 | 说明 |
|------|------|
| `size()` | 返回当前元素个数 |
| `capacity()` | 返回当前分配的存储容量 |
| `empty()` | 判断容器是否为空 |
| `resize(n)` | 调整容器大小为 n（多删少补） |
| `resize(n, val)` | 调整大小，新增元素用 val 填充 |
| `reserve(n)` | 预留 n 个空间（不改变 size） |
| `shrink_to_fit()` | 释放多余容量，使 capacity == size |

### 4.2 size、capacity、empty 的区别

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;

    // 初始状态：size=0, capacity=0
    cout << "初始: size=" << v.size()
         << ", capacity=" << v.capacity()
         << ", empty=" << v.empty() << endl;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    // size=3, capacity=4（具体值取决于编译器实现）
    cout << "添加3个元素后: size=" << v.size()
         << ", capacity=" << v.capacity() << endl;

    // size 表示实际存储了多少个元素
    // capacity 表示已经分配了多少存储空间（可能大于 size）
    // empty 等价于 size == 0

    return 0;
}
```

### 4.3 增长策略（翻倍机制）

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;

    // 观察 vector 的扩容过程
    int oldCapacity = 0;
    for (int i = 0; i < 100; i++) {
        v.push_back(i);
        if (v.capacity() != oldCapacity) {
            cout << "size=" << v.size()
                 << " 时扩容: capacity 从 " << oldCapacity
                 << " 变为 " << v.capacity() << endl;
            oldCapacity = v.capacity();
        }
    }
    // 典型输出（GCC/MSVC）：
    // size=1 时扩容: capacity 从 0 变为 1
    // size=2 时扩容: capacity 从 1 变为 2
    // size=3 时扩容: capacity 从 2 变为 4
    // size=5 时扩容: capacity 从 4 变为 8
    // size=9 时扩容: capacity 从 8 变为 16
    // ... 每次翻倍

    return 0;
}
```

### 4.4 resize 与 reserve 的区别（重要！）

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // ========== resize ==========
    // resize 改变的是 size（元素个数）
    // 会真正构造或销毁元素
    vector<int> v1;
    v1.resize(5);          // v1.size() == 5，有5个0
    v1.resize(8, 99);      // v1.size() == 8，多出的3个是99
    v1.resize(3);          // v1.size() == 3，丢弃后面的元素

    cout << "v1: ";
    for (int x : v1) cout << x << " ";  // 0 0 0
    cout << endl;

    // ========== reserve ==========
    // reserve 改变的是 capacity（预分配空间）
    // 不会构造新元素，size 不变
    vector<int> v2;
    v2.reserve(100);  // 预分配100个空间，但 size 仍为 0
    cout << "v2: size=" << v2.size()          // 0
         << ", capacity=" << v2.capacity();   // 100
    cout << endl;

    // reserve 的好处：提前知道需要多少元素时，避免多次扩容
    vector<int> v3;
    v3.reserve(10000);  // 一次性分配好，后续 push_back 不会触发扩容
    for (int i = 0; i < 10000; i++) {
        v3.push_back(i);  // 不会反复拷贝数据
    }

    return 0;
}
```

### 4.5 shrink_to_fit 收缩容量

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    v.reserve(1000);  // capacity = 1000
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    // size=3, capacity=1000，浪费了997个位置的内存

    cout << "收缩前: size=" << v.size()
         << ", capacity=" << v.capacity() << endl;

    v.shrink_to_fit();  // 释放多余空间

    cout << "收缩后: size=" << v.size()
         << ", capacity=" << v.capacity() << endl;
    // size=3, capacity=3

    return 0;
}
```

---

## 五、插入与删除操作

### 5.1 常用函数

| 函数 | 说明 |
|------|------|
| `push_back(val)` | 在尾部添加元素 |
| `pop_back()` | 删除尾部元素 |
| `insert(pos, val)` | 在迭代器 pos 位置插入元素 |
| `insert(pos, n, val)` | 在 pos 位置插入 n 个值为 val 的元素 |
| `erase(pos)` | 删除迭代器 pos 指向的元素 |
| `erase(first, last)` | 删除 [first, last) 范围的元素 |
| `clear()` | 清空所有元素（size 变为 0） |

### 5.2 完整示例

```cpp
#include <iostream>
#include <vector>
using namespace std;

void printVec(const vector<int>& v) {
    cout << "[size=" << v.size() << "] ";
    for (int x : v) cout << x << " ";
    cout << endl;
}

int main() {
    vector<int> v = {1, 2, 3, 4, 5};

    // push_back：在尾部添加
    v.push_back(6);
    printVec(v);  // [size=6] 1 2 3 4 5 6

    // pop_back：删除尾部
    v.pop_back();
    printVec(v);  // [size=5] 1 2 3 4 5

    // insert：在指定位置插入（需要迭代器）
    v.insert(v.begin() + 2, 99);  // 在下标2的位置插入99
    printVec(v);  // [size=6] 1 2 99 3 4 5

    // insert：在指定位置插入多个相同元素
    v.insert(v.begin(), 3, 88);  // 在开头插入3个88
    printVec(v);  // [size=9] 88 88 88 1 2 99 3 4 5

    // erase：删除指定位置的元素
    v.erase(v.begin());  // 删除第一个元素
    printVec(v);  // [size=8] 88 88 1 2 99 3 4 5

    // erase：删除范围 [first, last)
    v.erase(v.begin(), v.begin() + 2);  // 删除前2个
    printVec(v);  // [size=6] 1 2 99 3 4 5

    // clear：清空所有元素
    v.clear();
    cout << "clear后: size=" << v.size()
         << ", empty=" << v.empty() << endl;
    // size=0, empty=1

    return 0;
}
```

### 5.3 注意：insert/erase 后迭代器失效

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};

    // 错误示范：erase 后继续使用原迭代器
    // auto it = v.begin() + 2;  // 指向 3
    // v.erase(it);
    // cout << *it;  // 未定义行为！it 已失效

    // 正确做法：用 erase 的返回值获取新迭代器
    auto it = v.begin() + 2;  // 指向 3
    it = v.erase(it);  // 删除 3，it 现在指向 4
    cout << "删除后 it 指向: " << *it << endl;  // 4

    return 0;
}
```

---

## 六、元素存取操作

### 6.1 常用函数

| 函数 | 说明 |
|------|------|
| `operator[]` | 按下标访问，不做边界检查 |
| `at(index)` | 按下标访问，越界抛 out_of_range 异常 |
| `front()` | 返回第一个元素的引用 |
| `back()` | 返回最后一个元素的引用 |
| `data()` | 返回底层数组的指针（C++11） |

### 6.2 完整示例

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {10, 20, 30, 40, 50};

    // [] 运算符：不检查边界，越界是未定义行为
    cout << "v[0] = " << v[0] << endl;  // 10
    cout << "v[2] = " << v[2] << endl;  // 30

    // at()：检查边界，越界抛异常
    cout << "v.at(3) = " << v.at(3) << endl;  // 40
    try {
        cout << v.at(10) << endl;  // 越界！抛异常
    } catch (const out_of_range& e) {
        cout << "异常: " << e.what() << endl;
    }

    // front() 和 back()
    cout << "第一个元素: " << v.front() << endl;  // 10
    cout << "最后一个元素: " << v.back() << endl;  // 50

    // data()：获取底层数组指针，可以当普通数组用
    int* p = v.data();
    cout << "通过指针访问第2个: " << p[1] << endl;  // 20

    // 修改元素
    v[0] = 100;         // 用 [] 修改
    v.at(1) = 200;      // 用 at 修改
    v.front() = 1000;   // 用 front 修改

    cout << "修改后: ";
    for (int x : v) cout << x << " ";  // 1000 200 30 40 50
    cout << endl;

    return 0;
}
```

---

## 七、swap 收缩内存技巧

`swap` 不仅可以交换两个 vector 的内容，还能用来**真正释放多余内存**。

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;
    v.reserve(10000);  // 预分配 10000 个空间
    for (int i = 0; i < 5; i++) {
        v.push_back(i);
    }
    cout << "size=" << v.size()            // 5
         << ", capacity=" << v.capacity()  // 10000
         << endl;

    // 方法1：shrink_to_fit（C++11）
    // v.shrink_to_fit();

    // 方法2：swap 技巧（兼容 C++98）
    // 原理：用 v 的数据构造一个临时 vector（容量刚好=大小）
    // 再与 v 交换，临时 vector 销毁时释放原来的大量内存
    vector<int>(v).swap(v);

    cout << "swap后: size=" << v.size()            // 5
         << ", capacity=" << v.capacity()          // 5
         << endl;

    return 0;
}
```

**原理解析：**
1. `vector<int>(v)` -- 调用拷贝构造，创建一个临时 vector，它的 capacity 恰好等于 v 的 size
2. `.swap(v)` -- 将临时 vector 与 v 交换
3. 临时 vector（持有原来的大块内存）在语句结束后自动销毁，释放内存

---

## 八、迭代器详解

### 8.1 四种迭代器

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {10, 20, 30, 40, 50};

    // 正向迭代器：begin() -> end()
    cout << "正向遍历: ";
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";  // 10 20 30 40 50
    }
    cout << endl;

    // 反向迭代器：rbegin() -> rend()
    cout << "反向遍历: ";
    for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
        cout << *it << " ";  // 50 40 30 20 10
    }
    cout << endl;

    // const 迭代器（只读，不能修改元素）
    cout << "只读遍历: ";
    for (vector<int>::const_iterator it = v.cbegin(); it != v.cend(); it++) {
        cout << *it << " ";
        // *it = 99;  // 编译错误！const 迭代器不能修改
    }
    cout << endl;

    return 0;
}
```

### 8.2 迭代器失效问题

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {1, 2, 3, 4, 5};

    // ========== 情况1：insert 可能导致迭代器失效 ==========
    // 因为 insert 可能触发扩容，所有迭代器都失效
    auto it = v.begin();
    // v.insert(it, 99);  // 插入后 it 失效！

    // ========== 情况2：erase 导致被删除位置及之后的迭代器失效 ==========
    // 正确做法：用返回值更新迭代器
    for (auto it = v.begin(); it != v.end(); /* 不写 it++ */) {
        if (*it == 3) {
            it = v.erase(it);  // erase 返回下一个有效迭代器
        } else {
            it++;
        }
    }
    // 结果：1 2 4 5（3 被删除）

    // ========== 情况3：扩容导致所有迭代器失效 ==========
    v.reserve(100);  // 如果触发扩容，之前的所有迭代器失效
    auto it2 = v.begin();  // 需要重新获取
    cout << *it2 << endl;   // 安全

    return 0;
}
```

---

## 九、遍历方式汇总

```cpp
#include <iostream>
#include <vector>
#include <algorithm>  // for_each 需要
using namespace std;

int main() {
    vector<int> v = {10, 20, 30, 40, 50};

    // ========== 方式1：下标遍历 ==========
    cout << "下标遍历: ";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    // ========== 方式2：迭代器遍历 ==========
    cout << "迭代器遍历: ";
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    // ========== 方式3：范围 for（C++11，推荐） ==========
    cout << "范围for: ";
    for (int x : v) {       // 值拷贝（只读够用）
        cout << x << " ";
    }
    cout << endl;

    cout << "范围for引用: ";
    for (const auto& x : v) {  // 常量引用（避免拷贝，推荐）
        cout << x << " ";
    }
    cout << endl;

    // 需要修改元素时用引用
    for (auto& x : v) {
        x *= 2;  // 每个元素翻倍
    }
    cout << "翻倍后: ";
    for (const auto& x : v) cout << x << " ";  // 20 40 60 80 100
    cout << endl;

    // ========== 方式4：for_each 遍历 ==========
    cout << "for_each遍历: ";
    for_each(v.begin(), v.end(), [](int x) {
        cout << x << " ";
    });
    cout << endl;

    return 0;
}
```

---

## 十、综合实战案例

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    // 案例：用 vector 学生管理系统
    vector<pair<string, int>> students;  // 姓名, 成绩

    // 添加学生
    students.push_back({"张三", 85});
    students.push_back({"李四", 92});
    students.push_back({"王五", 78});
    students.push_back({"赵六", 95});
    students.push_back({"孙七", 88});

    // 按成绩降序排序
    sort(students.begin(), students.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });

    // 输出排名
    cout << "===== 成绩排名 =====" << endl;
    for (size_t i = 0; i < students.size(); i++) {
        cout << "第" << i + 1 << "名: "
             << students[i].first
             << " - " << students[i].second << "分" << endl;
    }

    // 查找最高分和最低分
    auto maxIt = max_element(students.begin(), students.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        });
    auto minIt = min_element(students.begin(), students.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second < b.second;
        });

    cout << "\n最高分: " << maxIt->first << " " << maxIt->second << "分" << endl;
    cout << "最低分: " << minIt->first << " " << minIt->second << "分" << endl;

    // 计算平均分
    int total = 0;
    for (const auto& s : students) {
        total += s.second;
    }
    cout << "平均分: " << (double)total / students.size() << "分" << endl;

    return 0;
}
```

---

## 十一、总结对比表

| 操作 | 函数 | 时间复杂度 |
|------|------|-----------|
| 尾部添加 | `push_back()` | O(1) 均摊 |
| 尾部删除 | `pop_back()` | O(1) |
| 随机访问 | `[]` / `at()` | O(1) |
| 中间插入 | `insert()` | O(n) |
| 中间删除 | `erase()` | O(n) |
| 查找 | `find()` | O(n) |
| 预留空间 | `reserve()` | O(1) |
| 调整大小 | `resize()` | O(n) 最坏 |
| 清空 | `clear()` | O(n) |

**使用建议：**
- 需要随机访问且主要在尾部操作时，优先选 vector
- 提前知道元素数量时，用 `reserve()` 预分配
- 频繁在头部/中间插入删除时，考虑用 `list` 或 `deque`
