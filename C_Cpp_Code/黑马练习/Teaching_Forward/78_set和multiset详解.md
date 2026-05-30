# set 和 multiset 详解

## 一、什么是 set？

`set` 是 STL 中的**关联容器**，底层实现是**红黑树**（一种自平衡二叉搜索树）。

### set 的核心特点

| 特点 | 说明 |
|------|------|
| 自动排序 | 默认**升序**排列 |
| 不允许重复 | 相同的值只能存在一个 |
| 底层结构 | 红黑树 |
| 查找效率 | O(log n) |
| 插入效率 | O(log n) |
| 不支持随机访问 | 不能用 `[]` 访问 |

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    // 创建set，插入元素会自动排序
    set<int> s;
    s.insert(30);
    s.insert(10);
    s.insert(50);
    s.insert(20);
    s.insert(40);

    // 遍历：自动升序排列
    cout << "set元素: ";
    for (auto it = s.begin(); it != s.end(); ++it) {
        cout << *it << " "; // 10 20 30 40 50
    }
    cout << endl;

    // 重复插入会被忽略
    s.insert(30); // 30已经存在，不会插入
    cout << "插入重复30后大小: " << s.size() << endl; // 仍然是5

    return 0;
}
```

## 二、set 的构造与插入

```cpp
#include <iostream>
#include <set>
using namespace std;

void printSet(const set<int>& s, const string& name) {
    cout << name << ": ";
    for (auto val : s) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    // ========== 构造方式 ==========

    // 方式1：默认构造
    set<int> s1;

    // 方式2：初始化列表（C++11）
    set<int> s2 = {5, 3, 1, 4, 2};
    printSet(s2, "初始化列表"); // 1 2 3 4 5（自动排序）

    // 方式3：迭代器区间构造
    int arr[] = {10, 30, 20, 50, 40};
    set<int> s3(arr, arr + 5);
    printSet(s3, "迭代器区间"); // 10 20 30 40 50

    // 方式4：拷贝构造
    set<int> s4(s2);
    printSet(s4, "拷贝构造"); // 1 2 3 4 5

    // ========== 插入操作 ==========

    set<int> s;

    // 方式1：insert(value) —— 返回pair<iterator, bool>
    auto result = s.insert(10);
    cout << "插入10: 迭代器指向" << *result.first
         << ", 是否成功: " << result.second << endl; // 10, 1(true)

    // 尝试插入重复值
    result = s.insert(10);
    cout << "再次插入10: 迭代器指向" << *result.first
         << ", 是否成功: " << result.second << endl; // 10, 0(false)

    // 方式2：insert(iterator, value) —— 提示位置（可加快插入）
    s.insert(s.end(), 20); // 提示在末尾附近插入

    // 方式3：insert初始化列表（C++11）
    s.insert({30, 40, 50});

    printSet(s, "插入后"); // 10 20 30 40 50

    // 方式4：emplace（C++11，原地构造）
    s.emplace(60); // 等价于insert(60)

    return 0;
}
```

## 三、大小、判空与查找

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s = {10, 20, 30, 40, 50};

    // ========== 大小与判空 ==========
    cout << "大小: " << s.size() << endl;     // 5
    cout << "是否为空: " << s.empty() << endl; // 0(false)

    // ========== find 查找 ==========
    // find(value)：返回指向该元素的迭代器，找不到返回end()
    auto it = s.find(30);
    if (it != s.end()) {
        cout << "找到了: " << *it << endl; // 30
    } else {
        cout << "没找到" << endl;
    }

    it = s.find(99);
    if (it == s.end()) {
        cout << "99不存在" << endl;
    }

    // ========== count 计数 ==========
    // set中count只返回0或1（因为不允许重复）
    cout << "count(30): " << s.count(30) << endl; // 1
    cout << "count(99): " << s.count(99) << endl; // 0

    // ========== lower_bound 和 upper_bound ==========
    // lower_bound(value)：返回第一个 >= value 的迭代器
    // upper_bound(value)：返回第一个 > value 的迭代器
    set<int> s2 = {10, 20, 30, 40, 50};

    auto lb = s2.lower_bound(25); // 第一个 >= 25 的元素
    cout << "lower_bound(25): " << *lb << endl; // 30

    auto ub = s2.upper_bound(30); // 第一个 > 30 的元素
    cout << "upper_bound(30): " << *ub << endl; // 40

    // 如果找不到，返回end()
    auto lb2 = s2.lower_bound(60);
    if (lb2 == s2.end()) {
        cout << "lower_bound(60): end()" << endl;
    }

    // ========== equal_range ==========
    // 返回一个pair，first是lower_bound，second是upper_bound
    // 对于set，区间内最多只有一个元素（不允许重复）
    auto range = s2.equal_range(30);
    cout << "equal_range(30): [";
    for (auto it = range.first; it != range.second; ++it) {
        cout << *it;
    }
    cout << "]" << endl; // [30]

    // 如果值不存在，区间为空（first == second）
    auto range2 = s2.equal_range(25);
    cout << "equal_range(25)是否为空: "
         << (range2.first == range2.second) << endl; // 1(true)

    return 0;
}
```

