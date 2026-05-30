/**
 * ============================================================================
 *  第11章 - 贪心算法 (Greedy Algorithm)
 * ============================================================================
 *
 *  贪心算法核心思想:
 *  在每一步选择中都做出当前看起来最优的选择(局部最优),
 *  期望通过每一步的局部最优选择达到全局最优。
 *
 *  贪心算法适用条件:
 *  1. 贪心选择性质: 通过局部最优选择能达到全局最优
 *  2. 最优子结构: 问题的最优解包含子问题的最优解
 *
 *  本章包含10道经典贪心算法题目:
 *  1.  跳跃游戏 (LeetCode 55, Medium)
 *  2.  合并区间 (LeetCode 56, Medium)
 *  3.  划分字母区间 (LeetCode 763, Medium)
 *  4.  买卖股票的最佳时机 II (LeetCode 122, Medium)
 *  5.  分发糖果 (LeetCode 135, Hard)
 *  6.  用最少数量的箭引爆气球 (LeetCode 452, Medium)
 *  7.  无重叠区间 (LeetCode 435, Medium)
 *  8.  会议室 II (LeetCode 253, Medium)
 *  9.  任务调度器 (LeetCode 621, Medium)
 *  10. 加油站 (LeetCode 134, Medium)
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <queue>
#include <functional>
using namespace std;

// ============================================================================
// 题目1: 跳跃游戏 (LeetCode 55, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个非负整数数组 nums, 你最初位于数组的第一个下标。
 *   数组中的每个元素代表你在该位置可以跳跃的最大长度。
 *   判断你是否能够到达最后一个下标。
 *
 * 示例:
 *   输入: nums = [2,3,1,1,4]
 *   输出: true
 *   解释: 从下标0跳1步到下标1, 再跳3步到达最后下标。
 *
 *   输入: nums = [3,2,1,0,4]
 *   输出: false
 *   解释: 无论怎么跳, 都会到达下标3, 而nums[3]=0, 无法继续前进。
 *
 * 为什么贪心算法有效:
 *   我们只需要维护一个变量 maxReach, 表示当前能到达的最远位置。
 *   遍历数组时, 如果当前位置 i <= maxReach, 说明可以到达这里,
 *   然后用 i + nums[i] 更新 maxReach。
 *   如果某时刻 maxReach >= 最后一个下标, 就说明可以到达。
 *
 *   这里贪心的策略是: 始终追踪能到达的最远位置。
 *   因为我们不需要知道具体怎么跳, 只需要知道能不能到达,
 *   所以只需维护最远可达距离即可。
 *
 *   ASCII图示: nums = [2, 3, 1, 1, 4]
 *
 *   下标:    0   1   2   3   4
 *   数值:   [2] [3] [1] [1] [4]
 *            |   |           ^
 *            |   +-----------+  从下标1可以跳3步直接到下标4
 *            +---+              从下标0可以跳2步到下标1或下标2
 *
 *   maxReach变化过程:
 *   i=0: maxReach = max(0, 0+2) = 2    可达范围 [0, 2]
 *   i=1: maxReach = max(2, 1+3) = 4    可达范围 [0, 4] >= 4, 返回true!
 *
 *   反例: nums = [3, 2, 1, 0, 4]
 *   i=0: maxReach = 3                   可达范围 [0, 3]
 *   i=1: maxReach = max(3, 1+2) = 3     可达范围 [0, 3]
 *   i=2: maxReach = max(3, 2+1) = 3     可达范围 [0, 3]
 *   i=3: maxReach = max(3, 3+0) = 3     可达范围 [0, 3], 卡住了!
 *   i=4: i=4 > maxReach=3, 无法到达下标4, 返回false
 */
bool canJump(vector<int>& nums) {
    int n = nums.size();
    int maxReach = 0; // 当前能到达的最远下标

    for (int i = 0; i < n; i++) {
        // 如果当前位置已经超过了能到达的最远距离, 说明到不了这里
        if (i > maxReach) {
            return false;
        }
        // 用当前位置能跳的最远距离更新 maxReach
        maxReach = max(maxReach, i + nums[i]);
        // 如果已经能到达最后一个下标, 提前返回
        if (maxReach >= n - 1) {
            return true;
        }
    }
    return true;
}

// 额外: 计算到达终点的最少跳跃次数 (贪心, 按层扩展)
/**
 * ASCII图示: 按层BFS思路的贪心版本
 *
 *   nums = [2, 3, 1, 1, 4]
 *
 *   第0步: 能到达下标 [0]          覆盖范围 [0, 0]
 *   第1步: 从下标0出发能到达 [1,2]  覆盖范围 [0, 2]
 *   第2步: 从下标1出发能到达 [2,3,4] 覆盖范围 [0, 4], 到达终点!
 *
 *   最少跳跃次数 = 2
 */
int jump(vector<int>& nums) {
    int n = nums.size();
    if (n <= 1) return 0;

    int jumps = 0;        // 跳跃次数
    int curEnd = 0;       // 当前这一跳能到达的最远边界
    int farthest = 0;     // 所有位置能到达的最远距离

    for (int i = 0; i < n - 1; i++) {
        farthest = max(farthest, i + nums[i]);
        // 到达当前跳跃的边界, 需要再跳一次
        if (i == curEnd) {
            jumps++;
            curEnd = farthest;
            // 如果已经能到达终点
            if (curEnd >= n - 1) break;
        }
    }
    return jumps;
}


