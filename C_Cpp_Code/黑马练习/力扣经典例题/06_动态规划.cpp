/**
 * ============================================================================
 *  动态规划经典例题 (Dynamic Programming Classic Problems)
 * ============================================================================
 *
 *  动态规划解题框架 (DP Problem-Solving Framework):
 *
 *  1. 定义状态 (Define State):  dp[i] 代表什么含义?
 *  2. 状态转移方程 (Transition):  dp[i] 与 dp[i-1], dp[i-2], ... 的关系
 *  3. 基础情况 (Base Case):      最简单的子问题的答案
 *  4. 迭代顺序 (Iteration Order): 正序 / 逆序 / 二维遍历顺序
 *  5. 最终答案 (Final Answer):    dp[n]? dp[n-1]? max(dp[...])?
 *
 *  核心思想: 将大问题拆分为重叠子问题, 通过记忆化避免重复计算
 *
 *  本文件包含以下 15 道经典题目:
 *   1. 爬楼梯         (LeetCode 70,   Easy)
 *   2. 零钱兑换       (LeetCode 322,  Medium)
 *   3. 最长递增子序列  (LeetCode 300,  Medium)
 *   4. 单词拆分       (LeetCode 139,  Medium)
 *   5. 打家劫舍       (LeetCode 198,  Medium)
 *   6. 不同路径       (LeetCode 62,   Medium)
 *   7. 编辑距离       (LeetCode 72,   Hard)
 *   8. 最长公共子序列  (LeetCode 1143, Medium)
 *   9. 最长回文子串    (LeetCode 5,    Medium)
 *  10. 零钱兑换 II    (LeetCode 518,  Medium)
 *  11. 最大子数组和    (LeetCode 53,   Medium)
 *  12. 买卖股票的最佳时机 (LeetCode 121, Easy)
 *  13. 完全平方数      (LeetCode 279,  Medium)
 *  14. 最长有效括号    (LeetCode 32,   Hard)
 *  15. 正则表达式匹配  (LeetCode 10,   Hard)
 *
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <cstring>
#include <queue>
#include <stack>

using namespace std;

// ============================================================================
//  1. 爬楼梯 (LeetCode 70, Easy)
// ============================================================================
/**
 * 题目描述:
 *   假设你正在爬楼梯, 需要 n 阶才能到达楼顶.
 *   每次你可以爬 1 或 2 个台阶, 你有多少种不同的方法可以爬到楼顶?
 *
 * 思路分析:
 *   到达第 i 阶的方法数 = 到达第 i-1 阶的方法数 + 到达第 i-2 阶的方法数
 *   因为最后一步要么走 1 阶, 要么走 2 阶
 *
 * 三种解法: 递归 -> 记忆化 -> 自底向上DP
 */

// ---- 解法1: 纯递归 (存在大量重复计算, 仅供理解思路) ----
int climbStairs_recursive(int n) {
    // 基础情况: 第1阶有1种, 第2阶有2种
    if (n <= 2) return n;
    // 递归: 爬到第n阶 = 爬到第n-1阶 + 爬到第n-2阶
    return climbStairs_recursive(n - 1) + climbStairs_recursive(n - 2);
}

// ---- 解法2: 记忆化递归 (自顶向下, 避免重复计算) ----
int climbStairs_memoHelper(int n, vector<int>& memo) {
    if (n <= 2) return n;
    // 如果已经计算过, 直接返回
    if (memo[n] != -1) return memo[n];
    // 记录结果到备忘录
    memo[n] = climbStairs_memoHelper(n - 1, memo) + climbStairs_memoHelper(n - 2, memo);
    return memo[n];
}

int climbStairs_memo(int n) {
    vector<int> memo(n + 1, -1);
    return climbStairs_memoHelper(n, memo);
}

// ---- 解法3: 自底向上DP (最优解) ----
int climbStairs_dp(int n) {
    if (n <= 2) return n;

    /*
     * DP Table 示意图 (n=5 的情况):
     *
     *   index:  0   1   2   3   4   5
     *   dp[i]:  0   1   2   3   5   8
     *                   ↑   ↑   ↑   ↑
     *                   |   |   |   |
     *               dp[2] dp[3] dp[4] dp[5]
     *                     =1+2  =2+3  =3+5
     *
     * 状态转移方程: dp[i] = dp[i-1] + dp[i-2]
     * 基础情况:     dp[1] = 1, dp[2] = 2
     */

    vector<int> dp(n + 1, 0);
    dp[1] = 1;
    dp[2] = 2;
    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

// ---- 解法4: 空间优化DP (只用两个变量) ----
int climbStairs_optimized(int n) {
    if (n <= 2) return n;
    // 只需要前两个状态, 不需要整个数组
    int prev2 = 1, prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

void testClimbStairs() {
    cout << "========================================" << endl;
    cout << "  1. 爬楼梯 (LeetCode 70)" << endl;
    cout << "========================================" << endl;

    // 基本测试用例
    vector<int> tests = {2, 3, 5, 10};
    for (int n : tests) {
        cout << "n = " << n << ": "
             << climbStairs_dp(n) << " 种方法"
             << endl;
    }

    // 验证三种解法结果一致
    int n = 15;
    int r1 = climbStairs_recursive(n);  // 递归 (n较小时可用)
    int r2 = climbStairs_memo(n);        // 记忆化递归
    int r3 = climbStairs_dp(n);          // 自底向上DP
    int r4 = climbStairs_optimized(n);   // 空间优化DP
    cout << "n = 15: 递归=" << r1
         << " 记忆化=" << r2
         << " DP=" << r3
         << " 优化=" << r4
         << " (全部一致)" << endl;
    cout << endl;
}


// ============================================================================
//  2. 零钱兑换 (LeetCode 322, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定不同面额的硬币 coins 和一个总金额 amount,
 *   计算可以凑成总金额所需的最少硬币数.
 *   如果无法凑成, 返回 -1.
 *   每种硬币数量无限.
 *
 * 状态转移方程: dp[i] = min(dp[i], dp[i - coin] + 1)  对所有 coin 遍历
 * 基础情况:     dp[0] = 0 (金额0需要0枚硬币)
 */

// ---- 解法1: 记忆化递归 ----
int coinChange_memoHelper(vector<int>& coins, int amount, vector<int>& memo) {
    if (amount < 0) return -1;   // 无法凑成
    if (amount == 0) return 0;   // 不需要硬币
    if (memo[amount] != -2) return memo[amount]; // 已计算

    int minCoins = INT_MAX;
    for (int coin : coins) {
        int sub = coinChange_memoHelper(coins, amount - coin, memo);
        // 子问题有解, 才更新最小值
        if (sub >= 0) {
            minCoins = min(minCoins, sub + 1);
        }
    }
    // 如果无法凑成, 记为 -1
    memo[amount] = (minCoins == INT_MAX) ? -1 : minCoins;
    return memo[amount];
}

int coinChange_memo(vector<int>& coins, int amount) {
    vector<int> memo(amount + 1, -2);
    return coinChange_memoHelper(coins, amount, memo);
}

// ---- 解法2: 自底向上DP ----
int coinChange_dp(vector<int>& coins, int amount) {
    /*
     * DP Table 示意图: coins = [1, 2, 5], amount = 11
     *
     *   amount:  0   1   2   3   4   5   6   7   8   9  10  11
     *   dp[i]:   0   1   1   2   2   1   2   2   3   3   2   3
     *                ↑       ↑           ↑               ↑
     *             coin=1  coin=2     coin=5          coin=5+coin=5
     *
     *   对于 amount=11:
     *     dp[11] = min(dp[11-1]+1, dp[11-2]+1, dp[11-5]+1)
     *            = min(dp[10]+1,   dp[9]+1,     dp[6]+1)
     *            = min(3,          4,           3)
     *            = 3  (即 5+5+1)
     */

    // 初始化为 amount+1 (不可能达到的值, 代表无穷大)
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int coin : coins) {
            if (coin <= i && dp[i - coin] + 1 < dp[i]) {
                dp[i] = dp[i - coin] + 1;
            }
        }
    }

    // 如果 dp[amount] 仍是初始值, 说明无法凑成
    return dp[amount] > amount ? -1 : dp[amount];
}

void testCoinChange() {
    cout << "========================================" << endl;
    cout << "  2. 零钱兑换 (LeetCode 322)" << endl;
    cout << "========================================" << endl;

    vector<int> coins1 = {1, 2, 5};
    cout << "coins = [1,2,5], amount = 11: "
         << coinChange_dp(coins1, 11) << endl;  // 输出: 3

    vector<int> coins2 = {2};
    cout << "coins = [2],     amount = 3: "
         << coinChange_dp(coins2, 3) << endl;   // 输出: -1

    vector<int> coins3 = {1};
    cout << "coins = [1],     amount = 0: "
         << coinChange_dp(coins3, 0) << endl;   // 输出: 0
    cout << endl;
}


