/**
 * 10_二分查找.cpp
 * ================
 * 力扣经典二分查找问题
 *
 * 本文件包含10道经典二分查找题目，涵盖:
 *   1. 二分查找 (LeetCode 704, Easy) - 基础二分模板
 *   2. 搜索旋转排序数组 (LeetCode 33, Medium) - 变体二分
 *   3. 寻找旋转排序数组中的最小值 (LeetCode 153, Medium) - 二分找最小值
 *   4. 在排序数组中查找元素的第一个和最后一个位置 (LeetCode 34, Medium) - 左右边界
 *   5. 寻找两个正序数组的中位数 (LeetCode 4, Hard) - 二分划分
 *   6. 搜索插入位置 (LeetCode 35, Easy) - 标准下界查找
 *   7. x的平方根 (LeetCode 69, Easy) - 整数二分
 *   8. 猜数字大小 (LeetCode 374, Easy) - 二分与API反馈
 *   9. 寻找峰值 (LeetCode 162, Medium) - 二分比较邻居
 *   10. H指数 (LeetCode 274, Medium) - 二分查找h值
 *
 * 核心知识点:
 *   - 二分查找的本质: 每次排除一半的搜索空间
 *   - 两种经典模板: 左闭右闭 [left, right] vs 左闭右开 [left, right)
 *   - 关键: 循环不变量 - 始终保证目标值在搜索区间内
 *   - 时间复杂度: O(log n)
 *
 * ASCII图示 - 二分查找原理:
 *
 *   搜索区间 [left, right]:
 *   +---+---+---+---+---+---+---+---+
 *   | 1 | 3 | 5 | 7 | 9 | 11| 13| 15|
 *   +---+---+---+---+---+---+---+---+
 *     0   1   2   3   4   5   6   7
 *     ^                   ^
 *   left               right
 *             mid = 3 (值为7)
 *
 *   若 target=5 < 7: right = mid - 1
 *   若 target=9 > 7: left = mid + 1
 *   若 target=7: 找到!
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// ============================================================================
// 题目1: 二分查找 (LeetCode 704)
// ============================================================================
/**
 * 给定一个 n 个元素有序的（升序）整型数组 nums 和一个目标值 target。
 * 写一个函数搜索 nums 中的 target，如果目标值存在返回下标，否则返回 -1。
 *
 * 示例:
 *   输入: nums = [-1,0,3,5,9,12], target = 9
 *   输出: 4
 *
 * 两种模板对比:
 *
 * 模板1: 左闭右闭 [left, right]
 *   - 初始: left=0, right=n-1
 *   - 循环条件: left <= right (因为right是有效下标)
 *   - mid偏大: right = mid - 1
 *   - mid偏小: left = mid + 1
 *
 *   ASCII图示:
 *   [left .... mid .... right]
 *    有效区间包含两端
 *
 * 模板2: 左闭右开 [left, right)
 *   - 初始: left=0, right=n
 *   - 循环条件: left < right (因为right不可达)
 *   - mid偏大: right = mid (right本身不可达)
 *   - mid偏小: left = mid + 1
 *
 *   ASCII图示:
 *   [left .... mid .... right)
 *    有效区间包含left，不包含right
 */

// 方法一: 左闭右闭区间 [left, right]
// 最常用的模板，推荐掌握
class Search_Standard {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;  // right指向最后一个有效元素

        // 循环条件: left <= right (区间非空时继续)
        while (left <= right) {
            int mid = left + (right - left) / 2;  // 防止溢出

            /*
             * 详细推演: nums = [-1,0,3,5,9,12], target = 9
             *
             * Iteration 1:
             *   left=0, right=5, mid=2
             *   nums[2]=3 < 9 -> 搜索右半部分
             *   left = mid+1 = 3
             *   区间变为 [3, 5] -> [5, 9, 12]
             *
             * Iteration 2:
             *   left=3, right=5, mid=4
             *   nums[4]=9 == 9 -> 找到! 返回4
             */

            if (nums[mid] == target) {
                return mid;           // 找到目标
            } else if (nums[mid] < target) {
                left = mid + 1;       // 目标在右半部分，排除mid及左半部分
            } else {
                right = mid - 1;      // 目标在左半部分，排除mid及右半部分
            }
        }

        return -1;  // 搜索区间为空，未找到
    }
};

// 方法二: 左闭右开区间 [left, right)
class Search_HalfOpen {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size();  // right指向下一个位置(不可达)

        // 循环条件: left < right (区间非空时继续，因为right不可达)
        while (left < right) {
            int mid = left + (right - left) / 2;

            /*
             * 区别在于right的更新:
             * 左闭右闭: right = mid - 1  (mid已排除)
             * 左闭右开: right = mid      (mid仍可达，但right本身不可达)
             */

            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;          // 注意: 这里不是mid-1
            }
        }

        return -1;
    }
};

