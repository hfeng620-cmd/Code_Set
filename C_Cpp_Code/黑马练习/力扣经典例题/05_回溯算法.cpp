/**
 * ========================================================================
 *  LeetCode 经典回溯算法题集 (Backtracking)
 * ========================================================================
 *
 *  回溯算法核心思想:
 *      回溯算法本质上是对决策树的遍历。在每一步做出选择，
 *      递归进入下一层，如果不满足条件则撤销选择（回溯），
 *      尝试其他分支，直到找到所有解或目标解。
 *
 *  通用回溯算法框架:
 *  ┌─────────────────────────────────────────────────┐
 *  │  void backtrack(路径, 选择列表) {                │
 *  │      if (满足结束条件) {                         │
 *  │          result.add(路径);                       │
 *  │          return;                                 │
 *  │      }                                           │
 *  │      for (选择 : 选择列表) {                     │
 *  │          做选择;                                 │
 *  │          backtrack(路径, 选择列表);              │
 *  │          撤销选择;                               │
 *  │      }                                           │
 *  │  }                                               │
 *  └─────────────────────────────────────────────────┘
 *
 *  本文件包含以下12道经典题目:
 *      1. 子集              (LeetCode 78)   - Medium
 *      2. 组合总和          (LeetCode 39)   - Medium
 *      3. 全排列            (LeetCode 46)   - Medium
 *      4. 单词搜索          (LeetCode 79)   - Medium
 *      5. N皇后             (LeetCode 51)   - Hard
 *      6. 括号生成          (LeetCode 22)   - Medium
 *      7. 电话号码的字母组合 (LeetCode 17)   - Medium
 *      8. 分割回文串        (LeetCode 131)  - Medium
 *      9. 复原IP地址        (LeetCode 93)   - Medium
 *     10. 火柴拼正方形      (LeetCode 473)  - Medium
 *     11. 划分为k个相等的子集 (LeetCode 698) - Medium
 *     12. 解数独            (LeetCode 37)   - Hard
 *
 *  编译: g++ -std=c++17 -O2 -o backtrack 05_回溯算法.cpp
 *  运行: ./backtrack
 */

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;


/* ======================================================================
 *  第1题: 子集 (LeetCode 78, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一组不含重复元素的整数数组 nums，返回该数组所有可能的子集。
 *      解集不能包含重复的子集。
 *
 *  示例:
 *      输入: nums = [1, 2, 3]
 *      输出: [[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]]
 *
 *  决策树 (以 [1,2,3] 为例):
 *
 *                          []
 *                        / | \
 *                      /   |   \
 *                   [1]   [2]   [3]
 *                  / \      |
 *                /    \     |
 *             [1,2] [1,3] [2,3]
 *              |
 *              |
 *           [1,2,3]
 *
 *  解题思路:
 *      每个元素有两种选择: 选 或 不选。
 *      从第一个元素开始，依次决定每个元素是否加入当前子集。
 *      当遍历完所有元素时，当前路径就是一个合法的子集。
 *
 *  回溯框架:
 *      void backtrack(当前路径path, 起始位置start) {
 *          if (遍历到末尾) { 收集当前路径; return; }
 *          for (从start开始的每个元素) {
 *              做选择: 将元素加入path;
 *              backtrack(path, i+1);
 *              撤销选择: 将元素从path弹出;
 *          }
 *      }
 *
 *  时间复杂度: O(n * 2^n)  -- 共有2^n个子集，每个子集拷贝需要O(n)
 *  空间复杂度: O(n)        -- 递归栈深度最多为n
 */

class Solution_Subsets {
public:
    // 主函数: 返回nums的所有子集
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;  // 存储所有子集的结果
        vector<int> path;            // 当前正在构建的子集路径

        // 从索引0开始回溯
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    // 回溯函数: nums为原数组, start为当前选择的起始位置
    void backtrack(const vector<int>& nums, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 每到达一个节点，就收集当前路径作为一个子集
        // (包括空集[]，即start=0时立即收集)
        result.push_back(path);

        // 从start位置开始，依次尝试将每个元素加入子集
        for (int i = start; i < nums.size(); i++) {
            // ---- 做选择 ----
            path.push_back(nums[i]);

            // ---- 递归进入下一层 ----
            // 注意: 下一层从i+1开始，避免重复选取同一个元素
            backtrack(nums, i + 1, path, result);

            // ---- 撤销选择 (回溯) ----
            path.pop_back();
        }
    }
};


/* ======================================================================
 *  第2题: 组合总和 (LeetCode 39, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个无重复元素的数组 candidates 和一个目标数 target，
 *      找出 candidates 中所有可以使数字和为 target 的组合。
 *      candidates 中的数字可以无限制重复被选取。
 *
 *  示例:
 *      输入: candidates = [2,3,6,7], target = 7
 *      输出: [[2,2,3], [7]]
 *
 *  决策树 (以 [2,3,6,7], target=7 为例):
 *
 *                        剩余=7
 *                      /   |   \    \
 *                    2     3     6    7
 *               剩余=5  剩余=4  剩余=1  剩余=0 -> 找到!
 *              / | \     / \
 *             2  3  6   3   (剩余=1, 剪枝)
 *          剩余=3 剩余=2
 *           /  \
 *          2    3
 *      剩余=1 剩余=0 -> 找到!
 *
 *  解题思路:
 *      回溯遍历所有可能的组合，利用 remaining (剩余目标值) 来决策:
 *      - 若 remaining == 0，找到一个有效组合
 *      - 若 remaining < 0，剪枝（当前路径无效）
 *      关键: 因为可以重复选取，下一层递归从当前位置 i 开始而非 i+1
 *
 *  剪枝优化:
 *      先对数组排序，当 candidates[i] > remaining 时，
 *      后面的元素更大，直接break（剪枝），减少无效搜索。
 *
 *  时间复杂度: O(S)  -- S为所有可行解的长度之和，实际取决于分支因子
 *  空间复杂度: O(target) -- 递归栈深度最多为 target/min(candidates)
 */

class Solution_CombinationSum {
public:
    // 主函数: 返回所有和为target的组合
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;  // 存储所有合法组合
        vector<int> path;            // 当前组合路径

        // 排序以便剪枝优化
        sort(candidates.begin(), candidates.end());

        // 从索引0开始回溯，初始剩余目标值为target
        backtrack(candidates, target, 0, path, result);
        return result;
    }

