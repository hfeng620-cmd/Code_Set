/**
 * 02_滑动窗口.cpp
 * 经典力扣滑动窗口专题 —— 10道经典题目 + 多种解法 + 详细注释
 *
 * 目录：
 *   1. 无重复字符的最长子串        (LeetCode 3,    Medium)
 *   2. 找到字符串中所有字母异位词   (LeetCode 438,  Medium)
 *   3. 最小覆盖子串               (LeetCode 76,   Hard)
 *   4. 最大连续1的个数 III         (LeetCode 1004, Medium)
 *   5. 长度最小的子数组            (LeetCode 209,  Medium)
 *   6. 替换后的最长重复字符        (LeetCode 424,  Medium)
 *   7. K个不同整数的子数组         (LeetCode 992,  Hard)
 *   8. 包含排列的子串              (LeetCode 567,  Medium)
 *   9. 长度为K的最大子数组和        (LeetCode 2461, Medium)
 *  10. 将x减到0的最少操作数        (LeetCode 1658, Medium)
 *
 * 编译：g++ -std=c++17 -O2 -o 02_滑动窗口 02_滑动窗口.cpp
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <climits>
using namespace std;

// ============================================================================
// 滑动窗口算法概述
// ============================================================================
/*
 * 滑动窗口是一种高效的数组/字符串遍历技巧，核心思想：
 *   1. 维护一个窗口 [left, right]，右指针不断向右扩展
 *   2. 当窗口状态不满足条件时，移动左指针收缩窗口
 *   3. 在窗口移动过程中维护/更新答案
 *
 * 两种常见模式：
 *
 * 模式一：可变窗口大小（找最优解）
 * -------------------------------------------------------
 *   int left = 0;
 *   for (int right = 0; right < n; right++) {
 *       // 1. 将 nums[right] 加入窗口
 *       // 2. 判断窗口是否需要收缩
 *       while (窗口需要收缩) {
 *           // 将 nums[left] 移出窗口
 *           left++;
 *       }
 *       // 3. 更新答案
 *   }
 *
 * 模式二：固定窗口大小（窗口长度固定为k）
 * -------------------------------------------------------
 *   for (int i = 0; i < n; i++) {
 *       // 1. 将 nums[i] 加入窗口
 *       if (i >= k) {
 *           // 2. 移除 nums[i - k]，保持窗口大小为k
 *       }
 *       // 3. 更新答案（当窗口大小 == k 时）
 *   }
 *
 * 时间复杂度分析：
 *   虽然有两层循环，但 left 和 right 各自最多移动 n 次，
 *   所以总时间复杂度为 O(n)，而非 O(n^2)。
 *   这就是滑动窗口的高效之处。
 *
 * 适用条件：
 *   - 求「连续」子数组/子串的最优解或计数
 *   - 窗口扩大时某种度量单调变化（和增大、不重复字符增多等）
 *   - 可以 O(1) 或 O(字符集) 维护窗口状态
 */

// ============================================================================
// 题目1：无重复字符的最长子串 (LeetCode 3, Medium)
// ============================================================================
//
// 题目描述：
//   给定一个字符串 s，请你找出其中不含有重复字符的「最长子串」的长度。
//
// 示例：
//   输入: s = "abcabcbb"    输出: 3  （"abc"）
//   输入: s = "bbbbb"       输出: 1  （"b"）
//   输入: s = "pwwkew"      输出: 3  （"wke"）
//
// 滑动窗口移动过程示意 (s = "abcabcbb")：
//
//   步骤1: [a] b c a b c b b    窗口="a"      长度=1
//   步骤2: [a b] c a b c b b    窗口="ab"     长度=2
//   步骤3: [a b c] a b c b b    窗口="abc"    长度=3
//   步骤4:  a [b c a] b c b b   窗口="bca"    长度=3  (遇到重复'a',左指针右移)
//   步骤5:  a b [c a b] c b b   窗口="cab"    长度=3  (遇到重复'b',左指针右移)
//   步骤6:  a b c [a b c] b b   窗口="abc"    长度=3  (遇到重复'c',左指针右移)
//   步骤7:  a b c a [b c b] b   窗口="bcb"->  长度=3  (遇到重复'b')
//   步骤8:  a b c a b [c b b]   窗口="cbb"->  长度=3  (遇到重复'b')
//   最终答案: 3

// ---------- 方法1：暴力法 O(n^2) ----------
// 对每个起始位置，向右扩展直到遇到重复字符
int lengthOfLongestSubstring_bruteforce(string s) {
    int n = (int)s.size();               // 获取字符串长度
    int maxLen = 0;                      // 记录最大长度
    for (int i = 0; i < n; i++) {        // 枚举每个起始位置 i
        unordered_set<char> seen;        // 用哈希集合记录当前窗口中的字符
        int curLen = 0;                  // 当前子串长度
        for (int j = i; j < n; j++) {    // 从 i 向右扩展
            if (seen.count(s[j])) {      // 如果 s[j] 已经出现过
                break;                   // 则当前子串结束，跳出内层循环
            }
            seen.insert(s[j]);           // 将 s[j] 加入集合
            curLen++;                    // 当前长度加1
        }
        maxLen = max(maxLen, curLen);    // 更新最大长度
    }
    return maxLen;                       // 返回结果
}
// 时间复杂度: O(n^2)，空间复杂度: O(min(n, 字符集大小))

// ---------- 方法2：滑动窗口 + HashSet O(n) ----------
// 利用滑动窗口：右指针扩展，遇到重复时左指针收缩
int lengthOfLongestSubstring_hashset(string s) {
    unordered_set<char> window;          // 滑动窗口中的字符集合
    int left = 0;                        // 窗口左边界
    int maxLen = 0;                      // 最大长度
    for (int right = 0; right < (int)s.size(); right++) { // 右指针遍历
        while (window.count(s[right])) { // 如果 s[right] 在窗口中已存在
            window.erase(s[left]);       // 移除窗口最左字符
            left++;                      // 左指针右移，缩小窗口
        }
        window.insert(s[right]);         // 将当前字符加入窗口
        maxLen = max(maxLen, right - left + 1); // 更新最大长度
    }
    return maxLen;                       // 返回结果
}
// 时间复杂度: O(n)，空间复杂度: O(min(n, 字符集大小))

// ---------- 方法3：滑动窗口 + 数组索引 O(n) ----------
// 用数组记录每个字符最后出现的位置，直接跳转左指针
int lengthOfLongestSubstring_array(string s) {
    vector<int> lastIndex(256, -1);      // 记录每个ASCII字符最后出现的位置，初始为-1
    int left = 0;                        // 窗口左边界
    int maxLen = 0;                      // 最大长度
    for (int right = 0; right < (int)s.size(); right++) { // 右指针遍历
        // 如果当前字符在窗口中出现过，将左边界跳到上次出现位置+1
        left = max(left, lastIndex[s[right]] + 1);
        lastIndex[s[right]] = right;     // 更新当前字符的最新位置
        maxLen = max(maxLen, right - left + 1); // 更新最大长度
    }
    return maxLen;                       // 返回结果
}
// 时间复杂度: O(n)，空间复杂度: O(字符集大小) = O(1)
// 这是最优解法，每个字符只被访问一次

// ============================================================================
// 题目2：找到字符串中所有字母异位词 (LeetCode 438, Medium)
// ============================================================================
//
// 题目描述：
//   给定两个字符串 s 和 p，找到 s 中所有 p 的异位词的子串，
//   返回这些子串的起始索引。异位词指字母相同但排列不同的字符串。
//
// 示例：
//   输入: s = "cbaebabacd", p = "abc"    输出: [0, 6]
//   输入: s = "abab", p = "ab"           输出: [0, 1, 2]
//
// 滑动窗口移动过程示意 (s = "cbaebabacd", p = "abc")：
//   p的频率表: {a:1, b:1, c:1}
//
//   步骤1: [c b a] e b a b a c d   窗口频率={a:1,b:1,c:1} 匹配! 加入0
//   步骤2:  c [b a e] b a b a c d   窗口频率={a:1,b:1,e:1} 不匹配
//   步骤3:  c b [a e b] a b a c d   窗口频率={a:1,b:1,e:1} 不匹配
//   步骤4:  c b a [e b a] b a c d   窗口频率={a:1,b:1,e:1} 不匹配
//   步骤5:  c b a e [b a b] a c d   窗口频率={a:1,b:2}    不匹配
//   步骤6:  c b a e b [a b a] c d   窗口频率={a:2,b:1}    不匹配
//   步骤7:  c b a e b a [b a c] d   窗口频率={a:1,b:1,c:1} 匹配! 加入6
//   步骤8:  c b a e b a b [a c d]   窗口频率={a:1,c:1,d:1} 不匹配
//   最终答案: [0, 6]