## 四、删除与遍历

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int> s = {10, 20, 30, 40, 50};

    // ========== 删除操作 ==========

    // 方式1：erase(value) —— 按值删除，返回删除个数
    size_t count = s.erase(30);
    cout << "删除30, 删除了" << count << "个" << endl; // 1

    // 删除不存在的值
    count = s.erase(99);
    cout << "删除99, 删除了" << count << "个" << endl; // 0

    // 方式2：erase(iterator) —— 按迭代器删除
    auto it = s.find(20);
    if (it != s.end()) {
        s.erase(it); // 删除20
    }

    // 方式3：erase(first, last) —— 删除区间
    // 删除40到末尾的所有元素
    s.erase(s.lower_bound(40), s.end());

    cout << "删除后: ";
    for (auto val : s) {
        cout << val << " "; // 10
    }
    cout << endl;

    // 方式4：clear() —— 清空
    s.clear();
    cout << "清空后大小: " << s.size() << endl; // 0

    // ========== 遍历方式 ==========
    set<int> s2 = {50, 30, 10, 40, 20};

    // 方式1：迭代器正向遍历（自动升序）
    cout << "正向遍历: ";
    for (auto it = s2.begin(); it != s2.end(); ++it) {
        cout << *it << " "; // 10 20 30 40 50
    }
    cout << endl;

    // 方式2：反向迭代器
    cout << "反向遍历: ";
    for (auto it = s2.rbegin(); it != s2.rend(); ++it) {
        cout << *it << " "; // 50 40 30 20 10
    }
    cout << endl;

    // 方式3：范围for（最简洁）
    cout << "范围for: ";
    for (auto val : s2) {
        cout << val << " "; // 10 20 30 40 50
    }
    cout << endl;

    // 方式4：范围for + const引用（推荐，避免拷贝）
    cout << "const引用: ";
    for (const auto& val : s2) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

## 五、修改排序规则（仿函数）

set 默认是升序排序，可以通过**仿函数**（函数对象）来修改排序规则。

```cpp
#include <iostream>
#include <set>
using namespace std;

// ========== 方法1：使用greater<int>实现降序 ==========
// greater<int> 是STL内置的仿函数，需要 #include <functional>
#include <functional>

int main() {
    // 降序排列的set
    set<int, greater<int>> s1 = {30, 10, 50, 20, 40};
    cout << "降序set: ";
    for (auto val : s1) {
        cout << val << " "; // 50 40 30 20 10
    }
    cout << endl;

    // ========== 方法2：自定义仿函数 ==========
    // 仿函数就是一个重载了 operator() 的类
    struct MyCompare {
        bool operator()(int a, int b) const {
            return a > b; // 降序
        }
    };

    set<int, MyCompare> s2 = {30, 10, 50, 20, 40};
    cout << "自定义仿函数降序: ";
    for (auto val : s2) {
        cout << val << " "; // 50 40 30 20 10
    }
    cout << endl;

    // ========== 方法3：Lambda表达式（C++11，较复杂） ==========
    // 注意：Lambda作为模板参数需要借助decltype和function
    auto comp = [](int a, int b) { return a > b; };
    set<int, decltype(comp)> s3(comp); // 需要把lambda传给构造函数
    s3.insert({30, 10, 50, 20, 40});
    cout << "Lambda降序: ";
    for (auto val : s3) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
```

### 仿函数详解

```cpp
#include <iostream>
#include <set>
using namespace std;

// 仿函数（函数对象）：就是一个类重载了 operator()
// 可以像函数一样被调用
struct Descending {
    // 重载()运算符
    // 参数：两个要比较的值
    // 返回：true表示a应该排在b前面
    bool operator()(int a, int b) const {
        return a > b; // 降序：大的排前面
    }
};

// 也可以用模板实现通用的比较器
template<typename T>
struct MyGreater {
    bool operator()(const T& a, const T& b) const {
        return a > b;
    }
};

int main() {
    // 使用自定义仿函数
    set<int, Descending> s = {30, 10, 50, 20, 40};
    cout << "自定义降序: ";
    for (auto val : s) {
        cout << val << " "; // 50 40 30 20 10
    }
    cout << endl;

    // 使用模板仿函数
    set<int, MyGreater<int>> s2 = {30, 10, 50};
    cout << "模板仿函数: ";
    for (auto val : s2) {
        cout << val << " "; // 50 30 10
    }
    cout << endl;

    return 0;
}
```

