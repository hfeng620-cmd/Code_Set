# STL 概述 —— C++ 标准模板库入门

## 一、什么是 STL？

STL（Standard Template Library，标准模板库）是 C++ 最核心的库之一。
它提供了一套**通用的模板类和函数**，包含常用的数据结构和算法。

**通俗理解：**
STL 就是一个"工具箱"，里面装好了各种现成的容器（数据结构）和算法，
你不需要自己从零写链表、排序、查找等，直接拿来用就行。

```cpp
#include <iostream>
#include <vector>   // 使用 vector 容器需要包含这个头文件
#include <algorithm> // 使用 sort 等算法需要包含这个头文件
using namespace std;

int main()
{
    // 创建一个 vector 容器，类似于"可变长度的数组"
    vector<int> v;

    // 向容器中添加数据
    v.push_back(30);
    v.push_back(10);
    v.push_back(50);
    v.push_back(20);
    v.push_back(40);

    // 使用 STL 提供的排序算法（从小到大）
    sort(v.begin(), v.end());

    // 遍历输出
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";  // 输出：10 20 30 40 50
    }
    cout << endl;

    return 0;
}
```

**STL 的好处：**
- 代码复用，不用重复造轮子
- 效率高，经过严格测试和优化
- 跨平台，标准统一


---

## 二、STL 的六大组件

STL 由以下 **六大组件** 构成：

```
┌─────────────────────────────────────────────┐
│                  STL 六大组件                │
├──────────┬──────────┬──────────┬────────────┤
│  容器     │  算法     │  迭代器   │  仿函数    │
│ Container│ Algorithm │ Iterator │ Function   │
├──────────┼──────────┼──────────┼────────────┤
│  适配器   │          │  空间配置器            │
│ Adapter  │          │  Allocator             │
└──────────┴──────────┴───────────────────────┘
```

### 1. 容器（Container）

容器就是用来**存放数据**的类模板，类似于各种"数据结构"。

```cpp
#include <iostream>
#include <vector>    // 动态数组
#include <list>      // 双向链表
#include <deque>     // 双端队列
#include <set>       // 集合（自动排序，无重复）
#include <map>       // 映射（键值对，按键排序）
#include <stack>     // 栈
#include <queue>     // 队列
using namespace std;

int main()
{
    // ---- 序列式容器：元素按插入顺序排列 ----

    // vector：动态数组，支持随机访问
    vector<int> vec = {1, 2, 3, 4, 5};
    cout << "vector 第一个元素: " << vec[0] << endl;  // 1

    // deque：双端队列，两端都能快速插入删除
    deque<int> deq = {10, 20, 30};
    deq.push_front(0);   // 在头部插入
    deq.push_back(40);   // 在尾部插入

    // list：双向链表，任意位置插入删除都很快
    list<int> lst = {100, 200, 300};

    // ---- 关联式容器：自动排序 ----

    // set：集合，元素不重复，自动从小到大排序
    set<int> s;
    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);  // 重复元素不会被插入

    // map：键值对映射，按键自动排序
    map<string, int> m;
    m["张三"] = 90;
    m["李四"] = 85;
    m["王五"] = 95;

    // ---- 容器适配器 ----

    // stack：栈，先进后出
    stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    cout << "栈顶元素: " << stk.top() << endl;  // 3

    // queue：队列，先进先出
    queue<int> que;
    que.push(1);
    que.push(2);
    que.push(3);
    cout << "队头元素: " << que.front() << endl;  // 1

    return 0;
}
```


### 2. 算法（Algorithm）