// ============================================================================
//  3. 最长递增子序列 (LeetCode 300, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定一个整数数组 nums, 找到其中最长严格递增子序列的长度.
 *
 *   例如: nums = [10,9,2,5,3,7,101,18]
 *   最长递增子序列是 [2,3,7,101], 长度为 4
 *
 * 两种解法: O(n^2) DP 与 O(n log n) 二分查找
 */

// ---- 解法1: O(n^2) 动态规划 ----
int lengthOfLIS_dp(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;

    /*
     * 状态定义: dp[i] = 以 nums[i] 结尾的最长递增子序列长度
     *
     * 状态转移: 对于每个 i, 遍历所有 j < i:
     *   如果 nums[j] < nums[i], 则 dp[i] = max(dp[i], dp[j] + 1)
     *
     * DP Table 示意图: nums = [10, 9, 2, 5, 3, 7, 101, 18]
     *
     *   index:   0    1    2    3    4    5    6      7
     *   nums[i]: 10   9    2    5    3    7    101    18
     *   dp[i]:   1    1    1    2    2    3    4      4
     *                       ↑    ↑    ↑    ↑
     *                       |    |    |    |
     *                   2<5  2<3  3<7  7<101
     *                   dp=2 dp=2 dp=3 dp=4
     *
     * 最终答案: max(dp[0..n-1]) = 4
     */

    vector<int> dp(n, 1);  // 每个元素自身就是长度为1的子序列
    int maxLen = 1;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[j] < nums[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        maxLen = max(maxLen, dp[i]);
    }

    return maxLen;
}

// ---- 解法2: O(n log n) 贪心 + 二分查找 ----
int lengthOfLIS_binarySearch(vector<int>& nums) {
    /*
     * 思路: 维护一个数组 tails, 其中 tails[i] 表示长度为 i+1 的
     *       递增子序列的最小末尾元素.
     *
     * 对于每个 nums[i]:
     *   - 如果 nums[i] > tails.back(), 直接追加 (扩展子序列)
     *   - 否则, 在 tails 中二分查找第一个 >= nums[i] 的位置并替换
     *     (保持最小末尾, 为后续扩展留出更大空间)
     *
     * 示例: nums = [10, 9, 2, 5, 3, 7, 101, 18]
     *
     *   处理 10:  tails = [10]
     *   处理  9:  tails = [9]        (替换10)
     *   处理  2:  tails = [2]        (替换9)
     *   处理  5:  tails = [2, 5]     (追加)
     *   处理  3:  tails = [2, 3]     (替换5)
     *   处理  7:  tails = [2, 3, 7]  (追加)
     *   处理101:  tails = [2, 3, 7, 101] (追加)
     *   处理 18:  tails = [2, 3, 7, 18]  (替换101)
     *
     * 最终 tails 长度 = 4, 即为答案
     * 注意: tails 本身不一定是一个合法的子序列, 但其长度等于答案
     */

    vector<int> tails;
    for (int num : nums) {
        // 二分查找第一个 >= num 的位置
        auto it = lower_bound(tails.begin(), tails.end(), num);
        if (it == tails.end()) {
            tails.push_back(num);  // num 比所有元素都大, 追加
        } else {
            *it = num;             // 替换, 保持最小末尾
        }
    }
    return tails.size();
}

void testLIS() {
    cout << "========================================" << endl;
    cout << "  3. 最长递增子序列 (LeetCode 300)" << endl;
    cout << "========================================" << endl;

    vector<int> nums1 = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "nums = [10,9,2,5,3,7,101,18]" << endl;
    cout << "  DP解法:          " << lengthOfLIS_dp(nums1) << endl;
    cout << "  二分查找解法:     " << lengthOfLIS_binarySearch(nums1) << endl;

    vector<int> nums2 = {0, 1, 0, 3, 2, 3};
    cout << "nums = [0,1,0,3,2,3]" << endl;
    cout << "  DP解法:          " << lengthOfLIS_dp(nums2) << endl;
    cout << "  二分查找解法:     " << lengthOfLIS_binarySearch(nums2) << endl;

    vector<int> nums3 = {7, 7, 7, 7, 7};
    cout << "nums = [7,7,7,7,7]" << endl;
    cout << "  DP解法:          " << lengthOfLIS_dp(nums3) << endl;
    cout << "  二分查找解法:     " << lengthOfLIS_binarySearch(nums3) << endl;
    cout << endl;
}


// ============================================================================
//  4. 单词拆分 (LeetCode 139, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定字符串 s 和一个字典 wordDict, 判断 s 是否可以被拆分为
 *   一个或多个字典中的单词.
 *
 *   例如: s = "leetcode", wordDict = ["leet", "code"]
 *   输出: true (因为 "leet" + "code" 都在字典中)
 *
 * 状态定义: dp[i] = true 表示 s[0..i-1] 可以被拆分为字典中的单词
 * 状态转移: dp[i] = true 当且仅当存在 j < i 使得:
 *           dp[j] == true 且 s[j..i-1] 在字典中
 * 基础情况: dp[0] = true (空字符串)
 */

bool wordBreak_dp(string s, vector<string>& wordDict) {
    int n = s.size();

    // 将字典转为 unordered_set, 加速查找
    unordered_set<string> wordSet(wordDict.begin(), wordDict.end());

    /*
     * DP Table 示意图: s = "leetcode", wordDict = ["leet", "code"]
     *
     *   dp[0] = true  (空字符串, 基础情况)
     *
     *   s:       l  e  e  t  c  o  d  e
     *   index:   0  1  2  3  4  5  6  7
     *   dp[i+1]: F  F  F  T  F  F  F  T
     *                   ↑                 ↑
     *               dp[0]=T              dp[4]=T
     *           s[0..3]="leet"       s[4..7]="code"
     *           在字典中? Yes         在字典中? Yes
     *
     *   推导过程:
     *     dp[4]: 检查 j=0: dp[0]=T, s[0..3]="leet" 在字典中 => dp[4]=T
     *     dp[8]: 检查 j=4: dp[4]=T, s[4..7]="code" 在字典中 => dp[8]=T
     *
     *   最终答案: dp[n] = dp[8] = true
     */

    // dp[i] 表示 s[0..i-1] 是否可拆分
    vector<bool> dp(n + 1, false);
    dp[0] = true;  // 空字符串可以拆分

    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            // dp[j] 为 true 且 s[j..i-1] 在字典中
            if (dp[j] && wordSet.count(s.substr(j, i - j))) {
                dp[i] = true;
                break;  // 找到一种拆分方式即可
            }
        }
    }

    return dp[n];
}

void testWordBreak() {
    cout << "========================================" << endl;
    cout << "  4. 单词拆分 (LeetCode 139)" << endl;
    cout << "========================================" << endl;

    string s1 = "leetcode";
    vector<string> dict1 = {"leet", "code"};
    cout << "s = \"leetcode\", dict = [\"leet\",\"code\"]: "
         << (wordBreak_dp(s1, dict1) ? "true" : "false") << endl;

    string s2 = "applepenapple";
    vector<string> dict2 = {"apple", "pen"};
    cout << "s = \"applepenapple\", dict = [\"apple\",\"pen\"]: "
         << (wordBreak_dp(s2, dict2) ? "true" : "false") << endl;

    string s3 = "catsandog";
    vector<string> dict3 = {"cats", "dog", "sand", "and", "cat"};
    cout << "s = \"catsandog\", dict = [\"cats\",\"dog\",\"sand\",\"and\",\"cat\"]: "
         << (wordBreak_dp(s3, dict3) ? "true" : "false") << endl;
    cout << endl;
}


// ============================================================================
//  5. 打家劫舍 (LeetCode 198, Medium)
// ============================================================================
/**
 * 题目描述:
 *   你是一个专业小偷, 计划偷窃沿街的房屋.
 *   每间房内有一定现金, 但相邻的房屋装有联动报警系统.
 *   如果两间相邻的房屋被偷, 系统会自动报警.
 *   给定一个数组 nums 代表每间房的现金, 计算一夜之内能偷到的最大金额.
 *
 * 状态定义: dp[i] = 偷前 i+1 间房能获得的最大金额
 * 状态转移: dp[i] = max(dp[i-1], dp[i-2] + nums[i])
 *   - dp[i-1]: 不偷第 i 间, 取前 i 间的最优
 *   - dp[i-2] + nums[i]: 偷第 i 间, 加上前 i-1 间的最优
 * 基础情况: dp[0] = nums[0], dp[1] = max(nums[0], nums[1])
 */

int rob_dp(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    if (n == 1) return nums[0];

    /*
     * DP Table 示意图: nums = [2, 7, 9, 3, 1]
     *
     *   index:   0    1    2    3    4
     *   nums[i]: 2    7    9    3    1
     *
     *   dp[0] = 2  (只能偷第0间)
     *   dp[1] = max(2, 7) = 7  (偷第0间或第1间)
     *   dp[2] = max(dp[1], dp[0]+nums[2]) = max(7, 2+9) = 11
     *   dp[3] = max(dp[2], dp[1]+nums[3]) = max(11, 7+3) = 11
     *   dp[4] = max(dp[3], dp[2]+nums[4]) = max(11, 11+1) = 12
     *
     *   dp[i]:   2    7   11   11   12
     *                    ↑         ↑
     *            偷0+2=11      偷0+2+4=12
     *            (2+9=11)      (2+9+1=12)
     *
     * 最终答案: dp[4] = 12 (偷第0、2、4间: 2+9+1=12)
     */

    vector<int> dp(n, 0);
    dp[0] = nums[0];
    dp[1] = max(nums[0], nums[1]);

    for (int i = 2; i < n; i++) {
        dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
    }

    return dp[n - 1];
}

