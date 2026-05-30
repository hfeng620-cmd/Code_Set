/*
 * 文件名: 76_queue队列详解.cpp
 * 描述: queue队列适配器的详细用法
 *       包括FIFO原理、基本操作、底层容器、应用实例等
 * 编译: g++ -std=c++11 76_queue队列详解.cpp -o 76_queue队列详解
 */

#include <iostream>
#include <queue>
#include <deque>
#include <list>
#include <vector>
#include <string>

using namespace std;

// 辅助函数：打印队列内容（使用副本，不影响原队列）
template<typename T>
void printQueue(const string& title, queue<T> q) {  // 按值传递
    cout << title << " [size=" << q.size() << "]: ";
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;
}

int main() {
    cout << "========== 一、什么是队列？ ==========" << endl;

    /*
     * 队列 (Queue) 是一种先进先出 (FIFO: First In, First Out) 的数据结构
     *
     * 想象排队买票:
     * - 新来的人排到队尾 (push)
     * - 队头的人先买完离开 (pop)
     * - 先来的人先服务
     *
     * 队列的操作限制:
     * - 队尾 (back):  只能添加元素
     * - 队头 (front): 只能移除和查看元素
     * - 不能随机访问中间元素
     * - 不能遍历
     *
     * 基本操作:
     * - push(x):  在队尾添加元素
     * - pop():    移除队头元素（不返回值！）
     * - front():  返回队头元素的引用
     * - back():   返回队尾元素的引用
     * - empty():  判断队列是否为空
     * - size():   返回队列中元素个数
     */

    cout << "queue是FIFO（先进先出）的数据结构" << endl;

    cout << "\n========== 二、queue基本操作 ==========" << endl;

    // 创建空队列
    queue<int> q;

    cout << "队列是否为空: " << (q.empty() ? "是" : "否") << endl;
    cout << "队列大小: " << q.size() << endl;

    // push: 在队尾添加元素
    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40);
    q.push(50);
    printQueue("push 10-50后", q);

    // front: 查看队头元素
    cout << "队头元素: " << q.front() << endl;

    // back: 查看队尾元素
    cout << "队尾元素: " << q.back() << endl;

    // pop: 移除队头元素（注意：pop不返回值！）
    q.pop();
    cout << "pop后队头: " << q.front() << endl;
    cout << "pop后大小: " << q.size() << endl;

    // 修改队头和队尾元素
    q.front() = 100;
    q.back() = 999;
    printQueue("修改队头队尾后", q);

    // emplace: 原地构造（C++11）
    queue<string> sq;
    sq.emplace("Hello");
    sq.emplace("World");
    printQueue("emplace后", sq);

    cout << "\n========== 三、queue的底层容器 ==========" << endl;

    /*
     * queue是容器适配器，其模板定义为:
     * template<class T, class Container = deque<T>> class queue;
     *
     * 可以选择的底层容器:
     * - deque<T> (默认): 双端队列，两端操作都高效
     * - list<T>:   双向链表，两端操作都高效
     *
     * 不能使用的底层容器:
     * - vector: 没有pop_front()，头部删除是O(n)
     * - array:  固定大小，无法动态增长
     * - forward_list: 没有push_back()和pop_front()同时支持
     */

    // 默认使用deque
    queue<int> q1;  // 等价于 queue<int, deque<int>>

    // 使用list作为底层容器
    queue<int, list<int>> q2;
    q2.push(10);
    q2.push(20);
    cout << "list队列队头: " << q2.front() << endl;

    // 以下代码会编译错误！vector不能作为queue的底层容器
    // queue<int, vector<int>> q3;  // 错误！vector没有pop_front

    cout << "\n========== 四、queue不支持迭代 ==========" << endl;

    /*
     * 重要: queue不提供迭代器！
     * 不能用for循环遍历队列
     * 只能访问队头(front)和队尾(back)
     *
     * 如需查看所有元素，只能复制后逐个pop
     */
    queue<int> iter_q;
    iter_q.push(1);
    iter_q.push(2);
    iter_q.push(3);

    // iter_q.begin();  // 错误！queue没有迭代器

    // 查看所有元素（使用副本）
    printQueue("查看队列内容", iter_q);
    cout << "原队列大小: " << iter_q.size() << " (未改变)" << endl;

    cout << "\n========== 五、应用1：BFS层序遍历 ==========" << endl;

    /*
     * BFS (Breadth-First Search) 广度优先搜索
     * 是队列的经典应用场景
     *
     * 以二叉树的层序遍历为例:
     *      1
     *     / \
     *    2   3
     *   / \   \
     *  4   5   6
     *
     * 层序遍历结果: 1, 2, 3, 4, 5, 6
     */

    // 简化的BFS演示（使用数组模拟）
    cout << "BFS层序遍历模拟:" << endl;

    // 假设有一个图的邻接表
    // 0 -> 1, 2
    // 1 -> 3, 4
    // 2 -> 5
    // 3 -> (无)
    // 4 -> (无)
    // 5 -> (无)

    vector<vector<int>> graph = {
        {1, 2},     // 节点0的邻居
        {3, 4},     // 节点1的邻居
        {5},        // 节点2的邻居
        {},         // 节点3的邻居
        {},         // 节点4的邻居
        {}          // 节点5的邻居
    };

    queue<int> bfs_queue;
    vector<bool> visited(6, false);

    // 从节点0开始BFS
    bfs_queue.push(0);
    visited[0] = true;

    cout << "BFS顺序: ";
    while (!bfs_queue.empty()) {
        int node = bfs_queue.front();
        bfs_queue.pop();
        cout << node << " ";

        // 将未访问的邻居加入队列
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                bfs_queue.push(neighbor);
            }
        }
    }
    cout << endl;

    cout << "\n========== 六、应用2：消息队列模拟 ==========" << endl;

    // 模拟简单的消息队列系统
    struct Message {
        string sender;
        string content;
        int priority;

        // 用于打印
        string toString() const {
            return "[" + sender + "]: " + content + " (优先级:" + to_string(priority) + ")";
        }
    };

    queue<Message> msg_queue;

    // 生产者：添加消息
    msg_queue.push({"系统", "服务器启动成功", 1});
    msg_queue.push({"用户A", "登录请求", 2});
    msg_queue.push({"用户B", "数据查询", 3});
    msg_queue.push({"系统", "备份完成", 1});
    msg_queue.push({"用户C", "文件上传", 2});

    cout << "消息队列中有 " << msg_queue.size() << " 条消息" << endl;

    // 消费者：处理消息（先进先出）
    cout << "\n按顺序处理消息:" << endl;
    int count = 0;
    while (!msg_queue.empty()) {
        Message msg = msg_queue.front();
        msg_queue.pop();
        cout << "处理消息 " << ++count << ": " << msg.toString() << endl;
    }

    cout << "\n========== 七、应用3：打印任务队列 ==========" << endl;

    // 模拟打印机任务队列
    queue<string> print_jobs;

    // 添加打印任务
    print_jobs.push("文档A - 10页");
    print_jobs.push("文档B - 5页");
    print_jobs.push("文档C - 20页");
    print_jobs.push("文档D - 3页");

    cout << "打印队列中有 " << print_jobs.size() << " 个任务" << endl;
    cout << "下一个任务: " << print_jobs.front() << endl;

    // 处理打印任务
    cout << "\n开始处理打印任务:" << endl;
    while (!print_jobs.empty()) {
        cout << "正在打印: " << print_jobs.front() << endl;
        print_jobs.pop();

        if (!print_jobs.empty()) {
            cout << "剩余 " << print_jobs.size() << " 个任务" << endl;
        }
    }
    cout << "所有打印任务完成！" << endl;

    cout << "\n========== 八、应用4：约瑟夫环问题 ==========" << endl;

    // 约瑟夫环：n个人围成一圈，从第1个人开始报数，报到m的人出列
    // 求最后剩下的人的编号

    auto josephus = [](int n, int m) -> int {
        queue<int> q;

        // 将所有人加入队列
        for (int i = 1; i <= n; ++i) {
            q.push(i);
        }

        // 模拟报数过程
        while (q.size() > 1) {
            // 前m-1个人移到队尾
            for (int i = 0; i < m - 1; ++i) {
                q.push(q.front());
                q.pop();
            }
            // 第m个人出列
            cout << "出列: " << q.front() << " ";
            q.pop();
        }

        cout << endl;
        return q.front();  // 最后剩下的人
    };

    int n = 7, m = 3;
    cout << "约瑟夫环: n=" << n << ", m=" << m << endl;
    int survivor = josephus(n, m);
    cout << "最后剩下的人: " << survivor << endl;

    cout << "\n========== 九、queue比较操作 ==========" << endl;

    queue<int> qa, qb;
    qa.push(1); qa.push(2); qa.push(3);
    qb.push(1); qb.push(2); qb.push(4);

    cout << "qa == qb: " << (qa == qb ? "true" : "false") << endl;
    cout << "qa < qb:  " << (qa < qb ? "true" : "false") << endl;
    cout << "qa > qb:  " << (qa > qb ? "true" : "false") << endl;

    cout << "\n========== 十、queue vs stack 对比 ==========" << endl;

    /*
     * +------------------+---------------------------+---------------------------+
     * | 特性             | stack (栈)                | queue (队列)              |
     * +------------------+---------------------------+---------------------------+
     * | 数据结构         | LIFO (后进先出)           | FIFO (先进先出)           |
     * | 添加元素         | push (栈顶)               | push (队尾)               |
     * | 移除元素         | pop (栈顶)                | pop (队头)                |
     * | 访问元素         | top (栈顶)                | front (队头), back (队尾) |
     * | 迭代器           | 无                        | 无                        |
     * | 默认底层容器     | deque                     | deque                     |
     * | vector作底层     | 可以                      | 不可以 (无pop_front)      |
     * | list作底层       | 可以                      | 可以                      |
     * +------------------+---------------------------+---------------------------+
     */

    cout << "\n========== 十一、常见错误和注意事项 ==========" << endl;

    /*
     * 常见错误1: 对空队列调用front()/back()/pop()
     *   这是未定义行为！
     *   正确做法: 先检查empty()
     */
    queue<int> empty_q;
    // empty_q.front();  // 未定义行为！
    // empty_q.pop();    // 未定义行为！

    if (!empty_q.empty()) {
        cout << "队头: " << empty_q.front() << endl;
    } else {
        cout << "队列为空，不能访问" << endl;
    }

    /*
     * 常见错误2: 期望pop()返回值
     *   pop()只移除队头元素，不返回值
     *   要获取队头值，先用front()再用pop()
     *
     *   错误: int val = q.pop();
     *   正确: int val = q.front(); q.pop();
     */

    /*
     * 常见错误3: 用vector作为queue的底层容器
     *   vector没有pop_front()，不能作为queue的底层
     *   只能用deque或list
     */

    /*
     * 常见错误4: 试图遍历队列
     *   queue没有迭代器，只能访问front和back
     *   如需遍历，只能复制后逐个pop
     */

    /*
     * 常见错误5: 混淆queue和deque
     *   queue是容器适配器，只能在一端添加、另一端删除
     *   deque是完整容器，两端都可以添加和删除，支持随机访问
     */

    cout << "queue队列详解完毕！" << endl;

    return 0;
}