// ============================================================================
// 题目2: 搜索旋转排序数组 (LeetCode 33)
// ============================================================================
/**
 * 整数数组 nums 按升序排列，数组中的值互不相同。
 * 在传递给函数之前，nums 在预先未知的某个下标上进行了旋转。
 * 例如，[0,1,2,4,5,6,7] 可能变为 [4,5,6,7,0,1,2]。
 * 给你旋转后的数组 nums 和一个整数 target，如果 nums 中存在这个目标值，
 * 则返回它的下标，否则返回 -1。
 *
 * 要求: O(log n) 时间复杂度
 *
 * ASCII图示 - 旋转数组的特点:
 *
 *   原始数组: [0, 1, 2, 4, 5, 6, 7]
 *   旋转后:   [4, 5, 6, 7, 0, 1, 2]
 *             ^^^^^^^^  ^^^^^^^^
 *             左半有序    右半有序
 *
 *   关键观察: 二分后，至少有一半是有序的!
 *
 *   情况1: [left ... mid] 有序 (nums[left] <= nums[mid])
 *   +-------+-------+
 *   | 有序   | 无序   |    若 target 在 [nums[left], nums[mid]) 之间
 *   | 4 5 6 | 7 0 1 |    -> 搜索左半部分
 *   +-------+-------+    否则 -> 搜索右半部分
 *
 *   情况2: [mid ... right] 有序 (nums[mid] <= nums[right])
 *   +-------+-------+
 *   | 无序   | 有序   |    若 target 在 (nums[mid], nums[right]] 之间
 *   | 7 0 1 | 2 4 5 |    -> 搜索右半部分
 *   +-------+-------+    否则 -> 搜索左半部分
 */

class SearchRotated {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                return mid;
            }

            // 判断哪一半是有序的
            if (nums[left] <= nums[mid]) {
                // 左半部分有序 [left, mid]
                // 注意: 等号处理只有一个元素的情况 (left == mid)

                if (nums[left] <= target && target < nums[mid]) {
                    // target在左半有序区间内
                    right = mid - 1;
                } else {
                    // target在右半部分
                    left = mid + 1;
                }
            } else {
                // 右半部分有序 [mid, right]

                if (nums[mid] < target && target <= nums[right]) {
                    // target在右半有序区间内
                    left = mid + 1;
                } else {
                    // target在左半部分
                    right = mid - 1;
                }
            }
        }

        return -1;
    }
};

// ============================================================================
// 题目3: 寻找旋转排序数组中的最小值 (LeetCode 153)
// ============================================================================
/**
 * 已知一个长度为 n 的数组，预先按照升序排列，经由 1 到 n 次旋转后，
 * 得到输入数组。给你一个元素值互不相同的数组 nums，返回其中的最小元素。
 *
 * 示例:
 *   输入: [3,4,5,1,2]  输出: 1
 *   输入: [4,5,6,7,0,1,2]  输出: 0
 *
 * ASCII图示 - 与rightmost比较:
 *
 *   [4, 5, 6, 7, 0, 1, 2]
 *                  ^
 *               最小值在"断崖"处
 *
 *   二分策略: 与最右元素比较
 *
 *   若 nums[mid] > nums[right]:
 *     最小值在 mid 右侧 (不含mid)
 *     [4, 5, 6, 7, 0, 1, 2]
 *                  mid=7 > right=2
 *                  -> 最小值在右半部分
 *
 *   若 nums[mid] <= nums[right]:
 *     最小值在 mid 左侧 (含mid，因为mid可能就是最小值)
 *     [0, 1, 2, 4, 5, 6, 7]  (未旋转的情况)
 *                  mid=4 <= right=7
 *                  -> 最小值在左半部分(含mid)
 */

class FindMin {
public:
    int findMin(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            /*
             * 详细推演: nums = [4, 5, 6, 7, 0, 1, 2]
             *
             * Iteration 1:
             *   left=0, right=6, mid=3
             *   nums[3]=7 > nums[6]=2
             *   -> 最小值在右半部分, left = mid+1 = 4
             *   搜索区间: [0, 1, 2]
             *
             * Iteration 2:
             *   left=4, right=6, mid=5
             *   nums[5]=1 <= nums[6]=2
             *   -> 最小值在左半部分(含mid), right = mid = 5
             *   搜索区间: [0, 1]
             *
             * Iteration 3:
             *   left=4, right=5, mid=4
             *   nums[4]=0 <= nums[6]=2
             *   -> right = mid = 4
             *   搜索区间: [0]
             *
             * left == right, 循环结束
             * 返回 nums[4] = 0
             */

            if (nums[mid] > nums[right]) {
                // mid在旋转点的左侧，最小值在mid右边
                left = mid + 1;
            } else {
                // mid在旋转点的右侧(或未旋转)，最小值在mid左边(含mid)
                right = mid;
            }
        }

        // left == right，即为最小值
        return nums[left];
    }
};

// ============================================================================
// 题目4: 在排序数组中查找元素的第一个和最后一个位置 (LeetCode 34)
// ============================================================================
/**
 * 给定一个按照升序排列的整数数组 nums，和一个目标值 target。
 * 找出给定目标值在数组中的开始位置和结束位置。
 * 如果数组中不存在目标值 target，返回 [-1, -1]。
 *
 * 要求: O(log n) 时间复杂度
 *
 * 示例:
 *   输入: nums = [5,7,7,8,8,10], target = 8
 *   输出: [3, 4]
 *
 * ASCII图示 - 左边界和右边界:
 *
 *   nums: [5, 7, 7, 8, 8, 8, 10]
 *                    ^     ^
 *                 左边界  右边界
 *                 (第一个8) (最后一个8)
 *
 *   查找左边界 (第一个 >= target 的位置):
 *   +---+---+---+---+---+---+---+
 *   | 5 | 7 | 7 | 8 | 8 | 8 |10 |
 *   +---+---+---+---+---+---+---+
 *               leftBound = 3
 *   所有 < 8 的元素在左边，所有 >= 8 的元素在右边
 *
 *   查找右边界 (最后一个 <= target 的位置):
 *   +---+---+---+---+---+---+---+
 *   | 5 | 7 | 7 | 8 | 8 | 8 |10 |
 *   +---+---+---+---+---+---+---+
 *                     rightBound = 5
 *   所有 <= 8 的元素在左边，所有 > 8 的元素在右边
 */

