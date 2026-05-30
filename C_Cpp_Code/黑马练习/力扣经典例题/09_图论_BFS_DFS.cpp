/**
 * 09_图论_BFS_DFS.cpp
 * =====================
 * 力扣经典图论问题 - BFS与DFS
 *
 * 本文件包含10道经典图论题目，涵盖:
 *   1. 岛屿数量 (LeetCode 200, Medium) - DFS/BFS遍历矩阵
 *   2. 课程表 (LeetCode 207, Medium) - 拓扑排序/环检测
 *   3. 克隆图 (LeetCode 133, Medium) - 图的深拷贝
 *   4. 腐烂的橘子 (LeetCode 994, Medium) - 多源BFS
 *   5. 单词接龙 (LeetCode 127, Hard) - BFS最短路径/双向BFS
 *   6. 被围绕的区域 (LeetCode 130, Medium) - DFS从边界开始
 *   7. 课程表 II (LeetCode 210, Medium) - 拓扑排序返回顺序
 *   8. 图是否是树 (LeetCode 261, Medium) - 判断连通无环图
 *   9. 钥匙和房间 (LeetCode 841, Medium) - DFS/BFS可达性
 *   10. 太平洋大西洋水流问题 (LeetCode 417, Medium) - 双向DFS
 *
 * 核心知识点:
 *   - DFS: 递归/栈，适合探索所有路径、连通分量
 *   - BFS: 队列，适合最短路径、层序遍历
 *   - 图的表示: 邻接矩阵、邻接表
 *   - 访问标记: 避免重复访问
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

// ============================================================================
// 题目1: 岛屿数量 (LeetCode 200)
// ============================================================================
/**
 * 给你一个由 '1'（陆地）和 '0'（水）组成的二维网格，请你计算网格中岛屿的数量。
 * 岛屿总是被水包围，并且每座岛屿只能由水平方向和垂直方向上相邻的陆地连接形成。
 *
 * 示例:
 *   输入:
 *     1 1 1 1 0
 *     1 1 0 1 0
 *     1 1 0 0 0
 *     0 0 0 0 0
 *   输出: 1
 *
 * 思路分析 (DFS):
 *   从每个未访问的 '1' 出发，用DFS将所有相连的 '1' 置为 '0'（沉岛思想）
 *   每启动一次DFS，岛屿数量+1
 *
 * 思路分析 (BFS):
 *   从每个未访问的 '1' 出发，用BFS将所有相连的 '1' 入队并置为 '0'
 *   每启动一次BFS，岛屿数量+1
 *
 * ASCII图示:
 *   原始网格:           DFS从(0,0):           最终结果:
 *   1 1 1 1 0          -> -> -> 1 0          0 0 0 0 0
 *   1 1 0 1 0          |  v     1 0          0 0 0 1 0 <- 孤岛
 *   1 1 0 0 0          -> ->   0 0          0 0 0 0 0
 *   0 0 0 0 0           0  0   0 0          0 0 0 0 0
 */

// 方法一: DFS - 沉岛法
// 时间复杂度: O(M*N)，空间复杂度: O(M*N) 最坏情况递归深度
class NumIslands_DFS {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = grid.size();
        int cols = grid[0].size();
        int count = 0;

        // 遍历每个格子，遇到'1'就启动DFS
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '1') {
                    count++;            // 发现新岛屿
                    dfs(grid, i, j);    // 将整个岛屿沉没
                }
            }
        }
        return count;
    }

private:
    // DFS: 从(i,j)出发，将所有相连的'1'变为'0'
    void dfs(vector<vector<char>>& grid, int i, int j) {
        int rows = grid.size();
        int cols = grid[0].size();

        // 边界检查 + 只处理陆地('1')
        if (i < 0 || i >= rows || j < 0 || j >= cols || grid[i][j] != '1')
            return;

        grid[i][j] = '0';  // 沉岛: 标记为已访问

        // 向四个方向递归
        dfs(grid, i - 1, j);  // 上
        dfs(grid, i + 1, j);  // 下
        dfs(grid, i, j - 1);  // 左
        dfs(grid, i, j + 1);  // 右
    }
};

// 方法二: BFS
// 时间复杂度: O(M*N)，空间复杂度: O(min(M,N)) 队列大小
class NumIslands_BFS {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        int rows = grid.size();
        int cols = grid[0].size();
        int count = 0;

        // 方向数组: 上下左右
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == '1') {
                    count++;

                    // BFS: 将当前岛屿所有格子入队并沉没
                    queue<pair<int, int>> q;
                    q.push({i, j});
                    grid[i][j] = '0';

                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols
                                && grid[nx][ny] == '1') {
                                grid[nx][ny] = '0';
                                q.push({nx, ny});
                            }
                        }
                    }
                }
            }
        }
        return count;
    }
};

// ============================================================================
// 题目2: 课程表 (LeetCode 207)
// ============================================================================
/**
 * 你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses-1。
 * 在选修某些课程之前需要一些先修课程。例如，想要学习课程 0，你需要先完成课程 1，
 * 我们用一个匹配来表示他们: [0,1]。
 * 给定课程总数和先修条件，判断是否可能完成所有课程。
 *
 * 示例:
 *   输入: numCourses = 2, prerequisites = [[1,0]]
 *   含义: 要学课程1，必须先学课程0
 *   输出: true (可以先学0，再学1)
 *
 *   输入: numCourses = 2, prerequisites = [[1,0],[0,1]]
 *   含义: 0依赖1，1依赖0 -> 循环依赖!
 *   输出: false
 *
 * ASCII图示 - 拓扑排序 (Kahn算法):
 *
 *   正常情况:             循环依赖:
 *   0 -> 1 -> 2           0 -> 1
 *                          ^    |
 *                          |    v
 *                          2 <- 3
 *
 *   Kahn算法: 不断移除入度为0的节点，若最终全部移除则无环
 */