// ============================================================================
// 题目2: 合并区间 (LeetCode 56, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给出一组区间, 将所有重叠的区间合并。
 *
 * 示例:
 *   输入: intervals = [[1,3],[2,6],[8,10],[15,18]]
 *   输出: [[1,6],[8,10],[15,18]]
 *
 * 贪心策略:
 *   1. 按区间起始位置排序
 *   2. 遍历区间, 如果当前区间与上一个合并区间有重叠, 就合并
 *   3. 否则将当前区间加入结果
 *
 * ASCII图示 - 合并过程:
 *
 *   排序后:
 *   [1----3]
 *      [2--------6]
 *                  [8-----10]
 *                             [15------18]
 *
 *   第1步: 结果 = [[1,3]]
 *
 *   第2步: [2,6] 与 [1,3] 重叠 (2 <= 3)
 *          合并为 [1,6]
 *   [1-----------6]
 *                  [8-----10]
 *                             [15------18]
 *   结果 = [[1,6]]
 *
 *   第3步: [8,10] 与 [1,6] 不重叠 (8 > 6)
 *          直接加入
 *   [1-----------6]
 *                  [8-----10]
 *                             [15------18]
 *   结果 = [[1,6], [8,10]]
 *
 *   第4步: [15,18] 与 [8,10] 不重叠 (15 > 10)
 *          直接加入
 *   结果 = [[1,6], [8,10], [15,18]]
 *
 * 时间复杂度: O(n log n), 主要来自排序
 * 空间复杂度: O(n), 存储结果
 */
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};

    // 按起始位置排序
    sort(intervals.begin(), intervals.end());

    vector<vector<int>> result;
    result.push_back(intervals[0]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        // 当前区间起始位置 <= 上一个合并区间的结束位置 -> 有重叠
        if (intervals[i][0] <= result.back()[1]) {
            // 合并: 更新结束位置为两者较大值
            result.back()[1] = max(result.back()[1], intervals[i][1]);
        } else {
            // 无重叠, 直接加入
            result.push_back(intervals[i]);
        }
    }
    return result;
}


// ============================================================================
// 题目3: 划分字母区间 (LeetCode 763, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个字符串 s, 将字符串划分为尽可能多的片段,
 *   同一字母最多出现在一个片段中, 返回每个片段的长度。
 *
 * 示例:
 *   输入: s = "ababcbacadefegdehijhklij"
 *   输出: [9,7,8]
 *   解释: 划分为 "ababcbaca", "defegde", "hijhklij"
 *
 * 贪心策略:
 *   1. 首先记录每个字符最后一次出现的位置
 *   2. 遍历字符串, 维护当前片段的结束位置(end)
 *   3. 对于每个字符, 用它的最后位置更新end
 *   4. 当遍历到end时, 就找到了一个完整片段
 *
 * ASCII图示:
 *
 *   s = "ababcbacadefegdehijhklij"
 *
 *   各字符最后出现位置:
 *   a:8  b:5  c:7  d:14  e:15  f:11  g:13
 *   h:19  i:22  j:23  k:20  l:21
 *
 *   遍历过程:
 *   a b a b c a b a c d e f e g d e h i j h k l i j
 *   0 1 2 3 4 5 6 7 8 9 ...
 *
 *   i=0, s[0]='a', end=max(0,8)=8
 *   i=1, s[1]='b', end=max(8,5)=8
 *   i=2, s[2]='a', end=max(8,8)=8
 *   ...
 *   i=8, s[8]='c', end=max(8,7)=8, i==end => 片段1长度=8-0+1=9
 *
 *   i=9, s[9]='d', end=max(9,14)=14
 *   i=10, s[10]='e', end=max(14,15)=15
 *   ...
 *   i=15, s[15]='e', end=max(15,15)=15, i==end => 片段2长度=15-9+1=7
 *
 *   i=16, s[16]='h', end=max(16,19)=19
 *   ...
 *   i=23, s[23]='j', end=max(23,23)=23, i==end => 片段3长度=23-16+1=8
 *
 * 时间复杂度: O(n)
 * 空间复杂度: O(1), 最多26个字母
 */
vector<int> partitionLabels(string s) {
    // 记录每个字符最后出现的位置
    int last[26] = {0};
    for (int i = 0; i < (int)s.size(); i++) {
        last[s[i] - 'a'] = i;
    }

    vector<int> result;
    int start = 0; // 当前片段起始位置
    int end = 0;   // 当前片段结束位置

    for (int i = 0; i < (int)s.size(); i++) {
        // 用当前字符的最后位置扩展片段结束边界
        end = max(end, last[s[i] - 'a']);
        // 到达片段末尾, 记录片段长度
        if (i == end) {
            result.push_back(end - start + 1);
            start = end + 1; // 下一个片段从 end+1 开始
        }
    }
    return result;
}


// ============================================================================
// 题目4: 买卖股票的最佳时机 II (LeetCode 122, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个数组 prices, 其中 prices[i] 是第 i 天的股票价格。
 *   你可以无限次买卖(但同时只能持有一股), 求最大利润。
 *
 * 示例:
 *   输入: prices = [7,1,5,3,6,4]
 *   输出: 7
 *   解释: 第2天买(1), 第3天卖(5) => 利润4
 *          第4天买(3), 第5天卖(6) => 利润3
 *          总利润 = 4 + 3 = 7
 *
 * 贪心策略:
 *   收集所有正的价格差! 只要明天比今天贵, 就今天买明天卖。
 *
 *   为什么这是正确的?
 *   假设价格序列为 a, b, c, 且 a < b < c:
 *     方案1: 第1天买第3天卖 => 利润 c - a
 *     方案2: 第1天买第2天卖, 第2天买第3天卖 => (b-a) + (c-b) = c - a
 *   两种方案利润相同! 所以收集所有正差价不会漏掉任何利润。
 *
 *   ASCII图示:
 *
 *   价格走势:
 *   7 |*
 *   6 |                    *
 *   5 |        *
 *   4 |                    *
 *   3 |              *
 *   2 |
 *   1 |    *
 *     +--+----+----+----+----+-->
 *       1    2    3    4    5   天数
 *
 *   正差价: (1->5) = 4, (3->6) = 3
 *   总利润 = 4 + 3 = 7
 *
 * DP 方式对比:
 *   dp[i][0] = 第 i 天不持有股票的最大利润
 *   dp[i][1] = 第 i 天持有股票的最大利润
 *   dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i])
 *   dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])
 *
 * 时间复杂度: 贪心 O(n), DP O(n)
 * 空间复杂度: 贪心 O(1), DP O(n) 可优化到 O(1)
 */