STL 提供了大量**通用算法**，通过模板实现，可以作用于各种容器。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>  // 算法头文件
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // ---- 非质变算法：不改变容器内容 ----

    // 查找元素（find 返回指向该元素的迭代器）
    auto it = find(v.begin(), v.end(), 3);
    if (it != v.end())
    {
        cout << "找到了元素 3" << endl;
    }

    // 计数
    int cnt = count(v.begin(), v.end(), 3);
    cout << "元素 3 出现了 " << cnt << " 次" << endl;

    // ---- 质变算法：会改变容器内容 ----

    // 排序（从小到大）
    sort(v.begin(), v.end());
    // 排序后：1 2 3 4 5

    // 反转
    reverse(v.begin(), v.end());
    // 反转后：5 4 3 2 1

    // 替换：把所有 3 替换成 30
    vector<int> v2 = {1, 3, 5, 3, 7};
    replace(v2.begin(), v2.end(), 3, 30);
    // 替换后：1 30 5 30 7

    // 遍历输出
    for (auto val : v2)
    {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```


### 3. 迭代器（Iterator）

迭代器是**容器和算法之间的桥梁**。
它就像一个"智能指针"，可以指向容器中的元素，让算法能够遍历容器。

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v = {10, 20, 30, 40, 50};

    // 声明一个迭代器，类型和容器一致
    // vector<int>::iterator 就是 vector<int> 的迭代器类型
    vector<int>::iterator it = v.begin();

    // begin() 返回指向第一个元素的迭代器
    // end()   返回指向最后一个元素**后面一个位置**的迭代器（不指向有效元素）

    // 用迭代器遍历容器
    cout << "使用迭代器遍历: ";
    for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++)
    {
        // *iter 就是取迭代器指向的元素值（类似指针解引用）
        cout << *iter << " ";
    }
    cout << endl;
    // 输出：10 20 30 40 50

    // C++11 简化写法：auto 自动推导类型
    cout << "auto 简化遍历: ";
    for (auto iter = v.begin(); iter != v.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;

    // C++11 更简单的写法：范围 for
    cout << "范围 for 遍历: ";
    for (auto val : v)
    {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

**迭代器的种类（从弱到强）：**

| 迭代器种类 | 功能 | 支持的容器 |
|-----------|------|-----------|
| 输入迭代器 | 只读，只能前进 | istream_iterator |
| 输出迭代器 | 只写，只能前进 | ostream_iterator |
| 前进迭代器 | 读写，只能前进 | forward_list |
| 双向迭代器 | 读写，可前进可后退 | list, set, map |
| 随机访问迭代器 | 读写，可任意跳转 | vector, deque, array |

```cpp
#include <iostream>
#include <vector>
#include <list>
using namespace std;

int main()
{
    // ---- 随机访问迭代器（vector）----
    vector<int> v = {10, 20, 30, 40, 50};
    vector<int>::iterator vit = v.begin();

    // 随机访问：可以直接跳到任意位置
    vit += 3;           // 前进 3 步，指向第 4 个元素（40）
    cout << *vit << endl; // 40

    cout << "vit[1] = " << vit[1] << endl;  // 50（vit 当前位置+1）

    // ---- 双向迭代器（list）----
    list<int> lst = {1, 2, 3, 4, 5};
    list<int>::iterator lit = lst.begin();

    // 双向迭代器：只能一步一步前进或后退
    lit++;    // 前进一步，OK
    lit--;    // 后退一步，OK
    // lit += 3;  // 错误！双向迭代器不支持随机跳转

    cout << *lit << endl;  // 1

    return 0;
}
```

> **常见错误：**
> ```cpp
> // 错误：对 end() 迭代器解引用
> vector<int> v = {1, 2, 3};
> cout << *v.end() << endl;  // 未定义行为！end() 指向最后一个元素的后面
>
> // 正确做法：
> auto it = v.end();
> it--;           // 先后退一步，指向最后一个元素
> cout << *it << endl;  // 3
> ```


### 4. 仿函数（Functor）

仿函数就是**重载了 () 运算符的类**，让对象可以像函数一样被调用。

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>  // 内置仿函数头文件
using namespace std;

// 自定义仿函数：用于排序时按从大到小排列
class MyCompare
{
public:
    // 重载 () 运算符
    bool operator()(int a, int b)
    {
        return a > b;  // 从大到小
    }
};

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // 使用自定义仿函数进行排序
    sort(v.begin(), v.end(), MyCompare());

    cout << "从大到小排序: ";
    for (auto val : v)
    {
        cout << val << " ";  // 5 4 3 2 1
    }
    cout << endl;

    // 使用 STL 内置的仿函数 greater<int>
    vector<int> v2 = {5, 3, 1, 4, 2};
    sort(v2.begin(), v2.end(), greater<int>());

    cout << "greater 排序: ";
    for (auto val : v2)
    {
        cout << val << " ";  // 5 4 3 2 1
    }
    cout << endl;

    return 0;
}
```


### 5. 适配器（Adapter）

适配器可以将一种接口**转换**成另一种接口。

```cpp
#include <iostream>
#include <stack>
#include <queue>
#include <deque>
using namespace std;

int main()
{
    // stack 和 queue 本身就是容器适配器
    // 默认情况下，它们底层使用 deque 实现

    // 指定底层容器为 vector
    stack<int, vector<int>> stk;
    stk.push(10);
    stk.push(20);
    cout << "栈顶: " << stk.top() << endl;  // 20

    // 指定底层容器为 list
    queue<int, list<int>> que;
    que.push(100);
    que.push(200);
    cout << "队头: " << que.front() << endl;  // 100

    return 0;
}
```


### 6. 空间配置器（Allocator）

空间配置器负责**内存的分配和释放**，一般我们不需要直接使用它。
STL 容器在内部会自动使用空间配置器来管理内存。

```cpp
#include <iostream>
#include <memory>  // 包含 allocator
using namespace std;

int main()
{
    // 创建一个 int 类型的空间配置器
    allocator<int> alloc;

    // 分配 5 个 int 的空间
    int* p = alloc.allocate(5);

    // 构造对象（在已分配的内存上构造值）
    for (int i = 0; i < 5; i++)
    {
        alloc.construct(p + i, (i + 1) * 10);
    }

    // 使用
    for (int i = 0; i < 5; i++)
    {
        cout << p[i] << " ";  // 10 20 30 40 50
    }
    cout << endl;

    // 销毁对象
    for (int i = 0; i < 5; i++)
    {
        alloc.destroy(p + i);
    }

    // 释放内存
    alloc.deallocate(p, 5);

    return 0;
}
```


---

## 三、容器的分类

### 1. 序列式容器

元素**按插入顺序**排列，不会自动排序。

| 容器 | 特点 | 底层结构 |
|------|------|---------|
| array | 固定大小数组 | 静态数组 |
| vector | 动态数组，尾部增删快 | 动态数组 |
| deque | 双端队列，两端增删快 | 分段连续空间 |
| list | 双向链表，任意位置增删快 | 双向链表 |
| forward_list | 单向链表 | 单向链表 |

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>
using namespace std;

int main()
{
    // vector：尾部操作效率高
    vector<int> v;
    v.push_back(1);  // 尾部插入 O(1)
    v.push_back(2);
    v.push_back(3);
    // v.push_front(0);  // vector 没有 push_front！头部插入效率低

    // deque：两端操作都高效
    deque<int> d;
    d.push_back(1);    // 尾部插入
    d.push_front(0);   // 头部插入（vector 没有这个功能）
    d.push_back(2);

    // list：任意位置操作都高效
    list<int> lst;
    lst.push_back(1);
    lst.push_front(0);
    auto it = lst.begin();
    it++;
    lst.insert(it, 99);  // 在第 2 个位置插入 99

    // 输出各容器内容
    cout << "vector: ";
    for (auto val : v) cout << val << " ";     // 1 2 3
    cout << endl;

    cout << "deque:  ";
    for (auto val : d) cout << val << " ";     // 0 1 2
    cout << endl;

    cout << "list:   ";
    for (auto val : lst) cout << val << " ";   // 0 99 1
    cout << endl;

    return 0;
}
```

### 2. 关联式容器

元素**自动排序**（默认从小到大），查找效率高（O(log n)）。

```cpp
#include <iostream>
#include <set>
#include <map>
using namespace std;

int main()
{
    // set：集合，元素不重复，自动排序
    set<int> s;
    s.insert(30);
    s.insert(10);
    s.insert(50);
    s.insert(20);
    s.insert(10);  // 重复，不会被插入

    cout << "set 元素（自动排序）: ";
    for (auto val : s)
    {
        cout << val << " ";  // 10 20 30 50
    }
    cout << endl;

    // multiset：允许重复元素
    multiset<int> ms;
    ms.insert(10);
    ms.insert(10);  // 允许重复
    ms.insert(20);
    cout << "multiset 元素个数: " << ms.count(10) << endl;  // 2

    // map：键值对，按键自动排序
    map<int, string> m;
    m[3] = "张三";
    m[1] = "李四";
    m[2] = "王五";

    cout << "map 按键排序:" << endl;
    for (auto& pair : m)
    {
        cout << "  键: " << pair.first << ", 值: " << pair.second << endl;
    }
    // 输出：键1 李四, 键2 王五, 键3 张三

    return 0;
}
```

### 3. 无序容器

元素**不排序**，使用哈希表实现，查找效率极高（接近 O(1)）。

```cpp
#include <iostream>
#include <unordered_set>
#include <unordered_map>
using namespace std;

int main()
{
    // unordered_set：无序集合
    unordered_set<int> us;
    us.insert(30);
    us.insert(10);
    us.insert(50);
    us.insert(20);

    cout << "unordered_set: ";
    for (auto val : us)
    {
        cout << val << " ";  // 顺序不确定
    }
    cout << endl;

    // unordered_map：无序映射
    unordered_map<string, int> um;
    um["apple"] = 5;
    um["banana"] = 3;
    um["cherry"] = 8;

    // 查找效率非常高
    if (um.find("banana") != um.end())
    {
        cout << "找到 banana, 值为: " << um["banana"] << endl;
    }

    return 0;
}
```


---

## 四、算法分类

### 1. 质变算法

会**改变**容器中元素的值或顺序。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // sort：排序（改变了顺序）
    sort(v.begin(), v.end());

    // reverse：反转（改变了顺序）
    reverse(v.begin(), v.end());

    // replace：替换（改变了值）
    replace(v.begin(), v.end(), 5, 50);

    // fill：填充（改变了值）
    fill(v.begin(), v.begin() + 2, 0);  // 前 2 个元素填充为 0

    for (auto val : v)
    {
        cout << val << " ";  // 0 0 3 4 50
    }
    cout << endl;

    return 0;
}
```

### 2. 非质变算法

**不会改变**容器中元素的值或顺序。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5, 3, 7};

    // find：查找（不改变内容）
    auto it = find(v.begin(), v.end(), 3);
    if (it != v.end())
    {
        cout << "找到 3, 位置: " << distance(v.begin(), it) << endl;  // 2
    }

    // count：计数（不改变内容）
    int n = count(v.begin(), v.end(), 3);
    cout << "3 出现了 " << n << " 次" << endl;  // 2

    // max_element / min_element（不改变内容）
    auto maxIt = max_element(v.begin(), v.end());
    auto minIt = min_element(v.begin(), v.end());
    cout << "最大值: " << *maxIt << endl;  // 7
    cout << "最小值: " << *minIt << endl;  // 1

    // accumulate：求和（在 numeric 头文件中）
    // #include <numeric>
    // int sum = accumulate(v.begin(), v.end(), 0);

    return 0;
}
```


---

## 五、常见错误和注意事项

### 注意事项 1：迭代器失效

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 错误写法：在遍历过程中删除元素，迭代器会失效
    // for (auto it = v.begin(); it != v.end(); it++)
    // {
    //     if (*it == 3)
    //         v.erase(it);  // 危险！erase 后 it 失效
    // }

    // 正确写法：使用 erase 的返回值（返回下一个有效迭代器）
    for (auto it = v.begin(); it != v.end(); )
    {
        if (*it == 3)
        {
            it = v.erase(it);  // erase 返回下一个位置的迭代器
        }
        else
        {
            it++;
        }
    }

    for (auto val : v)
    {
        cout << val << " ";  // 1 2 4 5（3 被删除了）
    }
    cout << endl;

    return 0;
}
```

