/*
 * =====================================================================
 *   LeetCode 经典例题 -- 数组与哈希 (Array & Hash Table)
 * =====================================================================
 *
 *   本文件包含 12 道 LeetCode 经典数组与哈希表题目，每道题提供：
 *     - 中文题目描述（块注释）
 *     - LeetCode 编号与难度
 *     - 多种解法（暴力解 + 最优解）
 *     - 逐行中文注释
 *     - 复杂度分析
 *     - main() 和 main2() 中的测试用例
 *
 *   题目列表：
 *     1. 两数之和              (LeetCode 1,   Easy)
 *     2. 有效的字母异位词        (LeetCode 242, Easy)
 *     3. 最长连续序列            (LeetCode 128, Medium)
 *     4. 移动零                (LeetCode 283, Easy)
 *     5. 盛最多水的容器          (LeetCode 11,  Medium)
 *     6. 接雨水                (LeetCode 42,  Hard)
 *     7. 三数之和              (LeetCode 15,  Medium)
 *     8. 四数之和              (LeetCode 18,  Medium)
 *     9. 字母异位词分组          (LeetCode 49,  Medium)
 *    10. 除自身以外数组的乘积     (LeetCode 238, Medium)
 *    11. 轮转数组              (LeetCode 189, Medium)
 *    12. 缺失的第一个正数        (LeetCode 41,  Hard)
 *
 *   编译命令：g++ -std=c++11 -o 01_数组与哈希 01_数组与哈希.cpp
 * =====================================================================
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
using namespace std;

// =====================================================================
//  辅助函数：打印分隔线，用于区分不同题目输出
// =====================================================================
void printSection(const string& title) {
    cout << "\n========================================" << endl;
    cout << "  " << title << endl;
    cout << "========================================" << endl;
}

// =====================================================================
//  辅助函数：打印 vector 内容
// =====================================================================
void printVector(const vector<int>& vec) {
    cout << "[";
    for (int i = 0; i < (int)vec.size(); i++) {
        cout << vec[i];
        if (i < (int)vec.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// =====================================================================
//  题目 1：两数之和 (LeetCode 1, Easy)
// =====================================================================
/*
 * 给定一个整数数组 nums 和一个整数目标值 target，请你在该数组中找出和
 * 为目标值 target 的那两个整数，并返回它们的数组下标。
 *
 * 假设每种输入只会对应一个答案，且同一个元素不能使用两遍。
 * 你可以按任意顺序返回答案。
 *
 * 示例：
 *   输入：nums = [2, 7, 11, 15], target = 9
 *   输出：[0, 1]
 *   解释：因为 nums[0] + nums[1] == 9，所以返回 [0, 1]
 */

// --- 解法一：暴力枚举 ---
// 思路：双重循环遍历所有数对，检查是否满足 nums[i] + nums[j] == target
// 时间复杂度：O(n^2)，空间复杂度：O(1)
vector<int> twoSum_BruteForce(vector<int>& nums, int target) {
    int n = nums.size();                   // 获取数组长度
    for (int i = 0; i < n; i++) {          // 外层循环，固定第一个数
        for (int j = i + 1; j < n; j++) {  // 内层循环，遍历后面的数
            if (nums[i] + nums[j] == target) {  // 如果两数之和等于目标值
                return {i, j};             // 返回两个数的下标
            }
        }
    }
    return {};                             // 没找到则返回空数组
}

// --- 解法二：哈希表（最优解）---
// 思路：用哈希表存储已遍历的数及其下标。
//       对于当前数 nums[i]，查找哈希表中是否存在 target - nums[i]。
//       如果存在，说明找到了答案；否则将当前数存入哈希表。
// 为什么哈希表可行？因为我们只需要一对答案，且同一个元素不能用两次。
// 当我们查找 complement 时，它一定在 i 之前被插入过，所以不会重复使用同一元素。
// 时间复杂度：O(n)，空间复杂度：O(n)
vector<int> twoSum_HashMap(vector<int>& nums, int target) {
    unordered_map<int, int> hashTable;     // 哈希表：键=数值，值=下标
    for (int i = 0; i < (int)nums.size(); i++) {  // 遍历数组
        int complement = target - nums[i]; // 计算需要的"补数"
        // 在哈希表中查找 complement 是否存在
        if (hashTable.find(complement) != hashTable.end()) {
            return {hashTable[complement], i};  // 找到答案，返回两个下标
        }
        hashTable[nums[i]] = i;            // 将当前数和下标存入哈希表
    }
    return {};                             // 未找到答案
}

// =====================================================================
//  题目 2：有效的字母异位词 (LeetCode 242, Easy)
// =====================================================================
/*
 * 给定两个字符串 s 和 t，编写一个函数来判断 t 是否是 s 的字母异位词。
 * 字母异位词：由相同字母以不同顺序排列组成的字符串。
 *
 * 注意：若 s 和 t 中每个字符出现的次数都相同，则称 s 和 t 互为字母异位词。
 *
 * 示例：
 *   输入：s = "anagram", t = "nagaram"
 *   输出：true
 *   输入：s = "rat", t = "car"
 *   输出：false
 */

// --- 解法一：排序比较 ---
// 思路：将两个字符串排序后比较是否相等
// 时间复杂度：O(n log n)，空间复杂度：O(n)（排序需要额外空间）
bool isAnagram_Sort(string s, string t) {
    if (s.size() != t.size()) return false; // 长度不同，直接返回 false
    sort(s.begin(), s.end());               // 对 s 排序
    sort(t.begin(), t.end());               // 对 t 排序
    return s == t;                          // 排序后比较是否相等
}

// --- 解法二：计数数组（最优解）---
// 思路：因为只包含小写字母（共 26 个），可以用长度 26 的数组统计每个字母出现次数。
//       先遍历 s 统计每个字符出现次数 +1，再遍历 t 对应字符 -1。
//       如果最终所有计数都为 0，则说明互为异位词。
// 时间复杂度：O(n)，空间复杂度：O(1)（固定 26 个元素）
bool isAnagram_CountArray(string s, string t) {
    if (s.size() != t.size()) return false; // 长度不同，不可能是异位词
    int count[26] = {0};                    // 初始化 26 个字母的计数数组，全部为 0
    for (int i = 0; i < (int)s.size(); i++) {
        count[s[i] - 'a']++;               // s 中每个字符对应计数 +1
    }
    for (int i = 0; i < (int)t.size(); i++) {
        count[t[i] - 'a']--;               // t 中每个字符对应计数 -1
        if (count[t[i] - 'a'] < 0) {       // 如果某个计数变为负数，说明 t 中该字符多了
            return false;                   // 直接返回 false
        }
    }
    return true;                            // 所有计数都为 0，是异位词
}

// --- 解法三：哈希表 ---
// 思路：与计数数组类似，但使用 unordered_map 来处理任意 Unicode 字符
// 时间复杂度：O(n)，空间复杂度：O(n)
bool isAnagram_HashMap(string s, string t) {
    if (s.size() != t.size()) return false; // 长度不同，直接返回 false
    unordered_map<char, int> charCount;     // 哈希表记录每个字符出现次数
    for (char c : s) {                      // 遍历字符串 s
        charCount[c]++;                     // 对应字符计数 +1
    }
    for (char c : t) {                      // 遍历字符串 t
        charCount[c]--;                     // 对应字符计数 -1
        if (charCount[c] < 0) {             // 如果某个字符计数变为负数
            return false;                   // 说明 t 中该字符比 s 多，返回 false
        }
    }
    return true;                            // 所有字符计数平衡，是异位词
}

// =====================================================================
//  题目 3：最长连续序列 (LeetCode 128, Medium)
// =====================================================================
/*
 * 给定一个未排序的整数数组 nums，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。
 * 请你设计并实现时间复杂度为 O(n) 的算法。
 *
 * 示例：
 *   输入：nums = [100, 4, 200, 1, 3, 2]
 *   输出：4
 *   解释：最长数字连续序列是 [1, 2, 3, 4]，长度为 4。
 *
 *   输入：nums = [0,3,7,2,5,8,4,6,0,1]
 *   输出：9
 */

