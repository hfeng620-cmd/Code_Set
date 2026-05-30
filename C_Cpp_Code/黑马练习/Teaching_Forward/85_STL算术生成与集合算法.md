# STL 算术生成与集合算法

## 一、概述

| 类别 | 算法 | 作用 |
|------|------|------|
| 算术生成 | `accumulate` | 计算范围内元素的累加和 |
| 算术生成 | `fill` | 用指定值填充容器 |
| 集合算法 | `set_intersection` | 求两个集合的交集 |
| 集合算法 | `set_union` | 求两个集合的并集 |
| 集合算法 | `set_difference` | 求两个集合的差集 |

**算术生成头文件：** `#include <numeric>`
**集合算法头文件：** `#include <algorithm>`

---

## 二、accumulate 算法

### 2.1 基本语法

```cpp
accumulate(begin, end, 初始值);
```

- `begin, end`：容器的起始和结束迭代器
- `初始值`：累加的起始值（通常为 0）
- **返回值：** 范围内所有元素加上初始值的总和

---

### 2.2 基本累加

```cpp
#include <iostream>
#include <vector>
#include <numeric>   // accumulate 在这个头文件中
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 计算所有元素的累加和，初始值为0
    int sum = accumulate(v.begin(), v.end(), 0);
    cout << "累加和: " << sum << endl;  // 输出：累加和: 15
    // 计算过程：0 + 1 + 2 + 3 + 4 + 5 = 15

    // 初始值为10
    int sum2 = accumulate(v.begin(), v.end(), 10);
    cout << "初始值为10的累加和: " << sum2 << endl;  // 输出：25
    // 计算过程：10 + 1 + 2 + 3 + 4 + 5 = 25

    return 0;
}
```

**注意：** 初始值决定了返回值的类型。如果初始值是 `int`，返回值也是 `int`；如果初始值是 `double`，返回值也是 `double`。

---

### 2.3 累乘操作

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <functional>  // multiplies 在这个头文件中
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 累乘：初始值为1，使用 multiplies 仿函数
    int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());
    cout << "累乘结果: " << product << endl;  // 输出：累乘结果: 120
    // 计算过程：1 * 1 * 2 * 3 * 4 * 5 = 120

    return 0;
}
```

**说明：**
- `multiplies<int>()` 是 STL 提供的仿函数，执行乘法运算
- 初始值必须为 1（乘法的单位元），否则结果不对
- 类似的仿函数还有：
  - `plus<int>()` —— 加法（默认就是这个）
  - `minus<int>()` —— 减法
  - `divides<int>()` —— 除法

---

### 2.4 自定义累加操作（使用 Lambda）

```cpp
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 自定义累加：计算平方和
    // sum = 初始值 + 1^2 + 2^2 + 3^2 + 4^2 + 5^2
    int sumOfSquares = accumulate(v.begin(), v.end(), 0, [](int sum, int val) {
        return sum + val * val;
    });
    cout << "平方和: " << sumOfSquares << endl;  // 输出：平方和: 55
    // 计算过程：0 + 1 + 4 + 9 + 16 + 25 = 55

    return 0;
}
```

---

### 2.5 计算平均值

```cpp
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int main()
{
    vector<int> v = {85, 90, 78, 92, 88};

    // 计算总分
    int total = accumulate(v.begin(), v.end(), 0);
    // 计算平均分
    double average = static_cast<double>(total) / v.size();

    cout << "总分: " << total << endl;       // 输出：总分: 433
    cout << "平均分: " << average << endl;    // 输出：平均分: 86.6

    return 0;
}
```

---

### 2.6 累加自定义类型

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
using namespace std;

class Product
{
public:
    string name;
    double price;

    Product(string n, double p) : name(n), price(p) {}
};

int main()
{
    vector<Product> products = {
        Product("苹果", 5.5),
        Product("香蕉", 3.0),
        Product("橙子", 4.5),
        Product("葡萄", 8.0)
    };

    // 计算总价
    double total = accumulate(products.begin(), products.end(), 0.0,
        [](double sum, const Product& p) {
            return sum + p.price;
        });

    cout << "总价: " << total << endl;  // 输出：总价: 21

    return 0;
}
```

---

## 三、fill 算法

### 3.1 基本语法

```cpp
fill(begin, end, value);
```

- 用指定值 `value` 填充 `[begin, end)` 范围内的所有元素
- 容器必须已经有空间（不会自动创建新元素）

---