private:
    // 回溯函数: remaining为剩余需要凑的目标值, start为当前选择的起始位置
    void backtrack(const vector<int>& candidates, int remaining, int start,
                   vector<int>& path, vector<vector<int>>& result) {
        // 结束条件: 剩余目标值恰好为0，说明找到一个合法组合
        if (remaining == 0) {
            result.push_back(path);  // 收集当前路径
            return;
        }

        // 从start位置开始遍历候选数字
        for (int i = start; i < candidates.size(); i++) {
            // ---- 剪枝优化 ----
            // 因为数组已排序，如果当前数字已经大于剩余目标值，
            // 后面的数字更大，不可能满足条件，直接终止循环
            if (candidates[i] > remaining) {
                break;
            }

            // ---- 做选择 ----
            path.push_back(candidates[i]);

            // ---- 递归进入下一层 ----
            // 关键: 传入 i 而非 i+1，因为同一个数字可以重复选取
            // remaining减去当前选择的数字
            backtrack(candidates, remaining - candidates[i], i, path, result);

            // ---- 撤销选择 (回溯) ----
            path.pop_back();
        }
    }
};


/* ======================================================================
 *  第3题: 全排列 (LeetCode 46, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个不含重复数字的数组 nums，返回其所有可能的全排列。
 *
 *  示例:
 *      输入: nums = [1,2,3]
 *      输出: [[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]]
 *
 *  决策树 (以 [1,2,3] 为例):
 *
 *                        []
 *                   /    |    \
 *                 1      2      3
 *                / \    / \    / \
 *               2   3  1   3  1   2
 *               |   |  |   |  |   |
 *               3   2  3   1  2   1
 *
 *  解题思路:
 *      方法一 (visited数组): 使用布尔数组记录哪些数字已被使用，
 *            每层选择一个未使用的数字，直到所有数字都被选完。
 *      方法二 (原地交换): 通过交换当前位置与后续位置的元素，
 *            每层固定一个位置的数字，递归处理剩余位置。
 *
 *  时间复杂度: O(n * n!)  -- 共有n!个排列，每个排列需要O(n)来拷贝
 *  空间复杂度: O(n)        -- 递归栈深度为n
 */

class Solution_Permutation {
public:
    // ========== 方法一: 使用visited数组标记已使用的数字 ==========
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;   // 存储所有排列结果
        vector<int> path;             // 当前排列路径
        vector<bool> used(nums.size(), false);  // 标记每个数字是否已使用

        backtrack(nums, used, path, result);
        return result;
    }

    // ========== 方法二: 原地交换法 (更节省空间) ==========
    vector<vector<int>> permute_swap(vector<int>& nums) {
        vector<vector<int>> result;
        // 从第一个位置开始回溯
        backtrack_swap(nums, 0, result);
        return result;
    }

private:
    // ----- 方法一的回溯函数 -----
    void backtrack(const vector<int>& nums, vector<bool>& used,
                   vector<int>& path, vector<vector<int>>& result) {
        // 结束条件: 路径长度等于数组长度，说明所有数字都已排列完毕
        if (path.size() == nums.size()) {
            result.push_back(path);  // 收集当前排列
            return;
        }

        // 遍历所有数字，选择一个尚未使用的数字
        for (int i = 0; i < nums.size(); i++) {
            // 如果该数字已在当前路径中，跳过
            if (used[i]) {
                continue;
            }

            // ---- 做选择 ----
            path.push_back(nums[i]);  // 将数字加入当前排列
            used[i] = true;           // 标记为已使用

            // ---- 递归进入下一层 ----
            backtrack(nums, used, path, result);

            // ---- 撤销选择 (回溯) ----
            used[i] = false;          // 取消标记
            path.pop_back();          // 从路径中移除
        }
    }

    // ----- 方法二的回溯函数 (原地交换) -----
    void backtrack_swap(vector<int>& nums, int depth,
                        vector<vector<int>>& result) {
        // 结束条件: 当前深度等于数组长度，说明所有位置都已固定
        if (depth == nums.size()) {
            result.push_back(nums);  // 直接收集当前排列 (无需额外拷贝路径)
            return;
        }

        // 从depth位置开始，依次将每个元素交换到depth位置
        for (int i = depth; i < nums.size(); i++) {
            // ---- 做选择: 将nums[i]交换到当前depth位置 ----
            swap(nums[depth], nums[i]);

            // ---- 递归: 固定下一个位置 ----
            backtrack_swap(nums, depth + 1, result);

            // ---- 撤销选择: 交换回来 (回溯) ----
            swap(nums[depth], nums[i]);
        }
    }
};


/* ======================================================================
 *  第4题: 单词搜索 (LeetCode 79, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个 m x n 二维字符网格 board 和一个字符串 word。
 *      如果 word 存在于网格中，返回 true；否则返回 false。
 *      单词必须按照字母顺序，通过相邻的单元格（水平或垂直相邻）
 *      内的字母构成，同一个单元格内的字母不允许被重复使用。
 *
 *  示例:
 *      board = [
 *          ['A','B','C','E'],
 *          ['S','F','C','S'],
 *          ['A','D','E','E']
 *      ]
 *      word = "ABCCED" -> true
 *      word = "SEE"    -> true
 *      word = "ABCB"   -> false (B不能重复使用)
 *
 *  解题思路:
 *      从网格中每个字符出发，尝试向四个方向搜索匹配word的下一个字符。
 *      使用原地标记法（将访问过的字符替换为特殊字符'#'）避免重复访问。
 *      回溯时恢复原字符，确保不影响其他路径的搜索。
 *
 *  时间复杂度: O(m * n * 4^L)  -- m*n个起点，每个方向最多4个分支，深度为L(word长度)
 *  空间复杂度: O(L)            -- 递归栈深度为word的长度
 */

class Solution_WordSearch {
public:
    // 主函数: 判断word是否存在于board中
    bool exist(vector<vector<char>>& board, string word) {
        rows = board.size();       // 网格行数
        cols = board[0].size();    // 网格列数

        // 遍历网格中的每个字符作为起始点
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                // 从(i,j)位置出发，尝试匹配整个word
                if (dfs(board, word, i, j, 0)) {
                    return true;  // 找到匹配，立即返回
                }
            }
        }
        return false;  // 遍历所有起点都没找到
    }

private:
    int rows;  // 网格行数
    int cols;  // 网格列数

    // 四个方向: 上、下、左、右
    // 用方向数组表示相邻格子的偏移量
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // 深度优先搜索: 从(board[r][c])出发，尝试匹配word[k..end]
    bool dfs(vector<vector<char>>& board, const string& word,
             int r, int c, int k) {
        // ---- 边界检查 ----
        // 越界检查、字符不匹配检查
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            return false;  // 越界
        }
        if (board[r][c] != word[k]) {
            return false;  // 当前字符不匹配
        }

        // ---- 结束条件 ----
        // 如果已经匹配完word的所有字符，说明找到了完整单词
        if (k == word.size() - 1) {
            return true;
        }

        // ---- 做选择: 标记当前格子为已访问 ----
        // 将原字符保存，然后替换为'#'表示已访问
        char original = board[r][c];
        board[r][c] = '#';  // 标记为已访问，防止重复使用

        // ---- 递归: 向四个方向搜索下一个字符 ----
        for (auto& dir : directions) {
            int nr = r + dir[0];  // 新行坐标
            int nc = c + dir[1];  // 新列坐标
            // 从相邻格子出发，匹配word的下一个字符 (k+1)
            if (dfs(board, word, nr, nc, k + 1)) {
                return true;  // 找到匹配，向上层返回true
            }
        }

        // ---- 撤销选择: 恢复当前格子的原始字符 (回溯) ----
        board[r][c] = original;

        return false;  // 四个方向都没有找到匹配
    }
};