// --- 解法一：排序 ---
// 思路：先排序，再遍历找最长连续序列
// 时间复杂度：O(n log n)，空间复杂度：O(1)
int longestConsecutive_Sort(vector<int>& nums) {
    if (nums.empty()) return 0;             // 空数组返回 0
    sort(nums.begin(), nums.end());         // 排序
    int longest = 1;                        // 记录最长连续序列长度，至少为 1
    int currentLen = 1;                     // 当前连续序列长度
    for (int i = 1; i < (int)nums.size(); i++) {
        if (nums[i] == nums[i - 1]) {       // 跳过重复元素
            continue;
        }
        if (nums[i] == nums[i - 1] + 1) {   // 如果当前数比前一个数刚好大 1
            currentLen++;                    // 当前连续长度 +1
        } else {
            currentLen = 1;                 // 连续中断，重置为 1
        }
        longest = max(longest, currentLen); // 更新最长长度
    }
    return longest;
}

// --- 解法二：HashSet（最优解）---
// 思路：将所有数放入 HashSet 中，然后对每个数判断它是否是一个连续序列的起点。
//       关键技巧：只有当 num-1 不在集合中时，num 才是某个序列的起点。
//       这样可以避免重复计算，每个序列只会被起点触发一次遍历。
// 为什么只需要从序列起点开始计数？
//   因为如果 num-1 存在，那么 num 一定不是起点，num-1 才可能是起点。
//   从起点开始计数，每个数最多被访问两次（一次放入集合，一次遍历序列），
//   所以总时间复杂度为 O(n)。
// 时间复杂度：O(n)，空间复杂度：O(n)
int longestConsecutive_HashSet(vector<int>& nums) {
    if (nums.empty()) return 0;             // 空数组返回 0
    unordered_set<int> numSet(nums.begin(), nums.end()); // 将数组所有元素放入 HashSet
    int longest = 0;                        // 记录最长连续序列长度
    for (int num : numSet) {                // 遍历集合中每个数
        // 关键判断：只有 num-1 不在集合中时，num 才是某个连续序列的起点
        if (numSet.find(num - 1) == numSet.end()) {
            int currentNum = num;           // 从当前起点开始
            int currentLen = 1;             // 当前连续长度初始化为 1
            // 持续查找下一个连续数
            while (numSet.find(currentNum + 1) != numSet.end()) {
                currentNum++;               // 移动到下一个连续数
                currentLen++;               // 连续长度 +1
            }
            longest = max(longest, currentLen); // 更新最长长度
        }
    }
    return longest;
}

// =====================================================================
//  题目 4：移动零 (LeetCode 283, Easy)
// =====================================================================
/*
 * 给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，
 * 同时保持非零元素的相对顺序。
 *
 * 注意：必须在不复制数组的情况下原地操作。
 *
 * 示例：
 *   输入：nums = [0, 1, 0, 3, 12]
 *   输出：[1, 3, 12, 0, 0]
 */

// --- 双指针交换法 ---
// 思路：使用两个指针 slow 和 fast。
//       slow 指向下一个非零元素应该放置的位置。
//       fast 遍历整个数组，遇到非零元素就和 slow 位置交换。
// 为什么这样做有效？
//   slow 之前的位置已经全部是非零元素且保持原始相对顺序。
//   当 fast 遇到非零元素时，交换到 slow 位置不会打乱顺序。
// 时间复杂度：O(n)，空间复杂度：O(1)
void moveZeroes(vector<int>& nums) {
    int slow = 0;                           // slow 指针：下一个非零元素应放的位置
    for (int fast = 0; fast < (int)nums.size(); fast++) { // fast 指针：遍历整个数组
        if (nums[fast] != 0) {              // 如果 fast 指向的元素不为 0
            swap(nums[slow], nums[fast]);   // 将非零元素交换到 slow 位置
            slow++;                         // slow 前移，指向下一个空位
        }
        // 如果 nums[fast] 为 0，fast 继续前进，slow 不动
        // 这样 0 就会被"跳过"，最终留在数组末尾
    }
}

// =====================================================================
//  题目 5：盛最多水的容器 (LeetCode 11, Medium)
// =====================================================================
/*
 * 给定 n 个非负整数 a1, a2, ..., an，每个数代表坐标中的一个点 (i, ai)。
 * 画 n 条垂直线，使得线 i 的两个端点分别为 (i, ai) 和 (i, 0)。
 * 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
 *
 * 返回可以储存的最大水量（容器中不能倾斜）。
 *
 * 示例：
 *   输入：height = [1,8,6,2,5,4,8,3,7]
 *   输出：49
 *   解释：选择下标 1 和 8 的线段，面积 = min(8,7) * (8-1) = 7 * 7 = 49
 */

// --- 解法一：暴力枚举 ---
// 思路：遍历所有可能的线段对，计算面积取最大值
// 面积 = min(height[left], height[right]) * (right - left)
// 时间复杂度：O(n^2)，空间复杂度：O(1)
int maxArea_BruteForce(vector<int>& height) {
    int maxWater = 0;                       // 记录最大水量
    int n = height.size();                  // 获取数组长度
    for (int i = 0; i < n; i++) {           // 枚举左边界
        for (int j = i + 1; j < n; j++) {   // 枚举右边界
            // 面积 = 较短线段的高度 * 两线段之间的距离
            int area = min(height[i], height[j]) * (j - i);
            maxWater = max(maxWater, area); // 更新最大面积
        }
    }
    return maxWater;
}

// --- 解法二：双指针（最优解）---
// 思路：使用左右两个指针分别指向数组两端。
//       面积 = min(height[left], height[right]) * (right - left)
//       每次移动较短的那条线的指针。
// 为什么移动较短的线？
//   面积受限于较短的线（木桶原理）。如果移动较长的线，宽度减小，
//   而高度不可能增加（受限于当前较短线），面积一定不会增大。
//   移动较短线，虽然宽度减小，但高度有可能增大，从而找到更大面积。
// 时间复杂度：O(n)，空间复杂度：O(1)
int maxArea_TwoPointer(vector<int>& height) {
    int left = 0;                           // 左指针指向数组开头
    int right = height.size() - 1;          // 右指针指向数组末尾
    int maxWater = 0;                       // 记录最大水量
    while (left < right) {                  // 当左右指针未相遇时
        // 计算当前面积：较小高度 * 宽度
        int area = min(height[left], height[right]) * (right - left);
        maxWater = max(maxWater, area);     // 更新最大面积
        // 移动较短那一侧的指针
        if (height[left] < height[right]) {
            left++;                         // 左边较短，左指针右移
        } else {
            right--;                        // 右边较短，右指针左移
        }
    }
    return maxWater;
}

// =====================================================================
//  题目 6：接雨水 (LeetCode 42, Hard)
// =====================================================================
/*
 * 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，
 * 下雨之后能接多少雨水。
 *
 * 示例：
 *   输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
 *   输出：6
 *
 * ASCII 图示（柱子用 █ 表示，雨水用 ~ 表示）：
 *
 *   位置:  0  1  2  3  4  5  6  7  8  9 10 11
 *
 *              █              █
 *         █    █    █     █   █  █     █
 *         █  ~ █  ~ █  █  █   █  █  ~  █
 *         █  ~ █  ~ █  ~ █   █  █  ~  █
 *   高度: 0  1  0  2  1  0  1  3  2  1  2  1
 *
 *   每个位置能接的雨水量 = min(左边最高, 右边最高) - 当前柱子高度
 *   位置 2: min(1,2)-0 = 1  |  位置 4: min(2,3)-1 = 1
 *   位置 5: min(2,3)-0 = 2  |  位置 9: min(3,2)-1 = 1
 *   总计: 1+1+2+1+1 = 6（验证结果）
 */