class SearchRange {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = findLeftBound(nums, target);
        int right = findRightBound(nums, target);
        return {left, right};
    }

private:
    // 查找左边界: 第一个等于target的位置
    int findLeftBound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        int result = -1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                result = mid;      // 记录当前位置
                right = mid - 1;   // 继续向左搜索，看有没有更早的
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return result;
    }

    // 查找右边界: 最后一个等于target的位置
    int findRightBound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        int result = -1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                result = mid;      // 记录当前位置
                left = mid + 1;    // 继续向右搜索，看有没有更晚的
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        return result;
    }
};

// 左右边界的另一种写法 (更通用的模板)
class SearchRange_Alternative {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = lowerBound(nums, target);
        int right = upperBound(nums, target) - 1;

        // 检查是否找到
        if (left <= right && left >= 0 && right < nums.size()
            && nums[left] == target && nums[right] == target) {
            return {left, right};
        }
        return {-1, -1};
    }

private:
    // lowerBound: 第一个 >= target 的位置
    // 即: 所有 < target 的元素都在返回值左边
    int lowerBound(vector<int>& nums, int target) {
        int left = 0, right = nums.size();

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;  // mid可能就是答案，不能排除
            }
        }
        return left;
    }

    // upperBound: 第一个 > target 的位置
    // 即: 所有 <= target 的元素都在返回值左边
    int upperBound(vector<int>& nums, int target) {
        int left = 0, right = nums.size();

        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] <= target) {
                left = mid + 1;  // mid不可能是答案(需要 > target)
            } else {
                right = mid;
            }
        }
        return left;
    }
};

// ============================================================================
// 题目5: 寻找两个正序数组的中位数 (LeetCode 4)
// ============================================================================
/**
 * 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
 * 请你找出并返回这两个正序数组的中位数。
 * 要求时间复杂度为 O(log(m+n))，但最优解是 O(log(min(m,n)))。
 *
 * 示例:
 *   输入: nums1 = [1,3], nums2 = [2]
 *   输出: 2.0
 *
 *   输入: nums1 = [1,2], nums2 = [3,4]
 *   输出: 2.5 (合并后[1,2,3,4]，中位数=(2+3)/2)
 *
 * ASCII图示 - 划分法核心思想:
 *
 *   nums1: [1, 3, 5, 7, 9]     (m=5)
 *   nums2: [2, 4, 6, 8]        (n=4)
 *   总长度: 9, 左半部分应有 5 个元素
 *
 *   在nums1中切一刀 (i=2), 在nums2中切一刀 (j=3):
 *   nums1: [1, 3 | 5, 7, 9]     左: [1,3]   右: [5,7,9]
 *   nums2: [2, 4, 6 | 8]        左: [2,4,6] 右: [8]
 *
 *   合并左半: [1, 2, 3, 4, 6]   (5个元素)
 *   合并右半: [5, 7, 8, 9]      (4个元素)
 *
 *   中位数 = max(左半最大) = 6? 不对!
 *   需要满足: maxLeft1 <= minRight1 且 maxLeft2 <= minRight2
 *
 *   正确划分: i=3, j=2
 *   nums1: [1, 3, 5 | 7, 9]     左: [1,3,5] 右: [7,9]
 *   nums2: [2, 4 | 6, 8]        左: [2,4]   右: [6,8]
 *   合并左半: [1, 2, 3, 4, 5]   maxLeft = 5
 *   合并右半: [6, 7, 8, 9]      minRight = 6
 *   5 <= 6, 满足条件!
 *   中位数 = 5 (奇数情况)
 *
 * 核心公式:
 *   i + j = (m + n + 1) / 2  (左半部分的元素个数)
 *   j = (m + n + 1) / 2 - i
 *
 *   只需对较短的数组做二分，确定i后j自动确定
 */

class FindMedianSortedArrays {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // 确保 nums1 是较短的数组
        if (nums1.size() > nums2.size()) {
            return findMedianSortedArrays(nums2, nums1);
        }

        int m = nums1.size();
        int n = nums2.size();
        int left = 0, right = m;

        // 左半部分应有的元素个数
        int halfLen = (m + n + 1) / 2;

