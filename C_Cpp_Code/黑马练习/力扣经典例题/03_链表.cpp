/*
 * 03_链表.cpp - 经典LeetCode链表问题合集
 * 1. 反转链表 (206)  2. 合并有序链表 (21)  3. 环形链表 (141)
 * 4. 环形链表II (142)  5. 删除倒数第N个 (19)  6. LRU缓存 (146)
 * 7. 两数相加 (2)  8. 排序链表 (148)  9. 回文链表 (234)
 * 10. 相交链表 (160)  11. 旋转链表 (61)  12. 两两交换节点 (24)
 * 核心技巧: 哨兵节点、快慢指针、递归与迭代的转换
 */

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using namespace std;

// ==================== 链表节点定义 ====================
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// ==================== 工具函数 ====================
ListNode* createList(const vector<int>& nums) {
    ListNode dummy;
    ListNode* tail = &dummy;
    for (int n : nums) { tail->next = new ListNode(n); tail = tail->next; }
    return dummy.next;
}

void printList(ListNode* head) {
    for (ListNode* c = head; c; c = c->next) {
        cout << c->val;
        if (c->next) cout << " -> ";
    }
    cout << endl;
}

void freeList(ListNode* head) {
    while (head) { ListNode* nx = head->next; delete head; head = nx; }
}


// ================================================================
// 题目1: 反转链表 (LeetCode 206, Easy)
// ================================================================
/*
 * 给你单链表的头节点 head, 请你反转链表, 并返回反转后的链表。
 * 输入: 1->2->3->4->5  输出: 5->4->3->2->1
 *
 * ASCII图解 - 迭代反转过程 (三指针 prev/curr/next):
 *
 *   初始:  NULL    1 -> 2 -> 3 -> NULL
 *          prev  curr
 *
 *   第1步: NULL <- 1    2 -> 3 -> NULL
 *                prev  curr
 *
 *   第2步: NULL <- 1 <- 2    3 -> NULL
 *                    prev  curr
 *
 *   第3步: NULL <- 1 <- 2 <- 3    NULL
 *                         prev   curr
 *   返回 prev (即节点3, 新的头)
 */

// 方法一: 迭代法 O(n)时间 O(1)空间
class Solution206_Iterative {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* next = curr->next;  // 暂存下一个
            /*
             * 核心: 反转指针方向
             *   反转前: prev    curr -> next
             *   反转后: prev <- curr    next
             */
            curr->next = prev;
            prev = curr;     // prev前进
            curr = next;     // curr前进
        }
        return prev;  // prev即新头节点
    }
};

// 方法二: 递归法 O(n)时间 O(n)空间(栈)
/*
 * 递归过程 (以 1->2->3 为例):
 *   reverseList(1) -> reverseList(2) -> reverseList(3)
 *   base case: 节点3, 返回3
 *   回溯: 2->next->next = 2, 即 3->next = 2 => 3->2->NULL
 *         2->next = NULL (断开旧连接)
 *   回溯: 1->next->next = 1, 即 2->next = 1 => 3->2->1->NULL
 *         1->next = NULL
 *   最终: 3->2->1->NULL
 */
class Solution206_Recursive {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode* newHead = reverseList(head->next);
        head->next->next = head;  // 尾节点指回当前
        head->next = nullptr;     // 断开旧连接
        return newHead;
    }
};

void test_206() {
    cout << "=== 题目1: 反转链表 (LeetCode 206) ===" << endl;
    ListNode* h = createList({1,2,3,4,5});
    cout << "原始: "; printList(h);
    Solution206_Iterative s1;
    ListNode* r = s1.reverseList(h);
    cout << "迭代: "; printList(r); freeList(r);

    Solution206_Recursive s2;
    ListNode* r2 = s2.reverseList(createList({1,2,3,4,5}));
    cout << "递归: "; printList(r2); freeList(r2);

    cout << "空链表: "; printList(s2.reverseList(nullptr));
    ListNode* r3 = s2.reverseList(new ListNode(42));
    cout << "单节点: "; printList(r3); freeList(r3);
    cout << endl;
}


// ================================================================
// 题目2: 合并两个有序链表 (LeetCode 21, Easy)
// ================================================================
/*
 * 将两个升序链表合并为一个新的升序链表。
 * 输入: 1->2->4, 1->3->4  输出: 1->1->2->3->4->4
 *
 * 关键技巧 - 哨兵节点(Dummy Head):
 * 不用哨兵需要特判头节点为空; 用了哨兵, 所有节点处理逻辑统一。
 *
 * ASCII图解 - 合并过程:
 *   l1: 1->2->4    l2: 1->3->4    dummy: 0
 *
 *   比较1<=1, 取l1: dummy->1
 *   比较2>1,  取l2: dummy->1->1
 *   比较2<=3, 取l1: dummy->1->1->2
 *   比较4>3,  取l2: dummy->1->1->2->3
 *   比较4<=4, 取l1: dummy->1->1->2->3->4
 *   l1空, 拼l2:     dummy->1->1->2->3->4->4
 */

// 方法一: 迭代法 O(n+m)时间 O(1)空间
class Solution21_Iterative {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) { tail->next = l1; l1 = l1->next; }
            else                     { tail->next = l2; l2 = l2->next; }
            tail = tail->next;
        }
        tail->next = l1 ? l1 : l2;  // 拼接剩余
        return dummy.next;
    }
};

// 方法二: 递归法 O(n+m)时间 O(n+m)空间
class Solution21_Recursive {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1->val <= l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
    }
};

