/**
 * ============================================================================
 *  第12章 - Trie字典树 与 红黑树
 * ============================================================================
 *
 *  本章包含:
 *  1. Trie 基础实现 (LeetCode 208, Medium)
 *  2. 添加与搜索单词 (LeetCode 211, Medium)
 *  3. 单词搜索 II (LeetCode 212, Hard)
 *  4. 红黑树实现 (Bonus 教学实现)
 *  5. 替换词 (LeetCode 648, Medium)
 *  6. 最大异或值 (LeetCode 421, Medium)
 *
 *  Trie (字典树) ASCII图示:
 *  存储 {"cat","car","card","dog","dot","do"} 的 Trie:
 *
 *              (root)
 *             /      \
 *           c         d
 *           |         |
 *           a         o
 *           |        / \
 *           t(*)    g(*) t(*)
 *           |              |
 *           r(*)           (null)
 *           |
 *           d(*)
 *
 *  (*) = isEnd = true, 表示该节点是某个单词的结尾
 *  每个节点: children[26] + isEnd
 *  时间: insert O(m), search O(m), startsWith O(m)
 *
 *  红黑树五大性质:
 *  1. 每个节点非红即黑
 *  2. 根节点是黑色
 *  3. 叶节点(NIL)是黑色
 *  4. 红色节点的子节点都是黑色 (不能有连续红色)
 *  5. 从任一节点到所有后代NIL的路径上, 黑色节点数相同
 *
 *  红黑树图示:
 *         [B:10]
 *        /      \
 *    [B:5]      [R:15]
 *    /    \     /     \
 *  [B:3] [B:7][B:12] [B:20]
 *  /  \         \
 * [R:1][R:4]   [R:13]
 *
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <functional>
using namespace std;


// ============================================================================
// 第1部分: Trie 基础实现 (LeetCode 208)
// ============================================================================
/**
 * 节点结构:
 *   ┌───┬───┬───┬───┬───┬─────────┬───┐
 *   │ a │ b │ c │ d │ e │   ...   │ z │  children[26]
 *   │ ↓ │ ↓ │ ↓ │ ↓ │ ↓ │         │ ↓ │
 *   │ptr│ptr│ptr│ptr│ptr│  null   │ptr│
 *   └───┴───┴───┴───┴───┴─────────┴───┘
 *   isEnd: 标记是否为单词结尾
 *
 * 插入 "cat" 然后插入 "car" 的过程:
 *   (root) --c--> [c] --a--> [a] --t--> [t*]
 *                                      \
 *                                       --r--> [r*]
 */
class Trie {
private:
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() : isEnd(false) {
            memset(children, 0, sizeof(children));
        }
    };
    TrieNode* root;

public:
    Trie() { root = new TrieNode(); }

    ~Trie() {
        function<void(TrieNode*)> destroy = [&](TrieNode* node) {
            if (!node) return;
            for (int i = 0; i < 26; i++) destroy(node->children[i]);
            delete node;
        };
        destroy(root);
    }

    /** 插入单词: 沿字符路径走, 不存在则创建新节点 */
    void insert(string word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx])
                cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    /** 搜索单词: 走完路径后检查 isEnd */
    bool search(string word) {
        TrieNode* node = findNode(word);
        return node != nullptr && node->isEnd;
    }

    /** 前缀匹配: 只要能走完前缀路径即可 */
    bool startsWith(string prefix) {
        return findNode(prefix) != nullptr;
    }

private:
    TrieNode* findNode(string& str) {
        TrieNode* cur = root;
        for (char ch : str) {
            int idx = ch - 'a';
            if (!cur->children[idx]) return nullptr;
            cur = cur->children[idx];
        }
        return cur;
    }
};


// ============================================================================
// 第2部分: 添加与搜索单词 (LeetCode 211)
// ============================================================================
/**
 * 支持 '.' 通配符的字典搜索
 *
 * search("b.t") 的 DFS 回溯过程 (Trie含 {bat, bit, bot, cat}):
 *
 *   Trie:
 *              (root)
 *             /      \
 *           b         c
 *          /|\         |
 *         a i o        a
 *         | | |        |
 *         t t t        t
 *
 *   DFS(b, index=1): ch='.' 通配符, 尝试所有子节点
 *   ├─ DFS(a, index=2): ch='t', a->t 存在且isEnd -> 找到 "bat"!
 *   ├─ 若没找到, DFS(i, index=2): ch='t', i->t 存在 -> 找到 "bit"!
 *   └─ 若没找到, DFS(o, index=2): ch='t', o->t 存在 -> 找到 "bot"!
 */