// ---- 空间优化版本: 只用两个变量 ----
int rob_optimized(vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    if (n == 1) return nums[0];

    int prev2 = nums[0];              // dp[i-2]
    int prev1 = max(nums[0], nums[1]); // dp[i-1]

    for (int i = 2; i < n; i++) {
        int curr = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

void testRob() {
    cout << "========================================" << endl;
    cout << "  5. 打家劫舍 (LeetCode 198)" << endl;
    cout << "========================================" << endl;

    vector<int> nums1 = {1, 2, 3, 1};
    cout << "nums = [1,2,3,1]: " << rob_dp(nums1) << endl;  // 4

    vector<int> nums2 = {2, 7, 9, 3, 1};
    cout << "nums = [2,7,9,3,1]: " << rob_dp(nums2) << endl;  // 12

    vector<int> nums3 = {2, 1, 1, 2};
    cout << "nums = [2,1,1,2]: " << rob_dp(nums3) << endl;  // 4
    cout << endl;
}


// ============================================================================
//  6. 不同路径 (LeetCode 62, Medium)
// ============================================================================
/**
 * 题目描述:
 *   一个机器人位于 m x n 网格的左上角, 每次只能向下或向右移动一步.
 *   机器人试图达到网格的右下角, 问总共有多少条不同路径?
 *
 * 状态定义: dp[i][j] = 从起点到达 (i, j) 的路径数
 * 状态转移: dp[i][j] = dp[i-1][j] + dp[i][j-1]
 * 基础情况: dp[0][j] = 1, dp[i][0] = 1 (第一行和第一列只有一种走法)
 */

int uniquePaths_dp(int m, int n) {
    /*
     * DP Grid 示意图: m=3, n=7 (3行7列)
     *
     *   列:      0    1    2    3    4    5    6
     *   行:
     *   0:       1    1    1    1    1    1    1
     *   1:       1    2    3    4    5    6    7
     *   2:       1    3    6   10   15   21   28
     *
     *   推导示例:
     *     dp[1][1] = dp[0][1] + dp[1][0] = 1 + 1 = 2
     *     dp[2][2] = dp[1][2] + dp[2][1] = 3 + 3 = 6
     *     dp[2][6] = dp[1][6] + dp[2][5] = 7 + 21 = 28
     *
     *   为什么第一行和第一列都是1?
     *     因为到第一行只能一直向右, 到第一列只能一直向下,
     *     各只有1条路径.
     *
     * 最终答案: dp[m-1][n-1] = dp[2][6] = 28
     */

    vector<vector<int>> dp(m, vector<int>(n, 0));

    // 基础情况: 第一行和第一列全部为 1
    for (int i = 0; i < m; i++) dp[i][0] = 1;
    for (int j = 0; j < n; j++) dp[0][j] = 1;

    // 填充 DP 表
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }

    return dp[m - 1][n - 1];
}

// ---- 空间优化: 只用一维数组 ----
int uniquePaths_optimized(int m, int n) {
    vector<int> dp(n, 1);  // 初始化为1, 相当于第一行

    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            // dp[j] 还没更新, 代表 dp[i-1][j] (上方)
            // dp[j-1] 已更新, 代表 dp[i][j-1] (左方)
            dp[j] = dp[j] + dp[j - 1];
        }
    }
    return dp[n - 1];
}

void testUniquePaths() {
    cout << "========================================" << endl;
    cout << "  6. 不同路径 (LeetCode 62)" << endl;
    cout << "========================================" << endl;

    // 测试用例1: 标准情况
    cout << "m=3, n=7: " << uniquePaths_dp(3, 7) << endl;   // 28
    // 测试用例2: 只有两列
    cout << "m=3, n=2: " << uniquePaths_dp(3, 2) << endl;   // 3
    // 测试用例3: 转置 (对称性: C(m+n-2, m-1) = C(m+n-2, n-1))
    cout << "m=7, n=3: " << uniquePaths_dp(7, 3) << endl;   // 28
    // 测试用例4: 正方形网格
    cout << "m=3, n=3: " << uniquePaths_dp(3, 3) << endl;   // 6
    // 测试用例5: 边界情况, 1x1 网格
    cout << "m=1, n=1: " << uniquePaths_dp(1, 1) << endl;   // 1
    // 测试用例6: 大网格, 验证空间优化版本
    cout << "m=10,n=10: " << uniquePaths_optimized(10, 10) << endl; // 48620
    cout << endl;
}


// ============================================================================
//  7. 编辑距离 (LeetCode 72, Hard) -- 面试高频题
// ============================================================================
/**
 * 题目描述:
 *   给定两个单词 word1 和 word2, 计算将 word1 转换成 word2 所需的
 *   最少操作数. 允许的操作有:
 *     - 插入一个字符
 *     - 删除一个字符
 *     - 替换一个字符
 *
 * 状态定义: dp[i][j] = word1[0..i-1] 转换为 word2[0..j-1] 的最小编辑距离
 * 状态转移:
 *   如果 word1[i-1] == word2[j-1]:
 *       dp[i][j] = dp[i-1][j-1]  (字符相同, 不需要操作)
 *   否则:
 *       dp[i][j] = 1 + min(
 *           dp[i-1][j],     删除: 删掉 word1[i-1]
 *           dp[i][j-1],     插入: 在 word1 中插入 word2[j-1]
 *           dp[i-1][j-1]    替换: 将 word1[i-1] 替换为 word2[j-1]
 *       )
 * 基础情况: dp[i][0] = i, dp[0][j] = j
 */

int minDistance_dp(string word1, string word2) {
    int m = word1.size();
    int n = word2.size();

    /*
     * DP Table 逐步推导: word1 = "horse", word2 = "ros"
     *
     * dp[i][j] 表示 word1[0..i-1] 到 word2[0..j-1] 的编辑距离
     *
     * 初始化:
     *        ""   r    o    s
     *   ""    0    1    2    3
     *   h     1    .    .    .
     *   o     2    .    .    .
     *   r     3    .    .    .
     *   s     4    .    .    .
     *   e     5    .    .    .
     *
     * 逐步填充:
     *
     * dp[1][1]: word1[0]='h' vs word2[0]='r', 不等
     *   = 1 + min(dp[0][1], dp[1][0], dp[0][0])
     *   = 1 + min(1, 1, 0) = 1
     *
     * dp[1][2]: 'h' vs 'o', 不等
     *   = 1 + min(dp[0][2], dp[1][1], dp[0][1])
     *   = 1 + min(2, 1, 1) = 2
     *
     * ... 依此类推 ...
     *
     * 最终 DP Table:
     *        ""   r    o    s
     *   ""    0    1    2    3
     *   h     1    1    2    3
     *   o     2    2    1    2
     *   r     3    2    2    2
     *   s     4    3    3    2
     *   e     5    4    4    3
     *                              ↑
     *                      dp[5][3] = 3
     *
     * 解释 dp[5][3] = 3:
     *   horse -> rorse (替换 h->r)
     *   rorse -> rose  (删除 r)
     *   rose  -> ros   (删除 e)
     *   共 3 次操作
     */

    // 创建 DP 表, (m+1) x (n+1)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 基础情况: word1 为空, 需要插入所有 word2 的字符
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    // 基础情况: word2 为空, 需要删除所有 word1 的字符
    for (int i = 0; i <= m; i++) dp[i][0] = i;

    // 填充 DP 表
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                // 字符相同, 不需要操作, 直接继承
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // 字符不同, 取三种操作的最小值 + 1
                dp[i][j] = 1 + min({
                    dp[i - 1][j],     // 删除: word1 删除当前字符
                    dp[i][j - 1],     // 插入: word1 插入 word2 的当前字符
                    dp[i - 1][j - 1]  // 替换: word1 当前字符替换为 word2 的
                });
            }
        }
    }

    return dp[m][n];
}

// ---- 空间优化: 使用两行滚动数组 ----
int minDistance_optimized(string word1, string word2) {
    int m = word1.size();
    int n = word2.size();

    // 只需要两行: 上一行和当前行
    vector<int> prev(n + 1), curr(n + 1);

    // 基础情况
    for (int j = 0; j <= n; j++) prev[j] = j;

    for (int i = 1; i <= m; i++) {
        curr[0] = i;  // 基础情况: dp[i][0] = i
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + min({prev[j], curr[j - 1], prev[j - 1]});
            }
        }
        swap(prev, curr);
    }

    return prev[n];
}

