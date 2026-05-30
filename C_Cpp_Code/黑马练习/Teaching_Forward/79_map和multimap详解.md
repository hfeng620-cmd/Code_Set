# map 和 multimap 详解

## 一、什么是 map？

`map` 是 STL 中的**关联容器**，存储**键值对（key-value pair）**，底层实现是**红黑树**。

### map 的核心特点

| 特点 | 说明 |
|------|------|
| 键值对 | 每个元素是一对 `key-value` |
| 按 key 排序 | 默认按 key 升序排列 |
| key 不允许重复 | 相同 key 只保留一个 |
| 底层结构 | 红黑树 |
| 查找效率 | O(log n) |

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    // 创建一个map：学号 -> 姓名
    map<int, string> m;
    m[1001] = "张三";
    m[1003] = "李四";
    m[1002] = "王五";

    // 遍历：按key自动排序
    for (auto it = m.begin(); it != m.end(); ++it) {
        cout << "学号: " << it->first << ", 姓名: " << it->second << endl;
    }
    // 输出：1001张三, 1002王五, 1003李四（按学号升序）

    return 0;
}
```

## 二、map 的构造与插入

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

void printMap(const map<int, string>& m, const string& name) {
    cout << name << ":" << endl;
    for (const auto& pair : m) {
        cout << "  key=" << pair.first << ", value=" << pair.second << endl;
    }
}

int main() {
    // ========== 构造方式 ==========

    // 方式1：默认构造
    map<int, string> m1;

    // 方式2：初始化列表（C++11）
    map<int, string> m2 = {
        {1, "苹果"},
        {2, "香蕉"},
        {3, "橘子"}
    };
    printMap(m2, "初始化列表");

    // 方式3：拷贝构造
    map<int, string> m3(m2);
    printMap(m3, "拷贝构造");

    // ========== 插入方式（重点！4种方式） ==========

    map<int, string> m;

    // 方式1：insert(pair) —— 用pair对象
    m.insert(pair<int, string>(1, "张三"));

    // 方式2：insert(make_pair) —— 用make_pair函数（推荐）
    m.insert(make_pair(2, "李四"));

    // 方式3：insert({key, value}) —— 用花括号（C++11，最简洁）
    m.insert({3, "王五"});

    // 方式4：operator[] —— 最常用的方式！
    m[4] = "赵六";
    m[5] = "钱七";

    printMap(m, "插入后");

    // ========== insert 的返回值 ==========
    // insert返回 pair<iterator, bool>
    auto result = m.insert(make_pair(1, "新张三"));
    cout << "插入已存在的key: 成功=" << result.second << endl; // 0(false)
    cout << "迭代器指向: key=" << result.first->first
         << ", value=" << result.first->second << endl; // 1, 张三（原值不变）

    // ========== insert_or_assign（C++17） ==========
    // 如果key存在就赋值，不存在就插入
    // m.insert_or_assign(1, "新张三"); // C++17才有

    return 0;
}
```

## 三、map 的访问操作（重点）

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> scores;
    scores["张三"] = 85;
    scores["李四"] = 92;
    scores["王五"] = 78;

    // ========== operator[] 访问 ==========
    // 最常用的访问方式
    cout << "张三的成绩: " << scores["张三"] << endl; // 85

    // ========== 非常重要的警告！ ==========
    // 用[]访问不存在的key，会自动创建该key！value为默认值！
    cout << "赵六的成绩: " << scores["赵六"] << endl; // 0（自动创建了！）
    cout << "map大小: " << scores.size() << endl;     // 4（赵六被自动加入了）

    // 如果只是想查询，不想自动创建，应该用find
    scores.erase("赵六"); // 先删掉刚才自动创建的

    // ========== at() 访问 ==========
    // at() 不存在会抛异常，不会自动创建
    cout << "李四的成绩: " << scores.at("李四") << endl; // 92
    // cout << scores.at("赵六"); // 抛出 out_of_range 异常！

    // ========== find 查找 ==========
    // find返回迭代器，找不到返回end()
    auto it = scores.find("王五");
    if (it != scores.end()) {
        cout << "找到王五: " << it->second << endl; // 78
    }

    it = scores.find("赵六");
    if (it == scores.end()) {
        cout << "赵六不存在" << endl;
    }

    // ========== count 计数 ==========
    // map中count只返回0或1
    cout << "count(张三): " << scores.count("张三") << endl; // 1
    cout << "count(赵六): " << scores.count("赵六") << endl; // 0

    // ========== 安全访问模式 ==========
    // 推荐：先find再访问，避免[]的副作用
    string name = "李四";
    auto pos = scores.find(name);
    if (pos != scores.end()) {
        cout << name << "的成绩是: " << pos->second << endl;
    } else {
        cout << name << "不存在" << endl;
    }

    return 0;
}
```

### operator[] 的本质

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> m;

    // m["hello"] 等价于：
    // (*((m.insert(make_pair("hello", int()))).first)).second
    // 即：如果key不存在，先插入一个默认值，再返回value的引用

    // 所以 [] 有三个作用：
    // 1. 查找：如果key存在，返回对应value
    // 2. 插入：如果key不存在，插入key和默认value
    // 3. 修改：返回的是引用，可以直接赋值

    m["a"] = 1;     // 插入
    m["a"] = 10;    // 修改（key已存在，覆盖原值）
    int val = m["b"]; // 查找+自动插入（b被自动创建，值为0）

    cout << "a=" << m["a"] << endl; // 10
    cout << "b=" << m["b"] << endl; // 0
    cout << "大小: " << m.size() << endl; // 2

    // 实际应用：词频统计（利用[]的自动创建特性）
    string words[] = {"hello", "world", "hello", "cpp", "hello", "world"};
    map<string, int> freq;
    for (const auto& word : words) {
        freq[word]++; // 如果不存在会自动创建并初始化为0，然后++
    }
    for (const auto& p : freq) {
        cout << p.first << ": " << p.second << "次" << endl;
    }
    // cpp: 1次, hello: 3次, world: 2次

    return 0;
}
```