class WordDictionary {
private:
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() : isEnd(false) {
            memset(children, 0, sizeof(children));
        }
    };
    TrieNode* root;

    /**
     * DFS 搜索: 遇到 '.' 时回溯尝试所有可能子节点
     * 这是核心: 通配符让搜索变成树形回溯
     */
    bool dfs(TrieNode* node, string& word, int index) {
        if (!node) return false;
        if (index == (int)word.size()) return node->isEnd;

        char ch = word[index];
        if (ch == '.') {
            // 通配符: 遍历所有非空子节点, 任一成功即返回true
            for (int i = 0; i < 26; i++) {
                if (node->children[i] &&
                    dfs(node->children[i], word, index + 1))
                    return true;
            }
            return false; // 所有子节点都不匹配
        } else {
            return dfs(node->children[ch - 'a'], word, index + 1);
        }
    }

public:
    WordDictionary() { root = new TrieNode(); }
    ~WordDictionary() {
        function<void(TrieNode*)> destroy = [&](TrieNode* node) {
            if (!node) return;
            for (int i = 0; i < 26; i++) destroy(node->children[i]);
            delete node;
        };
        destroy(root);
    }

    void addWord(string word) {
        TrieNode* cur = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!cur->children[idx])
                cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->isEnd = true;
    }

    bool search(string word) {
        return dfs(root, word, 0);
    }
};


// ============================================================================
// 第3部分: 单词搜索 II (LeetCode 212, Hard)
// ============================================================================
/**
 * 为什么 Trie + DFS 高效?
 *
 *   暴力法: 对每个单词在网格DFS -> O(W * M * N * 4^L), 非常慢
 *   Trie法: 所有单词建Trie, 网格DFS沿Trie走
 *   - 关键: 路径不在Trie中时立即剪枝! 大幅减少搜索空间
 *   - 构建Trie: O(W*L), 网格DFS: O(M*N*4^L)但剪枝后远小于此
 *
 * ASCII图示:
 *   board:        Trie (words=["oath","pea","eat","rain"]):
 *   o a a n              (root)
 *   n t a e             / |  \
 *   i h k r            o  p   e  r
 *   i f l v            |  |   |  |
 *                      a  e   a  a
 *   从(0,0)='o'出发:   |  |   |  |
 *   o->a->t->h: 找到!  t  a(*) t  i
 *   o->a->t->a: 剪枝!  |      (*) n(*)
 *   o->n:       剪枝!  h
 *                      |
 *                     (*)
 *
 * 剪枝: 当前字符在Trie子节点中不存在 => 不需要继续搜索
 */
vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
    // 构建 Trie, 每个结尾节点存储完整单词
    struct TrieNode {
        TrieNode* children[26];
        string word; // 非空表示找到一个完整单词
        TrieNode() : word("") {
            memset(children, 0, sizeof(children));
        }
    };
    TrieNode* root = new TrieNode();
    for (string& w : words) {
        TrieNode* cur = root;
        for (char ch : w) {
            int idx = ch - 'a';
            if (!cur->children[idx])
                cur->children[idx] = new TrieNode();
            cur = cur->children[idx];
        }
        cur->word = w;
    }

    vector<string> result;
    int m = board.size(), n = board[0].size();
    int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};

    // DFS: 从board[x][y]出发, 沿Trie路径搜索
    function<void(int, int, TrieNode*)> dfs = [&](int x, int y, TrieNode* node) {
        if (x < 0 || x >= m || y < 0 || y >= n) return;
        char ch = board[x][y];
        // 剪枝: 已访问('#') 或 该字符不在Trie子节点中
        if (ch == '#' || !node->children[ch - 'a']) return;

        node = node->children[ch - 'a'];
        if (!node->word.empty()) {
            result.push_back(node->word);
            node->word = ""; // 防止重复添加
        }

        board[x][y] = '#'; // 标记已访问
        for (int d = 0; d < 4; d++)
            dfs(x + dx[d], y + dy[d], node);
        board[x][y] = ch; // 回溯恢复
    };

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            dfs(i, j, root);

    // 释放Trie
    function<void(TrieNode*)> destroy = [&](TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; i++) destroy(node->children[i]);
        delete node;
    };
    destroy(root);
    return result;
}