### 注意事项 2：头文件要包含对

```cpp
// 必须包含对应的头文件！
#include <vector>      // vector
#include <deque>       // deque
#include <list>        // list
#include <stack>       // stack
#include <queue>       // queue
#include <set>         // set / multiset
#include <map>         // map / multimap
#include <algorithm>   // sort, find, reverse 等通用算法
#include <functional>  // greater, less 等仿函数
#include <numeric>     // accumulate 等数值算法
```

### 注意事项 3：容器的选择

```
需要频繁随机访问元素  →  vector
需要在两端插入删除    →  deque
需要在中间插入删除    →  list
需要自动排序去重      →  set
需要键值对映射        →  map
需要先进后出          →  stack
需要先进先出          →  queue
需要最快的查找        →  unordered_map / unordered_set
```


---

## 六、综合练习

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    // 练习：使用 STL 对一组学生成绩进行处理
    vector<int> scores = {85, 92, 78, 95, 88, 76, 90, 83};

    // 1. 排序（从高到低）
    sort(scores.begin(), scores.end(), greater<int>());

    cout << "成绩排名（从高到低）: ";
    for (auto score : scores)
    {
        cout << score << " ";
    }
    cout << endl;
    // 输出：95 92 90 88 85 83 78 76

    // 2. 查找是否有 90 分以上的学生
    auto it = find_if(scores.begin(), scores.end(),
                      [](int s) { return s >= 90; });
    if (it != scores.end())
    {
        cout << "有 90 分以上的成绩" << endl;
    }

    // 3. 统计 80 分以上的人数
    int cnt = count_if(scores.begin(), scores.end(),
                       [](int s) { return s >= 80; });
    cout << "80 分以上: " << cnt << " 人" << endl;  // 6

    // 4. 计算平均分
    int sum = 0;
    for (auto score : scores)
    {
        sum += score;
    }
    cout << "平均分: " << (double)sum / scores.size() << endl;

    // 5. 找最高分和最低分
    cout << "最高分: " << *max_element(scores.begin(), scores.end()) << endl;
    cout << "最低分: " << *min_element(scores.begin(), scores.end()) << endl;

    return 0;
}
```

---

## 总结

| 组件 | 作用 | 是否常用 |
|------|------|---------|
| 容器 | 存储数据 | 非常常用 |
| 算法 | 处理数据（排序、查找等） | 非常常用 |
| 迭代器 | 容器和算法的桥梁 | 非常常用 |
| 仿函数 | 自定义比较规则等 | 较常用 |
| 适配器 | 转换接口（stack、queue） | 常用 |
| 空间配置器 | 管理内存 | 很少直接使用 |

**学习建议：** 先掌握容器 + 算法 + 迭代器（核心三件套），再逐步学习仿函数和适配器。