void test_21() {
    cout << "=== 题目2: 合并两个有序链表 (LeetCode 21) ===" << endl;
    Solution21_Iterative s1;
    Solution21_Recursive s2;

    ListNode* r1 = s1.mergeTwoLists(createList({1,2,4}), createList({1,3,4}));
    cout << "标准: "; printList(r1); freeList(r1);

    ListNode* r2 = s2.mergeTwoLists(createList({1,2,3}), nullptr);
    cout << "一个空: "; printList(r2); freeList(r2);

    ListNode* r3 = s1.mergeTwoLists(nullptr, nullptr);
    cout << "都空: "; printList(r3);

    ListNode* r4 = s2.mergeTwoLists(createList({1,2,3}), createList({4,5,6}));
    cout << "不重叠: "; printList(r4); freeList(r4);
    cout << endl;
}


// ================================================================
// 题目3: 环形链表 (LeetCode 141, Easy)
// ================================================================
/*
 * 判断链表中是否有环。
 *
 * ASCII图解 - 快慢指针检测环:
 *
 *   无环: 1->2->3->NULL  fast先到NULL
 *
 *   有环: 1->2->3->4
 *             ^        |
 *             |________|
 *   步骤   slow  fast
 *   初始     1     1
 *   第1步    2     3
 *   第2步    3     3  <-- 相遇, 有环!
 *
 * 为什么一定能相遇?
 * 每步fast比slow多走1, 两者距离逐步缩小, 最多C步(环长)必相遇。
 */

// 方法一: 哈希集合 O(n)时间 O(n)空间
class Solution141_HashSet {
public:
    bool hasCycle(ListNode* head) {
        unordered_set<ListNode*> vis;
        for (ListNode* c = head; c; c = c->next) {
            if (vis.count(c)) return true;
            vis.insert(c);
        }
        return false;
    }
};

// 方法二: Floyd快慢指针 O(n)时间 O(1)空间
class Solution141_TwoPointers {
public:
    bool hasCycle(ListNode* head) {
        if (!head || !head->next) return false;
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }
};

void test_141() {
    cout << "=== 题目3: 环形链表 (LeetCode 141) ===" << endl;
    Solution141_HashSet sh;
    Solution141_TwoPointers st;

    // 有环: 1->2->3->4->(回到2)
    ListNode *n1=new ListNode(1),*n2=new ListNode(2);
    ListNode *n3=new ListNode(3),*n4=new ListNode(4);
    n1->next=n2; n2->next=n3; n3->next=n4; n4->next=n2;
    cout << "1->2->3->4->(回到2): ";
    cout << "哈希=" << (sh.hasCycle(n1)?"有环":"无环");
    cout << " 快慢=" << (st.hasCycle(n1)?"有环":"无环") << endl;
    n4->next=nullptr; freeList(n1);

    ListNode* h2 = createList({1,2,3,4,5});
    cout << "1->2->3->4->5: ";
    cout << "哈希=" << (sh.hasCycle(h2)?"有环":"无环");
    cout << " 快慢=" << (st.hasCycle(h2)?"有环":"无环") << endl;
    freeList(h2);

    cout << "空链表: " << (st.hasCycle(nullptr)?"有环":"无环") << endl;
    cout << endl;
}


// ================================================================
// 题目4: 环形链表 II (LeetCode 142, Medium)
// ================================================================
/*
 * 返回链表开始入环的第一个节点, 无环返回nullptr。
 *
 * 数学证明 (Floyd找环入口):
 *
 *   设: a=头到入口, b=入口到相遇点, c=相遇点回入口, 环长=b+c
 *
 *         a 步          b 步
 *   head ---------> 入口 ---------> 相遇点
 *                   ^  |              |
 *                   |__|____c 步______|
 *
 *   slow走了 a+b, fast走了 a+b+n(b+c)
 *   因为 fast=2*slow:
 *     2(a+b) = a+b+n(b+c)
 *     a+b = n(b+c)
 *     a = (n-1)(b+c) + c
 *
 *   结论: 从head走a步 == 从相遇点走c步, 都到达环入口!
 *   所以: 一个从head出发, 一个从相遇点出发, 各走1步, 必在入口相遇。
 */

// 方法一: 哈希集合 O(n)时间 O(n)空间
class Solution142_HashSet {
public:
    ListNode* detectCycle(ListNode* head) {
        unordered_set<ListNode*> vis;
        for (ListNode* c = head; c; c = c->next) {
            if (vis.count(c)) return c;  // 第一个重复的就是入口
            vis.insert(c);
        }
        return nullptr;
    }
};

// 方法二: Floyd算法 O(n)时间 O(1)空间
class Solution142_Floyd {
public:
    ListNode* detectCycle(ListNode* head) {
        if (!head || !head->next) return nullptr;
        ListNode *slow = head, *fast = head;
        // 阶段1: 找相遇点
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                // 阶段2: 一个从head, 一个从相遇点, 各走1步
                ListNode* entry = head;
                while (entry != slow) {
                    entry = entry->next;
                    slow = slow->next;
                }
                return entry;  // 相遇点即环入口
            }
        }
        return nullptr;
    }
};

void test_142() {
    cout << "=== 题目4: 环形链表 II (LeetCode 142) ===" << endl;
    Solution142_HashSet sh;
    Solution142_Floyd sf;

    ListNode *a=new ListNode(1),*b=new ListNode(2);
    ListNode *c=new ListNode(3),*d=new ListNode(4);
    a->next=b; b->next=c; c->next=d; d->next=b;
    auto e1=sh.detectCycle(a), e2=sf.detectCycle(a);
    cout << "1->2->3->4->(回2): 哈希入口=" << e1->val
         << " Floyd入口=" << e2->val << endl;
    d->next=nullptr; freeList(a);

    ListNode* h2 = createList({1,2,3,4,5});
    cout << "无环: Floyd入口="
         << (sf.detectCycle(h2)?"有":"无") << endl;
    freeList(h2);
    cout << endl;
}