        while (left <= right) {
            // i: nums1中左半部分的元素个数
            int i = left + (right - left) / 2;
            // j: nums2中左半部分的元素个数 (由i确定)
            int j = halfLen - i;

            // 获取四个边界值 (处理边界情况)
            // nums1左半的最大值: nums1[i-1] (若i=0则为负无穷)
            int maxLeft1 = (i == 0) ? INT_MIN : nums1[i - 1];
            // nums1右半的最小值: nums1[i] (若i=m则为正无穷)
            int minRight1 = (i == m) ? INT_MAX : nums1[i];
            // nums2左半的最大值: nums2[j-1] (若j=0则为负无穷)
            int maxLeft2 = (j == 0) ? INT_MIN : nums2[j - 1];
            // nums2右半的最小值: nums2[j] (若j=n则为正无穷)
            int minRight2 = (j == n) ? INT_MAX : nums2[j];

            /*
             * 判断划分是否正确:
             * 需要满足: maxLeft1 <= minRight1 且 maxLeft2 <= minRight2
             *
             * 情况1: maxLeft1 > minRight2
             *   -> nums1左半太大，i需要左移 (取更少的nums1元素)
             *   -> right = i - 1
             *
             * 情况2: maxLeft2 > minRight1
             *   -> nums2左半太大，i需要右移 (取更多的nums1元素)
             *   -> left = i + 1
             *
             * 情况3: 两个条件都满足 -> 划分正确
             */

            if (maxLeft1 <= minRight2 && maxLeft2 <= minRight1) {
                // 划分正确，计算中位数
                if ((m + n) % 2 == 1) {
                    // 奇数个元素: 中位数 = 左半部分的最大值
                    return max(maxLeft1, maxLeft2);
                } else {
                    // 偶数个元素: 中位数 = (左半最大 + 右半最小) / 2
                    return (max(maxLeft1, maxLeft2) + min(minRight1, minRight2)) / 2.0;
                }
            } else if (maxLeft1 > minRight2) {
                right = i - 1;  // i太大，左移
            } else {
                left = i + 1;   // i太小，右移
            }
        }

        return 0.0;  // 不会到达这里(输入保证有解)
    }
};

// ============================================================================
// 题目6: 搜索插入位置 (LeetCode 35)
// ============================================================================
/**
 * 给定一个排序数组和一个目标值，在数组中找到目标值并返回其索引。
 * 如果目标值不存在于数组中，返回它将会被按顺序插入的位置。
 * 必须使用时间复杂度为 O(log n) 的算法。
 *
 * 示例:
 *   输入: nums = [1,3,5,6], target = 5     输出: 2
 *   输入: nums = [1,3,5,6], target = 2     输出: 1
 *   输入: nums = [1,3,5,6], target = 7     输出: 4
 *
 * ASCII图示 - 标准下界查找 (lower bound):
 *
 *   nums: [1, 3, 5, 6], target = 2
 *
 *   查找第一个 >= 2 的位置:
 *   +---+---+---+---+
 *   | 1 | 3 | 5 | 6 |
 *   +---+---+---+---+
 *     0   1   2   3
 *         ^
 *     1 < 2, 所以插入位置在索引1
 *
 *   核心: 这就是标准的 lower_bound 查找
 */

// 方法一: 左闭右闭模板
// 思路: 标准二分，找到第一个 >= target 的位置
// 时间复杂度: O(log n)  空间复杂度: O(1)
int searchInsert(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;           // 找到目标
        } else if (nums[mid] < target) {
            left = mid + 1;       // 目标在右半部分
        } else {
            right = mid - 1;      // 目标在左半部分
        }
    }

    // 循环结束时，left 就是第一个 >= target 的位置
    // 即插入位置
    return left;
}

// 方法二: 左闭右开模板 (更直观的 lower_bound)
// 时间复杂度: O(log n)  空间复杂度: O(1)
int searchInsert_v2(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size();  // 注意: 右开区间

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] < target) {
            left = mid + 1;       // mid太小，排除
        } else {
            right = mid;          // mid可能就是答案，保留
        }
    }

    return left;  // left == right，就是插入位置
}