// ---------- 方法1：排序法 O(n * k log k) ----------
// 每次取长度为p.size()的子串，排序后与排序后的p比较
vector<int> findAnagrams_sort(string s, string p) {
    vector<int> result;                  // 存储结果
    int n = (int)s.size(), k = (int)p.size(); // n为s长度，k为p长度
    if (n < k) return result;            // 如果s比p短，无解
    string sortedP = p;                  // 复制p
    sort(sortedP.begin(), sortedP.end()); // 对p排序作为参考
    for (int i = 0; i <= n - k; i++) {   // 枚举每个长度为k的子串
        string sub = s.substr(i, k);     // 取出子串
        sort(sub.begin(), sub.end());    // 对子串排序
        if (sub == sortedP) {            // 如果排序后相等，说明是异位词
            result.push_back(i);         // 记录起始位置
        }
    }
    return result;                       // 返回结果
}
// 时间复杂度: O(n * k log k)，空间复杂度: O(k)

// ---------- 方法2：滑动窗口 + 频率数组 O(n) ----------
// 固定大小窗口，用频率数组判断窗口内字符是否为p的异位词
vector<int> findAnagrams_slidingwindow(string s, string p) {
    vector<int> result;                  // 存储结果
    int n = (int)s.size(), k = (int)p.size(); // n为s长度，k为p长度
    if (n < k) return result;            // 如果s比p短，无解

    vector<int> pCount(26, 0);           // p的字符频率表
    vector<int> wCount(26, 0);           // 当前窗口的字符频率表
    for (char c : p) pCount[c - 'a']++;  // 统计p中每个字符出现次数

    for (int i = 0; i < n; i++) {        // 右指针从0开始遍历
        wCount[s[i] - 'a']++;            // 将右指针字符加入窗口频率
        if (i >= k) {                    // 如果窗口大小超过k
            wCount[s[i - k] - 'a']--;    // 移除窗口最左字符的频率
        }
        if (wCount == pCount) {          // 如果窗口频率与p频率完全一致
            result.push_back(i - k + 1); // 记录窗口起始位置
        }
    }
    return result;                       // 返回结果
}
// 时间复杂度: O(n)，空间复杂度: O(1)（固定26个字母）
// 核心思想：固定大小的滑动窗口，窗口长度始终等于p的长度

// ============================================================================
// 题目3：最小覆盖子串 (LeetCode 76, Hard)
// ============================================================================
//
// 题目描述：
//   给定字符串 s 和 t，找出 s 中涵盖 t 所有字符的最短子串。
//   如果 s 中不存在这样的子串，返回空字符串。
//
// 示例：
//   输入: s = "ADOBECODEBANC", t = "ABC"  输出: "BANC"
//   输入: s = "a", t = "a"                输出: "a"
//   输入: s = "a", t = "aa"               输出: ""
//
// 滑动窗口移动过程示意 (s = "ADOBECODEBANC", t = "ABC")：
//
//   t的频率表: {A:1, B:1, C:1}，需要满足的字符数 = 3
//
//   right=0: [A] D O B E C O D E B A N C   满足A,还需2个
//   right=1: [A D] O B E C O D E B A N C   满足A,还需2个
//   right=2: [A D O] B E C O D E B A N C   满足A,还需2个
//   right=3: [A D O B] E C O D E B A N C   满足A,B,还需1个
//   right=4: [A D O B E] C O D E B A N C   满足A,B,还需1个
//   right=5: [A D O B E C] O D E B A N C   满足A,B,C! -> "ADOBEC"长度6
//            尝试收缩: D O B E C O... -> 不再满足,停止收缩
//   right=6:  A [D O B E C O] D E B A N C   窗口仍满足 -> 不更优
//   ...
//   right=10: A D O B E C O D E B [A] N C   满足A,还需B,C
//   right=11: A D O B E C O D E B [A N] C   满足A,还需B,C
//   right=12: A D O B E C O D E B [A N C]   满足A,C,还需B -> 不满足
//   回溯收缩... 最终 "BANC" 长度4
//   最终答案: "BANC"

string minWindow(string s, string t) {
    // 统计 t 中每个字符需要的数量
    unordered_map<char, int> need;       // need[c] 表示字符c还需要出现几次
    for (char c : t) need[c]++;          // 统计t中每个字符频率

    int total = (int)t.size();           // 总共需要匹配的字符数
    int left = 0;                        // 窗口左边界
    int minLen = INT_MAX;                // 最短子串长度，初始为无穷大
    int minStart = 0;                    // 最短子串的起始位置

    for (int right = 0; right < (int)s.size(); right++) { // 右指针扩展窗口
        char c = s[right];               // 当前右指针字符
        if (need.count(c)) {             // 如果这个字符在t中需要
            if (need[c] > 0) total--;    // 如果还需要该字符，待匹配总数减1
            need[c]--;                   // 需要的数量减1（可能变为负数，表示窗口中有多余）
        }

        // 当 total == 0 时，说明窗口已包含t的所有字符
        while (total == 0) {             // 尝试收缩窗口以找到更短的子串
            if (right - left + 1 < minLen) { // 如果当前窗口更短
                minLen = right - left + 1;   // 更新最短长度
                minStart = left;             // 更新起始位置
            }
            char lc = s[left];           // 左指针即将移出的字符
            if (need.count(lc)) {        // 如果该字符在t中
                need[lc]++;              // 移出后，需要的数量加1
                if (need[lc] > 0) total++; // 如果变为正数，说明又缺字符了
            }
            left++;                      // 左指针右移，收缩窗口
        }
    }

    return minLen == INT_MAX ? "" : s.substr(minStart, minLen); // 返回结果
}
// 时间复杂度: O(|s| + |t|)，空间复杂度: O(|t|)
// 关键点：need数组中值可以为负数，表示窗口中该字符有多余的数量

// ============================================================================
// 题目4：最大连续1的个数 III (LeetCode 1004, Medium)
// ============================================================================
//
// 题目描述：
//   给定一个二进制数组 nums 和一个整数 k，
//   如果可以翻转最多 k 个 0，则返回数组中连续 1 的最大个数。
//
// 示例：
//   输入: nums = [1,1,0,0,1,1,1,0,1], k = 2   输出: 7  （翻转索引2和3的0）
//   输入: nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k = 3  输出: 10
//
// 滑动窗口移动过程示意 (nums = [1,1,0,0,1,1,1,0,1], k = 2)：
//
//   核心思想：找最多包含k个0的最长子数组
//
//   right=0: [1] 1 0 0 1 1 1 0 1    0的个数=0, 窗口长度=1
//   right=1: [1 1] 0 0 1 1 1 0 1    0的个数=0, 窗口长度=2
//   right=2: [1 1 0] 0 1 1 1 0 1    0的个数=1, 窗口长度=3
//   right=3: [1 1 0 0] 1 1 1 0 1    0的个数=2, 窗口长度=4
//   right=4: [1 1 0 0 1] 1 1 0 1    0的个数=2, 窗口长度=5
//   right=5: [1 1 0 0 1 1] 1 0 1    0的个数=2, 窗口长度=6
//   right=6: [1 1 0 0 1 1 1] 0 1    0的个数=2, 窗口长度=7
//   right=7: [1 1 0 0 1 1 1 0] 1    0的个数=3 > k! 需要收缩
//            收缩: 1 [1 0 0 1 1 1 0] 1    0的个数=3 > k,继续收缩
//            收缩: 1 1 [0 0 1 1 1 0] 1    0的个数=3 > k,继续收缩
//            收缩: 1 1 0 [0 1 1 1 0] 1    0的个数=2, 窗口长度=5
//   right=8: 1 1 0 [0 1 1 1 0 1]    0的个数=2, 窗口长度=6
//   注意: 最大长度在right=6时已记录为7 ([1,1,0,0,1,1,1]恰好含2个0)
//   最终答案: 7

int longestOnes(vector<int>& nums, int k) {
    int left = 0;                        // 窗口左边界
    int zeroCount = 0;                   // 当前窗口中0的个数
    int maxLen = 0;                      // 最大长度

    for (int right = 0; right < (int)nums.size(); right++) { // 右指针遍历
        if (nums[right] == 0) {          // 如果当前元素是0
            zeroCount++;                 // 窗口内0的计数加1
        }
        // 当窗口内0的个数超过允许的翻转次数k时，收缩窗口
        while (zeroCount > k) {          // 需要收缩直到0的个数不超过k
            if (nums[left] == 0) {       // 如果左边界是0
                zeroCount--;             // 移出后0的计数减1
            }
            left++;                      // 左边界右移
        }
        maxLen = max(maxLen, right - left + 1); // 更新最大长度
    }
    return maxLen;                       // 返回结果
}
// 时间复杂度: O(n)，空间复杂度: O(1)
// 关键点：将问题转化为"最多包含k个0的最长子数组"

