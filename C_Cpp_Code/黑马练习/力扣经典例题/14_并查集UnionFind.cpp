/*
 * =====================================================================================
 *  并查集 (Union-Find / Disjoint Set Union)
 * =====================================================================================
 *
 *  并查集是一种用于处理不相交集合的合并与查询问题的数据结构。
 *  主要支持两种操作：
 *    - Find:   查找元素属于哪个集合（确定根节点）
 *    - Union:  合并两个元素所在的集合
 *
 *  应用场景:
 *    - 判断图的连通性
 *    - 检测无向图中的环
 *    - 求连通分量个数
 *    - Kruskal最小生成树算法
 *    - 等价类合并
 *
 *  本文件包含:
 *    1. 并查集三种实现 (Quick Find / Quick Union / 优化版)
 *    2. LeetCode 684  - 冗余连接 (检测环)
 *    3. LeetCode 547  - 省份数量 (连通分量)
 *    4. LeetCode 721  - 账户合并
 *    5. LeetCode 1319 - 连通网络的操作次数
 *    6. LeetCode 785  - 判断二分图
 *    7. LeetCode 1631 - 最小体力消耗值
 *
 *  编译: g++ -std=c++17 -O2 -o unionfind 14_并查集UnionFind.cpp
 *  运行: ./unionfind
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iomanip>
#include <queue>

using namespace std;

// 分隔线
void separator(const string& title) {
    cout << "\n" << string(70, '=') << endl;
    cout << "  " << title << endl;
    cout << string(70, '=') << endl;
}


// ============================================================================
//  1. 并查集基础实现
// ============================================================================
/*
 * 三种实现方式的对比:
 *
 * ---- Quick Find ----
 *   思想: 同一个集合中的所有元素指向相同的id值
 *   Find:  O(1) - 直接查表
 *   Union: O(n) - 需要遍历整个数组更新所有相同id的元素
 *
 *   数组: id[i] 表示元素i所属集合的标识
 *   例: id = [0, 0, 0, 1, 1] 表示 {0,1,2} 和 {3,4} 两个集合
 *
 * ---- Quick Union ----
 *   思想: 用树形结构表示集合，每个元素指向父节点，根节点是集合代表
 *   Find:  O(h) - h为树高，需要沿父节点链找到根
 *   Union: O(h) - 合并两棵树
 *
 *   数组: parent[i] 表示元素i的父节点，根节点的parent[i] = i
 *
 *   树形结构示意:
 *     0         3
 *    / \        |
 *   1   2       4
 *
 *   parent = [0, 0, 0, 3, 3]
 *
 * ---- 加权Quick Union + 路径压缩 ----
 *   优化1: 加权合并 - 将小树挂到大树上，控制树高
 *   优化2: 路径压缩 - Find时将沿途节点直接指向根
 *   Find:  近似 O(1) (反阿克曼函数)
 *   Union: 近似 O(1)
 */

// ===========================
// 1.1 Quick Find 实现
// ===========================
class QuickFind {
private:
    vector<int> id;  // id[i] = 元素i所属集合的标识
    int count;        // 连通分量个数

public:
    QuickFind(int n) : id(n), count(n) {
        // 初始时，每个元素自成一个集合
        for (int i = 0; i < n; i++) {
            id[i] = i;
        }
    }

    // 查找元素p所属集合 - O(1)
    int find(int p) {
        return id[p];
    }

    // 判断两个元素是否在同一集合 - O(1)
    bool connected(int p, int q) {
        return find(p) == find(q);
    }

    // 合并元素p和q所在的集合 - O(n)
    void unite(int p, int q) {
        int pid = find(p);
        int qid = find(q);
        if (pid == qid) return;  // 已在同一集合

        // 将所有属于pid集合的元素改为qid集合
        for (int i = 0; i < (int)id.size(); i++) {
            if (id[i] == pid) {
                id[i] = qid;
            }
        }
        count--;
    }

    int getCount() { return count; }