void testSearchInsert() {
    cout << "========================================" << endl;
    cout << "题目6: 搜索插入位置 (LeetCode 35)" << endl;
    cout << "========================================" << endl;

    vector<pair<pair<vector<int>, int>, int>> testCases = {
        {{{1, 3, 5, 6}, 5}, 2},
        {{{1, 3, 5, 6}, 2}, 1},
        {{{1, 3, 5, 6}, 7}, 4},
        {{{1, 3, 5, 6}, 0}, 0},
        {{{1}, 1}, 0},
        {{{1, 3}, 2}, 1},
        {{{}, 5}, 0}
    };

    for (auto& [input, expected] : testCases) {
        auto& [nums, target] = input;
        int result1 = searchInsert(nums, target);
        int result2 = searchInsert_v2(nums, target);

        cout << "nums=[";
        for (int i = 0; i < (int)nums.size(); i++)
            cout << nums[i] << (i < (int)nums.size() - 1 ? "," : "");
        cout << "], target=" << target
             << " 期望:" << expected
             << " 方法1:" << result1
             << " 方法2:" << result2
             << (result1 == expected && result2 == expected ? " [通过]" : " [失败]")
             << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目7: x的平方根 (LeetCode 69)
// ============================================================================
/**
 * 给你一个非负整数 x，计算并返回 x 的算术平方根。
 * 由于返回类型是整数，结果只保留整数部分，小数部分将被舍去。
 * 注意：不允许使用任何内置指数函数和算符。
 *
 * 示例:
 *   输入: x = 4     输出: 2
 *   输入: x = 8     输出: 2 (解释: sqrt(8) = 2.828..., 舍去小数)
 *   输入: x = 0     输出: 0
 *
 * ASCII图示 - 二分查找:
 *
 *   x = 8, 查找最大的整数 mid 使得 mid*mid <= 8
 *
 *   搜索区间 [0, 8]:
 *   0  1  2  3  4  5  6  7  8
 *
 *   mid=4: 4*4=16 > 8  -> right=3
 *   mid=1: 1*1=1  <= 8 -> left=2
 *   mid=2: 2*2=4  <= 8 -> left=3
 *   mid=3: 3*3=9  > 8  -> right=2
 *
 *   left > right, 结束
 *   返回 right = 2 (最大的满足 mid*mid <= x 的整数)
 */

// 方法一: 二分查找（推荐）
// 思路: 在 [0, x] 中查找最大的整数 mid 使得 mid*mid <= x
// 时间复杂度: O(log x)  空间复杂度: O(1)
int mySqrt(int x) {
    if (x < 2) return x;  // 0和1的平方根是自己

    long left = 1;
    long right = x / 2;  // 平方根不会超过 x/2 (x>=4时)

    while (left <= right) {
        long mid = left + (right - left) / 2;
        long square = mid * mid;

        if (square == x) {
            return mid;           // 恰好是平方根
        } else if (square < x) {
            left = mid + 1;       // mid太小，向右搜索
        } else {
            right = mid - 1;      // mid太大，向左搜索
        }
    }

    // 循环结束时，right 是最大的满足 mid*mid <= x 的整数
    return right;
}

// 方法二: 牛顿迭代法
// 思路: 用牛顿法逼近平方根
// 公式: x_{n+1} = (x_n + a/x_n) / 2
// 时间复杂度: O(log x)  空间复杂度: O(1)
int mySqrt_newton(int x) {
    if (x < 2) return x;

    double guess = x;  // 初始猜测值

    // 牛顿迭代直到收敛
    while (guess * guess > x) {
        guess = (guess + x / guess) / 2.0;
    }

    return (int)guess;
}

void testMySqrt() {
    cout << "========================================" << endl;
    cout << "题目7: x的平方根 (LeetCode 69)" << endl;
    cout << "========================================" << endl;

    vector<pair<int, int>> testCases = {
        {0, 0}, {1, 1}, {2, 1}, {3, 1},
        {4, 2}, {8, 2}, {9, 3}, {16, 4},
        {27, 5}, {100, 10}, {2147395599, 46339}
    };

    for (auto& [x, expected] : testCases) {
        int result1 = mySqrt(x);
        int result2 = mySqrt_newton(x);
        cout << "sqrt(" << x << ") 期望:" << expected
             << " 二分:" << result1
             << " 牛顿:" << result2
             << (result1 == expected && result2 == expected ? " [通过]" : " [失败]")
             << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目8: 猜数字大小 (LeetCode 374)
// ============================================================================
/**
 * 猜数字游戏的规则如下：
 * - 我从 1 到 n 之间选择一个数字。
 * - 你来猜我选了哪个数字。
 * - 每次你猜错了，我会告诉你你猜的数字是大了还是小了。
 *
 * 给定一个预先选好的数字 pick，返回你猜的次数。
 *
 * 假设 API guess(int num) 已定义：
 *   - 返回 -1: 你猜的数字比我选的大 (num > pick)
 *   - 返回 1: 你猜的数字比我选的小 (num < pick)
 *   - 返回 0: 你猜对了 (num == pick)
 *
 * 示例:
 *   输入: n = 10, pick = 6     输出: 4 (猜: 5, 7, 6)
 *   输入: n = 1, pick = 1      输出: 1
 *
 * ASCII图示:
 *
 *   n=10, pick=6
 *
 *   1  2  3  4  5  6  7  8  9  10
 *                  mid=5
 *   guess(5) = 1 (太小) -> left = 6
 *
 *   6  7  8  9  10
 *   mid=8
 *   guess(8) = -1 (太大) -> right = 7
 *
 *   6  7
 *   mid=6
 *   guess(6) = 0 -> 找到!
 */

// 模拟 guess API (实际题目中已定义)
int pickNumber = 0;  // 全局变量，模拟pick值

int guess(int num) {
    if (num == pickNumber) return 0;
    return num > pickNumber ? -1 : 1;
}

// 方法: 标准二分查找
// 思路: 利用 guess API 的反馈缩小搜索范围
// 时间复杂度: O(log n)  空间复杂度: O(1)
int guessNumber(int n) {
    int left = 1;
    int right = n;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int result = guess(mid);

        if (result == 0) {
            return mid;           // 猜对了
        } else if (result < 0) {
            right = mid - 1;      // 猜大了，缩小右边界
        } else {
            left = mid + 1;       // 猜小了，扩大左边界
        }
    }

    return -1;  // 不会到达这里
}

void testGuessNumber() {
    cout << "========================================" << endl;
    cout << "题目8: 猜数字大小 (LeetCode 374)" << endl;
    cout << "========================================" << endl;

    vector<pair<int, int>> testCases = {
        {10, 6}, {1, 1}, {10, 1}, {10, 10},
        {100, 50}, {1000, 777}, {2126753390, 1702766719}
    };

    for (auto& [n, pick] : testCases) {
        pickNumber = pick;  // 设置模拟的pick值
        int result = guessNumber(n);
        cout << "n=" << n << ", pick=" << pick
             << " 结果:" << result
             << (result == pick ? " [通过]" : " [失败]") << endl;
    }
    cout << endl;
}

// ============================================================================
// 题目9: 寻找峰值 (LeetCode 162)
// ============================================================================
/**
 * 峰值元素是指其值严格大于左右相邻值的元素。
 * 给你一个整数数组 nums，找到峰值元素并返回其索引。
 * 假设 nums[-1] = nums[n] = -∞ (即边界元素也算峰值)。
 *
 * 你可以实现时间复杂度为 O(log n) 的解决方案。
 *
 * 示例:
 *   输入: nums = [1,2,3,1]     输出: 2 (3是峰值)
 *   输入: nums = [1,2,1,3,5,6,4] 输出: 5 (6是峰值) 或 1 (2是峰值)
 *
 * ASCII图示:
 *
 *   nums = [1, 2, 1, 3, 5, 6, 4]
 *
 *   数值:  6|        *
 *        5|       *
 *        4|              *
 *        3|      *
 *        2|   *
 *        1| *    *
 *          +--+--+--+--+--+--+--+
 *          0  1  2  3  4  5  6
 *
 *   峰值在索引5 (值为6)
 *
 *   二分策略:
 *   如果 nums[mid] < nums[mid+1]，说明右边是上升趋势 -> 峰值在右半部分
 *   否则，峰值在左半部分 (含mid)
 *
 *   关键: 只要数组中有峰值，二分一定能找到
 */

// 方法: 二分查找
// 思路: 比较 mid 和 mid+1，决定搜索方向
// 时间复杂度: O(log n)  空间复杂度: O(1)
int findPeakElement(vector<int>& nums) {
    int left = 0;
    int right = nums.size() - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] < nums[mid + 1]) {
            // 右边是上升趋势，峰值一定在 mid+1 及其右侧
            left = mid + 1;
        } else {
            // 左边是下降趋势或 mid 就是峰值
            // 峰值在 mid 及其左侧
            right = mid;
        }
    }

    // left == right，即为峰值索引
    return left;
}