// 贪心解法
int maxProfitGreedy(vector<int>& prices) {
    int profit = 0;
    for (int i = 1; i < (int)prices.size(); i++) {
        // 只要今天比昨天贵, 就赚差价
        if (prices[i] > prices[i - 1]) {
            profit += prices[i] - prices[i - 1];
        }
    }
    return profit;
}

// DP解法 (用于对比)
int maxProfitDP(vector<int>& prices) {
    int n = prices.size();
    if (n <= 1) return 0;

    // dp0: 不持有股票的最大利润
    // dp1: 持有股票的最大利润
    int dp0 = 0;
    int dp1 = -prices[0];

    for (int i = 1; i < n; i++) {
        int newDp0 = max(dp0, dp1 + prices[i]);
        int newDp1 = max(dp1, dp0 - prices[i]);
        dp0 = newDp0;
        dp1 = newDp1;
    }
    return dp0; // 最后一天不持有股票肯定利润最大
}


// ============================================================================
// 题目5: 分发糖果 (LeetCode 135, Hard)
// ============================================================================
/**
 * 问题描述:
 *   n 个孩子站成一排, 每个孩子有一个评分。
 *   给每个孩子分配糖果, 要求:
 *     - 每个孩子至少分到 1 个糖果
 *     - 评分更高的孩子比相邻的孩子获得更多糖果
 *   求最少需要多少个糖果。
 *
 * 示例:
 *   输入: ratings = [1,0,2]
 *   输出: 5
 *   解释: 分配 [2,1,2], 共5个
 *
 *   输入: ratings = [1,2,2]
 *   输出: 4
 *   解释: 分配 [1,2,1], 共4个
 *   注意: 评分相同不需要比相邻的多
 *
 * 贪心策略: 两次遍历
 *   1. 从左到右: 如果右边评分比左边高, 右边糖果 = 左边 + 1
 *   2. 从右到左: 如果左边评分比右边高, 取 max(当前值, 右边+1)
 *
 * 为什么需要两次遍历?
 *   一次遍历只能保证一个方向的约束。
 *   从左到右保证: 右边比左边高的情况
 *   从右到左保证: 左边比右边高的情况
 *   两次遍历的 max 操作确保两个方向都满足。
 *
 * ASCII图示:
 *
 *   ratings = [1, 0, 2, 3, 1, 2, 1]
 *
 *   评分:  1   0   2   3   1   2   1
 *
 *   第一步(从左到右): 每人先分1个
 *   糖果:  1   1   1   1   1   1   1
 *
 *   从左到右扫描, 右边比左边高则+1:
 *   i=2: ratings[2]=2 > ratings[0]=1, 糖果[2]=糖果[1]+1=2
 *   i=3: ratings[3]=3 > ratings[2]=2, 糖果[3]=糖果[2]+1=3
 *   i=5: ratings[5]=2 > ratings[4]=1, 糖果[5]=糖果[4]+1=2
 *
 *   糖果:  1   1   2   3   1   2   1
 *
 *   从右到左扫描, 左边比右边高则取max:
 *   i=4: ratings[4]=1 < ratings[5]=2, 无变化
 *   i=3: ratings[3]=3 > ratings[4]=1, 糖果[3]=max(3,1+1)=3
 *   i=2: ratings[2]=2 > ratings[3]=1, 无变化(右边更大)
 *        实际: ratings[2]=2 < ratings[3]=3, 无变化
 *   i=1: ratings[1]=0 < ratings[2]=2, 无变化
 *   i=0: ratings[0]=1 > ratings[1]=0, 糖果[0]=max(1,1+1)=2
 *
 *   糖果:  2   1   2   3   1   2   1
 *   总计: 2+1+2+3+1+2+1 = 12
 *
 * 时间复杂度: O(n)
 * 空间复杂度: O(n)
 */
int candy(vector<int>& ratings) {
    int n = ratings.size();
    if (n <= 1) return n;

    vector<int> candies(n, 1); // 每人至少1个

    // 从左到右: 右边比左边高, 右边 = 左边 + 1
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1]) {
            candies[i] = candies[i - 1] + 1;
        }
    }

    // 从右到左: 左边比右边高, 取max(当前值, 右边+1)
    for (int i = n - 2; i >= 0; i--) {
        if (ratings[i] > ratings[i + 1]) {
            candies[i] = max(candies[i], candies[i + 1] + 1);
        }
    }

    // 累加总糖果数
    return accumulate(candies.begin(), candies.end(), 0);
}


