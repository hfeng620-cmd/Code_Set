# queue 队列详解

## 一、队列的概念

队列（Queue）是一种 **先进先出**（FIFO, First In First Out）的数据结构。
可以想象成排队买票：先来的人先买到票离开。

```
队列的操作示意：

  push(1)  push(2)  push(3)    pop()      pop()
+---+    +---+---+    +---+---+---+    +---+---+    +---+
| 1 |    | 1 | 2 |    | 1 | 2 | 3 |    | 2 | 3 |    | 3 |
+---+    +---+---+    +---+---+---+    +---+---+    +---+
 队尾      队尾          队尾 队首       队尾 队首      队尾
                                      |出队
```

**核心特点：**
- 从 **队尾** 插入（push）
- 从 **队首** 删除（pop）
- 先进先出（FIFO）
- 不支持随机访问，不能遍历

```cpp
#include <queue>        // queue 头文件
using namespace std;
```

---

## 二、queue 的构造

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    // 默认构造（底层容器为 deque）
    queue<int> q;

    cout << "队列是否为空：" << q.empty() << endl;   // 1 (true)
    cout << "队列大小：" << q.size() << endl;        // 0

    return 0;
}
```

---

## 三、queue 的基本操作

### 3.1 push 和 pop

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> q;

    // push：入队（从队尾插入）
    q.push(10);
    q.push(20);
    q.push(30);

    cout << "大小：" << q.size() << endl;           // 3

    // front：查看队首元素
    cout << "队首：" << q.front() << endl;          // 10

    // back：查看队尾元素
    cout << "队尾：" << q.back() << endl;           // 30

    // pop：出队（从队首删除）
    q.pop();    // 弹出 10
    cout << "pop 后队首：" << q.front() << endl;    // 20
    cout << "pop 后大小：" << q.size() << endl;     // 2

    q.pop();    // 弹出 20
    q.pop();    // 弹出 30

    cout << "全部出队后是否为空：" << q.empty() << endl;  // 1 (true)

    return 0;
}
```

### 3.2 front 和 back

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

int main() {
    queue<string> q;

    q.push("张三");
    q.push("李四");
    q.push("王五");

    // front：队首（最先入队的）
    cout << "队首：" << q.front() << endl;  // 张三

    // back：队尾（最后入队的）
    cout << "队尾：" << q.back() << endl;   // 王五

    // front 和 back 只是查看，不会删除
    cout << "再次查看队首：" << q.front() << endl;  // 张三

    // 也可以修改
    q.front() = "修改后的张三";
    cout << "修改后队首：" << q.front() << endl;    // 修改后的张三

    return 0;
}
```

### 3.3 empty 和 size

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> q;

    cout << "空队列 empty = " << q.empty() << endl;   // 1 (true)
    cout << "空队列 size = " << q.size() << endl;      // 0

    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.push(5);

    cout << "push后 empty = " << q.empty() << endl;    // 0 (false)
    cout << "push后 size = " << q.size() << endl;      // 5

    return 0;
}
```

---

## 四、queue 不能遍历

与 stack 类似，queue 不支持迭代器，不能用 for 循环遍历。

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40);

    // 错误：queue 没有 begin() 和 end()，不能用迭代器
    // for (auto it = q.begin(); it != q.end(); it++) { ... }

    // 正确做法：逐个出队并打印
    cout << "队列中元素（从队首到队尾）：" << endl;
    while (!q.empty()) {
        cout << q.front() << " ";   // 查看队首
        q.pop();                    // 出队
    }
    cout << endl;   // 输出：10 20 30 40

    // 注意：打印后队列就空了
    cout << "打印后 size = " << q.size() << endl;   // 0

    return 0;
}
```

**如果不希望破坏原队列，可以拷贝一份再遍历：**

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    // 拷贝一份
    queue<int> temp = q;

    cout << "遍历（不破坏原队列）：" << endl;
    while (!temp.empty()) {
        cout << temp.front() << " ";
        temp.pop();
    }
    cout << endl;   // 10 20 30

    // 原队列不受影响
    cout << "原队列大小：" << q.size() << endl;   // 3

    return 0;
}
```

---

## 五、queue 的底层容器

### 5.1 默认底层容器

`queue` 是一个 **容器适配器**，默认底层容器是 `deque`。