void testFindPeakElement() {
    cout << "========================================" << endl;
    cout << "题目9: 寻找峰值 (LeetCode 162)" << endl;
    cout << "========================================" << endl;

    auto isPeak = [](vector<int>& nums, int idx) -> bool {
        int n = nums.size();
        long left = (idx == 0) ? LONG_MIN : nums[idx - 1];
        long right = (idx == n - 1) ? LONG_MIN : nums[idx + 1];
        return nums[idx] > left && nums[idx] > right;
    };

    // 测试用例 1
    vector<int> nums1 = {1, 2, 3, 1};
    int result1 = findPeakElement(nums1);
    cout << "nums=[1,2,3,1] 峰值索引:" << result1
         << " 值:" << nums1[result1]
         << (isPeak(nums1, result1) ? " [通过]" : " [失败]") << endl;

    // 测试用例 2
    vector<int> nums2 = {1, 2, 1, 3, 5, 6, 4};
    int result2 = findPeakElement(nums2);
    cout << "nums=[1,2,1,3,5,6,4] 峰值索引:" << result2
         << " 值:" << nums2[result2]
         << (isPeak(nums2, result2) ? " [通过]" : " [失败]") << endl;

    // 测试用例 3: 单个元素
    vector<int> nums3 = {1};
    int result3 = findPeakElement(nums3);
    cout << "nums=[1] 峰值索引:" << result3
         << " 值:" << nums3[result3]
         << (isPeak(nums3, result3) ? " [通过]" : " [失败]") << endl;

    // 测试用例 4: 两个元素
    vector<int> nums4 = {1, 2};
    int result4 = findPeakElement(nums4);
    cout << "nums=[1,2] 峰值索引:" << result4
         << " 值:" << nums4[result4]
         << (isPeak(nums4, result4) ? " [通过]" : " [失败]") << endl;

    // 测试用例 5: 严格递减
    vector<int> nums5 = {5, 4, 3, 2, 1};
    int result5 = findPeakElement(nums5);
    cout << "nums=[5,4,3,2,1] 峰值索引:" << result5
         << " 值:" << nums5[result5]
         << (isPeak(nums5, result5) ? " [通过]" : " [失败]") << endl;

    // 测试用例 6: 严格递增
    vector<int> nums6 = {1, 2, 3, 4, 5};
    int result6 = findPeakElement(nums6);
    cout << "nums=[1,2,3,4,5] 峰值索引:" << result6
         << " 值:" << nums6[result6]
         << (isPeak(nums6, result6) ? " [通过]" : " [失败]") << endl;
    cout << endl;
}

// ============================================================================
// 题目10: H指数 (LeetCode 274)
// ============================================================================
/**
 * 给你一个整数数组 citations，其中 citations[i] 表示研究者的第 i 篇论文被引用的次数。
 * 计算并返回该研究者的 h 指数。
 *
 * h 指数的定义：h 代表"高引用论文"篇数，
 * 一名科研人员的 h 指数是指他至多有 h 篇论文分别被引用了至少 h 次。
 *
 * 例如：h=3 表示有至少3篇论文被引用了至少3次。
 *
 * 示例:
 *   输入: citations = [3,0,6,1,5]  输出: 3
 *   解释: 有3篇论文被引用了至少3次 (引用: 3, 6, 5)
 *
 *   输入: citations = [1,3,1]      输出: 1
 *
 * ASCII图示 - 二分法:
 *
 *   citations = [3, 0, 6, 1, 5]
 *   排序后: [0, 1, 3, 5, 6]
 *
 *   h=3 时检查: 至少有3篇论文的引用 >= 3?
 *   从右往左数3个: 6, 5, 3 -> 都 >= 3 -> 满足!
 *
 *   二分查找h值 [0, n]:
 *   h候选值: 0, 1, 2, 3, 4, 5
 *
 *   对于候选h，检查是否至少有h篇论文的引用 >= h
 *   这可以用排序后检查 citations[n-h] >= h 来判断
 */