// ================================================================
// 题目5: 删除链表的倒数第N个结点 (LeetCode 19, Medium)
// ================================================================
/*
 * 删除链表的倒数第 n 个结点, 返回头结点。
 * 输入: 1->2->3->4->5, n=2  输出: 1->2->3->5
 *
 * ASCII图解 - 双指针法 (n=2):
 *
 *   初始:  dummy->1->2->3->4->5
 *          slow
 *          fast
 *
 *   fast先走n+1=3步:
 *          dummy->1->2->3->4->5
 *          slow        fast
 *
 *   同步移动直到fast=NULL:
 *          dummy->1->2->3->4->5->NULL
 *                     slow     fast
 *
 *   slow->next 即要删除的节点(值4)
 *   执行: slow->next = slow->next->next
 *
 * 为什么用dummy? 删除头节点时无需特判。
 */

// 方法一: 两次遍历 O(L)时间
class Solution19_TwoPass {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        int len = 0;
        for (ListNode* c = head; c; c = c->next) len++;
        ListNode* cur = &dummy;
        for (int i = 0; i < len - n; i++) cur = cur->next;
        ListNode* del = cur->next;
        cur->next = cur->next->next;
        delete del;
        return dummy.next;
    }
};

// 方法二: 一次遍历 双指针 O(L)时间 O(1)空间
/*
 * fast先走n+1步, 之后fast和slow同步走。
 * 当fast到NULL时, slow在要删除节点的前一个。
 * 为什么走n+1而不是n? 因为要停在 "前一个" 位置。
 */
class Solution19_OnePass {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode *slow = &dummy, *fast = &dummy;
        for (int i = 0; i <= n; i++) fast = fast->next;
        while (fast) { slow = slow->next; fast = fast->next; }
        ListNode* del = slow->next;
        slow->next = slow->next->next;
        delete del;
        return dummy.next;
    }
};

void test_19() {
    cout << "=== 题目5: 删除倒数第N个结点 (LeetCode 19) ===" << endl;
    Solution19_TwoPass s1;
    Solution19_OnePass s2;

    ListNode* r1 = s1.removeNthFromEnd(createList({1,2,3,4,5}), 2);
    cout << "1-5删倒数2: "; printList(r1); freeList(r1);

    ListNode* r2 = s2.removeNthFromEnd(createList({1,2,3,4,5}), 5);
    cout << "1-5删倒数5: "; printList(r2); freeList(r2);

    ListNode* r3 = s2.removeNthFromEnd(createList({1}), 1);
    cout << "单节点删:   "; printList(r3);

    ListNode* r4 = s1.removeNthFromEnd(createList({1,2,3}), 1);
    cout << "删尾节点:   "; printList(r4); freeList(r4);
    cout << endl;
}


// ================================================================
// 题目6: LRU缓存 (LeetCode 146, Medium) - 高频面试题
// ================================================================
/*
 * 实现LRU (最近最少使用) 缓存: get(key) 和 put(key,value) 均 O(1)。
 *
 * 数据结构选择: 哈希表 + 双向链表
 *
 * ASCII图解 - 整体设计:
 *
 *   哈希表 (O(1)查找):
 *     key=1 -> 指向链表节点
 *     key=2 -> 指向链表节点
 *
 *   双向链表 (O(1)插入/删除, 维护使用顺序):
 *     dummyHead <-> 最近使用 <-> ... <-> 最久使用 <-> dummyTail
 *
 *   get(key): 哈希找节点 -> 移到头部(标记最近使用) -> 返回值
 *   put(key,val):
 *     已存在: 更新值, 移到头部
 *     不存在且满: 删尾部(最久未用), 插头部, 更新哈希
 *     不存在未满: 插头部, 更新哈希
 *
 * 为什么双向链表? 单链表O(1)删不掉给定节点(需找前驱)。
 * 为什么哨兵? dummyHead/dummyTail消除空链表/单节点边界判断。
 */

class LRUCache {
private:
    struct DNode {
        int key, value;
        DNode *prev, *next;
        DNode(int k=0, int v=0) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int cap_, size_;
    unordered_map<int, DNode*> cache;
    DNode *dHead, *dTail;  // 哨兵头/尾

    // 头插: dummyHead <-> X <-> nodeA  =>  dummyHead <-> X <-> new <-> nodeA
    void addToHead(DNode* node) {
        node->prev = dHead;
        node->next = dHead->next;
        dHead->next->prev = node;
        dHead->next = node;
    }

    // 摘除节点: ... <-> A <-> B <-> C  =>  ... <-> A <-> C
    void removeNode(DNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 移到头部 = 摘除 + 头插
    void moveToHead(DNode* node) { removeNode(node); addToHead(node); }

    // 删除尾部(最久未用)
    DNode* removeTail() {
        DNode* node = dTail->prev;
        removeNode(node);
        return node;
    }

public:
    LRUCache(int capacity) : cap_(capacity), size_(0) {
        dHead = new DNode();
        dTail = new DNode();
        dHead->next = dTail;
        dTail->prev = dHead;
    }

    ~LRUCache() {
        DNode* cur = dHead;
        while (cur) { DNode* nx = cur->next; delete cur; cur = nx; }
    }

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;
        moveToHead(it->second);  // 标记为最近使用
        return it->second->value;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            // key已存在: 更新值, 移到头部
            it->second->value = value;
            moveToHead(it->second);
        } else {
            // key不存在
            if (size_ >= cap_) {
                // 满了: 淘汰最久未用(尾部)
                DNode* tail = removeTail();
                cache.erase(tail->key);
                delete tail;
                size_--;
            }
            DNode* node = new DNode(key, value);
            addToHead(node);
            cache[key] = node;
            size_++;
        }
    }
};

/*
 * LRU操作图解 (容量=2):
 *
 *   put(1,1): dummyHead <-> (1,1) <-> dTail
 *   put(2,2): dummyHead <-> (2,2) <-> (1,1) <-> dTail
 *   get(1)=1: dummyHead <-> (1,1) <-> (2,2) <-> dTail   [1移到头部]
 *   put(3,3): dummyHead <-> (3,3) <-> (1,1) <-> dTail   [2被淘汰]
 *   get(2)=-1: 已被淘汰
 *   put(4,4): dummyHead <-> (4,4) <-> (3,3) <-> dTail   [1被淘汰]
 *   get(1)=-1, get(3)=3, get(4)=4
 */

