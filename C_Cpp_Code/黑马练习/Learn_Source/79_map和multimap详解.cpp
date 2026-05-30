/*
 ============================================================================
 文件名 : 79_map和multimap详解.cpp
 描  述 : 详细讲解C++ STL map 和 multimap 关联容器的用法
          包括：map键值对容器、operator[]/insert/emplace、
          find/count/erase、遍历map、multimap重复键、
          make_pair/pair类型、自定义比较器、词频统计应用
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 79_map和multimap详解.cpp -o 79_map和multimap详解
 ============================================================================
*/

#include <iostream>
#include <map>
#include <string>
#include <functional>  // greater<>
#include <vector>
using namespace std;

// 打印 map 内容的辅助函数
template<typename K, typename V>
void printMap(const string& title, const map<K, V>& m) {
    cout << title << " [size=" << m.size() << "]: ";
    for (const auto& p : m) {
        cout << "{" << p.first << ":" << p.second << "} ";
    }
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "   map 和 multimap 详解 演示" << endl;
    cout << "========================================" << endl;

    // ==================== 1. map 基础介绍 ====================
    /*
     * map 是 STL 中的关联容器，存储键值对（key-value pairs）
     *
     * 特点:
     *   1. 键唯一：每个 key 只能出现一次
     *   2. 自动排序：按 key 升序排列（默认）
     *   3. 底层实现：红黑树
     *   4. 查找/插入/删除：O(log n)
     *   5. 支持 [] 运算符访问
     *
     * map vs unordered_map:
     *   map:           有序, O(log n), 红黑树
     *   unordered_map: 无序, O(1)摊还, 哈希表
     */

    // ==================== 2. map 构造和插入 ====================
    cout << "\n--- 1. map 构造和插入 ---" << endl;

    map<string, int> m1;

    // 方式1: operator[] 插入
    // 如果 key 不存在，会创建新元素（value 默认初始化）
    // 如果 key 已存在，会修改 value
    m1["Alice"] = 90;
    m1["Bob"] = 85;
    m1["Charlie"] = 95;
    printMap("[]插入 m1", m1);

    // 方式2: insert + pair
    m1.insert(pair<string, int>("David", 88));
    m1.insert(make_pair("Eve", 92));
    printMap("insert后 m1", m1);

    // 方式3: insert + 初始化列表（C++11）
    m1.insert({"Frank", 78});
    printMap("初始化列表insert", m1);

    // 方式4: emplace（C++11，原地构造，更高效）
    m1.emplace("Grace", 87);
    printMap("emplace后 m1", m1);

    // 初始化列表构造
    map<string, int> m2 = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
    printMap("初始化构造 m2", m2);

    // insert 的返回值
    cout << "\n--- insert 的返回值 ---" << endl;
    map<string, int> score;
    auto result1 = score.insert({"Alice", 90});
    cout << "插入Alice: " << (result1.second ? "成功" : "失败") << endl;

    auto result2 = score.insert({"Alice", 95});  // key已存在
    cout << "再次插入Alice: " << (result2.second ? "成功" : "失败") << endl;
    cout << "Alice的值: " << result2.first->second << endl;  // 仍然是90

    // insert vs [] 的重要区别
    cout << "\n--- insert vs [] 的区别 ---" << endl;
    map<string, int> demo;
    demo.insert({"key", 1});
    demo.insert({"key", 2});  // 不会修改，仍然是1
    cout << "insert后key的值: " << demo["key"] << endl;  // 1

    demo["key"] = 3;  // 会修改为3
    cout << "[]赋值后key的值: " << demo["key"] << endl;  // 3

    // ==================== 3. map 查找操作 ====================
    cout << "\n--- 2. map 查找操作 ---" << endl;
    map<string, int> find_map = {
        {"apple", 1}, {"banana", 2}, {"cherry", 3},
        {"date", 4}, {"elderberry", 5}
    };

    // 方式1: operator[]
    // 警告：如果 key 不存在，会自动创建一个值为0的元素！
    cout << "apple: " << find_map["apple"] << endl;
    // cout << "grape: " << find_map["grape"] << endl;  // 危险！会创建 grape=0

    // 方式2: at() —— 不存在时抛出 out_of_range 异常
    cout << "banana: " << find_map.at("banana") << endl;
    try {
        cout << find_map.at("grape") << endl;
    } catch (const out_of_range& e) {
        cout << "at()异常: " << e.what() << endl;
    }

    // 方式3: find() —— 推荐用于判断 key 是否存在
    auto it = find_map.find("cherry");
    if (it != find_map.end()) {
        cout << "find(cherry): " << it->second << endl;
    }

    it = find_map.find("grape");
    cout << "find(grape): " << (it != find_map.end() ? "找到" : "未找到") << endl;

    // 方式4: count() —— 返回0或1
    cout << "count(date): " << find_map.count("date") << endl;
    cout << "count(grape): " << find_map.count("grape") << endl;

    // ==================== 4. map 删除操作 ====================
    cout << "\n--- 3. map 删除操作 ---" << endl;
    map<string, int> del_map = {
        {"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}
    };
    printMap("删除前", del_map);

    del_map.erase("c");  // 按key删除
    printMap("erase(c)后", del_map);

    auto dit = del_map.find("e");
    if (dit != del_map.end()) del_map.erase(dit);  // 按迭代器删除
    printMap("erase(e)后", del_map);

    // ==================== 5. map 遍历 ====================
    cout << "\n--- 4. map 遍历 ---" << endl;
    map<string, int> iter_map = {
        {"Charlie", 95}, {"Alice", 90}, {"Bob", 85},
        {"David", 88}, {"Eve", 92}
    };

    // 正向遍历（按键升序）
    cout << "正向遍历(升序):" << endl;
    for (auto it = iter_map.begin(); it != iter_map.end(); ++it) {
        cout << "  " << it->first << " -> " << it->second << endl;
    }

    // 反向遍历（按键降序）
    cout << "反向遍历(降序):" << endl;
    for (auto rit = iter_map.rbegin(); rit != iter_map.rend(); ++rit) {
        cout << "  " << rit->first << " -> " << rit->second << endl;
    }

    // C++11 范围for
    cout << "范围for:" << endl;
    for (const auto& p : iter_map) {
        cout << "  " << p.first << " -> " << p.second << endl;
    }

    // ==================== 6. pair 类型 ====================
    cout << "\n--- 5. pair 类型 ---" << endl;
    pair<string, int> p1("Alice", 90);
    auto p2 = make_pair("Bob", 85);           // make_pair 自动推导
    pair<string, int> p3 = {"Charlie", 95};   // C++11 初始化列表

    cout << "p1: " << p1.first << " -> " << p1.second << endl;
    cout << "p2: " << p2.first << " -> " << p2.second << endl;
    cout << "p3: " << p3.first << " -> " << p3.second << endl;

    // pair 比较（先比 first，再比 second）
    cout << "p1 < p2: " << (p1 < p2 ? "true" : "false") << endl;

    // ==================== 7. 自定义比较器 ====================
    cout << "\n--- 6. 自定义比较器 ---" << endl;

    // 降序排列
    map<string, int, greater<string>> desc_map = {
        {"apple", 1}, {"banana", 2}, {"cherry", 3}
    };
    cout << "降序map: ";
    for (const auto& p : desc_map) cout << p.first << " ";
    cout << endl;

    // 按字符串长度排序
    struct LengthCompare {
        bool operator()(const string& a, const string& b) const {
            if (a.length() != b.length())
                return a.length() < b.length();
            return a < b;
        }
    };

    map<string, int, LengthCompare> len_map;
    len_map["banana"] = 2;
    len_map["apple"] = 1;
    len_map["hi"] = 10;
    len_map["cat"] = 3;
    len_map["a"] = 100;
    cout << "按长度排序: ";
    for (const auto& p : len_map)
        cout << p.first << "(" << p.first.length() << ") ";
    cout << endl;

    // ==================== 8. multimap 详解 ====================
    cout << "\n--- 7. multimap 详解 ---" << endl;
    /*
     * multimap 与 map 的区别:
     *   map:      key 唯一，重复 key 的 insert 会被忽略
     *   multimap: 允许重复 key
     *
     * 注意：multimap 不支持 [] 运算符！
     * 因为一个 key 可能对应多个 value，[] 无法确定返回哪个
     */
    multimap<string, int> mmap;
    mmap.insert({"Alice", 90});
    mmap.insert({"Bob", 85});
    mmap.insert({"Alice", 95});  // 重复key，会插入
    mmap.insert({"Alice", 88});  // 重复key，会插入
    mmap.insert({"Charlie", 92});

    cout << "multimap:" << endl;
    for (const auto& p : mmap) {
        cout << "  " << p.first << " -> " << p.second << endl;
    }

    // mmap["Alice"];  // 编译错误！multimap不支持[]

    // count: 返回 key 的出现次数
    cout << "Alice出现次数: " << mmap.count("Alice") << endl;

    // 遍历所有匹配元素
    cout << "所有Alice的成绩: ";
    auto range = mmap.equal_range("Alice");
    for (auto it = range.first; it != range.second; ++it) {
        cout << it->second << " ";
    }
    cout << endl;

    // 删除所有匹配的 key
    mmap.erase("Alice");
    cout << "删除所有Alice后:" << endl;
    for (const auto& p : mmap) {
        cout << "  " << p.first << " -> " << p.second << endl;
    }

    // ==================== 9. 应用示例：词频统计 ====================
    cout << "\n--- 8. 应用示例：词频统计 ---" << endl;
    vector<string> words = {
        "apple", "banana", "apple", "cherry",
        "banana", "apple", "date", "banana", "apple"
    };

    map<string, int> word_count;
    for (const auto& word : words) {
        word_count[word]++;  // 利用[]的特性：不存在则创建(0)，存在则修改
    }

    cout << "词频统计:" << endl;
    for (const auto& p : word_count) {
        cout << "  " << p.first << ": " << p.second << "次" << endl;
    }

    // ==================== 10. 常见错误 ====================
    cout << "\n--- 9. 常见错误 ---" << endl;
    cout << "  错误1: 用[]判断key是否存在 => 不存在时会自动创建!" << endl;
    cout << "         应使用 find() 或 count()" << endl;
    cout << "  错误2: 对multimap使用[] => 编译错误" << endl;
    cout << "  错误3: insert不会修改已存在的key => 用[]或insert_or_assign" << endl;
    cout << "  错误4: 修改map的key => key是const的，需要删除再插入" << endl;
    cout << "  错误5: 遍历时删除元素 => 使用 erase 返回的迭代器" << endl;

    // 遍历中安全删除示例
    cout << "\n【遍历中安全删除】" << endl;
    map<int, int> erase_map = {{1,10}, {2,20}, {3,30}, {4,40}, {5,50}};
    for (auto it = erase_map.begin(); it != erase_map.end(); ) {
        if (it->second > 20) {
            it = erase_map.erase(it);  // 返回下一个有效迭代器
        } else {
            ++it;
        }
    }
    cout << "删除>20后: ";
    for (const auto& p : erase_map)
        cout << "{" << p.first << ":" << p.second << "} ";
    cout << endl;

    cout << "\n========================================" << endl;
    cout << "   map 和 multimap 详解完毕" << endl;
    cout << "========================================" << endl;

    return 0;
}