void testMinDistance() {
    cout << "========================================" << endl;
    cout << "  7. 编辑距离 (LeetCode 72)" << endl;
    cout << "========================================" << endl;

    string w1 = "horse", w2 = "ros";
    cout << "word1=\"horse\", word2=\"ros\": "
         << minDistance_dp(w1, w2) << endl;  // 3

    string w3 = "intention", w4 = "execution";
    cout << "word1=\"intention\", word2=\"execution\": "
         << minDistance_dp(w3, w4) << endl;  // 5

    string w5 = "", w6 = "abc";
    cout << "word1=\"\", word2=\"abc\": "
         << minDistance_dp(w5, w6) << endl;  // 3

    string w7 = "abc", w8 = "abc";
    cout << "word1=\"abc\", word2=\"abc\": "
         << minDistance_dp(w7, w8) << endl;  // 0
    cout << endl;
}


// ============================================================================
//  8. 最长公共子序列 (LeetCode 1143, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定两个字符串 text1 和 text2, 返回这两个字符串的最长公共子序列的长度.
 *   如果不存在公共子序列, 返回 0.
 *   子序列: 原字符串删除某些字符(也可以不删除)后剩余字符保持原顺序组成的新字符串.
 *
 *   例如: text1 = "abcde", text2 = "ace"
 *   最长公共子序列是 "ace", 长度为 3
 *
 * 状态定义: dp[i][j] = text1[0..i-1] 与 text2[0..j-1] 的最长公共子序列长度
 * 状态转移:
 *   如果 text1[i-1] == text2[j-1]:
 *       dp[i][j] = dp[i-1][j-1] + 1  (当前字符匹配, 长度+1)
 *   否则:
 *       dp[i][j] = max(dp[i-1][j], dp[i][j-1])  (不匹配, 取两种跳过方案的较大值)
 * 基础情况: dp[0][j] = 0, dp[i][0] = 0 (任一字符串为空时, 公共子序列长度为0)
 */

int longestCommonSubsequence(string text1, string text2) {
    int m = text1.size();
    int n = text2.size();

    /*
     * DP Table 逐步推导: text1 = "abcde", text2 = "ace"
     *
     * dp[i][j] 表示 text1[0..i-1] 与 text2[0..j-1] 的 LCS 长度
     *
     *        ""   a    c    e
     *   ""    0    0    0    0
     *   a     0    1    1    1
     *   b     0    1    1    1
     *   c     0    1    2    2
     *   d     0    1    2    2
     *   e     0    1    2    3
     *                       ↑
     *               dp[5][3] = 3, 即 LCS = "ace"
     *
     * 推导过程:
     *   dp[1][1]: text1[0]='a' == text2[0]='a'
     *     dp[1][1] = dp[0][0] + 1 = 0 + 1 = 1
     *
     *   dp[2][2]: text1[1]='b' != text2[1]='c'
     *     dp[2][2] = max(dp[1][2], dp[2][1]) = max(1, 1) = 1
     *
     *   dp[3][2]: text1[2]='c' == text2[1]='c'
     *     dp[3][2] = dp[2][1] + 1 = 1 + 1 = 2
     *
     *   dp[5][3]: text1[4]='e' == text2[2]='e'
     *     dp[5][3] = dp[4][2] + 1 = 2 + 1 = 3
     *
     * 最终答案: dp[5][3] = 3
     */

    // 创建 DP 表, (m+1) x (n+1), 全部初始化为 0
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    // 填充 DP 表
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                // 字符匹配: 两个字符串各消耗一个字符, 长度+1
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // 字符不匹配: 跳过 text1[i-1] 或跳过 text2[j-1], 取较大值
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[m][n];
}

// ---- 空间优化: 使用两行滚动数组 ----
int longestCommonSubsequence_optimized(string text1, string text2) {
    int m = text1.size();
    int n = text2.size();

    // 只需两行: 上一行和当前行
    vector<int> prev(n + 1, 0), curr(n + 1, 0);

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        swap(prev, curr);
        fill(curr.begin(), curr.end(), 0);
    }

    return prev[n];
}

void testLCS() {
    cout << "========================================" << endl;
    cout << "  8. 最长公共子序列 (LeetCode 1143)" << endl;
    cout << "========================================" << endl;

    string t1 = "abcde", t2 = "ace";
    cout << "text1=\"abcde\", text2=\"ace\": "
         << longestCommonSubsequence(t1, t2) << endl;  // 3

    string t3 = "abc", t4 = "abc";
    cout << "text1=\"abc\",   text2=\"abc\": "
         << longestCommonSubsequence(t3, t4) << endl;  // 3

    string t5 = "abc", t6 = "def";
    cout << "text1=\"abc\",   text2=\"def\": "
         << longestCommonSubsequence(t5, t6) << endl;  // 0

    // 验证空间优化版本
    cout << "text1=\"abcde\", text2=\"ace\" (优化): "
         << longestCommonSubsequence_optimized(t1, t2) << endl;  // 3
    cout << endl;
}


// ============================================================================
//  9. 最长回文子串 (LeetCode 5, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定字符串 s, 找到 s 中最长的回文子串.
 *
 *   例如: s = "babad", 最长回文子串是 "bab" 或 "aba"
 *
 * 三种解法:
 *   1. 中心扩展法 O(n^2) -- 推荐, 直观易懂
 *   2. 动态规划 O(n^2)
 *   3. Manacher 算法 O(n) -- 最优, 但较复杂
 *
 * 状态定义 (DP): dp[i][j] = true 表示 s[i..j] 是回文串
 * 状态转移:
 *   dp[i][j] = (s[i] == s[j]) && dp[i+1][j-1]
 * 基础情况: dp[i][i] = true (单个字符是回文), dp[i][i+1] = (s[i] == s[i+1])
 */

// ---- 解法1: 中心扩展法 O(n^2) ----
string longestPalindrome_expand(string s) {
    /*
     * 思路: 遍历每个位置, 以该位置为中心向两侧扩展, 寻找回文
     *
     * 回文有两种形式:
     *   1. 奇数长度: "aba"  -- 中心是单个字符
     *   2. 偶数长度: "abba" -- 中心是两个相同字符之间
     *
     * 示例: s = "babad"
     *
     *   中心 i=0 ('b'): 扩展失败, 回文 "b" (长度1)
     *   中心 i=1 ('a'): 向两侧扩展, "bab" (长度3)
     *   中心 i=2 ('b'): 向两侧扩展, "aba" (长度3)
     *   中心 i=3 ('a'): 扩展失败, 回文 "a" (长度1)
     *   中心 i=4 ('d'): 扩展失败, 回文 "d" (长度1)
     *
     *   最长回文: "bab" 或 "aba", 长度 3
     */

    int n = s.size();
    if (n < 2) return s;

    int start = 0, maxLen = 1;

    // 辅助函数: 从 [left, right] 向两侧扩展
    auto expand = [&](int left, int right) {
        while (left >= 0 && right < n && s[left] == s[right]) {
            left--;
            right++;
        }
        // 循环结束时, [left+1, right-1] 是回文
        int len = right - left - 1;
        if (len > maxLen) {
            maxLen = len;
            start = left + 1;
        }
    };

    for (int i = 0; i < n; i++) {
        expand(i, i);       // 奇数长度回文, 中心为 s[i]
        expand(i, i + 1);   // 偶数长度回文, 中心为 s[i] 和 s[i+1]
    }

    return s.substr(start, maxLen);
}

// ---- 解法2: 动态规划 O(n^2) ----
string longestPalindrome_dp(string s) {
    /*
     * DP Table 示意图: s = "babad"
     *
     *   dp[i][j] 表示 s[i..j] 是否为回文
     *
     *        b   a   b   a   d
     *   b    T   F   T   F   F
     *   a    .   T   F   T   F
     *   b    .   .   T   F   F
     *   a    .   .   .   T   F
     *   d    .   .   .   .   T
     *
     *   对角线为 T (单个字符是回文)
     *   dp[0][2]: s[0]='b'==s[2]='b' && dp[1][1]=T => T, 回文 "bab"
     *   dp[1][3]: s[1]='a'==s[3]='a' && dp[2][2]=T => T, 回文 "aba"
     *
     *   最长回文: dp[0][2] 或 dp[1][3], 长度 3
     */

    int n = s.size();
    if (n < 2) return s;

    vector<vector<bool>> dp(n, vector<bool>(n, false));
    int start = 0, maxLen = 1;

    // 基础情况: 单个字符是回文
    for (int i = 0; i < n; i++) dp[i][i] = true;

    // 按子串长度从小到大填充 (关键: 长度递增, 保证 dp[i+1][j-1] 已计算)
    for (int len = 2; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;  // 子串右端点
            if (s[i] == s[j]) {
                if (len == 2) {
                    dp[i][j] = true;  // 两个相同字符
                } else {
                    dp[i][j] = dp[i + 1][j - 1];  // 取决于内部子串
                }
            }
            if (dp[i][j] && len > maxLen) {
                maxLen = len;
                start = i;
            }
        }
    }

    return s.substr(start, maxLen);
}