// 方法一: BFS拓扑排序 (Kahn算法)
// 核心思想: 不断移除入度为0的节点，若最终所有节点都被移除则无环
// 时间复杂度: O(V+E)，空间复杂度: O(V+E)
class CanFinish_BFS {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // 构建邻接表和入度数组
        vector<vector<int>> graph(numCourses);
        vector<int> inDegree(numCourses, 0);

        for (auto& pre : prerequisites) {
            graph[pre[1]].push_back(pre[0]);  // pre[1] -> pre[0]
            inDegree[pre[0]]++;               // pre[0]的入度+1
        }

        // 将所有入度为0的节点入队 (没有先修要求的课程)
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }

        int processed = 0;  // 已处理的节点数

        while (!q.empty()) {
            int course = q.front();
            q.pop();
            processed++;

            // 将course的所有后继节点的入度减1
            for (int next : graph[course]) {
                inDegree[next]--;
                if (inDegree[next] == 0) {
                    q.push(next);  // 入度变为0，可以学了
                }
            }
        }

        // 若所有课程都能被处理，说明无环
        return processed == numCourses;
    }
};

// 方法二: DFS环检测
// 核心思想: 用三种状态标记节点: 未访问(0)、正在访问(1)、已完成(2)
// 若DFS过程中遇到"正在访问"的节点，说明存在环
// 时间复杂度: O(V+E)，空间复杂度: O(V+E)
class CanFinish_DFS {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // 构建邻接表
        vector<vector<int>> graph(numCourses);
        for (auto& pre : prerequisites) {
            graph[pre[1]].push_back(pre[0]);
        }

        // 状态: 0=未访问, 1=正在访问(在当前DFS路径上), 2=已完成
        vector<int> state(numCourses, 0);

        // 对每个未访问的节点启动DFS
        for (int i = 0; i < numCourses; i++) {
            if (state[i] == 0 && hasCycle(graph, state, i)) {
                return false;  // 发现环
            }
        }
        return true;  // 无环
    }

private:
    bool hasCycle(vector<vector<int>>& graph, vector<int>& state, int node) {
        state[node] = 1;  // 标记为"正在访问"

        for (int neighbor : graph[node]) {
            if (state[neighbor] == 1) {
                return true;   // 遇到正在访问的节点 -> 有环!
            }
            if (state[neighbor] == 0 && hasCycle(graph, state, neighbor)) {
                return true;   // 递归发现环
            }
        }

        state[node] = 2;  // 标记为"已完成"
        return false;
    }
};

// ============================================================================
// 题目3: 克隆图 (LeetCode 133)
// ============================================================================
/**
 * 给你无向连通图中一个节点的引用，请你返回该图的深拷贝。
 *
 * ASCII图示:
 *   原始图:           克隆图:
 *   1 --- 2           1' --- 2'
 *   |     |           |      |
 *   4 --- 3           4' --- 3'
 *
 * 关键: 用哈希表记录 原节点 -> 克隆节点 的映射，避免重复创建
 */

// Node定义
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() : val(0) {}
    Node(int _val) : val(_val) {}
    Node(int _val, vector<Node*> _neighbors) : val(_val), neighbors(_neighbors) {}
};

// 方法一: DFS + 哈希表
// 时间复杂度: O(V+E)，空间复杂度: O(V)
class CloneGraph_DFS {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        unordered_map<Node*, Node*> visited;  // 原节点 -> 克隆节点
        return dfs(node, visited);
    }

private:
    Node* dfs(Node* node, unordered_map<Node*, Node*>& visited) {
        // 如果已经克隆过，直接返回克隆节点
        if (visited.count(node)) {
            return visited[node];
        }

        // 创建克隆节点 (先创建节点，再处理邻居)
        // 注意: 必须先放入map，否则递归时会重复创建
        Node* clone = new Node(node->val);
        visited[node] = clone;

        // 递归克隆所有邻居
        for (Node* neighbor : node->neighbors) {
            clone->neighbors.push_back(dfs(neighbor, visited));
        }

        return clone;
    }
};

// 方法二: BFS + 哈希表
// 时间复杂度: O(V+E)，空间复杂度: O(V)
class CloneGraph_BFS {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        unordered_map<Node*, Node*> visited;

        // 先创建起点的克隆
        visited[node] = new Node(node->val);
        queue<Node*> q;
        q.push(node);

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            // 处理当前节点的所有邻居
            for (Node* neighbor : curr->neighbors) {
                if (!visited.count(neighbor)) {
                    // 邻居未被克隆 -> 创建克隆并入队
                    visited[neighbor] = new Node(neighbor->val);
                    q.push(neighbor);
                }
                // 将克隆邻居加入当前克隆节点的邻居列表
                visited[curr]->neighbors.push_back(visited[neighbor]);
            }
        }

        return visited[node];
    }
};