## 四、map 的遍历

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    map<string, int> m = {
        {"苹果", 5},
        {"香蕉", 3},
        {"橘子", 8},
        {"葡萄", 2}
    };

    // ========== 方式1：迭代器遍历（基础方式） ==========
    cout << "迭代器遍历:" << endl;
    for (map<string, int>::iterator it = m.begin(); it != m.end(); ++it) {
        cout << "  " << it->first << " : " << it->second << endl;
        // 注意：map的迭代器指向的是pair，用 -> 访问成员
    }

    // ========== 方式2：简化迭代器（auto） ==========
    cout << "\nauto迭代器:" << endl;
    for (auto it = m.begin(); it != m.end(); ++it) {
        cout << "  " << it->first << " : " << it->second << endl;
    }

    // ========== 方式3：范围for（最简洁，推荐） ==========
    cout << "\n范围for:" << endl;
    for (const auto& p : m) {
        cout << "  " << p.first << " : " << p.second << endl;
    }

    // ========== 方式4：反向遍历 ==========
    cout << "\n反向遍历:" << endl;
    for (auto it = m.rbegin(); it != m.rend(); ++it) {
        cout << "  " << it->first << " : " << it->second << endl;
    }
    // 橘子8, 葡萄2, 香蕉3, 苹果5（按key反序）

    // ========== C++17 结构化绑定（更优雅） ==========
    // 需要编译器支持 C++17
    // cout << "\nC++17结构化绑定:" << endl;
    // for (const auto& [key, value] : m) {
    //     cout << "  " << key << " : " << value << endl;
    // }

    return 0;
}
```

## 五、map 的删除与大小

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

void printMap(const map<int, string>& m) {
    for (const auto& p : m) {
        cout << "  " << p.first << "->" << p.second << endl;
    }
}

int main() {
    map<int, string> m = {
        {1, "张三"}, {2, "李四"}, {3, "王五"},
        {4, "赵六"}, {5, "钱七"}
    };

    // ========== 大小操作 ==========
    cout << "大小: " << m.size() << endl;     // 5
    cout << "是否为空: " << m.empty() << endl; // 0(false)

    // ========== 删除操作 ==========

    // 方式1：erase(key) —— 按key删除
    size_t count = m.erase(3);
    cout << "删除key=3, 删除了" << count << "个" << endl;
    printMap(m);

    // 方式2：erase(iterator) —— 按迭代器删除
    auto it = m.find(2);
    if (it != m.end()) {
        m.erase(it);
    }
    cout << "\n删除key=2后:" << endl;
    printMap(m);

    // 方式3：erase(first, last) —— 删除区间
    m.erase(m.begin(), m.find(4)); // 删除4之前的所有元素
    cout << "\n删除区间后:" << endl;
    printMap(m);

    // 方式4：clear() —— 清空
    m.clear();
    cout << "\n清空后大小: " << m.size() << endl;

    // ========== swap 交换 ==========
    map<int, string> a = {{1, "a"}, {2, "b"}};
    map<int, string> b = {{3, "c"}, {4, "d"}};
    a.swap(b);
    cout << "\n交换后a:" << endl;
    printMap(a); // 3->c, 4->d

    return 0;
}
```