    // 打印内部状态
    void printState() {
        cout << "    id数组:    [";
        for (int i = 0; i < (int)id.size(); i++) {
            cout << id[i];
            if (i < (int)id.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "    连通分量数: " << count << endl;
    }
};

// ===========================
// 1.2 Quick Union 实现
// ===========================
class QuickUnion {
private:
    vector<int> parent;  // parent[i] = 元素i的父节点
    int count;            // 连通分量个数

public:
    QuickUnion(int n) : parent(n), count(n) {
        // 初始时，每个元素是自己的父节点（自己就是根）
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // 查找元素p的根节点 - O(h)，h为树高
    int find(int p) {
        while (parent[p] != p) {
            p = parent[p];
        }
        return p;
    }

    // 判断两个元素是否在同一集合
    bool connected(int p, int q) {
        return find(p) == find(q);
    }

    // 合并元素p和q所在的集合 - O(h)
    void unite(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;

        // 将一棵树的根指向另一棵树的根
        parent[rootP] = rootQ;
        count--;
    }

    int getCount() { return count; }

    // 打印树形结构
    void printState() {
        cout << "    parent数组: [";
        for (int i = 0; i < (int)parent.size(); i++) {
            cout << parent[i];
            if (i < (int)parent.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "    连通分量数: " << count << endl;

        // 打印树形结构
        cout << "    树形结构:" << endl;
        // 找到所有根节点
        unordered_map<int, vector<int>> trees;
        for (int i = 0; i < (int)parent.size(); i++) {
            trees[find(i)].push_back(i);
        }
        for (auto& [root, nodes] : trees) {
            cout << "      根" << root << ": ";
            for (int node : nodes) {
                if (node != root) cout << node << " ";
            }
            cout << "(根:" << root << ")" << endl;
        }
    }
};

// ===========================
// 1.3 加权Quick Union + 路径压缩
// ===========================
class WeightedUnionFind {
private:
    vector<int> parent;  // parent[i] = 元素i的父节点
    vector<int> size;    // size[i] = 以i为根的树的大小(元素个数)
    int count;            // 连通分量个数

public:
    WeightedUnionFind(int n) : parent(n), size(n, 1), count(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // 查找根节点 (带路径压缩)
    // 路径压缩: 将查找路径上的所有节点直接指向根
    int find(int p) {
        /*
         * 路径压缩示意:
         * 压缩前:          压缩后:
         *     0               0
         *     |              /|\
         *     1             1 2 3
         *     |
         *     2
         *     |
         *     3
         *
         * find(3)后，路径上所有节点直接指向根0
         */
        while (parent[p] != p) {
            // 路径压缩: 将p指向其祖父节点(隔代压缩)
            parent[p] = parent[parent[p]];
            p = parent[p];
        }
        return p;
        // 完全路径压缩版本(递归):
        // if (parent[p] != p) parent[p] = find(parent[p]);
        // return parent[p];
    }

    // 判断两个元素是否在同一集合
    bool connected(int p, int q) {
        return find(p) == find(q);
    }

    // 加权合并
    void unite(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;

        /*
         * 加权合并示意:
         *
         * 合并前:  小树(rootP, size=2)  大树(rootQ, size=4)
         *              0                    3
         *             /                   / | \
         *            1                   4  5  6
         *
         * 合并后: 小树挂到大树上
         *                  3
         *                / | \  \
         *               4  5  6  0
         *                         |
         *                         1
         *
         * 这样保证树高增长最慢
         */
        if (size[rootP] < size[rootQ]) {
            parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        } else {
            parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }
        count--;
    }

    // 获取p所在集合的大小
    int getSize(int p) {
        return size[find(p)];
    }

    int getCount() { return count; }

    void printState() {
        cout << "    parent数组: [";
        for (int i = 0; i < (int)parent.size(); i++) {
            cout << parent[i];
            if (i < (int)parent.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "    size数组:   [";
        for (int i = 0; i < (int)size.size(); i++) {
            cout << size[i];
            if (i < (int)size.size() - 1) cout << ", ";
        }
        cout << "]" << endl;
        cout << "    连通分量数: " << count << endl;
    }
};


void testUnionFindBasics() {
    separator("1. 并查集基础实现");
    cout << endl;

    // ---- Quick Find 演示 ----
    cout << "========== Quick Find ==========" << endl;
    cout << "初始: 5个元素，各自独立" << endl;
    QuickFind qf(5);
    qf.printState();

    cout << "\n合并 0 和 1:" << endl;
    cout << "  unite(0, 1): 将id[0]=0改为id[1]=1" << endl;
    qf.unite(0, 1);
    qf.printState();

    cout << "\n合并 2 和 3:" << endl;
    qf.unite(2, 3);
    qf.printState();

    cout << "\n合并 1 和 3:" << endl;
    cout << "  unite(1, 3): 需要遍历整个数组，将所有id=1的改为id=3" << endl;
    qf.unite(1, 3);
    qf.printState();

    cout << "\n查询:" << endl;
    cout << "  find(0) = " << qf.find(0) << ", find(2) = " << qf.find(2) << endl;
    cout << "  connected(0, 2) = " << (qf.connected(0, 2) ? "是" : "否") << endl;
    cout << "  connected(0, 4) = " << (qf.connected(0, 4) ? "是" : "否") << endl;

    // ---- Quick Union 演示 ----
    cout << "\n\n========== Quick Union ==========" << endl;
    cout << "初始: 7个元素" << endl;
    QuickUnion qu(7);
    qu.printState();

    cout << "\n合并 0-1, 1-2, 3-4, 5-6:" << endl;
    qu.unite(0, 1);
    qu.unite(1, 2);
    qu.unite(3, 4);
    qu.unite(5, 6);
    qu.printState();

    cout << "\n合并 0-4:" << endl;
    qu.unite(0, 4);
    qu.printState();

    cout << "\n合并 2-6:" << endl;
    qu.unite(2, 6);
    qu.printState();

    cout << "\n查询:" << endl;
    cout << "  connected(0, 6) = " << (qu.connected(0, 6) ? "是" : "否") << endl;

    // ---- 加权Quick Union + 路径压缩 ----
    cout << "\n\n========== 加权Quick Union + 路径压缩 ==========" << endl;
    cout << "初始: 8个元素" << endl;
    WeightedUnionFind wuf(8);
    wuf.printState();

    cout << "\n合并 0-1, 2-3, 4-5, 6-7:" << endl;
    wuf.unite(0, 1);
    wuf.unite(2, 3);
    wuf.unite(4, 5);
    wuf.unite(6, 7);
    wuf.printState();

    cout << "\n合并 0-2 (两个大小为2的树合并):" << endl;
    wuf.unite(0, 2);
    wuf.printState();

    cout << "\n合并 4-6 (两个大小为2的树合并):" << endl;
    wuf.unite(4, 6);
    wuf.printState();

    cout << "\n合并 0-4 (两个大小为4的树合并):" << endl;
    wuf.unite(0, 4);
    wuf.printState();

    cout << "\n查询:" << endl;
    cout << "  connected(1, 7) = " << (wuf.connected(1, 7) ? "是" : "否") << endl;
    cout << "  集合大小 getSize(3) = " << wuf.getSize(3) << endl;

    cout << "\n优化效果对比:" << endl;
    cout << "  Quick Find:  Find O(1), Union O(n)" << endl;
    cout << "  Quick Union: Find O(h), Union O(h), h可能退化为n" << endl;
    cout << "  优化版:      Find/Union 近似 O(1)" << endl;
}


// ============================================================================
//  2. 冗余连接 (LeetCode 684, Medium)
// ============================================================================
/*
 * 题目描述:
 *   在本问题中，树指的是一个连通且无向的图。
 *   输入一个用边表示的图 edges，其中 edges[i] = [ai, bi] 表示节点
 *   ai 和 bi 之间有一条边。返回一条可以删去的边，使得剩余部分是一棵树。
 *   如果有多个答案，返回最后出现的边。
 *
 * 思路:
 *   利用并查集，依次遍历每条边:
 *   - 如果两个节点已经在同一个集合中，说明加上这条边会产生环
 *   - 这条边就是冗余连接
 *   - 否则，将两个节点合并到同一个集合
 *
 * 图解:
 *   edges = [[1,2], [1,3], [2,3]]
 *
 *   遍历 [1,2]: 1和2不在同一集合 -> 合并
 *     集合: {1, 2}
 *
 *   遍历 [1,3]: 1和3不在同一集合 -> 合并
 *     集合: {1, 2, 3}
 *
 *   遍历 [2,3]: 2和3在同一集合 -> 产生环!
 *     返回 [2, 3] 为冗余连接
 *
 * 时间复杂度: O(n * alpha(n)) ≈ O(n)
 * 空间复杂度: O(n)
 */
class Solution684 {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();

        // 并查集，节点编号从1到n
        vector<int> parent(n + 1);
        vector<int> rank(n + 1, 0);
        for (int i = 0; i <= n; i++) parent[i] = i;

        // Find with path compression
        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        // Union by rank
        auto unite = [&](int x, int y) -> bool {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) return false;  // 已在同一集合

            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            return true;
        };

        // 遍历每条边
        for (auto& edge : edges) {
            if (!unite(edge[0], edge[1])) {
                return edge;  // 找到冗余边
            }
        }
        return {};
    }
};

void testLeetCode684() {
    separator("2. 冗余连接 (LeetCode 684)");
    cout << endl;

    cout << "题目: 给定一组边，找出一条冗余边（删除后使图成为树）" << endl;
    cout << endl;

    Solution684 sol;

    // 测试用例1
    cout << "--- 测试用例 1 ---" << endl;
    vector<vector<int>> edges1 = {{1, 2}, {1, 3}, {2, 3}};
    cout << "边: [1,2], [1,3], [2,3]" << endl;
    cout << "图示:" << endl;
    cout << "    1 --- 2" << endl;
    cout << "     \\   /" << endl;
    cout << "      \\ /" << endl;
    cout << "       3" << endl;
    cout << "  边[2,3]形成环(1->2->3->1)" << endl;

    vector<int> result1 = sol.findRedundantConnection(edges1);
    cout << "冗余边: [" << result1[0] << ", " << result1[1] << "]" << endl;
    cout << "正确: " << (result1 == vector<int>{2, 3} ? "是" : "否") << endl;

    // 逐步演示
    cout << "\n逐步过程:" << endl;
    {
        int n = 3;
        vector<int> par(n + 1);
        iota(par.begin(), par.end(), 0);
        auto find = [&](int x) -> int {
            while (par[x] != x) x = par[x];
            return x;
        };

        for (auto& e : edges1) {
            int ra = find(e[0]), rb = find(e[1]);
            if (ra == rb) {
                cout << "  [" << e[0] << "," << e[1] << "]: "
                     << "根都是" << ra << "，产生环! -> 冗余边" << endl;
            } else {
                par[ra] = rb;
                cout << "  [" << e[0] << "," << e[1] << "]: "
                     << "根" << ra << "->" << rb << "，合并成功" << endl;
            }
        }
    }

    // 测试用例2
    cout << "\n--- 测试用例 2 ---" << endl;
    vector<vector<int>> edges2 = {{1, 2}, {2, 3}, {3, 4}, {1, 4}, {1, 5}};
    cout << "边: [1,2], [2,3], [3,4], [1,4], [1,5]" << endl;
    cout << "图示:" << endl;
    cout << "    5" << endl;
    cout << "    |" << endl;
    cout << "    1 --- 2" << endl;
    cout << "    |     |" << endl;
    cout << "    4 --- 3" << endl;
    cout << "  边[1,4]形成环(1->2->3->4->1)" << endl;

    vector<int> result2 = sol.findRedundantConnection(edges2);
    cout << "冗余边: [" << result2[0] << ", " << result2[1] << "]" << endl;
    cout << "正确: " << (result2 == vector<int>{1, 4} ? "是" : "否") << endl;
}


// ============================================================================
//  3. 省份数量 (LeetCode 547, Medium)
// ============================================================================
/*
 * 题目描述:
 *   有 n 个城市，其中一些彼此相连，另一些没有相连。
 *   如果城市 a 与城市 b 直接相连，且城市 b 与城市 c 直接相连，
 *   那么城市 a 与城市 c 间接相连。
 *   省份是一组直接或间接相连的城市，组内不含其他没有相连的城市。
 *   给定一个 n x n 的矩阵 isConnected，其中 isConnected[i][j] = 1
 *   表示第 i 个城市和第 j 个城市直接相连，否则为 0。
 *   返回省份的数量。
 *
 * 思路:
 *   使用并查集:
 *   - 遍历邻接矩阵上三角
 *   - 如果 isConnected[i][j] == 1，合并 i 和 j
 *   - 最终并查集中的连通分量数就是省份数
 *
 * 图解:
 *   isConnected = [[1,1,0], [1,1,0], [0,0,1]]
 *
 *   城市关系图:       连通分量:
 *     0 --- 1           {0, 1}    <- 省份1
 *                         2       <- 省份2
 *   结果: 2个省份
 *
 *   isConnected = [[1,0,0], [0,1,0], [0,0,1]]
 *
 *   城市关系图:       连通分量:
 *     0   1   2         {0}  {1}  {2}
 *   结果: 3个省份
 *
 * 时间复杂度: O(n^2)
 * 空间复杂度: O(n)
 */
class Solution547 {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();

        // 初始化并查集
        vector<int> parent(n);
        vector<int> rank(n, 0);
        iota(parent.begin(), parent.end(), 0);
        int provinces = n;  // 初始有n个省份

        // Find with path compression
        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        // Union by rank
        auto unite = [&](int x, int y) -> bool {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) return false;

            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            provinces--;
            return true;
        };

        // 遍历邻接矩阵上三角
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isConnected[i][j] == 1) {
                    unite(i, j);
                }
            }
        }

        return provinces;
    }
};

void testLeetCode547() {
    separator("3. 省份数量 (LeetCode 547)");
    cout << endl;

    cout << "题目: 给定城市连接矩阵，求省份数量(连通分量个数)" << endl;
    cout << endl;

    Solution547 sol;

    // 测试用例1
    cout << "--- 测试用例 1 ---" << endl;
    vector<vector<int>> isConnected1 = {
        {1, 1, 0},
        {1, 1, 0},
        {0, 0, 1}
    };
    cout << "邻接矩阵:" << endl;
    cout << "    城市  0  1  2" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "    " << i << "   ";
        for (int j = 0; j < 3; j++) {
            cout << setw(3) << isConnected1[i][j];
        }
        cout << endl;
    }
    cout << "\n图示:" << endl;
    cout << "    0 --- 1      2" << endl;
    cout << "    (省份1)     (省份2)" << endl;