// ============================================================================
// 题目6: 用最少数量的箭引爆气球 (LeetCode 452, Medium)
// ============================================================================
/**
 * 问题描述:
 *   有一些球形气球贴在一堵用 XY 平面表示的墙面上。
 *   墙面上的气球用整数数组 points 表示, 其中 points[i] = [xstart, xend]
 *   表示气球在水平 x 轴上的范围 [xstart, xend]。
 *   你只能沿着 y 轴垂直射箭, 弓箭可以从 x 轴任意位置射出。
 *   如果一个气球的 x 坐标范围包含 x, 那么这个气球会被引爆。
 *   求引爆所有气球所需的最少弓箭数。
 *
 * 示例:
 *   输入: points = [[10,16],[2,8],[1,6],[7,12]]
 *   输出: 2
 *   解释: 在 x=6 处射箭引爆 [2,8] 和 [1,6], 在 x=11 处射箭引爆 [7,12] 和 [10,16]
 *
 * 贪心策略:
 *   按结束位置排序! 每支箭射在当前未引爆气球中最早结束的位置。
 *   这样一箭能覆盖尽可能多的重叠气球。
 *
 *   为什么按结束位置排序?
 *   - 结束位置越早, 与后续气球重叠的可能性越小
 *   - 贪心: 尽可能在靠近末尾的位置射箭, 能覆盖更多气球
 *
 * ASCII图示:
 *
 *   排序前(按start): [[1,6],[2,8],[7,12],[10,16]]
 *   排序后(按end):   [[1,6],[2,8],[7,12],[10,16]]
 *
 *   x轴: 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
 *        [=======1=======]
 *           [==========2==========]
 *                              [==========3==========]
 *                                     [============4============]
 *
 *   第1箭: end=6, 射在x=6处
 *         [=======1=======]  <- 被引爆
 *           [==========2==========]  <- 也被引爆(start=2 <= 6)
 *
 *   第2箭: end=12, 射在x=12处
 *                              [==========3==========]  <- 被引爆
 *                                     [============4============]  <- 也被引爆
 *
 *   共需2箭
 *
 * 时间复杂度: O(n log n), 排序
 * 空间复杂度: O(1)
 */
int findMinArrowShots(vector<vector<int>>& points) {
    if (points.empty()) return 0;

    // 按结束位置排序
    sort(points.begin(), points.end(),
         [](const vector<int>& a, const vector<int>& b) {
             return a[1] < b[1];
         });

    int arrows = 1;                // 至少需要1箭
    long long arrowPos = points[0][1]; // 第1箭射在第1个气球的结束位置

    for (int i = 1; i < (int)points.size(); i++) {
        // 当前气球的起始位置 > 箭的位置, 说明这箭引爆不了
        if (points[i][0] > arrowPos) {
            arrows++;                    // 需要新箭
            arrowPos = points[i][1];     // 新箭射在当前气球结束位置
        }
        // 否则: 当前气球已被之前的箭引爆, 无需额外箭
    }
    return arrows;
}


// ============================================================================
// 题目7: 无重叠区间 (LeetCode 435, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个区间的集合 intervals, 其中 intervals[i] = [start, end]。
 *   返回需要移除的最小区间数量, 使剩余区间互不重叠。
 *
 * 示例:
 *   输入: intervals = [[1,2],[2,3],[3,4],[1,3]]
 *   输出: 1
 *   解释: 移除 [1,3] 后, 剩下的区间互不重叠。
 *
 *   输入: intervals = [[1,2],[1,2],[1,2]]
 *   输出: 2
 *   解释: 需要移除2个 [1,2], 只留1个。
 *
 * 贪心策略:
 *   按结束位置排序, 保留不重叠的区间, 统计重叠数量。
 *   这等价于: 找到最大数量的不重叠区间, 用总数减去它。
 *
 *   为什么按结束位置排序?
 *   - 结束越早, 占用空间越小, 给后续区间留更多空间
 *   - 这是经典的"区间调度"问题, 贪心选择最早结束的区间是最优的
 *
 * ASCII图示:
 *
 *   intervals = [[1,2],[2,3],[3,4],[1,3]]
 *
 *   排序后(按end): [[1,2],[2,3],[1,3],[3,4]]
 *
 *   x轴: 1    2    3    4
 *        [1,2]
 *             [2,3]
 *        [====1,3====]        <- 与[2,3]重叠, 需要移除
 *                  [3,4]
 *
 *   保留: [1,2], [2,3], [3,4] => 3个不重叠区间
 *   移除: 4 - 3 = 1个
 *
 * 时间复杂度: O(n log n)
 * 空间复杂度: O(1)
 */
int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;

    // 按结束位置排序
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) {
             return a[1] < b[1];
         });

    int count = 0;             // 需要移除的区间数
    int prevEnd = intervals[0][1]; // 上一个保留区间的结束位置

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < prevEnd) {
            // 当前区间与上一个保留区间重叠, 需要移除
            count++;
        } else {
            // 不重叠, 保留当前区间, 更新结束位置
            prevEnd = intervals[i][1];
        }
    }
    return count;
}


// ============================================================================
// 题目8: 会议室 II (LeetCode 253, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个会议时间安排的数组 intervals, 每个区间包含开始和结束时间
 *   [start, end], 计算所需的最小会议室数量。
 *
 * 示例:
 *   输入: intervals = [[0,30],[5,10],[15,20]]
 *   输出: 2
 *   解释: [0,30] 和 [5,10] 重叠, 需要2间; [5,10] 结束后 [15,20] 可用同一间
 *
 *   输入: intervals = [[7,10],[2,4]]
 *   输出: 1
 *   解释: 两个会议不重叠, 1间即可
 *
 * 方法一: 最小堆 (贪心 + 优先队列)
 *   1. 按开始时间排序
 *   2. 用最小堆维护每间会议室的结束时间
 *   3. 对于每个会议: 如果最早结束的会议室已空闲, 就复用; 否则开新会议室
 *
 * 方法二: 扫描线 (时间轴差分)
 *   将开始和结束看成事件, 开始+1, 结束-1, 求最大前缀和
 *
 * ASCII图示 - 最小堆方法:
 *
 *   intervals = [[0,30],[5,10],[15,20]]
 *
 *   时间轴:
 *   0    5    10   15   20   25   30
 *   [========================会议0========================]
 *        [==会议1==]
 *                   [===会议2===]
 *
 *   处理过程:
 *   会议[0,30]: 堆为空 -> 新开房间1, 堆=[30]
 *   会议[5,10]: 堆顶30 > 5(未空闲) -> 新开房间2, 堆=[10, 30]
 *   会议[15,20]: 堆顶10 <= 15(已空闲) -> 复用房间2, 堆=[20, 30]
 *
 *   最终堆大小 = 2 -> 需要2间会议室
 *
 * ASCII图示 - 扫描线方法:
 *
 *   事件: (0,+1), (5,+1), (10,-1), (15,+1), (20,-1), (30,-1)
 *   排序: (0,+1), (5,+1), (10,-1), (15,+1), (20,-1), (30,-1)
 *
 *   扫描:
 *   时间0:  active=1  最大=1
 *   时间5:  active=2  最大=2
 *   时间10: active=1  最大=2
 *   时间15: active=2  最大=2
 *   时间20: active=1  最大=2
 *   时间30: active=0  最大=2
 *
 *   结果: 最大同时活跃 = 2 -> 需要2间会议室
 *
 * 时间复杂度: O(n log n)
 * 空间复杂度: O(n)
 */