// ---- 解法3: Manacher 算法 O(n) (概念展示) ----
string longestPalindrome_manacher(string s) {
    /*
     * Manacher 算法核心思想:
     *   1. 在每两个字符之间插入 '#', 统一处理奇偶长度回文
     *      "aba" => "#a#b#a#", "abba" => "#a#b#b#a#"
     *   2. 利用已知回文的对称性, 避免重复计算
     *   3. 维护一个 "右边界最远的回文" 的中心, 利用对称性初始化
     *
     * 示例: s = "babad" => T = "#b#a#b#a#d#"
     *
     *   遍历 T, 计算每个位置的回文半径:
     *     # b # a # b # a # d #
     *     1 2 1 4 1 4 1 2 1 2 1
     *           ↑       ↑
     *       最大半径4   最大半径4
     *       对应 "bab" 对应 "aba"
     *
     * 时间复杂度 O(n), 空间复杂度 O(n)
     * 此处给出简化实现, 便于理解
     */

    int n = s.size();
    if (n < 2) return s;

    // 预处理: 插入 '#'
    string t = "#";
    for (char c : s) {
        t += c;
        t += '#';
    }
    int m = t.size();
    vector<int> p(m, 0);  // p[i] = 以 t[i] 为中心的回文半径

    int center = 0, right = 0;  // 当前回文的中心和右边界
    int maxCenter = 0, maxRadius = 0;

    for (int i = 0; i < m; i++) {
        // 利用对称性初始化 p[i]
        if (i < right) {
            p[i] = min(right - i, p[2 * center - i]);
        }

        // 尝试扩展
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < m &&
               t[i - p[i] - 1] == t[i + p[i] + 1]) {
            p[i]++;
        }

        // 更新右边界最远的回文
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }

        // 记录最大回文
        if (p[i] > maxRadius) {
            maxRadius = p[i];
            maxCenter = i;
        }
    }

    // 从 T 映射回 s: 起始位置 = (maxCenter - maxRadius) / 2
    int start = (maxCenter - maxRadius) / 2;
    return s.substr(start, maxRadius);
}

void testLongestPalindrome() {
    cout << "========================================" << endl;
    cout << "  9. 最长回文子串 (LeetCode 5)" << endl;
    cout << "========================================" << endl;

    string s1 = "babad";
    cout << "s = \"babad\" (中心扩展): "
         << longestPalindrome_expand(s1) << endl;  // "bab" 或 "aba"
    cout << "s = \"babad\" (DP解法):    "
         << longestPalindrome_dp(s1) << endl;
    cout << "s = \"babad\" (Manacher):  "
         << longestPalindrome_manacher(s1) << endl;

    string s2 = "cbbd";
    cout << "s = \"cbbd\"  (中心扩展): "
         << longestPalindrome_expand(s2) << endl;  // "bb"
    cout << "s = \"cbbd\"  (DP解法):    "
         << longestPalindrome_dp(s2) << endl;

    string s3 = "a";
    cout << "s = \"a\"     (中心扩展): "
         << longestPalindrome_expand(s3) << endl;  // "a"

    string s4 = "racecar";
    cout << "s = \"racecar\" (Manacher): "
         << longestPalindrome_manacher(s4) << endl;  // "racecar"
    cout << endl;
}


// ============================================================================
//  10. 零钱兑换 II (LeetCode 518, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定不同面额的硬币 coins 和一个总金额 amount,
 *   计算可以凑成总金额的硬币组合数.
 *   每种硬币数量无限.
 *
 *   与零钱兑换 (LeetCode 322) 的区别:
 *     322 求最少硬币数, 518 求组合数
 *
 * 状态定义: dp[i] = 凑成金额 i 的硬币组合数
 * 状态转移: dp[i] += dp[i - coin]  (遍历每种硬币)
 * 基础情况: dp[0] = 1 (金额0有1种方案: 不选任何硬币)
 *
 * 关键: 遍历顺序 -- 外层遍历硬币, 内层遍历金额 (保证是组合而非排列)
 */

int change(int amount, vector<int>& coins) {
    /*
     * DP Table 示意图: coins = [1, 2, 5], amount = 5
     *
     *   外层遍历硬币, 内层遍历金额 (正序), 保证组合不重复
     *
     *   初始: dp = [1, 0, 0, 0, 0, 0]
     *
     *   处理 coin=1:
     *     amount: 0   1   2   3   4   5
     *     dp:     1   1   1   1   1   1
     *     (只用硬币1, 每个金额都只有1种方式)
     *
     *   处理 coin=2:
     *     amount: 0   1   2   3   4   5
     *     dp:     1   1   2   2   3   3
     *                        ↑   ↑   ↑
     *                   dp[3]+=dp[1]=1  dp[4]+=dp[2]=2  dp[5]+=dp[3]=2
     *                   (加了coin=2后)  (加了coin=2后)  (加了coin=2后)
     *
     *   处理 coin=5:
     *     amount: 0   1   2   3   4   5
     *     dp:     1   1   2   2   3   4
     *                             ↑
     *                        dp[5]+=dp[0]=1
     *
     *   最终 dp[5] = 4
     *
     *   4 种组合:
     *     5 = 5
     *     5 = 2 + 2 + 1
     *     5 = 2 + 1 + 1 + 1
     *     5 = 1 + 1 + 1 + 1 + 1
     */

    vector<int> dp(amount + 1, 0);
    dp[0] = 1;  // 金额0有1种方案

    // 外层遍历硬币 (保证组合而非排列, 避免重复计数)
    for (int coin : coins) {
        // 内层正序遍历金额 (完全背包: 每种硬币可无限使用)
        for (int i = coin; i <= amount; i++) {
            dp[i] += dp[i - coin];
        }
    }

    return dp[amount];
}

void testChangeII() {
    cout << "========================================" << endl;
    cout << "  10. 零钱兑换 II (LeetCode 518)" << endl;
    cout << "========================================" << endl;

    vector<int> coins1 = {1, 2, 5};
    cout << "coins = [1,2,5], amount = 5: "
         << change(5, coins1) << endl;  // 4

    vector<int> coins2 = {2};
    cout << "coins = [2],     amount = 3: "
         << change(3, coins2) << endl;  // 0

    vector<int> coins3 = {10};
    cout << "coins = [10],    amount = 10: "
         << change(10, coins3) << endl;  // 1

    vector<int> coins4 = {1, 5, 10, 25};
    cout << "coins = [1,5,10,25], amount = 50: "
         << change(50, coins4) << endl;  // 49
    cout << endl;
}


// ============================================================================
//  11. 最大子数组和 (LeetCode 53, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定一个整数数组 nums, 找到一个具有最大和的连续子数组 (子数组最少包含一个元素),
 *   返回其最大和.
 *
 *   例如: nums = [-2,1,-3,4,-1,2,1,-5,4]
 *   最大子数组是 [4,-1,2,1], 和为 6
 *
 * 状态定义: dp[i] = 以 nums[i] 结尾的最大子数组和
 * 状态转移: dp[i] = max(nums[i], dp[i-1] + nums[i])
 *   要么从 nums[i] 重新开始, 要么接在前面的子数组后面
 *
 * Kadane's Algorithm: 空间优化版, 只需一个变量
 */

// ---- 解法1: 标准DP ----
int maxSubArray_dp(vector<int>& nums) {
    /*
     * DP Table 示意图: nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
     *
     *   index:   0    1    2    3    4    5    6    7    8
     *   nums[i]: -2   1   -3   4   -1   2    1   -5   4
     *   dp[i]:   -2   1   -2   4    3   5    6    1    5
     *                  ↑        ↑        ↑    ↑
     *             max(1, -2+1) max(4, -2+4) max(2, 3+2) max(1, 5+1)
     *             = 1          = 4          = 5          = 6
     *
     *   dp[i] 的含义: 以 nums[i] 结尾的最大子数组和
     *     dp[0] = -2  (只有 -2)
     *     dp[1] = 1   (重新开始: max(1, -2+1) = 1)
     *     dp[2] = -2  (接在后面: max(-3, 1-3) = -2)
     *     dp[3] = 4   (重新开始: max(4, -2+4) = 4)
     *     dp[4] = 3   (接在后面: max(-1, 4-1) = 3)
     *     dp[5] = 5   (接在后面: max(2, 3+2) = 5)
     *     dp[6] = 6   (接在后面: max(1, 5+1) = 6)  <-- 最大
     *     dp[7] = 1   (接在后面: max(-5, 6-5) = 1)
     *     dp[8] = 5   (接在后面: max(4, 1+4) = 5)
     *
     *   最终答案: max(dp[0..8]) = 6
     *   对应子数组: [4, -1, 2, 1]
     */

    int n = nums.size();
    vector<int> dp(n, 0);
    dp[0] = nums[0];
    int maxSum = dp[0];

    for (int i = 1; i < n; i++) {
        // 核心决策: 接在前面的子数组后面, 还是重新开始?
        dp[i] = max(nums[i], dp[i - 1] + nums[i]);
        maxSum = max(maxSum, dp[i]);
    }

    return maxSum;
}