    // 逐步演示
    cout << "\n合并过程:" << endl;
    {
        int n = 3;
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        auto find = [&](int x) -> int {
            while (par[x] != x) x = par[x];
            return x;
        };
        int provinces = n;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isConnected1[i][j] == 1) {
                    int ri = find(i), rj = find(j);
                    if (ri != rj) {
                        par[ri] = rj;
                        provinces--;
                        cout << "  [" << i << "," << j << "]: 连接, 合并"
                             << " -> 省份数=" << provinces << endl;
                    }
                } else {
                    cout << "  [" << i << "," << j << "]: 不连接, 跳过" << endl;
                }
            }
        }
        cout << "  最终省份数: " << provinces << endl;
    }

    int result1 = sol.findCircleNum(isConnected1);
    cout << "答案: " << result1 << endl;
    cout << "正确: " << (result1 == 2 ? "是" : "否") << endl;

    // 测试用例2
    cout << "\n--- 测试用例 2 ---" << endl;
    vector<vector<int>> isConnected2 = {
        {1, 0, 0, 1},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {1, 0, 0, 1}
    };
    cout << "邻接矩阵 (4x4):" << endl;
    cout << "    城市  0  1  2  3" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "    " << i << "   ";
        for (int j = 0; j < 4; j++) {
            cout << setw(3) << isConnected2[i][j];
        }
        cout << endl;
    }
    cout << "\n图示:" << endl;
    cout << "    0 --- 3      1 --- 2" << endl;
    cout << "    (省份1)     (省份2)" << endl;

    int result2 = sol.findCircleNum(isConnected2);
    cout << "答案: " << result2 << endl;
    cout << "正确: " << (result2 == 2 ? "是" : "否") << endl;

    // 测试用例3
    cout << "\n--- 测试用例 3 ---" << endl;
    vector<vector<int>> isConnected3 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    cout << "所有城市都相连:" << endl;
    int result3 = sol.findCircleNum(isConnected3);
    cout << "答案: " << result3 << " (1个省份)" << endl;
    cout << "正确: " << (result3 == 1 ? "是" : "否") << endl;
}