// 方法一: 最小堆
int minMeetingRoomsHeap(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;

    // 按开始时间排序
    sort(intervals.begin(), intervals.end());

    // 最小堆: 存储每间会议室的结束时间
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (auto& interval : intervals) {
        // 如果最早结束的会议室已空闲(结束时间 <= 当前开始时间), 复用
        if (!minHeap.empty() && minHeap.top() <= interval[0]) {
            minHeap.pop();
        }
        // 将当前会议的结束时间加入堆
        minHeap.push(interval[1]);
    }

    return minHeap.size(); // 堆大小 = 所需会议室数
}

// 方法二: 扫描线 (差分数组)
int minMeetingRoomsSweepLine(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;

    vector<pair<int, int>> events; // (时间, 变化量)
    for (auto& interval : intervals) {
        events.push_back({interval[0], 1});  // 开始: +1
        events.push_back({interval[1], -1}); // 结束: -1
    }

    // 排序: 相同时间, 结束事件优先(避免重复计算)
    sort(events.begin(), events.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second; // -1 排在 +1 前面
    });

    int active = 0, maxActive = 0;
    for (auto& [time, delta] : events) {
        active += delta;
        maxActive = max(maxActive, active);
    }

    return maxActive;
}


// ============================================================================
// 题目9: 任务调度器 (LeetCode 621, Medium)
// ============================================================================
/**
 * 问题描述:
 *   给定一个用字符数组 tasks 表示的 CPU 需要执行的任务列表,
 *   其中每个字母表示一种不同任务。任务可以以任意顺序执行,
 *   并且每个任务都可以在 1 个单位时间内执行完。
 *   CPU 有一个冷却时间 n, 两个相同种类的任务之间必须间隔至少 n 个单位时间。
 *   计算完成所有任务所需要的最短时间。
 *
 * 示例:
 *   输入: tasks = ["A","A","A","B","B","B"], n = 2
 *   输出: 8
 *   解释: A -> B -> 空 -> A -> B -> 空 -> A -> B
 *         序列: A B _ A B _ A B, 共8个时间单位
 *
 * 贪心策略 (冷却间隔公式):
 *   最频繁的任务出现次数为 maxFreq, 出现 maxFreq 次的任务有 maxCount 种。
 *
 *   构造框架: 以最频繁任务为骨架, 填充其他任务到冷却间隔中
 *   A _ _ A _ _ A    (n=2, maxFreq=3)
 *   每个A后面有n个位置需要填充
 *
 *   总间隔数 = (maxFreq - 1) * (n + 1) + maxCount
 *   最短时间 = max(总间隔数, tasks总长度)
 *
 *   为什么取max?
 *   - 如果任务种类很多, 不需要空闲, 直接按长度排列
 *   - 如果任务种类少, 需要空闲来满足冷却时间
 *
 * ASCII图示:
 *
 *   tasks = ["A","A","A","B","B","B"], n = 2
 *   maxFreq = 3 (A和B都出现3次), maxCount = 2
 *
 *   框架: A _ _ A _ _ A
 *   填充B: A B _ A B _ A
 *   最后填B: A B _ A B _ A B
 *
 *   总长度 = (3-1) * (2+1) + 2 = 2*3+2 = 8
 *   tasks总长 = 6
 *   结果 = max(8, 6) = 8
 *
 *   另一个例子: tasks = ["A","A","A","B","B","B","C","C","D","D"], n = 2
 *   maxFreq = 3, maxCount = 1 (只有A)
 *   框架: A _ _ A _ _ A
 *   填充: A B C A B D A
 *   剩余: B C D -> 直接追加
 *   结果 = max(7, 10) = 10 (种类足够多, 不需要空闲)
 *
 * 时间复杂度: O(n), 遍历tasks统计频率
 * 空间复杂度: O(1), 最多26种任务
 */
int leastInterval(vector<char>& tasks, int n) {
    // 统计每个任务出现的频率
    int freq[26] = {0};
    for (char t : tasks) {
        freq[t - 'A']++;
    }

    // 找到最大频率和出现最大频率的任务种类数
    int maxFreq = 0, maxCount = 0;
    for (int f : freq) {
        if (f > maxFreq) {
            maxFreq = f;
            maxCount = 1;
        } else if (f == maxFreq) {
            maxCount++;
        }
    }

    // 公式: (maxFreq - 1) * (n + 1) + maxCount
    int intervals = (maxFreq - 1) * (n + 1) + maxCount;
    // 如果任务种类很多, 不需要空闲, 直接用总长度
    return max(intervals, (int)tasks.size());
}