// ============================================================================
// 第4部分: 红黑树实现 (Bonus 教学实现)
// ============================================================================
/**
 * 旋转操作图示:
 *
 * 左旋(x):            右旋(y):
 *     x    ->    y       y    ->    x
 *    / \        / \     / \        / \
 *   a   y      x   c   x   c     a   y
 *      / \    / \      / \          / \
 *     b   c  a   b    a   b        b   c
 *
 * 插入后三种调整情况:
 *
 * 情况1: 叔叔红 -> 父叔变黑, 祖父变红, 继续向上
 *        [G]B            [G']R  <- 继续检查
 *       /   \    =>     /    \
 *    [P]R   [U]R     [P]B   [U]B
 *    /                /
 *  [C]R            [C]R
 *
 * 情况2: 叔叔黑, 当前是右子 -> 左旋父, 转为情况3
 * 情况3: 叔叔黑, 当前是左子 -> 父变黑, 祖父变红, 右旋祖父
 */
enum Color { RED, BLACK };

struct RBNode {
    int key;
    Color color;
    RBNode *left, *right, *parent;
    RBNode(int k) : key(k), color(RED), left(nullptr),
                    right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    RBNode* root;
    RBNode* NIL; // 黑色哨兵节点

    void leftRotate(RBNode* x) {
        RBNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent) root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBNode* y) {
        RBNode* x = y->left;
        y->left = x->right;
        if (x->right != NIL) x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent) root = x;
        else if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
        x->right = y;
        y->parent = x;
    }

    /** 修复插入后的红黑树性质违规 */
    void insertFixup(RBNode* z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                RBNode* uncle = z->parent->parent->right;
                if (uncle->color == RED) {
                    // 情况1: 叔叔红 -> 变色, 继续向上
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        // 情况2: 叔叔黑, z是右子 -> 左旋转为情况3
                        z = z->parent;
                        leftRotate(z);
                    }
                    // 情况3: 叔叔黑, z是左子 -> 变色+右旋
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                // 镜像: 父节点是祖父的右子
                RBNode* uncle = z->parent->parent->left;
                if (uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK; // 性质2: 根必须黑
    }

    void inorderHelper(RBNode* node) {
        if (node == NIL) return;
        inorderHelper(node->left);
        cout << node->key << "(" << (node->color == RED ? "R" : "B") << ") ";
        inorderHelper(node->right);
    }

    void printHelper(RBNode* node, string prefix, bool isLeft) {
        if (node == NIL) return;
        cout << prefix << (isLeft ? "|-- " : "\\-- ")
             << node->key << (node->color == RED ? "(R)" : "(B)") << endl;
        printHelper(node->left, prefix + (isLeft ? "|   " : "    "), true);
        printHelper(node->right, prefix + (isLeft ? "|   " : "    "), false);
    }

    void destroyTree(RBNode* node) {
        if (node == NIL) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    RBNode* searchHelper(RBNode* node, int key) {
        while (node != NIL) {
            if (key == node->key) return node;
            node = (key < node->key) ? node->left : node->right;
        }
        return nullptr;
    }

public:
    RedBlackTree() {
        NIL = new RBNode(0);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = nullptr;
        root = NIL;
    }
    ~RedBlackTree() { destroyTree(root); delete NIL; }

    /**
     * 插入: 新节点默认红色(避免违反性质5), 然后修复性质4
     */
    void insert(int key) {
        RBNode* z = new RBNode(key);
        z->left = z->right = NIL;

        RBNode* y = nullptr;
        RBNode* x = root;
        while (x != NIL) {
            y = x;
            if (key < x->key) x = x->left;
            else if (key > x->key) x = x->right;
            else { delete z; return; } // 重复键
        }

        z->parent = y;
        if (!y) root = z;
        else if (key < y->key) y->left = z;
        else y->right = z;

        insertFixup(z);
    }

    bool search(int key) { return searchHelper(root, key) != nullptr; }

    void inorder() { inorderHelper(root); cout << endl; }

    void print() {
        if (root == NIL) { cout << "(空树)" << endl; return; }
        printHelper(root, "", false);
    }

    /** 验证红黑树所有性质 */
    bool verify() {
        if (root == NIL) return true;
        if (root->color != BLACK) { cout << "违反性质2" << endl; return false; }

        function<int(RBNode*)> check = [&](RBNode* node) -> int {
            if (node == NIL) return 1;
            if (node->color == RED) {
                if (node->left->color == RED || node->right->color == RED) {
                    cout << "违反性质4: 节点" << node->key << endl;
                    return -1;
                }
            }
            int l = check(node->left), r = check(node->right);
            if (l == -1 || r == -1) return -1;
            if (l != r) {
                cout << "违反性质5: 节点" << node->key << endl;
                return -1;
            }
            return l + (node->color == BLACK ? 1 : 0);
        };
        return check(root) != -1;
    }
};


// ============================================================================
// 第5部分: 替换词 (LeetCode 648, Medium)
// ============================================================================
/**
 * 问题描述:
 *   在英语中, 我们有一个叫做 "词根"(root) 的概念,
 *   它可以跟其他一些词组成另一个较长的单词, 我们称这个词为 "继承词"(successor)。
 *   例如, 词根 "an" 跟单词 "other" 组成继承词 "another"。
 *   现在给定一个由许多词根组成的词典 dictionary 和一个用空格分隔的句子 sentence。
 *   请你将句子中的所有继承词用词根替换掉。如果继承词有许多词根, 则用最短的词根替换。
 *   返回替换后的句子。
 *
 * 示例:
 *   输入: dictionary = ["cat","bat","rat"],
 *         sentence = "the cattle was rattled by the battery"
 *   输出: "the cat was rat by the bat"
 *
 *   输入: dictionary = ["a","b","c"],
 *         sentence = "aadsfasf absbs bbab cadsfafs"
 *   输出: "a a b c"
 *
 * 解题思路 (Trie):
 *   1. 将所有词根插入 Trie
 *   2. 对句子中的每个单词, 沿 Trie 查找最短前缀匹配
 *   3. 如果找到前缀匹配, 用词根替换; 否则保留原单词
 *
 * ASCII图示:
 *
 *   dictionary = ["cat","bat","rat"]
 *
 *   Trie:
 *              (root)
 *             /  |  \
 *            c   b   r
 *            |   |   |
 *            a   a   a
 *            |   |   |
 *            t   t   t
 *
 *   单词 "cattle" 查找过程:
 *   c -> a -> t: 找到匹配 "cat"! 替换为 "cat"
 *
 *   单词 "battery" 查找过程:
 *   b -> a -> t: 找到匹配 "bat"! 替换为 "bat"
 *
 *   单词 "the" 查找过程:
 *   t: 无子节点匹配, 保留 "the"
 *
 * 时间复杂度: O(D*L + S*W), D=词根数, L=词根平均长度, S=句子长度, W=单词平均长度
 * 空间复杂度: O(D*L)
 */
class ReplaceWords {
private:
    struct TrieNode {
        TrieNode* children[26];
        bool isEnd;
        TrieNode() : isEnd(false) {
            memset(children, 0, sizeof(children));
        }
    };
    TrieNode* root;

    // 在Trie中查找word的最短前缀词根
    string findRoot(const string& word) {
        TrieNode* cur = root;
        for (int i = 0; i < (int)word.size(); i++) {
            int idx = word[i] - 'a';
            if (!cur->children[idx]) return word; // 无匹配, 返回原词
            cur = cur->children[idx];
            if (cur->isEnd) return word.substr(0, i + 1); // 找到最短词根
        }
        return word; // 走完了也没找到isEnd, 返回原词
    }

public:
    ReplaceWords() { root = new TrieNode(); }

    ~ReplaceWords() {
        function<void(TrieNode*)> destroy = [&](TrieNode* node) {
            if (!node) return;
            for (int i = 0; i < 26; i++) destroy(node->children[i]);
            delete node;
        };
        destroy(root);
    }

    // 将所有词根插入Trie
    void buildTrie(vector<string>& dictionary) {
        for (string& word : dictionary) {
            TrieNode* cur = root;
            for (char ch : word) {
                int idx = ch - 'a';
                if (!cur->children[idx])
                    cur->children[idx] = new TrieNode();
                cur = cur->children[idx];
            }
            cur->isEnd = true;
        }
    }

    string replaceWords(vector<string>& dictionary, string sentence) {
        // 1. 构建Trie
        buildTrie(dictionary);

        // 2. 按空格分割句子, 逐词替换
        string result;
        string word;
        for (int i = 0; i <= (int)sentence.size(); i++) {
            if (i == (int)sentence.size() || sentence[i] == ' ') {
                if (!word.empty()) {
                    if (!result.empty()) result += " ";
                    result += findRoot(word);
                    word.clear();
                }
            } else {
                word += sentence[i];
            }
        }
        return result;
    }
};


// ============================================================================
// 第6部分: 最大异或值 (LeetCode 421, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给你一个整数数组 nums, 返回 nums[i] XOR nums[j] 的最大运算结果,
 *   其中 0 <= i < j < n。
 *
 * 示例:
 *   输入: nums = [3,10,5,25,2,8]
 *   输出: 28
 *   解释: 5 XOR 25 = 28 (二进制: 0101 ^ 11000 = 11100 = 28)
 *
 *   输入: nums = [14,70,53,83,49,91,36,80,92,51,66,70]
 *   输出: 127
 *
 * 解题思路 (Trie + 位操作):
 *   核心思想: 对于每个数, 在已有的数中找到能产生最大异或值的配对。
 *
 *   异或的性质: a XOR b 中, 如果某一位不同, 该位结果为1。
 *   所以要让异或值最大, 就要尽可能让高位不同!
 *
 *   用Trie存储所有数的二进制位(从高位到低位, 31位)。
 *   对于每个数, 在Trie中贪心地走"相反位"路径:
 *   - 当前位是0, 优先走1方向 (异或结果为1)
 *   - 当前位是1, 优先走0方向 (异或结果为1)
 *   - 如果相反方向不存在, 只能走相同方向 (异或结果为0)
 *
 * ASCII图示:
 *
 *   nums = [3, 10, 5] (用4位简化表示)
 *
 *   3  = 0011
 *   10 = 1010
 *   5  = 0101
 *
 *   Trie (从高位到低位):
 *              (root)
 *             /      \
 *           0(位3)   1(位3)
 *          /    \       |
 *        0(位2) 1(位2) 0(位2)
 *         |      |      |
 *       0(位1) 1(位1) 1(位1)
 *         |      |      |
 *       1(位0) 0(位0) 0(位0)
 *
 *   查找与5(0101)配对的最大异或:
 *   位3: 5的位3=0, 优先走1 -> 走到 1xxx 路径 (异或=1)
 *   位2: 5的位2=1, 优先走0 -> 走到 10xx 路径 (异或=1)
 *   位1: 5的位1=0, 优先走1 -> 走到 101x 路径 (异或=1)
 *   位0: 5的位0=1, 优先走0 -> 走到 1010 路径 (异或=1)
 *   找到 1010(=10), 5 XOR 10 = 1111 = 15
 *
 * 时间复杂度: O(n * 32) = O(n)
 * 空间复杂度: O(n * 32) = O(n)
 */
int findMaximumXOR(vector<int>& nums) {
    // Trie节点: 只有0和1两个子节点
    struct BitTrieNode {
        BitTrieNode* children[2]; // 0和1
        BitTrieNode() {
            children[0] = children[1] = nullptr;
        }
    };

    BitTrieNode* root = new BitTrieNode();

    // 插入一个数的二进制位到Trie (从最高位31到最低位0)
    auto insert = [&](int num) {
        BitTrieNode* cur = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!cur->children[bit])
                cur->children[bit] = new BitTrieNode();
            cur = cur->children[bit];
        }
    };

    // 查询与num异或值最大的已有数
    auto query = [&](int num) -> int {
        BitTrieNode* cur = root;
        int maxXor = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int desired = 1 - bit; // 期望走相反位
            if (cur->children[desired]) {
                // 相反位存在, 该位异或结果为1
                maxXor |= (1 << i);
                cur = cur->children[desired];
            } else {
                // 只能走相同位, 该位异或结果为0
                cur = cur->children[bit];
            }
        }
        return maxXor;
    };

    int result = 0;
    for (int num : nums) {
        insert(num);
        result = max(result, query(num));
    }

    // 释放Trie
    function<void(BitTrieNode*)> destroy = [&](BitTrieNode* node) {
        if (!node) return;
        destroy(node->children[0]);
        destroy(node->children[1]);
        delete node;
    };
    destroy(root);

    return result;
}