// ============================================================================
// 题目5：长度最小的子数组 (LeetCode 209, Medium)
// ============================================================================
//
// 题目描述：
//   给定一个含有 n 个正整数的数组和一个正整数 target。
//   找出满足其和 >= target 的长度最小的连续子数组，
//   并返回其长度。如果不存在则返回 0。
//
// 示例：
//   输入: target = 7, nums = [2,3,1,2,4,3]        输出: 2  （[4,3]）
//   输入: target = 4, nums = [1,4,4]              输出: 1  （[4]）
//   输入: target = 11, nums = [1,1,1,1,1,1,1,1]   输出: 0
//
// 滑动窗口移动过程示意 (target = 7, nums = [2,3,1,2,4,3])：
//
//   right=0: [2] 3 1 2 4 3          sum=2 < 7, 继续扩展
//   right=1: [2 3] 1 2 4 3          sum=5 < 7, 继续扩展
//   right=2: [2 3 1] 2 4 3          sum=6 < 7, 继续扩展
//   right=3: [2 3 1 2] 4 3          sum=8 >= 7! 记录长度4,尝试收缩
//            2 [3 1 2] 4 3          sum=6 < 7, 停止收缩
//   right=4: 2 [3 1 2 4] 3          sum=10 >= 7! 记录长度4,尝试收缩
//            2 3 [1 2 4] 3          sum=7 >= 7! 记录长度3,尝试收缩
//            2 3 1 [2 4] 3          sum=6 < 7, 停止收缩
//   right=5: 2 3 1 [2 4 3]          sum=9 >= 7! 记录长度3,尝试收缩
//            2 3 1 2 [4 3]          sum=7 >= 7! 记录长度2,尝试收缩
//            2 3 1 2 4 [3]          sum=3 < 7, 停止收缩
//   最终答案: 2

// ---------- 方法1：暴力法 O(n^2) ----------
// 对每个起始位置，找满足条件的最短子数组
int minSubArrayLen_bruteforce(int target, vector<int>& nums) {
    int n = (int)nums.size();            // 数组长度
    int minLen = INT_MAX;                // 最短长度，初始为无穷大

    for (int i = 0; i < n; i++) {        // 枚举每个起始位置
        int sum = 0;                     // 当前子数组和
        for (int j = i; j < n; j++) {    // 从i开始向右扩展
            sum += nums[j];              // 累加当前元素
            if (sum >= target) {         // 如果和满足条件
                minLen = min(minLen, j - i + 1); // 更新最短长度
                break;                   // 找到以i开头的最短子数组，跳出
            }
        }
    }
    return minLen == INT_MAX ? 0 : minLen; // 如果没找到返回0
}
// 时间复杂度: O(n^2)，空间复杂度: O(1)

// ---------- 方法2：滑动窗口 O(n) ----------
// 利用数组元素为正整数的性质：扩展窗口使和增大，收缩窗口使和减小
int minSubArrayLen_slidingwindow(int target, vector<int>& nums) {
    int n = (int)nums.size();            // 数组长度
    int left = 0;                        // 窗口左边界
    int sum = 0;                         // 当前窗口和
    int minLen = INT_MAX;                // 最短长度

    for (int right = 0; right < n; right++) { // 右指针遍历
        sum += nums[right];              // 将右指针元素加入窗口和

        // 当窗口和满足条件时，尝试收缩窗口
        while (sum >= target) {          // 和仍满足条件就继续收缩
            minLen = min(minLen, right - left + 1); // 更新最短长度
            sum -= nums[left];           // 移除左边界元素
            left++;                      // 左边界右移
        }
    }
    return minLen == INT_MAX ? 0 : minLen; // 如果没找到返回0
}
// 时间复杂度: O(n)，空间复杂度: O(1)
// 每个元素最多被访问两次（right加一次，left减一次）

// ============================================================================
// 何时使用滑动窗口 vs 其他技巧
// ============================================================================
/*
 * 滑动窗口适用场景：
 *   1. 问题涉及「连续」子数组/子串
 *   2. 窗口具有单调性：右指针右移时窗口"变大/变强"，
 *      左指针右移时窗口"变小/变弱"
 *   3. 可以在 O(1) 或 O(字符集) 时间内维护窗口信息
 *
 * 不适用的场景：
 *   1. 子序列（非连续）问题 -> 通常用动态规划
 *   2. 需要枚举所有子集 -> 通常用回溯/位运算
 *   3. 窗口不具有单调性 -> 考虑其他方法
 *
 * 常见套路：
 *   - 可变窗口：while (不满足条件) 收缩左边界
 *   - 固定窗口：窗口大小固定为k，维护窗口信息
 *   - 计数型：用频率表/计数器维护窗口状态
 *
 * ============================================================================
 * 十道题目的解法对比总结
 * ============================================================================
 *
 * +------+---------------------------+--------+-------------------------------------+
 * | 题号 | 题目                      | 难度   | 关键技巧                            |
 * +------+---------------------------+--------+-------------------------------------+
 * |   3  | 无重复字符的最长子串      | Medium | HashSet/数组记录字符位置             |
 * | 438  | 字母异位词                | Medium | 固定窗口+频率数组比较                |
 * |  76  | 最小覆盖子串              | Hard   | need计数+total变量判断满足           |
 * | 1004 | 最大连续1的个数 III       | Medium | 转化为"最多k个0"的最长子数组         |
 * | 209  | 长度最小的子数组          | Medium | 正整数性质保证窗口单调性             |
 * | 424  | 替换后的最长重复字符      | Medium | 窗口长度-maxFreq<=k,历史最大频率     |
 * | 992  | K个不同整数的子数组       | Hard   | 恰好K=最多K-最多(K-1) 差值技巧       |
 * | 567  | 包含排列的子串            | Medium | 固定窗口+频率匹配/matched计数        |
 * | 2461 | 长度为K的最大子数组和     | Medium | 固定窗口+无重复约束                  |
 * | 1658 | 将x减到0的最少操作数      | Medium | 转化为找中间子数组和=target          |
 * +------+---------------------------+--------+-------------------------------------+
 *
 * 窗口收缩条件对比：
 *   题目3:    窗口中出现重复字符时收缩（while 有重复）
 *   题目438:  窗口大小超过k时移除最左元素（固定窗口）
 *   题目76:   total==0 时尝试收缩（while 仍满足）
 *   题目1004: 0的个数超过k时收缩（while zeroCount > k）
 *   题目209:  和>=target时尝试收缩（while sum >= target）
 *   题目424:  窗口长度-maxFreq>k时收缩（while 替换数超限）
 *   题目992:  不同整数超过k时收缩（while freq.size() > k）
 *   题目567:  窗口大小超过len1时移除最左元素（固定窗口）
 *   题目2461: 窗口超过k或出现重复时收缩（while 过大或重复）
 *   题目1658: 窗口和超过target时收缩（while sum > target）
 *
 * 常见滑动窗口错误：
 *   1. 收缩条件写反：应该在 while 循环中收缩，而非 if
 *   2. 边界条件：left <= right，窗口大小 = right - left + 1
 *   3. 空窗口处理：注意 left > right 的情况
 *   4. 更新答案的位置：通常在收缩之后或每次迭代中
 *
 * 高级技巧总结：
 *   - 差值技巧（题992）：恰好K = 最多K - 最多(K-1)，适用于"恰好"计数
 *   - 问题转化（题1658）：两端移除 -> 中间保留，转化为标准滑动窗口
 *   - 历史最大值（题424）：maxFreq不需要精确维护，利用单调性优化
 *   - 匹配计数（题567）：matchedCount避免每次比较整个频率数组
 */

// ============================================================================
// 题目6：替换后的最长重复字符 (LeetCode 424, Medium)
// ============================================================================
//
// 题目描述：
//   给你一个字符串 s 和一个整数 k。你可以选择字符串中的任一字符，
//   并将其替换为其他大写英文字母，最多可执行 k 次。
//   在执行上述操作后，返回包含相同字母的最长子字符串的长度。
//
// 示例：
//   输入: s = "ABAB", k = 2             输出: 4  （替换两个'A'为'B'或反之）
//   输入: s = "AABABBA", k = 1          输出: 4  （将中间的'A'替换为'B'得到"AABBBBA"->"BBBB"）
//
// 滑动窗口移动过程示意 (s = "AABABBA", k = 1)：
//
//   核心思想：窗口合法条件 = 窗口长度 - 窗口中出现次数最多的字符的次数 <= k
//   即：需要替换的字符数 = 窗口大小 - 最多字符出现次数
//
//   right=0: [A] A B A B B A    maxFreq=1, 窗口长=1, 替换数=0 <= 1
//   right=1: [A A] B A B B A    maxFreq=2, 窗口长=2, 替换数=0 <= 1
//   right=2: [A A B] A B B A    maxFreq=2, 窗口长=3, 替换数=1 <= 1
//   right=3: [A A B A] B B A    maxFreq=3, 窗口长=4, 替换数=1 <= 1
//   right=4: [A A B A B] B A    maxFreq=3, 窗口长=5, 替换数=2 > 1! 收缩
//            A [A B A B] B A    maxFreq=3, 窗口长=4, 替换数=1 <= 1
//   right=5: A [A B A B B] A    maxFreq=3, 窗口长=5, 替换数=2 > 1! 收缩
//            A A [B A B B] A    maxFreq=3, 窗口长=4, 替换数=1 <= 1
//   right=6: A A [B A B B A]    maxFreq=3, 窗口长=5, 替换数=2 > 1! 收缩
//            A A B [A B B A]    maxFreq=3, 窗口长=4, 替换数=1 <= 1
//   最终答案: 4

