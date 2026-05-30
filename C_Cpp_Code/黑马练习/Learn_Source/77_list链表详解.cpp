/*
 ============================================================================
 文件名 : 77_list链表详解.cpp
 描  述 : 详细讲解C++ STL list 双向链表容器的用法
          包括：构造、push_front/push_back、insert迭代器插入( O(1) )、
          remove/erase删除、sort()排序(不能用std::sort)、
          reverse/unique/merge/splice、list vs vector 对比
 作  者 : 黑马程序员 C++教程
 日  期 : 2026-05-29
 编  译 : g++ -std=c++11 77_list链表详解.cpp -o 77_list链表详解
 ============================================================================
*/

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>  // greater<>
using namespace std;

// 打印 list 内容的辅助函数
template<typename T>
void printList(const string& title, const list<T>& lst) {
    cout << title << " [size=" << lst.size() << "]: ";
    for (const auto& elem : lst) {
        cout << elem << " ";
    }
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "       list 链表详解 演示" << endl;
    cout << "========================================" << endl;

    // ==================== 1. list 基础介绍 ====================
    /*
     * list 是 STL 中的双向链表容器
     *
     * 特点:
     *   1. 双向链表：每个节点包含数据和前后指针
     *   2. 插入/删除在任意位置都是 O(1)（已有迭代器时）
     *   3. 不支持随机访问（不能用 [] 或 at()）
     *   4. 插入和删除不会导致其他迭代器失效
     *   5. 每个元素额外存储两个指针（prev 和 next）
     *
     * 节点结构:
     *   +--------+--------+--------+
     *   | prev指针 | 数据  | next指针 |
     *   +--------+--------+--------+
     *
     * 链表结构:
     *   nullptr <- [A] <-> [B] <-> [C] <-> [D] -> nullptr
     */

    // ==================== 2. 构造方式 ====================
    cout << "\n--- 1. list 构造方式 ---" << endl;

    list<int> l1;                       // 默认构造（空链表）
    list<int> l2(5);                    // 5个元素，默认值0
    list<int> l3(5, 100);              // 5个元素，值为100
    list<int> l4 = {1, 2, 3, 4, 5};   // 初始化列表（C++11）
    list<int> l5(l4);                  // 拷贝构造
    list<int> l6(l4.begin(), l4.end()); // 迭代器范围

    printList("l4 初始化列表", l4);
    printList("l5 拷贝构造", l5);

    // ==================== 3. push_front / push_back ====================
    cout << "\n--- 2. 双端插入 ---" << endl;
    list<int> lst;
    lst.push_back(30);   // 尾部插入 O(1)
    lst.push_back(40);
    lst.push_back(50);
    printList("push_back后", lst);

    lst.push_front(20);  // 头部插入 O(1)  <-- list优势！vector头部插入是O(n)
    lst.push_front(10);
    printList("push_front后", lst);

    // pop_back / pop_front
    lst.pop_back();      // 删除尾部 O(1)
    printList("pop_back后", lst);
    lst.pop_front();     // 删除头部 O(1)
    printList("pop_front后", lst);

    // ==================== 4. insert 迭代器插入 ====================
    cout << "\n--- 3. insert 迭代器插入 (核心优势) ---" << endl;
    /*
     * list 的 insert 是 O(1) 操作（已有迭代器的情况下）
     * 这是 list 相对于 vector 的核心优势
     * vector 的 insert 需要移动后续所有元素，是 O(n)
     */
    list<int> insert_lst = {1, 2, 3, 4, 5};
    auto it = insert_lst.begin();
    advance(it, 2);  // 移动到第3个元素位置
    insert_lst.insert(it, 25);  // 在位置前插入，O(1)
    printList("在位置2前插入25", insert_lst);

    // 插入多个相同值
    it = insert_lst.begin();
    advance(it, 1);
    insert_lst.insert(it, 3, 99);  // 插入3个99
    printList("在位置1前插入3个99", insert_lst);

    // ==================== 5. erase 和 remove 删除 ====================
    cout << "\n--- 4. 删除操作 ---" << endl;
    list<int> erase_lst = {1, 2, 3, 4, 5, 3, 6, 3, 7};

    // erase(iterator): 删除指定位置，O(1)，返回下一个迭代器
    auto eit = erase_lst.begin();
    advance(eit, 2);
    eit = erase_lst.erase(eit);
    printList("erase位置2后", erase_lst);

    // remove(val): 删除所有等于 val 的元素，O(n)
    erase_lst.remove(3);
    printList("remove(3)后", erase_lst);

    // remove_if: 按条件删除
    erase_lst.remove_if([](int x) { return x > 5; });
    printList("remove_if(>5)后", erase_lst);

    // ==================== 6. sort 排序 ====================
    cout << "\n--- 5. sort 排序 (重要!) ---" << endl;
    /*
     * 重要：list 不能使用 std::sort()！
     * 因为 std::sort 需要随机访问迭代器，而 list 只有双向迭代器
     * 必须使用 list 自己的成员函数 sort()
     *
     * list.sort() 使用归并排序，O(n log n)，稳定排序
     */
    list<int> sort_lst = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    printList("排序前", sort_lst);

    sort_lst.sort();  // 升序排序
    printList("升序排序后", sort_lst);

    sort_lst.sort(greater<int>());  // 降序排序
    printList("降序排序后", sort_lst);

    // 自定义比较排序
    list<string> str_lst = {"banana", "apple", "cherry", "date"};
    printList("字符串排序前", str_lst);
    str_lst.sort([](const string& a, const string& b) {
        return a.length() < b.length();  // 按长度排序
    });
    printList("按长度排序后", str_lst);

    // 常见错误：sort(sort_lst.begin(), sort_lst.end());  // 编译错误!

    // ==================== 7. reverse ====================
    cout << "\n--- 6. reverse 反转 ---" << endl;
    list<int> rev_lst = {1, 2, 3, 4, 5};
    printList("反转前", rev_lst);
    rev_lst.reverse();
    printList("反转后", rev_lst);

    // ==================== 8. unique ====================
    cout << "\n--- 7. unique 去重 ---" << endl;
    /*
     * unique: 删除连续重复的元素
     * 注意：只删除连续的重复！如果需要删除所有重复，先排序再去重
     */
    list<int> uni_lst = {1, 1, 2, 2, 2, 3, 3, 1, 1, 4, 4};
    printList("去重前", uni_lst);
    uni_lst.unique();  // 只删除连续重复
    printList("直接unique后", uni_lst);

    // 先排序再去重（删除所有重复）
    uni_lst = {1, 1, 2, 2, 2, 3, 3, 1, 1, 4, 4};
    uni_lst.sort();
    uni_lst.unique();
    printList("排序后unique", uni_lst);

    // ==================== 9. merge ====================
    cout << "\n--- 8. merge 合并 ---" << endl;
    /*
     * merge: 合并两个已排序的链表，结果仍有序
     * 要求：两个链表必须已按相同规则排序
     * 效果：合并后另一个链表变为空
     */
    list<int> merge_a = {1, 3, 5, 7, 9};
    list<int> merge_b = {2, 4, 6, 8, 10};
    printList("merge_a", merge_a);
    printList("merge_b", merge_b);
    merge_a.merge(merge_b);
    printList("merge后merge_a", merge_a);
    printList("merge后merge_b(为空)", merge_b);

    // ==================== 10. splice ====================
    cout << "\n--- 9. splice 拼接 (list独有) ---" << endl;
    /*
     * splice: 将一个链表的元素转移到另一个链表
     * 这是 list 独有的高效操作！O(1)
     */
    list<int> sp_a = {1, 2, 3};
    list<int> sp_b = {4, 5, 6};
    printList("splice前sp_a", sp_a);
    printList("splice前sp_b", sp_b);

    sp_a.splice(sp_a.end(), sp_b);  // 将sp_b全部移到sp_a末尾
    printList("splice后sp_a", sp_a);
    printList("splice后sp_b(为空)", sp_b);

    // splice 单个元素
    list<int> sp_c = {10, 20, 30};
    auto splice_it = sp_c.begin();
    advance(splice_it, 1);  // 指向20
    sp_a.splice(sp_a.begin(), sp_c, splice_it);  // 将20移到sp_a开头
    printList("splice单个后sp_a", sp_a);
    printList("splice单个后sp_c", sp_c);

    // ==================== 11. 当使用 list vs vector ====================
    cout << "\n--- 10. list vs vector 对比 ---" << endl;
    /*
     * +------------------+-------------------------+-------------------------+
     * | 特性             | vector                  | list                    |
     * +------------------+-------------------------+-------------------------+
     * | 内存结构         | 连续内存                | 双向链表(非连续)        |
     * | 随机访问         | O(1)                    | O(n)，不支持[]          |
     * | 尾部插入/删除    | O(1)摊还                | O(1)                    |
     * | 头部插入/删除    | O(n)                    | O(1)                    |
     * | 中间插入/删除    | O(n)                    | O(1) (有迭代器时)       |
     * | 迭代器类型       | 随机访问迭代器          | 双向迭代器              |
     * | 迭代器失效       | 重新分配时全部失效      | 只有被删元素失效        |
     * | cache友好性      | 高                      | 低                      |
     * | 内存开销         | 低                      | 高(每节点两个指针)      |
     * | std::sort        | 支持                    | 不支持，用list.sort()   |
     * | splice           | 不支持                  | 支持，O(1)              |
     * +------------------+-------------------------+-------------------------+
     *
     * 选择建议:
     *   - 需要随机访问 => vector
     *   - 频繁在头部插入删除 => list
     *   - 频繁在中间插入删除（有迭代器）=> list
     *   - 需要 cache 友好 => vector
     *   - 需要 splice 操作 => list
     */

    // ==================== 12. 常见错误 ====================
    cout << "\n--- 11. 常见错误 ---" << endl;
    cout << "  错误1: 对list使用std::sort() => 编译错误" << endl;
    cout << "         应使用 list.sort() 成员函数" << endl;
    cout << "  错误2: 用[]或at()访问 => list不支持随机访问" << endl;
    cout << "  错误3: unique前忘记排序 => 只删除连续重复" << endl;
    cout << "  错误4: merge前忘记排序 => 结果不确定" << endl;
    cout << "  错误5: 忽视内存开销 => 每节点多两个指针" << endl;

    cout << "\n========================================" << endl;
    cout << "   list 链表详解完毕" << endl;
    cout << "========================================" << endl;

    return 0;
}