// ============================================================================
// 测试用例
// ============================================================================

void testTrie() {
    cout << "========================================" << endl;
    cout << "题目1: Trie 基础实现 (LeetCode 208)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    Trie结构 (apple, app, apt, bat, ban, band):
             (root)
            /      \
           a         b
           |         |
           p         a
          / \       / \
         p   t(*)  t(*) n(*)
        / \             |
      (*)  l            d(*)
            |
            e(*)
    )" << endl;

    Trie trie;
    trie.insert("apple"); trie.insert("app"); trie.insert("apt");
    trie.insert("bat"); trie.insert("ban"); trie.insert("band");

    cout << "search(\"apple\")  = " << (trie.search("apple")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"app\")    = " << (trie.search("app")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"ap\")     = " << (trie.search("ap")?"true":"false")
         << " (期望: false)" << endl;
    cout << "search(\"apt\")    = " << (trie.search("apt")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"bat\")    = " << (trie.search("bat")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"band\")   = " << (trie.search("band")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"bank\")   = " << (trie.search("bank")?"true":"false")
         << " (期望: false)" << endl;
    cout << "startsWith(\"app\")= " << (trie.startsWith("app")?"true":"false")
         << " (期望: true)" << endl;
    cout << "startsWith(\"xyz\")= " << (trie.startsWith("xyz")?"true":"false")
         << " (期望: false)" << endl;
    cout << endl;
}

void testWordDictionary() {
    cout << "========================================" << endl;
    cout << "题目2: 添加与搜索单词 (LeetCode 211)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    DFS回溯: search("b.t") 在 {bad,bat,bid,bit,cat} 中搜索
    ├─ '.' 通配符 -> 尝试 a: a没有t子节点 -> 回溯
    ├─ 尝试 d: d->t 存在且isEnd -> 找到 "bat"!
    └─ (若没找到) 尝试 i: i->t 存在 -> 找到 "bit"!
    )" << endl;

    WordDictionary wd;
    wd.addWord("bad"); wd.addWord("bat"); wd.addWord("bid");
    wd.addWord("bit"); wd.addWord("cat");

    cout << "已添加: bad, bat, bid, bit, cat" << endl;
    cout << "search(\"bad\") = " << (wd.search("bad")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"b.d\") = " << (wd.search("b.d")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"b.t\") = " << (wd.search("b.t")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\".at\") = " << (wd.search(".at")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"...\") = " << (wd.search("...")?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(\"b.g\") = " << (wd.search("b.g")?"true":"false")
         << " (期望: false)" << endl;
    cout << endl;
}

void testFindWords() {
    cout << "========================================" << endl;
    cout << "题目3: 单词搜索 II (LeetCode 212)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    网格:         Trie剪枝示例:
    o a a n       从(0,0)='o'出发:
    n t a e       o->a->t->h: 找到 "oath"!
    i h k r       o->a->t->a: t后无a -> 剪枝!
    i f l v       o->n:       o后无n -> 剪枝!
    )" << endl;

    vector<vector<char>> board = {
        {'o','a','a','n'}, {'e','t','a','e'},
        {'i','h','k','r'}, {'i','f','l','v'}
    };
    vector<string> words = {"oath", "pea", "eat", "rain"};

    cout << "网格:" << endl;
    for (auto& row : board) {
        cout << "  ";
        for (char c : row) cout << c << " ";
        cout << endl;
    }

    vector<string> result = findWords(board, words);
    cout << "单词列表: [oath, pea, eat, rain]" << endl;
    cout << "找到: [";
    for (int i = 0; i < (int)result.size(); i++) {
        if (i) cout << ", ";
        cout << result[i];
    }
    cout << "]" << endl;
    cout << "期望: [oath, eat] (pea和rain不在网格中)" << endl;
    cout << endl;
}

void testRedBlackTree() {
    cout << "========================================" << endl;
    cout << "题目4: 红黑树实现 (Bonus)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    插入 10,20,30 演示:
    插入10: [10](B)          插入20:  [10](B)
                                      \
                                      [20](R)

    插入30: 违反性质4 -> 左旋+变色:
            [10](B)           [20](B)
                \            /     \
              [20](R)    [10](R)  [30](R)
                  \
                [30](R)
    )" << endl;

    RedBlackTree rbt;
    int values[] = {10, 20, 30, 15, 25, 5, 1, 12, 18, 22, 28};

    cout << "依次插入: ";
    for (int v : values) { cout << v << " "; rbt.insert(v); }
    cout << endl << endl;

    cout << "中序遍历: ";
    rbt.inorder();
    cout << endl;

    cout << "树形结构:" << endl;
    rbt.print();
    cout << endl;

    cout << "性质验证: " << (rbt.verify() ? "通过!" : "失败!") << endl;
    cout << endl;

    cout << "查找测试:" << endl;
    cout << "  search(15) = " << (rbt.search(15)?"true":"false")
         << " (期望: true)" << endl;
    cout << "  search(25) = " << (rbt.search(25)?"true":"false")
         << " (期望: true)" << endl;
    cout << "  search(99) = " << (rbt.search(99)?"true":"false")
         << " (期望: false)" << endl;

    // 大量插入验证
    cout << endl << "--- 大量插入测试 (1~500) ---" << endl;
    RedBlackTree bigTree;
    for (int i = 1; i <= 500; i++) bigTree.insert(i);
    cout << "插入1~500, 性质验证: "
         << (bigTree.verify() ? "通过!" : "失败!") << endl;
    cout << "search(250)  = " << (bigTree.search(250)?"true":"false")
         << " (期望: true)" << endl;
    cout << "search(501)  = " << (bigTree.search(501)?"true":"false")
         << " (期望: false)" << endl;
    cout << endl;
}

void testReplaceWords() {
    cout << "========================================" << endl;
    cout << "题目5: 替换词 (LeetCode 648)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    dictionary = ["cat","bat","rat"]

    Trie:
             (root)
            /  |  \
           c   b   r
           |   |   |
           a   a   a
           |   |   |
           t   t   t

    "cattle" -> c->a->t: 匹配 "cat"! 替换
    "rattled" -> r->a->t: 匹配 "rat"! 替换
    "battery" -> b->a->t: 匹配 "bat"! 替换
    "the" -> t: 无子节点, 保留原词
    )" << endl;

    ReplaceWords sol1;
    vector<string> dict1 = {"cat", "bat", "rat"};
    string sentence1 = "the cattle was rattled by the battery";
    string result1 = sol1.replaceWords(dict1, sentence1);
    cout << "输入句子: \"" << sentence1 << "\"" << endl;
    cout << "输出: \"" << result1 << "\"" << endl;
    cout << "期望: \"the cat was rat by the bat\"" << endl;
    cout << "正确: " << (result1 == "the cat was rat by the bat" ? "是" : "否") << endl << endl;

    // 测试用例2
    ReplaceWords sol2;
    vector<string> dict2 = {"a", "b", "c"};
    string sentence2 = "aadsfasf absbs bbab cadsfafs";
    string result2 = sol2.replaceWords(dict2, sentence2);
    cout << "输入: dict=[\"a\",\"b\",\"c\"]" << endl;
    cout << "句子: \"" << sentence2 << "\"" << endl;
    cout << "输出: \"" << result2 << "\"" << endl;
    cout << "期望: \"a a b c\"" << endl;
    cout << "正确: " << (result2 == "a a b c" ? "是" : "否") << endl << endl;

    // 测试用例3: 无匹配
    ReplaceWords sol3;
    vector<string> dict3 = {"cat", "bat", "rat"};
    string sentence3 = "hello world";
    string result3 = sol3.replaceWords(dict3, sentence3);
    cout << "输入: dict=[\"cat\",\"bat\",\"rat\"]" << endl;
    cout << "句子: \"" << sentence3 << "\"" << endl;
    cout << "输出: \"" << result3 << "\"" << endl;
    cout << "期望: \"hello world\" (无匹配, 保留原词)" << endl;
    cout << "正确: " << (result3 == "hello world" ? "是" : "否") << endl << endl;
}

void testFindMaximumXOR() {
    cout << "========================================" << endl;
    cout << "题目6: 最大异或值 (LeetCode 421)" << endl;
    cout << "========================================" << endl;

    cout << R"(
    异或(XOR)性质: 不同为1, 相同为0
    要最大化异或值 -> 尽量让高位不同!

    Trie位查找策略:
    - 当前位=0, 优先走1方向 (异或结果=1)
    - 当前位=1, 优先走0方向 (异或结果=1)
    - 相反方向不存在 -> 只能走相同方向 (异或结果=0)
    )" << endl;

    vector<int> nums1 = {3, 10, 5, 25, 2, 8};
    cout << "--- 测试用例 1 ---" << endl;
    cout << "输入: [3, 10, 5, 25, 2, 8]" << endl;

    // 展示二进制
    cout << "二进制:" << endl;
    for (int x : nums1) {
        cout << "  " << x << " = ";
        for (int i = 7; i >= 0; i--) cout << ((x >> i) & 1);
        cout << endl;
    }
    cout << "  5  = 00000101" << endl;
    cout << "  25 = 00011001" << endl;
    cout << "  5 XOR 25 = 00011100 = 28" << endl;

    int result1 = findMaximumXOR(nums1);
    cout << "最大异或值: " << result1 << " (期望: 28)" << endl;
    cout << "正确: " << (result1 == 28 ? "是" : "否") << endl << endl;

    // 测试用例2
    vector<int> nums2 = {14, 70, 53, 83, 49, 91, 36, 80, 92, 51, 66, 70};
    cout << "--- 测试用例 2 ---" << endl;
    cout << "输入: [14,70,53,83,49,91,36,80,92,51,66,70]" << endl;
    int result2 = findMaximumXOR(nums2);
    cout << "最大异或值: " << result2 << " (期望: 127)" << endl;
    cout << "正确: " << (result2 == 127 ? "是" : "否") << endl << endl;

    // 测试用例3
    vector<int> nums3 = {1};
    cout << "--- 测试用例 3 ---" << endl;
    cout << "输入: [1]" << endl;
    int result3 = findMaximumXOR(nums3);
    cout << "最大异或值: " << result3 << " (期望: 0)" << endl;
    cout << "正确: " << (result3 == 0 ? "是" : "否") << endl << endl;

    // 测试用例4
    vector<int> nums4 = {0, 0};
    cout << "--- 测试用例 4 ---" << endl;
    cout << "输入: [0, 0]" << endl;
    int result4 = findMaximumXOR(nums4);
    cout << "最大异或值: " << result4 << " (期望: 0)" << endl;
    cout << "正确: " << (result4 == 0 ? "是" : "否") << endl << endl;

    // 测试用例5: 全1和全0
    vector<int> nums5 = {2147483647, 0}; // INT_MAX 和 0
    cout << "--- 测试用例 5 ---" << endl;
    cout << "输入: [2147483647, 0]" << endl;
    int result5 = findMaximumXOR(nums5);
    cout << "最大异或值: " << result5 << " (期望: 2147483647)" << endl;
    cout << "正确: " << (result5 == 2147483647 ? "是" : "否") << endl << endl;
}


// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "********************************************" << endl;
    cout << "*   第12章 - Trie字典树 与 红黑树            *" << endl;
    cout << "********************************************" << endl << endl;

    testTrie();
    testWordDictionary();
    testFindWords();
    testRedBlackTree();
    testReplaceWords();
    testFindMaximumXOR();

    cout << "=========================================" << endl;
    cout << "  所有测试用例执行完毕!" << endl;
    cout << "=========================================" << endl;
    return 0;
}