// --- 解法一：暴力法 ---
// 思路：对每个位置，分别向左和向右扫描找最大高度。
//       每个位置的雨水量 = min(左边最高, 右边最高) - height[i]
// 时间复杂度：O(n^2)，空间复杂度：O(1)
int trap_BruteForce(vector<int>& height) {
    int n = height.size();                  // 获取数组长度
    int totalWater = 0;                     // 记录总雨水量
    for (int i = 1; i < n - 1; i++) {       // 从第二个位置遍历到倒数第二个（两端无法积水）
        // 向左扫描找最高柱子
        int leftMax = 0;
        for (int j = i; j >= 0; j--) {
            leftMax = max(leftMax, height[j]);
        }
        // 向右扫描找最高柱子
        int rightMax = 0;
        for (int j = i; j < n; j++) {
            rightMax = max(rightMax, height[j]);
        }
        // 当前位置能接的雨水 = 两侧较低的最高柱子 - 当前柱子高度
        totalWater += min(leftMax, rightMax) - height[i];
    }
    return totalWater;
}

// --- 解法二：动态规划（预计算 leftMax 和 rightMax）---
// 思路：提前计算每个位置的左边最大值和右边最大值，避免重复扫描。
//       leftMax[i] 表示位置 i 左边（含 i）的最高柱子
//       rightMax[i] 表示位置 i 右边（含 i）的最高柱子
//       每个位置的雨水量 = min(leftMax[i], rightMax[i]) - height[i]
// 时间复杂度：O(n)，空间复杂度：O(n)
int trap_DP(vector<int>& height) {
    int n = height.size();                  // 获取数组长度
    if (n <= 2) return 0;                   // 不足 3 根柱子无法积水
    vector<int> leftMax(n, 0);              // leftMax[i]: 位置 i 左侧（含自身）最高柱子
    vector<int> rightMax(n, 0);             // rightMax[i]: 位置 i 右侧（含自身）最高柱子
    // 从左往右填充 leftMax
    leftMax[0] = height[0];                 // 第一个位置的左边最大值就是它自己
    for (int i = 1; i < n; i++) {
        leftMax[i] = max(leftMax[i - 1], height[i]); // 递推：取前一个位置的最大值和当前高度的较大者
    }
    // 从右往左填充 rightMax
    rightMax[n - 1] = height[n - 1];        // 最后一个位置的右边最大值就是它自己
    for (int i = n - 2; i >= 0; i--) {
        rightMax[i] = max(rightMax[i + 1], height[i]); // 递推：取后一个位置的最大值和当前高度的较大者
    }
    // 计算每个位置能接的雨水量并累加
    int totalWater = 0;
    for (int i = 1; i < n - 1; i++) {       // 两端不能积水，跳过
        totalWater += min(leftMax[i], rightMax[i]) - height[i];
    }
    return totalWater;
}

// --- 解法三：双指针（最优解，O(1) 空间）---
// 思路：使用左右双指针，从两端向中间逼近。
//       维护 leftMax 和 rightMax 分别记录左右两侧的最高柱子。
//       每次移动较小一侧的指针，因为该侧的积水由较小侧决定。
// 为什么可以省去数组？
//   当 leftMax < rightMax 时，left 位置的积水只取决于 leftMax（右边保证有更高的）。
//   同理，right 位置的积水只取决于 rightMax。
//   所以我们不需要完整数组，只需要维护两个变量即可。
// 时间复杂度：O(n)，空间复杂度：O(1)
int trap_TwoPointer(vector<int>& height) {
    int left = 0;                           // 左指针
    int right = height.size() - 1;          // 右指针
    int leftMax = 0;                        // 左侧最高柱子高度
    int rightMax = 0;                       // 右侧最高柱子高度
    int totalWater = 0;                     // 总雨水量
    while (left < right) {                  // 当左右指针未相遇时
        if (height[left] < height[right]) { // 左边较短，处理 left 位置
            // 如果当前柱子比 leftMax 高，更新 leftMax（此处无法积水）
            if (height[left] >= leftMax) {
                leftMax = height[left];
            } else {
                // 否则该位置可以积水，水量 = leftMax - 当前高度
                totalWater += leftMax - height[left];
            }
            left++;                         // 左指针右移
        } else {                            // 右边较短（或相等），处理 right 位置
            // 如果当前柱子比 rightMax 高，更新 rightMax（此处无法积水）
            if (height[right] >= rightMax) {
                rightMax = height[right];
            } else {
                // 否则该位置可以积水，水量 = rightMax - 当前高度
                totalWater += rightMax - height[right];
            }
            right--;                        // 右指针左移
        }
    }
    return totalWater;
}

// =====================================================================
//  题目 7：三数之和 (LeetCode 15, Medium)
// =====================================================================
/*
 * 给你一个整数数组 nums，判断是否存在三元组 [nums[i], nums[j], nums[k]]
 * 满足 i != j、i != k 且 j != k，同时还满足 nums[i] + nums[j] + nums[k] == 0。
 *
 * 请你返回所有和为 0 且不重复的三元组。
 *
 * 注意：答案中不可以包含重复的三元组。
 *
 * 示例：
 *   输入：nums = [-1, 0, 1, 2, -1, -4]
 *   输出：[[-1, -1, 2], [-1, 0, 1]]
 *
 *   输入：nums = [0, 1, 1]
 *   输出：[]
 *
 *   输入：nums = [0, 0, 0]
 *   输出：[[0, 0, 0]]
 */

// --- 解法一：暴力枚举 ---
// 思路：三重循环遍历所有三元组，检查和是否为 0，用 set 去重
// 时间复杂度：O(n^3)，空间复杂度：O(n)（去重用的 set）
vector<vector<int>> threeSum_BruteForce(vector<int>& nums) {
    int n = nums.size();                        // 获取数组长度
    set<vector<int>> resultSet;                 // 用 set 自动去重（set 内元素有序）
    for (int i = 0; i < n; i++) {               // 第一个数
        for (int j = i + 1; j < n; j++) {       // 第二个数（不能和 i 相同）
            for (int k = j + 1; k < n; k++) {   // 第三个数（不能和 i、j 相同）
                if (nums[i] + nums[j] + nums[k] == 0) {  // 三数之和为 0
                    vector<int> triplet = {nums[i], nums[j], nums[k]};
                    sort(triplet.begin(), triplet.end()); // 排序保证去重效果
                    resultSet.insert(triplet);   // 插入 set 去重
                }
            }
        }
    }
    return vector<vector<int>>(resultSet.begin(), resultSet.end()); // 转为 vector 返回
}

// --- 解法二：排序 + 双指针（最优解）---
// 思路：
//   1. 先对数组排序，这样可以利用双指针技巧，也方便跳过重复元素
//   2. 固定第一个数 nums[i]，然后在 i+1 到 n-1 的范围内用双指针找另外两个数
//   3. 左指针 left = i+1，右指针 right = n-1
//   4. 计算 sum = nums[i] + nums[left] + nums[right]
//      - sum == 0：找到答案，记录后同时移动 left 和 right，并跳过重复元素
//      - sum < 0：说明总和太小，left 右移使总和增大
//      - sum > 0：说明总和太大，right 左移使总和减小
//   5. 每次固定 nums[i] 后，跳过相同的 nums[i] 以避免重复三元组
//
// 为什么排序 + 双指针能去重？
//   - 排序后，相同的数会相邻，跳过相邻相同数即可避免重复
//   - 双指针从两端向中间逼近，不会漏掉任何有效组合
//
// 时间复杂度：O(n^2)（排序 O(n log n) + 双指针扫描 O(n^2)）
// 空间复杂度：O(1)（不考虑输出数组，仅用常数额外空间）
vector<vector<int>> threeSum_TwoPointer(vector<int>& nums) {
    vector<vector<int>> result;                 // 存储结果
    int n = nums.size();
    if (n < 3) return result;                   // 不足 3 个数，无法组成三元组

    sort(nums.begin(), nums.end());             // 排序，便于双指针和去重

    for (int i = 0; i < n - 2; i++) {           // 固定第一个数（只需遍历到 n-3）
        // 剪枝：如果当前数已经大于 0，后面都更大，三数之和不可能为 0
        if (nums[i] > 0) break;

        // 跳过重复的第一个数，避免产生重复三元组
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        int left = i + 1;                       // 左指针：从 i 的下一个位置开始
        int right = n - 1;                      // 右指针：从数组末尾开始

        while (left < right) {                  // 双指针未相遇时继续
            int sum = nums[i] + nums[left] + nums[right]; // 计算三数之和

            if (sum == 0) {                     // 找到一个有效三元组
                result.push_back({nums[i], nums[left], nums[right]});
                // 跳过重复的 left 元素
                while (left < right && nums[left] == nums[left + 1]) left++;
                // 跳过重复的 right 元素
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++;                         // 移动左指针
                right--;                        // 移动右指针
            } else if (sum < 0) {               // 总和太小，需要增大
                left++;                         // 左指针右移（取更大的数）
            } else {                            // sum > 0，总和太大，需要减小
                right--;                        // 右指针左移（取更小的数）
            }
        }
    }
    return result;
}