void test_146() {
    cout << "=== 题目6: LRU缓存 (LeetCode 146) ===" << endl;

    cout << "capacity=2:" << endl;
    LRUCache lru(2);
    lru.put(1,1); cout << "put(1,1)" << endl;
    lru.put(2,2); cout << "put(2,2)" << endl;
    cout << "get(1)=" << lru.get(1) << endl;       // 1
    lru.put(3,3); cout << "put(3,3) [淘汰key=2]" << endl;
    cout << "get(2)=" << lru.get(2) << endl;       // -1
    lru.put(4,4); cout << "put(4,4) [淘汰key=1]" << endl;
    cout << "get(1)=" << lru.get(1) << endl;       // -1
    cout << "get(3)=" << lru.get(3) << endl;       // 3
    cout << "get(4)=" << lru.get(4) << endl;       // 4

    cout << "\n更新已存在的key:" << endl;
    LRUCache lru2(2);
    lru2.put(1,1); lru2.put(2,2); lru2.put(1,10);
    cout << "get(1)=" << lru2.get(1) << endl;      // 10
    cout << "get(2)=" << lru2.get(2) << endl;      // 2

    cout << "\n容量为1:" << endl;
    LRUCache lru3(1);
    lru3.put(1,1); lru3.put(2,2);
    cout << "get(1)=" << lru3.get(1) << endl;      // -1
    cout << "get(2)=" << lru3.get(2) << endl;      // 2
    cout << endl;
}


// ================================================================
// 题目7: 两数相加 (LeetCode 2, Medium)
// ================================================================
/*
 * 给你两个非空链表, 表示两个非负整数。每个节点存储一位数字, 且按逆序排列。
 * 将两个数相加, 并以相同形式返回一个表示和的链表。
 * 输入: l1=[2,4,3], l2=[5,6,4]  输出: [7,0,8]  (342+465=807)
 *
 * ASCII图解 - 逐位相加过程 (带进位):
 *
 *   l1:  2 -> 4 -> 3    代表 342
 *   l2:  5 -> 6 -> 4    代表 465
 *
 *   第1位: 2+5=7,  carry=0  =>  7
 *   第2位: 4+6=10, carry=1  =>  0
 *   第3位: 3+4+1=8, carry=0 =>  8
 *   结果:  7 -> 0 -> 8     代表 807
 *
 *   特殊情况 - 长度不同:
 *   l1:  9 -> 9    l2:  1
 *   第1位: 9+1=10, carry=1   => 0
 *   第2位: 9+0+1=10, carry=1 => 0
 *   进位: carry=1             => 1
 *   结果:  0 -> 0 -> 1       代表 100
 *
 * ASCII图解 - 链表节点操作:
 *
 *   dummy -> ...
 *   tail ^
 *   每步: tail->next = new ListNode(sum % 10), tail = tail->next
 */

// 方法一: 迭代法 O(max(m,n))时间 O(max(m,n))空间
class Solution2_Iterative {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;
        int carry = 0;  // 进位
        while (l1 || l2 || carry) {
            int sum = carry;  // 先加上进位
            if (l1) { sum += l1->val; l1 = l1->next; }
            if (l2) { sum += l2->val; l2 = l2->next; }
            carry = sum / 10;           // 计算新的进位
            tail->next = new ListNode(sum % 10);  // 当前位
            tail = tail->next;
        }
        return dummy.next;
    }
};

// 方法二: 递归法 O(max(m,n))时间 O(max(m,n))空间
/*
 * 递归思路: 每次处理一位, 将进位传递给下一层递归。
 * base case: 两个链表都为空且无进位时返回nullptr。
 */
class Solution2_Recursive {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2, int carry = 0) {
        if (!l1 && !l2 && !carry) return nullptr;
        int sum = carry;
        if (l1) { sum += l1->val; l1 = l1->next; }
        if (l2) { sum += l2->val; l2 = l2->next; }
        ListNode* node = new ListNode(sum % 10);
        node->next = addTwoNumbers(l1, l2, sum / 10);
        return node;
    }
};

void test_2() {
    cout << "=== 题目7: 两数相加 (LeetCode 2) ===" << endl;
    Solution2_Iterative s1;
    Solution2_Recursive s2;

    // 342 + 465 = 807
    ListNode* r1 = s1.addTwoNumbers(createList({2,4,3}), createList({5,6,4}));
    cout << "342+465: "; printList(r1); freeList(r1);

    // 0 + 0 = 0
    ListNode* r2 = s1.addTwoNumbers(createList({0}), createList({0}));
    cout << "0+0:    "; printList(r2); freeList(r2);

    // 99 + 1 = 100
    ListNode* r3 = s2.addTwoNumbers(createList({9,9}), createList({1}));
    cout << "99+1:   "; printList(r3); freeList(r3);

    // 9999 + 99 = 10098
    ListNode* r4 = s1.addTwoNumbers(createList({9,9,9,9}), createList({9,9}));
    cout << "9999+99:"; printList(r4); freeList(r4);

    // 不同长度: 1 + 999 = 1000
    ListNode* r5 = s2.addTwoNumbers(createList({1}), createList({9,9,9}));
    cout << "1+999:  "; printList(r5); freeList(r5);
    cout << endl;
}