// ============================================================================
// 题目10: 加油站 (LeetCode 134, Medium)
// ============================================================================
/**
 * 问题描述:
 *   在一条环路上有 n 个加油站, 其中第 i 个加油站有汽油 gas[i] 升。
 *   你有一辆油箱容量无限的汽车, 从第 i 个加油站开往第 i+1 个加油站
 *   需要消耗 cost[i] 升汽油。从某个加油站出发, 汽车油箱为空。
 *   如果你可以绕环路行驶一周, 返回出发加油站的编号; 否则返回 -1。
 *
 * 示例:
 *   输入: gas = [1,2,3,4,5], cost = [3,4,5,1,2]
 *   输出: 3
 *   解释: 从3号加油站出发:
 *         到达4号: 油=4, 耗=1, 剩余=3+5=8
 *         到达0号: 油=8, 耗=2, 剩余=6+1=7
 *         到达1号: 油=7, 耗=3, 剩余=4+2=6
 *         到达2号: 油=6, 耗=4, 剩余=2+3=5
 *         到达3号: 油=5, 耗=5, 剩余=0, 到达起点!
 *
 * 贪心策略:
 *   1. 如果总油量 < 总消耗, 一定无解 (返回-1)
 *   2. 从起点开始, 维护当前剩余油量 tank
 *   3. 如果 tank < 0, 说明从当前起点到不了这里, 将起点设为 i+1
 *   4. 因为总油量 >= 总消耗, 一定存在唯一解
 *
 *   为什么 tank < 0 时起点设为 i+1 是正确的?
 *   从 start 到 i 都是负的, 说明 [start, i] 之间任何点都不能作为起点:
 *   - 因为从 start 到 j (start <= j <= i) 的油量一定 >= 从 j 到 i 的油量
 *   - 所以从 j 出发的初始油量比从 start 出发更少, 更不可能到达 i+1
 *   - 因此直接跳到 i+1 作为新起点
 *
 * ASCII图示:
 *
 *   gas  = [1, 2, 3, 4, 5]
 *   cost = [3, 4, 5, 1, 2]
 *   diff = [-2,-2,-2, 3, 3]
 *
 *   从start=0出发:
 *   i=0: tank = -2 < 0 -> start=1, tank=0
 *   i=1: tank = -2 < 0 -> start=2, tank=0
 *   i=2: tank = -2 < 0 -> start=3, tank=0
 *   i=3: tank = 3 >= 0  -> 继续
 *   i=4: tank = 3+3 = 6 >= 0 -> 继续
 *
 *   总和: -2-2-2+3+3 = 0 >= 0, 有解
 *   返回 start = 3
 *
 * 时间复杂度: O(n)
 * 空间复杂度: O(1)
 */
int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
    int n = gas.size();
    int totalTank = 0;  // 总油量 - 总消耗
    int curTank = 0;    // 当前从start出发的剩余油量
    int start = 0;      // 起点

    for (int i = 0; i < n; i++) {
        int diff = gas[i] - cost[i];
        totalTank += diff;
        curTank += diff;

        // 如果当前油量不够到达下一站
        if (curTank < 0) {
            start = i + 1; // 起点设为下一站
            curTank = 0;   // 重置油量
        }
    }

    // 总油量不够, 无解
    return totalTank >= 0 ? start : -1;
}


// ============================================================================
// 测试用例
// ============================================================================

void testCanJump() {
    cout << "========================================" << endl;
    cout << "题目1: 跳跃游戏 (LeetCode 55)" << endl;
    cout << "========================================" << endl;

    vector<int> nums1 = {2, 3, 1, 1, 4};
    cout << "输入: [2,3,1,1,4] => " << (canJump(nums1) ? "true" : "false")
         << " (期望: true)" << endl;

    vector<int> nums2 = {3, 2, 1, 0, 4};
    cout << "输入: [3,2,1,0,4] => " << (canJump(nums2) ? "true" : "false")
         << " (期望: false)" << endl;

    vector<int> nums3 = {0};
    cout << "输入: [0]          => " << (canJump(nums3) ? "true" : "false")
         << " (期望: true)" << endl;

    vector<int> nums4 = {2, 0, 0};
    cout << "输入: [2,0,0]      => " << (canJump(nums4) ? "true" : "false")
         << " (期望: true)" << endl;

    // 测试最少跳跃次数
    vector<int> nums5 = {2, 3, 1, 1, 4};
    cout << "最少跳跃次数 [2,3,1,1,4] => " << jump(nums5)
         << " (期望: 2)" << endl;

    vector<int> nums6 = {2, 3, 0, 1, 4};
    cout << "最少跳跃次数 [2,3,0,1,4] => " << jump(nums6)
         << " (期望: 2)" << endl;
    cout << endl;
}

void testMerge() {
    cout << "========================================" << endl;
    cout << "题目2: 合并区间 (LeetCode 56)" << endl;
    cout << "========================================" << endl;

    auto printIntervals = [](const vector<vector<int>>& intervals) {
        cout << "[";
        for (int i = 0; i < (int)intervals.size(); i++) {
            cout << "[" << intervals[i][0] << "," << intervals[i][1] << "]";
            if (i < (int)intervals.size() - 1) cout << ",";
        }
        cout << "]" << endl;
    };

    vector<vector<int>> intervals1 = {{1,3},{2,6},{8,10},{15,18}};
    cout << "输入: ";
    printIntervals(intervals1);
    cout << "输出: ";
    printIntervals(merge(intervals1));
    cout << "期望: [[1,6],[8,10],[15,18]]" << endl << endl;

    vector<vector<int>> intervals2 = {{1,4},{4,5}};
    cout << "输入: ";
    printIntervals(intervals2);
    cout << "输出: ";
    printIntervals(merge(intervals2));
    cout << "期望: [[1,5]]" << endl << endl;

    vector<vector<int>> intervals3 = {{1,4},{0,4}};
    cout << "输入: ";
    printIntervals(intervals3);
    cout << "输出: ";
    printIntervals(merge(intervals3));
    cout << "期望: [[0,4]]" << endl << endl;
}