// =====================================================================
//  题目 8：四数之和 (LeetCode 18, Medium)
// =====================================================================
/*
 * 给你一个由 n 个整数组成的数组 nums，和一个目标值 target。
 * 请你找出并返回满足下述全部条件且不重复的四元组 [nums[a], nums[b], nums[c], nums[d]]：
 *
 *   - 0 <= a, b, c, d < n
 *   - a、b、c 和 d 互不相同
 *   - nums[a] + nums[b] + nums[c] + nums[d] == target
 *
 * 注意：答案中不可以包含重复的四元组。
 *
 * 示例：
 *   输入：nums = [1, 0, -1, 0, -2, 2], target = 0
 *   输出：[[-2, -1, 1, 2], [-2, 0, 0, 2], [-1, 0, 0, 1]]
 *
 *   输入：nums = [2, 2, 2, 2, 2], target = 8
 *   输出：[[2, 2, 2, 2]]
 */

// --- 解法：排序 + 双层循环 + 双指针 ---
// 思路：三数之和的扩展，外面多套一层循环
//   1. 先排序
//   2. 外层两层循环固定前两个数 nums[i] 和 nums[j]
//   3. 内层用双指针 left = j+1, right = n-1 找后两个数
//   4. 四数之和与 target 比较，移动双指针
//   5. 每层循环都要跳过重复元素
//
// 与三数之和的区别：
//   - 多了一层循环（从 O(n^2) 变为 O(n^3)）
//   - 比较对象从 0 变为 target
//   - 剪枝条件更复杂（不能简单地判断 nums[i] > target，因为 target 可能为负数）
//
// 时间复杂度：O(n^3)
// 空间复杂度：O(1)（不考虑输出数组）
vector<vector<int>> fourSum(vector<int>& nums, int target) {
    vector<vector<int>> result;                 // 存储结果
    int n = nums.size();
    if (n < 4) return result;                   // 不足 4 个数，无法组成四元组

    sort(nums.begin(), nums.end());             // 排序

    for (int i = 0; i < n - 3; i++) {           // 固定第一个数
        // 跳过重复的第一个数
        if (i > 0 && nums[i] == nums[i - 1]) continue;

        // 剪枝优化：如果 nums[i] 加上后面三个最小的数都大于 target，后面不可能有解
        if ((long long)nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) break;

        // 剪枝优化：如果 nums[i] 加上后面三个最大的数都小于 target，当前 i 无解
        if ((long long)nums[i] + nums[n - 1] + nums[n - 2] + nums[n - 3] < target) continue;

        for (int j = i + 1; j < n - 2; j++) {   // 固定第二个数
            // 跳过重复的第二个数
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;

            // 剪枝优化：类似外层循环的剪枝
            if ((long long)nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target) break;
            if ((long long)nums[i] + nums[j] + nums[n - 1] + nums[n - 2] < target) continue;

            int left = j + 1;                   // 左指针
            int right = n - 1;                  // 右指针

            while (left < right) {              // 双指针扫描
                // 用 long long 防止整数溢出
                long long sum = (long long)nums[i] + nums[j] + nums[left] + nums[right];

                if (sum == target) {            // 找到有效四元组
                    result.push_back({nums[i], nums[j], nums[left], nums[right]});
                    // 跳过重复的 left 元素
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    // 跳过重复的 right 元素
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < target) {      // 总和太小
                    left++;                     // 左指针右移
                } else {                        // sum > target，总和太大
                    right--;                    // 右指针左移
                }
            }
        }
    }
    return result;
}

// =====================================================================
//  题目 9：字母异位词分组 (LeetCode 49, Medium)
// =====================================================================
/*
 * 给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序返回结果列表。
 * 字母异位词 是由重新排列源单词的所有字母得到的一个新单词。
 *
 * 示例：
 *   输入：strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
 *   输出：[["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]
 *
 *   输入：strs = [""]
 *   输出：[[""]]
 *
 *   输入：strs = ["a"]
 *   输出：[["a"]]
 */

// --- 解法一：排序作为键 ---
// 思路：将每个字符串排序后的结果作为哈希表的键。
//       字母异位词排序后一定相同（如 "eat" 和 "tea" 排序后都是 "aet"）。
//       将具有相同排序结果的字符串归入同一组。
// 时间复杂度：O(n * k log k)，其中 n 是字符串数量，k 是字符串最大长度
// 空间复杂度：O(n * k)（存储所有字符串）
vector<vector<string>> groupAnagrams_SortKey(vector<string>& strs) {
    unordered_map<string, vector<string>> groups; // 哈希表：键=排序后的字符串，值=原字符串列表
    for (const string& s : strs) {              // 遍历每个字符串
        string sortedStr = s;                   // 复制一份
        sort(sortedStr.begin(), sortedStr.end()); // 排序得到键
        groups[sortedStr].push_back(s);         // 将原字符串加入对应分组
    }
    vector<vector<string>> result;              // 结果数组
    for (auto& pair : groups) {                 // 遍历哈希表
        result.push_back(pair.second);          // 将每个分组加入结果
    }
    return result;
}

// --- 解法二：计数作为键（最优解）---
// 思路：用长度为 26 的计数数组作为哈希表的键（"字符指纹"）。
//       每个字符串的字符指纹唯一标识了一组字母异位词。
//       例如 "eat" 的指纹是 [1,0,0,0,1,0,...,1,...,0]（a=1, e=1, t=1）
//       比排序方法更高效，因为构造指纹是 O(k) 而排序是 O(k log k)。
//
// 如何将 26 个计数值转为字符串作为哈希键？
//   将每个计数值用分隔符 '#' 连接，如 "1#0#0#0#1#0#...#1#..."
//   使用分隔符是为了防止 "ab" 和 "ba" 的计数混淆（如 "11" 无法区分）
//
// 时间复杂度：O(n * k)，其中 n 是字符串数量，k 是字符串最大长度
// 空间复杂度：O(n * k)
vector<vector<string>> groupAnagrams_CountKey(vector<string>& strs) {
    unordered_map<string, vector<string>> groups; // 哈希表：键=字符指纹，值=原字符串列表
    for (const string& s : strs) {              // 遍历每个字符串
        int count[26] = {0};                    // 26 个字母的计数数组
        for (char c : s) {                      // 统计每个字符出现次数
            count[c - 'a']++;
        }
        // 将计数数组转为字符串键，用 '#' 分隔避免歧义
        string key = "";
        for (int i = 0; i < 26; i++) {
            key += to_string(count[i]) + "#";
        }
        groups[key].push_back(s);               // 将原字符串加入对应分组
    }
    vector<vector<string>> result;              // 结果数组
    for (auto& pair : groups) {                 // 遍历哈希表
        result.push_back(pair.second);          // 将每个分组加入结果
    }
    return result;
}

// =====================================================================
//  题目 10：除自身以外数组的乘积 (LeetCode 238, Medium)
// =====================================================================
/*
 * 给你一个整数数组 nums，返回数组 answer，其中 answer[i] 等于 nums 中
 * 除 nums[i] 之外其余各元素的乘积。
 *
 * 题目保证 nums 中任意元素的全部前缀元素和后缀的乘积都在 32 位整数范围内。
 * 请不要使用除法，且在 O(n) 时间复杂度内完成此题。
 *
 * 示例：
 *   输入：nums = [1, 2, 3, 4]
 *   输出：[24, 12, 8, 6]
 *   解释：
 *     answer[0] = 2 * 3 * 4 = 24
 *     answer[1] = 1 * 3 * 4 = 12
 *     answer[2] = 1 * 2 * 4 = 8
 *     answer[3] = 1 * 2 * 3 = 6
 *
 *   输入：nums = [-1, 1, 0, -3, 3]
 *   输出：[0, 0, 9, 0, 0]
 */