// ---------- 方法1：滑动窗口 + 频率数组 O(26n) ----------
// 维护窗口中每个字符的频率，当窗口长度 - maxFreq > k 时收缩
int characterReplacement_slidingwindow(string s, int k) {
    vector<int> freq(26, 0);             // 记录窗口中每个大写字母的出现次数
    int left = 0;                        // 窗口左边界
    int maxFreq = 0;                     // 窗口中出现次数最多的字符的频率
    int maxLen = 0;                      // 最终结果：最长子串长度

    for (int right = 0; right < (int)s.size(); right++) { // 右指针扩展窗口
        freq[s[right] - 'A']++;          // 当前字符频率加1
        maxFreq = max(maxFreq, freq[s[right] - 'A']); // 更新最大频率

        // 窗口长度 - 最多字符频率 = 需要替换的字符数
        // 如果需要替换的字符数超过k，说明窗口不合法，需要收缩
        while (right - left + 1 - maxFreq > k) {
            freq[s[left] - 'A']--;       // 左边界字符频率减1
            left++;                      // 左边界右移
            // 注意：这里不需要更新maxFreq，因为我们要的是历史最大值
            // 如果maxFreq变小，窗口只会更不合法，不会产生更优解
        }

        maxLen = max(maxLen, right - left + 1); // 更新最大长度
    }
    return maxLen;                       // 返回结果
}
// 时间复杂度: O(n)，虽然有内层while，但left最多移动n次
// 空间复杂度: O(1)（固定26个字母）
// 关键点：maxFreq不需要精确更新，因为只有maxFreq变大时答案才可能更优

// ---------- 方法2：滑动窗口（不收缩优化）O(n) ----------
// 进一步优化：窗口只在maxFreq变大时才真正"有效"地扩大
int characterReplacement_optimized(string s, int k) {
    vector<int> freq(26, 0);             // 记录窗口中每个大写字母的出现次数
    int left = 0;                        // 窗口左边界
    int maxFreq = 0;                     // 历史最大频率
    int maxLen = 0;                      // 最终结果

    for (int right = 0; right < (int)s.size(); right++) { // 右指针遍历
        freq[s[right] - 'A']++;          // 当前字符频率加1
        maxFreq = max(maxFreq, freq[s[right] - 'A']); // 更新历史最大频率

        // 只需要检查窗口是否合法，left最多右移一次
        if (right - left + 1 - maxFreq > k) {
            freq[s[left] - 'A']--;       // 移除左边界字符
            left++;                      // 左边界右移一位
        }

        maxLen = max(maxLen, right - left + 1); // 更新最大长度
    }
    return maxLen;
}
// 时间复杂度: O(n)，空间复杂度: O(1)
// 与方法1本质相同，但更简洁——left每次最多移动1步

// ============================================================================
// 题目7：K个不同整数的子数组 (LeetCode 992, Hard)
// ============================================================================
//
// 题目描述：
//   给定一个正整数数组 nums 和一个整数 k，返回含有 k 个不同整数的子数组数目。
//
// 示例：
//   输入: nums = [1,2,1,2,3], k = 2    输出: 7  （子数组为[1,2],[2,1],[1,2],[2,3],[1,2,1],[2,1,2],[1,2,1,2]）
//   输入: nums = [1,2,1,3,4], k = 3    输出: 3  （子数组为[1,2,1,3],[2,1,3],[2,1,3,4]）
//
// 核心技巧：「恰好K个」= 「最多K个」- 「最多K-1个」
//   exactlyK(K) = atMostK(K) - atMostK(K-1)
//   这是处理"恰好"类问题的经典技巧！
//
// 滑动窗口过程示意 (nums = [1,2,1,2,3], k = 2, 计算atMostK(2))：
//
//   right=0: [1] 2 1 2 3    distinct=1, 贡献1个子数组: [1]
//   right=1: [1 2] 1 2 3    distinct=2, 贡献2个: [1,2],[2]
//   right=2: [1 2 1] 2 3    distinct=2, 贡献3个: [1,2,1],[2,1],[1]
//   right=3: [1 2 1 2] 3    distinct=2, 贡献4个: [1,2,1,2],[2,1,2],[1,2],[2]
//   right=4: [1 2 1 2 3]    distinct=3 > 2! 收缩
//            1 [2 1 2 3]    distinct=3 > 2! 继续收缩
//            1 2 [1 2 3]    distinct=3 > 2! 继续收缩
//            1 2 1 [2 3]    distinct=2, 贡献2个: [2,3],[3]
//   atMostK(2) = 1+2+3+4+0+0+0+2 = 12
//   atMostK(1) = 1+0+1+0+0+0+0+1 = 5
//   exactlyK(2) = 12 - 5 = 7

// ---------- 辅助函数：计算最多包含k个不同整数的子数组数目 ----------
int atMostK(vector<int>& nums, int k) {
    unordered_map<int, int> freq;        // 窗口中每个数字的频率
    int left = 0;                        // 窗口左边界
    int count = 0;                       // 子数组计数

    for (int right = 0; right < (int)nums.size(); right++) { // 右指针扩展
        freq[nums[right]]++;             // 当前数字频率加1

        // 当窗口中不同整数超过k时，收缩窗口
        while ((int)freq.size() > k) {
            freq[nums[left]]--;          // 左边界数字频率减1
            if (freq[nums[left]] == 0) { // 如果频率变为0
                freq.erase(nums[left]);  // 从哈希表中移除
            }
            left++;                      // 左边界右移
        }

        // 以right为右端点、包含在当前窗口内的子数组数目 = right - left + 1
        count += right - left + 1;
    }
    return count;                        // 返回子数组数目
}

// ---------- 方法1：滑动窗口（atMostK差值法）O(n) ----------
int subarraysWithKDistinct(vector<int>& nums, int k) {
    return atMostK(nums, k) - atMostK(nums, k - 1); // 恰好K = 最多K - 最多(K-1)
}
// 时间复杂度: O(n)，两次遍历
// 空间复杂度: O(k)，哈希表最多存储k个不同元素
// 核心思想：「恰好K个」的计数可以通过「最多K个」的差值高效计算

// ---------- 方法2：暴力法 O(n^2) ----------
// 枚举所有子数组，统计不同整数个数
int subarraysWithKDistinct_bruteforce(vector<int>& nums, int k) {
    int n = (int)nums.size();            // 数组长度
    int count = 0;                       // 结果计数

    for (int i = 0; i < n; i++) {        // 枚举左端点
        unordered_set<int> seen;         // 记录当前子数组中的不同整数
        for (int j = i; j < n; j++) {    // 枚举右端点
            seen.insert(nums[j]);        // 将当前数字加入集合
            if ((int)seen.size() == k) count++;  // 恰好k个不同整数，计数加1
            if ((int)seen.size() > k) break;     // 超过k个，后续只会更多，提前退出
        }
    }
    return count;                        // 返回结果
}
// 时间复杂度: O(n^2)，空间复杂度: O(k)

// ============================================================================
// 题目8：包含排列的子串 (LeetCode 567, Medium)
// ============================================================================
//
// 题目描述：
//   给定两个字符串 s1 和 s2，写一个函数来判断 s2 是否包含 s1 的排列。
//   换句话说，s1 的一个排列之一是 s2 的子串则返回 true。
//
// 示例：
//   输入: s1 = "ab", s2 = "eidbaooo"    输出: true  （s2包含"ba"，是"ab"的排列）
//   输入: s1 = "ab", s2 = "eidboaoo"    输出: false
//
// 滑动窗口移动过程示意 (s1 = "ab", s2 = "eidbaooo")：
//   s1的频率表: {a:1, b:1}，窗口大小固定为2
//
//   i=0: [e i] d b a o o o    窗口频率={e:1,i:1} != {a:1,b:1}
//   i=1:  e [i d] b a o o o   窗口频率={i:1,d:1} != {a:1,b:1}
//   i=2:  e i [d b] a o o o   窗口频率={d:1,b:1} != {a:1,b:1}
//   i=3:  e i d [b a] o o o   窗口频率={a:1,b:1} == {a:1,b:1}  -> true!
//   最终答案: true