// ---- 解法2: Kadane's Algorithm (空间优化) ----
int maxSubArray_kadane(vector<int>& nums) {
    /*
     * Kadane's Algorithm: 只维护两个变量
     *   - currentMax: 以当前位置结尾的最大子数组和
     *   - globalMax:  全局最大子数组和
     *
     * 核心思想: 如果前面的和是负数, 不如从当前元素重新开始
     */

    int currentMax = nums[0];
    int globalMax = nums[0];

    for (int i = 1; i < nums.size(); i++) {
        currentMax = max(nums[i], currentMax + nums[i]);
        globalMax = max(globalMax, currentMax);
    }

    return globalMax;
}

void testMaxSubArray() {
    cout << "========================================" << endl;
    cout << "  11. 最大子数组和 (LeetCode 53)" << endl;
    cout << "========================================" << endl;

    vector<int> nums1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    cout << "nums = [-2,1,-3,4,-1,2,1,-5,4]: "
         << "DP=" << maxSubArray_dp(nums1)
         << ", Kadane=" << maxSubArray_kadane(nums1) << endl;  // 6

    vector<int> nums2 = {1};
    cout << "nums = [1]: "
         << "DP=" << maxSubArray_dp(nums2)
         << ", Kadane=" << maxSubArray_kadane(nums2) << endl;  // 1

    vector<int> nums3 = {5, 4, -1, 7, 8};
    cout << "nums = [5,4,-1,7,8]: "
         << "DP=" << maxSubArray_dp(nums3)
         << ", Kadane=" << maxSubArray_kadane(nums3) << endl;  // 23

    vector<int> nums4 = {-1, -2, -3, -4};
    cout << "nums = [-1,-2,-3,-4]: "
         << "DP=" << maxSubArray_dp(nums4)
         << ", Kadane=" << maxSubArray_kadane(nums4) << endl;  // -1
    cout << endl;
}


// ============================================================================
//  12. 买卖股票的最佳时机 (LeetCode 121, Easy)
// ============================================================================
/**
 * 题目描述:
 *   给定数组 prices, 其中 prices[i] 是第 i 天的股票价格.
 *   你只能选择某一天买入并选择未来的某一天卖出, 计算能获得的最大利润.
 *   如果无法获利, 返回 0.
 *
 *   例如: prices = [7,1,5,3,6,4]
 *   第2天买入(price=1), 第5天卖出(price=6), 利润 = 5
 *
 * 状态定义: dp[i] = 第 i 天卖出时能获得的最大利润
 * 状态转移: dp[i] = max(0, prices[i] - minPrice)
 *   其中 minPrice = 前 i 天的最低价格
 *
 * 优化: 不需要完整 DP 数组, 只需维护 minPrice 和 maxProfit
 */

// ---- 解法1: DP思路 ----
int maxProfit_dp(vector<int>& prices) {
    /*
     * DP Table 示意图: prices = [7, 1, 5, 3, 6, 4]
     *
     *   天数 i:     0    1    2    3    4    5
     *   prices[i]:  7    1    5    3    6    4
     *   minPrice:   7    1    1    1    1    1
     *   dp[i]:      0    0    4    2    5    3
     *                       ↑         ↑
     *                  5-1=4      6-1=5 (最大利润)
     *
     *   推导过程:
     *     dp[0] = 0 (第0天买入第0天卖出, 无利润)
     *     dp[1] = max(0, 1-7) = 0 (价格下跌, 无法获利)
     *     dp[2] = max(0, 5-1) = 4 (第1天买第2天卖)
     *     dp[3] = max(0, 3-1) = 2 (第1天买第3天卖)
     *     dp[4] = max(0, 6-1) = 5 (第1天买第4天卖) <-- 最大
     *     dp[5] = max(0, 4-1) = 3 (第1天买第5天卖)
     *
     *   最终答案: max(dp) = 5
     */

    int n = prices.size();
    if (n < 2) return 0;

    vector<int> dp(n, 0);       // dp[i] = 第i天卖出的最大利润
    int minPrice = prices[0];   // 前i天的最低价格

    for (int i = 1; i < n; i++) {
        // 更新最低买入价格
        minPrice = min(minPrice, prices[i]);
        // 第i天卖出的利润 = 当前价格 - 历史最低价
        dp[i] = max(0, prices[i] - minPrice);
    }

    return *max_element(dp.begin(), dp.end());
}

// ---- 解法2: 空间优化 (一次遍历) ----
int maxProfit_optimized(vector<int>& prices) {
    /*
     * 核心思想: 一次遍历, 同时维护历史最低价和最大利润
     *
     *   对于每一天, 如果在历史最低点买入, 今天卖出能赚多少?
     *   取所有天的最大值即为答案
     */

    int minPrice = INT_MAX;  // 历史最低价
    int maxProfit = 0;       // 最大利润

    for (int price : prices) {
        // 更新历史最低价
        minPrice = min(minPrice, price);
        // 如果今天卖出, 利润是多少?
        maxProfit = max(maxProfit, price - minPrice);
    }

    return maxProfit;
}

void testMaxProfit() {
    cout << "========================================" << endl;
    cout << "  12. 买卖股票的最佳时机 (LeetCode 121)" << endl;
    cout << "========================================" << endl;

    vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    cout << "prices = [7,1,5,3,6,4]: "
         << "DP=" << maxProfit_dp(prices1)
         << ", 优化=" << maxProfit_optimized(prices1) << endl;  // 5

    vector<int> prices2 = {7, 6, 4, 3, 1};
    cout << "prices = [7,6,4,3,1]:   "
         << "DP=" << maxProfit_dp(prices2)
         << ", 优化=" << maxProfit_optimized(prices2) << endl;  // 0

    vector<int> prices3 = {2, 4, 1};
    cout << "prices = [2,4,1]:       "
         << "DP=" << maxProfit_dp(prices3)
         << ", 优化=" << maxProfit_optimized(prices3) << endl;  // 2

    vector<int> prices4 = {3, 2, 6, 5, 0, 3};
    cout << "prices = [3,2,6,5,0,3]: "
         << "DP=" << maxProfit_dp(prices4)
         << ", 优化=" << maxProfit_optimized(prices4) << endl;  // 4
    cout << endl;
}


// ============================================================================
//  13. 完全平方数 (LeetCode 279, Medium)
// ============================================================================
/**
 * 题目描述:
 *   给定正整数 n, 找到若干个完全平方数 (如 1, 4, 9, 16, ...)
 *   使得它们的和等于 n, 返回最少需要多少个完全平方数.
 *
 *   例如: n = 12, 最优解: 12 = 4 + 4 + 4, 返回 3
 *   例如: n = 13, 最优解: 13 = 4 + 9,     返回 2
 *
 * 状态定义: dp[i] = 和为 i 的最少完全平方数个数
 * 状态转移: dp[i] = min(dp[i], dp[i - j*j] + 1)  对所有 j*j <= i
 * 基础情况: dp[0] = 0
 *
 * 两种解法: DP O(n * sqrt(n)) 和 BFS O(n)
 */

// ---- 解法1: 动态规划 ----
int numSquares_dp(int n) {
    /*
     * DP Table 示意图: n = 12
     *
     *   完全平方数: 1, 4, 9 (因为 16 > 12)
     *
     *   i:     0   1   2   3   4   5   6   7   8   9  10  11  12
     *   dp[i]: 0   1   2   3   1   2   3   4   2   1   2   3   3
     *              ↑           ↑               ↑           ↑   ↑
     *             1=1^2     4=2^2           9=3^2       1+11  4+4+4
     *                                                     =12  =12
     *
     *   推导过程:
     *     dp[1] = dp[1-1] + 1 = 0 + 1 = 1  (1 = 1)
     *     dp[2] = min(dp[2-1]+1) = min(2) = 2  (2 = 1+1)
     *     dp[4] = min(dp[4-1]+1, dp[4-4]+1) = min(4, 1) = 1  (4 = 4)
     *     dp[12] = min(dp[12-1]+1, dp[12-4]+1, dp[12-9]+1)
     *            = min(4, 3, 4) = 3  (12 = 4+4+4)
     *
     *   最终答案: dp[12] = 3
     */

    // 初始化: dp[i] = i (最坏情况: 全用 1^2)
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = 0;

    for (int i = 1; i <= n; i++) {
        // 尝试每个完全平方数 j*j
        for (int j = 1; j * j <= i; j++) {
            dp[i] = min(dp[i], dp[i - j * j] + 1);
        }
    }

    return dp[n];
}