## 六、修改排序规则

```cpp
#include <iostream>
#include <map>
#include <string>
#include <functional>
using namespace std;

int main() {
    // ========== 按key降序排列 ==========
    map<int, string, greater<int>> m1 = {
        {1, "一"}, {3, "三"}, {2, "二"}, {5, "五"}, {4, "四"}
    };
    cout << "按key降序:" << endl;
    for (const auto& p : m1) {
        cout << "  " << p.first << "->" << p.second << endl;
    }
    // 5->五, 4->四, 3->三, 2->二, 1->一

    // ========== 自定义比较器（仿函数） ==========
    struct CompareByLength {
        bool operator()(const string& a, const string& b) const {
            if (a.length() != b.length()) {
                return a.length() < b.length(); // 按字符串长度排序
            }
            return a < b; // 长度相同按字典序
        }
    };

    map<string, int, CompareByLength> m2;
    m2["hello"] = 1;
    m2["hi"] = 2;
    m2["goodbye"] = 3;
    m2["hey"] = 4;
    m2["cpp"] = 5;

    cout << "\n按key长度排序:" << endl;
    for (const auto& p : m2) {
        cout << "  " << p.first << "(" << p.first.length()
             << ") -> " << p.second << endl;
    }
    // cpp(3)->5, hey(3)->4, hi(2)->2, hello(5)->1, goodbye(7)->3

    return 0;
}
```

## 七、自定义类型作为 key

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

// 自定义类型作为key，必须提供比较方式
struct Point {
    int x, y;