// ============================================================================
//  4. 账户合并 (LeetCode 721, Medium)
// ============================================================================
/*
 * 题目描述:
 *   给定一个列表 accounts，每个元素 accounts[i] 是一个字符串列表，
 *   其中第一个元素是名称，其余元素是该账户的邮箱地址。
 *   如果两个账户有共同的邮箱地址，则这两个账户属于同一个人。
 *   请合并属于同一个人的账户。
 *
 * 思路:
 *   1. 使用并查集，将每个邮箱地址作为节点
 *   2. 同一个账户内的所有邮箱合并在一起
 *   3. 如果不同账户有相同邮箱，也会被合并
 *   4. 最后按集合分组，每个集合代表一个人
 *
 * 图解:
 *   accounts = [
 *     ["John", "john@mail.com", "john00@mail.com"],
 *     ["John", "johnny@mail.com", "john00@mail.com"],
 *     ["John", "johnsmith@mail.com"],
 *     ["Mary", "mary@mail.com"]
 *   ]
 *
 *   步骤1: 为每个邮箱分配编号
 *     0: john@mail.com
 *     1: john00@mail.com
 *     2: johnny@mail.com
 *     3: johnsmith@mail.com
 *     4: mary@mail.com
 *
 *   步骤2: 合并同一账户的邮箱
 *     账户1: 合并 0 和 1     -> {0, 1}
 *     账户2: 合并 1 和 2     -> {0, 1, 2}  (通过邮箱1连接)
 *     账户3: 合并 3          -> {3} (单独)
 *     账户4: 合并 4          -> {4} (单独)
 *
 *   步骤3: 按集合分组
 *     {0, 1, 2}: john@mail.com, john00@mail.com, johnny@mail.com -> John
 *     {3}: johnsmith@mail.com -> John
 *     {4}: mary@mail.com -> Mary
 *
 * 时间复杂度: O(N * alpha(N)), N为邮箱总数
 * 空间复杂度: O(N)
 */
class Solution721 {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        // 为每个邮箱分配唯一编号
        unordered_map<string, int> emailToId;
        unordered_map<int, string> idToEmail;
        unordered_map<int, string> idToName;
        int idCounter = 0;

        for (auto& account : accounts) {
            string name = account[0];
            for (int i = 1; i < (int)account.size(); i++) {
                string& email = account[i];
                if (emailToId.find(email) == emailToId.end()) {
                    emailToId[email] = idCounter;
                    idToEmail[idCounter] = email;
                    idToName[idCounter] = name;
                    idCounter++;
                }
            }
        }

        int n = idCounter;

        // 初始化并查集
        vector<int> parent(n);
        vector<int> rank(n, 0);
        iota(parent.begin(), parent.end(), 0);

        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) return;
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        };

        // 合并同一账户内的邮箱
        for (auto& account : accounts) {
            int firstId = emailToId[account[1]];
            for (int i = 2; i < (int)account.size(); i++) {
                unite(firstId, emailToId[account[i]]);
            }
        }

        // 按集合分组
        unordered_map<int, set<string>> groups;
        for (int i = 0; i < n; i++) {
            int root = find(i);
            groups[root].insert(idToEmail[i]);
        }

        // 构造结果
        vector<vector<string>> result;
        for (auto& [root, emails] : groups) {
            vector<string> merged;
            merged.push_back(idToName[root]);  // 名字
            for (auto& email : emails) {
                merged.push_back(email);  // 邮箱（已排序）
            }
            result.push_back(merged);
        }

        return result;
    }
};

