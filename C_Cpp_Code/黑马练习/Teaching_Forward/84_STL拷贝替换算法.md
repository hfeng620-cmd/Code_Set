# STL 拷贝替换算法

## 一、概述

拷贝和替换算法用于在容器间复制元素或替换容器中的元素。

| 算法 | 作用 |
|------|------|
| `copy` | 将容器中的元素拷贝到另一个容器 |
| `replace` | 将指定值替换为新值 |
| `replace_if` | 将满足条件的元素替换为新值 |
| `swap` | 交换两个容器（或变量）的内容 |
| `fill` | 用指定值填充容器 |

**头文件：** `#include <algorithm>`

---

## 二、copy 算法

### 2.1 基本语法

```cpp
copy(src_begin, src_end, dst_begin);
```

- `src_begin, src_end`：源容器的起始和结束迭代器
- `dst_begin`：目标容器的起始迭代器
- **目标容器必须提前开辟空间！**

---

### 2.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst;

    // 【重要】目标容器必须提前开辟空间
    dst.resize(src.size());

    // 将 src 的元素拷贝到 dst
    copy(src.begin(), src.end(), dst.begin());

    // 验证拷贝结果
    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 2 3 4 5

    cout << endl;
    return 0;
}
```

---

### 2.3 常见错误：忘记分配空间

```cpp
vector<int> src = {1, 2, 3, 4, 5};
vector<int> dst;  // 空容器！

// 错误！dst 没有空间，写入会导致未定义行为（通常会崩溃）
copy(src.begin(), src.end(), dst.begin());

// 正确做法：
dst.resize(src.size());  // 先分配空间
copy(src.begin(), src.end(), dst.begin());
```

---

### 2.4 部分拷贝

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> src = {1, 2, 3, 4, 5};
    vector<int> dst;
    dst.resize(3);  // 只拷贝3个元素

    // 只拷贝前3个元素
    copy(src.begin(), src.begin() + 3, dst.begin());

    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 2 3

    cout << endl;
    return 0;
}
```

---

### 2.5 拷贝到输出流（用于调试打印）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 将元素拷贝到输出流（直接打印）
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
    // 输出：1 2 3 4 5

    cout << endl;
    return 0;
}
```

---

### 2.6 不同容器类型之间的拷贝

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

int main()
{
    list<int> src = {10, 20, 30, 40, 50};
    vector<int> dst;
    dst.resize(src.size());

    // 从 list 拷贝到 vector
    copy(src.begin(), src.end(), dst.begin());

    for_each(dst.begin(), dst.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：10 20 30 40 50

    cout << endl;
    return 0;
}
```

---

## 三、replace 算法

### 3.1 基本语法

```cpp
replace(begin, end, old_value, new_value);
```

- `begin, end`：容器的起始和结束迭代器
- `old_value`：要被替换的旧值
- `new_value`：替换后的新值
- **功能：** 将范围内所有等于 `old_value` 的元素替换为 `new_value`

---

### 3.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    cout << "替换前: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：替换前: 1 2 3 2 4 2 5

    // 将所有 2 替换为 20
    replace(v.begin(), v.end(), 2, 20);

    cout << "替换后: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：替换后: 1 20 3 20 4 20 5

    return 0;
}
```

---

### 3.3 替换字符串

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    vector<string> v = {"hello", "world", "hello", "cpp", "hello"};

    // 将所有 "hello" 替换为 "HI"
    replace(v.begin(), v.end(), string("hello"), string("HI"));

    for_each(v.begin(), v.end(), [](const string& s) {
        cout << s << " ";
    });
    // 输出：HI world HI cpp HI

    cout << endl;
    return 0;
}
```

---

## 四、replace_if 算法

### 4.1 基本语法

```cpp
replace_if(begin, end, 谓词, new_value);
```

- `谓词`：一元谓词，返回 `bool`
- `new_value`：替换后的新值
- **功能：** 将范围内所有满足谓词条件的元素替换为 `new_value`

---

### 4.2 将所有偶数替换为0

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    cout << "替换前: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：替换前: 1 2 3 4 5 6 7 8

    // 将所有偶数替换为 0
    replace_if(v.begin(), v.end(), [](int val) {
        return val % 2 == 0;  // 是偶数就返回 true
    }, 0);

    cout << "替换后: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：替换后: 1 0 3 0 5 0 7 0

    return 0;
}
```

---

### 4.3 将大于某值的元素替换

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {10, 55, 30, 80, 45, 90, 20};

    cout << "替换前: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;

    // 将所有大于60的元素替换为60
    replace_if(v.begin(), v.end(), [](int val) {
        return val > 60;
    }, 60);

    cout << "替换后: ";
    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：替换后: 10 55 30 60 45 60 20

    return 0;
}
```

---

### 4.4 替换自定义类型

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

    bool operator==(const Person& other) const
    {
        return this->name == other.name && this->age == other.age;
    }
};