// ============================================================================
// 题目4: 腐烂的橘子 (LeetCode 994)
// ============================================================================
/**
 * 在给定的网格中，每个单元格可以是以下三个值之一:
 *   0 - 空单元格
 *   1 - 新鲜橘子
 *   2 - 腐烂的橘子
 *
 * 每分钟，腐烂的橘子会使上下左右的新鲜橘子腐烂。
 * 返回直到没有新鲜橘子为止所必须经过的最小分钟数。如果不可能，返回-1。
 *
 * 示例:
 *   输入: [[2,1,1],[1,1,0],[0,1,1]]
 *   输出: 4
 *
 * ASCII图示 - 多源BFS过程:
 *
 *   t=0:           t=1:           t=2:           t=3:           t=4:
 *   2 1 1          2 2 1          2 2 2          2 2 2          2 2 2
 *   1 1 0          2 1 0          2 2 0          2 2 0          2 2 0
 *   0 1 1          0 1 1          0 1 2          0 2 2          0 2 2
 *
 * 关键: 所有初始腐烂的橘子同时开始传播 -> 多源BFS (每层=1分钟)
 */

class OrangesRotting {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        queue<pair<int, int>> q;  // 腐烂橘子的队列
        int fresh = 0;            // 新鲜橘子计数

        // Step1: 将所有初始腐烂的橘子入队，统计新鲜橘子数
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (grid[i][j] == 2) {
                    q.push({i, j});
                } else if (grid[i][j] == 1) {
                    fresh++;
                }
            }
        }

        // 没有新鲜橘子 -> 0分钟
        if (fresh == 0) return 0;

        int minutes = 0;
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        // Step2: 多源BFS - 每一层代表一分钟
        while (!q.empty()) {
            int size = q.size();  // 当前层的腐烂橘子数量
            bool rotted = false;  // 这一分钟是否有新的橘子腐烂

            for (int i = 0; i < size; i++) {
                auto [x, y] = q.front();
                q.pop();

                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d];
                    int ny = y + dy[d];

                    if (nx >= 0 && nx < rows && ny >= 0 && ny < cols
                        && grid[nx][ny] == 1) {
                        grid[nx][ny] = 2;   // 腐烂!
                        fresh--;             // 新鲜橘子减1
                        rotted = true;
                        q.push({nx, ny});
                    }
                }
            }

            // 如果这一分钟有橘子腐烂，时间+1
            if (rotted) minutes++;
        }

        // 如果还有新鲜橘子剩下，说明无法全部腐烂
        return fresh == 0 ? minutes : -1;
    }
};

// ============================================================================
// 题目5: 单词接龙 (LeetCode 127)
// ============================================================================
/**
 * 给定两个单词 beginWord 和 endWord，和一个字典 wordList。
 * 找到从 beginWord 到 endWord 的最短转换序列的长度。
 * 每次转换只能改变一个字母，且转换后的单词必须在字典中。
 *
 * 示例:
 *   beginWord = "hit", endWord = "cog"
 *   wordList = ["hot","dot","dog","lot","log","cog"]
 *   输出: 5 (hit -> hot -> dot -> dog -> cog)
 *
 * ASCII图示 - BFS最短路径:
 *
 *   hit (beginWord)
 *    |
 *   hot
 *   / \
 *  dot  lot
 *  |     |
 *  dog  log
 *   \   /
 *    cog (endWord)
 *
 * 构建邻接表的技巧:
 *   用通配模式! "hot" -> "*ot", "h*t", "ho*"
 *   同一模式的单词互为邻居
 *   例如 *ot: [hot, dot, lot], *og: [dog, log, cog]
 */

// 方法一: BFS (标准)
// 时间复杂度: O(M^2 * N)，M为单词长度，N为单词数量
class LadderLength_BFS {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // 将wordList放入set，方便查找
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) return 0;

        queue<string> q;
        q.push(beginWord);
        int level = 1;  // 起始单词算第1层

        while (!q.empty()) {
            int size = q.size();

            for (int i = 0; i < size; i++) {
                string word = q.front();
                q.pop();

                // 尝试改变每个位置的每个字母
                for (int j = 0; j < word.size(); j++) {
                    char original = word[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;
                        word[j] = c;

                        if (word == endWord) {
                            return level + 1;  // 找到终点
                        }

                        if (wordSet.count(word)) {
                            q.push(word);
                            wordSet.erase(word);  // 标记已访问
                        }
                    }

                    word[j] = original;  // 恢复
                }
            }

            level++;  // 进入下一层
        }

        return 0;  // 无法到达
    }
};

// 方法二: 双向BFS优化
// 从起点和终点同时开始搜索，相遇时即为最短路径
// 时间复杂度: O(M^2 * N)，但实际运行更快(搜索空间更小)
class LadderLength_BiBFS {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) return 0;

        // 两端的已访问集合
        unordered_set<string> beginVisited;
        unordered_set<string> endVisited;
        beginVisited.insert(beginWord);
        endVisited.insert(endWord);

        int level = 1;

        while (!beginVisited.empty() && !endVisited.empty()) {
            // 始终从小的一端扩展 (优化搜索效率)
            if (beginVisited.size() > endVisited.size()) {
                swap(beginVisited, endVisited);
            }

            unordered_set<string> nextLevel;

            for (const string& word : beginVisited) {
                string curr = word;

                for (int j = 0; j < curr.size(); j++) {
                    char original = curr[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == original) continue;
                        curr[j] = c;

                        // 如果另一端已经访问过 -> 相遇!
                        if (endVisited.count(curr)) {
                            return level + 1;
                        }

                        if (wordSet.count(curr)) {
                            nextLevel.insert(curr);
                            wordSet.erase(curr);
                        }
                    }

                    curr[j] = original;
                }
            }

            beginVisited = nextLevel;
            level++;
        }

        return 0;
    }
};