/* ======================================================================
 *  第5题: N皇后 (LeetCode 51, Hard)
 * ======================================================================
 *
 *  题目描述:
 *      在 n x n 的棋盘上放置 n 个皇后，使得它们互不攻击。
 *      皇后可以攻击同一行、同一列、同一对角线上的任意棋子。
 *      返回所有合法的放置方案。
 *
 *  示例 (N=4):
 *      解1:  . Q . .      解2:  . . Q .
 *            . . . Q            Q . . .
 *            Q . . .            . . . Q
 *            . . Q .            . Q . .
 *
 *  解题思路:
 *      逐行放置皇后。对于每一行，尝试每一列:
 *      - 如果该列、主对角线、副对角线上都没有其他皇后，则可以放置
 *      - 放置后递归处理下一行
 *      - 回溯时移除皇后，尝试下一列
 *
 *      对角线编号技巧:
 *      - 主对角线 (左上到右下): 同一对角线上 r - c 相同
 *      - 副对角线 (右上到左下): 同一对角线上 r + c 相同
 *
 *      图示 (N=4, 解1):
 *        列:  0   1   2   3
 *       行0 [ . | Q | . | . ]   皇后位置: (0,1)
 *       行1 [ . | . | . | Q ]   皇后位置: (1,3)
 *       行2 [ Q | . | . | . ]   皇后位置: (2,0)
 *       行3 [ . | . | Q | . ]   皇后位置: (3,2)
 *
 *  时间复杂度: O(n!)  -- 第一行n种选择，第二行最多n-1种，以此类推
 *  空间复杂度: O(n)   -- 需要三个集合来记录列和对角线的占用情况
 */

class Solution_NQueens {
public:
    // 主函数: 返回所有N皇后的解法
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;  // 存储所有解法
        vector<string> board(n, string(n, '.'));  // 初始化n x n空棋盘

        // 三个集合分别记录:
        // cols:    已占用的列号
        // diag1:   已占用的主对角线编号 (r - c)
        // diag2:   已占用的副对角线编号 (r + c)
        set<int> cols;
        set<int> diag1;
        set<int> diag2;

        // 从第0行开始放置皇后
        backtrack(n, 0, board, cols, diag1, diag2, result);
        return result;
    }

private:
    // 回溯函数: row为当前处理的行号
    void backtrack(int n, int row, vector<string>& board,
                   set<int>& cols, set<int>& diag1, set<int>& diag2,
                   vector<vector<string>>& result) {
        // ---- 结束条件: 所有行都已成功放置皇后 ----
        if (row == n) {
            result.push_back(board);  // 收集当前棋盘布局
            return;
        }

        // 尝试在当前行的每一列放置皇后
        for (int col = 0; col < n; col++) {
            // 计算当前格子所在的对角线编号
            int d1 = row - col;  // 主对角线编号 (左上到右下)
            int d2 = row + col;  // 副对角线编号 (右上到左下)

            // ---- 检查冲突: 列、主对角线、副对角线是否已被占用 ----
            if (cols.count(col) || diag1.count(d1) || diag2.count(d2)) {
                continue;  // 存在冲突，跳过此列
            }

            // ---- 做选择: 在(row, col)放置皇后 ----
            board[row][col] = 'Q';     // 放置皇后
            cols.insert(col);           // 标记列已占用
            diag1.insert(d1);           // 标记主对角线已占用
            diag2.insert(d2);           // 标记副对角线已占用

            // ---- 递归: 处理下一行 ----
            backtrack(n, row + 1, board, cols, diag1, diag2, result);

            // ---- 撤销选择: 移除皇后 (回溯) ----
            board[row][col] = '.';     // 移除皇后
            cols.erase(col);            // 取消列占用标记
            diag1.erase(d1);            // 取消主对角线占用标记
            diag2.erase(d2);            // 取消副对角线占用标记
        }
    }
};


/* ======================================================================
 *  第6题: 括号生成 (LeetCode 22, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      数字 n 代表生成括号的对数，请你设计一个函数，
 *      用于能够生成所有可能的并且有效的括号组合。
 *
 *  示例:
 *      输入: n = 3
 *      输出: ["((()))", "(()())", "(())()", "()(())", "()()()"]
 *
 *  决策树 (以 n=2 为例):
 *                        ""
 *                       /
 *                      "("
 *                    /     \
 *                  "(("     "()"
 *                  /          \
 *                "(()"        "()()"
 *                /
 *              "(())"
 *
 *  解题思路:
 *      使用回溯法，维护两个计数器:
 *      - open:  已使用的左括号数量
 *      - close: 已使用的右括号数量
 *      规则:
 *      - 当 open < n 时，可以添加左括号 '('
 *      - 当 close < open 时，可以添加右括号 ')'
 *      - 当路径长度达到 2*n 时，收集结果
 *
 *  剪枝条件:
 *      - 左括号数量不能超过n
 *      - 右括号数量不能超过左括号数量 (保证有效性)
 *
 *  时间复杂度: O(4^n / sqrt(n))  -- 卡特兰数
 *  空间复杂度: O(n)               -- 递归栈深度
 */

class Solution_GenerateParenthesis {
public:
    // 主函数: 返回所有有效的n对括号组合
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        string path;
        backtrack(n, 0, 0, path, result);
        return result;
    }

private:
    // 回溯函数: open为已用左括号数, close为已用右括号数
    void backtrack(int n, int open, int close,
                   string& path, vector<string>& result) {
        // ---- 结束条件: 路径长度达到2*n ----
        if (path.size() == 2 * n) {
            result.push_back(path);
            return;
        }

        // ---- 选择1: 添加左括号 ----
        // 条件: 左括号数量未达到n
        if (open < n) {
            path.push_back('(');              // 做选择
            backtrack(n, open + 1, close, path, result);  // 递归
            path.pop_back();                  // 撤销选择
        }

        // ---- 选择2: 添加右括号 ----
        // 条件: 右括号数量小于左括号数量 (保证括号匹配)
        if (close < open) {
            path.push_back(')');              // 做选择
            backtrack(n, open, close + 1, path, result);  // 递归
            path.pop_back();                  // 撤销选择
        }
    }
};