void testLeetCode721() {
    separator("4. 账户合并 (LeetCode 721)");
    cout << endl;

    cout << "题目: 合并有共同邮箱的账户" << endl;
    cout << endl;

    Solution721 sol;

    // 测试用例
    cout << "--- 测试用例 ---" << endl;
    vector<vector<string>> accounts = {
        {"John", "john@mail.com", "john00@mail.com"},
        {"John", "johnny@mail.com", "john00@mail.com"},
        {"John", "johnsmith@mail.com"},
        {"Mary", "mary@mail.com"}
    };

    cout << "输入账户:" << endl;
    for (auto& acc : accounts) {
        cout << "  " << acc[0] << ": ";
        for (int i = 1; i < (int)acc.size(); i++) {
            cout << acc[i];
            if (i < (int)acc.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    // 详细演示合并过程
    cout << "\n详细过程:" << endl;

    cout << "\n  步骤1: 为邮箱分配编号" << endl;
    unordered_map<string, int> emailToId;
    int id = 0;
    for (auto& acc : accounts) {
        for (int i = 1; i < (int)acc.size(); i++) {
            if (emailToId.find(acc[i]) == emailToId.end()) {
                emailToId[acc[i]] = id++;
                cout << "    " << acc[i] << " -> " << (id - 1) << endl;
            }
        }
    }

    cout << "\n  步骤2: 合并同一账户的邮箱" << endl;
    {
        int n = emailToId.size();
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        auto find = [&](int x) -> int {
            while (par[x] != x) x = par[x];
            return x;
        };

        for (auto& acc : accounts) {
            int firstId = emailToId[acc[1]];
            for (int i = 2; i < (int)acc.size(); i++) {
                int curId = emailToId[acc[i]];
                int rootA = find(firstId), rootB = find(curId);
                if (rootA != rootB) {
                    par[rootA] = rootB;
                    cout << "    合并 " << acc[1] << "(" << firstId << ") 和 "
                         << acc[i] << "(" << curId << ")" << endl;
                }
            }
        }

        // 检查johnsmith是否和其他john邮箱连通
        int rootSmith = find(emailToId["johnsmith@mail.com"]);
        int rootJohn = find(emailToId["john@mail.com"]);
        cout << "\n  johnsmith的根: " << rootSmith << endl;
        cout << "  john的根:      " << rootJohn << endl;
        if (rootSmith != rootJohn) {
            cout << "  johnsmith和其他John邮箱不连通，单独成组" << endl;
        }
    }

    cout << "\n  步骤3: 按集合分组输出" << endl;

    // 实际调用
    vector<vector<string>> result = sol.accountsMerge(accounts);

    cout << "\n合并结果:" << endl;
    for (auto& group : result) {
        cout << "  " << group[0] << ": ";
        for (int i = 1; i < (int)group.size(); i++) {
            cout << group[i];
            if (i < (int)group.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    // 第二个测试用例
    cout << "\n--- 测试用例 2 (跨账户合并) ---" << endl;
    vector<vector<string>> accounts2 = {
        {"Gabe", "Gabe0@m.co", "Gabe3@m.co", "Gabe1@m.co"},
        {"Kevin", "Kevin3@m.co", "Kevin5@m.co", "Kevin0@m.co"},
        {"Ethan", "Ethan5@m.co", "Ethan4@m.co", "Ethan0@m.co"},
        {"Hanzo", "Hanzo3@m.co", "Hanzo1@m.co", "Hanzo0@m.co"},
        {"Fern", "Fern5@m.co", "Fern1@m.co", "Fern0@m.co"}
    };

    cout << "输入账户 (5个独立账户):" << endl;
    for (auto& acc : accounts2) {
        cout << "  " << acc[0] << ": ";
        for (int i = 1; i < (int)acc.size(); i++) {
            cout << acc[i];
            if (i < (int)acc.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    vector<vector<string>> result2 = sol.accountsMerge(accounts2);
    cout << "\n合并结果 (" << result2.size() << "组，无共同邮箱所以各独立):" << endl;
    for (auto& group : result2) {
        cout << "  " << group[0] << ": ";
        for (int i = 1; i < (int)group.size(); i++) {
            cout << group[i];
            if (i < (int)group.size() - 1) cout << ", ";
        }
        cout << endl;
    }

    // 第三个测试用例
    cout << "\n--- 测试用例 3 (链式合并) ---" << endl;
    vector<vector<string>> accounts3 = {
        {"Alex", "Alex5@m.co", "Alex4@m.co", "Alex0@m.co"},
        {"Ethan", "Ethan3@m.co", "Ethan3@m.co", "Ethan0@m.co"},
        {"Kevin", "Kevin4@m.co", "Kevin2@m.co", "Kevin2@m.co"},
        {"Gabe", "Gabe0@m.co", "Gabe3@m.co", "Gabe2@m.co"},
        {"Gabe", "Gabe3@m.co", "Gabe4@m.co", "Gabe2@m.co"}
    };

    cout << "输入账户:" << endl;
    for (auto& acc : accounts3) {
        cout << "  " << acc[0] << ": ";
        for (int i = 1; i < (int)acc.size(); i++) {
            cout << acc[i];
            if (i < (int)acc.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    cout << "\n注意: Gabe的两个账户通过 Gabe3@m.co 和 Gabe2@m.co 连接" << endl;

    vector<vector<string>> result3 = sol.accountsMerge(accounts3);
    cout << "\n合并结果 (" << result3.size() << "组):" << endl;
    for (auto& group : result3) {
        cout << "  " << group[0] << ": ";
        for (int i = 1; i < (int)group.size(); i++) {
            cout << group[i];
            if (i < (int)group.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}


// ============================================================================
//  5. 连通网络的操作次数 (LeetCode 1319, Medium)
// ============================================================================
/*
 * 题目描述:
 *   有 n 台计算机, 编号为 0 到 n-1, 用网线 connections 连接,
 *   其中 connections[i] = [a, b] 表示计算机 a 和 b 之间有一条网线。
 *   所有计算机最初都是断开的。每条网线连接两台计算机, 网线不能共用。
 *   请计算最少需要多少操作才能使所有计算机连通。如果不可能, 返回 -1。
 *   一次操作: 将一条网线从一对计算机拔出, 插到另一对计算机上。
 *
 * 思路:
 *   1. 用并查集统计连通分量个数 (components)
 *   2. 如果网线数 < n-1, 不可能连通 -> 返回 -1
 *   3. 至少需要 (components - 1) 次操作 (每操作减少一个分量)
 *
 * 图解:
 *   n=4, connections=[[0,1],[0,2],[1,2]]
 *
 *   初始图:        并查集合并过程:
 *     0 --- 1      [0,1]: 合并0,1 -> {0,1}, {2}, {3}
 *     |  \ /       [0,2]: 合并0,2 -> {0,1,2}, {3}
 *     |   X        [1,2]: 1和2已同一集合, 跳过(这是多余边)
 *     |  / \
 *     2     3
 *
 *   连通分量 = 2 ({0,1,2} 和 {3})
 *   多余边数 = 1 (总共3条边, 连通3个节点只需2条)
 *   操作次数 = 连通分量 - 1 = 2 - 1 = 1
 *   将边[1,2]从(1,2)拔出, 插到(2,3)上
 *
 * 时间复杂度: O(n + m * alpha(n)), m为边数
 * 空间复杂度: O(n)
 */
class Solution1319 {
public:
    int makeConnected(int n, vector<vector<int>>& connections) {
        int m = connections.size();

        // 如果网线数少于 n-1, 不可能连通所有计算机
        if (m < n - 1) return -1;

        // 初始化并查集
        vector<int> parent(n);
        vector<int> rank(n, 0);
        iota(parent.begin(), parent.end(), 0);

        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int x, int y) -> bool {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) return false;
            if (rank[rootX] < rank[rootY]) parent[rootX] = rootY;
            else if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
            else { parent[rootY] = rootX; rank[rootX]++; }
            return true;
        };

        // 合并所有边, 统计连通分量
        int components = n;
        for (auto& conn : connections) {
            if (unite(conn[0], conn[1])) {
                components--;
            }
        }

        // 操作次数 = 连通分量数 - 1
        return components - 1;
    }
};

void testLeetCode1319() {
    separator("5. 连通网络的操作次数 (LeetCode 1319)");
    cout << endl;

    cout << "题目: 计算使所有计算机连通所需的最少操作次数" << endl;
    cout << endl;

    Solution1319 sol;

    // 测试用例1
    cout << "--- 测试用例 1 ---" << endl;
    cout << "n=4, connections=[[0,1],[0,2],[1,2]]" << endl;
    cout << "图示:" << endl;
    cout << "    0 --- 1" << endl;
    cout << "    |  \\/ " << endl;
    cout << "    |  /\\ " << endl;
    cout << "    2     3 (孤立)" << endl;
    cout << "  网线3条, 节点4个, 3 >= 4-1=3, 可能连通" << endl;
    cout << "  连通分量: {0,1,2}, {3} -> 2个" << endl;
    cout << "  操作: 拔掉[1,2], 接到[2,3] -> 1次" << endl;

    vector<vector<int>> conn1 = {{0,1},{0,2},{1,2}};
    int result1 = sol.makeConnected(4, conn1);
    cout << "答案: " << result1 << endl;
    cout << "正确: " << (result1 == 1 ? "是" : "否") << endl;

    // 测试用例2
    cout << "\n--- 测试用例 2 ---" << endl;
    cout << "n=6, connections=[[0,1],[0,2],[0,3],[1,2],[1,3]]" << endl;
    cout << "图示:" << endl;
    cout << "    0 --- 1" << endl;
    cout << "    |\\   /|\\" << endl;
    cout << "    | X   | \\" << endl;
    cout << "    |/   \\|  \\" << endl;
    cout << "    2 --- 3   4   5" << endl;
    cout << "  网线5条, 节点6个, 5 >= 6-1=5, 可能连通" << endl;
    cout << "  连通分量: {0,1,2,3}, {4}, {5} -> 3个" << endl;
    cout << "  操作: 3-1 = 2次" << endl;

    vector<vector<int>> conn2 = {{0,1},{0,2},{0,3},{1,2},{1,3}};
    int result2 = sol.makeConnected(6, conn2);
    cout << "答案: " << result2 << endl;
    cout << "正确: " << (result2 == 2 ? "是" : "否") << endl;

    // 测试用例3: 不可能
    cout << "\n--- 测试用例 3 (不可能) ---" << endl;
    cout << "n=6, connections=[[0,1],[0,2],[0,3],[1,2]]" << endl;
    cout << "  网线4条, 节点6个, 4 < 6-1=5, 不可能连通!" << endl;

    vector<vector<int>> conn3 = {{0,1},{0,2},{0,3},{1,2}};
    int result3 = sol.makeConnected(6, conn3);
    cout << "答案: " << result3 << endl;
    cout << "正确: " << (result3 == -1 ? "是" : "否") << endl;

    // 测试用例4: 已经连通
    cout << "\n--- 测试用例 4 (已连通) ---" << endl;
    cout << "n=3, connections=[[0,1],[1,2]]" << endl;

    vector<vector<int>> conn4 = {{0,1},{1,2}};
    int result4 = sol.makeConnected(3, conn4);
    cout << "答案: " << result4 << endl;
    cout << "正确: " << (result4 == 0 ? "是" : "否") << endl;
}


// ============================================================================
//  6. 判断二分图 (LeetCode 785, Medium)
// ============================================================================
/*
 * 题目描述:
 *   给定一个无向图 graph, 其中 graph[i] 是节点 i 的邻居列表。
 *   当且仅当它是二分图时返回 true。
 *   二分图: 能将节点分成两个独立集合 A 和 B, 使得每条边的两个节点
 *   分别属于 A 和 B (即同集合内无边)。
 *
 * 思路 (并查集):
 *   对于每个节点 i 和它的每个邻居 j:
 *   - 如果 i 和 j 已经在同一集合 -> 不是二分图!
 *   - 否则, 将 i 的所有邻居合并到同一集合
 *
 *   关键: 节点 i 的所有邻居必须在同一集合 (因为它们都与 i 不同色)
 *
 * 图解:
 *   graph = [[1,3], [0,2], [1,3], [0,2]]
 *
 *   图形:
 *     0 --- 1
 *     |     |
 *     3 --- 2
 *
 *   是二分图! 分为 {0,2} 和 {1,3}
 *
 *   并查集过程:
 *   i=0, 邻居[1,3]: 合并1,3 -> {1,3}, 检查0和1不同集合, 0和3不同集合
 *   i=1, 邻居[0,2]: 合并0,2 -> {0,2}, 检查1和0不同集合, 1和2不同集合
 *   i=2, 邻居[1,3]: 1,3已同一集合, 检查2和1不同集合 -> OK
 *   i=3, 邻居[0,2]: 0,2已同一集合, 检查3和0不同集合 -> OK
 *   -> 是二分图
 *
 *   反例: graph = [[1,2,3], [0,2], [0,1,3], [0,2]]
 *
 *   图形:
 *     0 --- 1
 *     |\   /|
 *     | X   |
 *     |/ \  |
 *     3 --- 2
 *   三角形 0-1-2, 不是二分图!
 *
 * 时间复杂度: O(V + E)
 * 空间复杂度: O(V)
 */
class Solution785 {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();

        vector<int> parent(n);
        vector<int> rank(n, 0);
        iota(parent.begin(), parent.end(), 0);

        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            if (rootX == rootY) return;
            if (rank[rootX] < rank[rootY]) parent[rootX] = rootY;
            else if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
            else { parent[rootY] = rootX; rank[rootX]++; }
        };

        for (int i = 0; i < n; i++) {
            for (int neighbor : graph[i]) {
                // 如果节点i和邻居在同一集合 -> 不是二分图
                if (find(i) == find(neighbor)) return false;
                // 将i的所有邻居合并到同一集合(它们都与i不同色)
                if (!graph[i].empty()) {
                    unite(graph[i][0], neighbor);
                }
            }
        }

        return true;
    }
};

void testLeetCode785() {
    separator("6. 判断二分图 (LeetCode 785)");
    cout << endl;

    cout << "题目: 判断图是否为二分图(可二着色)" << endl;
    cout << endl;

    Solution785 sol;

    // 测试用例1: 是二分图
    cout << "--- 测试用例 1 (是二分图) ---" << endl;
    cout << "graph = [[1,3],[0,2],[1,3],[0,2]]" << endl;
    cout << "图示:" << endl;
    cout << "    0(A) --- 1(B)" << endl;
    cout << "    |         |" << endl;
    cout << "    3(B) --- 2(A)" << endl;
    cout << "  分组: A={0,2}, B={1,3}, 同组无边 -> 是二分图" << endl;

    vector<vector<int>> graph1 = {{1,3},{0,2},{1,3},{0,2}};
    bool result1 = sol.isBipartite(graph1);
    cout << "答案: " << (result1 ? "true" : "false") << endl;
    cout << "正确: " << (result1 ? "是" : "否") << endl;

    // 测试用例2: 不是二分图
    cout << "\n--- 测试用例 2 (不是二分图) ---" << endl;
    cout << "graph = [[1,2,3],[0,2],[0,1,3],[0,2]]" << endl;
    cout << "图示:" << endl;
    cout << "    0 --- 1" << endl;
    cout << "    |\\   /|" << endl;
    cout << "    | X   |" << endl;
    cout << "    |/ \\" << endl;
    cout << "    3 --- 2" << endl;
    cout << "  三角形 0-1-2, 无法二着色 -> 不是二分图" << endl;

    vector<vector<int>> graph2 = {{1,2,3},{0,2},{0,1,3},{0,2}};
    bool result2 = sol.isBipartite(graph2);
    cout << "答案: " << (result2 ? "true" : "false") << endl;
    cout << "正确: " << (!result2 ? "是" : "否") << endl;

    // 测试用例3: 空图 (单节点)
    cout << "\n--- 测试用例 3 (单节点) ---" << endl;
    cout << "graph = [[]]" << endl;
    vector<vector<int>> graph3 = {{}};
    bool result3 = sol.isBipartite(graph3);
    cout << "答案: " << (result3 ? "true" : "false") << " (期望: true)" << endl;
    cout << "正确: " << (result3 ? "是" : "否") << endl;

    // 测试用例4: 链状图
    cout << "\n--- 测试用例 4 (链状图) ---" << endl;
    cout << "graph = [[1],[0,2],[1,3],[2]]" << endl;
    cout << "图示: 0 -- 1 -- 2 -- 3" << endl;
    vector<vector<int>> graph4 = {{1},{0,2},{1,3},{2}};
    bool result4 = sol.isBipartite(graph4);
    cout << "答案: " << (result4 ? "true" : "false") << " (期望: true)" << endl;
    cout << "正确: " << (result4 ? "是" : "否") << endl;
}


// ============================================================================
//  7. 最小体力消耗值 (LeetCode 1631, Medium)
// ============================================================================
/*
 * 题目描述:
 *   你准备参加一场远足活动, 给定一个 m x n 的网格 heights,
 *   其中 heights[r][c] 表示坐标 (r,c) 的高度。
 *   从左上角 (0,0) 出发, 到右下角 (m-1,n-1) 结束。
 *   每一步可以往上/下/左/右走。一条路径的「体力消耗值」是相邻格子之间
 *   高度差绝对值的最大值。返回从左上到右下的最小体力消耗值。
 *
 * 示例:
 *   输入: heights = [[1,2,2],[3,8,2],[5,3,5]]
 *   输出: 2
 *   解释: 路径 1->3->5->3->5, 相邻高度差最大为2
 *
 * 思路 (并查集 + 按边权排序):
 *   1. 将所有相邻格子的边收集起来, 按高度差排序
 *   2. 从小到大依次加入并查集
 *   3. 当起点(0,0)和终点(m-1,n-1)连通时, 当前边的高度差就是答案
 *   (这本质上是 Kruskal 最小生成树的变体)
 *
 * 思路二 (二分 + BFS):
 *   1. 二分体力消耗值 x
 *   2. BFS验证: 只走高度差 <= x 的路径, 能否到达终点
 *   3. 找到最小的可行 x
 *
 * ASCII图示:
 *
 *   heights = [[1,2,2],
 *              [3,8,2],
 *              [5,3,5]]
 *
 *   格子编号:
 *   (0,0)=1  (0,1)=2  (0,2)=2
 *   (1,0)=3  (1,1)=8  (1,2)=2
 *   (2,0)=5  (2,1)=3  (2,2)=5
 *
 *   所有相邻边(按高度差排序):
 *   高度差1: (0,0)-(0,1), (0,1)-(0,2), (1,2)-(2,2), (1,0)-(2,1)
 *   高度差2: (0,0)-(1,0), (0,2)-(1,2), (0,1)-(1,0), (2,1)-(2,2)
 *   高度差5: (1,0)-(1,1), (0,1)-(1,1), (1,1)-(1,2), (1,1)-(2,1)
 *   高度差6: (1,0)-(2,0), (2,0)-(2,1)
 *
 *   按顺序加边到并查集:
 *   加高度差1的边后: (0,0)-(0,1)-(0,2) 连通, (2,2)与(1,2)连通...
 *   加高度差2的边后: (0,0)和(0,2)通过(1,2)与(2,2)连通!
 *   此时(0,0)与(2,2)连通 -> 答案 = 2
 *
 * 时间复杂度: 并查集 O(mn * log(mn)), 二分+BFS O(mn * log(maxHeight))
 * 空间复杂度: O(mn)
 */
class Solution1631 {
public:
    // 方法一: 并查集 + 按边权排序 (类Kruskal)
    int minimumEffortPath(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();

        // 收集所有相邻边: (高度差, 节点u, 节点v)
        vector<tuple<int,int,int>> edges;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int idx = i * n + j;
                if (i + 1 < m) {
                    int diff = abs(heights[i][j] - heights[i+1][j]);
                    edges.push_back({diff, idx, (i+1)*n+j});
                }
                if (j + 1 < n) {
                    int diff = abs(heights[i][j] - heights[i][j+1]);
                    edges.push_back({diff, idx, i*n+j+1});
                }
            }
        }

        // 按高度差排序
        sort(edges.begin(), edges.end());

        // 并查集
        vector<int> parent(m * n);
        vector<int> rank(m * n, 0);
        iota(parent.begin(), parent.end(), 0);

        auto find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int x, int y) {
            int rootX = find(x), rootY = find(y);
            if (rootX == rootY) return;
            if (rank[rootX] < rank[rootY]) parent[rootX] = rootY;
            else if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
            else { parent[rootY] = rootX; rank[rootX]++; }
        };

        int start = 0, target = m * n - 1;

        // 按边权从小到大加边, 直到起点和终点连通
        for (auto& [diff, u, v] : edges) {
            unite(u, v);
            if (find(start) == find(target)) {
                return diff; // 此时的高度差就是答案
            }
        }

        return 0; // 起点等于终点
    }
};

void testLeetCode1631() {
    separator("7. 最小体力消耗值 (LeetCode 1631)");
    cout << endl;

    cout << "题目: 从左上角到右下角, 找高度差绝对值最大值最小的路径" << endl;
    cout << endl;

    Solution1631 sol;

    // 测试用例1
    cout << "--- 测试用例 1 ---" << endl;
    vector<vector<int>> heights1 = {{1,2,2},{3,8,2},{5,3,5}};
    cout << "heights:" << endl;
    cout << "    1  2  2" << endl;
    cout << "    3  8  2" << endl;
    cout << "    5  3  5" << endl;
    cout << "最优路径: (0,0)=1 -> (1,0)=3 -> (2,0)=5 -> (2,1)=3 -> (2,2)=5" << endl;
    cout << "高度差: |1-3|=2, |3-5|=2, |5-3|=2, |3-5|=2 -> 最大=2" << endl;

    int result1 = sol.minimumEffortPath(heights1);
    cout << "答案: " << result1 << endl;
    cout << "正确: " << (result1 == 2 ? "是" : "否") << endl;

    // 测试用例2
    cout << "\n--- 测试用例 2 ---" << endl;
    vector<vector<int>> heights2 = {{1,2,3},{3,8,4},{5,3,5}};
    cout << "heights:" << endl;
    cout << "    1  2  3" << endl;
    cout << "    3  8  4" << endl;
    cout << "    5  3  5" << endl;

    int result2 = sol.minimumEffortPath(heights2);
    cout << "答案: " << result2 << endl;
    cout << "正确: " << (result2 == 1 ? "是" : "否") << endl;

    // 测试用例3: 单格子
    cout << "\n--- 测试用例 3 (单格子) ---" << endl;
    vector<vector<int>> heights3 = {{42}};
    int result3 = sol.minimumEffortPath(heights3);
    cout << "heights: [[42]]" << endl;
    cout << "答案: " << result3 << " (期望: 0, 无需移动)" << endl;
    cout << "正确: " << (result3 == 0 ? "是" : "否") << endl;

    // 测试用例4: 一行
    cout << "\n--- 测试用例 4 (一行) ---" << endl;
    vector<vector<int>> heights4 = {{1,10,6,7,9,10,4,9}};
    cout << "heights: [1,10,6,7,9,10,4,9]" << endl;

    int result4 = sol.minimumEffortPath(heights4);
    cout << "答案: " << result4 << endl;
    cout << "(路径必须依次走过, 相邻高度差最大值最小化)" << endl;

    // 测试用例5: 高度递减
    cout << "\n--- 测试用例 5 (阶梯式) ---" << endl;
    vector<vector<int>> heights5 = {{1,2,1,1,1},{1,2,1,2,1},{1,2,1,2,1},{1,2,1,2,1},{1,1,1,2,1}};
    cout << "heights (5x5):" << endl;
    for (auto& row : heights5) {
        cout << "    ";
        for (int h : row) cout << h << " ";
        cout << endl;
    }

    int result5 = sol.minimumEffortPath(heights5);
    cout << "答案: " << result5 << " (期望: 0)" << endl;
    cout << "正确: " << (result5 == 0 ? "是" : "否") << endl;
}


// ============================================================================
//  并查集知识总结
// ============================================================================
void printSummary() {
    separator("并查集知识总结");

    cout << R"(
    并查集 (Union-Find) 核心要点:

    1. 数据结构本质:
       - 维护不相交集合的合并与查询
       - 用树/森林表示集合，每个树的根是集合代表

    2. 两种核心操作:
       - Find(x):  找到x所在集合的根节点
       - Union(x,y): 合并x和y所在的集合

    3. 两种优化策略:
       ┌─────────────────┬──────────────────────────────────┐
       │ 优化             │ 说明                             │
       ├─────────────────┼──────────────────────────────────┤
       │ 按秩/大小合并    │ 将小树挂到大树上，控制树高       │
       │ 路径压缩         │ Find时将节点直接指向根，降低树高  │
       └─────────────────┴──────────────────────────────────┘

    4. 复杂度 (使用两种优化):
       - 单次操作: O(alpha(n)), alpha为反阿克曼函数
       - m次操作: O(m * alpha(n)) ≈ O(m)
       - alpha(n) 在实际数据范围(n < 10^80)内 <= 4

    5. 典型应用:
       - 图的连通性判断
       - 检测无向图的环
       - 连通分量计数
       - Kruskal最小生成树
       - 等价类合并
       - 朋友圈/省份数量

    6. LeetCode经典题目:
       - 684:  冗余连接 (环检测)
       - 547:  省份数量 (连通分量)
       - 721:  账户合并
       - 1319: 连通网络的操作次数
       - 785:  判断二分图
       - 1631: 最小体力消耗值
       - 200:  岛屿数量
       - 128:  最长连续序列
)" << endl;
}


// ============================================================================
//  主函数
// ============================================================================
int main() {
    cout << "====================================================================" << endl;
    cout << "       并查集 (Union-Find) 完整教程" << endl;
    cout << "====================================================================" << endl;

    testUnionFindBasics();
    testLeetCode684();
    testLeetCode547();
    testLeetCode721();
    testLeetCode1319();
    testLeetCode785();
    testLeetCode1631();
    printSummary();

    cout << "\n" << string(70, '=') << endl;
    cout << "  所有并查集测试完成!" << endl;
    cout << string(70, '=') << endl;

    return 0;
}