// 方法三: BFS + 邻接表 (预构建通配模式)
class LadderLength_AdjList {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) return 0;
        wordSet.insert(beginWord);

        // Step1: 构建邻接表 (通过通配模式)
        // 每个模式映射到属于该模式的所有单词
        unordered_map<string, vector<string>> patternMap;

        for (const string& word : wordSet) {
            for (int i = 0; i < word.size(); i++) {
                string pattern = word;
                pattern[i] = '*';  // "hot" -> "*ot"
                patternMap[pattern].push_back(word);
            }
        }

        // Step2: BFS
        queue<string> q;
        unordered_set<string> visited;
        q.push(beginWord);
        visited.insert(beginWord);
        int level = 1;

        while (!q.empty()) {
            int size = q.size();

            for (int i = 0; i < size; i++) {
                string word = q.front();
                q.pop();

                // 生成该单词的所有通配模式，找到邻居
                for (int j = 0; j < word.size(); j++) {
                    string pattern = word;
                    pattern[j] = '*';

                    for (const string& neighbor : patternMap[pattern]) {
                        if (neighbor == endWord) {
                            return level + 1;
                        }
                        if (!visited.count(neighbor)) {
                            visited.insert(neighbor);
                            q.push(neighbor);
                        }
                    }
                }
            }

            level++;
        }

        return 0;
    }
};

// ============================================================================
// 题目6: 被围绕的区域 (LeetCode 130)
// ============================================================================
/**
 * 给你一个 m x n 的矩阵 board，由若干字符 'X' 和 'O' 组成。
 * 捕获所有被围绕的区域：将所有被 'X' 围绕的 'O' 区域置为 'X'。
 * 一个 'O' 如果在边界上，或者与边界上的 'O' 相连，则不会被捕获。
 *
 * 示例:
 *   输入:
 *     X X X X
 *     X O O X
 *     X X O X
 *     X O X X
 *   输出:
 *     X X X X
 *     X X X X
 *     X X X X
 *     X O X X
 *
 * ASCII图示 - 思路:
 *
 *   从边界上的 'O' 出发做DFS/BFS，标记所有与边界连通的 'O'
 *   这些 'O' 不会被捕获（因为它们没有被完全包围）
 *   剩余未标记的 'O' 全部变为 'X'
 *
 *   边界 'O':           标记后(用#表示):     最终结果:
 *   X X X X              X X X X              X X X X
 *   X O O X              X O O X              X X X X
 *   X X O X              X X O X              X X X X
 *   X O X X              X # X X              X O X X
 *
 *   (0,1)处的O与边界(3,1)连通 -> 不被捕获
 *   (1,1)(1,2)(2,2)处的O不与边界连通 -> 被捕获 -> 变为X
 */

// 方法: DFS从边界开始
// 时间复杂度: O(M*N)，空间复杂度: O(M*N) 最坏情况递归深度
class Solve {
public:
    void solve(vector<vector<char>>& board) {
        if (board.empty() || board[0].empty()) return;

        int rows = board.size();
        int cols = board[0].size();

        // Step1: 从边界的 'O' 出发，用DFS标记所有与边界连通的 'O'
        // 标记方式: 将 'O' 暂时变为 '#'
        for (int i = 0; i < rows; i++) {
            if (board[i][0] == 'O') dfs(board, i, 0);           // 左边界
            if (board[i][cols - 1] == 'O') dfs(board, i, cols - 1); // 右边界
        }
        for (int j = 0; j < cols; j++) {
            if (board[0][j] == 'O') dfs(board, 0, j);           // 上边界
            if (board[rows - 1][j] == 'O') dfs(board, rows - 1, j); // 下边界
        }

        // Step2: 遍历整个矩阵
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] == 'O') {
                    board[i][j] = 'X';   // 未标记的 'O' 被捕获，变为 'X'
                } else if (board[i][j] == '#') {
                    board[i][j] = 'O';   // 标记的 '#' 恢复为 'O'
                }
            }
        }
    }

private:
    void dfs(vector<vector<char>>& board, int i, int j) {
        int rows = board.size();
        int cols = board[0].size();

        // 边界检查 + 只处理 'O'
        if (i < 0 || i >= rows || j < 0 || j >= cols || board[i][j] != 'O')
            return;

        board[i][j] = '#';  // 标记为安全（与边界连通）

        // 向四个方向搜索
        dfs(board, i - 1, j);
        dfs(board, i + 1, j);
        dfs(board, i, j - 1);
        dfs(board, i, j + 1);
    }
};

// ============================================================================
// 题目7: 课程表 II (LeetCode 210)
// ============================================================================
/**
 * 现在你总共有 numCourses 门课需要选，记为 0 到 numCourses-1。
 * 给定先修条件数组 prerequisites，返回你为了学完所有课程所安排的学习顺序。
 * 如果不可能完成所有课程，返回空数组。
 *
 * 示例:
 *   输入: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 *   输出: [0,2,1,3] 或 [0,1,2,3]  (合法的拓扑排序)
 *
 * ASCII图示:
 *   0 -> 1 -> 3
 *   |         ^
 *   +-> 2 ----+
 *
 *   拓扑排序: 不断移除入度为0的节点
 *   Step1: 移除0 -> 结果[0], 1和2的入度变为0
 *   Step2: 移除1(或2) -> 结果[0,1], 3的入度-1
 *   Step3: 移除2 -> 结果[0,1,2], 3的入度变为0
 *   Step4: 移除3 -> 结果[0,1,2,3]
 */