/* ======================================================================
 *  第7题: 电话号码的字母组合 (LeetCode 17, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。
 *      答案可以按任意顺序返回。
 *      映射关系如下 (与电话按键相同):
 *          2 -> abc, 3 -> def, 4 -> ghi, 5 -> jkl
 *          6 -> mno, 7 -> pqrs, 8 -> tuv, 9 -> wxyz
 *
 *  示例:
 *      输入: digits = "23"
 *      输出: ["ad","ae","af","bd","be","bf","cd","ce","cf"]
 *
 *  决策树 (以 "23" 为例):
 *                        ""
 *                 /      |      \
 *               "a"     "b"     "c"        <- digit '2': abc
 *              /|\     /|\     /|\
 *            ad ae af bd be bf cd ce cf     <- digit '3': def
 *
 *  解题思路:
 *      建立数字到字母的映射表。
 *      回溯时，对于当前数字，遍历其对应的所有字母，
 *      选择一个字母后递归处理下一个数字。
 *      当处理完所有数字时，收集当前路径。
 *
 *  时间复杂度: O(3^m * 4^n)  -- m个对应3个字母的数字，n个对应4个字母的数字
 *  空间复杂度: O(m + n)       -- 递归栈深度为数字个数
 */

class Solution_LetterCombinations {
public:
    // 主函数: 返回所有字母组合
    vector<string> letterCombinations(string digits) {
        vector<string> result;
        if (digits.empty()) return result;

        // 数字到字母的映射表
        string mapping[10] = {
            "",     // 0
            "",     // 1
            "abc",  // 2
            "def",  // 3
            "ghi",  // 4
            "jkl",  // 5
            "mno",  // 6
            "pqrs", // 7
            "tuv",  // 8
            "wxyz"  // 9
        };

        string path;
        backtrack(digits, 0, mapping, path, result);
        return result;
    }

private:
    // 回溯函数: index为当前处理的数字索引
    void backtrack(const string& digits, int index,
                   string mapping[], string& path, vector<string>& result) {
        // ---- 结束条件: 所有数字都已处理 ----
        if (index == digits.size()) {
            result.push_back(path);
            return;
        }

        // 获取当前数字对应的字母集合
        int digit = digits[index] - '0';
        const string& letters = mapping[digit];

        // 遍历当前数字对应的所有字母
        for (char ch : letters) {
            // ---- 做选择 ----
            path.push_back(ch);

            // ---- 递归: 处理下一个数字 ----
            backtrack(digits, index + 1, mapping, path, result);

            // ---- 撤销选择 ----
            path.pop_back();
        }
    }
};


/* ======================================================================
 *  第8题: 分割回文串 (LeetCode 131, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给你一个字符串 s，请你将 s 分割成一些子串，使每个子串都是回文串。
 *      返回 s 所有可能的分割方案。
 *
 *  示例:
 *      输入: s = "aab"
 *      输出: [["a","a","b"], ["aa","b"]]
 *
 *  决策树 (以 "aab" 为例):
 *                        ""
 *                 /       |       \
 *              "a"       "aa"    "aab"(非回文,剪枝)
 *             /   \        |
 *          "a"   "ab"(x)  "aa","b"
 *          |
 *        "a","a","b"  ✓
 *
 *  解题思路:
 *      回溯枚举所有可能的分割位置。
 *      对于当前位置start，尝试所有可能的结束位置end:
 *      - 如果 s[start..end] 是回文串，则将其加入路径，递归处理end+1
 *      - 如果不是回文串，跳过 (剪枝)
 *      当start到达字符串末尾时，收集当前分割方案。
 *
 *  时间复杂度: O(n * 2^n)  -- 每个位置有选/不选两种可能，回文检查O(n)
 *  空间复杂度: O(n)         -- 递归栈深度为n
 */

class Solution_Partition {
public:
    // 主函数: 返回所有回文分割方案
    vector<vector<string>> partition(string s) {
        vector<vector<string>> result;
        vector<string> path;
        backtrack(s, 0, path, result);
        return result;
    }

private:
    // 判断字符串是否为回文串
    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }

    // 回溯函数: start为当前分割的起始位置
    void backtrack(const string& s, int start,
                   vector<string>& path, vector<vector<string>>& result) {
        // ---- 结束条件: 已经处理完所有字符 ----
        if (start == s.size()) {
            result.push_back(path);
            return;
        }

        // 尝试所有可能的分割结束位置
        for (int end = start; end < s.size(); end++) {
            // ---- 剪枝: 只处理回文子串 ----
            if (!isPalindrome(s, start, end)) {
                continue;  // 非回文串，跳过
            }

            // ---- 做选择: 将s[start..end]加入路径 ----
            path.push_back(s.substr(start, end - start + 1));

            // ---- 递归: 从end+1位置继续分割 ----
            backtrack(s, end + 1, path, result);

            // ---- 撤销选择 ----
            path.pop_back();
        }
    }
};


/* ======================================================================
 *  第9题: 复原IP地址 (LeetCode 93, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个只包含数字的字符串 s，用以表示一个 IP 地址，
 *      返回所有可能的有效 IP 地址。
 *      有效的 IP 地址正好由四个整数组成，每个整数位于 0 到 255 之间，
 *      且不能含有前导零。
 *
 *  示例:
 *      输入: s = "25525511135"
 *      输出: ["255.255.11.135", "255.255.111.35"]
 *
 *      输入: s = "0000"
 *      输出: ["0.0.0.0"]
 *
 *      输入: s = "101023"
 *      输出: ["1.0.10.23","1.0.102.3","10.1.0.23","10.10.2.3","101.0.2.3"]
 *
 *  解题思路:
 *      回溯法将字符串分为4段，每段是一个0-255的整数。
 *      对于当前段，尝试取1位、2位或3位数字:
 *      - 如果段长度>1且以'0'开头，无效 (前导零)
 *      - 如果段值>255，无效
 *      当成功分出4段且用完所有字符时，收集结果。
 *
 *  剪枝条件:
 *      - 每段最多3位数字
 *      - 段值不能超过255
 *      - 不能有前导零 (长度>1时)
 *      - 剩余字符数不能超过剩余段数*3
 *
 *  时间复杂度: O(1)  -- 最多3^4=81种组合
 *  空间复杂度: O(1)  -- 递归栈深度最多为4
 */

class Solution_RestoreIPAddresses {
public:
    // 主函数: 返回所有有效IP地址
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        string path;
        backtrack(s, 0, 0, path, result);
        return result;
    }