int main()
{
    vector<Person> people;
    people.push_back(Person("张三", 15));
    people.push_back(Person("李四", 22));
    people.push_back(Person("王五", 17));
    people.push_back(Person("赵六", 25));

    // 将所有未成年人（年龄<18）替换为 "未成年"
    Person replacement("未成年", 0);
    replace_if(people.begin(), people.end(), [](const Person& p) {
        return p.age < 18;
    }, replacement);

    for_each(people.begin(), people.end(), [](const Person& p) {
        cout << p.name << "(" << p.age << ") ";
    });
    // 输出：未成年(0) 李四(22) 未成年(0) 赵六(25)

    cout << endl;
    return 0;
}
```

---

## 五、swap 算法

### 5.1 基本语法

```cpp
swap(a, b);   // 交换两个变量或容器
```

- 可以交换两个变量
- 可以交换两个容器
- 两个变量/容器的类型必须相同

---

### 5.2 交换两个变量

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int a = 10;
    int b = 20;

    cout << "交换前: a=" << a << ", b=" << b << endl;
    // 输出：交换前: a=10, b=20

    swap(a, b);

    cout << "交换后: a=" << a << ", b=" << b << endl;
    // 输出：交换后: a=20, b=10

    return 0;
}
```

---

### 5.3 交换两个容器

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 2, 3};
    vector<int> v2 = {10, 20, 30, 40, 50};

    cout << "交换前:" << endl;
    cout << "v1: ";
    for_each(v1.begin(), v1.end(), [](int val) { cout << val << " "; });
    cout << endl;

    cout << "v2: ";
    for_each(v2.begin(), v2.end(), [](int val) { cout << val << " "; });
    cout << endl;

    // 交换两个容器的内容
    swap(v1, v2);

    cout << "\n交换后:" << endl;
    cout << "v1: ";
    for_each(v1.begin(), v1.end(), [](int val) { cout << val << " "; });
    cout << endl;

    cout << "v2: ";
    for_each(v2.begin(), v2.end(), [](int val) { cout << val << " "; });
    cout << endl;

    return 0;
}
```

**输出：**
```
交换前:
v1: 1 2 3
v2: 10 20 30 40 50

交换后:
v1: 10 20 30 40 50
v2: 1 2 3
```

**注意：** `swap` 交换容器的时间复杂度是 O(1)（只交换指针），非常快！

---

### 5.4 swap 的常见用途

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {5, 3, 1, 4, 2};

    // 用途1：清空容器并释放内存
    // 直接 clear() 不会释放内存，swap 可以
    vector<int>().swap(v);  // 与空容器交换，相当于清空并释放内存

    cout << "size: " << v.size() << endl;      // 输出：size: 0
    cout << "capacity: " << v.capacity() << endl; // 输出：capacity: 0

    return 0;
}
```

---

## 六、fill 算法

### 6.1 基本语法

```cpp
fill(begin, end, value);
```

- `begin, end`：容器的起始和结束迭代器
- `value`：要填充的值
- **功能：** 将范围内所有元素设置为指定值

---

### 6.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v;
    v.resize(5);  // 先分配5个空间

    // 用 100 填充所有元素
    fill(v.begin(), v.end(), 100);

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：100 100 100 100 100

    cout << endl;
    return 0;
}
```

---

### 6.3 部分填充

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 只填充前3个元素
    fill(v.begin(), v.begin() + 3, 0);

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：0 0 0 4 5

    cout << endl;
    return 0;
}
```

---

### 6.4 fill 与 fill_n 的区别

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1(5), v2(5);

    // fill: 指定范围 [begin, end)
    fill(v1.begin(), v1.end(), 10);

    // fill_n: 指定起始位置和数量
    fill_n(v2.begin(), 5, 20);  // 从 v2.begin() 开始填充5个20

    cout << "fill 结果: ";
    for_each(v1.begin(), v1.end(), [](int val) { cout << val << " "; });
    cout << endl;  // 输出：fill 结果: 10 10 10 10 10

    cout << "fill_n 结果: ";
    for_each(v2.begin(), v2.end(), [](int val) { cout << val << " "; });
    cout << endl;  // 输出：fill_n 结果: 20 20 20 20 20

    return 0;
}
```

---

## 七、综合示例

### 示例1：数据清洗

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    // 假设这是采集到的数据，其中 -1 表示无效数据
    vector<int> data = {10, 25, -1, 30, -1, 45, 50, -1, 60};

    cout << "原始数据: ";
    for_each(data.begin(), data.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;

    // 将所有 -1（无效数据）替换为 0
    replace(data.begin(), data.end(), -1, 0);

    cout << "清洗后: ";
    for_each(data.begin(), data.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：清洗后: 10 25 0 30 0 45 50 0 60

    return 0;
}
```

### 示例2：数据归一化

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> scores = {85, 92, 78, 95, 60, 88, 45, 100};

    cout << "原始成绩: ";
    for_each(scores.begin(), scores.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;

    // 将低于60分的成绩全部替换为60分（保底分数）
    replace_if(scores.begin(), scores.end(), [](int val) {
        return val < 60;
    }, 60);

    cout << "调整后: ";
    for_each(scores.begin(), scores.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：调整后: 85 92 78 95 60 88 60 100

    // 备份数据
    vector<int> backup;
    backup.resize(scores.size());
    copy(scores.begin(), scores.end(), backup.begin());

    cout << "备份: ";
    for_each(backup.begin(), backup.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;

    return 0;
}
```

---

## 八、注意事项总结

1. **`copy` 的目标容器必须提前分配空间**，用 `resize()` 分配
2. **`replace` 替换所有匹配的值**，不只是第一个
3. **`replace_if` 使用谓词判断**，更灵活
4. **`swap` 交换容器是 O(1) 操作**，非常高效
5. **`fill` 要求容器已有空间**，不会自动创建新元素
6. **`fill_n` 需要确保有足够的空间**，否则是未定义行为
7. **`replace` 和 `replace_if` 是就地修改**，直接修改原容器
8. **头文件：`#include <algorithm>`**