// 方法: BFS拓扑排序 (Kahn算法)
// 时间复杂度: O(V+E)，空间复杂度: O(V+E)
class FindOrder {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // 构建邻接表和入度数组
        vector<vector<int>> graph(numCourses);
        vector<int> inDegree(numCourses, 0);

        for (auto& pre : prerequisites) {
            graph[pre[1]].push_back(pre[0]);  // pre[1] -> pre[0]
            inDegree[pre[0]]++;
        }

        // 将所有入度为0的节点入队
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> order;  // 存储拓扑排序结果

        while (!q.empty()) {
            int course = q.front();
            q.pop();
            order.push_back(course);

            // 将course的所有后继节点入度减1
            for (int next : graph[course]) {
                inDegree[next]--;
                if (inDegree[next] == 0) {
                    q.push(next);
                }
            }
        }

        // 如果排序结果包含所有课程，返回排序；否则返回空数组（有环）
        if ((int)order.size() == numCourses) {
            return order;
        }
        return {};
    }
};

// ============================================================================
// 题目8: 图是否是树 (LeetCode 261)
// ============================================================================
/**
 * 给定编号从 0 到 n-1 的 n 个节点和一个无向边列表 edges，
 * 判断这些边是否构成一棵合法的树。
 *
 * 树的定义: 连通 + 无环 + 边数 = n-1
 *
 * 示例:
 *   输入: n = 5, edges = [[0,1],[0,2],[0,3],[1,4]]
 *   输出: true
 *
 *   输入: n = 5, edges = [[0,1],[1,2],[2,3],[1,3],[1,4]]
 *   输出: false (存在环)
 *
 * ASCII图示:
 *   合法的树:          非法(有环):
 *       0              0
 *      /|\             |
 *     1 2 3            1 - 3
 *     |                |   |
 *     4                2   4
 *
 * 判断条件:
 *   1. 边数必须恰好为 n-1 (n个节点的树必有n-1条边)
 *   2. 图必须连通 (从任意节点出发DFS能访问所有节点)
 *   满足这两个条件就一定是树 (边数=n-1且连通 -> 必然无环)
 */

// 方法: 边数检查 + DFS连通性检查
// 时间复杂度: O(V+E)，空间复杂度: O(V+E)
class ValidTree {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        // 条件1: 树的边数必须恰好为 n-1
        if ((int)edges.size() != n - 1) return false;

        // 条件2: 检查连通性 (从节点0出发DFS)
        // 构建邻接表
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            graph[e[1]].push_back(e[0]);  // 无向图
        }

        // DFS遍历
        vector<bool> visited(n, false);
        stack<int> st;
        st.push(0);
        visited[0] = true;
        int count = 0;

        while (!st.empty()) {
            int node = st.top();
            st.pop();
            count++;

            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    st.push(neighbor);
                }
            }
        }

        // 如果访问了所有节点，则图连通
        return count == n;
    }
};

// ============================================================================
// 题目9: 钥匙和房间 (LeetCode 841)
// ============================================================================
/**
 * 有 n 个房间，编号从 0 到 n-1，所有房间初始都是锁着的。
 * 你一开始在 0 号房间，里面有其他房间的钥匙。
 * 每个房间都有一把不同的钥匙可以打开另一个房间。
 * 请你判断是否可以进入所有房间。
 *
 * 示例:
 *   输入: rooms = [[1],[2],[3],[]]
 *   输出: true
 *   解释: 我们从0号房间拿到1号钥匙 -> 进入1号拿到2号 -> 进入2号拿到3号 -> 进入3号
 *
 * ASCII图示:
 *   0 -> 1 -> 2 -> 3 -> (空)
 *   可以访问所有房间!
 *
 *   输入: rooms = [[1,3],[3,0,1],[2],[0]]
 *   输出: false
 *   解释: 2号房间的钥匙无法从其他房间获得
 *
 * 思路: DFS/BFS从0号房间开始遍历，检查是否能访问所有房间
 */

// 方法: DFS
// 时间复杂度: O(N+E)，空间复杂度: O(N)
class CanVisitAllRooms {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        int n = rooms.size();
        vector<bool> visited(n, false);

        // DFS从0号房间开始
        stack<int> st;
        st.push(0);
        visited[0] = true;
        int count = 0;

        while (!st.empty()) {
            int room = st.top();
            st.pop();
            count++;

            // 收集当前房间中的钥匙
            for (int key : rooms[room]) {
                if (!visited[key]) {
                    visited[key] = true;
                    st.push(key);
                }
            }
        }

        return count == n;
    }
};