private:
    // 回溯函数: index为当前字符位置, segmentCount为已分段数
    void backtrack(const string& s, int index, int segmentCount,
                   string& path, vector<string>& result) {
        // ---- 结束条件: 已分出4段 ----
        if (segmentCount == 4) {
            // 如果恰好用完所有字符，则是一个有效IP
            if (index == s.size()) {
                result.push_back(path);
            }
            return;
        }

        // ---- 剪枝: 剩余字符不足以分完剩余段 ----
        int remaining = s.size() - index;
        int segmentsLeft = 4 - segmentCount;
        if (remaining < segmentsLeft || remaining > segmentsLeft * 3) {
            return;
        }

        // 尝试取1位、2位、3位数字作为当前段
        int num = 0;
        for (int i = 0; i < 3 && index + i < s.size(); i++) {
            num = num * 10 + (s[index + i] - '0');

            // ---- 剪枝: 前导零检查 ----
            if (i > 0 && s[index] == '0') {
                break;  // 有前导零，不能再取更多位
            }

            // ---- 剪枝: 段值不能超过255 ----
            if (num > 255) {
                break;
            }

            // ---- 做选择: 将当前段加入路径 ----
            int oldSize = path.size();
            if (segmentCount > 0) {
                path.push_back('.');  // 添加分隔符
            }
            path.append(s.substr(index, i + 1));

            // ---- 递归: 处理下一段 ----
            backtrack(s, index + i + 1, segmentCount + 1, path, result);

            // ---- 撤销选择: 恢复路径 ----
            path.resize(oldSize);
        }
    }
};


/* ======================================================================
 *  第10题: 火柴拼正方形 (LeetCode 473, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      你将得到一个整数数组 matchsticks，其中 matchsticks[i] 是第 i 个火柴棒的长度。
 *      你要用所有的火柴棍拼成一个正方形。
 *      你不能折断火柴棍，但可以把它们连在一起，每根火柴棍必须且只能使用一次。
 *      如果你能拼成正方形，返回 true；否则返回 false。
 *
 *  示例:
 *      输入: matchsticks = [1,1,2,2,2]
 *      输出: true
 *      解释: 可以拼成一个边长为2的正方形 (两个边各用1+1, 另两个边各用2)
 *
 *      输入: matchsticks = [3,3,3,3,4]
 *      输出: false
 *      解释: 无法用这些火柴拼成正方形
 *
 *  解题思路:
 *      正方形有4条等长的边，总长度必须能被4整除。
 *      将问题转化为: 将火柴分为4组，每组和等于边长target。
 *      使用回溯法，维护4个桶(边)，每根火柴尝试放入某个桶中。
 *
 *  优化策略:
 *      1. 降序排序: 先处理较长的火柴，减少搜索空间
 *      2. 剪枝: 如果当前火柴放入某桶后超过target，跳过
 *      3. 去重: 相同长度的火柴放入相同值的桶时，只尝试一次
 *      4. 提前返回: 一旦找到一种方案立即返回true
 *
 *  时间复杂度: O(4^n)  -- 每根火柴最多4个选择，实际因剪枝远小于此
 *  空间复杂度: O(n)    -- 递归栈深度为火柴数量
 */

class Solution_Makesquare {
public:
    // 主函数: 判断是否能拼成正方形
    bool makesquare(vector<int>& matchsticks) {
        int n = matchsticks.size();
        if (n < 4) return false;

        // 计算总长度，必须能被4整除
        int total = 0;
        for (int s : matchsticks) total += s;
        if (total % 4 != 0) return false;

        int target = total / 4;

        // ---- 优化: 降序排序，先处理长火柴 ----
        sort(matchsticks.begin(), matchsticks.end(), greater<int>());

        // 如果最长的火柴超过target，不可能拼成
        if (matchsticks[0] > target) return false;

        // 4条边的当前长度
        vector<int> sides(4, 0);

        return backtrack(matchsticks, 0, target, sides);
    }

private:
    // 回溯函数: index为当前火柴索引, target为每边目标长度
    bool backtrack(const vector<int>& matchsticks, int index,
                   int target, vector<int>& sides) {
        // ---- 结束条件: 所有火柴都已分配 ----
        if (index == matchsticks.size()) {
            // 检查4条边是否都等于target
            return sides[0] == target && sides[1] == target &&
                   sides[2] == target && sides[3] == target;
        }

        int stick = matchsticks[index];

        // 尝试将当前火柴放入4条边中的某一条
        for (int i = 0; i < 4; i++) {
            // ---- 剪枝1: 放入后不能超过target ----
            if (sides[i] + stick > target) {
                continue;
            }

            // ---- 剪枝2: 去重 ----
            // 如果当前边和前一条边长度相同，说明之前已经尝试过相同的组合
            bool skip = false;
            for (int j = 0; j < i; j++) {
                if (sides[j] == sides[i]) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue;

            // ---- 做选择 ----
            sides[i] += stick;

            // ---- 递归 ----
            if (backtrack(matchsticks, index + 1, target, sides)) {
                return true;  // 找到方案，提前返回
            }

            // ---- 撤销选择 ----
            sides[i] -= stick;
        }

        return false;  // 所有边都尝试过，无法放下当前火柴
    }
};


/* ======================================================================
 *  第11题: 划分为k个相等的子集 (LeetCode 698, Medium)
 * ======================================================================
 *
 *  题目描述:
 *      给定一个整数数组 nums 和一个正整数 k，
 *      判断是否可以将这个数组分成 k 个非空子集，使得每个子集的元素和相等。
 *
 *  示例:
 *      输入: nums = [4,3,2,3,5,2,1], k = 4
 *      输出: true
 *      解释: 可以分成4个子集 (5), (4,1), (3,2), (3,2)，每个子集和为5
 *
 *      输入: nums = [1,2,3,4], k = 3
 *      输出: false
 *
 *  解题思路:
 *      与火柴拼正方形类似，将问题转化为k个桶。
 *      每个桶的目标和为 total / k。
 *      使用回溯法，每个数字尝试放入k个桶中的某一个。
 *
 *  优化策略:
 *      1. 降序排序: 先处理较大的数字，减少分支
 *      2. 剪枝: 放入后超过目标和则跳过
 *      3. 去重: 相同值的桶只尝试一次
 *      4. 空桶优化: 如果放入第一个空桶失败，后面所有空桶都会失败
 *
 *  时间复杂度: O(k^n)  -- 每个数字最多k个选择
 *  空间复杂度: O(n)    -- 递归栈深度
 */

class Solution_PartitionToKEqualSumSubsets {
public:
    // 主函数: 判断是否可以分成k个和相等的子集
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int n = nums.size();
        if (n < k) return false;

        // 计算总和，必须能被k整除
        int total = 0;
        for (int num : nums) total += num;
        if (total % k != 0) return false;

        int target = total / k;

        // ---- 优化: 降序排序 ----
        sort(nums.begin(), nums.end(), greater<int>());

        // 如果最大元素超过target，不可能分组
        if (nums[0] > target) return false;

        // k个桶的当前和
        vector<int> buckets(k, 0);