    // 方法1：重载 < 运算符
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

// 方法2：外部比较器
struct PointCompare {
    bool operator()(const Point& a, const Point& b) const {
        // 先按x比较，再按y比较
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

int main() {
    // 使用重载 < 的方式
    map<Point, string> m1;
    m1[{1, 2}] = "点A";
    m1[{3, 4}] = "点B";
    m1[{1, 5}] = "点C";

    cout << "Point作为key:" << endl;
    for (const auto& p : m1) {
        cout << "  (" << p.first.x << "," << p.first.y
             << ") -> " << p.second << endl;
    }

    // 使用外部比较器
    map<Point, string, PointCompare> m2;
    m2[{5, 1}] = "点D";
    m2[{2, 3}] = "点E";

    cout << "\n外部比较器:" << endl;
    for (const auto& p : m2) {
        cout << "  (" << p.first.x << "," << p.first.y
             << ") -> " << p.second << endl;
    }

    return 0;
}
```

## 八、map 的实际应用

### 应用1：词频统计

```cpp
#include <iostream>
#include <map>
#include <string>
#include <sstream>
using namespace std;

int main() {
    string text = "the quick brown fox jumps over the lazy dog "
                  "the dog barked at the fox";

    map<string, int> wordCount;
    istringstream iss(text);
    string word;

    // 逐词读入并统计
    while (iss >> word) {
        wordCount[word]++; // 利用[]的自动创建特性
    }

    // 输出词频（按字母序排列，因为map自动排序）
    cout << "词频统计:" << endl;
    for (const auto& p : wordCount) {
        cout << "  " << p.first << ": " << p.second << "次" << endl;
    }

    // 找出出现次数最多的词
    string maxWord;
    int maxCount = 0;
    for (const auto& p : wordCount) {
        if (p.second > maxCount) {
            maxCount = p.second;
            maxWord = p.first;
        }
    }
    cout << "\n出现最多的词: " << maxWord << " (" << maxCount << "次)" << endl;

    return 0;
}
```

### 应用2：简易通讯录

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    // 通讯录：姓名 -> 电话号码
    map<string, string> contacts;

    // 添加联系人
    contacts["张三"] = "138-0000-1111";
    contacts["李四"] = "139-0000-2222";
    contacts["王五"] = "137-0000-3333";
    contacts["赵六"] = "136-0000-4444";

    // 查询
    string name = "李四";
    auto it = contacts.find(name);
    if (it != contacts.end()) {
        cout << name << "的电话: " << it->second << endl;
    } else {
        cout << name << "不在通讯录中" << endl;
    }

    // 修改
    contacts["张三"] = "138-9999-1111"; // 更新电话

    // 删除
    contacts.erase("赵六");

    // 遍历通讯录
    cout << "\n通讯录列表:" << endl;
    for (const auto& c : contacts) {
        cout << "  " << c.first << ": " << c.second << endl;
    }

    return 0;
}
```

## 九、multimap 与 map 的区别

`multimap` 允许重复的 key，但不能使用 `operator[]`。

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main() {
    // ========== multimap 允许重复key ==========
    multimap<string, int> mm;
    mm.insert({"张三", 85});
    mm.insert({"张三", 90});  // 同一个人可以有多个成绩
    mm.insert({"李四", 78});
    mm.insert({"张三", 95});
    mm.insert({"王五", 88});

    cout << "multimap大小: " << mm.size() << endl; // 5

    // ========== 不能使用 operator[] ==========
    // mm["张三"] = 100; // 编译错误！multimap没有operator[]

    // ========== count 返回匹配数量 ==========
    cout << "张三的数量: " << mm.count("张三") << endl; // 3

    // ========== 遍历所有同key的元素 ==========
    cout << "\n张三的所有成绩:" << endl;
    for (auto it = mm.find("张三"); it != mm.end() && it->first == "张三"; ++it) {
        cout << "  " << it->second << endl;
    }

    // ========== equal_range：获取同key的范围（推荐方式） ==========
    // equal_range返回pair<iterator, iterator>
    // first: 第一个 >= key 的位置（即第一个匹配的）
    // second: 第一个 > key 的位置（即最后一个匹配的下一个）
    cout << "\n用equal_range查找张三:" << endl;
    auto range = mm.equal_range("张三");
    for (auto it = range.first; it != range.second; ++it) {
        cout << "  " << it->first << ": " << it->second << endl;
    }

    // ========== equal_range 查找不存在的key ==========
    auto range2 = mm.equal_range("赵六");
    cout << "赵六是否存在: " << (range2.first != range2.second) << endl; // 0

    // ========== 删除操作 ==========
    // erase(key) 会删除所有匹配的key
    mm.insert({"张三", 100});
    cout << "删除前张三数量: " << mm.count("张三") << endl; // 4
    mm.erase("张三"); // 删除所有张三
    cout << "删除后张三数量: " << mm.count("张三") << endl; // 0

    // ========== 应用场景：一对多关系 ==========
    // 比如：一个老师教多个班级
    multimap<string, string> teacherClass;
    teacherClass.insert({"王老师", "一班"});
    teacherClass.insert({"王老师", "二班"});
    teacherClass.insert({"李老师", "三班"});
    teacherClass.insert({"李老师", "四班"});
    teacherClass.insert({"王老师", "五班"});

    cout << "\n教师任课表:" << endl;
    // 获取所有不重复的教师名
    string lastTeacher;
    for (const auto& p : teacherClass) {
        if (p.first != lastTeacher) {
            // 打印该老师的所有班级
            cout << p.first << ": ";
            auto r = teacherClass.equal_range(p.first);
            for (auto it = r.first; it != r.second; ++it) {
                cout << it->second << " ";
            }
            cout << endl;
            lastTeacher = p.first;
        }
    }

    return 0;
}
```

### map vs multimap 对比

| 特性 | map | multimap |
|------|-----|---------|
| 重复key | **不允许** | **允许** |
| operator[] | **支持** | **不支持** |
| at() | 支持 | 不支持 |
| insert返回值 | `pair<iterator, bool>` | `iterator` |
| count返回值 | 0 或 1 | 0 到 n |
| find | 返回第一个匹配 | 返回第一个匹配 |
| equal_range | 区间最多1个 | 区间可多个 |
| erase(key) | 删0或1个 | 删除所有匹配 |

## 十、综合实战：统计字符出现次数并排序

```cpp
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string text = "hello world this is a test hello world";

    // 用map统计词频
    map<string, int> freq;
    string word;
    for (char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                freq[word]++;
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) freq[word]++;

    // map本身按key排序，如果想按频率排序，需要放到vector中
    vector<pair<string, int>> vec(freq.begin(), freq.end());
    sort(vec.begin(), vec.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second; // 按频率降序
         });

    cout << "词频排名:" << endl;
    int rank = 1;
    for (const auto& p : vec) {
        cout << "  " << rank++ << ". " << p.first
             << " (" << p.second << "次)" << endl;
    }

    return 0;
}
```

## 十一、总结

| 要点 | 内容 |
|------|------|
| map特点 | 键值对、按key排序、key不重复 |
| 插入 | `insert(pair/make_pair/{})`、`m[key]=value` |
| 访问 | `[]`（不存在会自动创建）、`at()`（不存在抛异常）、`find` |
| [] 的本质 | 查找+不存在则插入默认值+返回引用 |
| 遍历 | 迭代器 `it->first`/`it->second`、范围for `p.first`/`p.second` |
| 排序规则 | 模板参数传仿函数 |
| 自定义key | 重载 `<` 或提供比较器 |
| 应用 | 词频统计、通讯录、缓存等 |
| multimap | 允许重复key、不能用[]、用equal_range遍历同key元素 |