// ============================================================================
// 题目10: 太平洋大西洋水流问题 (LeetCode 417)
// ============================================================================
/**
 * 有一个 m x n 的长方形岛屿，岛屿的太平洋和大西洋边界分别在左上和右下。
 * 岛屿上每个格子的高度给定。水可以流向相邻的格子（上下左右），
 * 条件是目标格子的高度不大于当前格子。
 *
 * 找出所有既能流到太平洋又能流到大西洋的格子坐标。
 *
 * 示例:
 *   输入: heights = [[1,2,2,3,5],[3,2,3,4,4],[2,4,5,3,1],[6,7,1,4,5],[5,1,1,2,4]]
 *   输出: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]
 *
 * ASCII图示:
 *   太平洋在左上 (第0行 + 第0列)
 *   大西洋在右下 (第m-1行 + 第n-1列)
 *
 *   P P P P P
 *   P . . . A
 *   P . . . A
 *   P . . . A
 *   P A A A A
 *
 * 思路:
 *   不能从每个格子出发模拟水流 (太慢)。
 *   正确做法: 从海洋出发反向搜索!
 *   - 从太平洋边界出发DFS/BFS，标记所有能到达的格子
 *   - 从大西洋边界出发DFS/BFS，标记所有能到达的格子
 *   - 两个集合的交集就是答案
 */

// 方法: 双向DFS (从两个海洋分别出发)
// 时间复杂度: O(M*N)，空间复杂度: O(M*N)
class PacificAtlantic {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        vector<vector<int>> result;
        if (heights.empty() || heights[0].empty()) return result;

        int rows = heights.size();
        int cols = heights[0].size();

        // 分别标记能到达太平洋和大西洋的格子
        vector<vector<bool>> pacific(rows, vector<bool>(cols, false));
        vector<vector<bool>> atlantic(rows, vector<bool>(cols, false));

        // 从太平洋边界出发DFS (上边界 + 左边界)
        for (int i = 0; i < rows; i++) {
            dfs(heights, pacific, i, 0, rows, cols);           // 左边界
        }
        for (int j = 0; j < cols; j++) {
            dfs(heights, pacific, 0, j, rows, cols);           // 上边界
        }

        // 从大西洋边界出发DFS (下边界 + 右边界)
        for (int i = 0; i < rows; i++) {
            dfs(heights, atlantic, i, cols - 1, rows, cols);   // 右边界
        }
        for (int j = 0; j < cols; j++) {
            dfs(heights, atlantic, rows - 1, j, rows, cols);   // 下边界
        }

        // 找交集: 既能到太平洋又能到大西洋的格子
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pacific[i][j] && atlantic[i][j]) {
                    result.push_back({i, j});
                }
            }
        }

        return result;
    }

private:
    // 反向DFS: 从海洋边界向内搜索，水从低到高流 (反向)
    void dfs(vector<vector<int>>& heights, vector<vector<bool>>& visited,
             int i, int j, int rows, int cols) {
        // 已访问则跳过
        if (visited[i][j]) return;

        visited[i][j] = true;

        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d];
            int nj = j + dy[d];

            // 边界检查 + 反向条件: 邻居高度 >= 当前高度 (水能从邻居流到当前格子)
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols
                && !visited[ni][nj]
                && heights[ni][nj] >= heights[i][j]) {
                dfs(heights, visited, ni, nj, rows, cols);
            }
        }
    }
};

// ============================================================================
// 测试代码
// ============================================================================

void testSolve() {
    cout << "========================================" << endl;
    cout << "题目6: 被围绕的区域 (LeetCode 130)" << endl;
    cout << "========================================" << endl;

    Solve sol;

    vector<vector<char>> board = {
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'X'},
        {'X', 'O', 'X', 'X'}
    };

    cout << "输入:" << endl;
    for (auto& row : board) {
        cout << "  ";
        for (char c : row) cout << c << " ";
        cout << endl;
    }

    sol.solve(board);

    cout << "输出:" << endl;
    for (auto& row : board) {
        cout << "  ";
        for (char c : row) cout << c << " ";
        cout << endl;
    }

    // 测试2: 全部是O
    vector<vector<char>> board2 = {
        {'O', 'O', 'O'},
        {'O', 'O', 'O'},
        {'O', 'O', 'O'}
    };
    sol.solve(board2);
    cout << "\n全部O测试 (应全部保持O):" << endl;
    for (auto& row : board2) {
        cout << "  ";
        for (char c : row) cout << c << " ";
        cout << endl;
    }
    cout << endl;
}

void testFindOrder() {
    cout << "========================================" << endl;
    cout << "题目7: 课程表 II (LeetCode 210)" << endl;
    cout << "========================================" << endl;

    FindOrder sol;

    // 测试1: 正常情况
    vector<vector<int>> pre1 = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    auto res1 = sol.findOrder(4, pre1);
    cout << "测试1 期望: 有效拓扑排序, 结果: [";
    for (int i = 0; i < (int)res1.size(); i++)
        cout << res1[i] << (i < (int)res1.size() - 1 ? "," : "");
    cout << "]"
         << (res1.size() == 4 ? " [通过]" : " [失败]") << endl;

    // 测试2: 有环
    vector<vector<int>> pre2 = {{1, 0}, {0, 1}};
    auto res2 = sol.findOrder(2, pre2);
    cout << "测试2 有环, 结果: [";
    for (int i = 0; i < (int)res2.size(); i++)
        cout << res2[i] << (i < (int)res2.size() - 1 ? "," : "");
    cout << "]"
         << (res2.empty() ? " [通过]" : " [失败]") << endl;

    // 测试3: 无依赖
    auto res3 = sol.findOrder(3, {});
    cout << "测试3 无依赖, 结果: [";
    for (int i = 0; i < (int)res3.size(); i++)
        cout << res3[i] << (i < (int)res3.size() - 1 ? "," : "");
    cout << "]"
         << (res3.size() == 3 ? " [通过]" : " [失败]") << endl;
    cout << endl;
}

