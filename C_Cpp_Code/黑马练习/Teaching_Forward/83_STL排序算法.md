# STL 排序算法

## 一、概述

排序算法是最常用的 STL 算法之一。

| 算法 | 作用 | 特点 |
|------|------|------|
| `sort` | 排序 | 最常用，快速排序（introsort），不稳定 |
| `stable_sort` | 稳定排序 | 相等元素保持原有顺序，归并排序 |
| `partial_sort` | 部分排序 | 只排序前 N 个最小元素 |
| `random_shuffle` | 随机打乱 | 随机重排元素顺序（C++17 被 `shuffle` 替代） |
| `merge` | 合并 | 将两个有序容器合并到另一个容器 |
| `reverse` | 反转 | 反转容器中元素的顺序 |

**头文件：** `#include <algorithm>`

---

## 二、sort 算法（最常用）

### 2.1 基本语法

```cpp
sort(begin, end);              // 默认升序
sort(begin, end, 比较函数);     // 自定义排序规则
```

- `begin, end`：容器的起始和结束迭代器
- 排序范围：`[begin, end)`，包含 begin，不包含 end
- **底层实现：** introsort（内省排序）= 快速排序 + 堆排序 + 插入排序的混合
- **平均时间复杂度：** O(n log n)

---

### 2.2 默认升序排序

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // 默认升序排序
    sort(v.begin(), v.end());

    // 打印结果
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 2 3 4 5

    cout << endl;
    return 0;
}
```

---

### 2.3 降序排序 —— 使用 greater<int>()

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // greater<int>() 是 STL 提供的函数对象，用于降序排序
    sort(v.begin(), v.end(), greater<int>());

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：5 4 3 2 1

    cout << endl;
    return 0;
}
```

**注意：** `greater<int>()` 中的 `()` 不能省略，因为它是一个函数对象（仿函数）的实例化。

---

### 2.4 自定义排序 —— 使用仿函数

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 仿函数：自定义降序排序
class MyCompare
{
public:
    // 二元谓词：接收两个参数，返回 bool
    // 返回 true 表示 a 应该排在 b 前面
    bool operator()(int a, int b)
    {
        return a > b;  // 降序：大的在前面
    }
};

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    sort(v.begin(), v.end(), MyCompare());

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：5 4 3 2 1

    cout << endl;
    return 0;
}
```

---

### 2.5 Lambda 表达式排序（C++11）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // Lambda 降序排序
    // [](int a, int b) { return a > b; }
    // 返回 true 表示 a 排在 b 前面
    sort(v.begin(), v.end(), [](int a, int b) {
        return a > b;
    });

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：5 4 3 2 1

    cout << endl;
    return 0;
}
```

---

### 2.6 自定义类型排序

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
    int score;

    Person(string n, int a, int s) : name(n), age(a), score(s) {}
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 20, 85));
    people.push_back(Person("李四", 25, 90));
    people.push_back(Person("王五", 18, 78));
    people.push_back(Person("赵六", 22, 92));

    // 按成绩降序排序
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.score > b.score;
    });

    cout << "按成绩降序:" << endl;
    for_each(people.begin(), people.end(), [](const Person& p) {
        cout << p.name << " - " << p.score << endl;
    });
    // 输出：
    // 赵六 - 92
    // 李四 - 90
    // 张三 - 85
    // 王五 - 78

    // 按年龄升序排序
    sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;
    });

    cout << "\n按年龄升序:" << endl;
    for_each(people.begin(), people.end(), [](const Person& p) {
        cout << p.name << " - " << p.age << endl;
    });
    // 输出：
    // 王五 - 18
    // 张三 - 20
    // 赵六 - 22
    // 李四 - 25

    return 0;
}
```

---

### 2.7 按字符串长度排序

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    vector<string> v = {"apple", "hi", "banana", "ok", "watermelon"};

    // 按字符串长度升序排序
    sort(v.begin(), v.end(), [](const string& a, const string& b) {
        return a.size() < b.size();
    });

    for_each(v.begin(), v.end(), [](const string& s) {
        cout << s << "(" << s.size() << ") ";
    });
    // 输出：hi(2) ok(2) apple(5) banana(6) watermelon(10)

    cout << endl;
    return 0;
}
```

---

## 三、stable_sort 算法

### 3.1 什么是稳定排序？

**稳定排序**保证：如果两个元素"相等"（按排序标准），排序后它们的相对位置不变。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Student
{
public:
    string name;
    int score;

    Student(string n, int s) : name(n), score(s) {}
};