// ---------- 方法1：滑动窗口 + 频率数组 O(n) ----------
// 固定窗口大小为s1.length()，用频率数组比较
bool checkInclusion_slidingwindow(string s1, string s2) {
    int len1 = (int)s1.size(), len2 = (int)s2.size(); // 获取两个字符串的长度
    if (len1 > len2) return false;       // 如果s1比s2长，不可能包含排列

    vector<int> cnt1(26, 0);             // s1的字符频率表
    vector<int> cnt2(26, 0);             // 当前窗口的字符频率表

    for (int i = 0; i < len1; i++) {     // 初始化：统计s1频率 + 第一个窗口频率
        cnt1[s1[i] - 'a']++;            // 统计s1中字符频率
        cnt2[s2[i] - 'a']++;            // 统计s2前len1个字符的频率
    }

    if (cnt1 == cnt2) return true;       // 如果第一个窗口就是排列，直接返回true

    for (int i = len1; i < len2; i++) {  // 从第二个窗口开始滑动
        cnt2[s2[i] - 'a']++;            // 加入右边界新字符
        cnt2[s2[i - len1] - 'a']--;     // 移除左边界的旧字符
        if (cnt1 == cnt2) return true;   // 如果频率匹配，说明找到了排列
    }
    return false;                        // 遍历完都没找到，返回false
}
// 时间复杂度: O(n)，空间复杂度: O(1)（固定26个字母）

// ---------- 方法2：滑动窗口 + 匹配计数 O(n) ----------
// 优化：用matchedCount跟踪已匹配的字符种类数，避免每次比较整个频率数组
bool checkInclusion_optimized(string s1, string s2) {
    int len1 = (int)s1.size(), len2 = (int)s2.size();
    if (len1 > len2) return false;

    vector<int> freq(26, 0);             // 频率差值表：正数表示还需要，负数表示多余
    for (int i = 0; i < len1; i++) {
        freq[s1[i] - 'a']++;            // s1中的字符需要
        freq[s2[i] - 'a']--;            // 窗口中已有的字符消耗
    }

    // 统计有多少个字符的频率恰好为0（即已完全匹配）
    int matched = 0;
    for (int i = 0; i < 26; i++) {
        if (freq[i] == 0) matched++;
    }

    for (int i = len1; i < len2; i++) {
        if (matched == 26) return true;  // 全部26个字符都匹配，找到排列

        int r = s2[i] - 'a';            // 右边界新加入的字符
        int l = s2[i - len1] - 'a';     // 左边界即将移出的字符

        // 处理右边界字符：加入前是否匹配 -> 加入后是否匹配
        if (freq[r] == 0) matched--;     // 加入前恰好匹配，加入后将不匹配
        freq[r]--;                       // 频率减1（表示消耗了一个需求）
        if (freq[r] == 0) matched++;     // 加入后恰好匹配

        // 处理左边界字符：移出前是否匹配 -> 移出后是否匹配
        if (freq[l] == 0) matched--;     // 移出前恰好匹配，移出后将不匹配
        freq[l]++;                       // 频率加1（表示释放了一个需求）
        if (freq[l] == 0) matched++;     // 移出后恰好匹配
    }

    return matched == 26;                // 最后检查一次
}
// 时间复杂度: O(n)，空间复杂度: O(1)
// 优化点：避免了每次比较两个长度为26的数组，而是维护一个匹配计数器

// ---------- 方法3：暴力法 O(n * k) ----------
bool checkInclusion_bruteforce(string s1, string s2) {
    int len1 = (int)s1.size(), len2 = (int)s2.size();
    if (len1 > len2) return false;

    string sortedS1 = s1;
    sort(sortedS1.begin(), sortedS1.end()); // 对s1排序作为参考

    for (int i = 0; i <= len2 - len1; i++) { // 枚举每个长度为len1的子串
        string sub = s2.substr(i, len1); // 取出子串
        sort(sub.begin(), sub.end());    // 对子串排序
        if (sub == sortedS1) return true; // 如果排序后相等，说明是排列
    }
    return false;
}
// 时间复杂度: O(n * k log k)，其中k = len1

// ============================================================================
// 题目9：长度为K的最大子数组和 (LeetCode 2461, Medium)
// ============================================================================
//
// 题目描述：
//   给你一个整数数组 nums 和一个整数 k，返回长度为 k 的子数组中的最大和。
//   且子数组中的元素互不相同。如果不存在这样的子数组，返回 0。
//
// 示例：
//   输入: nums = [1,5,4,2,9,9,9], k = 3    输出: 15  （子数组[5,4,2]或[4,2,9]）
//   输入: nums = [4,4,4], k = 3             输出: 0   （不存在长度为3且元素互不相同的子数组）
//
// 滑动窗口移动过程示意 (nums = [1,5,4,2,9,9,9], k = 3)：
//
//   right=0: [1] 5 4 2 9 9 9    distinct=1, sum=1
//   right=1: [1 5] 4 2 9 9 9    distinct=2, sum=6
//   right=2: [1 5 4] 2 9 9 9    distinct=3, sum=10, 窗口大小=k! 记录maxSum=10
//   right=3: 1 [5 4 2] 9 9 9    distinct=3, sum=11, 窗口大小=k! maxSum=11
//   right=4: 1 5 [4 2 9] 9 9    distinct=3, sum=15, 窗口大小=k! maxSum=15
//   right=5: 1 5 4 [2 9 9] 9    distinct=2! 重复的9,收缩
//            1 5 4 2 [9 9] 9    distinct=1! 继续收缩
//            1 5 4 2 9 [9] 9    distinct=1, 窗口大小 < k
//   right=6: 1 5 4 2 9 [9 9]    distinct=1! 重复,收缩
//            1 5 4 2 9 9 [9]    distinct=1, 窗口大小 < k
//   最终答案: 15

// ---------- 方法1：滑动窗口 + 哈希表 O(n) ----------
// 维护一个大小不超过k、元素互不相同的滑动窗口
long long maximumSubarraySum(vector<int>& nums, int k) {
    int n = (int)nums.size();            // 数组长度
    if (n < k) return 0;                 // 如果数组长度小于k，无解

    unordered_map<int, int> freq;        // 窗口中每个元素的频率
    long long windowSum = 0;             // 当前窗口的和
    long long maxSum = 0;                // 最大和
    int left = 0;                        // 窗口左边界

    for (int right = 0; right < n; right++) { // 右指针扩展窗口
        freq[nums[right]]++;             // 当前元素频率加1
        windowSum += nums[right];        // 加入窗口和

        // 如果窗口大小超过k，或者有重复元素，收缩窗口
        while (right - left + 1 > k || freq[nums[right]] > 1) {
            freq[nums[left]]--;          // 左边界元素频率减1
            windowSum -= nums[left];     // 从窗口和中移除
            left++;                      // 左边界右移
        }

        // 窗口大小恰好为k且元素互不相同时更新答案
        if (right - left + 1 == k) {
            maxSum = max(maxSum, windowSum);
        }
    }
    return maxSum;                       // 返回最大和
}
// 时间复杂度: O(n)，空间复杂度: O(k)
// 关键点：收缩条件有两个——窗口过大 或 出现重复元素

// ---------- 方法2：暴力法 O(nk) ----------
long long maximumSubarraySum_bruteforce(vector<int>& nums, int k) {
    int n = (int)nums.size();            // 数组长度
    long long maxSum = 0;                // 最大和

    for (int i = 0; i <= n - k; i++) {   // 枚举每个长度为k的起始位置
        unordered_set<int> seen;         // 记录当前窗口中的元素
        long long sum = 0;               // 当前子数组和
        bool valid = true;               // 标记是否所有元素互不相同

        for (int j = i; j < i + k; j++) { // 遍历长度为k的窗口
            if (seen.count(nums[j])) {   // 如果有重复元素
                valid = false;           // 标记无效
                break;                   // 提前退出
            }
            seen.insert(nums[j]);        // 将元素加入集合
            sum += nums[j];             // 累加和
        }

        if (valid) {                     // 如果窗口有效
            maxSum = max(maxSum, sum);   // 更新最大和
        }
    }
    return maxSum;                       // 返回结果
}
// 时间复杂度: O(nk)，空间复杂度: O(k)