void testValidTree() {
    cout << "========================================" << endl;
    cout << "题目8: 图是否是树 (LeetCode 261)" << endl;
    cout << "========================================" << endl;

    ValidTree sol;

    // 测试1: 合法的树
    vector<vector<int>> edges1 = {{0, 1}, {0, 2}, {0, 3}, {1, 4}};
    cout << "测试1 n=5, 边=[[0,1],[0,2],[0,3],[1,4]]: "
         << (sol.validTree(5, edges1) ? "true" : "false")
         << " (期望: true)" << endl;

    // 测试2: 有环
    vector<vector<int>> edges2 = {{0, 1}, {1, 2}, {2, 3}, {1, 3}, {1, 4}};
    cout << "测试2 n=5, 边=[[0,1],[1,2],[2,3],[1,3],[1,4]]: "
         << (sol.validTree(5, edges2) ? "true" : "false")
         << " (期望: false)" << endl;

    // 测试3: 不连通
    vector<vector<int>> edges3 = {{0, 1}, {2, 3}};
    cout << "测试3 n=4, 边=[[0,1],[2,3]]: "
         << (sol.validTree(4, edges3) ? "true" : "false")
         << " (期望: false)" << endl;

    // 测试4: 单节点
    cout << "测试4 n=1, 边=[]: "
         << (sol.validTree(1, {}) ? "true" : "false")
         << " (期望: true)" << endl;
    cout << endl;
}

void testCanVisitAllRooms() {
    cout << "========================================" << endl;
    cout << "题目9: 钥匙和房间 (LeetCode 841)" << endl;
    cout << "========================================" << endl;

    CanVisitAllRooms sol;

    // 测试1: 可以访问所有房间
    vector<vector<int>> rooms1 = {{1}, {2}, {3}, {}};
    cout << "测试1 rooms=[[1],[2],[3],[]]: "
         << (sol.canVisitAllRooms(rooms1) ? "true" : "false")
         << " (期望: true)" << endl;

    // 测试2: 无法访问所有房间
    vector<vector<int>> rooms2 = {{1, 3}, {3, 0, 1}, {2}, {0}};
    cout << "测试2 rooms=[[1,3],[3,0,1],[2],[0]]: "
         << (sol.canVisitAllRooms(rooms2) ? "true" : "false")
         << " (期望: false)" << endl;

    // 测试3: 单个房间
    vector<vector<int>> rooms3 = {{}};
    cout << "测试3 rooms=[[]]: "
         << (sol.canVisitAllRooms(rooms3) ? "true" : "false")
         << " (期望: true)" << endl;

    // 测试4: 复杂情况
    vector<vector<int>> rooms4 = {{1}, {2}, {0, 3}, {}};
    cout << "测试4 rooms=[[1],[2],[0,3],[]]: "
         << (sol.canVisitAllRooms(rooms4) ? "true" : "false")
         << " (期望: true)" << endl;
    cout << endl;
}

void testPacificAtlantic() {
    cout << "========================================" << endl;
    cout << "题目10: 太平洋大西洋水流问题 (LeetCode 417)" << endl;
    cout << "========================================" << endl;

    PacificAtlantic sol;

    vector<vector<int>> heights = {
        {1, 2, 2, 3, 5},
        {3, 2, 3, 4, 4},
        {2, 4, 5, 3, 1},
        {6, 7, 1, 4, 5},
        {5, 1, 1, 2, 4}
    };

    auto result = sol.pacificAtlantic(heights);
    cout << "测试1 结果 (期望: [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]):" << endl;
    cout << "  [";
    for (int i = 0; i < (int)result.size(); i++) {
        cout << "[" << result[i][0] << "," << result[i][1] << "]"
             << (i < (int)result.size() - 1 ? "," : "");
    }
    cout << "]" << endl;

    // 测试2: 1x1矩阵
    vector<vector<int>> h2 = {{1}};
    auto result2 = sol.pacificAtlantic(h2);
    cout << "\n测试2 1x1矩阵: ";
    cout << "[";
    for (int i = 0; i < (int)result2.size(); i++) {
        cout << "[" << result2[i][0] << "," << result2[i][1] << "]"
             << (i < (int)result2.size() - 1 ? "," : "");
    }
    cout << "] (期望: [[0,0]])" << endl;
    cout << endl;
}

void testNumIslands() {
    cout << "========================================" << endl;
    cout << "题目1: 岛屿数量 (LeetCode 200)" << endl;
    cout << "========================================" << endl;

    NumIslands_DFS sol_dfs;
    NumIslands_BFS sol_bfs;

    // 测试用例1
    vector<vector<char>> grid1 = {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };
    cout << "测试1 - DFS: " << sol_dfs.numIslands(grid1) << endl;  // 输出: 1

    grid1 = {{'1','1','1','1','0'},{'1','1','0','1','0'},{'1','1','0','0','0'},{'0','0','0','0','0'}};
    cout << "测试1 - BFS: " << sol_bfs.numIslands(grid1) << endl;  // 输出: 1

    // 测试用例2: 多个岛屿
    vector<vector<char>> grid2 = {
        {'1','1','0','0','0'},{'1','1','0','0','0'},
        {'0','0','1','0','0'},{'0','0','0','1','1'}
    };
    cout << "测试2 - DFS: " << sol_dfs.numIslands(grid2) << endl;  // 输出: 3

    grid2 = {{'1','1','0','0','0'},{'1','1','0','0','0'},{'0','0','1','0','0'},{'0','0','0','1','1'}};
    cout << "测试2 - BFS: " << sol_bfs.numIslands(grid2) << endl;  // 输出: 3
    cout << endl;
}