## 六、自定义类型存储 set

自定义类型要放入 set，必须提供比较方式，否则编译器不知道如何排序。

```cpp
#include <iostream>
#include <set>
#include <string>
using namespace std;

// ========== 方法1：重载 < 运算符 ==========
struct Student {
    string name;
    int age;

    // 重载 < 运算符（set默认用 < 比较）
    bool operator<(const Student& other) const {
        // 按年龄升序排序
        if (age != other.age) {
            return age < other.age;
        }
        // 年龄相同按姓名排序
        return name < other.name;
    }
};

// ========== 方法2：提供外部比较器（仿函数） ==========
struct CompareByName {
    bool operator()(const Student& a, const Student& b) const {
        return a.name < b.name; // 按姓名排序
    }
};

int main() {
    // 使用方法1：重载 < 运算符
    set<Student> s1;
    s1.insert({"张三", 20});
    s1.insert({"李四", 18});
    s1.insert({"王五", 22});
    s1.insert({"赵六", 18}); // 同年龄不同姓名，可以插入

    cout << "按年龄排序:" << endl;
    for (const auto& stu : s1) {
        cout << "  " << stu.name << ", " << stu.age << "岁" << endl;
    }

    // 使用方法2：外部比较器
    set<Student, CompareByName> s2;
    s2.insert({"张三", 20});
    s2.insert({"李四", 18});
    s2.insert({"王五", 22});

    cout << "按姓名排序:" << endl;
    for (const auto& stu : s2) {
        cout << "  " << stu.name << ", " << stu.age << "岁" << endl;
    }

    // 注意：如果没有重载 < 也没有提供比较器，以下代码会编译错误
    // set<Student> s3; // 错误！不知道如何比较Student

    return 0;
}
```

## 七、multiset 与 set 的区别

`multiset` 和 `set` 几乎完全一样，唯一区别是 **multiset 允许重复元素**。

```cpp
#include <iostream>
#include <set>
using namespace std;

int main() {
    // ========== multiset 允许重复 ==========
    multiset<int> ms = {10, 20, 30, 20, 10, 20};
    cout << "multiset元素: ";
    for (auto val : ms) {
        cout << val << " "; // 10 10 20 20 20 30
    }
    cout << endl;
    cout << "multiset大小: " << ms.size() << endl; // 6

    // ========== count 返回值可以大于1 ==========
    cout << "count(20): " << ms.count(20) << endl; // 3
    cout << "count(10): " << ms.count(10) << endl; // 2
    cout << "count(99): " << ms.count(99) << endl; // 0

    // ========== insert 总是成功 ==========
    auto result = ms.insert(20); // multiset的insert总是成功
    cout << "插入20是否成功: " << result.second << endl; // 1(true)
    // 注意：multiset的insert返回的是iterator，不是pair
    // 实际上C++11中multiset::insert也返回iterator

    // ========== find 返回第一个匹配的迭代器 ==========
    auto it = ms.find(20);
    if (it != ms.end()) {
        cout << "find(20)找到: " << *it << endl; // 20
    }

    // ========== erase 删除所有匹配值 ==========
    size_t count = ms.erase(20); // 删除所有20
    cout << "删除了" << count << "个20" << endl; // 4（3个旧的+1个新插入的）

    cout << "删除后: ";
    for (auto val : ms) {
        cout << val << " "; // 10 10 30
    }
    cout << endl;

    // ========== 只删除一个匹配值 ==========
    ms.insert(10);
    ms.insert(10);
    cout << "有" << ms.count(10) << "个10" << endl; // 4

    auto it10 = ms.find(10);
    ms.erase(it10); // 只删除一个10（删除迭代器指向的那个）
    cout << "删除一个10后: " << ms.count(10) << "个10" << endl; // 3

    // ========== lower_bound / upper_bound 的区别 ==========
    multiset<int> ms2 = {10, 20, 20, 20, 30};

    // lower_bound：第一个 >= value 的位置
    auto lb = ms2.lower_bound(20);
    cout << "lower_bound(20): " << *lb << endl; // 20（第一个20）

    // upper_bound：第一个 > value 的位置
    auto ub = ms2.upper_bound(20);
    cout << "upper_bound(20): " << *ub << endl; // 30

    // 利用 lower_bound 和 upper_bound 遍历所有20
    cout << "所有20: ";
    for (auto it = ms2.lower_bound(20); it != ms2.upper_bound(20); ++it) {
        cout << *it << " "; // 20 20 20
    }
    cout << endl;

    return 0;
}
```