### 3.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v;
    v.resize(5);  // 先分配5个空间

    // 用 42 填充所有元素
    fill(v.begin(), v.end(), 42);

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：42 42 42 42 42

    cout << endl;
    return 0;
}
```

---

### 3.3 部分填充

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4, 5};

    // 只填充索引1到3（不包含3）的元素
    fill(v.begin() + 1, v.begin() + 4, 0);

    for_each(v.begin(), v.end(), [](int val) {
        cout << val << " ";
    });
    // 输出：1 0 0 0 5

    cout << endl;
    return 0;
}
```

---

### 3.4 常见错误

```cpp
vector<int> v;  // 空容器

// 错误！v 没有空间，fill 会导致未定义行为
fill(v.begin(), v.end(), 10);

// 正确做法1：先 resize
v.resize(5);
fill(v.begin(), v.end(), 10);

// 正确做法2：创建时指定大小
vector<int> v2(5);  // 5个元素，默认值为0
fill(v2.begin(), v2.end(), 10);
```

---

## 四、集合算法概述

集合算法用于对两个已排序的容器进行集合运算。

**【非常重要】两个容器都必须是有序的！**（默认升序）

如果容器无序，需要先排序：
```cpp
sort(v1.begin(), v1.end());
sort(v2.begin(), v2.end());
```

---

## 五、set_intersection（交集）

### 5.1 基本语法

```cpp
set_intersection(begin1, end1, begin2, end2, dst_begin);
```

- 求两个有序容器的交集（同时存在于两个容器中的元素）
- 返回目标容器的结束迭代器
- **目标容器的大小：** 取两个源容器中较小的那个

---

### 5.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = {3, 4, 5, 6, 7};
    vector<int> dst;

    // 目标容器大小取两个源容器中较小的那个
    dst.resize(min(v1.size(), v2.size()));

    // 求交集
    // 返回值是目标容器中实际有数据的结束位置
    vector<int>::iterator itEnd = set_intersection(
        v1.begin(), v1.end(),
        v2.begin(), v2.end(),
        dst.begin()
    );

    // 打印交集结果（从 begin 到 itEnd，不是到 dst.end()）
    cout << "交集: ";
    for_each(dst.begin(), itEnd, [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：交集: 3 4 5

    return 0;
}
```

**关键点：**
- 交集结果从 `dst.begin()` 到返回的迭代器 `itEnd` 之间
- 不能用 `dst.end()`，因为多余的元素是无意义的

---

### 5.3 使用注意事项

```cpp
// 错误1：容器未排序
vector<int> v1 = {5, 3, 1};  // 未排序！
vector<int> v2 = {4, 3, 2};
// 结果未定义！

// 错误2：目标容器未分配空间
vector<int> dst;  // 空的！
// 崩溃！

// 正确做法
sort(v1.begin(), v1.end());
sort(v2.begin(), v2.end());
dst.resize(min(v1.size(), v2.size()));
set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());
```

---

## 六、set_union（并集）

### 6.1 基本语法

```cpp
set_union(begin1, end1, begin2, end2, dst_begin);
```

- 求两个有序容器的并集（所有不重复的元素）
- 返回目标容器的结束迭代器
- **目标容器的大小：** 取两个源容器大小之和

---

### 6.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = {3, 4, 5, 6, 7};
    vector<int> dst;

    // 目标容器大小取两个源容器大小之和
    dst.resize(v1.size() + v2.size());

    // 求并集
    vector<int>::iterator itEnd = set_union(
        v1.begin(), v1.end(),
        v2.begin(), v2.end(),
        dst.begin()
    );

    cout << "并集: ";
    for_each(dst.begin(), itEnd, [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：并集: 1 2 3 4 5 6 7

    return 0;
}
```

**注意：** 并集中的重复元素只会出现一次。`3, 4, 5` 在两个容器中都有，但并集中只出现一次。

---

## 七、set_difference（差集）

### 7.1 基本语法

```cpp
set_difference(begin1, end1, begin2, end2, dst_begin);
```

- 求两个有序容器的差集
- **v1 对 v2 的差集：** 在 v1 中但不在 v2 中的元素
- 返回目标容器的结束迭代器
- **目标容器的大小：** 取两个源容器中较大的那个

---

