# list 链表详解

## 一、什么是 list？

`list` 是 C++ STL 中的**双向链表**容器。它在内存中不连续存储，每个元素通过指针连接前后元素。

### 核心特点

| 特点 | 说明 |
|------|------|
| 底层结构 | 双向链表（每个节点有前驱和后继指针） |
| 任意位置插入删除 | **O(1)** 时间复杂度（已知位置时） |
| 不支持随机访问 | 不能用 `[]` 或 `at()` 直接访问第n个元素 |
| 内存 | 不连续，每个节点额外存储两个指针 |

```cpp
#include <iostream>
#include <list>
using namespace std;

int main() {
    // 创建一个整型双向链表
    list<int> lst;

    // list 不支持随机访问，以下代码会编译错误！
    // lst[0];    // 错误！list没有operator[]
    // lst.at(0); // 错误！list没有at()方法

    cout << "list 创建成功" << endl;
    return 0;
}
```

## 二、list 与 vector 的区别

这是面试高频考点，必须牢记：

| 对比项 | vector | list |
|--------|--------|------|
| 底层结构 | 动态数组（连续内存） | 双向链表（不连续内存） |
| 随机访问 | 支持，O(1) | **不支持**，O(n) |
| 头部插入删除 | O(n)，需要移动元素 | **O(1)** |
| 尾部插入 | 均摊O(1) | O(1) |
| 中间插入删除 | O(n) | O(1)（已知迭代器位置） |
| 内存开销 | 较小 | 较大（额外指针） |
| 缓存友好 | 好（连续内存） | 差（内存不连续） |
| 迭代器类型 | 随机访问迭代器 | **双向迭代器** |

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <ctime>
using namespace std;

int main() {
    // vector：支持随机访问
    vector<int> v = {1, 2, 3, 4, 5};
    cout << "vector第3个元素: " << v[2] << endl;  // O(1) 直接访问

    // list：不支持随机访问，只能遍历
    list<int> lst = {1, 2, 3, 4, 5};
    // cout << lst[2]; // 编译错误！

    // 要访问list的第3个元素，只能用迭代器移动
    auto it = lst.begin();
    ++it; // 移动到第2个
    ++it; // 移动到第3个
    cout << "list第3个元素: " << *it << endl;

    // 头部插入：list 比 vector 快得多
    // vector 头插需要把所有元素往后移，O(n)
    // list 头插只需要改指针，O(1)
    lst.push_front(0); // 头插，O(1)

    return 0;
}
```

## 三、list 的构造函数（5种）

```cpp
#include <iostream>
#include <list>
using namespace std;