// 方法一: 二分查找 h 值（推荐）
// 思路: 对 h 值做二分，检查是否满足条件
// 时间复杂度: O(n log n) (排序)  空间复杂度: O(1)
int hIndex_binarySearch(vector<int>& citations) {
    sort(citations.begin(), citations.end());
    int n = citations.size();

    int left = 0;
    int right = n;  // h 的最大值不超过 n

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (mid == 0) {
            left = mid + 1;
            continue;
        }

        // 检查: 是否至少有 mid 篇论文被引用 >= mid 次
        // 排序后，检查倒数第 mid 篇论文的引用次数
        // citations[n-mid] 是从右往左数第 mid 篇
        if (citations[n - mid] >= mid) {
            // 满足条件，尝试更大的 h
            left = mid + 1;
        } else {
            // 不满足，尝试更小的 h
            right = mid - 1;
        }
    }

    return right;  // right 是满足条件的最大 h
}

// 方法二: 排序后线性扫描
// 思路: 排序后从大到小扫描，找到最大的 h 使得 citations[n-h] >= h
// 时间复杂度: O(n log n)  空间复杂度: O(1)
int hIndex_linear(vector<int>& citations) {
    sort(citations.begin(), citations.end());
    int n = citations.size();
    int h = 0;

    for (int i = n - 1; i >= 0; i--) {
        if (citations[i] >= n - i) {
            h = n - i;  // 当前有 n-i 篇论文被引用 >= citations[i] 次
        } else {
            break;
        }
    }

    return h;
}

// 方法三: 计数排序 (O(n) 时间)
// 思路: 用桶计数，h 最大不会超过 n
// 时间复杂度: O(n)  空间复杂度: O(n)
int hIndex_counting(vector<int>& citations) {
    int n = citations.size();
    vector<int> buckets(n + 1, 0);

    // 计数: 引用次数 >= n 的都放在 buckets[n] 中
    for (int c : citations) {
        buckets[min(c, n)]++;
    }

    // 从大到小累加，找到 h
    int count = 0;
    for (int h = n; h >= 0; h--) {
        count += buckets[h];
        if (count >= h) {
            return h;
        }
    }

    return 0;
}

void testHIndex() {
    cout << "========================================" << endl;
    cout << "题目10: H指数 (LeetCode 274)" << endl;
    cout << "========================================" << endl;

    vector<pair<vector<int>, int>> testCases = {
        {{3, 0, 6, 1, 5}, 3},
        {{1, 3, 1}, 1},
        {{100}, 1},
        {{0, 0, 0}, 0},
        {{1, 1, 1, 1, 1}, 1},
        {{5, 5, 5, 5, 5}, 5},
        {{0, 1, 3, 5, 6}, 3},
        {{4, 4, 4, 4}, 4},
        {{0}, 0},
        {{10, 8, 5, 4, 3}, 4}
    };

    for (auto& [citations, expected] : testCases) {
        // 每次都要重新排序，所以复制一份
        vector<int> c1 = citations;
        vector<int> c2 = citations;
        vector<int> c3 = citations;

        int result1 = hIndex_binarySearch(c1);
        int result2 = hIndex_linear(c2);
        int result3 = hIndex_counting(c3);

        cout << "citations=[";
        for (int i = 0; i < (int)citations.size(); i++)
            cout << citations[i] << (i < (int)citations.size() - 1 ? "," : "");
        cout << "] 期望:" << expected
             << " 二分:" << result1
             << " 线性:" << result2
             << " 计数:" << result3
             << (result1 == expected && result2 == expected && result3 == expected
                 ? " [通过]" : " [失败]")
             << endl;
    }
    cout << endl;
}

// ============================================================================
// 测试代码
// ============================================================================

void testBinarySearch() {
    cout << "========================================" << endl;
    cout << "题目1: 二分查找 (LeetCode 704)" << endl;
    cout << "========================================" << endl;

    Search_Standard sol1;
    Search_HalfOpen sol2;

    vector<int> nums = {-1, 0, 3, 5, 9, 12};

    cout << "数组: [-1, 0, 3, 5, 9, 12]" << endl;
    cout << "查找 9  - 左闭右闭: " << sol1.search(nums, 9) << endl;   // 4
    cout << "查找 9  - 左闭右开: " << sol2.search(nums, 9) << endl;   // 4
    cout << "查找 2  - 左闭右闭: " << sol1.search(nums, 2) << endl;   // -1
    cout << "查找 2  - 左闭右开: " << sol2.search(nums, 2) << endl;   // -1
    cout << "查找 -1 - 左闭右闭: " << sol1.search(nums, -1) << endl;  // 0
    cout << "查找 12 - 左闭右闭: " << sol1.search(nums, 12) << endl;  // 5
    cout << endl;
}

void testSearchRotated() {
    cout << "========================================" << endl;
    cout << "题目2: 搜索旋转排序数组 (LeetCode 33)" << endl;
    cout << "========================================" << endl;

    SearchRotated sol;

    vector<int> nums1 = {4, 5, 6, 7, 0, 1, 2};
    cout << "数组: [4,5,6,7,0,1,2]" << endl;
    cout << "查找 0: " << sol.search(nums1, 0) << endl;  // 4
    cout << "查找 3: " << sol.search(nums1, 3) << endl;  // -1
    cout << "查找 4: " << sol.search(nums1, 4) << endl;  // 0
    cout << "查找 7: " << sol.search(nums1, 7) << endl;  // 3

    vector<int> nums2 = {1};
    cout << "\n数组: [1]" << endl;
    cout << "查找 0: " << sol.search(nums2, 0) << endl;  // -1

    vector<int> nums3 = {3, 1};
    cout << "\n数组: [3,1]" << endl;
    cout << "查找 1: " << sol.search(nums3, 1) << endl;  // 1
    cout << endl;
}