void testPartitionLabels() {
    cout << "========================================" << endl;
    cout << "题目3: 划分字母区间 (LeetCode 763)" << endl;
    cout << "========================================" << endl;

    auto printVec = [](const vector<int>& v) {
        cout << "[";
        for (int i = 0; i < (int)v.size(); i++) {
            cout << v[i];
            if (i < (int)v.size() - 1) cout << ",";
        }
        cout << "]" << endl;
    };

    string s1 = "ababcbacadefegdehijhklij";
    cout << "输入: \"" << s1 << "\"" << endl;
    cout << "输出: ";
    printVec(partitionLabels(s1));
    cout << "期望: [9,7,8]" << endl << endl;

    string s2 = "eccbbbbdec";
    cout << "输入: \"" << s2 << "\"" << endl;
    cout << "输出: ";
    printVec(partitionLabels(s2));
    cout << "期望: [10]" << endl << endl;
}

void testMaxProfit() {
    cout << "========================================" << endl;
    cout << "题目4: 买卖股票的最佳时机 II (LeetCode 122)" << endl;
    cout << "========================================" << endl;

    vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    cout << "输入: [7,1,5,3,6,4]" << endl;
    cout << "贪心: " << maxProfitGreedy(prices1)
         << " (期望: 7)" << endl;
    cout << "DP:   " << maxProfitDP(prices1)
         << " (期望: 7)" << endl << endl;

    vector<int> prices2 = {1, 2, 3, 4, 5};
    cout << "输入: [1,2,3,4,5]" << endl;
    cout << "贪心: " << maxProfitGreedy(prices2)
         << " (期望: 4)" << endl;
    cout << "DP:   " << maxProfitDP(prices2)
         << " (期望: 4)" << endl << endl;

    vector<int> prices3 = {7, 6, 4, 3, 1};
    cout << "输入: [7,6,4,3,1]" << endl;
    cout << "贪心: " << maxProfitGreedy(prices3)
         << " (期望: 0)" << endl;
    cout << "DP:   " << maxProfitDP(prices3)
         << " (期望: 0)" << endl << endl;
}

void testCandy() {
    cout << "========================================" << endl;
    cout << "题目5: 分发糖果 (LeetCode 135)" << endl;
    cout << "========================================" << endl;

    vector<int> ratings1 = {1, 0, 2};
    cout << "输入: [1,0,2] => " << candy(ratings1)
         << " (期望: 5)" << endl;

    vector<int> ratings2 = {1, 2, 2};
    cout << "输入: [1,2,2] => " << candy(ratings2)
         << " (期望: 4)" << endl;

    vector<int> ratings3 = {1, 3, 2, 2, 1};
    cout << "输入: [1,3,2,2,1] => " << candy(ratings3)
         << " (期望: 7)" << endl;

    vector<int> ratings4 = {1, 2, 3, 4, 5};
    cout << "输入: [1,2,3,4,5] => " << candy(ratings4)
         << " (期望: 15)" << endl;

    vector<int> ratings5 = {5, 4, 3, 2, 1};
    cout << "输入: [5,4,3,2,1] => " << candy(ratings5)
         << " (期望: 15)" << endl;

    vector<int> ratings6 = {1, 0, 2, 3, 1, 2, 1};
    cout << "输入: [1,0,2,3,1,2,1] => " << candy(ratings6)
         << " (期望: 12)" << endl;
    cout << endl;
}

void testFindMinArrowShots() {
    cout << "========================================" << endl;
    cout << "题目6: 用最少数量的箭引爆气球 (LeetCode 452)" << endl;
    cout << "========================================" << endl;

    vector<vector<int>> points1 = {{10,16},{2,8},{1,6},{7,12}};
    cout << "输入: [[10,16],[2,8],[1,6],[7,12]]" << endl;
    cout << "输出: " << findMinArrowShots(points1) << " (期望: 2)" << endl;

    vector<vector<int>> points2 = {{1,2},{3,4},{5,6},{7,8}};
    cout << "输入: [[1,2],[3,4],[5,6],[7,8]]" << endl;
    cout << "输出: " << findMinArrowShots(points2) << " (期望: 4)" << endl;

    vector<vector<int>> points3 = {{1,2},{2,3},{3,4},{4,5}};
    cout << "输入: [[1,2],[2,3],[3,4],[4,5]]" << endl;
    cout << "输出: " << findMinArrowShots(points3) << " (期望: 2)" << endl;

    vector<vector<int>> points4 = {{1,2}};
    cout << "输入: [[1,2]]" << endl;
    cout << "输出: " << findMinArrowShots(points4) << " (期望: 1)" << endl;

    vector<vector<int>> points5 = {{-2147483646,-2147483645},{2147483646,2147483647}};
    cout << "输入: [[-2147483646,-2147483645],[2147483646,2147483647]]" << endl;
    cout << "输出: " << findMinArrowShots(points5) << " (期望: 2)" << endl;
    cout << endl;
}

void testEraseOverlapIntervals() {
    cout << "========================================" << endl;
    cout << "题目7: 无重叠区间 (LeetCode 435)" << endl;
    cout << "========================================" << endl;

    vector<vector<int>> intervals1 = {{1,2},{2,3},{3,4},{1,3}};
    cout << "输入: [[1,2],[2,3],[3,4],[1,3]]" << endl;
    cout << "输出: " << eraseOverlapIntervals(intervals1) << " (期望: 1)" << endl;

    vector<vector<int>> intervals2 = {{1,2},{1,2},{1,2}};
    cout << "输入: [[1,2],[1,2],[1,2]]" << endl;
    cout << "输出: " << eraseOverlapIntervals(intervals2) << " (期望: 2)" << endl;

    vector<vector<int>> intervals3 = {{1,2},{2,3}};
    cout << "输入: [[1,2],[2,3]]" << endl;
    cout << "输出: " << eraseOverlapIntervals(intervals3) << " (期望: 0)" << endl;

    vector<vector<int>> intervals4 = {{1,100},{11,22},{1,11},{2,12}};
    cout << "输入: [[1,100],[11,22],[1,11],[2,12]]" << endl;
    cout << "输出: " << eraseOverlapIntervals(intervals4) << " (期望: 2)" << endl;
    cout << endl;
}