void testCanFinish() {
    cout << "========================================" << endl;
    cout << "题目2: 课程表 (LeetCode 207)" << endl;
    cout << "========================================" << endl;

    CanFinish_BFS sol_bfs;
    CanFinish_DFS sol_dfs;

    vector<vector<int>> pre1 = {{1, 0}};
    cout << "测试1 [1,0] - BFS: " << (sol_bfs.canFinish(2, pre1) ? "true" : "false")
         << " | DFS: " << (sol_dfs.canFinish(2, pre1) ? "true" : "false") << endl;

    vector<vector<int>> pre2 = {{1, 0}, {0, 1}};
    cout << "测试2 有环 - BFS: " << (sol_bfs.canFinish(2, pre2) ? "true" : "false")
         << " | DFS: " << (sol_dfs.canFinish(2, pre2) ? "true" : "false") << endl;

    vector<vector<int>> pre3 = {{1, 0}, {2, 1}, {3, 2}};
    cout << "测试3 链式 - BFS: " << (sol_bfs.canFinish(4, pre3) ? "true" : "false")
         << " | DFS: " << (sol_dfs.canFinish(4, pre3) ? "true" : "false") << endl;
    cout << endl;
}

void testCloneGraph() {
    cout << "========================================" << endl;
    cout << "题目3: 克隆图 (LeetCode 133)" << endl;
    cout << "========================================" << endl;

    // 构建测试图: 1--2, 1--4, 2--3, 3--4
    Node* n1 = new Node(1);
    Node* n2 = new Node(2);
    Node* n3 = new Node(3);
    Node* n4 = new Node(4);
    n1->neighbors = {n2, n4};
    n2->neighbors = {n1, n3};
    n3->neighbors = {n2, n4};
    n4->neighbors = {n1, n3};

    CloneGraph_DFS sol_dfs;
    Node* clone1 = sol_dfs.cloneGraph(n1);
    cout << "DFS克隆 - 节点1邻居: ";
    for (auto n : clone1->neighbors) cout << n->val << " ";
    cout << endl;

    CloneGraph_BFS sol_bfs;
    Node* clone2 = sol_bfs.cloneGraph(n1);
    cout << "BFS克隆 - 节点1邻居: ";
    for (auto n : clone2->neighbors) cout << n->val << " ";
    cout << endl;

    // 验证深拷贝
    clone1->val = 999;
    cout << "深拷贝验证 - 原值:" << n1->val << " 克隆值:" << clone1->val << endl;
    cout << endl;

    delete n1; delete n2; delete n3; delete n4;
}

void testOrangesRotting() {
    cout << "========================================" << endl;
    cout << "题目4: 腐烂的橘子 (LeetCode 994)" << endl;
    cout << "========================================" << endl;

    OrangesRotting sol;

    // 测试1
    vector<vector<int>> grid1 = {{2,1,1},{1,1,0},{0,1,1}};
    cout << "测试1: " << sol.orangesRotting(grid1) << endl;  // 输出: 4

    // 测试2: 无法全部腐烂
    vector<vector<int>> grid2 = {{2,1,1},{0,1,1},{1,0,1}};
    cout << "测试2 (不可能): " << sol.orangesRotting(grid2) << endl;  // 输出: -1

    // 测试3: 已经全部腐烂
    vector<vector<int>> grid3 = {{0,2}};
    cout << "测试3 (已全部腐烂): " << sol.orangesRotting(grid3) << endl;  // 输出: 0
    cout << endl;
}

void testLadderLength() {
    cout << "========================================" << endl;
    cout << "题目5: 单词接龙 (LeetCode 127)" << endl;
    cout << "========================================" << endl;

    LadderLength_BFS sol_bfs;
    LadderLength_BiBFS sol_bibfs;
    LadderLength_AdjList sol_adj;

    // 测试1
    string begin1 = "hit", end1 = "cog";
    vector<string> list1 = {"hot","dot","dog","lot","log","cog"};
    cout << "测试1 - BFS:     " << sol_bfs.ladderLength(begin1, end1, list1) << endl;

    vector<string> list1b = {"hot","dot","dog","lot","log","cog"};
    cout << "测试1 - 双向BFS: " << sol_bibfs.ladderLength(begin1, end1, list1b) << endl;

    vector<string> list1c = {"hot","dot","dog","lot","log","cog"};
    cout << "测试1 - 邻接表:  " << sol_adj.ladderLength(begin1, end1, list1c) << endl;

    // 测试2: 无法转换
    string begin2 = "hit", end2 = "cog";
    vector<string> list2 = {"hot","dot","dog","lot","log"};
    cout << "测试2 (无法到达) - BFS: " << sol_bfs.ladderLength(begin2, end2, list2) << endl;
    cout << endl;
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    cout << "****************************************" << endl;
    cout << "*   图论 BFS/DFS 经典例题合集          *" << endl;
    cout << "****************************************" << endl;
    cout << endl;

    testNumIslands();
    testCanFinish();
    testCloneGraph();
    testOrangesRotting();
    testLadderLength();
    testSolve();
    testFindOrder();
    testValidTree();
    testCanVisitAllRooms();
    testPacificAtlantic();

    cout << "========================================" << endl;
    cout << "所有测试完成!" << endl;
    cout << "========================================" << endl;

    return 0;
}
