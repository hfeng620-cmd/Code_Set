# STL 查找算法

## 一、概述

查找算法用于在容器中搜索特定元素或满足条件的元素。

| 算法 | 作用 | 返回值 |
|------|------|--------|
| `find` | 查找指定值的元素 | 迭代器 |
| `find_if` | 查找满足条件的第一个元素 | 迭代器 |
| `binary_search` | 二分查找指定值 | `bool` |
| `count` | 统计指定值出现次数 | `int` |
| `count_if` | 统计满足条件的元素个数 | `int` |
| `adjacent_find` | 查找相邻重复元素 | 迭代器 |

**头文件：** `#include <algorithm>`

---

## 二、find 算法

### 2.1 基本语法

```cpp
find(begin, end, value);
```

- `begin, end`：容器的起始和结束迭代器
- `value`：要查找的目标值
- **返回值：** 指向找到元素的迭代器，找不到则返回 `end`

---

### 2.2 基本类型查找

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {10, 20, 30, 40, 50};

    // 查找值为 30 的元素
    vector<int>::iterator it = find(v.begin(), v.end(), 30);

    if (it != v.end())
    {
        cout << "找到了！值为: " << *it << endl;      // 输出：找到了！值为: 30
        cout << "位置索引: " << it - v.begin() << endl; // 输出：位置索引: 2
    }
    else
    {
        cout << "没有找到" << endl;
    }

    // 查找一个不存在的值
    it = find(v.begin(), v.end(), 99);
    if (it != v.end())
    {
        cout << "找到了" << endl;
    }
    else
    {
        cout << "没有找到 99" << endl;  // 输出：没有找到 99
    }

    return 0;
}
```

**关键点：**
- `find` 返回的是迭代器，不是索引
- 判断是否找到：`it != v.end()`
- 如果找到，`*it` 就是找到的值，`it - v.begin()` 就是索引

---

### 2.3 自定义类型查找（需要重载 == 运算符）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}

    // 【重要】自定义类型必须重载 == 运算符，find 才能比较
    bool operator==(const Person& other) const
    {
        // 这里定义"相等"的含义：姓名和年龄都相同
        return this->name == other.name && this->age == other.age;
    }
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 20));
    people.push_back(Person("李四", 25));
    people.push_back(Person("王五", 30));

    // 查找 "李四"
    Person target("李四", 25);
    vector<Person>::iterator it = find(people.begin(), people.end(), target);

    if (it != people.end())
    {
        cout << "找到了: " << it->name << ", 年龄: " << it->age << endl;
        // 输出：找到了: 李四, 年龄: 25
    }
    else
    {
        cout << "没有找到" << endl;
    }

    // 查找一个不存在的人
    Person target2("赵六", 35);
    it = find(people.begin(), people.end(), target2);
    if (it != people.end())
    {
        cout << "找到了" << endl;
    }
    else
    {
        cout << "没有找到赵六" << endl;  // 输出：没有找到赵六
    }

    return 0;
}
```

**常见错误：自定义类型忘记重载 == 运算符**

```cpp
class Person
{
public:
    string name;
    int age;
    // 忘记重载 == 运算符
};

// 编译报错！find 内部需要用 == 来比较元素
vector<Person>::iterator it = find(v.begin(), v.end(), target);
```

---

## 三、find_if 算法

### 3.1 基本语法

```cpp
find_if(begin, end, 谓词);
```

- `谓词（Predicate）`：一个返回 `bool` 的函数/仿函数/Lambda
- 返回第一个满足条件的元素的迭代器，找不到返回 `end`

**什么是谓词？**
- 谓词就是一个返回 `bool` 类型的可调用对象
- **一元谓词**：只有一个参数（用于 `find_if`、`count_if` 等）
- **二元谓词**：有两个参数（用于 `sort` 等）

---

### 3.2 用普通函数作为谓词

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 一元谓词：判断是否大于30
bool greaterThan30(int val)
{
    return val > 30;
}

int main()
{
    vector<int> v = {10, 20, 35, 40, 50};

    // 查找第一个大于30的元素
    vector<int>::iterator it = find_if(v.begin(), v.end(), greaterThan30);

    if (it != v.end())
    {
        cout << "第一个大于30的元素: " << *it << endl;
        // 输出：第一个大于30的元素: 35
    }

    return 0;
}
```

---

### 3.3 用仿函数作为谓词

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 仿函数：查找大于指定值的元素
class GreaterThan
{
public:
    int threshold;  // 阈值

    GreaterThan(int t) : threshold(t) {}

    // 重载 () 运算符
    bool operator()(int val)
    {
        return val > threshold;
    }
};

int main()
{
    vector<int> v = {10, 20, 35, 40, 50};

    // 查找第一个大于25的元素
    vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterThan(25));

    if (it != v.end())
    {
        cout << "第一个大于25的元素: " << *it << endl;
        // 输出：第一个大于25的元素: 35
    }

    return 0;
}
```