void testMinMeetingRooms() {
    cout << "========================================" << endl;
    cout << "题目8: 会议室 II (LeetCode 253)" << endl;
    cout << "========================================" << endl;

    vector<vector<int>> intervals1 = {{0,30},{5,10},{15,20}};
    cout << "输入: [[0,30],[5,10],[15,20]]" << endl;
    cout << "堆方法: " << minMeetingRoomsHeap(intervals1)
         << " (期望: 2)" << endl;
    cout << "扫描线: " << minMeetingRoomsSweepLine(intervals1)
         << " (期望: 2)" << endl;

    vector<vector<int>> intervals2 = {{7,10},{2,4}};
    cout << "输入: [[7,10],[2,4]]" << endl;
    cout << "堆方法: " << minMeetingRoomsHeap(intervals2)
         << " (期望: 1)" << endl;
    cout << "扫描线: " << minMeetingRoomsSweepLine(intervals2)
         << " (期望: 1)" << endl;

    vector<vector<int>> intervals3 = {{1,5},{2,6},{3,7},{4,8}};
    cout << "输入: [[1,5],[2,6],[3,7],[4,8]]" << endl;
    cout << "堆方法: " << minMeetingRoomsHeap(intervals3)
         << " (期望: 4)" << endl;
    cout << "扫描线: " << minMeetingRoomsSweepLine(intervals3)
         << " (期望: 4)" << endl;

    vector<vector<int>> intervals4 = {{1,10},{2,3},{4,5},{6,7}};
    cout << "输入: [[1,10],[2,3],[4,5],[6,7]]" << endl;
    cout << "堆方法: " << minMeetingRoomsHeap(intervals4)
         << " (期望: 2)" << endl;
    cout << "扫描线: " << minMeetingRoomsSweepLine(intervals4)
         << " (期望: 2)" << endl;
    cout << endl;
}

void testLeastInterval() {
    cout << "========================================" << endl;
    cout << "题目9: 任务调度器 (LeetCode 621)" << endl;
    cout << "========================================" << endl;

    vector<char> tasks1 = {'A','A','A','B','B','B'};
    cout << "输入: [A,A,A,B,B,B], n=2" << endl;
    cout << "输出: " << leastInterval(tasks1, 2) << " (期望: 8)" << endl;

    vector<char> tasks2 = {'A','A','A','B','B','B'};
    cout << "输入: [A,A,A,B,B,B], n=0" << endl;
    cout << "输出: " << leastInterval(tasks2, 0) << " (期望: 6)" << endl;

    vector<char> tasks3 = {'A','A','A','A','A','A','B','C','D','E','F','G'};
    cout << "输入: [A,A,A,A,A,A,B,C,D,E,F,G], n=2" << endl;
    cout << "输出: " << leastInterval(tasks3, 2) << " (期望: 12)" << endl;

    vector<char> tasks4 = {'A','B','C','D','E','A','B','C','D','E'};
    cout << "输入: [A,B,C,D,E,A,B,C,D,E], n=4" << endl;
    cout << "输出: " << leastInterval(tasks4, 4) << " (期望: 10)" << endl;

    vector<char> tasks5 = {'A','A','A','B','B','B','C','C','D','D'};
    cout << "输入: [A,A,A,B,B,B,C,C,D,D], n=2" << endl;
    cout << "输出: " << leastInterval(tasks5, 2) << " (期望: 10)" << endl;
    cout << endl;
}

void testCanCompleteCircuit() {
    cout << "========================================" << endl;
    cout << "题目10: 加油站 (LeetCode 134)" << endl;
    cout << "========================================" << endl;

    vector<int> gas1 = {1,2,3,4,5}, cost1 = {3,4,5,1,2};
    cout << "输入: gas=[1,2,3,4,5], cost=[3,4,5,1,2]" << endl;
    cout << "输出: " << canCompleteCircuit(gas1, cost1) << " (期望: 3)" << endl;

    vector<int> gas2 = {2,3,4}, cost2 = {3,4,3};
    cout << "输入: gas=[2,3,4], cost=[3,4,3]" << endl;
    cout << "输出: " << canCompleteCircuit(gas2, cost2) << " (期望: -1)" << endl;

    vector<int> gas3 = {3,1,1}, cost3 = {1,2,2};
    cout << "输入: gas=[3,1,1], cost=[1,2,2]" << endl;
    cout << "输出: " << canCompleteCircuit(gas3, cost3) << " (期望: 0)" << endl;

    vector<int> gas4 = {5,1,2,3,4}, cost4 = {4,4,1,5,1};
    cout << "输入: gas=[5,1,2,3,4], cost=[4,4,1,5,1]" << endl;
    cout << "输出: " << canCompleteCircuit(gas4, cost4) << " (期望: 4)" << endl;

    vector<int> gas5 = {1,2,3,4,5,5,7}, cost5 = {2,3,4,3,9,6,2};
    cout << "输入: gas=[1,2,3,4,5,5,7], cost=[2,3,4,3,9,6,2]" << endl;
    cout << "输出: " << canCompleteCircuit(gas5, cost5) << " (期望: 4)" << endl;
    cout << endl;
}


// ============================================================================
// 主函数
// ============================================================================

int main() {
    cout << "********************************************" << endl;
    cout << "*       第11章 - 贪心算法 (Greedy)          *" << endl;
    cout << "********************************************" << endl << endl;

    testCanJump();
    testMerge();
    testPartitionLabels();
    testMaxProfit();
    testCandy();
    testFindMinArrowShots();
    testEraseOverlapIntervals();
    testMinMeetingRooms();
    testLeastInterval();
    testCanCompleteCircuit();

    cout << "=========================================" << endl;
    cout << "  所有测试用例执行完毕!" << endl;
    cout << "=========================================" << endl;

    return 0;
}