// 打印链表的辅助函数
void printList(const list<int>& lst, const string& name) {
    cout << name << ": ";
    for (list<int>::const_iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << " (大小: " << lst.size() << ")" << endl;
}

int main() {
    // ========== 构造方式1：默认构造（空链表） ==========
    list<int> lst1;
    printList(lst1, "lst1默认构造");

    // ========== 构造方式2：构造并指定n个相同元素 ==========
    // list<int>(n, value) —— 创建包含n个value的链表
    list<int> lst2(5, 100); // 5个100
    printList(lst2, "lst2(5,100)");

    // ========== 构造方式3：拷贝构造 ==========
    // 用另一个list来初始化
    list<int> lst3(lst2);
    printList(lst3, "lst3拷贝构造");

    // ========== 构造方式4：迭代器区间构造 ==========
    // 用另一个容器的迭代器区间来构造
    int arr[] = {10, 20, 30, 40, 50};
    list<int> lst4(arr, arr + 5); // 用数组初始化
    printList(lst4, "lst4迭代器区间");

    // ========== 构造方式5：初始化列表（C++11） ==========
    list<int> lst5 = {1, 2, 3, 4, 5};
    printList(lst5, "lst5初始化列表");

    return 0;
}
```

**输出：**
```
lst1默认构造:  (大小: 0)
lst2(5,100): 100 100 100 100 100  (大小: 5)
lst3拷贝构造: 100 100 100 100 100  (大小: 5)
lst4迭代器区间: 10 20 30 40 50  (大小: 5)
lst5初始化列表: 1 2 3 4 5  (大小: 5)
```

## 四、赋值与大小操作

```cpp
#include <iostream>
#include <list>
using namespace std;

void printList(const list<int>& lst, const string& name) {
    cout << name << ": ";
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int main() {
    list<int> lst1 = {1, 2, 3};
    list<int> lst2;

    // ========== 赋值操作 ==========
    // 方式1：operator=
    lst2 = lst1;
    printList(lst2, "赋值后lst2");

    // 方式2：assign(n, value) —— 用n个value赋值
    lst2.assign(4, 99);
    printList(lst2, "assign(4,99)");

    // 方式3：assign迭代器区间
    lst2.assign(lst1.begin(), lst1.end());
    printList(lst2, "assign迭代器区间");

    // ========== 大小操作 ==========
    cout << "lst1大小: " << lst1.size() << endl;    // 3
    cout << "lst1是否为空: " << lst1.empty() << endl; // 0（不为空）

    // resize：调整大小
    lst1.resize(5, 10); // 扩大到5个，新元素用10填充
    printList(lst1, "resize(5,10)");

    lst1.resize(2); // 缩小到2个，多余元素被删除
    printList(lst1, "resize(2)");

    // ========== 交换 ==========
    list<int> a = {1, 2, 3};
    list<int> b = {4, 5};
    a.swap(b);
    printList(a, "swap后a");
    printList(b, "swap后b");

    return 0;
}
```

## 五、插入与删除操作（重点）

这是 list 最核心的操作，list 的优势就在于高效的插入和删除。

```cpp
#include <iostream>
#include <list>
using namespace std;

void printList(const list<int>& lst, const string& name) {
    cout << name << ": ";
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int main() {
    list<int> lst = {1, 2, 3, 4, 5};

    // ==================== 尾部操作 ====================
    // push_back：尾部添加元素
    lst.push_back(6);
    printList(lst, "push_back(6)");  // 1 2 3 4 5 6

    // pop_back：删除尾部元素
    lst.pop_back();
    printList(lst, "pop_back()");    // 1 2 3 4 5

    // ==================== 头部操作 ====================
    // push_front：头部添加元素（这是list独有的，vector没有！）
    lst.push_front(0);
    printList(lst, "push_front(0)"); // 0 1 2 3 4 5

    // pop_front：删除头部元素（也是list独有的）
    lst.pop_front();
    printList(lst, "pop_front()");   // 1 2 3 4 5

    // ==================== 任意位置插入 insert ====================
    // insert(iterator, value) —— 在迭代器位置前插入
    auto it = lst.begin();
    ++it; // 指向第2个元素（值为2）
    lst.insert(it, 99); // 在2前面插入99
    printList(lst, "insert在2前插入99"); // 1 99 2 3 4 5

    // insert(iterator, n, value) —— 插入n个相同元素
    it = lst.begin();
    lst.insert(it, 3, 88); // 在开头插入3个88
    printList(lst, "插入3个88"); // 88 88 88 1 99 2 3 4 5

    // ==================== 任意位置删除 erase ====================
    // erase(iterator) —— 删除迭代器指向的元素
    it = lst.begin();
    ++it; // 指向第2个88
    lst.erase(it);
    printList(lst, "erase第2个88"); // 88 88 1 99 2 3 4 5

    // erase(first, last) —— 删除区间[first, last)
    it = lst.begin();
    auto it2 = lst.begin();
    advance(it2, 3); // it2前进3步，指向第4个元素
    lst.erase(it, it2); // 删除前3个元素
    printList(lst, "erase前3个"); // 99 2 3 4 5

    // ==================== remove：删除所有匹配值 ====================
    // 注意：remove按值删除，删除所有等于该值的元素
    list<int> lst2 = {1, 2, 3, 2, 4, 2, 5};
    lst2.remove(2); // 删除所有值为2的元素
    printList(lst2, "remove(2)"); // 1 3 4 5

    // ==================== clear：清空所有元素 ====================
    lst2.clear();
    cout << "clear后大小: " << lst2.size() << endl; // 0

    return 0;
}
```

### 重点总结

| 操作 | 说明 | 时间复杂度 |
|------|------|-----------|
| `push_back(val)` | 尾部添加 | O(1) |
| `pop_back()` | 删除尾部 | O(1) |
| `push_front(val)` | 头部添加 | O(1) |
| `pop_front()` | 删除头部 | O(1) |
| `insert(it, val)` | 迭代器位置前插入 | O(1) |
| `erase(it)` | 删除迭代器位置 | O(1) |
| `remove(val)` | 删除所有等于val的元素 | O(n) |
| `clear()` | 清空 | O(n) |

> **注意：** `push_front` 和 `pop_front` 是 list 独有的操作，vector 没有这两个函数。

## 六、特殊操作（sort、reverse、unique、merge、splice）

这些是 list 特有的成员函数，因为 list 的迭代器是双向迭代器，不能使用标准算法中的 `sort`。

```cpp
#include <iostream>
#include <list>
using namespace std;

void printList(const list<int>& lst, const string& name) {
    cout << name << ": ";
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int main() {
    // ==================== sort 排序 ====================
    // 重要：list 必须用自己的 sort 成员函数，不能用 <algorithm> 的 sort！
    // 因为 std::sort 需要随机访问迭代器，而 list 只有双向迭代器
    list<int> lst = {5, 3, 1, 4, 2};
    lst.sort(); // 默认升序排序
    printList(lst, "sort升序"); // 1 2 3 4 5

    // 降序排序：传入 greater<int>() 仿函数
    lst.sort(greater<int>());
    printList(lst, "sort降序"); // 5 4 3 2 1

    // 以下代码会编译错误！list不能用std::sort
    // sort(lst.begin(), lst.end()); // 错误！

    // ==================== reverse 反转 ====================
    lst.reverse();
    printList(lst, "reverse反转"); // 1 2 3 4 5（变回升序）

    // ==================== unique 去重 ====================
    // 重要：unique 只能去除"相邻"的重复元素，所以必须先排序！
    list<int> lst2 = {1, 3, 2, 3, 1, 2, 3};
    lst2.unique(); // 没排序就去重，效果不对
    printList(lst2, "未排序去重(不对)"); // 1 3 2 3 1 2 3（没有相邻重复，没变化）

    lst2.sort(); // 先排序
    printList(lst2, "排序后");      // 1 1 2 2 3 3 3
    lst2.unique(); // 再去重
    printList(lst2, "排序后去重");   // 1 2 3

    // ==================== merge 合并有序链表 ====================
    // 要求：两个链表必须已经排好序，合并后仍然有序
    list<int> a = {1, 3, 5, 7};
    list<int> b = {2, 4, 6, 8};
    a.merge(b); // 合并b到a中，b变为空
    printList(a, "merge合并"); // 1 2 3 4 5 6 7 8
    printList(b, "merge后b");  // 空（b的元素被移到a中）

    // ==================== splice 拼接 ====================
    // splice：将一个链表的元素"剪切"到另一个链表
    list<int> lst3 = {1, 2, 3};
    list<int> lst4 = {10, 20, 30};

    // splice(iterator pos, list& other)
    // 将other的所有元素插入到pos位置前，other变为空
    auto it = lst3.begin();
    ++it; // 指向2
    lst3.splice(it, lst4); // 把lst4的所有元素插到2前面
    printList(lst3, "splice后lst3"); // 1 10 20 30 2 3
    printList(lst4, "splice后lst4"); // 空

    // splice也可以只移动一个元素
    list<int> lst5 = {100, 200};
    list<int> lst6 = {1, 2, 3};
    auto src = lst5.begin();    // 指向100
    auto dest = lst6.begin();
    ++dest; // 指向2
    lst6.splice(dest, lst5, src); // 把100移到2前面
    printList(lst6, "splice单个"); // 1 100 2 3

    return 0;
}
```

## 七、迭代器使用详解

list 的迭代器是**双向迭代器**，只能 `++` 和 `--`，不能 `+n` 或 `-n`。

```cpp
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;

int main() {
    list<int> lst = {10, 20, 30, 40, 50};

    // ========== 正向遍历 ==========
    cout << "正向遍历: ";
    for (list<int>::iterator it = lst.begin(); it != lst.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // ========== 反向遍历 ==========
    cout << "反向遍历: ";
    for (list<int>::reverse_iterator it = lst.rbegin(); it != lst.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // ========== 只读遍历（const_iterator） ==========
    cout << "只读遍历: ";
    for (list<int>::const_iterator it = lst.cbegin(); it != lst.cend(); ++it) {
        cout << *it << " ";
        // *it = 100; // 错误！const_iterator不能修改值
    }
    cout << endl;

    // ========== C++11 范围for ==========
    cout << "范围for: ";
    for (auto& val : lst) {
        cout << val << " ";
    }
    cout << endl;

    // ========== 双向迭代器的限制 ==========
    auto it = lst.begin();
    ++it;  // 正确：前进1步
    --it;  // 正确：后退1步
    // it = it + 3;  // 错误！不能+n
    // it = it - 2;  // 错误！不能-n
    // 但可以用 advance 函数来移动迭代器
    advance(it, 3); // it前进3步，指向40
    cout << "advance后: " << *it << endl; // 40

    // distance：计算两个迭代器之间的距离
    auto d = distance(lst.begin(), it);
    cout << "距离: " << d << endl; // 3

    return 0;
}
```

## 八、迭代器失效问题

这是使用 list 必须了解的重要概念。

```cpp
#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> lst = {1, 2, 3, 4, 5};

    // ========== 删除操作与迭代器失效 ==========
    // list 的 erase 只会使被删除元素的迭代器失效
    // 其他迭代器不受影响（这是list的优势！）

    auto it = lst.begin();
    ++it; // 指向2

    // 删除元素3（先找到3的位置）
    auto it3 = lst.begin();
    advance(it3, 2); // 指向3
    lst.erase(it3);  // 删除3

    // it仍然有效！仍然指向2
    cout << "删除3后，it仍然指向: " << *it << endl; // 2

    // ========== 错误示范：使用已失效的迭代器 ==========
    auto it2 = lst.begin();
    advance(it2, 2); // 现在指向4（因为3已被删除）
    lst.erase(it2);  // 删除4
    // 此时 it2 已失效，不能再使用！
    // cout << *it2; // 未定义行为！绝对不能这样做！

    // ========== 正确的删除循环 ==========
    // 错误写法：
    // for (auto it = lst.begin(); it != lst.end(); ++it) {
    //     if (*it == 2) lst.erase(it); // 错！erase后it失效，++it未定义
    // }

    // 正确写法：用erase的返回值
    for (auto it = lst.begin(); it != lst.end(); ) {
        if (*it == 2) {
            it = lst.erase(it); // erase返回下一个有效迭代器
        } else {
            ++it;
        }
    }

    // 更简单的写法：用remove
    lst.remove(5); // 删除所有值为5的元素，不需要担心迭代器

    cout << "最终链表: ";
    for (auto val : lst) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

### 迭代器失效总结

| 容器 | 插入操作 | 删除操作 |
|------|---------|---------|
| vector | 可能全部失效（扩容时） | 被删及之后全部失效 |
| list | **不失效** | **仅被删元素失效** |
| deque | 全部失效 | 被删及之后失效 |

> **list 的核心优势之一：** 插入不会使任何迭代器失效，删除只使被删除元素的迭代器失效。

## 九、综合实战：用 list 实现简易通讯录

```cpp
#include <iostream>
#include <list>
#include <string>
using namespace std;

struct Contact {
    string name;
    string phone;

    // 重载<运算符，按姓名排序
    bool operator<(const Contact& other) const {
        return name < other.name;
    }
};

// 打印所有联系人
void printContacts(const list<Contact>& contacts) {
    cout << "------ 通讯录 ------" << endl;
    for (const auto& c : contacts) {
        cout << "姓名: " << c.name << ", 电话: " << c.phone << endl;
    }
    cout << "--------------------" << endl;
}

int main() {
    list<Contact> contacts;

    // 添加联系人
    contacts.push_back({"张三", "13800001111"});
    contacts.push_back({"李四", "13900002222"});
    contacts.push_back({"王五", "13700003333"});
    contacts.push_back({"赵六", "13600004444"});

    cout << "添加后：" << endl;
    printContacts(contacts);

    // 按姓名排序
    contacts.sort();
    cout << "排序后：" << endl;
    printContacts(contacts);

    // 删除联系人（按姓名匹配）
    // remove需要Contact支持==比较，这里用remove_if
    contacts.remove_if([](const Contact& c) {
        return c.name == "李四";
    });
    cout << "删除李四后：" << endl;
    printContacts(contacts);

    return 0;
}
```

## 十、总结

| 要点 | 内容 |
|------|------|
| 底层结构 | 双向链表 |
| 随机访问 | 不支持 |
| 头尾插入删除 | O(1)，非常高效 |
| 中间插入删除 | O(1)（已知位置时） |
| 排序 | 用成员函数 `sort()`，不能用 `std::sort` |
| 去重 | 先 `sort()` 再 `unique()` |
| 迭代器 | 双向迭代器，只能 `++` `--` |
| 迭代器失效 | 插入不失效，删除仅被删元素失效 |
| 适用场景 | 频繁在任意位置插入删除，不需要随机访问 |