---

### 3.4 用 Lambda 表达式作为谓词

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {15, 8, 30, 42, 7, 55};

    // 用 Lambda 查找第一个偶数
    vector<int>::iterator it = find_if(v.begin(), v.end(), [](int val) {
        return val % 2 == 0;  // 是否为偶数
    });

    if (it != v.end())
    {
        cout << "第一个偶数: " << *it << endl;
        // 输出：第一个偶数: 8
    }

    return 0;
}
```

---

### 3.5 查找自定义类型

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 20));
    people.push_back(Person("李四", 25));
    people.push_back(Person("王五", 35));
    people.push_back(Person("赵六", 40));

    // 查找第一个年龄大于30的人（用 Lambda）
    vector<Person>::iterator it = find_if(people.begin(), people.end(), [](const Person& p) {
        return p.age > 30;
    });

    if (it != people.end())
    {
        cout << "找到了: " << it->name << ", 年龄: " << it->age << endl;
        // 输出：找到了: 王五, 年龄: 35
    }

    return 0;
}
```

---

## 四、binary_search 算法

### 4.1 基本语法

```cpp
binary_search(begin, end, value);
```

- 返回 `bool`：找到返回 `true`，找不到返回 `false`
- **【非常重要】容器必须有序！默认按升序排列**
- 如果容器无序，结果是未定义的（可能返回错误结果）

---

### 4.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 3, 5, 7, 9, 11, 13};

    // 容器已经是有序的，可以直接使用 binary_search
    bool found = binary_search(v.begin(), v.end(), 7);
    if (found)
    {
        cout << "找到了 7" << endl;  // 输出：找到了 7
    }

    found = binary_search(v.begin(), v.end(), 6);
    if (!found)
    {
        cout << "没有找到 6" << endl;  // 输出：没有找到 6
    }

    return 0;
}
```

**常见错误：容器未排序就使用 binary_search**

```cpp
vector<int> v = {5, 3, 1, 4, 2};  // 未排序！

// 错误！结果未定义，可能返回 true 也可能返回 false
bool found = binary_search(v.begin(), v.end(), 3);

// 正确做法：先排序
sort(v.begin(), v.end());
found = binary_search(v.begin(), v.end(), 3);  // 现在结果是正确的
```

---

### 4.3 降序排列的二分查找

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {13, 11, 9, 7, 5, 3, 1};  // 降序排列

    // 降序排列需要指定比较函数
    bool found = binary_search(v.begin(), v.end(), 7, greater<int>());
    if (found)
    {
        cout << "找到了 7" << endl;  // 输出：找到了 7
    }

    return 0;
}
```

---

### 4.4 binary_search 的局限性

**注意：binary_search 只告诉你找没找到，不告诉你位置！**

```cpp
// 如果需要知道位置，用 lower_bound 或 upper_bound
vector<int> v = {1, 3, 5, 7, 9};

// lower_bound 返回第一个 >= value 的元素的迭代器
auto it = lower_bound(v.begin(), v.end(), 5);
if (it != v.end())
{
    cout << "位置: " << it - v.begin() << endl;  // 输出：位置: 2
}
```

---

## 五、count 算法

### 5.1 基本语法

```cpp
count(begin, end, value);
```

- 返回 `value` 在范围内出现的次数
- 返回类型是 `int`（实际上是迭代器的差值类型）

---

### 5.2 基本类型统计

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    // 统计 2 出现了多少次
    int num = count(v.begin(), v.end(), 2);
    cout << "2 出现了 " << num << " 次" << endl;  // 输出：2 出现了 3 次

    // 统计 99 出现了多少次
    num = count(v.begin(), v.end(), 99);
    cout << "99 出现了 " << num << " 次" << endl;  // 输出：99 出现了 0 次

    return 0;
}
```

---

### 5.3 自定义类型统计（需要重载 == 运算符）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}

    // 【重要】count 也需要重载 == 运算符
    bool operator==(const Person& other) const
    {
        return this->name == other.name && this->age == other.age;
    }
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 20));
    people.push_back(Person("李四", 25));
    people.push_back(Person("张三", 20));  // 重复
    people.push_back(Person("王五", 30));
    people.push_back(Person("张三", 20));  // 重复

    Person target("张三", 20);
    int num = count(people.begin(), people.end(), target);
    cout << "张三(20岁) 出现了 " << num << " 次" << endl;
    // 输出：张三(20岁) 出现了 3 次

    return 0;
}
```