// ---- 解法2: BFS ----
int numSquares_bfs(int n) {
    /*
     * BFS 思路: 将问题看作图的最短路径
     *
     *   节点: 数字 0, 1, 2, ..., n
     *   边:   从数字 x 到 x - j*j (减去一个完全平方数)
     *   目标: 从 n 到 0 的最短路径长度
     *
     *   例如 n = 12:
     *   第0层: {12}
     *   第1层: {12-1=11, 12-4=8, 12-9=3}
     *   第2层: {11-1=10, 11-4=7, 11-9=2, 8-1=7, 8-4=4, 8-9<0, 3-1=2, ...}
     *   第3层: {4-4=0} => 找到! 步数 = 3
     */

    queue<int> q;
    q.push(n);
    int level = 0;
    vector<bool> visited(n + 1, false);
    visited[n] = true;

    while (!q.empty()) {
        level++;
        int size = q.size();
        for (int i = 0; i < size; i++) {
            int curr = q.front();
            q.pop();
            // 尝试减去每个完全平方数
            for (int j = 1; j * j <= curr; j++) {
                int next = curr - j * j;
                if (next == 0) return level;  // 到达目标
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
    }

    return level;
}

void testNumSquares() {
    cout << "========================================" << endl;
    cout << "  13. 完全平方数 (LeetCode 279)" << endl;
    cout << "========================================" << endl;

    cout << "n = 12: DP=" << numSquares_dp(12)
         << ", BFS=" << numSquares_bfs(12) << endl;  // 3 (4+4+4)

    cout << "n = 13: DP=" << numSquares_dp(13)
         << ", BFS=" << numSquares_bfs(13) << endl;  // 2 (4+9)

    cout << "n =  1: DP=" << numSquares_dp(1)
         << ", BFS=" << numSquares_bfs(1) << endl;  // 1 (1)

    cout << "n =  4: DP=" << numSquares_dp(4)
         << ", BFS=" << numSquares_bfs(4) << endl;  // 1 (4)

    cout << "n =  7: DP=" << numSquares_dp(7)
         << ", BFS=" << numSquares_bfs(7) << endl;  // 4 (4+1+1+1)

    cout << "n = 100: DP=" << numSquares_dp(100)
         << ", BFS=" << numSquares_bfs(100) << endl;  // 1 (100)
    cout << endl;
}


// ============================================================================
//  14. 最长有效括号 (LeetCode 32, Hard)
// ============================================================================
/**
 * 题目描述:
 *   给定一个只包含 '(' 和 ')' 的字符串, 找出最长有效 (格式正确且连续)
 *   括号子串的长度.
 *
 *   例如: s = ")()())", 最长有效括号子串是 "()()", 长度 4
 *
 * 状态定义: dp[i] = 以 s[i] 结尾的最长有效括号子串长度
 * 状态转移:
 *   如果 s[i] == '(' : dp[i] = 0 (左括号不可能结尾)
 *   如果 s[i] == ')' :
 *     情况1: s[i-1] == '(' : dp[i] = dp[i-2] + 2
 *     情况2: s[i-1] == ')' && s[i-dp[i-1]-1] == '(' :
 *            dp[i] = dp[i-1] + dp[i-dp[i-1]-2] + 2
 *
 * 两种解法: 栈 和 DP
 */

// ---- 解法1: 栈 ----
int longestValidParentheses_stack(string s) {
    /*
     * 栈思路: 用栈存储下标, 栈底维护一个"上一个未匹配的右括号位置"
     *
     *   遍历字符串:
     *     '(' : 将下标压栈
     *     ')' : 弹栈, 计算当前有效长度
     *           如果栈空, 将当前下标压入 (作为新的基准)
     *           否则, 有效长度 = 当前下标 - 栈顶元素
     *
     * 示例: s = ")()())"
     *
     *   初始: stack = [-1]  (哨兵, 表示上一个未匹配右括号位置)
     *
     *   i=0, s[0]=')': 栈顶=-1, 弹出, 栈空, push(0)
     *         stack = [0]
     *
     *   i=1, s[1]='(': push(1)
     *         stack = [0, 1]
     *
     *   i=2, s[2]=')': 栈顶=1, 弹出, 栈不空, 长度=2-0=2
     *         stack = [0], maxLen = 2
     *
     *   i=3, s[3]='(': push(3)
     *         stack = [0, 3]
     *
     *   i=4, s[4]=')': 栈顶=3, 弹出, 栈不空, 长度=4-0=4
     *         stack = [0], maxLen = 4
     *
     *   i=5, s[5]=')': 栈顶=0, 弹出, 栈空, push(5)
     *         stack = [5]
     *
     *   最终: maxLen = 4
     */

    stack<int> stk;
    stk.push(-1);  // 哨兵: 上一个未匹配的右括号位置
    int maxLen = 0;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            stk.push(i);  // 左括号: 压入下标
        } else {
            stk.pop();     // 右括号: 弹出匹配
            if (stk.empty()) {
                stk.push(i);  // 栈空: 当前右括号成为新的基准
            } else {
                maxLen = max(maxLen, i - stk.top());  // 计算有效长度
            }
        }
    }

    return maxLen;
}

// ---- 解法2: 动态规划 ----
int longestValidParentheses_dp(string s) {
    /*
     * DP Table 示意图: s = ")()())"
     *
     *   index:  0   1   2   3   4   5
     *   s[i]:   )   (   )   (   )   )
     *   dp[i]:  0   0   2   0   4   0
     *                   ↑       ↑
     *               s[1]='('   s[3]='('
     *               s[2]=')'   s[4]=')'
     *               匹配!      匹配!
     *               dp[2]=     dp[4]=
     *               dp[0]+2=2  dp[2]+2=4
     *
     *   推导:
     *     dp[2]: s[1]='(' 且 s[2]=')', 匹配
     *       dp[2] = dp[0] + 2 = 0 + 2 = 2
     *     dp[4]: s[3]='(' 且 s[4]=')', 匹配
     *       dp[4] = dp[2] + 2 = 2 + 2 = 4
     *
     *   最终答案: max(dp) = 4
     */

    int n = s.size();
    if (n < 2) return 0;

    vector<int> dp(n, 0);  // dp[i] = 以 s[i] 结尾的最长有效括号长度
    int maxLen = 0;

    for (int i = 1; i < n; i++) {
        if (s[i] == ')') {
            if (s[i - 1] == '(') {
                // 情况1: ...(), dp[i] = dp[i-2] + 2
                dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
            } else if (dp[i - 1] > 0) {
                // 情况2: ...)), 需要检查 s[i-dp[i-1]-1] 是否为 '('
                int matchIdx = i - dp[i - 1] - 1;  // 与当前 ')' 匹配的 '(' 位置
                if (matchIdx >= 0 && s[matchIdx] == '(') {
                    // dp[i-1] 是内部匹配的长度, +2 是外层括号
                    // 还要加上 matchIdx 之前的匹配长度 dp[matchIdx-1]
                    dp[i] = dp[i - 1] + 2 + (matchIdx >= 1 ? dp[matchIdx - 1] : 0);
                }
            }
            maxLen = max(maxLen, dp[i]);
        }
    }

    return maxLen;
}

void testLongestValidParentheses() {
    cout << "========================================" << endl;
    cout << "  14. 最长有效括号 (LeetCode 32)" << endl;
    cout << "========================================" << endl;

    string s1 = ")()())";
    cout << "s = \")()())\": "
         << "栈=" << longestValidParentheses_stack(s1)
         << ", DP=" << longestValidParentheses_dp(s1) << endl;  // 4

    string s2 = "(()";
    cout << "s = \"(()\":    "
         << "栈=" << longestValidParentheses_stack(s2)
         << ", DP=" << longestValidParentheses_dp(s2) << endl;  // 2

    string s3 = "";
    cout << "s = \"\":      "
         << "栈=" << longestValidParentheses_stack(s3)
         << ", DP=" << longestValidParentheses_dp(s3) << endl;  // 0

    string s4 = "()()()";
    cout << "s = \"()()()\": "
         << "栈=" << longestValidParentheses_stack(s4)
         << ", DP=" << longestValidParentheses_dp(s4) << endl;  // 6

    string s5 = "()(())";
    cout << "s = \"()(())\": "
         << "栈=" << longestValidParentheses_stack(s5)
         << ", DP=" << longestValidParentheses_dp(s5) << endl;  // 6
    cout << endl;
}


// ============================================================================
//  15. 正则表达式匹配 (LeetCode 10, Hard)
// ============================================================================
/**
 * 题目描述:
 *   给定字符串 s 和字符模式 p, 实现支持 '.' 和 '*' 的正则表达式匹配.
 *     - '.' 匹配任意单个字符
 *     - '*' 匹配零个或多个前面的元素
 *   匹配需覆盖整个字符串 s (非部分匹配).
 *
 *   例如: s = "aab", p = "c*a*b" => true
 *   解释: c* 匹配0个c, a* 匹配2个a, b 匹配b
 *
 * 状态定义: dp[i][j] = s[0..i-1] 与 p[0..j-1] 是否匹配
 * 状态转移:
 *   如果 p[j-1] != '*':
 *     dp[i][j] = dp[i-1][j-1] && (s[i-1] == p[j-1] || p[j-1] == '.')
 *   如果 p[j-1] == '*':
 *     dp[i][j] = dp[i][j-2]  (匹配0次: 忽略 x*)
 *              || dp[i][j] = dp[i-1][j] && (s[i-1] == p[j-2] || p[j-2] == '.')
 *                (匹配1次或多次: 当前字符匹配, 继续用 *)
 * 基础情况: dp[0][0] = true, dp[0][j] 需要处理 a*b* 等情况
 */