// ============================================================================
// 题目10：将x减到0的最少操作数 (LeetCode 1658, Medium)
// ============================================================================
//
// 题目描述：
//   给你一个整数数组 nums 和一个整数 x。每一次操作时，
//   你应当移除数组 nums 最左边或最右边的元素，然后从 x 中减去该元素的值。
//   返回将 x 恰好减到 0 所需要的最少操作数。
//   如果不可能，返回 -1。
//
// 示例：
//   输入: nums = [1,1,4,2,3], x = 5    输出: 2  （移除最左1和最右3 -> 剩[1,4,2]）
//   输入: nums = [5,6,7,8,9], x = 4    输出: -1
//   输入: nums = [3,2,20,1,1,3], x = 10 输出: 5 （移除[3,2,20,1,1]中的3,2和3,1,1）
//
// 核心转化：
//   从两端移除使和等于x  <=>  找中间连续子数组使其和等于 totalSum - x
//   最少操作数 = 数组长度 - 最长满足条件的子数组长度
//
// 滑动窗口过程示意 (nums = [1,1,4,2,3], x = 5)：
//   totalSum = 11, target = totalSum - x = 6
//   目标：找和为6的最长子数组
//
//   right=0: [1] 1 4 2 3    sum=1 < 6
//   right=1: [1 1] 4 2 3    sum=2 < 6
//   right=2: [1 1 4] 2 3    sum=7 > 6! 收缩
//            1 [1 4] 2 3    sum=5 < 6
//   right=3: 1 [1 4 2] 3    sum=7 > 6! 收缩
//            1 1 [4 2] 3    sum=6 == 6! 记录长度2
//            1 1 4 [2] 3    sum=2 < 6
//   right=4: 1 1 4 [2 3]    sum=5 < 6
//   最长子数组长度 = 2
//   最少操作数 = 5 - 2 = 3... 但实际上最优是2
//
//   更正：最优解从两端移除[1,3]得到0，操作2次
//   对应中间子数组[1,4,2]和=7, totalSum-x=6, 不匹配
//   实际上[1,4,2]和=7 != 6
//   正确理解：两端移除的和=x=5, 移除[1,3]=4!=5, 移除[1,1,3]=5, 操作3次
//   或移除[1,2,3]... 不行
//   最优解：移除左边1个(1)和右边1个(3): 和=1+3=4!=5
//   移除左边2个(1,1)和右边1个(3): 和=1+1+3=5, 操作3次
//   移除左边1个(1)和右边2个(2,3): 和=1+2+3=6!=5
//   移除右边2个(3,2): 和=3+2=5, 操作2次!
//   对应中间子数组[1,1,4]和=6 = totalSum-x=11-5=6, 长度3
//   操作数 = 5-3 = 2 ✓
//
//   right=0: [1] 1 4 2 3    sum=1 < 6
//   right=1: [1 1] 4 2 3    sum=2 < 6
//   right=2: [1 1 4] 2 3    sum=6 == 6! 记录长度3
//   right=3: [1 1 4 2] 3    sum=8 > 6! 收缩
//            1 [1 4 2] 3    sum=7 > 6! 收缩
//            1 1 [4 2] 3    sum=6 == 6! 记录长度2
//            1 1 4 [2] 3    sum=2 < 6
//   right=4: 1 1 4 [2 3]    sum=5 < 6
//   最长子数组长度 = 3, 最少操作数 = 5-3 = 2 ✓

// ---------- 方法1：滑动窗口（转化为最长子数组）O(n) ----------
// 将问题转化为：找和为 totalSum-x 的最长子数组
int minOperations_slidingwindow(vector<int>& nums, int x) {
    int n = (int)nums.size();            // 数组长度
    int totalSum = 0;                    // 数组总和
    for (int num : nums) totalSum += num;

    int target = totalSum - x;           // 需要在中间找到的子数组和
    if (target < 0) return -1;           // 如果target为负，无解
    if (target == 0) return n;           // 如果target为0，需要移除所有元素

    int left = 0;                        // 窗口左边界
    int sum = 0;                         // 当前窗口和
    int maxLen = -1;                     // 满足条件的最长子数组长度，-1表示未找到

    for (int right = 0; right < n; right++) { // 右指针扩展窗口
        sum += nums[right];              // 将右指针元素加入窗口和

        // 当窗口和超过target时，收缩窗口
        while (sum > target && left <= right) {
            sum -= nums[left];           // 移除左边界元素
            left++;                      // 左边界右移
        }

        // 如果窗口和恰好等于target，更新最长长度
        if (sum == target) {
            maxLen = max(maxLen, right - left + 1);
        }
    }

    return maxLen == -1 ? -1 : n - maxLen; // 如果没找到返回-1，否则返回操作数
}
// 时间复杂度: O(n)，空间复杂度: O(1)
// 关键转化：两端移除的问题转化为中间保留的最长子数组问题

// ---------- 方法2：前缀和 + 哈希表 O(n) ----------
// 找和为target的最长子数组，用前缀和+哈希表
int minOperations_prefixsum(vector<int>& nums, int x) {
    int n = (int)nums.size();            // 数组长度
    int totalSum = 0;                    // 数组总和
    for (int num : nums) totalSum += num;

    int target = totalSum - x;           // 目标子数组和
    if (target < 0) return -1;           // 无解
    if (target == 0) return n;           // 全部移除

    unordered_map<int, int> prefixMap;   // 前缀和 -> 最早出现的位置
    prefixMap[0] = -1;                   // 空子数组的前缀和为0，位置为-1
    int sum = 0;                         // 当前前缀和
    int maxLen = -1;                     // 最长子数组长度

    for (int i = 0; i < n; i++) {        // 遍历数组
        sum += nums[i];                  // 累加前缀和
        // 如果存在一个之前的前缀和使得 sum - prefixSum == target
        if (prefixMap.count(sum - target)) {
            maxLen = max(maxLen, i - prefixMap[sum - target]); // 更新最长长度
        }
        if (!prefixMap.count(sum)) {     // 只记录最早的出现位置
            prefixMap[sum] = i;
        }
    }

    return maxLen == -1 ? -1 : n - maxLen; // 返回结果
}
// 时间复杂度: O(n)，空间复杂度: O(n)
// 两种方法殊途同归，方法1用滑动窗口（因为元素为正整数），方法2用前缀和

// ---------- 方法3：暴力法 O(n^2) ----------
int minOperations_bruteforce(vector<int>& nums, int x) {
    int n = (int)nums.size();            // 数组长度
    int minOps = INT_MAX;                // 最少操作数

    for (int left = 0; left <= n; left++) {      // 枚举左边移除的数量
        for (int right = 0; right <= n - left; right++) { // 枚举右边移除的数量
            int removedSum = 0;          // 移除元素的和
            for (int i = 0; i < left; i++) removedSum += nums[i];     // 左边移除的
            for (int i = n - right; i < n; i++) removedSum += nums[i]; // 右边移除的
            if (removedSum == x) {       // 如果移除的和等于x
                minOps = min(minOps, left + right); // 更新最少操作数
            }
        }
    }
    return minOps == INT_MAX ? -1 : minOps; // 如果没找到返回-1
}
// 时间复杂度: O(n^2)，空间复杂度: O(1)