---

## 六、count_if 算法

### 6.1 基本语法

```cpp
count_if(begin, end, 谓词);
```

- 返回满足条件的元素个数
- 谓词：返回 `bool` 的一元谓词

---

### 6.2 统计大于某值的元素个数

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {10, 25, 30, 45, 50, 15, 60};

    // 统计大于30的元素个数
    int num = count_if(v.begin(), v.end(), [](int val) {
        return val > 30;
    });
    cout << "大于30的元素有 " << num << " 个" << endl;
    // 输出：大于30的元素有 3 个

    // 统计偶数的个数
    num = count_if(v.begin(), v.end(), [](int val) {
        return val % 2 == 0;
    });
    cout << "偶数有 " << num << " 个" << endl;
    // 输出：偶数有 4 个

    return 0;
}
```

---

### 6.3 统计自定义类型

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 15));
    people.push_back(Person("李四", 22));
    people.push_back(Person("王五", 17));
    people.push_back(Person("赵六", 25));
    people.push_back(Person("钱七", 18));

    // 统计成年人（年龄>=18）的数量
    int num = count_if(people.begin(), people.end(), [](const Person& p) {
        return p.age >= 18;
    });
    cout << "成年人有 " << num << " 个" << endl;
    // 输出：成年人有 3 个

    return 0;
}
```

---

## 七、adjacent_find 算法

### 7.1 基本语法

```cpp
adjacent_find(begin, end);
```

- 查找第一对**相邻且重复**的元素
- 返回这对元素中第一个元素的迭代器
- 找不到返回 `end`

---

### 7.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 3, 4, 5, 5, 6};

    // 查找第一对相邻重复元素
    vector<int>::iterator it = adjacent_find(v.begin(), v.end());

    if (it != v.end())
    {
        cout << "找到相邻重复元素: " << *it << endl;
        // 输出：找到相邻重复元素: 3
        cout << "位置索引: " << it - v.begin() << endl;
        // 输出：位置索引: 2
    }

    return 0;
}
```

**注意：** `adjacent_find` 只找相邻的重复元素，不相邻的重复不算：

```cpp
vector<int> v = {1, 2, 3, 2, 4};  // 2 重复了但不相邻
vector<int>::iterator it = adjacent_find(v.begin(), v.end());
// it == v.end()，没有找到相邻重复元素
```

---

### 7.3 自定义类型的相邻查找

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Person
{
public:
    string name;
    int age;

    Person(string n, int a) : name(n), age(a) {}

    // 需要重载 == 运算符
    bool operator==(const Person& other) const
    {
        return this->name == other.name && this->age == other.age;
    }
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 20));
    people.push_back(Person("李四", 25));
    people.push_back(Person("李四", 25));  // 相邻重复
    people.push_back(Person("王五", 30));

    vector<Person>::iterator it = adjacent_find(people.begin(), people.end());
    if (it != people.end())
    {
        cout << "找到相邻重复: " << it->name << endl;
        // 输出：找到相邻重复: 李四
    }

    return 0;
}
```

---

## 八、综合对比表

| 算法 | 返回值 | 需要有序 | 需要重载== | 用途 |
|------|--------|----------|-----------|------|
| `find` | 迭代器 | 不需要 | 自定义类型需要 | 查找指定值 |
| `find_if` | 迭代器 | 不需要 | 不需要 | 按条件查找 |
| `binary_search` | `bool` | **必须有序** | 不需要 | 快速判断是否存在 |
| `count` | `int` | 不需要 | 自定义类型需要 | 统计出现次数 |
| `count_if` | `int` | 不需要 | 不需要 | 按条件统计个数 |
| `adjacent_find` | 迭代器 | 不需要 | 自定义类型需要 | 查找相邻重复 |

---

## 九、注意事项总结

1. **所有查找算法的结束条件都是 `end()` 迭代器**，找不到时返回 `end()`
2. **自定义类型使用 `find`、`count`、`adjacent_find` 时必须重载 `==` 运算符**
3. **`binary_search` 容器必须有序**，否则结果不可靠
4. **`binary_search` 只返回 `bool`**，不返回位置；需要位置用 `lower_bound`
5. **`find_if` 和 `count_if` 使用谓词**，不需要重载 `==`，更灵活
6. **`adjacent_find` 只查找相邻的重复元素**，不相邻的不算
7. **`find` 和 `find_if` 找到第一个就停止**，不会继续往后找
8. **头文件：`#include <algorithm>`**
