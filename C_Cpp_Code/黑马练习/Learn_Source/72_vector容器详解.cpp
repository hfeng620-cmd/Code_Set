/*
 * 文件名: 72_vector容器详解.cpp
 * 描述: vector动态数组容器的详细用法
 *       包括构造、访问、容量管理、迭代器、常见陷阱等
 * 编译: g++ -std=c++11 72_vector容器详解.cpp -o 72_vector容器详解
 */

#include <iostream>
#include <vector>
#include <algorithm>  // sort, find, for_each
#include <stdexcept>  // out_of_range异常

using namespace std;

// ==================== 辅助函数 ====================

// 打印vector内容的模板函数
template<typename T>
void printVector(const string& title, const vector<T>& v) {
    cout << title << " [size=" << v.size() << ", capacity=" << v.capacity() << "]: ";
    for (const auto& elem : v) {
        cout << elem << " ";
    }
    cout << endl;
}

// ==================== 主函数 ====================

int main() {
    cout << "========== 一、vector构造方式 ==========" << endl;

    // 方式1: 默认构造（空容器）
    vector<int> v1;
    printVector("默认构造 v1", v1);

    // 方式2: 指定大小，元素默认初始化为0
    vector<int> v2(5);
    printVector("指定大小 v2(5)", v2);

    // 方式3: 指定大小和初始值
    vector<int> v3(5, 100);
    printVector("指定大小和值 v3(5,100)", v3);

    // 方式4: 使用初始化列表（C++11）
    vector<int> v4 = {1, 2, 3, 4, 5};
    printVector("初始化列表 v4", v4);

    // 方式5: 拷贝构造
    vector<int> v5(v4);
    printVector("拷贝构造 v5(v4)", v5);

    // 方式6: 使用迭代器范围构造
    vector<int> v6(v4.begin() + 1, v4.begin() + 4);
    printVector("迭代器范围 v6[1,4)", v6);

    // 方式7: 从数组构造
    int arr[] = {10, 20, 30, 40, 50};
    vector<int> v7(arr, arr + 5);  // arr是首地址，arr+5是尾后地址
    printVector("从数组构造 v7", v7);

    cout << "\n========== 二、添加和删除元素 ==========" << endl;

    vector<int> vec;

    // push_back: 在尾部添加元素，O(1)摊还时间
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    vec.push_back(50);
    printVector("push_back后", vec);

    // pop_back: 删除尾部元素，O(1)
    // 注意：pop_back不返回被删除的元素值！
    vec.pop_back();
    printVector("pop_back后", vec);

    // insert: 在指定位置插入元素，O(n)
    // 参数：迭代器位置, 要插入的值
    vec.insert(vec.begin() + 2, 25);  // 在索引2处插入25
    printVector("insert(位置2, 25)", vec);

    // insert: 在指定位置插入多个相同元素
    vec.insert(vec.begin(), 3, 5);  // 在开头插入3个5
    printVector("insert(开头, 3个5)", vec);

    // erase: 删除指定位置的元素，O(n)
    // 注意：erase返回下一个有效迭代器
    auto it = vec.erase(vec.begin() + 3);  // 删除索引3的元素
    printVector("erase(位置3)", vec);
    cout << "erase返回的迭代器指向: " << *it << endl;

    // erase: 删除迭代器范围内的元素
    vec.erase(vec.begin(), vec.begin() + 2);  // 删除前2个元素
    printVector("erase(前2个)", vec);

    // clear: 清空所有元素，但capacity不变
    vec.clear();
    printVector("clear后", vec);

    cout << "\n========== 三、元素访问 ==========" << endl;

    vector<int> data = {10, 20, 30, 40, 50};

    // 方式1: []运算符访问（不检查边界，越界是未定义行为）
    cout << "data[0] = " << data[0] << endl;
    cout << "data[2] = " << data[2] << endl;

    // 方式2: at()成员函数（检查边界，越界抛出out_of_range异常）
    cout << "data.at(3) = " << data.at(3) << endl;
    // data.at(10);  // 会抛出 std::out_of_range 异常！

    // 方式3: front() 和 back()
    cout << "front() = " << data.front() << " (第一个元素)" << endl;
    cout << "back() = " << data.back() << " (最后一个元素)" << endl;

    // 方式4: data() 返回底层数组指针（C++11）
    int* ptr = data.data();
    cout << "底层数组首元素: " << *ptr << endl;

    // 演示at()的异常处理
    cout << "\nat()边界检查演示:" << endl;
    try {
        cout << data.at(10) << endl;  // 越界访问
    } catch (const out_of_range& e) {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== 四、容量管理 ==========" << endl;

    /*
     * size():     当前元素个数
     * capacity(): 当前分配的存储空间能容纳的元素个数
     * empty():    判断容器是否为空
     *
     * 重要: capacity >= size，当size超过capacity时，vector会重新分配内存
     *       重新分配会导致所有迭代器、指针、引用失效！
     */

    vector<int> cap;
    cout << "初始状态 - size: " << cap.size()
         << ", capacity: " << cap.capacity() << endl;

    // 观察capacity的增长规律
    for (int i = 0; i < 20; ++i) {
        cap.push_back(i);
        cout << "push_back(" << i << ") - size: " << cap.size()
             << ", capacity: " << cap.capacity() << endl;
    }

    cout << "\n--- reserve: 预分配空间 ---" << endl;
    vector<int> res;
    res.reserve(100);  // 预分配100个元素的空间，但size仍为0
    cout << "reserve(100)后 - size: " << res.size()
         << ", capacity: " << res.capacity() << endl;
    // 好处: 如果知道大致需要多少元素，reserve可以避免多次重新分配

    cout << "\n--- resize: 调整大小 ---" << endl;
    vector<int> rsz = {1, 2, 3, 4, 5};
    printVector("resize前", rsz);

    // resize变大：新元素默认初始化为0
    rsz.resize(8);
    printVector("resize(8)后", rsz);

    // resize变大并指定填充值
    rsz.resize(12, 99);
    printVector("resize(12, 99)后", rsz);

    // resize变小：删除多余的元素
    rsz.resize(4);
    printVector("resize(4)后", rsz);

    cout << "\n--- shrink_to_fit: 释放多余内存 ---" << endl;
    vector<int> shrink = {1, 2, 3};
    shrink.reserve(100);
    cout << "reserve(100)后 - size: " << shrink.size()
         << ", capacity: " << shrink.capacity() << endl;
    shrink.shrink_to_fit();  // 请求释放多余的内存
    cout << "shrink_to_fit后 - size: " << shrink.size()
         << ", capacity: " << shrink.capacity() << endl;

    cout << "\n========== 五、迭代器 ==========" << endl;

    vector<int> iter_vec = {10, 20, 30, 40, 50};

    // begin(): 指向第一个元素
    // end():   指向最后一个元素的下一个位置（尾后迭代器）
    cout << "正向遍历: ";
    for (vector<int>::iterator it = iter_vec.begin(); it != iter_vec.end(); ++it) {
        cout << *it << " ";  // 迭代器解引用获取值
    }
    cout << endl;

    // rbegin() / rend(): 反向迭代器
    cout << "反向遍历: ";
    for (vector<int>::reverse_iterator rit = iter_vec.rbegin(); rit != iter_vec.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << endl;

    // C++11: cbegin() / cend() 返回const迭代器（不能修改元素）
    cout << "const遍历: ";
    for (auto cit = iter_vec.cbegin(); cit != iter_vec.cend(); ++cit) {
        cout << *cit << " ";
        // *cit = 100;  // 错误！const迭代器不能修改元素
    }
    cout << endl;

    cout << "\n========== 六、范围for循环（C++11） ==========" << endl;

    vector<int> range_vec = {1, 2, 3, 4, 5};

    // 基本用法：按值拷贝（不推荐，有拷贝开销）
    cout << "按值遍历: ";
    for (int val : range_vec) {
        cout << val << " ";
    }
    cout << endl;

    // 推荐用法：按const引用遍历（只读，无拷贝开销）
    cout << "const引用遍历: ";
    for (const int& val : range_vec) {
        cout << val << " ";
    }
    cout << endl;

    // 按引用遍历（可以修改元素）
    for (int& val : range_vec) {
        val *= 10;  // 每个元素乘以10
    }
    cout << "修改后: ";
    for (const auto& val : range_vec) {  // auto自动推导类型
        cout << val << " ";
    }
    cout << endl;

    cout << "\n========== 七、swap和赋值 ==========" << endl;

    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5, 6, 7, 8};

    cout << "交换前: ";
    printVector("a", a);
    printVector("b", b);

    // swap: 交换两个vector的内容（O(1)，只交换内部指针）
    a.swap(b);
    cout << "交换后: ";
    printVector("a", a);
    printVector("b", b);

    // assign: 重新赋值
    a.assign(3, 100);  // 3个100
    printVector("assign(3, 100)", a);

    a.assign({10, 20, 30, 40});  // 初始化列表赋值
    printVector("assign({10,20,30,40})", a);

    cout << "\n========== 八、常见陷阱和最佳实践 ==========" << endl;

    /*
     * 陷阱1: 迭代器失效
     * 在vector中插入或删除元素可能导致所有迭代器失效
     * 原因：vector连续存储，容量变化时需要重新分配内存
     */
    cout << "--- 陷阱1: 迭代器失效 ---" << endl;
    vector<int> trap1 = {1, 2, 3, 4, 5};
    // 错误写法:
    // for (auto it = trap1.begin(); it != trap1.end(); ++it) {
    //     if (*it == 3) trap1.erase(it);  // 错误！erase后it失效
    // }

    // 正确写法: 使用erase返回值更新迭代器
    for (auto it = trap1.begin(); it != trap1.end(); ) {
        if (*it == 3) {
            it = trap1.erase(it);  // 正确：erase返回下一个有效迭代器
        } else {
            ++it;
        }
    }
    printVector("删除3后", trap1);

    /*
     * 陷阱2: []不检查边界
     * vec[i]不会检查i是否越界，越界是未定义行为（可能不会崩溃但数据损坏）
     * 建议: 不确定索引是否有效时使用at()
     */
    cout << "\n--- 陷阱2: 边界检查 ---" << endl;
    vector<int> trap2 = {1, 2, 3};
    // trap2[10];      // 未定义行为！不安全
    // trap2.at(10);   // 抛出out_of_range异常，安全

    /*
     * 陷阱3: reserve vs resize 混淆
     * reserve(n): 只分配内存，不改变size（不影响元素个数）
     * resize(n):  改变size（会添加或删除元素）
     */
    cout << "\n--- 陷阱3: reserve vs resize ---" << endl;
    vector<int> trap3a, trap3b;
    trap3a.reserve(5);  // capacity=5, size=0
    trap3b.resize(5);   // capacity=5, size=5
    cout << "reserve(5)后: size=" << trap3a.size() << ", capacity=" << trap3a.capacity() << endl;
    cout << "resize(5)后:  size=" << trap3b.size() << ", capacity=" << trap3b.capacity() << endl;

    /*
     * 最佳实践:
     * 1. 如果知道大致元素数量，先reserve()避免多次重新分配
     * 2. 需要边界检查时使用at()而非[]
     * 3. 删除元素时使用erase返回的迭代器
     * 4. 只读遍历使用const引用：for (const auto& x : vec)
     * 5. 使用shrink_to_fit()释放多余内存
     */

    cout << "\n========== 九、综合示例：vector排序和查找 ==========" << endl;

    vector<int> scores = {85, 92, 78, 95, 88, 76, 90, 83};

    // 排序（默认升序）
    sort(scores.begin(), scores.end());
    printVector("升序排序", scores);

    // 降序排序
    sort(scores.begin(), scores.end(), greater<int>());
    printVector("降序排序", scores);

    // 查找
    auto found = find(scores.begin(), scores.end(), 88);
    if (found != scores.end()) {
        cout << "找到88，位于索引: " << distance(scores.begin(), found) << endl;
    }

    // 二分查找（要求已排序）
    sort(scores.begin(), scores.end());  // 先升序排列
    bool exists = binary_search(scores.begin(), scores.end(), 88);
    cout << "二分查找88: " << (exists ? "找到" : "未找到") << endl;

    // lower_bound / upper_bound（要求已排序）
    auto lb = lower_bound(scores.begin(), scores.end(), 85);  // >= 85的第一个位置
    auto ub = upper_bound(scores.begin(), scores.end(), 90);  // > 90的第一个位置
    cout << ">=85的元素范围: ";
    for (auto it = lb; it != ub; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "\nvector容器详解完毕！" << endl;

    return 0;
}