// --- 解法一：左右前缀积数组 ---
// 思路：answer[i] = 左边所有数的乘积 * 右边所有数的乘积
//       即 answer[i] = leftProduct[i] * rightProduct[i]
//       leftProduct[i] = nums[0] * nums[1] * ... * nums[i-1]（i 左边所有数的积）
//       rightProduct[i] = nums[i+1] * nums[i+2] * ... * nums[n-1]（i 右边所有数的积）
//
// 构造 leftProduct：从左往右累乘
//   leftProduct[0] = 1（第一个数左边没有数，乘积定义为 1）
//   leftProduct[i] = leftProduct[i-1] * nums[i-1]
//
// 构造 rightProduct：从右往左累乘
//   rightProduct[n-1] = 1（最后一个数右边没有数，乘积定义为 1）
//   rightProduct[i] = rightProduct[i+1] * nums[i+1]
//
// 时间复杂度：O(n)，空间复杂度：O(n)（使用了两个额外数组）
vector<int> productExceptSelf_TwoArrays(vector<int>& nums) {
    int n = nums.size();                        // 获取数组长度
    vector<int> leftProduct(n, 0);              // 左前缀积数组
    vector<int> rightProduct(n, 0);             // 右前缀积数组
    vector<int> answer(n, 0);                   // 结果数组

    // 构造左前缀积数组
    leftProduct[0] = 1;                         // 第一个元素左边没有数，乘积为 1
    for (int i = 1; i < n; i++) {
        leftProduct[i] = leftProduct[i - 1] * nums[i - 1]; // 累乘左边的数
    }

    // 构造右前缀积数组
    rightProduct[n - 1] = 1;                    // 最后一个元素右边没有数，乘积为 1
    for (int i = n - 2; i >= 0; i--) {
        rightProduct[i] = rightProduct[i + 1] * nums[i + 1]; // 累乘右边的数
    }

    // 结果 = 左前缀积 * 右前缀积
    for (int i = 0; i < n; i++) {
        answer[i] = leftProduct[i] * rightProduct[i];
    }

    return answer;
}

// --- 解法二：O(1) 空间优化（最优解）---
// 思路：先用 answer 数组存储左前缀积（复用输出数组）。
//       然后从右往左遍历，用一个变量 rightProduct 维护右侧累乘积，
//       直接将 rightProduct 乘到 answer[i] 上。
//       这样就不需要额外的 rightProduct 数组了。
//
// 为什么可以省掉 rightProduct 数组？
//   因为 rightProduct 是从右往左逐个计算的，我们只需要一个变量来追踪"当前右侧乘积"。
//   每次处理完 answer[i] 后，将 nums[i] 乘入 rightProduct，供下一次使用。
//
// 时间复杂度：O(n)，空间复杂度：O(1)（不考虑输出数组）
vector<int> productExceptSelf_Optimal(vector<int>& nums) {
    int n = nums.size();                        // 获取数组长度
    vector<int> answer(n, 1);                   // 初始化结果数组全部为 1

    // 第一次遍历：answer[i] 存储 i 左边所有数的乘积
    // answer[0] = 1（左边没有数）
    // answer[i] = answer[i-1] * nums[i-1]
    for (int i = 1; i < n; i++) {
        answer[i] = answer[i - 1] * nums[i - 1];
    }

    // 第二次遍历：从右往左，用 rightProduct 变量累乘右侧的数
    int rightProduct = 1;                       // 右侧乘积，初始为 1
    for (int i = n - 1; i >= 0; i--) {
        answer[i] = answer[i] * rightProduct;   // 左前缀积 * 右前缀积
        rightProduct *= nums[i];                // 将当前数乘入右侧乘积，供下一轮使用
    }

    return answer;
}

// =====================================================================
//  题目 11：轮转数组 (LeetCode 189, Medium)
// =====================================================================
/*
 * 给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置，其中 k 是非负数。
 *
 * 示例：
 *   输入：nums = [1, 2, 3, 4, 5, 6, 7], k = 3
 *   输出：[5, 6, 7, 1, 2, 3, 4]
 *   解释：
 *     向右轮转 1 步: [7, 1, 2, 3, 4, 5, 6]
 *     向右轮转 2 步: [6, 7, 1, 2, 3, 4, 5]
 *     向右轮转 3 步: [5, 6, 7, 1, 2, 3, 4]
 *
 *   输入：nums = [-1, -100, 3, 99], k = 2
 *   输出：[3, 99, -1, -100]
 */

// --- 解法一：使用额外数组 ---
// 思路：创建一个新数组，将每个元素放到正确的新位置。
//       原来在位置 i 的元素，轮转后应该在位置 (i + k) % n。
//       或者说，新位置 j 的元素来自原位置 (j - k + n) % n。
//
// 为什么取模？因为轮转会"绕回"数组开头，取模保证下标在 [0, n-1] 范围内。
//
// 时间复杂度：O(n)，空间复杂度：O(n)
void rotate_ExtraArray(vector<int>& nums, int k) {
    int n = nums.size();                        // 获取数组长度
    k = k % n;                                  // k 可能大于 n，取模得到实际轮转步数
    if (k == 0) return;                         // 轮转 0 步，不需要操作
    vector<int> rotated(n);                     // 新数组存储轮转后的结果
    for (int i = 0; i < n; i++) {
        // 原位置 i 的元素 -> 新位置 (i + k) % n
        rotated[(i + k) % n] = nums[i];
    }
    nums = rotated;                             // 将结果复制回原数组
}

// --- 解法二：三次翻转法（最优解）---
// 思路：这是一个非常巧妙的原地算法，分三步完成：
//   1. 先将整个数组翻转：[1,2,3,4,5,6,7] -> [7,6,5,4,3,2,1]
//   2. 翻转前 k 个元素：  [7,6,5,4,3,2,1] -> [5,6,7,4,3,2,1]
//   3. 翻转后 n-k 个元素：[5,6,7,4,3,2,1] -> [5,6,7,1,2,3,4]
//
// 为什么三次翻转能得到正确结果？
//   向右轮转 k 步，相当于把后 k 个元素移到前面。
//   整体翻转后，后 k 个元素到了前面但顺序反了，前 n-k 个元素到了后面但顺序也反了。
//   分别翻转两部分，就恢复了正确顺序。
//
// 时间复杂度：O(n)，空间复杂度：O(1)
void rotate_Reverse(vector<int>& nums, int k) {
    int n = nums.size();                        // 获取数组长度
    k = k % n;                                  // 取模得到实际轮转步数
    if (k == 0) return;                         // 轮转 0 步，不需要操作

    // 辅助 lambda：翻转数组中 [left, right] 区间的元素
    auto reverseRange = [&](int left, int right) {
        while (left < right) {                  // 双指针从两端向中间逼近
            swap(nums[left], nums[right]);      // 交换两端元素
            left++;                             // 左指针右移
            right--;                            // 右指针左移
        }
    };

    reverseRange(0, n - 1);                     // 第一步：翻转整个数组
    reverseRange(0, k - 1);                     // 第二步：翻转前 k 个元素
    reverseRange(k, n - 1);                     // 第三步：翻转后 n-k 个元素
}

// =====================================================================
//  题目 12：缺失的第一个正数 (LeetCode 41, Hard)
// =====================================================================
/*
 * 给你一个未排序的整数数组 nums，请你找出其中没有出现的最小的正整数。
 * 请你实现时间复杂度为 O(n)、空间复杂度为 O(1) 的算法。
 *
 * 正整数的范围是 [1, 2, 3, ...]，不包括 0 和负数。
 *
 * 示例：
 *   输入：nums = [1, 2, 0]
 *   输出：3
 *   解释：范围 [1, 2] 中的数都在，缺失的最小正整数是 3
 *
 *   输入：nums = [3, 4, -1, 1]
 *   输出：2
 *   解释：1 在，但 2 不在，所以缺失的最小正整数是 2
 *
 *   输入：nums = [7, 8, 9, 11, 12]
 *   输出：1
 *   解释：最小的正整数 1 就不在数组中
 */