// ================================================================
// 题目8: 排序链表 (LeetCode 148, Medium)
// ================================================================
/*
 * 给你链表的头结点 head, 请将其按升序排列并返回排序后的链表。
 * 要求: O(n log n) 时间复杂度, O(1) 空间复杂度 (自底向上归并)
 *
 * 为什么选归并排序?
 *   - 数组排序可用快排, 但链表不支持随机访问
 *   - 归并排序天然适合链表: 找中点O(n), 合并O(n), 总O(nlogn)
 *   - 不需要额外空间(不像数组归并需要辅助数组)
 *
 * ASCII图解 - 自顶向下归并排序:
 *
 *   原始: 4 -> 2 -> 1 -> 3
 *
 *   分割 (快慢指针找中点):
 *     4 -> 2 -> 1 -> 3
 *     slow    fast
 *     中点: 节点1 (slow->next), 断开: 4->2 | 1->3
 *
 *   递归分割:
 *     4->2  =>  4 | 2
 *     1->3  =>  1 | 3
 *
 *   合并:
 *     4 | 2   =>  2 -> 4
 *     1 | 3   =>  1 -> 3
 *     2->4 + 1->3  =>  1 -> 2 -> 3 -> 4
 *
 * ASCII图解 - 快慢指针找中点:
 *
 *   4 -> 2 -> 1 -> 3 -> NULL
 *   s    f               初始: s=head, f=head->next
 *        s         f     第1步
 *             s              f  (fast到末尾, slow->next即中点)
 *   断开: left=4->2, right=1->3
 */

// 方法一: 自顶向下归并排序(递归) O(nlogn)时间 O(logn)空间(递归栈)
class Solution148_TopDown {
public:
    ListNode* sortList(ListNode* head) {
        // base case: 空或单节点, 已有序
        if (!head || !head->next) return head;

        // 快慢指针找中点
        ListNode *slow = head, *fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode* mid = slow->next;  // 右半部分起点
        slow->next = nullptr;        // 断开链表

        // 递归排序左右两半
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);

        // 合并两个有序链表
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (left && right) {
            if (left->val <= right->val) { tail->next = left; left = left->next; }
            else                          { tail->next = right; right = right->next; }
            tail = tail->next;
        }
        tail->next = left ? left : right;
        return dummy.next;
    }
};

// 方法二: 自底向上归并排序(迭代) O(nlogn)时间 O(1)空间
/*
 * 自底向上: 逐步倍增步长(1,2,4,8...), 每轮合并相邻子链表。
 *
 * ASCII图解 (步长从1开始倍增):
 *
 *   原始: 4 -> 2 -> 1 -> 3
 *
 *   step=1: 合并相邻1个节点
 *     [4]|[2] => 2->4    [1]|[3] => 1->3
 *     结果: 2->4->1->3
 *
 *   step=2: 合并相邻2个节点
 *     [2->4]|[1->3] => 1->2->3->4
 *     结果: 1->2->3->4
 *
 *   step=4 >= len, 排序完成
 */
class Solution148_BottomUp {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;

        // 计算链表长度
        int len = 0;
        for (ListNode* c = head; c; c = c->next) len++;

        ListNode dummy(0, head);  // 哨兵头

        // step: 子链表步长, 从1开始倍增
        for (int step = 1; step < len; step <<= 1) {
            ListNode *prev = &dummy, *curr = dummy.next;
            while (curr) {
                // 取出左半部分 [curr, curr+step)
                ListNode* left = curr;
                ListNode* right = split(left, step);    // 右半起点
                curr = split(right, step);               // 下一组起点

                // 合并left和right, 接到prev后面
                prev = merge(left, right, prev);
            }
        }
        return dummy.next;
    }

private:
    // 从head截取n个节点, 返回剩余部分的头
    ListNode* split(ListNode* head, int n) {
        while (--n && head) head = head->next;
        ListNode* rest = head ? head->next : nullptr;
        if (head) head->next = nullptr;  // 断开
        return rest;
    }

    // 合并l1和l2, 接到prev后面, 返回合并后的尾节点
    ListNode* merge(ListNode* l1, ListNode* l2, ListNode* prev) {
        ListNode* curr = prev;
        while (l1 && l2) {
            if (l1->val <= l2->val) { curr->next = l1; l1 = l1->next; }
            else                      { curr->next = l2; l2 = l2->next; }
            curr = curr->next;
        }
        curr->next = l1 ? l1 : l2;
        while (curr->next) curr = curr->next;  // 移到尾部
        return curr;
    }
};

void test_148() {
    cout << "=== 题目8: 排序链表 (LeetCode 148) ===" << endl;
    Solution148_TopDown s1;
    Solution148_BottomUp s2;

    ListNode* r1 = s1.sortList(createList({4,2,1,3}));
    cout << "自顶向下 [4,2,1,3]: "; printList(r1); freeList(r1);

    ListNode* r2 = s2.sortList(createList({4,2,1,3}));
    cout << "自底向上 [4,2,1,3]: "; printList(r2); freeList(r2);

    ListNode* r3 = s1.sortList(createList({-1,5,3,4,0}));
    cout << "负数 [-1,5,3,4,0]:  "; printList(r3); freeList(r3);

    ListNode* r4 = s2.sortList(createList({1}));
    cout << "单节点 [1]:         "; printList(r4); freeList(r4);

    ListNode* r5 = s1.sortList(nullptr);
    cout << "空链表:             "; printList(r5);

    ListNode* r6 = s1.sortList(createList({5,4,3,2,1}));
    cout << "逆序 [5,4,3,2,1]:   "; printList(r6); freeList(r6);
    cout << endl;
}