### 7.2 基本用法

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = {3, 4, 5, 6, 7};
    vector<int> dst;

    // 目标容器大小取两个源容器中较大的那个
    dst.resize(max(v1.size(), v2.size()));

    // v1 对 v2 的差集：在 v1 中但不在 v2 中的元素
    vector<int>::iterator itEnd = set_difference(
        v1.begin(), v1.end(),
        v2.begin(), v2.end(),
        dst.begin()
    );

    cout << "v1对v2的差集: ";
    for_each(dst.begin(), itEnd, [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：v1对v2的差集: 1 2

    return 0;
}
```

**解释：**
- v1 = {1, 2, 3, 4, 5}
- v2 = {3, 4, 5, 6, 7}
- v1 对 v2 的差集 = 在 v1 中但不在 v2 中的元素 = {1, 2}

---

### 7.3 差集的方向很重要！

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = {3, 4, 5, 6, 7};
    vector<int> dst;

    dst.resize(max(v1.size(), v2.size()));

    // v1 对 v2 的差集
    auto itEnd1 = set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());
    cout << "v1对v2的差集: ";
    for_each(dst.begin(), itEnd1, [](int val) { cout << val << " "; });
    cout << endl;  // 输出：1 2

    // v2 对 v1 的差集（注意方向不同！）
    auto itEnd2 = set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), dst.begin());
    cout << "v2对v1的差集: ";
    for_each(dst.begin(), itEnd2, [](int val) { cout << val << " "; });
    cout << endl;  // 输出：6 7

    return 0;
}
```

**结论：** 差集的方向不同，结果也不同！
- v1 对 v2 的差集：{1, 2}（在 v1 中但不在 v2 中）
- v2 对 v1 的差集：{6, 7}（在 v2 中但不在 v1 中）

---

## 八、综合示例

### 示例1：两个班级的学生交集（找出同时参加两个社团的人）

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    // 参加编程社团的学生（按学号排序）
    vector<string> club1 = {"001", "002", "003", "004", "005"};
    // 参加数学社团的学生（按学号排序）
    vector<string> club2 = {"002", "004", "006", "008"};

    // 找出同时参加两个社团的学生（交集）
    vector<string> both;
    both.resize(min(club1.size(), club2.size()));

    auto itEnd = set_intersection(
        club1.begin(), club1.end(),
        club2.begin(), club2.end(),
        both.begin()
    );

    cout << "同时参加两个社团的学生: ";
    for_each(both.begin(), itEnd, [](const string& s) {
        cout << s << " ";
    });
    cout << endl;  // 输出：同时参加两个社团的学生: 002 004

    return 0;
}
```

---

### 示例2：数据合并与去重

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    // 两个班级的成绩（已排序）
    vector<int> class1 = {70, 75, 80, 85, 90};
    vector<int> class2 = {65, 75, 85, 95, 100};

    // 求并集（合并所有成绩，去重）
    vector<int> all;
    all.resize(class1.size() + class2.size());

    auto itEnd = set_union(
        class1.begin(), class1.end(),
        class2.begin(), class2.end(),
        all.begin()
    );

    cout << "所有成绩（去重）: ";
    for_each(all.begin(), itEnd, [](int val) {
        cout << val << " ";
    });
    cout << endl;  // 输出：所有成绩（去重）: 65 70 75 80 85 90 95 100

    return 0;
}
```

---

## 九、集合算法对比总结

| 算法 | 结果含义 | 目标容器大小 |
|------|----------|-------------|
| `set_intersection` | 同时在两个容器中的元素 | `min(size1, size2)` |
| `set_union` | 两个容器中所有不重复的元素 | `size1 + size2` |
| `set_difference` | 在第一个容器中但不在第二个容器中的元素 | `max(size1, size2)` |

---

## 十、注意事项总结

1. **`accumulate` 在 `<numeric>` 头文件中**，不在 `<algorithm>` 中
2. **`accumulate` 的初始值决定了返回类型**，通常用 `0`（整数累加）或 `0.0`（浮点累加）
3. **累乘用 `accumulate` + `multiplies<int>()`**，初始值必须为 1
4. **`fill` 要求容器已有空间**，不会自动创建新元素
5. **所有集合算法都要求两个源容器必须有序！**
6. **目标容器必须提前分配空间**，用 `resize()` 分配
7. **用返回的迭代器来确定结果范围**，而不是 `dst.end()`
8. **差集有方向性**，v1 对 v2 和 v2 对 v1 的结果不同
9. **`set_intersection` 的结果不会重复**，即使源容器中有重复元素
10. **头文件：**
    - `accumulate`：`#include <numeric>`
    - `fill`、集合算法：`#include <algorithm>`