// --- 解法一：HashSet ---
// 思路：将所有数放入 HashSet 中，然后从 1 开始逐个检查是否存在于集合中。
//       第一个不存在的正整数就是答案。
//       上界分析：长度为 n 的数组，最多包含 [1, n] 这 n 个正整数，
//       所以缺失的第一个正整数一定在 [1, n+1] 范围内。
// 时间复杂度：O(n)，空间复杂度：O(n)
int firstMissingPositive_HashSet(vector<int>& nums) {
    int n = nums.size();                        // 获取数组长度
    unordered_set<int> numSet(nums.begin(), nums.end()); // 将所有数放入 HashSet

    // 从 1 开始逐个检查，最多检查到 n+1
    for (int i = 1; i <= n + 1; i++) {
        if (numSet.find(i) == numSet.end()) {   // 如果 i 不在集合中
            return i;                           // i 就是缺失的最小正整数
        }
    }
    return n + 1;                               // 理论上不会到这里
}

// --- 解法二：原地哈希 / 索引作为哈希（最优解，O(1) 空间）---
// 思路：
//   核心思想：对于长度为 n 的数组，答案一定在 [1, n+1] 范围内。
//   如果数组中包含 1~n 的所有数，答案就是 n+1；否则就是某个缺失的数。
//
//   利用数组本身作为"哈希表"：将数值 x 放到下标 x-1 的位置上。
//   即：如果 nums[i] = x 且 1 <= x <= n，则将 nums[x-1] 放到正确位置。
//
//   具体做法：
//   1. 遍历数组，对于每个位置 i：
//      当 nums[i] 在 [1, n] 范围内，且 nums[i] 不在正确位置（nums[nums[i]-1] != nums[i]），
//      就交换 nums[i] 和 nums[nums[i]-1]，将 nums[i] 放到它应该在的位置。
//      重复交换直到 nums[i] 不在范围内或已在正确位置。
//   2. 再次遍历数组，第一个 nums[i] != i+1 的位置 i，说明 i+1 缺失。
//   3. 如果所有位置都正确，说明 1~n 都存在，返回 n+1。
//
// 为什么交换一定能在有限步内结束？
//   每次交换至少把一个数放到正确位置，最多交换 n 次。
//   所以虽然有 while 循环，但总的交换次数是 O(n)。
//
// 时间复杂度：O(n)，空间复杂度：O(1)
int firstMissingPositive_InPlaceHash(vector<int>& nums) {
    int n = nums.size();                        // 获取数组长度

    // 第一步：将每个数放到它应该在的位置
    for (int i = 0; i < n; i++) {
        // 循环条件：
        //   nums[i] 在 [1, n] 范围内（正整数且不超过数组长度）
        //   nums[i] 不在正确位置（nums[nums[i]-1] 应该等于 nums[i]）
        while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
            swap(nums[i], nums[nums[i] - 1]);   // 将 nums[i] 交换到正确位置
        }
    }

    // 第二步：找到第一个不在正确位置的数
    for (int i = 0; i < n; i++) {
        if (nums[i] != i + 1) {                 // 位置 i 应该存放 i+1
            return i + 1;                       // i+1 就是缺失的最小正整数
        }
    }

    // 所有位置都正确，说明 1~n 都存在，返回 n+1
    return n + 1;
}

// =====================================================================
//  main 函数：运行所有测试用例
// =====================================================================
int main() {

    // =================================================================
    // 测试题目 1：两数之和
    // =================================================================
    printSection("题目 1: 两数之和 (LeetCode 1, Easy)");

    vector<int> nums1 = {2, 7, 11, 15};
    int target1 = 9;
    cout << "输入: nums = [2, 7, 11, 15], target = 9" << endl;
    cout << "暴力解法输出: ";
    printVector(twoSum_BruteForce(nums1, target1));
    cout << "哈希表解法输出: ";
    printVector(twoSum_HashMap(nums1, target1));
    // 预期输出: [0, 1]

    cout << endl;
    vector<int> nums1b = {3, 2, 4};
    int target1b = 6;
    cout << "输入: nums = [3, 2, 4], target = 6" << endl;
    cout << "暴力解法输出: ";
    printVector(twoSum_BruteForce(nums1b, target1b));
    cout << "哈希表解法输出: ";
    printVector(twoSum_HashMap(nums1b, target1b));
    // 预期输出: [1, 2]

    cout << endl;
    vector<int> nums1c = {3, 3};
    int target1c = 6;
    cout << "输入: nums = [3, 3], target = 6" << endl;
    cout << "暴力解法输出: ";
    printVector(twoSum_BruteForce(nums1c, target1c));
    cout << "哈希表解法输出: ";
    printVector(twoSum_HashMap(nums1c, target1c));
    // 预期输出: [0, 1]

    // =================================================================
    // 测试题目 2：有效的字母异位词
    // =================================================================
    printSection("题目 2: 有效的字母异位词 (LeetCode 242, Easy)");

    string s1 = "anagram", t1 = "nagaram";
    cout << "输入: s = \"" << s1 << "\", t = \"" << t1 << "\"" << endl;
    cout << "排序解法输出: " << (isAnagram_Sort(s1, t1) ? "true" : "false") << endl;
    cout << "计数数组输出: " << (isAnagram_CountArray(s1, t1) ? "true" : "false") << endl;
    cout << "哈希表解法输出: " << (isAnagram_HashMap(s1, t1) ? "true" : "false") << endl;
    // 预期输出: true

    cout << endl;
    string s2 = "rat", t2 = "car";
    cout << "输入: s = \"" << s2 << "\", t = \"" << t2 << "\"" << endl;
    cout << "排序解法输出: " << (isAnagram_Sort(s2, t2) ? "true" : "false") << endl;
    cout << "计数数组输出: " << (isAnagram_CountArray(s2, t2) ? "true" : "false") << endl;
    cout << "哈希表解法输出: " << (isAnagram_HashMap(s2, t2) ? "true" : "false") << endl;
    // 预期输出: false

    cout << endl;
    string s3 = "listen", t3 = "silent";
    cout << "输入: s = \"" << s3 << "\", t = \"" << t3 << "\"" << endl;
    cout << "排序解法输出: " << (isAnagram_Sort(s3, t3) ? "true" : "false") << endl;
    cout << "计数数组输出: " << (isAnagram_CountArray(s3, t3) ? "true" : "false") << endl;
    cout << "哈希表解法输出: " << (isAnagram_HashMap(s3, t3) ? "true" : "false") << endl;
    // 预期输出: true

    // =================================================================
    // 测试题目 3：最长连续序列
    // =================================================================
    printSection("题目 3: 最长连续序列 (LeetCode 128, Medium)");

    vector<int> nums3a = {100, 4, 200, 1, 3, 2};
    cout << "输入: [100, 4, 200, 1, 3, 2]" << endl;
    cout << "排序解法输出: " << longestConsecutive_Sort(nums3a) << endl;
    cout << "HashSet解法输出: " << longestConsecutive_HashSet(nums3a) << endl;
    // 预期输出: 4（序列 [1,2,3,4]）

    cout << endl;
    vector<int> nums3b = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    cout << "输入: [0, 3, 7, 2, 5, 8, 4, 6, 0, 1]" << endl;
    cout << "排序解法输出: " << longestConsecutive_Sort(nums3b) << endl;
    cout << "HashSet解法输出: " << longestConsecutive_HashSet(nums3b) << endl;
    // 预期输出: 9（序列 [0,1,2,3,4,5,6,7,8]）

    cout << endl;
    vector<int> nums3c = {9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6};
    cout << "输入: [9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6]" << endl;
    cout << "排序解法输出: " << longestConsecutive_Sort(nums3c) << endl;
    cout << "HashSet解法输出: " << longestConsecutive_HashSet(nums3c) << endl;
    // 预期输出: 7（序列 [-1,0,1,3,4,5,6] 不连续; 序列 [3,4,5,6,7,8,9] 长度为 7）

    // =================================================================
    // 测试题目 4：移动零
    // =================================================================
    printSection("题目 4: 移动零 (LeetCode 283, Easy)");

    vector<int> nums4a = {0, 1, 0, 3, 12};
    cout << "输入: [0, 1, 0, 3, 12]" << endl;
    moveZeroes(nums4a);
    cout << "输出: ";
    printVector(nums4a);
    // 预期输出: [1, 3, 12, 0, 0]

    vector<int> nums4b = {0};
    cout << "\n输入: [0]" << endl;
    moveZeroes(nums4b);
    cout << "输出: ";
    printVector(nums4b);
    // 预期输出: [0]

    vector<int> nums4c = {1, 0, 0, 2, 3, 0, 4};
    cout << "\n输入: [1, 0, 0, 2, 3, 0, 4]" << endl;
    moveZeroes(nums4c);
    cout << "输出: ";
    printVector(nums4c);
    // 预期输出: [1, 2, 3, 4, 0, 0, 0]

    // =================================================================
    // 测试题目 5：盛最多水的容器
    // =================================================================
    printSection("题目 5: 盛最多水的容器 (LeetCode 11, Medium)");

    vector<int> height5a = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "输入: [1, 8, 6, 2, 5, 4, 8, 3, 7]" << endl;
    cout << "暴力解法输出: " << maxArea_BruteForce(height5a) << endl;
    cout << "双指针解法输出: " << maxArea_TwoPointer(height5a) << endl;
    // 预期输出: 49

    vector<int> height5b = {1, 1};
    cout << "\n输入: [1, 1]" << endl;
    cout << "暴力解法输出: " << maxArea_BruteForce(height5b) << endl;
    cout << "双指针解法输出: " << maxArea_TwoPointer(height5b) << endl;
    // 预期输出: 1

    vector<int> height5c = {4, 3, 2, 1, 4};
    cout << "\n输入: [4, 3, 2, 1, 4]" << endl;
    cout << "暴力解法输出: " << maxArea_BruteForce(height5c) << endl;
    cout << "双指针解法输出: " << maxArea_TwoPointer(height5c) << endl;
    // 预期输出: 16

    // =================================================================
    // 测试题目 6：接雨水
    // =================================================================
    printSection("题目 6: 接雨水 (LeetCode 42, Hard)");

    vector<int> height6a = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "输入: [0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]" << endl;
    cout << "暴力解法输出: " << trap_BruteForce(height6a) << endl;
    cout << "动态规划输出: " << trap_DP(height6a) << endl;
    cout << "双指针解法输出: " << trap_TwoPointer(height6a) << endl;
    // 预期输出: 6

    vector<int> height6b = {4, 2, 0, 3, 2, 5};
    cout << "\n输入: [4, 2, 0, 3, 2, 5]" << endl;
    cout << "暴力解法输出: " << trap_BruteForce(height6b) << endl;
    cout << "动态规划输出: " << trap_DP(height6b) << endl;
    cout << "双指针解法输出: " << trap_TwoPointer(height6b) << endl;
    // 预期输出: 9

    vector<int> height6c = {5, 4, 1, 2};
    cout << "\n输入: [5, 4, 1, 2]" << endl;
    cout << "暴力解法输出: " << trap_BruteForce(height6c) << endl;
    cout << "动态规划输出: " << trap_DP(height6c) << endl;
    cout << "双指针解法输出: " << trap_TwoPointer(height6c) << endl;
    // 预期输出: 1

    cout << "\n========================================" << endl;
    cout << "  所有测试用例运行完毕！" << endl;
    cout << "========================================" << endl;

    return 0;
}