// ================================================================
// 题目9: 回文链表 (LeetCode 234, Easy)
// ================================================================
/*
 * 给你一个单链表的头节点 head, 判断该链表是否为回文链表。
 * 回文: 正读反读相同, 如 1->2->2->1
 *
 * ASCII图解 - 方法: 快慢指针找中点 + 反转后半 + 逐一比较
 *
 *   原始: 1 -> 2 -> 2 -> 1
 *
 *   步骤1: 快慢指针找中点
 *     slow=1, fast=1
 *     slow=2, fast=2  (fast到末尾)
 *     中点: 节点2(第二个)
 *
 *   步骤2: 反转后半部分
 *     前半: 1 -> 2
 *     后半(反转): 1 <- 2   即 2 -> 1
 *
 *   步骤3: 逐一比较
 *     前半: 1 -> 2
 *     后半: 2 -> 1
 *     比较: 1==1 ✓, 2==2 ✓ => 回文!
 *
 * 奇数个节点的情况:
 *   原始: 1 -> 2 -> 3 -> 2 -> 1
 *   中点: 节点3 (跳过), 后半从节点2(第二个)开始反转
 *   前半: 1 -> 2 -> 3
 *   后半(反转): 1 -> 2
 *   比较: 1==1 ✓, 2==2 ✓ => 回文!
 */

// 方法一: 反转后半部分 O(n)时间 O(1)空间
class Solution234_Reverse {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) return true;

        // 快慢指针找中点
        ListNode *slow = head, *fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        // slow指向左半部分的末尾(中点前一个或中点)

        // 反转后半部分
        ListNode* secondHalf = reverseList(slow->next);

        // 逐一比较前半和反转后的后半
        ListNode *p1 = head, *p2 = secondHalf;
        bool result = true;
        while (p2) {  // 后半可能更短(奇数), 以后半为准
            if (p1->val != p2->val) { result = false; break; }
            p1 = p1->next;
            p2 = p2->next;
        }

        // 恢复链表结构(可选, 面试中加分)
        slow->next = reverseList(secondHalf);
        return result;
    }

private:
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        while (head) {
            ListNode* next = head->next;
            head->next = prev;
            prev = head;
            head = next;
        }
        return prev;
    }
};

// 方法二: 栈 O(n)时间 O(n)空间
/*
 * 思路: 遍历链表将所有值入栈, 再遍历一次从栈顶比较。
 * 栈的后进先出特性天然实现了"反序"比较。
 */
class Solution234_Stack {
public:
    bool isPalindrome(ListNode* head) {
        vector<int> vals;
        for (ListNode* c = head; c; c = c->next) vals.push_back(c->val);
        // 双指针从两端向中间比较
        int left = 0, right = (int)vals.size() - 1;
        while (left < right) {
            if (vals[left] != vals[right]) return false;
            left++; right--;
        }
        return true;
    }
};

void test_234() {
    cout << "=== 题目9: 回文链表 (LeetCode 234) ===" << endl;
    Solution234_Reverse s1;
    Solution234_Stack s2;

    ListNode* h1 = createList({1,2,2,1});
    cout << "1->2->2->1: 反转=" << (s1.isPalindrome(h1)?"是":"否")
         << " 栈=" << (s2.isPalindrome(h1)?"是":"否") << endl;
    freeList(h1);

    ListNode* h2 = createList({1,2,3,2,1});
    cout << "1->2->3->2->1(奇数): 反转=" << (s1.isPalindrome(h2)?"是":"否")
         << " 栈=" << (s2.isPalindrome(h2)?"是":"否") << endl;
    freeList(h2);

    ListNode* h3 = createList({1,2,3,4,5});
    cout << "1->2->3->4->5: 反转=" << (s1.isPalindrome(h3)?"是":"否")
         << " 栈=" << (s2.isPalindrome(h3)?"是":"否") << endl;
    freeList(h3);

    ListNode* h4 = createList({1});
    cout << "单节点1: 反转=" << (s1.isPalindrome(h4)?"是":"否")
         << " 栈=" << (s2.isPalindrome(h4)?"是":"否") << endl;
    freeList(h4);

    cout << "空链表: 反转=" << (s1.isPalindrome(nullptr)?"是":"否")
         << " 栈=" << (s2.isPalindrome(nullptr)?"是":"否") << endl;

    ListNode* h5 = createList({1,1,1,1});
    cout << "1->1->1->1: 反转=" << (s1.isPalindrome(h5)?"是":"否")
         << " 栈=" << (s2.isPalindrome(h5)?"是":"否") << endl;
    freeList(h5);
    cout << endl;
}


// ================================================================
// 题目10: 相交链表 (LeetCode 160, Easy)
// ================================================================
/*
 * 给你两个单链表的头节点 headA 和 headB, 请你找出并返回两个链表相交的起始节点。
 * 如果不存在相交节点, 返回 nullptr。
 *
 * ASCII图解 - 问题描述:
 *
 *   A:    a1 -> a2 \
 *                    -> c1 -> c2 -> c3
 *   B: b1 -> b2 -> b3 /
 *
 *   相交节点: c1 (注意: 相交意味着共享节点, 不只是值相同)
 *
 * 为什么两指针能相遇? 数学证明:
 *
 *   设: A独有部分长度=a, B独有部分长度=b, 公共部分长度=c
 *   指针pA走完A再走B: a+c+b 步到达交点
 *   指针pB走完B再走A: b+c+a 步到达交点
 *   a+c+b == b+c+a, 所以两指针一定在交点相遇!
 *
 * ASCII图解 - 双指针过程:
 *
 *   pA: a1->a2->c1->c2->c3->b1->b2->b3->c1  <- 在c1相遇!
 *   pB: b1->b2->b3->c1->c2->c3->a1->a2->c1  <- 在c1相遇!
 *
 *   无交点的情况:
 *   pA: a1->a2->b1->b2->NULL
 *   pB: b1->b2->a1->a2->NULL  都走到NULL, 返回nullptr
 */

// 方法一: 双指针法 O(m+n)时间 O(1)空间
class Solution160_TwoPointers {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (!headA || !headB) return nullptr;
        ListNode *pA = headA, *pB = headB;
        // 当pA和pB不相等时继续走
        // 如果有交点: 走 a+c+b 步后在交点相遇
        // 如果无交点: 走 a+b 步后同时到达nullptr
        while (pA != pB) {
            pA = pA ? pA->next : headB;  // 走完A接着走B
            pB = pB ? pB->next : headA;  // 走完B接着走A
        }
        return pA;  // 交点或nullptr
    }
};