int main()
{
    vector<Student> students = {
        Student("张三", 85),
        Student("李四", 90),
        Student("王五", 85),  // 和张三成绩相同
        Student("赵六", 90)   // 和李四成绩相同
    };

    // stable_sort 保证相等元素的原有顺序
    stable_sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.score > b.score;  // 按成绩降序
    });

    for_each(students.begin(), students.end(), [](const Student& s) {
        cout << s.name << " - " << s.score << endl;
    });
    // 输出：
    // 李四 - 90  （李四在赵六前面，保持原有顺序）
    // 赵六 - 90
    // 张三 - 85  （张三在王五前面，保持原有顺序）
    // 王五 - 85

    return 0;
}
```

**sort 与 stable_sort 的区别：**
- `sort`：不稳定排序，相等元素的顺序可能改变，速度更快
- `stable_sort`：稳定排序，相等元素保持原有顺序，速度稍慢
- **需要稳定排序的场景：** 当排序依据只是数据的一部分，且你希望保持原始顺序时

---

## 四、partial_sort 算法

### 4.1 基本语法

```cpp
partial_sort(begin, middle, end);
```

- 排序后，`[begin, middle)` 范围内的元素是整个 `[begin, end)` 范围内最小的 N 个元素（已排序）
- `[middle, end)` 范围内的元素顺序不确定

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {50, 10, 40, 20, 30, 5, 15};

    // 只排序前3个最小的元素
    partial_sort(v.begin(), v.begin() + 3, v.end());

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：5 10 15 50 40 20 30
    //       |--排序--| |--未确定--|

    cout << endl;
    return 0;
}
```

**使用场景：** 只需要前 N 个最小/最大的元素时，partial_sort 比 sort 更高效。

---

## 五、random_shuffle 算法

### 5.1 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>    // time()
#include <cstdlib>  // srand()
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 设置随机数种子，否则每次打乱的结果相同
    srand((unsigned int)time(NULL));

    // 随机打乱顺序
    random_shuffle(v.begin(), v.end());

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 每次运行结果不同，例如：3 7 1 9 5 2 8 4 10 6

    cout << endl;
    return 0;
}
```

**注意事项：**
- **必须设置随机数种子** `srand()`，否则每次运行结果相同
- `random_shuffle` 在 C++14 中被弃用，C++17 中被移除
- 替代方案是 `shuffle`，使用更好的随机数引擎：

```cpp
#include <random>
#include <chrono>

// C++17 推荐用法
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
shuffle(v.begin(), v.end(), default_random_engine(seed));
```

---

## 六、merge 算法

### 6.1 基本语法

```cpp
merge(begin1, end1, begin2, end2, dst_begin);
```

- 将两个**有序**容器合并到一个目标容器
- 两个源容器**必须有序**（排序方式要一致）
- 目标容器**必须提前开辟空间**

---

### 6.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 3, 5, 7, 9};     // 必须有序
    vector<int> v2 = {2, 4, 6, 8, 10};    // 必须有序
    vector<int> dst;

    // 【重要】目标容器必须提前开辟空间
    // 大小至少为两个源容器大小之和
    dst.resize(v1.size() + v2.size());

    // 合并两个有序容器
    merge(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());

    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 2 3 4 5 6 7 8 9 10

    cout << endl;
    return 0;
}
```

---

### 6.3 常见错误

```cpp
// 错误1：源容器未排序
vector<int> v1 = {5, 3, 1};      // 未排序！
vector<int> v2 = {2, 4, 6};
// merge(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());
// 结果未定义！

// 错误2：目标容器未分配空间
vector<int> dst;  // 空的！
// merge(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());
// 崩溃！写入了未分配的内存

// 正确做法
dst.resize(v1.size() + v2.size());
merge(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());
```

---

## 七、reverse 算法

### 7.1 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 反转容器中的元素
    reverse(v.begin(), v.end());

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：5 4 3 2 1

    cout << endl;
    return 0;
}
```

---

### 7.2 反转字符串

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    string s = "Hello World";

    // 反转字符串
    reverse(s.begin(), s.end());

    cout << s << endl;  // 输出：dlroW olleH

    return 0;
}
```

---

### 7.3 部分反转

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 只反转前3个元素
    reverse(v.begin(), v.begin() + 3);

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：3 2 1 4 5

    cout << endl;
    return 0;
}
```

---

## 八、综合示例

### 示例1：学生成绩排名

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

class Student
{
public:
    string name;
    int score;

    Student(string n, int s) : name(n), score(s) {}
};

int main()
{
    vector<Student> students = {
        Student("张三", 85),
        Student("李四", 92),
        Student("王五", 78),
        Student("赵六", 95),
        Student("钱七", 88)
    };

    // 按成绩降序排序
    sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.score > b.score;
    });

    // 打印排名
    cout << "=== 成绩排名 ===" << endl;
    int rank = 1;
    for_each(students.begin(), students.end(), [&rank](const Student& s) {
        cout << "第" << rank++ << "名: " << s.name << " - " << s.score << "分" << endl;
    });

    return 0;
}
```

**输出：**
```
=== 成绩排名 ===
第1名: 赵六 - 95分
第2名: 李四 - 92分
第3名: 钱七 - 88分
第4名: 张三 - 85分
第5名: 王五 - 78分
```

---

## 九、注意事项总结

1. **`sort` 是最常用的排序算法**，平均时间复杂度 O(n log n)
2. **降序排序：** `sort(begin, end, greater<int>())`
3. **自定义排序规则：** 使用仿函数或 Lambda 表达式
4. **`stable_sort` 保持相等元素的原有顺序**，需要时使用
5. **`merge` 要求两个源容器必须有序**，目标容器必须提前分配空间
6. **`random_shuffle` 需要 `srand()` 设置随机种子**，C++17 中已被 `shuffle` 替代
7. **`reverse` 可以反转任意范围的元素**，包括字符串
8. **排序比较函数的返回值含义：** `return a < b` 表示升序（a 排在 b 前面），`return a > b` 表示降序
9. **头文件：`#include <algorithm>`**