// ============================================================================
// 何时使用滑动窗口 vs 其他技巧（更新版）
// ============================================================================
int main() {
    cout << "========================================" << endl;
    cout << "  滑动窗口专题 - 10道经典力扣题目" << endl;
    cout << "========================================" << endl;

    // ----- 题目1测试 -----
    cout << "\n--- 题目1: 无重复字符的最长子串 (LeetCode 3) ---" << endl;

    cout << "方法1 - 暴力法:" << endl;
    cout << "  \"abcabcbb\" -> " << lengthOfLongestSubstring_bruteforce("abcabcbb")
         << " (期望: 3)" << endl;
    cout << "  \"bbbbb\"    -> " << lengthOfLongestSubstring_bruteforce("bbbbb")
         << " (期望: 1)" << endl;
    cout << "  \"pwwkew\"   -> " << lengthOfLongestSubstring_bruteforce("pwwkew")
         << " (期望: 3)" << endl;
    cout << "  \"\"         -> " << lengthOfLongestSubstring_bruteforce("")
         << " (期望: 0)" << endl;
    cout << "  \"au\"       -> " << lengthOfLongestSubstring_bruteforce("au")
         << " (期望: 2)" << endl;

    cout << "方法2 - HashSet:" << endl;
    cout << "  \"abcabcbb\" -> " << lengthOfLongestSubstring_hashset("abcabcbb")
         << " (期望: 3)" << endl;
    cout << "  \"bbbbb\"    -> " << lengthOfLongestSubstring_hashset("bbbbb")
         << " (期望: 1)" << endl;
    cout << "  \"pwwkew\"   -> " << lengthOfLongestSubstring_hashset("pwwkew")
         << " (期望: 3)" << endl;

    cout << "方法3 - 数组索引(最优):" << endl;
    cout << "  \"abcabcbb\" -> " << lengthOfLongestSubstring_array("abcabcbb")
         << " (期望: 3)" << endl;
    cout << "  \"bbbbb\"    -> " << lengthOfLongestSubstring_array("bbbbb")
         << " (期望: 1)" << endl;
    cout << "  \"pwwkew\"   -> " << lengthOfLongestSubstring_array("pwwkew")
         << " (期望: 3)" << endl;
    // 预期输出:
    // "abcabcbb" -> 3, "bbbbb" -> 1, "pwwkew" -> 3, "" -> 0, "au" -> 2

    // ----- 题目2测试 -----
    cout << "\n--- 题目2: 找到字符串中所有字母异位词 (LeetCode 438) ---" << endl;

    cout << "方法1 - 排序法:" << endl;
    auto res1 = findAnagrams_sort("cbaebabacd", "abc");
    cout << "  s=\"cbaebabacd\", p=\"abc\" -> [";
    for (int i = 0; i < (int)res1.size(); i++) {
        if (i) cout << ", ";
        cout << res1[i];
    }
    cout << "] (期望: [0, 6])" << endl;
    // 预期输出: [0, 6]

    auto res2 = findAnagrams_sort("abab", "ab");
    cout << "  s=\"abab\", p=\"ab\" -> [";
    for (int i = 0; i < (int)res2.size(); i++) {
        if (i) cout << ", ";
        cout << res2[i];
    }
    cout << "] (期望: [0, 1, 2])" << endl;
    // 预期输出: [0, 1, 2]

    cout << "方法2 - 滑动窗口:" << endl;
    auto res3 = findAnagrams_slidingwindow("cbaebabacd", "abc");
    cout << "  s=\"cbaebabacd\", p=\"abc\" -> [";
    for (int i = 0; i < (int)res3.size(); i++) {
        if (i) cout << ", ";
        cout << res3[i];
    }
    cout << "] (期望: [0, 6])" << endl;
    // 预期输出: [0, 6]

    auto res4 = findAnagrams_slidingwindow("abab", "ab");
    cout << "  s=\"abab\", p=\"ab\" -> [";
    for (int i = 0; i < (int)res4.size(); i++) {
        if (i) cout << ", ";
        cout << res4[i];
    }
    cout << "] (期望: [0, 1, 2])" << endl;
    // 预期输出: [0, 1, 2]

    auto res5 = findAnagrams_slidingwindow("aaaaaaaaaa", "aaaaaaaaaaaaa");
    cout << "  s=\"aaaaaaaaaa\", p=\"aaaaaaaaaaaaa\" -> [";
    for (int i = 0; i < (int)res5.size(); i++) {
        if (i) cout << ", ";
        cout << res5[i];
    }
    cout << "] (期望: [])" << endl;
    // 预期输出: []

    // ----- 题目3测试 -----
    cout << "\n--- 题目3: 最小覆盖子串 (LeetCode 76) ---" << endl;

    cout << "  s=\"ADOBECODEBANC\", t=\"ABC\" -> \""
         << minWindow("ADOBECODEBANC", "ABC") << "\" (期望: \"BANC\")" << endl;
    cout << "  s=\"a\", t=\"a\"               -> \""
         << minWindow("a", "a") << "\" (期望: \"a\")" << endl;
    cout << "  s=\"a\", t=\"aa\"              -> \""
         << minWindow("a", "aa") << "\" (期望: \"\")" << endl;
    cout << "  s=\"ab\", t=\"b\"              -> \""
         << minWindow("ab", "b") << "\" (期望: \"b\")" << endl;
    cout << "  s=\"aa\", t=\"aa\"             -> \""
         << minWindow("aa", "aa") << "\" (期望: \"aa\")" << endl;
    // 预期输出:
    // "BANC", "a", "", "b", "aa"

    // ----- 题目4测试 -----
    cout << "\n--- 题目4: 最大连续1的个数 III (LeetCode 1004) ---" << endl;

    vector<int> nums1 = {1, 1, 0, 0, 1, 1, 1, 0, 1};
    cout << "  nums=[1,1,0,0,1,1,1,0,1], k=2 -> "
         << longestOnes(nums1, 2) << " (期望: 7)" << endl;

    vector<int> nums2 = {0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1};
    cout << "  nums=[0,0,1,1,...,1,1], k=3   -> "
         << longestOnes(nums2, 3) << " (期望: 10)" << endl;

    vector<int> nums3 = {1, 1, 1, 1, 1};
    cout << "  nums=[1,1,1,1,1], k=0         -> "
         << longestOnes(nums3, 0) << " (期望: 5)" << endl;

    vector<int> nums4 = {0, 0, 0, 0};
    cout << "  nums=[0,0,0,0], k=4           -> "
         << longestOnes(nums4, 4) << " (期望: 4)" << endl;
    // 预期输出: 7, 10, 5, 4

    // ----- 题目5测试 -----
    cout << "\n--- 题目5: 长度最小的子数组 (LeetCode 209) ---" << endl;

    cout << "方法1 - 暴力法:" << endl;
    vector<int> arr1 = {2, 3, 1, 2, 4, 3};
    cout << "  target=7, nums=[2,3,1,2,4,3]     -> "
         << minSubArrayLen_bruteforce(7, arr1) << " (期望: 2)" << endl;
    vector<int> arr2 = {1, 4, 4};
    cout << "  target=4, nums=[1,4,4]            -> "
         << minSubArrayLen_bruteforce(4, arr2) << " (期望: 1)" << endl;
    vector<int> arr3 = {1, 1, 1, 1, 1, 1, 1, 1};
    cout << "  target=11, nums=[1,1,1,1,1,1,1,1] -> "
         << minSubArrayLen_bruteforce(11, arr3) << " (期望: 0)" << endl;

    cout << "方法2 - 滑动窗口:" << endl;
    vector<int> arr4 = {2, 3, 1, 2, 4, 3};
    cout << "  target=7, nums=[2,3,1,2,4,3]     -> "
         << minSubArrayLen_slidingwindow(7, arr4) << " (期望: 2)" << endl;
    vector<int> arr5 = {1, 4, 4};
    cout << "  target=4, nums=[1,4,4]            -> "
         << minSubArrayLen_slidingwindow(4, arr5) << " (期望: 1)" << endl;
    vector<int> arr6 = {1, 1, 1, 1, 1, 1, 1, 1};
    cout << "  target=11, nums=[1,1,1,1,1,1,1,1] -> "
         << minSubArrayLen_slidingwindow(11, arr6) << " (期望: 0)" << endl;
    vector<int> arr7 = {1, 2, 3, 4, 5};
    cout << "  target=15, nums=[1,2,3,4,5]       -> "
         << minSubArrayLen_slidingwindow(15, arr7) << " (期望: 5)" << endl;
    vector<int> arr8 = {1, 2, 3, 4, 5};
    cout << "  target=100, nums=[1,2,3,4,5]      -> "
         << minSubArrayLen_slidingwindow(100, arr8) << " (期望: 0)" << endl;
    // 预期输出: 2, 1, 0, 5, 0

    // ----- 边界情况额外测试 -----
    cout << "\n--- 边界情况额外测试 ---" << endl;

    // 题目3边界: 单字符
    cout << "  minWindow(\"a\",\"a\")          -> \""
         << minWindow("a", "a") << "\" (期望: \"a\")" << endl;
    cout << "  minWindow(\"aa\",\"a\")         -> \""
         << minWindow("aa", "a") << "\" (期望: \"a\")" << endl;

    // 题目4边界: 全0数组，k=0
    vector<int> allZero = {0, 0, 0};
    cout << "  longestOnes([0,0,0], k=0)  -> "
         << longestOnes(allZero, 0) << " (期望: 0)" << endl;

    // 题目4边界: 单元素
    vector<int> singleOne = {1};
    cout << "  longestOnes([1], k=0)      -> "
         << longestOnes(singleOne, 0) << " (期望: 1)" << endl;
    vector<int> singleZero = {0};
    cout << "  longestOnes([0], k=1)      -> "
         << longestOnes(singleZero, 1) << " (期望: 1)" << endl;

    // 题目5边界: 单元素恰好等于target
    vector<int> singleArr = {7};
    cout << "  minSubArrayLen(7, [7])     -> "
         << minSubArrayLen_slidingwindow(7, singleArr) << " (期望: 1)" << endl;

    // 题目5边界: 全数组和刚好等于target
    vector<int> exactArr = {2, 3, 2};
    cout << "  minSubArrayLen(7, [2,3,2]) -> "
         << minSubArrayLen_slidingwindow(7, exactArr) << " (期望: 3)" << endl;

    // ----- 所有方法交叉验证 -----
    cout << "\n--- 交叉验证（所有三种方法结果一致） ---" << endl;
    vector<string> testCases = {"abcabcbb", "bbbbb", "pwwkew", "au", "dvdf", "anviaj"};
    for (const string& tc : testCases) {
        int r1 = lengthOfLongestSubstring_bruteforce(tc);
        int r2 = lengthOfLongestSubstring_hashset(tc);
        int r3 = lengthOfLongestSubstring_array(tc);
        string status = (r1 == r2 && r2 == r3) ? "PASS" : "FAIL";
        cout << "  \"" << tc << "\": brute=" << r1
             << " hashset=" << r2 << " array=" << r3
             << " -> " << status << endl;
    }
    // 预期输出:
    // "abcabcbb": 3, "bbbbb": 1, "pwwkew": 3, "au": 2, "dvdf": 3, "anviaj": 5

    // ----- 题目6测试 -----
    cout << "\n--- 题目6: 替换后的最长重复字符 (LeetCode 424) ---" << endl;

    cout << "方法1 - 滑动窗口:" << endl;
    cout << "  s=\"ABAB\", k=2       -> " << characterReplacement_slidingwindow("ABAB", 2)
         << " (期望: 4)" << endl;
    cout << "  s=\"AABABBA\", k=1   -> " << characterReplacement_slidingwindow("AABABBA", 1)
         << " (期望: 4)" << endl;
    cout << "  s=\"AAAA\", k=2      -> " << characterReplacement_slidingwindow("AAAA", 2)
         << " (期望: 4)" << endl;
    cout << "  s=\"ABBB\", k=2      -> " << characterReplacement_slidingwindow("ABBB", 2)
         << " (期望: 4)" << endl;
    cout << "  s=\"A\", k=0         -> " << characterReplacement_slidingwindow("A", 0)
         << " (期望: 1)" << endl;

    cout << "方法2 - 优化版:" << endl;
    cout << "  s=\"ABAB\", k=2       -> " << characterReplacement_optimized("ABAB", 2)
         << " (期望: 4)" << endl;
    cout << "  s=\"AABABBA\", k=1   -> " << characterReplacement_optimized("AABABBA", 1)
         << " (期望: 4)" << endl;
    // 预期输出: 4, 4, 4, 4, 1

    // ----- 题目7测试 -----
    cout << "\n--- 题目7: K个不同整数的子数组 (LeetCode 992) ---" << endl;

    cout << "方法1 - atMostK差值法:" << endl;
    vector<int> nums7a = {1, 2, 1, 2, 3};
    cout << "  nums=[1,2,1,2,3], k=2 -> "
         << subarraysWithKDistinct(nums7a, 2) << " (期望: 7)" << endl;
    vector<int> nums7b = {1, 2, 1, 3, 4};
    cout << "  nums=[1,2,1,3,4], k=3 -> "
         << subarraysWithKDistinct(nums7b, 3) << " (期望: 3)" << endl;
    vector<int> nums7c = {1, 1, 1, 1};
    cout << "  nums=[1,1,1,1], k=2   -> "
         << subarraysWithKDistinct(nums7c, 2) << " (期望: 0)" << endl;
    vector<int> nums7d = {1, 2, 3};
    cout << "  nums=[1,2,3], k=1     -> "
         << subarraysWithKDistinct(nums7d, 1) << " (期望: 3)" << endl;

    cout << "方法2 - 暴力法:" << endl;
    vector<int> nums7e = {1, 2, 1, 2, 3};
    cout << "  nums=[1,2,1,2,3], k=2 -> "
         << subarraysWithKDistinct_bruteforce(nums7e, 2) << " (期望: 7)" << endl;
    // 预期输出: 7, 3, 0, 3, 7

    // ----- 题目8测试 -----
    cout << "\n--- 题目8: 包含排列的子串 (LeetCode 567) ---" << endl;

    cout << "方法1 - 滑动窗口:" << endl;
    cout << "  s1=\"ab\", s2=\"eidbaooo\" -> "
         << (checkInclusion_slidingwindow("ab", "eidbaooo") ? "true" : "false")
         << " (期望: true)" << endl;
    cout << "  s1=\"ab\", s2=\"eidboaoo\" -> "
         << (checkInclusion_slidingwindow("ab", "eidboaoo") ? "true" : "false")
         << " (期望: false)" << endl;
    cout << "  s1=\"adc\", s2=\"dcda\"   -> "
         << (checkInclusion_slidingwindow("adc", "dcda") ? "true" : "false")
         << " (期望: true)" << endl;
    cout << "  s1=\"hello\", s2=\"ooolleoooleh\" -> "
         << (checkInclusion_slidingwindow("hello", "ooolleoooleh") ? "true" : "false")
         << " (期望: false)" << endl;

    cout << "方法2 - 优化版(匹配计数):" << endl;
    cout << "  s1=\"ab\", s2=\"eidbaooo\" -> "
         << (checkInclusion_optimized("ab", "eidbaooo") ? "true" : "false")
         << " (期望: true)" << endl;
    cout << "  s1=\"ab\", s2=\"eidboaoo\" -> "
         << (checkInclusion_optimized("ab", "eidboaoo") ? "true" : "false")
         << " (期望: false)" << endl;

    cout << "方法3 - 排序法:" << endl;
    cout << "  s1=\"ab\", s2=\"eidbaooo\" -> "
         << (checkInclusion_bruteforce("ab", "eidbaooo") ? "true" : "false")
         << " (期望: true)" << endl;
    // 预期输出: true, false, true, false

    // ----- 题目9测试 -----
    cout << "\n--- 题目9: 长度为K的最大子数组和 (LeetCode 2461) ---" << endl;

    cout << "方法1 - 滑动窗口:" << endl;
    vector<int> nums9a = {1, 5, 4, 2, 9, 9, 9};
    cout << "  nums=[1,5,4,2,9,9,9], k=3 -> "
         << maximumSubarraySum(nums9a, 3) << " (期望: 15)" << endl;
    vector<int> nums9b = {4, 4, 4};
    cout << "  nums=[4,4,4], k=3          -> "
         << maximumSubarraySum(nums9b, 3) << " (期望: 0)" << endl;
    vector<int> nums9c = {1, 2, 3, 4, 5};
    cout << "  nums=[1,2,3,4,5], k=2      -> "
         << maximumSubarraySum(nums9c, 2) << " (期望: 9)" << endl;
    vector<int> nums9d = {9, 9, 9, 9};
    cout << "  nums=[9,9,9,9], k=2        -> "
         << maximumSubarraySum(nums9d, 2) << " (期望: 0)" << endl;

    cout << "方法2 - 暴力法:" << endl;
    vector<int> nums9e = {1, 5, 4, 2, 9, 9, 9};
    cout << "  nums=[1,5,4,2,9,9,9], k=3 -> "
         << maximumSubarraySum_bruteforce(nums9e, 3) << " (期望: 15)" << endl;
    // 预期输出: 15, 0, 9, 0, 15

    // ----- 题目10测试 -----
    cout << "\n--- 题目10: 将x减到0的最少操作数 (LeetCode 1658) ---" << endl;

    cout << "方法1 - 滑动窗口:" << endl;
    vector<int> nums10a = {1, 1, 4, 2, 3};
    cout << "  nums=[1,1,4,2,3], x=5  -> "
         << minOperations_slidingwindow(nums10a, 5) << " (期望: 2)" << endl;
    vector<int> nums10b = {5, 6, 7, 8, 9};
    cout << "  nums=[5,6,7,8,9], x=4  -> "
         << minOperations_slidingwindow(nums10b, 4) << " (期望: -1)" << endl;
    vector<int> nums10c = {3, 2, 20, 1, 1, 3};
    cout << "  nums=[3,2,20,1,1,3], x=10 -> "
         << minOperations_slidingwindow(nums10c, 10) << " (期望: 5)" << endl;
    vector<int> nums10d = {1, 1};
    cout << "  nums=[1,1], x=3         -> "
         << minOperations_slidingwindow(nums10d, 3) << " (期望: -1)" << endl;
    vector<int> nums10e = {1, 2, 3};
    cout << "  nums=[1,2,3], x=6       -> "
         << minOperations_slidingwindow(nums10e, 6) << " (期望: 3)" << endl;

    cout << "方法2 - 前缀和+哈希表:" << endl;
    vector<int> nums10f = {1, 1, 4, 2, 3};
    cout << "  nums=[1,1,4,2,3], x=5  -> "
         << minOperations_prefixsum(nums10f, 5) << " (期望: 2)" << endl;
    vector<int> nums10g = {5, 6, 7, 8, 9};
    cout << "  nums=[5,6,7,8,9], x=4  -> "
         << minOperations_prefixsum(nums10g, 4) << " (期望: -1)" << endl;
    // 预期输出: 2, -1, 5, -1, 3

    cout << "\n========================================" << endl;
    cout << "  全部测试用例执行完毕！" << endl;
    cout << "========================================" << endl;

    return 0;
}