// =====================================================================
//  辅助函数：打印二维 vector（用于三数之和、四数之和等结果输出）
// =====================================================================
void printVector2D(const vector<vector<int>>& vec2d) {
    cout << "[" << endl;
    for (int i = 0; i < (int)vec2d.size(); i++) {
        cout << "  ";
        printVector(vec2d[i]);
    }
    cout << "]" << endl;
}

// =====================================================================
//  辅助函数：打印字符串二维 vector（用于字母异位词分组结果输出）
// =====================================================================
void printStringGroups(const vector<vector<string>>& groups) {
    cout << "[" << endl;
    for (int i = 0; i < (int)groups.size(); i++) {
        cout << "  [";
        for (int j = 0; j < (int)groups[i].size(); j++) {
            cout << "\"" << groups[i][j] << "\"";
            if (j < (int)groups[i].size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
    cout << "]" << endl;
}

// =====================================================================
//  main2 函数：运行题目 7-12 的测试用例
// =====================================================================
int main2() {

    // =================================================================
    // 测试题目 7：三数之和
    // =================================================================
    printSection("题目 7: 三数之和 (LeetCode 15, Medium)");

    vector<int> nums7a = {-1, 0, 1, 2, -1, -4};
    cout << "输入: [-1, 0, 1, 2, -1, -4]" << endl;
    cout << "暴力解法输出: ";
    printVector2D(threeSum_BruteForce(nums7a));
    vector<int> nums7a2 = {-1, 0, 1, 2, -1, -4};
    cout << "双指针解法输出: ";
    printVector2D(threeSum_TwoPointer(nums7a2));
    // 预期输出: [[-1, -1, 2], [-1, 0, 1]]

    cout << endl;
    vector<int> nums7b = {0, 1, 1};
    cout << "输入: [0, 1, 1]" << endl;
    cout << "暴力解法输出: ";
    printVector2D(threeSum_BruteForce(nums7b));
    vector<int> nums7b2 = {0, 1, 1};
    cout << "双指针解法输出: ";
    printVector2D(threeSum_TwoPointer(nums7b2));
    // 预期输出: []

    cout << endl;
    vector<int> nums7c = {0, 0, 0};
    cout << "输入: [0, 0, 0]" << endl;
    cout << "暴力解法输出: ";
    printVector2D(threeSum_BruteForce(nums7c));
    vector<int> nums7c2 = {0, 0, 0};
    cout << "双指针解法输出: ";
    printVector2D(threeSum_TwoPointer(nums7c2));
    // 预期输出: [[0, 0, 0]]

    cout << endl;
    vector<int> nums7d = {-2, 0, 1, 1, 2};
    cout << "输入: [-2, 0, 1, 1, 2]" << endl;
    cout << "暴力解法输出: ";
    printVector2D(threeSum_BruteForce(nums7d));
    vector<int> nums7d2 = {-2, 0, 1, 1, 2};
    cout << "双指针解法输出: ";
    printVector2D(threeSum_TwoPointer(nums7d2));
    // 预期输出: [[-2, 0, 2], [-2, 1, 1]]

    // =================================================================
    // 测试题目 8：四数之和
    // =================================================================
    printSection("题目 8: 四数之和 (LeetCode 18, Medium)");

    vector<int> nums8a = {1, 0, -1, 0, -2, 2};
    cout << "输入: [1, 0, -1, 0, -2, 2], target = 0" << endl;
    cout << "输出: ";
    printVector2D(fourSum(nums8a, 0));
    // 预期输出: [[-2, -1, 1, 2], [-2, 0, 0, 2], [-1, 0, 0, 1]]

    cout << endl;
    vector<int> nums8b = {2, 2, 2, 2, 2};
    cout << "输入: [2, 2, 2, 2, 2], target = 8" << endl;
    cout << "输出: ";
    printVector2D(fourSum(nums8b, 8));
    // 预期输出: [[2, 2, 2, 2]]

    cout << endl;
    vector<int> nums8c = {0, 0, 0, 0};
    cout << "输入: [0, 0, 0, 0], target = 0" << endl;
    cout << "输出: ";
    printVector2D(fourSum(nums8c, 0));
    // 预期输出: [[0, 0, 0, 0]]

    cout << endl;
    vector<int> nums8d = {-3, -2, -1, 0, 0, 2, 3, 4};
    cout << "输入: [-3, -2, -1, 0, 0, 2, 3, 4], target = 0" << endl;
    cout << "输出: ";
    printVector2D(fourSum(nums8d, 0));
    // 预期输出: [[-3, -2, 1, 4]... 实际输出中含 [-3,-1,0,4],[-3,0,0,3],[-2,-1,0,3],[-2,0,0,2]]

    // =================================================================
    // 测试题目 9：字母异位词分组
    // =================================================================
    printSection("题目 9: 字母异位词分组 (LeetCode 49, Medium)");

    vector<string> strs9a = {"eat", "tea", "tan", "ate", "nat", "bat"};
    cout << "输入: [\"eat\", \"tea\", \"tan\", \"ate\", \"nat\", \"bat\"]" << endl;
    cout << "排序键解法输出: ";
    printStringGroups(groupAnagrams_SortKey(strs9a));
    vector<string> strs9a2 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    cout << "计数键解法输出: ";
    printStringGroups(groupAnagrams_CountKey(strs9a2));
    // 预期输出: [["bat"], ["nat","tan"], ["ate","eat","tea"]]（顺序可不同）

    cout << endl;
    vector<string> strs9b = {""};
    cout << "输入: [\"\"]" << endl;
    cout << "排序键解法输出: ";
    printStringGroups(groupAnagrams_SortKey(strs9b));
    vector<string> strs9b2 = {""};
    cout << "计数键解法输出: ";
    printStringGroups(groupAnagrams_CountKey(strs9b2));
    // 预期输出: [[""]]

    cout << endl;
    vector<string> strs9c = {"a"};
    cout << "输入: [\"a\"]" << endl;
    cout << "排序键解法输出: ";
    printStringGroups(groupAnagrams_SortKey(strs9c));
    vector<string> strs9c2 = {"a"};
    cout << "计数键解法输出: ";
    printStringGroups(groupAnagrams_CountKey(strs9c2));
    // 预期输出: [["a"]]

    // =================================================================
    // 测试题目 10：除自身以外数组的乘积
    // =================================================================
    printSection("题目 10: 除自身以外数组的乘积 (LeetCode 238, Medium)");

    vector<int> nums10a = {1, 2, 3, 4};
    cout << "输入: [1, 2, 3, 4]" << endl;
    cout << "左右前缀积解法输出: ";
    printVector(productExceptSelf_TwoArrays(nums10a));
    vector<int> nums10a2 = {1, 2, 3, 4};
    cout << "O(1)空间优化输出: ";
    printVector(productExceptSelf_Optimal(nums10a2));
    // 预期输出: [24, 12, 8, 6]

    cout << endl;
    vector<int> nums10b = {-1, 1, 0, -3, 3};
    cout << "输入: [-1, 1, 0, -3, 3]" << endl;
    cout << "左右前缀积解法输出: ";
    printVector(productExceptSelf_TwoArrays(nums10b));
    vector<int> nums10b2 = {-1, 1, 0, -3, 3};
    cout << "O(1)空间优化输出: ";
    printVector(productExceptSelf_Optimal(nums10b2));
    // 预期输出: [0, 0, 9, 0, 0]

    cout << endl;
    vector<int> nums10c = {2, 3, 4, 5};
    cout << "输入: [2, 3, 4, 5]" << endl;
    cout << "左右前缀积解法输出: ";
    printVector(productExceptSelf_TwoArrays(nums10c));
    vector<int> nums10c2 = {2, 3, 4, 5};
    cout << "O(1)空间优化输出: ";
    printVector(productExceptSelf_Optimal(nums10c2));
    // 预期输出: [60, 40, 30, 24]

    // =================================================================
    // 测试题目 11：轮转数组
    // =================================================================
    printSection("题目 11: 轮转数组 (LeetCode 189, Medium)");

    vector<int> nums11a = {1, 2, 3, 4, 5, 6, 7};
    cout << "输入: [1, 2, 3, 4, 5, 6, 7], k = 3" << endl;
    rotate_ExtraArray(nums11a, 3);
    cout << "额外数组解法输出: ";
    printVector(nums11a);
    // 预期输出: [5, 6, 7, 1, 2, 3, 4]

    vector<int> nums11a2 = {1, 2, 3, 4, 5, 6, 7};
    rotate_Reverse(nums11a2, 3);
    cout << "三次翻转解法输出: ";
    printVector(nums11a2);
    // 预期输出: [5, 6, 7, 1, 2, 3, 4]

    cout << endl;
    vector<int> nums11b = {-1, -100, 3, 99};
    cout << "输入: [-1, -100, 3, 99], k = 2" << endl;
    rotate_ExtraArray(nums11b, 2);
    cout << "额外数组解法输出: ";
    printVector(nums11b);
    // 预期输出: [3, 99, -1, -100]

    vector<int> nums11b2 = {-1, -100, 3, 99};
    rotate_Reverse(nums11b2, 2);
    cout << "三次翻转解法输出: ";
    printVector(nums11b2);
    // 预期输出: [3, 99, -1, -100]

    cout << endl;
    vector<int> nums11c = {1, 2};
    cout << "输入: [1, 2], k = 3" << endl;
    rotate_ExtraArray(nums11c, 3);
    cout << "额外数组解法输出: ";
    printVector(nums11c);
    // 预期输出: [2, 1]（k=3 等价于 k=1，因为 3%2=1）

    vector<int> nums11c2 = {1, 2};
    rotate_Reverse(nums11c2, 3);
    cout << "三次翻转解法输出: ";
    printVector(nums11c2);
    // 预期输出: [2, 1]

    // =================================================================
    // 测试题目 12：缺失的第一个正数
    // =================================================================
    printSection("题目 12: 缺失的第一个正数 (LeetCode 41, Hard)");

    vector<int> nums12a = {1, 2, 0};
    cout << "输入: [1, 2, 0]" << endl;
    cout << "HashSet解法输出: " << firstMissingPositive_HashSet(nums12a) << endl;
    vector<int> nums12a2 = {1, 2, 0};
    cout << "原地哈希解法输出: " << firstMissingPositive_InPlaceHash(nums12a2) << endl;
    // 预期输出: 3

    cout << endl;
    vector<int> nums12b = {3, 4, -1, 1};
    cout << "输入: [3, 4, -1, 1]" << endl;
    cout << "HashSet解法输出: " << firstMissingPositive_HashSet(nums12b) << endl;
    vector<int> nums12b2 = {3, 4, -1, 1};
    cout << "原地哈希解法输出: " << firstMissingPositive_InPlaceHash(nums12b2) << endl;
    // 预期输出: 2

    cout << endl;
    vector<int> nums12c = {7, 8, 9, 11, 12};
    cout << "输入: [7, 8, 9, 11, 12]" << endl;
    cout << "HashSet解法输出: " << firstMissingPositive_HashSet(nums12c) << endl;
    vector<int> nums12c2 = {7, 8, 9, 11, 12};
    cout << "原地哈希解法输出: " << firstMissingPositive_InPlaceHash(nums12c2) << endl;
    // 预期输出: 1

    cout << endl;
    vector<int> nums12d = {1, 2, 3, 4, 5};
    cout << "输入: [1, 2, 3, 4, 5]" << endl;
    cout << "HashSet解法输出: " << firstMissingPositive_HashSet(nums12d) << endl;
    vector<int> nums12d2 = {1, 2, 3, 4, 5};
    cout << "原地哈希解法输出: " << firstMissingPositive_InPlaceHash(nums12d2) << endl;
    // 预期输出: 6（所有 1~5 都存在，最小缺失的是 6）

    cout << endl;
    vector<int> nums12e = {1, 1, 1, 1};
    cout << "输入: [1, 1, 1, 1]" << endl;
    cout << "HashSet解法输出: " << firstMissingPositive_HashSet(nums12e) << endl;
    vector<int> nums12e2 = {1, 1, 1, 1};
    cout << "原地哈希解法输出: " << firstMissingPositive_InPlaceHash(nums12e2) << endl;
    // 预期输出: 2

    cout << "\n========================================" << endl;
    cout << "  题目 7-12 测试用例运行完毕！" << endl;
    cout << "========================================" << endl;

    return 0;
}