void testFindMin() {
    cout << "========================================" << endl;
    cout << "题目3: 寻找旋转排序数组中的最小值 (LeetCode 153)" << endl;
    cout << "========================================" << endl;

    FindMin sol;

    vector<int> nums1 = {3, 4, 5, 1, 2};
    cout << "数组: [3,4,5,1,2] -> 最小值: " << sol.findMin(nums1) << endl;  // 1

    vector<int> nums2 = {4, 5, 6, 7, 0, 1, 2};
    cout << "数组: [4,5,6,7,0,1,2] -> 最小值: " << sol.findMin(nums2) << endl;  // 0

    vector<int> nums3 = {11, 13, 15, 17};
    cout << "数组: [11,13,15,17] (未旋转) -> 最小值: " << sol.findMin(nums3) << endl;  // 11

    vector<int> nums4 = {2, 1};
    cout << "数组: [2,1] -> 最小值: " << sol.findMin(nums4) << endl;  // 1
    cout << endl;
}

void testSearchRange() {
    cout << "========================================" << endl;
    cout << "题目4: 在排序数组中查找元素的第一个和最后一个位置 (LeetCode 34)" << endl;
    cout << "========================================" << endl;

    SearchRange sol1;
    SearchRange_Alternative sol2;

    vector<int> nums1 = {5, 7, 7, 8, 8, 10};
    cout << "数组: [5,7,7,8,8,10], target=8" << endl;
    auto res1 = sol1.searchRange(nums1, 8);
    cout << "方法1结果: [" << res1[0] << ", " << res1[1] << "]" << endl;  // [3, 4]

    auto res2 = sol2.searchRange(nums1, 8);
    cout << "方法2结果: [" << res2[0] << ", " << res2[1] << "]" << endl;  // [3, 4]

    cout << "\n数组: [5,7,7,8,8,10], target=6" << endl;
    auto res3 = sol1.searchRange(nums1, 6);
    cout << "结果: [" << res3[0] << ", " << res3[1] << "]" << endl;  // [-1, -1]

    vector<int> nums2 = {};
    cout << "\n数组: [], target=0" << endl;
    auto res4 = sol1.searchRange(nums2, 0);
    cout << "结果: [" << res4[0] << ", " << res4[1] << "]" << endl;  // [-1, -1]

    vector<int> nums3 = {1};
    cout << "\n数组: [1], target=1" << endl;
    auto res5 = sol1.searchRange(nums3, 1);
    cout << "结果: [" << res5[0] << ", " << res5[1] << "]" << endl;  // [0, 0]

    vector<int> nums4 = {2, 2};
    cout << "\n数组: [2,2], target=2" << endl;
    auto res6 = sol1.searchRange(nums4, 2);
    cout << "结果: [" << res6[0] << ", " << res6[1] << "]" << endl;  // [0, 1]
    cout << endl;
}

void testFindMedian() {
    cout << "========================================" << endl;
    cout << "题目5: 寻找两个正序数组的中位数 (LeetCode 4)" << endl;
    cout << "========================================" << endl;

    FindMedianSortedArrays sol;

    vector<int> a1 = {1, 3}, b1 = {2};
    cout << "nums1=[1,3], nums2=[2] -> " << sol.findMedianSortedArrays(a1, b1) << endl;  // 2.0

    vector<int> a2 = {1, 2}, b2 = {3, 4};
    cout << "nums1=[1,2], nums2=[3,4] -> " << sol.findMedianSortedArrays(a2, b2) << endl;  // 2.5

    vector<int> a3 = {}, b3 = {1};
    cout << "nums1=[], nums2=[1] -> " << sol.findMedianSortedArrays(a3, b3) << endl;  // 1.0

    vector<int> a4 = {2}, b4 = {};
    cout << "nums1=[2], nums2=[] -> " << sol.findMedianSortedArrays(a4, b4) << endl;  // 2.0

    vector<int> a5 = {1, 3, 5, 7, 9}, b5 = {2, 4, 6, 8};
    cout << "nums1=[1,3,5,7,9], nums2=[2,4,6,8] -> " << sol.findMedianSortedArrays(a5, b5) << endl;  // 5.0

    vector<int> a6 = {1, 2, 3}, b6 = {4, 5, 6, 7, 8};
    cout << "nums1=[1,2,3], nums2=[4,5,6,7,8] -> " << sol.findMedianSortedArrays(a6, b6) << endl;  // 4.5
    cout << endl;
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    cout << "****************************************" << endl;
    cout << "*   二分查找经典例题合集                *" << endl;
    cout << "****************************************" << endl;
    cout << endl;

    testBinarySearch();
    testSearchRotated();
    testFindMin();
    testSearchRange();
    testFindMedian();
    testSearchInsert();
    testMySqrt();
    testGuessNumber();
    testFindPeakElement();
    testHIndex();

    cout << "========================================" << endl;
    cout << "所有测试完成!" << endl;
    cout << "========================================" << endl;

    return 0;
}