```cpp
/*
    queue 的定义（简化版）：
    template <class T, class Container = deque<T>>
    class queue {
    protected:
        Container c;    // 底层容器
    public:
        void push(const T& val) { c.push_back(val); }    // 队尾入
        void pop() { c.pop_front(); }                     // 队首出
        T& front() { return c.front(); }                  // 查看队首
        T& back() { return c.back(); }                    // 查看队尾
        bool empty() { return c.empty(); }
        size_t size() { return c.size(); }
    };
*/
```

### 5.2 指定底层容器

```cpp
#include <iostream>
#include <queue>
#include <list>
using namespace std;

int main() {
    // 默认：底层是 deque
    queue<int> q1;

    // 指定底层为 list（不能用 vector，因为 vector 没有 pop_front）
    queue<int, list<int>> q2;

    // 用法完全一样
    q1.push(10);
    q2.push(10);

    cout << q1.front() << endl;    // 10
    cout << q2.front() << endl;    // 10

    return 0;
}
```

### 5.3 底层容器的选择

```cpp
/*
    可以作为 queue 底层容器的条件：
    必须支持 front(), back(), push_back(), pop_front(), empty(), size()
    满足条件的有：deque（默认）, list

    注意：vector 不能作为 queue 的底层容器！
    因为 vector 没有 pop_front() 函数。

    选择建议：
    - deque（默认）：头尾操作都高效，内存分段
    - list：每次插入删除都是O(1)，但内存不连续
    - 一般不需要改，默认的 deque 就很好
*/
```

---

## 六、queue 的应用场景

### 6.1 BFS 广度优先搜索

BFS 是图和树的层序遍历算法，核心就是用队列。

```cpp
#include <iostream>
#include <queue>
using namespace std;

// 用 BFS 遍历二叉树的层序
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

void levelOrder(TreeNode* root) {
    if (!root) return;

    queue<TreeNode*> q;
    q.push(root);

    cout << "层序遍历：";
    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();

        cout << node->val << " ";

        // 左子节点入队
        if (node->left) {
            q.push(node->left);
        }
        // 右子节点入队
        if (node->right) {
            q.push(node->right);
        }
    }
    cout << endl;
}

int main() {
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6

    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);

    levelOrder(root);   // 层序遍历：1 2 3 4 5 6

    return 0;
}
```

**BFS 工作流程：**
```
初始：队列 [1]

步骤1: 出队 1，入队 2、3        队列：[2, 3]
步骤2: 出队 2，入队 4、5        队列：[3, 4, 5]
步骤3: 出队 3，入队 6           队列：[4, 5, 6]
步骤4: 出队 4，无子节点          队列：[5, 6]
步骤5: 出队 5，无子节点          队列：[6]
步骤6: 出队 6，无子节点          队列：空

遍历顺序：1 2 3 4 5 6
```

### 6.2 消息队列（简单模拟）

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

class MessageQueue {
private:
    queue<string> m_Messages;

public:
    // 发送消息
    void send(string msg) {
        m_Messages.push(msg);
        cout << "发送消息：" << msg << endl;
    }

    // 接收消息
    string receive() {
        if (m_Messages.empty()) {
            return "无消息";
        }
        string msg = m_Messages.front();
        m_Messages.pop();
        return msg;
    }

    // 是否有消息
    bool hasMessages() {
        return !m_Messages.empty();
    }
};

int main() {
    MessageQueue mq;

    // 生产者发送消息
    mq.send("你好");
    mq.send("在吗");
    mq.send("收到请回复");

    cout << endl;

    // 消费者接收消息（先进先出）
    cout << "接收：" << mq.receive() << endl;   // 你好
    cout << "接收：" << mq.receive() << endl;   // 在吗
    cout << "接收：" << mq.receive() << endl;   // 收到请回复
    cout << "接收：" << mq.receive() << endl;   // 无消息

    return 0;
}
```

### 6.3 打印队列

```cpp
#include <iostream>
#include <queue>
#include <string>
using namespace std;

class PrintJob {
public:
    string m_User;
    string m_Document;
    int m_Pages;

    PrintJob(string user, string doc, int pages)
        : m_User(user), m_Document(doc), m_Pages(pages) {}
};

class Printer {
private:
    queue<PrintJob> m_JobQueue;

public:
    // 提交打印任务
    void submitJob(PrintJob job) {
        m_JobQueue.push(job);
        cout << "任务已提交：[" << job.m_User << "] " << job.m_Document
             << " (" << job.m_Pages << "页)" << endl;
    }