        return backtrack(nums, 0, target, buckets, k);
    }

private:
    // 回溯函数: index为当前数字索引
    bool backtrack(const vector<int>& nums, int index,
                   int target, vector<int>& buckets, int k) {
        // ---- 结束条件: 所有数字都已分配 ----
        if (index == nums.size()) {
            return true;  // 能走到这里说明所有桶都恰好等于target
        }

        int num = nums[index];

        // 尝试将当前数字放入k个桶中的某一个
        for (int i = 0; i < k; i++) {
            // ---- 剪枝1: 放入后不能超过target ----
            if (buckets[i] + num > target) {
                continue;
            }

            // ---- 剪枝2: 去重 ----
            // 如果当前桶和前面某个桶的值相同，跳过 (避免重复计算)
            bool skip = false;
            for (int j = 0; j < i; j++) {
                if (buckets[j] == buckets[i]) {
                    skip = true;
                    break;
                }
            }
            if (skip) continue;

            // ---- 做选择 ----
            buckets[i] += num;

            // ---- 递归 ----
            if (backtrack(nums, index + 1, target, buckets, k)) {
                return true;
            }

            // ---- 撤销选择 ----
            buckets[i] -= num;

            // ---- 优化: 如果放入第一个空桶失败，则后面所有空桶都会失败 ----
            if (buckets[i] == 0) {
                break;
            }
        }

        return false;
    }
};


/* ======================================================================
 *  第12题: 解数独 (LeetCode 37, Hard)
 * ======================================================================
 *
 *  题目描述:
 *      编写一个程序，通过已填充的数字来解决数独问题。
 *      数独的解法需遵循如下规则:
 *      - 数字 1-9 在每一行只能出现一次
 *      - 数字 1-9 在每一列只能出现一次
 *      - 数字 1-9 在每一个以粗实线分隔的 3x3 宫内只能出现一次
 *      空白格用 '.' 表示。
 *
 *  示例:
 *      输入:
 *      [["5","3",".",".","7",".",".",".","."],
 *       ["6",".",".","1","9","5",".",".","."],
 *       [".","9","8",".",".",".",".","6","."],
 *       ["8",".",".",".","6",".",".",".","3"],
 *       ["4",".",".","8",".","3",".",".","1"],
 *       ["7",".",".",".","2",".",".",".","6"],
 *       [".","6",".",".",".",".","2","8","."],
 *       [".",".",".","4","1","9",".",".","5"],
 *       [".",".",".",".","8",".",".","7","9"]]
 *
 *  解题思路:
 *      回溯法遍历每个空格，尝试填入数字1-9。
 *      对于每个空格:
 *      - 尝试填入1-9中的某个数字
 *      - 检查该数字在当前行、列、3x3宫中是否合法
 *      - 如果合法，填入并递归处理下一个空格
 *      - 如果不合法或后续递归失败，回溯 (恢复为'.')
 *
 *      优化: 使用三个布尔数组快速判断数字是否可用
 *      - row[i][d]: 第i行是否已使用数字d
 *      - col[j][d]: 第j列是否已使用数字d
 *      - box[b][d]: 第b个3x3宫是否已使用数字d
 *
 *  时间复杂度: O(9^m)  -- m为空格数量，每个空格最多9种选择
 *  空间复杂度: O(m)    -- 递归栈深度为空格数量
 */

class Solution_SolveSudoku {
public:
    // 主函数: 解数独 (原地修改board)
    void solveSudoku(vector<vector<char>>& board) {
        // 初始化行、列、宫的标记数组
        // row[i][d] = true 表示第i行已经使用了数字d+1
        vector<vector<bool>> row(9, vector<bool>(9, false));
        vector<vector<bool>> col(9, vector<bool>(9, false));
        vector<vector<bool>> box(9, vector<bool>(9, false));

        // 记录所有空格的位置
        vector<pair<int, int>> emptyCells;

        // 第一遍扫描: 初始化标记数组，收集空格位置
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == '.') {
                    emptyCells.push_back({i, j});
                } else {
                    int d = board[i][j] - '1';  // 数字转为0-8的索引
                    row[i][d] = true;
                    col[j][d] = true;
                    box[(i / 3) * 3 + j / 3][d] = true;
                }
            }
        }

        // 开始回溯求解
        backtrack(board, emptyCells, 0, row, col, box);
    }

private:
    // 回溯函数: index为当前处理的空格索引
    bool backtrack(vector<vector<char>>& board,
                   const vector<pair<int, int>>& emptyCells, int index,
                   vector<vector<bool>>& row,
                   vector<vector<bool>>& col,
                   vector<vector<bool>>& box) {
        // ---- 结束条件: 所有空格都已填完 ----
        if (index == emptyCells.size()) {
            return true;
        }

        int r = emptyCells[index].first;   // 当前空格的行
        int c = emptyCells[index].second;  // 当前空格的列
        int b = (r / 3) * 3 + c / 3;      // 当前空格所属的3x3宫编号

        // 尝试填入数字1-9
        for (int d = 0; d < 9; d++) {
            // ---- 检查合法性: 行、列、宫中都没有使用过数字d+1 ----
            if (row[r][d] || col[c][d] || box[b][d]) {
                continue;  // 冲突，尝试下一个数字
            }

            // ---- 做选择: 填入数字 ----
            board[r][c] = '1' + d;
            row[r][d] = true;
            col[c][d] = true;
            box[b][d] = true;

            // ---- 递归: 处理下一个空格 ----
            if (backtrack(board, emptyCells, index + 1, row, col, box)) {
                return true;  // 找到解，提前返回
            }

            // ---- 撤销选择: 恢复为空格 ----
            board[r][c] = '.';
            row[r][d] = false;
            col[c][d] = false;
            box[b][d] = false;
        }

        return false;  // 1-9都无法填入，说明前面的选择有误
    }
};


/* ======================================================================
 *  辅助函数: 打印工具
 * ====================================================================== */

// 打印一维整数向量
void printVector(const vector<int>& vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++) {
        cout << vec[i];
        if (i < vec.size() - 1) cout << ", ";
    }
    cout << "]";
}