// 方法二: 哈希集合 O(m+n)时间 O(m)空间
class Solution160_HashSet {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        unordered_set<ListNode*> visited;
        // 先遍历A, 记录所有节点
        for (ListNode* c = headA; c; c = c->next) visited.insert(c);
        // 遍历B, 第一个已访问的就是交点
        for (ListNode* c = headB; c; c = c->next)
            if (visited.count(c)) return c;
        return nullptr;
    }
};

// 方法三: 长度差法 O(m+n)时间 O(1)空间
/*
 * 思路: 先求两链表长度差d, 让长的先走d步, 然后同步走, 相遇即交点。
 */
class Solution160_LengthDiff {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        int lenA = 0, lenB = 0;
        for (ListNode* c = headA; c; c = c->next) lenA++;
        for (ListNode* c = headB; c; c = c->next) lenB++;
        // 让长链表先走差值步
        ListNode *longList = headA, *shortList = headB;
        if (lenB > lenA) { longList = headB; shortList = headA; }
        int diff = abs(lenA - lenB);
        for (int i = 0; i < diff; i++) longList = longList->next;
        // 同步走, 找交点
        while (longList != shortList) {
            longList = longList->next;
            shortList = shortList->next;
        }
        return longList;
    }
};

void test_160() {
    cout << "=== 题目10: 相交链表 (LeetCode 160) ===" << endl;
    Solution160_TwoPointers s1;
    Solution160_HashSet s2;
    Solution160_LengthDiff s3;

    // 构造相交链表: A=4->1->8->4->5, B=5->6->1->8->4->5, 交于节点8
    ListNode *c1=new ListNode(8), *c2=new ListNode(4), *c3=new ListNode(5);
    c1->next=c2; c2->next=c3;
    ListNode *a1=new ListNode(4), *a2=new ListNode(1);
    a1->next=a2; a2->next=c1;
    ListNode *b1=new ListNode(5), *b2=new ListNode(6), *b3=new ListNode(1);
    b1->next=b2; b2->next=b3; b3->next=c1;

    cout << "A=4->1->8->4->5, B=5->6->1->8->4->5:" << endl;
    cout << "  双指针: 交点=" << (s1.getIntersectionNode(a1,b1)?to_string(s1.getIntersectionNode(a1,b1)->val):"无") << endl;
    cout << "  哈希:   交点=" << (s2.getIntersectionNode(a1,b1)?to_string(s2.getIntersectionNode(a1,b1)->val):"无") << endl;
    cout << "  长度差: 交点=" << (s3.getIntersectionNode(a1,b1)?to_string(s3.getIntersectionNode(a1,b1)->val):"无") << endl;
    // 清理(注意不能freeList共享部分两次)
    freeList(a1); freeList(b1); // b1到c1之间的b2,b3已被freeList(b1)处理, c1->c3由a1处理

    // 无交点
    ListNode* h1 = createList({1,2,3});
    ListNode* h2 = createList({4,5,6});
    cout << "无交点 [1,2,3] & [4,5,6]: "
         << (s1.getIntersectionNode(h1,h2)?"有":"无") << endl;
    freeList(h1); freeList(h2);

    // 空链表
    cout << "空链表: " << (s1.getIntersectionNode(nullptr,nullptr)?"有":"无") << endl;
    cout << endl;
}


// ================================================================
// 题目11: 旋转链表 (LeetCode 61, Medium)
// ================================================================
/*
 * 给你一个链表的头节点 head, 旋转链表, 将链表每个节点向右移动 k 个位置。
 * 输入: 1->2->3->4->5, k=2  输出: 4->5->1->2->3
 *
 * ASCII图解 - 旋转过程 (k=2):
 *
 *   原始: 1 -> 2 -> 3 -> 4 -> 5
 *
 *   关键观察: 向右移k个 = 从倒数第k个位置断开
 *   k%len = 2%5 = 2, 新尾在倒数第3个(即节点3)
 *
 *   步骤1: 找到尾节点(5), 连成环: 1->2->3->4->5->1->2->...
 *   步骤2: 新尾 = 第 len-k%len = 3 个节点(节点3)
 *   步骤3: 新头 = 新尾->next = 节点4
 *   步骤4: 断开: 新尾->next = nullptr
 *
 *   结果: 4 -> 5 -> 1 -> 2 -> 3
 *
 * ASCII图解 - 成环再断开:
 *
 *   原始: 1->2->3->4->5->NULL
 *   成环: 1->2->3->4->5->1->2->3->4->5->1...
 *         新尾=3, 新头=4
 *   断开: 4->5->1->2->3->NULL
 *
 * 特殊情况:
 *   - k是len的倍数: 旋转后不变(如k=5, len=5)
 *   - k > len: 取模 k % len
 *   - 空链表或单节点: 直接返回
 */

// 方法: 成环再断开 O(n)时间 O(1)空间
class Solution61 {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) return head;

        // 步骤1: 求长度, 并让tail指向尾节点
        int len = 1;
        ListNode* tail = head;
        while (tail->next) { tail = tail->next; len++; }

        // 步骤2: 成环
        tail->next = head;  // 尾连头, 形成环

        // 步骤3: 找新尾 (第 len - k%len 个节点)
        k = k % len;
        int stepsToNewTail = len - k;
        ListNode* newTail = head;
        for (int i = 1; i < stepsToNewTail; i++) newTail = newTail->next;

        // 步骤4: 新头 = newTail->next, 断开环
        ListNode* newHead = newTail->next;
        newTail->next = nullptr;

        return newHead;
    }
};

/*
 * 补充: 递归思路 (理解用, 非最优)
 *   每次将最后一个节点移到最前面, 重复k次。
 *   时间O(n*k), 不如成环法高效, 但思路直观。
 */