### set vs multiset 对比总结

| 特性 | set | multiset |
|------|-----|---------|
| 重复元素 | **不允许** | **允许** |
| insert返回值 | `pair<iterator, bool>` | `iterator` |
| count返回值 | 0 或 1 | 0 到 n |
| erase(value) | 删除0或1个 | 删除所有匹配的 |
| 其他操作 | 相同 | 相同 |

## 八、pair 对组

`pair` 是 STL 中的一个模板类，可以将两个值组合成一对。在 set/map 中经常使用。

```cpp
#include <iostream>
#include <set>
#include <string>
using namespace std;

int main() {
    // ========== 创建 pair 的方式 ==========

    // 方式1：直接构造
    pair<string, int> p1("张三", 20);
    cout << p1.first << ", " << p1.second << endl; // 张三, 20

    // 方式2：make_pair（推荐，可以自动推导类型）
    auto p2 = make_pair("李四", 25);
    cout << p2.first << ", " << p2.second << endl; // 李四, 25

    // 方式3：花括号初始化（C++11）
    pair<string, int> p3{"王五", 30};
    cout << p3.first << ", " << p3.second << endl; // 王五, 30

    // ========== pair 的比较 ==========
    // pair先比较first，first相同时再比较second
    pair<int, int> a = {1, 2};
    pair<int, int> b = {1, 3};
    pair<int, int> c = {2, 1};

    cout << "a < b: " << (a < b) << endl; // 1(true)，first相同，2<3
    cout << "a < c: " << (a < c) << endl; // 1(true)，1<2

    // ========== pair 在 set 中的应用 ==========
    set<pair<int, int>> sp;
    sp.insert({1, 2});
    sp.insert({1, 3});
    sp.insert({2, 1});
    sp.insert({1, 2}); // 重复，不会插入

    cout << "set<pair>元素: ";
    for (const auto& p : sp) {
        cout << "(" << p.first << "," << p.second << ") ";
    }
    cout << endl; // (1,2) (1,3) (2,1)

    // ========== set::insert 返回 pair ==========
    set<int> s;
    auto result = s.insert(10);
    // result 是 pair<set<int>::iterator, bool>
    cout << "插入10: 值=" << *result.first
         << ", 成功=" << result.second << endl;

    result = s.insert(10); // 重复插入
    cout << "再次插入10: 值=" << *result.first
         << ", 成功=" << result.second << endl;

    return 0;
}
```

## 九、综合实战：学生成绩管理系统

```cpp
#include <iostream>
#include <set>
#include <string>
using namespace std;

struct Student {
    string name;
    int score;

    // 按成绩降序排列（分数高的排前面）
    // 分数相同按姓名升序
    bool operator<(const Student& other) const {
        if (score != other.score) {
            return score > other.score; // 降序
        }
        return name < other.name;
    }
};

int main() {
    set<Student> students;

    // 插入学生成绩
    students.insert({"张三", 85});
    students.insert({"李四", 92});
    students.insert({"王五", 78});
    students.insert({"赵六", 92}); // 与李四同分
    students.insert({"钱七", 88});

    // 按成绩排名输出
    cout << "====== 成绩排名 ======" << endl;
    int rank = 1;
    for (const auto& stu : students) {
        cout << "第" << rank++ << "名: "
             << stu.name << " - " << stu.score << "分" << endl;
    }

    // 查找某个分数段的学生
    cout << "\n90分以上的学生:" << endl;
    for (const auto& stu : students) {
        if (stu.score >= 90) {
            cout << "  " << stu.name << ": " << stu.score << "分" << endl;
        }
    }

    // 用multiset按成绩分组（允许重复分数）
    multiset<int> scores;
    for (const auto& stu : students) {
        scores.insert(stu.score);
    }
    cout << "\n92分有" << scores.count(92) << "人" << endl;

    return 0;
}
```

## 十、总结

| 要点 | 内容 |
|------|------|
| set特点 | 自动排序、不允许重复、红黑树 |
| 排序默认 | 升序（`less<T>`） |
| 修改排序 | 仿函数模板参数，如 `set<int, greater<int>>` |
| 查找 | `find` 返回迭代器，`count` 只有0或1 |
| 边界查找 | `lower_bound(>=)`、`upper_bound(>)` |
| 自定义类型 | 重载 `<` 或提供比较器 |
| multiset | 允许重复，其余与set相同 |
| pair | `first`/`second`，`make_pair` 创建 |