// 打印二维整数向量
void printVector2D(const vector<vector<int>>& vec2d) {
    cout << "[";
    for (int i = 0; i < vec2d.size(); i++) {
        printVector(vec2d[i]);
        if (i < vec2d.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// 打印棋盘 (用于N皇后)
void printBoard(const vector<string>& board) {
    for (const auto& row : board) {
        cout << "  ";
        for (char c : row) {
            cout << c << " ";
        }
        cout << endl;
    }
}


/* ======================================================================
 *  主函数: 测试所有题目
 * ====================================================================== */

int main() {
    cout << "============================================================" << endl;
    cout << "        LeetCode 经典回溯算法题集 - 测试" << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // ----------------------------------------------------------
    // 第1题测试: 子集
    // ----------------------------------------------------------
    cout << "---------- 第1题: 子集 (LeetCode 78) ----------" << endl;
    {
        Solution_Subsets sol;
        vector<int> nums = {1, 2, 3};
        cout << "输入: nums = [1, 2, 3]" << endl;
        auto result = sol.subsets(nums);
        cout << "输出: ";
        printVector2D(result);
        cout << "期望: [[], [1], [1,2], [1,2,3], [1,3], [2], [2,3], [3]]" << endl;
        cout << "子集个数: " << result.size() << " (期望: 8 = 2^3)" << endl;
        cout << "复杂度: 时间 O(n*2^n), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第2题测试: 组合总和
    // ----------------------------------------------------------
    cout << "---------- 第2题: 组合总和 (LeetCode 39) ----------" << endl;
    {
        Solution_CombinationSum sol;
        vector<int> candidates = {2, 3, 6, 7};
        int target = 7;
        cout << "输入: candidates = [2,3,6,7], target = 7" << endl;
        auto result = sol.combinationSum(candidates, target);
        cout << "输出: ";
        printVector2D(result);
        cout << "期望: [[2,2,3], [7]]" << endl;
        cout << "复杂度: 时间 O(S), 空间 O(target/min)" << endl;
    }
    {
        Solution_CombinationSum sol;
        vector<int> candidates = {2, 3, 5};
        int target = 8;
        cout << "输入: candidates = [2,3,5], target = 8" << endl;
        auto result = sol.combinationSum(candidates, target);
        cout << "输出: ";
        printVector2D(result);
        cout << "期望: [[2,2,2,2], [2,3,3], [3,5]]" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第3题测试: 全排列
    // ----------------------------------------------------------
    cout << "---------- 第3题: 全排列 (LeetCode 46) ----------" << endl;
    {
        Solution_Permutation sol;
        vector<int> nums = {1, 2, 3};
        cout << "输入: nums = [1, 2, 3]" << endl;

        cout << "方法一 (visited数组): ";
        auto result1 = sol.permute(nums);
        printVector2D(result1);
        cout << "排列个数: " << result1.size() << " (期望: 6 = 3!)" << endl;

        cout << "方法二 (原地交换):   ";
        auto result2 = sol.permute_swap(nums);
        printVector2D(result2);
        cout << "排列个数: " << result2.size() << " (期望: 6 = 3!)" << endl;
        cout << "复杂度: 时间 O(n*n!), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第4题测试: 单词搜索
    // ----------------------------------------------------------
    cout << "---------- 第4题: 单词搜索 (LeetCode 79) ----------" << endl;
    {
        Solution_WordSearch sol;
        vector<vector<char>> board = {
            {'A', 'B', 'C', 'E'},
            {'S', 'F', 'C', 'S'},
            {'A', 'D', 'E', 'E'}
        };
        cout << "Board:" << endl;
        cout << "  A B C E" << endl;
        cout << "  S F C S" << endl;
        cout << "  A D E E" << endl;

        vector<pair<string, bool>> tests = {
            {"ABCCED", true},   // 路径: A(0,0)->B(0,1)->C(0,2)->C(1,2)->E(2,2)->D(2,1)
            {"SEE", true},      // 路径: S(1,3)->E(2,3)->E(2,2)
            {"ABCB", false}     // B(0,1)不能重复使用
        };
        for (auto& [word, expected] : tests) {
            bool result = sol.exist(board, word);
            cout << "word=\"" << word << "\" -> "
                 << (result ? "true" : "false")
                 << " (期望: " << (expected ? "true" : "false") << ")"
                 << (result == expected ? " [通过]" : " [失败]")
                 << endl;
        }
        cout << "复杂度: 时间 O(m*n*4^L), 空间 O(L)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第5题测试: N皇后
    // ----------------------------------------------------------
    cout << "---------- 第5题: N皇后 (LeetCode 51) ----------" << endl;
    {
        Solution_NQueens sol;
        int n = 4;
        cout << "输入: n = " << n << endl;
        auto result = sol.solveNQueens(n);
        cout << "解法个数: " << result.size() << " (期望: 2)" << endl;
        for (int i = 0; i < result.size(); i++) {
            cout << "解法 " << (i + 1) << ":" << endl;
            printBoard(result[i]);
            if (i < result.size() - 1) cout << endl;
        }
        cout << "复杂度: 时间 O(n!), 空间 O(n)" << endl;
    }
    cout << endl;

    // 额外测试: N=8 (经典八皇后问题)
    {
        Solution_NQueens sol;
        int n = 8;
        cout << "扩展测试: n = " << n << " (八皇后问题)" << endl;
        auto result = sol.solveNQueens(n);
        cout << "解法个数: " << result.size() << " (期望: 92)" << endl;
    }

    cout << endl;

    // ----------------------------------------------------------
    // 第6题测试: 括号生成
    // ----------------------------------------------------------
    cout << "---------- 第6题: 括号生成 (LeetCode 22) ----------" << endl;
    {
        Solution_GenerateParenthesis sol;
        vector<pair<int, vector<string>>> tests = {
            {1, {"()"}},
            {2, {"(())", "()()"}},
            {3, {"((()))", "(()())", "(())()", "()(())", "()()()"}}
        };
        for (auto& [n, expected] : tests) {
            auto result = sol.generateParenthesis(n);
            cout << "n=" << n << " -> [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            cout << "  个数: " << result.size() << " (期望: " << expected.size() << ")"
                 << endl;
        }
        cout << "复杂度: 时间 O(4^n/sqrt(n)), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第7题测试: 电话号码的字母组合
    // ----------------------------------------------------------
    cout << "---------- 第7题: 电话号码的字母组合 (LeetCode 17) ----------" << endl;
    {
        Solution_LetterCombinations sol;
        // 测试用例1
        {
            string digits = "23";
            auto result = sol.letterCombinations(digits);
            cout << "输入: digits = \"" << digits << "\"" << endl;
            cout << "输出: [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            cout << "  个数: " << result.size() << " (期望: 9)" << endl;
        }
        // 测试用例2: 空输入
        {
            string digits = "";
            auto result = sol.letterCombinations(digits);
            cout << "输入: digits = \"\" -> 输出: [] (期望: [])"
                 << (result.empty() ? " [通过]" : " [失败]") << endl;
        }
        // 测试用例3: 单个数字
        {
            string digits = "7";
            auto result = sol.letterCombinations(digits);
            cout << "输入: digits = \"" << digits << "\" -> [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "] (期望: [\"p\",\"q\",\"r\",\"s\"])" << endl;
        }
        cout << "复杂度: 时间 O(3^m * 4^n), 空间 O(m+n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第8题测试: 分割回文串
    // ----------------------------------------------------------
    cout << "---------- 第8题: 分割回文串 (LeetCode 131) ----------" << endl;
    {
        Solution_Partition sol;
        // 测试用例1
        {
            string s = "aab";
            auto result = sol.partition(s);
            cout << "输入: s = \"" << s << "\"" << endl;
            cout << "输出: [";
            for (int i = 0; i < result.size(); i++) {
                cout << "[";
                for (int j = 0; j < result[i].size(); j++) {
                    cout << "\"" << result[i][j] << "\"";
                    if (j < result[i].size() - 1) cout << ",";
                }
                cout << "]";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            cout << "  方案数: " << result.size() << " (期望: 2)" << endl;
        }
        // 测试用例2
        {
            string s = "a";
            auto result = sol.partition(s);
            cout << "输入: s = \"" << s << "\" -> 方案数: " << result.size()
                 << " (期望: 1)" << endl;
        }
        // 测试用例3
        {
            string s = "efe";
            auto result = sol.partition(s);
            cout << "输入: s = \"" << s << "\" -> 方案数: " << result.size()
                 << " (期望: 3)" << endl;
        }
        cout << "复杂度: 时间 O(n*2^n), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第9题测试: 复原IP地址
    // ----------------------------------------------------------
    cout << "---------- 第9题: 复原IP地址 (LeetCode 93) ----------" << endl;
    {
        Solution_RestoreIPAddresses sol;
        // 测试用例1
        {
            string s = "25525511135";
            auto result = sol.restoreIpAddresses(s);
            cout << "输入: s = \"" << s << "\"" << endl;
            cout << "输出: [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            cout << "  期望: [\"255.255.11.135\", \"255.255.111.35\"]" << endl;
        }
        // 测试用例2
        {
            string s = "0000";
            auto result = sol.restoreIpAddresses(s);
            cout << "输入: s = \"" << s << "\" -> [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "] (期望: [\"0.0.0.0\"])" << endl;
        }
        // 测试用例3
        {
            string s = "101023";
            auto result = sol.restoreIpAddresses(s);
            cout << "输入: s = \"" << s << "\" -> [";
            for (int i = 0; i < result.size(); i++) {
                cout << "\"" << result[i] << "\"";
                if (i < result.size() - 1) cout << ", ";
            }
            cout << "]" << endl;
            cout << "  期望: [\"1.0.10.23\",\"1.0.102.3\",\"10.1.0.23\",\"10.10.2.3\",\"101.0.2.3\"]" << endl;
        }
        cout << "复杂度: 时间 O(1), 空间 O(1)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第10题测试: 火柴拼正方形
    // ----------------------------------------------------------
    cout << "---------- 第10题: 火柴拼正方形 (LeetCode 473) ----------" << endl;
    {
        Solution_Makesquare sol;
        // 测试用例1
        {
            vector<int> matchsticks = {1, 1, 2, 2, 2};
            bool result = sol.makesquare(matchsticks);
            cout << "输入: [1,1,2,2,2] -> " << (result ? "true" : "false")
                 << " (期望: true)" << (result ? " [通过]" : " [失败]") << endl;
        }
        // 测试用例2
        {
            vector<int> matchsticks = {3, 3, 3, 3, 4};
            bool result = sol.makesquare(matchsticks);
            cout << "输入: [3,3,3,3,4] -> " << (result ? "true" : "false")
                 << " (期望: false)" << (!result ? " [通过]" : " [失败]") << endl;
        }
        // 测试用例3
        {
            vector<int> matchsticks = {5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3};
            bool result = sol.makesquare(matchsticks);
            cout << "输入: [5,5,5,5,4,4,4,4,3,3,3,3] -> " << (result ? "true" : "false")
                 << " (期望: true)" << (result ? " [通过]" : " [失败]") << endl;
        }
        cout << "复杂度: 时间 O(4^n), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第11题测试: 划分为k个相等的子集
    // ----------------------------------------------------------
    cout << "---------- 第11题: 划分为k个相等的子集 (LeetCode 698) ----------" << endl;
    {
        Solution_PartitionToKEqualSumSubsets sol;
        // 测试用例1
        {
            vector<int> nums = {4, 3, 2, 3, 5, 2, 1};
            int k = 4;
            bool result = sol.canPartitionKSubsets(nums, k);
            cout << "输入: nums = [4,3,2,3,5,2,1], k = " << k << " -> "
                 << (result ? "true" : "false")
                 << " (期望: true)" << (result ? " [通过]" : " [失败]") << endl;
        }
        // 测试用例2
        {
            vector<int> nums = {1, 2, 3, 4};
            int k = 3;
            bool result = sol.canPartitionKSubsets(nums, k);
            cout << "输入: nums = [1,2,3,4], k = " << k << " -> "
                 << (result ? "true" : "false")
                 << " (期望: false)" << (!result ? " [通过]" : " [失败]") << endl;
        }
        // 测试用例3
        {
            vector<int> nums = {2, 2, 2, 2, 3, 4, 5};
            int k = 4;
            bool result = sol.canPartitionKSubsets(nums, k);
            cout << "输入: nums = [2,2,2,2,3,4,5], k = " << k << " -> "
                 << (result ? "true" : "false")
                 << " (期望: false)" << (!result ? " [通过]" : " [失败]") << endl;
        }
        cout << "复杂度: 时间 O(k^n), 空间 O(n)" << endl;
    }
    cout << endl;

    // ----------------------------------------------------------
    // 第12题测试: 解数独
    // ----------------------------------------------------------
    cout << "---------- 第12题: 解数独 (LeetCode 37) ----------" << endl;
    {
        Solution_SolveSudoku sol;
        vector<vector<char>> board = {
            {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
            {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
            {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
            {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
            {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
            {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
            {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
            {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
            {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
        };

        cout << "输入数独:" << endl;
        for (int i = 0; i < 9; i++) {
            cout << "  ";
            for (int j = 0; j < 9; j++) {
                cout << board[i][j] << " ";
                if (j % 3 == 2 && j < 8) cout << "| ";
            }
            cout << endl;
            if (i % 3 == 2 && i < 8) {
                cout << "  ------+-------+------" << endl;
            }
        }

        sol.solveSudoku(board);

        cout << "解:" << endl;
        for (int i = 0; i < 9; i++) {
            cout << "  ";
            for (int j = 0; j < 9; j++) {
                cout << board[i][j] << " ";
                if (j % 3 == 2 && j < 8) cout << "| ";
            }
            cout << endl;
            if (i % 3 == 2 && i < 8) {
                cout << "  ------+-------+------" << endl;
            }
        }
        cout << "复杂度: 时间 O(9^m), 空间 O(m)" << endl;
    }
    cout << endl;

    cout << "============================================================" << endl;
    cout << "              所有测试用例执行完毕" << endl;
    cout << "============================================================" << endl;

    return 0;
}