bool isMatch(string s, string p) {
    /*
     * DP Table 逐步推导: s = "aab", p = "c*a*b"
     *
     * dp[i][j] 表示 s[0..i-1] 与 p[0..j-1] 是否匹配
     *
     *        ""   c    *    a    *    b
     *   ""    T    F    T    F    T    F
     *   a     F    F    T    T    T    F
     *   a     F    F    T    F    T    F
     *   b     F    F    T    F    T    T
     *                             ↑
     *                      dp[3][6] = true
     *
     * 推导过程:
     *
     * 初始化 (第0行, s为空):
     *   dp[0][0] = true  (空匹配空)
     *   dp[0][1] = false ('c' 非 *, 不匹配空)
     *   dp[0][2] = true  ('c*' 可匹配0次c, dp[0][0]=T)
     *   dp[0][3] = false ('a' 非 *, 不匹配空)
     *   dp[0][4] = true  ('a*' 可匹配0次a, dp[0][2]=T)
     *   dp[0][5] = false ('b' 非 *, 不匹配空)
     *
     * 填充 (逐行):
     *
     * dp[1][3]: s[0]='a', p[2]='a', 字母匹配
     *   dp[1][3] = dp[0][2] && (s[0]=='a') = T && T = T
     *
     * dp[1][4]: p[3]='*'
     *   方案1: 匹配0次, dp[1][2] = T  => dp[1][4] = T
     *   (不需要检查匹配多次, 因为方案1已为true)
     *
     * dp[2][4]: p[3]='*'
     *   方案1: 匹配0次, dp[2][2] = T  => dp[2][4] = T
     *
     * dp[3][6]: s[2]='b', p[5]='b', 字母匹配
     *   dp[3][6] = dp[2][5] && (s[2]=='b') = T && T = T
     *
     * 最终答案: dp[3][6] = true
     */

    int m = s.size();
    int n = p.size();

    // dp[i][j] = s[0..i-1] 与 p[0..j-1] 是否匹配
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));

    // 基础情况: 空字符串与空模式匹配
    dp[0][0] = true;

    // 基础情况: 处理 p 中的 a*b*c* 模式 (可以匹配空字符串)
    for (int j = 2; j <= n; j++) {
        if (p[j - 1] == '*') {
            dp[0][j] = dp[0][j - 2];  // '*' 匹配0次: 忽略 p[j-2] 和 p[j-1]
        }
    }

    // 填充 DP 表
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                // 情况1: '*' 匹配0次 -- 忽略 p[j-2]* 这一对
                dp[i][j] = dp[i][j - 2];

                // 情况2: '*' 匹配1次或多次 -- 当前 s[i-1] 匹配 p[j-2]
                //        且 p[0..j-1] 仍能匹配 s[0..i-2] (即 dp[i-1][j])
                if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                    dp[i][j] = dp[i][j] || dp[i - 1][j];
                }
            } else {
                // p[j-1] 不是 '*': 直接匹配当前字符
                if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                // 否则 dp[i][j] = false (默认值)
            }
        }
    }

    return dp[m][n];
}

void testIsMatch() {
    cout << "========================================" << endl;
    cout << "  15. 正则表达式匹配 (LeetCode 10)" << endl;
    cout << "========================================" << endl;

    cout << "s=\"aa\", p=\"a\":      "
         << (isMatch("aa", "a") ? "true" : "false") << endl;    // false

    cout << "s=\"aa\", p=\"a*\":     "
         << (isMatch("aa", "a*") ? "true" : "false") << endl;   // true

    cout << "s=\"ab\", p=\".*\":     "
         << (isMatch("ab", ".*") ? "true" : "false") << endl;   // true

    cout << "s=\"aab\", p=\"c*a*b\": "
         << (isMatch("aab", "c*a*b") ? "true" : "false") << endl; // true

    cout << "s=\"mississippi\", p=\"mis*is*p*.\": "
         << (isMatch("mississippi", "mis*is*p*.") ? "true" : "false") << endl; // false

    cout << "s=\"mississippi\", p=\"mis*is*ip*.\": "
         << (isMatch("mississippi", "mis*is*ip*.") ? "true" : "false") << endl; // true

    cout << "s=\"\", p=\"a*\":      "
         << (isMatch("", "a*") ? "true" : "false") << endl;     // true

    cout << "s=\"\", p=\"\":        "
         << (isMatch("", "") ? "true" : "false") << endl;       // true
    cout << endl;
}


// ============================================================================
//  主函数: 运行所有测试
// ============================================================================
int main() {
    cout << "############################################" << endl;
    cout << "##  动态规划经典例题 -- 15道 LeetCode 题目  ##" << endl;
    cout << "############################################" << endl;
    cout << endl;

    // 运行每道题的测试
    testClimbStairs();               //  1. 爬楼梯
    testCoinChange();                //  2. 零钱兑换
    testLIS();                       //  3. 最长递增子序列
    testWordBreak();                 //  4. 单词拆分
    testRob();                       //  5. 打家劫舍
    testUniquePaths();               //  6. 不同路径
    testMinDistance();                //  7. 编辑距离
    testLCS();                       //  8. 最长公共子序列
    testLongestPalindrome();         //  9. 最长回文子串
    testChangeII();                  // 10. 零钱兑换 II
    testMaxSubArray();               // 11. 最大子数组和
    testMaxProfit();                 // 12. 买卖股票的最佳时机
    testNumSquares();                // 13. 完全平方数
    testLongestValidParentheses();   // 14. 最长有效括号
    testIsMatch();                   // 15. 正则表达式匹配

    // 总结: 15道题的难度与时间复杂度一览
    cout << "========================================" << endl;
    cout << "  动态规划解题总结" << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "  15道题目总览:" << endl;
    cout << "  ┌──────────────────────┬────────┬────────────────┬────────────────┐" << endl;
    cout << "  │ 题目                 │ 难度   │ 时间复杂度     │ 空间复杂度     │" << endl;
    cout << "  ├──────────────────────┼────────┼────────────────┼────────────────┤" << endl;
    cout << "  │ 爬楼梯               │ Easy   │ O(n)           │ O(1) 优化后    │" << endl;
    cout << "  │ 零钱兑换             │ Medium │ O(n*amount)    │ O(amount)      │" << endl;
    cout << "  │ 最长递增子序列       │ Medium │ O(n log n)最优 │ O(n)           │" << endl;
    cout << "  │ 单词拆分             │ Medium │ O(n^2 * L)     │ O(n)           │" << endl;
    cout << "  │ 打家劫舍             │ Medium │ O(n)           │ O(1) 优化后    │" << endl;
    cout << "  │ 不同路径             │ Medium │ O(m*n)         │ O(n) 优化后    │" << endl;
    cout << "  │ 编辑距离             │ Hard   │ O(m*n)         │ O(n) 优化后    │" << endl;
    cout << "  │ 最长公共子序列       │ Medium │ O(m*n)         │ O(n) 优化后    │" << endl;
    cout << "  │ 最长回文子串         │ Medium │ O(n^2) 扩展法  │ O(1) 扩展法    │" << endl;
    cout << "  │ 零钱兑换 II          │ Medium │ O(n*amount)    │ O(amount)      │" << endl;
    cout << "  │ 最大子数组和         │ Medium │ O(n) Kadane    │ O(1) Kadane    │" << endl;
    cout << "  │ 买卖股票的最佳时机   │ Easy   │ O(n)           │ O(1)           │" << endl;
    cout << "  │ 完全平方数           │ Medium │ O(n*sqrt(n))   │ O(n)           │" << endl;
    cout << "  │ 最长有效括号         │ Hard   │ O(n)           │ O(n)           │" << endl;
    cout << "  │ 正则表达式匹配       │ Hard   │ O(m*n)         │ O(m*n)         │" << endl;
    cout << "  └──────────────────────┴────────┴────────────────┴────────────────┘" << endl;
    cout << endl;

    // 五步法详解
    cout << "  动态规划五步法:" << endl;
    cout << "    1. 定义状态:    dp[i] 代表什么? (最关键的第一步)" << endl;
    cout << "    2. 状态转移:    dp[i] 与子问题的关系 (写出方程)" << endl;
    cout << "    3. 基础情况:    最简单的子问题答案 (递归出口)" << endl;
    cout << "    4. 迭代顺序:    确保计算 dp[i] 时依赖项已计算" << endl;
    cout << "    5. 最终答案:    dp[n]? max(dp)? dp[m][n]? (答案在哪)" << endl;
    cout << endl;

    // 常见优化技巧
    cout << "  常见优化技巧:" << endl;
    cout << "    - 空间优化: 滚动数组 / 只保留必要的状态" << endl;
    cout << "    - 记忆化:   递归 + 备忘录, 自顶向下求解" << endl;
    cout << "    - 状态压缩: 用位运算表示状态集合" << endl;
    cout << "    - 降维:     二维 DP 压缩为一维 (如编辑距离、不同路径)" << endl;
    cout << endl;

    // 递推到 DP 的思维路径
    cout << "  从递归到DP的思维路径:" << endl;
    cout << "    递归 (暴力) --> 记忆化 (去重) --> 自底向上DP --> 空间优化" << endl;
    cout << "    建议: 先写递归版本, 确认正确后再逐步优化" << endl;
    cout << endl;

    return 0;
}
