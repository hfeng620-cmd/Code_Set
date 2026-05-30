/*
 * 文件名: 74_deque容器详解.cpp
 * 描述: deque双端队列容器的详细用法
 *       包括构造、双端操作、随机访问、与vector的对比等
 * 编译: g++ -std=c++11 74_deque容器详解.cpp -o 74_deque容器详解
 */

#include <iostream>
#include <deque>
#include <algorithm>  // sort, for_each
#include <vector>
#include <string>

using namespace std;

// 打印deque内容的辅助函数
template<typename T>
void printDeque(const string& title, const deque<T>& d) {
    cout << title << " [size=" << d.size() << "]: ";
    for (const auto& elem : d) {
        cout << elem << " ";
    }
    cout << endl;
}

int main() {
    cout << "========== 一、什么是deque？ ==========" << endl;

    /*
     * deque (Double-Ended Queue) 双端队列
     *
     * 特点:
     * 1. 两端都可以高效地插入和删除元素（O(1)）
     * 2. 支持随机访问（O(1)，通过[]或at()）
     * 3. 内存结构：分段连续存储
     *    - deque由多个固定大小的数组块组成
     *    - 通过一个中控器（map）管理这些数组块
     *    - 对外表现为连续的线性空间
     *
     * 与vector的区别:
     * vector: 连续内存，尾部插入O(1)，头部插入O(n)
     * deque:  分段内存，两端插入都是O(1)，中间插入O(n)
     *
     * deque的适用场景:
     * - 需要在头部和尾部都频繁插入/删除的场景
     * - 作为stack和queue的底层容器
     * - 不需要连续内存但需要随机访问的场景
     */

    cout << "deque是双端队列，支持两端高效操作" << endl;

    cout << "\n========== 二、deque构造方式 ==========" << endl;

    // 方式1: 默认构造（空deque）
    deque<int> d1;
    printDeque("默认构造 d1", d1);

    // 方式2: 指定大小，元素默认初始化为0
    deque<int> d2(5);
    printDeque("指定大小 d2(5)", d2);

    // 方式3: 指定大小和初始值
    deque<int> d3(5, 100);
    printDeque("指定大小和值 d3(5,100)", d3);

    // 方式4: 使用初始化列表（C++11）
    deque<int> d4 = {1, 2, 3, 4, 5};
    printDeque("初始化列表 d4", d4);

    // 方式5: 拷贝构造
    deque<int> d5(d4);
    printDeque("拷贝构造 d5(d4)", d5);

    // 方式6: 使用迭代器范围构造
    deque<int> d6(d4.begin() + 1, d4.begin() + 4);
    printDeque("迭代器范围 d6[1,4)", d6);

    // 方式7: 从vector构造
    vector<int> vec = {10, 20, 30};
    deque<int> d7(vec.begin(), vec.end());
    printDeque("从vector构造 d7", d7);

    cout << "\n========== 三、双端插入和删除 ==========" << endl;

    deque<int> dq;

    // push_back: 尾部插入，O(1)
    dq.push_back(30);
    dq.push_back(40);
    dq.push_back(50);
    printDeque("push_back后", dq);

    // push_front: 头部插入，O(1)（这是deque相对于vector的优势！）
    dq.push_front(20);
    dq.push_front(10);
    printDeque("push_front后", dq);

    // pop_back: 尾部删除，O(1)
    dq.pop_back();
    printDeque("pop_back后", dq);

    // pop_front: 头部删除，O(1)（这也是deque相对于vector的优势！）
    dq.pop_front();
    printDeque("pop_front后", dq);

    // emplace_front / emplace_back: 原地构造（C++11）
    // 对于复杂类型，emplace比push更高效（避免拷贝）
    deque<string> sdq;
    sdq.emplace_back("Hello");   // 在尾部直接构造string
    sdq.emplace_front("World");  // 在头部直接构造string
    printDeque("emplace后", sdq);

    cout << "\n========== 四、随机访问 ==========" << endl;

    deque<int> access = {10, 20, 30, 40, 50};

    // 方式1: []运算符（不检查边界，O(1)）
    cout << "access[0] = " << access[0] << endl;
    cout << "access[2] = " << access[2] << endl;
    cout << "access[4] = " << access[4] << endl;

    // 方式2: at()成员函数（检查边界，O(1)）
    cout << "access.at(1) = " << access.at(1) << endl;
    cout << "access.at(3) = " << access.at(3) << endl;
    // access.at(10);  // 会抛出out_of_range异常

    // 方式3: front() 和 back()
    cout << "front() = " << access.front() << endl;
    cout << "back() = " << access.back() << endl;

    // 修改元素
    access[0] = 100;
    access.at(4) = 500;
    printDeque("修改后", access);

    cout << "\n========== 五、insert和erase ==========" << endl;

    deque<int> ops = {1, 2, 3, 4, 5};

    // insert: 在指定位置插入（O(n)，需要移动元素）
    ops.insert(ops.begin() + 2, 25);  // 在位置2插入25
    printDeque("insert(位置2, 25)", ops);

    // insert: 插入多个相同元素
    ops.insert(ops.begin(), 3, 99);  // 在开头插入3个99
    printDeque("insert(开头, 3个99)", ops);

    // erase: 删除指定位置的元素
    auto it = ops.erase(ops.begin() + 3);  // 删除位置3的元素
    printDeque("erase(位置3)", ops);
    cout << "erase返回迭代器指向: " << *it << endl;

    // erase: 删除范围
    ops.erase(ops.begin(), ops.begin() + 2);  // 删除前2个
    printDeque("erase(前2个)", ops);

    // clear: 清空所有元素
    ops.clear();
    printDeque("clear后", ops);

    cout << "\n========== 六、迭代器遍历 ==========" << endl;

    deque<int> iter_dq = {10, 20, 30, 40, 50};

    // 正向迭代器
    cout << "正向遍历: ";
    for (deque<int>::iterator it = iter_dq.begin(); it != iter_dq.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    // 反向迭代器
    cout << "反向遍历: ";
    for (deque<int>::reverse_iterator rit = iter_dq.rbegin(); rit != iter_dq.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << endl;

    // 范围for
    cout << "范围for: ";
    for (const auto& val : iter_dq) {
        cout << val << " ";
    }
    cout << endl;

    // auto迭代器
    cout << "auto迭代器: ";
    for (auto it = iter_dq.cbegin(); it != iter_dq.cend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "\n========== 七、deque容量操作 ==========" << endl;

    deque<int> cap = {1, 2, 3, 4, 5};

    cout << "size(): " << cap.size() << endl;
    cout << "max_size(): " << cap.max_size() << endl;
    cout << "empty(): " << (cap.empty() ? "空" : "非空") << endl;

    // resize: 调整大小
    cap.resize(8, 99);  // 扩大到8个，新元素用99填充
    printDeque("resize(8, 99)", cap);

    cap.resize(3);  // 缩小到3个
    printDeque("resize(3)", cap);

    // 注意: deque没有capacity()和reserve()！
    // 因为deque的内存是分段的，不需要像vector那样预分配连续空间
    // cap.capacity();  // 错误！deque没有capacity()
    // cap.reserve(100);  // 错误！deque没有reserve()

    cout << "\n========== 八、deque排序和算法 ==========" << endl;

    deque<int> sort_dq = {5, 3, 1, 4, 2, 8, 6, 7};

    // sort: 排序（deque支持随机访问迭代器，可以用std::sort）
    sort(sort_dq.begin(), sort_dq.end());
    printDeque("升序排序", sort_dq);

    // 降序排序
    sort(sort_dq.begin(), sort_dq.end(), greater<int>());
    printDeque("降序排序", sort_dq);

    // reverse: 反转
    reverse(sort_dq.begin(), sort_dq.end());
    printDeque("反转后", sort_dq);

    // find: 查找
    auto found = find(sort_dq.begin(), sort_dq.end(), 4);
    if (found != sort_dq.end()) {
        cout << "找到元素4" << endl;
    }

    cout << "\n========== 九、deque vs vector 对比 ==========" << endl;

    /*
     * +------------------+---------------------------+---------------------------+
     * | 特性             | vector                    | deque                     |
     * +------------------+---------------------------+---------------------------+
     * | 内存结构         | 连续内存                  | 分段连续内存              |
     * | 尾部插入/删除    | O(1)摊还                  | O(1)                      |
     * | 头部插入/删除    | O(n)                      | O(1)                      |
     * | 中间插入/删除    | O(n)                      | O(n)                      |
     * | 随机访问         | O(1)                      | O(1)                      |
     * | 迭代器失效       | 重新分配时全部失效        | 插入/删除只影响部分       |
     * | cache友好性      | 高（连续内存）            | 较低（分段内存）          |
     * | capacity/reserve | 支持                      | 不支持                    |
     * | 数据局部性       | 好                        | 较差                      |
     * +------------------+---------------------------+---------------------------+
     *
     * 选择建议:
     * - 需要频繁在头部操作: 选择deque
     * - 需要最佳随机访问性能: 选择vector（cache友好）
     * - 不确定大小但主要在尾部操作: 选择vector
     * - 需要两端操作: 选择deque
     * - 作为stack/queue底层: 选择deque（默认）
     */

    cout << "\n========== 十、deque的内存结构说明 ==========" << endl;

    /*
     * deque的内存结构（分段连续）:
     *
     * 中控器(map): 存储指向各个数组块的指针
     * +------+------+------+------+------+
     * |  指针 |  指针 |  指针 |  指针 |  指针 |
     * +--+---+--+---+--+---+--+---+--+---+
     *    |       |       |       |       |
     *    v       v       v       v       v
     * +------+ +------+ +------+ +------+ +------+
     * |数据块| |数据块| |数据块| |数据块| |数据块|
     * +------+ +------+ +------+ +------+ +------+
     *
     * 优点:
     * - 头尾插入不需要移动所有元素
     * - 不需要大块连续内存
     *
     * 缺点:
     * - 随机访问需要两次指针解引用（比vector慢）
     * - cache局部性不如vector
     */

    cout << "\n========== 十一、综合示例：滑动窗口最大值 ==========" << endl;

    // 使用deque解决滑动窗口问题（经典应用）
    deque<int> window;  // 存储索引，维护单调递减队列
    vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;  // 窗口大小

    cout << "数组: ";
    for (auto n : nums) cout << n << " ";
    cout << endl;
    cout << "窗口大小: " << k << endl;
    cout << "滑动窗口最大值: ";

    for (int i = 0; i < nums.size(); ++i) {
        // 移除超出窗口的元素
        while (!window.empty() && window.front() <= i - k) {
            window.pop_front();
        }
        // 维护单调递减队列
        while (!window.empty() && nums[window.back()] < nums[i]) {
            window.pop_back();
        }
        window.push_back(i);
        // 输出窗口最大值
        if (i >= k - 1) {
            cout << nums[window.front()] << " ";
        }
    }
    cout << endl;

    cout << "\n========== 十二、常见错误和注意事项 ==========" << endl;

    /*
     * 常见错误1: 对deque使用capacity()和reserve()
     *   deque没有这两个成员函数！这是vector特有的
     *
     * 常见错误2: 认为deque和vector完全一样
     *   虽然接口相似，但性能特性不同
     *   deque的随机访问比vector稍慢（两次解引用）
     *
     * 常见错误3: 在需要cache友好的场景使用deque
     *   如果需要大量遍历和随机访问，vector通常更好
     *
     * 常见错误4: 迭代器失效
     *   deque的insert和erase可能导致迭代器失效
     *   但比vector好：只有受影响的迭代器会失效
     *
     * 常见错误5: 使用std::sort对deque排序
     *   这是可行的！deque支持随机访问迭代器
     *   但list不行，list有自己的sort()成员函数
     */

    cout << "deque容器详解完毕！" << endl;

    return 0;
}