void test_61() {
    cout << "=== 题目11: 旋转链表 (LeetCode 61) ===" << endl;
    Solution61 s;

    ListNode* r1 = s.rotateRight(createList({1,2,3,4,5}), 2);
    cout << "[1,2,3,4,5] k=2: "; printList(r1); freeList(r1);

    ListNode* r2 = s.rotateRight(createList({0,1,2}), 4);
    cout << "[0,1,2] k=4:     "; printList(r2); freeList(r2);

    ListNode* r3 = s.rotateRight(createList({1,2,3}), 3);
    cout << "[1,2,3] k=3(倍数):"; printList(r3); freeList(r3);

    ListNode* r4 = s.rotateRight(createList({1,2}), 1);
    cout << "[1,2] k=1:       "; printList(r4); freeList(r4);

    ListNode* r5 = s.rotateRight(createList({1}), 5);
    cout << "[1] k=5:         "; printList(r5); freeList(r5);

    ListNode* r6 = s.rotateRight(nullptr, 3);
    cout << "空链表 k=3:      "; printList(r6);

    ListNode* r7 = s.rotateRight(createList({1,2,3,4,5}), 0);
    cout << "[1,2,3,4,5] k=0: "; printList(r7); freeList(r7);
    cout << endl;
}


// ================================================================
// 题目12: 两两交换链表中的节点 (LeetCode 24, Medium)
// ================================================================
/*
 * 给你一个链表, 两两交换其中相邻的节点, 并返回交换后链表的头节点。
 * 你必须在不修改节点内部的值的情况下完成本题(即只能进行节点交换)。
 * 输入: 1->2->3->4  输出: 2->1->4->3
 *
 * ASCII图解 - 迭代法 (dummy head):
 *
 *   dummy -> 1 -> 2 -> 3 -> 4 -> NULL
 *
 *   第1轮: 交换1和2
 *     prev=dummy, first=1, second=2
 *     prev->next = 2       (dummy->2)
 *     first->next = second->next  (1->3)
 *     second->next = first  (2->1)
 *     结果: dummy -> 2 -> 1 -> 3 -> 4 -> NULL
 *     prev移到1(下一轮的前驱)
 *
 *   第2轮: 交换3和4
 *     prev=1, first=3, second=4
 *     prev->next = 4       (1->4)
 *     first->next = NULL   (3->NULL)
 *     second->next = first (4->3)
 *     结果: dummy -> 2 -> 1 -> 4 -> 3 -> NULL
 *     prev移到3
 *
 *   返回 dummy->next = 2
 *
 * ASCII图解 - 递归法:
 *
 *   swapPairs(1->2->3->4)
 *     = 2->1->swapPairs(3->4)
 *     = 2->1->4->3->swapPairs(NULL)
 *     = 2->1->4->3->NULL
 *
 *   递归树:
 *     swapPairs(1->2->3->4)
 *       第二个节点2成为新头
 *       1->next = swapPairs(3->4)
 *         第二个节点4成为新头
 *         3->next = swapPairs(NULL) = NULL
 *         返回 4->3->NULL
 *       1->next = 4->3->NULL
 *       2->1->4->3->NULL
 */

// 方法一: 迭代法 O(n)时间 O(1)空间
class Solution24_Iterative {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0, head);  // 哨兵头
        ListNode* prev = &dummy;

        while (prev->next && prev->next->next) {
            ListNode* first = prev->next;
            ListNode* second = first->next;

            // 交换操作 (三步)
            prev->next = second;            // 前驱指向第二个
            first->next = second->next;     // 第一个指向后面剩余
            second->next = first;           // 第二个指向第一个

            prev = first;  // prev移到已交换对的末尾(即原来的first)
        }
        return dummy.next;
    }
};

// 方法二: 递归法 O(n)时间 O(n)空间
class Solution24_Recursive {
public:
    ListNode* swapPairs(ListNode* head) {
        // base case: 不足两个节点, 不需要交换
        if (!head || !head->next) return head;

        ListNode* first = head;
        ListNode* second = head->next;

        // 递归处理后面的部分, 接到first后面
        first->next = swapPairs(second->next);
        // 交换: second成为新头
        second->next = first;

        return second;  // 返回新的头节点
    }
};

void test_24() {
    cout << "=== 题目12: 两两交换链表中的节点 (LeetCode 24) ===" << endl;
    Solution24_Iterative s1;
    Solution24_Recursive s2;

    ListNode* r1 = s1.swapPairs(createList({1,2,3,4}));
    cout << "迭代 [1,2,3,4]:     "; printList(r1); freeList(r1);

    ListNode* r2 = s2.swapPairs(createList({1,2,3,4}));
    cout << "递归 [1,2,3,4]:     "; printList(r2); freeList(r2);

    ListNode* r3 = s1.swapPairs(createList({1,2,3}));
    cout << "奇数个 [1,2,3]:     "; printList(r3); freeList(r3);

    ListNode* r4 = s2.swapPairs(createList({1}));
    cout << "单节点 [1]:         "; printList(r4); freeList(r4);

    ListNode* r5 = s1.swapPairs(nullptr);
    cout << "空链表:              "; printList(r5);

    ListNode* r6 = s2.swapPairs(createList({1,2,3,4,5,6}));
    cout << "六个节点 [1..6]:     "; printList(r6); freeList(r6);
    cout << endl;
}


// ==================== 主函数 ====================
int main() {
    cout << "============================================" << endl;
    cout << "  LeetCode经典链表问题 - 测试运行" << endl;
    cout << "============================================" << endl << endl;

    test_206();
    test_21();
    test_141();
    test_142();
    test_19();
    test_146();
    test_2();
    test_148();
    test_234();
    test_160();
    test_61();
    test_24();

    cout << "============================================" << endl;
    cout << "  所有测试用例执行完毕!" << endl;
    cout << "============================================" << endl;
    return 0;
}