    // 处理下一个任务
    void processNext() {
        if (m_JobQueue.empty()) {
            cout << "没有待处理的任务" << endl;
            return;
        }

        PrintJob job = m_JobQueue.front();
        m_JobQueue.pop();
        cout << "正在打印：[" << job.m_User << "] " << job.m_Document
             << " (" << job.m_Pages << "页)" << endl;
    }

    // 剩余任务数
    int pendingCount() {
        return m_JobQueue.size();
    }
};

int main() {
    Printer printer;

    // 多个用户提交打印任务
    printer.submitJob(PrintJob("张三", "报告.pdf", 10));
    printer.submitJob(PrintJob("李四", "简历.doc", 2));
    printer.submitJob(PrintJob("王五", "论文.pdf", 30));

    cout << "待处理任务数：" << printer.pendingCount() << endl;  // 3
    cout << endl;

    // 按提交顺序处理（FIFO）
    while (printer.pendingCount() > 0) {
        printer.processNext();
    }

    return 0;
}
```

---

## 七、queue vs stack 对比

```cpp
#include <iostream>
#include <queue>
#include <stack>
using namespace std;

int main() {
    // 相同数据入栈和入队
    stack<int> stk;
    queue<int> que;

    for (int i = 1; i <= 5; i++) {
        stk.push(i);
        que.push(i);
    }

    // 出栈：后进先出（LIFO）
    cout << "stack 出栈顺序：";
    while (!stk.empty()) {
        cout << stk.top() << " ";
        stk.pop();
    }
    cout << endl;   // 5 4 3 2 1

    // 出队：先进先出（FIFO）
    cout << "queue 出队顺序：";
    while (!que.empty()) {
        cout << que.front() << " ";
        que.pop();
    }
    cout << endl;   // 1 2 3 4 5

    return 0;
}
```

---

## 八、优先队列 priority_queue（扩展）

与普通队列不同，优先队列会自动按优先级排序，最大（或最小）的元素总是在队首。

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    // 默认：最大值优先（大顶堆）
    priority_queue<int> maxHeap;

    maxHeap.push(30);
    maxHeap.push(10);
    maxHeap.push(50);
    maxHeap.push(20);
    maxHeap.push(40);

    cout << "最大值优先出队：";
    while (!maxHeap.empty()) {
        cout << maxHeap.top() << " ";   // top() 返回最大值
        maxHeap.pop();
    }
    cout << endl;   // 50 40 30 20 10

    // 最小值优先（小顶堆）
    priority_queue<int, vector<int>, greater<int>> minHeap;

    minHeap.push(30);
    minHeap.push(10);
    minHeap.push(50);
    minHeap.push(20);
    minHeap.push(40);

    cout << "最小值优先出队：";
    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";   // top() 返回最小值
        minHeap.pop();
    }
    cout << endl;   // 10 20 30 40 50

    return 0;
}
```

---

## 九、综合练习：约瑟夫环问题

n 个人围成一圈，从第一个人开始报数，报到 m 的人出列，求出列顺序。

```cpp
#include <iostream>
#include <queue>
using namespace std;

int main() {
    int n = 7;  // 7个人
    int m = 3;  // 报数到3出列

    queue<int> q;

    // 所有人入队
    for (int i = 1; i <= n; i++) {
        q.push(i);
    }

    cout << "约瑟夫环（n=" << n << ", m=" << m << "）出列顺序：" << endl;

    int count = 0;
    while (!q.empty()) {
        count++;
        int person = q.front();
        q.pop();

        if (count % m == 0) {
            // 报数到 m，出列
            cout << person << " ";
        } else {
            // 没到 m，放回队尾
            q.push(person);
        }
    }
    cout << endl;
    // 输出：3 6 2 7 5 1 4

    return 0;
}
```

---

## 十、总结

| 知识点 | 要点 |
|--------|------|
| 队列的特性 | 先进先出（FIFO） |
| push | 入队（从队尾插入） |
| pop | 出队（从队首删除），不返回值 |
| front | 查看队首元素 |
| back | 查看队尾元素 |
| empty | 判断是否为空 |
| size | 返回元素个数 |
| 不能遍历 | queue 没有迭代器，不支持随机访问 |
| 底层容器 | 默认 deque，可选 list（不能用 vector） |
| 应用场景 | BFS、消息队列、打印队列、任务调度 |
| 扩展 | priority_queue 优先队列，自动排序 |
