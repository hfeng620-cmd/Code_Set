/*
 * ========================================================================
 *   力扣经典例题 - 二叉树专题 (16道经典题目)
 * ========================================================================
 *
 *   本文件包含以下力扣二叉树经典题目:
 *   1.  二叉树的最大深度 (LeetCode 104, Easy)
 *   2.  翻转二叉树 (LeetCode 226, Easy)
 *   3.  二叉树的层序遍历 (LeetCode 102, Medium)
 *   4.  验证二叉搜索树 (LeetCode 98, Medium)
 *   5.  二叉树的最近公共祖先 (LeetCode 236, Medium)
 *   6.  二叉树的直径 (LeetCode 543, Easy)
 *   7.  将有序数组转换为二叉搜索树 (LeetCode 108, Easy)
 *   8.  二叉树的锯齿形层序遍历 (LeetCode 103, Medium)
 *   9.  二叉树的中序遍历 (LeetCode 94, Easy)
 *   10. 二叉树的前序遍历 (LeetCode 144, Easy)
 *   11. 对称二叉树 (LeetCode 101, Easy)
 *   12. 路径总和 (LeetCode 112, Easy)
 *   13. 二叉树展开为链表 (LeetCode 114, Medium)
 *   14. 从前序与中序遍历序列构造二叉树 (LeetCode 105, Medium)
 *   15. 二叉搜索树中第K小的元素 (LeetCode 230, Medium)
 *   16. 二叉树的最大路径和 (LeetCode 124, Hard)
 *
 *   每道题目均包含:
 *     - 中文题目描述
 *     - ASCII 图示
 *     - 多种解法及复杂度分析
 *     - 详细中文注释
 *     - main() 中的测试用例
 *
 *   编译命令: g++ -std=c++17 -o 04_二叉树 04_二叉树.cpp
 * ========================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>

using namespace std;

// ========================================================================
//  二叉树节点定义 (所有题目共用)
// ========================================================================
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// 工具函数: 释放二叉树内存
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// 工具函数: 打印二维树结构 (辅助调试)
void printTree(TreeNode* root, string prefix = "", bool isLeft = true) {
    if (!root) return;
    printTree(root->right, prefix + (isLeft ? "|   " : "    "), false);
    cout << prefix << (isLeft ? "\\-- " : "/-- ") << root->val << endl;
    printTree(root->left, prefix + (isLeft ? "    " : "|   "), true);
}

// ========================================================================
//  题目 1: 二叉树的最大深度 (LeetCode 104, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给定一个二叉树 root，返回其最大深度。
 *  二叉树的最大深度是从根节点到最远叶子节点的最长路径上的节点数。
 *
 * 【示意图】
 *       3
 *      / \
 *     9  20
 *       /  \
 *      15   7
 *
 *  输入: [3, 9, 20, null, null, 15, 7]
 *  输出: 3
 *
 * 【解法一】递归 DFS (深度优先搜索)
 *  思路: 最大深度 = max(左子树深度, 右子树深度) + 1
 *  时间复杂度: O(n)，每个节点访问一次
 *  空间复杂度: O(h)，h 为树的高度，递归栈深度
 */
int maxDepth_DFS(TreeNode* root) {
    // 递归终止条件: 空节点深度为 0
    if (!root) return 0;

    // 递归计算左右子树的最大深度
    int leftDepth = maxDepth_DFS(root->left);
    int rightDepth = maxDepth_DFS(root->right);

    // 当前节点的深度 = 左右子树较大值 + 1 (当前节点自身)
    return max(leftDepth, rightDepth) + 1;
}

/*
 * 【解法二】迭代 BFS (广度优先搜索 / 层序遍历)
 *  思路: 逐层遍历，统计层数即为最大深度
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)，队列中最多存一层的节点数
 */
int maxDepth_BFS(TreeNode* root) {
    if (!root) return 0;

    queue<TreeNode*> q;
    q.push(root);
    int depth = 0;

    // 每处理完一层，深度加 1
    while (!q.empty()) {
        int levelSize = q.size();  // 当前层的节点数
        depth++;

        // 将当前层的所有节点出队，并将其子节点入队
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return depth;
}

// ========================================================================
//  题目 2: 翻转二叉树 (LeetCode 226, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你一棵二叉树的根节点 root，翻转这棵二叉树，并返回其根节点。
 *  翻转: 将每个节点的左右子树交换。
 *
 * 【示意图】
 *  翻转前:          翻转后:
 *       4                4
 *      / \              / \
 *     2   7            7   2
 *    / \ / \          / \ / \
 *   1  3 6  9        9  6 3  1
 *
 *  输入: [4, 2, 7, 1, 3, 6, 9]
 *  输出: [4, 7, 2, 9, 6, 3, 1]
 *
 * 【解法一】递归法
 *  思路: 对每个节点，交换其左右子树，然后递归处理子树
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)，递归栈深度
 */
TreeNode* invertTree_Recursive(TreeNode* root) {
    // 递归终止: 空节点无需翻转
    if (!root) return nullptr;

    // 【关键步骤】交换当前节点的左右子树
    TreeNode* temp = root->left;
    root->left = root->right;
    root->right = temp;

    // 递归翻转左右子树
    invertTree_Recursive(root->left);
    invertTree_Recursive(root->right);

    return root;
}

/*
 * 【解法二】迭代法 (使用队列进行 BFS)
 *  思路: 层序遍历每个节点，逐个交换左右子树
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
TreeNode* invertTree_Iterative(TreeNode* root) {
    if (!root) return nullptr;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* node = q.front();
        q.pop();

        // 交换当前节点的左右子树
        swap(node->left, node->right);

        // 将子节点入队，等待后续处理
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return root;
}

// ========================================================================
//  题目 3: 二叉树的层序遍历 (LeetCode 102, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给你二叉树的根节点 root，返回其节点值的层序遍历
 *  (即逐层地，从左到右访问所有节点)。
 *
 * 【示意图】
 *       3
 *      / \
 *     9  20
 *       /  \
 *      15   7
 *
 *  输入: [3, 9, 20, null, null, 15, 7]
 *  输出: [[3], [9, 20], [15, 7]]
 *
 * 【解法】BFS + 记录每层大小
 *  思路: 使用队列进行 BFS，每次处理一整层节点
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();   // 当前层的节点数
        vector<int> currentLevel;   // 存储当前层的值

        // 【关键】循环 levelSize 次，精确处理一整层
        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            currentLevel.push_back(node->val);

            // 将下一层节点入队
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(currentLevel);
    }
    return result;
}

// ========================================================================
//  题目 4: 验证二叉搜索树 (LeetCode 98, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给你一个二叉树的根节点 root，判断其是否是一个有效的二叉搜索树。
 *  有效 BST 定义:
 *    - 节点的左子树只包含小于当前节点的数
 *    - 节点的右子树只包含大于当前节点的数
 *    - 所有左子树和右子树自身必须也是 BST
 *
 * 【示意图】
 *  有效 BST:        无效 BST:
 *       5                5
 *      / \              / \
 *     1   7            1   7
 *        / \              / \    ← 错误! 6 < 7 但 6 > 5,
 *       6   8                6   不满足右子树全部大于根节点
 *
 * 【常见错误】只检查直接子节点!
 *  错误做法: node->left->val < node->val && node->right->val > node->val
 *  这样无法检测到: 右子树中存在小于根节点的值 (如上图右侧示例)
 *
 * 【解法一】中序遍历法 (推荐)
 *  思路: BST 的中序遍历结果一定是严格递增的序列
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
bool isValidBST_Inorder(TreeNode* root) {
    TreeNode* prev = nullptr;  // 记录中序遍历的前一个节点

    // 使用 Morris 中序遍历的变体，这里用递归实现
    function<bool(TreeNode*)> inorder = [&](TreeNode* node) -> bool {
        if (!node) return true;

        // 递归检查左子树
        if (!inorder(node->left)) return false;

        // 【关键检查】当前节点值必须大于前一个节点值
        if (prev && node->val <= prev->val) return false;
        prev = node;

        // 递归检查右子树
        return inorder(node->right);
    };

    return inorder(root);
}

/*
 * 【解法二】递归 + 范围限制
 *  思路: 每个节点都有一个合法的值域范围 (min, max)
 *        根节点范围: (-∞, +∞)
 *        左子节点范围: (父节点下限, 父节点值)
 *        右子节点范围: (父节点值, 父节点上限)
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
bool isValidBST_Range(TreeNode* root, long minVal = LONG_MIN, long maxVal = LONG_MAX) {
    // 空节点视为有效
    if (!root) return true;

    // 当前节点值必须在合法范围内 (严格大于 min，严格小于 max)
    if (root->val <= minVal || root->val >= maxVal) return false;

    // 递归检查: 左子树的上界变为当前节点值，右子树的下界变为当前节点值
    return isValidBST_Range(root->left, minVal, root->val)
        && isValidBST_Range(root->right, root->val, maxVal);
}

// ========================================================================
//  题目 5: 二叉树的最近公共祖先 (LeetCode 236, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给定一个二叉树，找到该树中两个指定节点 p 和 q 的最近公共祖先 (LCA)。
 *  最近公共祖先: 对于树中两个节点，最近公共祖先是指
 *  在树中同时是 p 和 q 的祖先且深度最大的节点。
 *  (一个节点也可以是它自己的祖先)
 *
 * 【示意图】
 *           3
 *          / \
 *         5   1
 *        / \ / \
 *       6  2 0  8
 *         / \
 *        7   4
 *
 *  输入: root = [3, 5, 1, 6, 2, 0, 8, null, null, 7, 4], p = 5, q = 1
 *  输出: 3 (节点 3 是节点 5 和节点 1 的最近公共祖先)
 *
 *  输入: p = 5, q = 4
 *  输出: 5 (节点 5 是节点 5 和节点 4 的最近公共祖先)
 *
 * 【解法】递归法
 *  思路:
 *    1. 递归在左右子树中查找 p 和 q
 *    2. 如果当前节点就是 p 或 q，直接返回当前节点
 *    3. 如果 p 和 q 分别在当前节点的左右子树中，则当前节点就是 LCA
 *    4. 如果 p 和 q 都在同一侧子树中，则 LCA 在那一侧
 *
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // 【基础情况 1】到达空节点，返回 nullptr
    if (!root) return nullptr;

    // 【基础情况 2】当前节点就是 p 或 q，直接返回
    // (如果另一个节点在其子树中，则当前节点就是 LCA)
    if (root == p || root == q) return root;

    // 递归在左右子树中查找 p 和 q
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);

    // 【关键逻辑】
    // 情况 1: p 和 q 分别在左右子树中 → 当前节点是 LCA
    if (left && right) return root;

    // 情况 2: p 和 q 都在左子树中 → LCA 在左侧
    // 情况 3: p 和 q 都在右子树中 → LCA 在右侧
    // 情况 4: 都没找到 → 返回 nullptr
    return left ? left : right;
}

// ========================================================================
//  题目 6: 二叉树的直径 (LeetCode 543, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你一棵二叉树的根节点，返回该树的直径。
 *  二叉树的直径: 树中任意两个节点之间最长路径的长度 (边数)。
 *  这条路径可能穿过也可能不穿过根节点。
 *
 * 【示意图】
 *       1
 *      / \
 *     2   3       最长路径: [4, 2, 1, 3] 或 [5, 2, 1, 3]
 *    / \           直径 = 3 (经过的边数)
 *   4   5
 *
 *  输入: [1, 2, 3, 4, 5]
 *  输出: 3
 *
 * 【核心思想】
 *  对于每个节点，经过该节点的最长路径 = 左子树深度 + 右子树深度
 *  全局的直径 = 所有节点中，"左深度 + 右深度" 的最大值
 *
 * 【解法】DFS + 全局最大值
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
int diameterOfBinaryTree(TreeNode* root) {
    int maxDiameter = 0;  // 全局记录最大直径

    // 辅助函数: 计算以 node 为根的子树深度，同时更新直径
    function<int(TreeNode*)> depth = [&](TreeNode* node) -> int {
        if (!node) return 0;

        // 递归计算左右子树的深度
        int leftDepth = depth(node->left);
        int rightDepth = depth(node->right);

        // 【关键】经过当前节点的路径长度 = 左深度 + 右深度
        // 更新全局最大直径
        maxDiameter = max(maxDiameter, leftDepth + rightDepth);

        // 返回当前节点的深度 (供父节点使用)
        return max(leftDepth, rightDepth) + 1;
    };

    depth(root);
    return maxDiameter;
}

// ========================================================================
//  题目 7: 将有序数组转换为二叉搜索树 (LeetCode 108, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你一个整数数组 nums，其中元素已经按升序排列，
 *  请你将其转换为一棵高度平衡二叉搜索树。
 *  高度平衡二叉树: 一个二叉树每个节点的左右两个子树的高度差不超过 1。
 *
 * 【示意图】
 *  输入: nums = [-10, -3, 0, 5, 9]
 *
 *  可能的输出 (高度平衡 BST):
 *           0
 *          / \
 *        -3   9
 *        /   /
 *     -10   5
 *
 * 【为什么取中间元素?】
 *  - 有序数组的中间元素作为根，可以保证左右子树元素数量大致相等
 *  - 这样构建的树一定是高度平衡的
 *  - 左半部分自然都小于根，右半部分自然都大于根，满足 BST 性质
 *
 * 【解法】递归 + 取中间值
 *  时间复杂度: O(n)，每个元素恰好访问一次
 *  空间复杂度: O(log n)，递归栈深度 (平衡树)
 */
TreeNode* sortedArrayToBST(vector<int>& nums, int left, int right) {
    // 递归终止: 左边界超过右边界，无元素可构建
    if (left > right) return nullptr;

    // 【关键】选取中间元素作为当前子树的根
    int mid = left + (right - left) / 2;  // 防止整数溢出
    TreeNode* root = new TreeNode(nums[mid]);

    // 递归构建左子树 (使用左半部分数组)
    root->left = sortedArrayToBST(nums, left, mid - 1);

    // 递归构建右子树 (使用右半部分数组)
    root->right = sortedArrayToBST(nums, mid + 1, right);

    return root;
}

TreeNode* sortedArrayToBST(vector<int>& nums) {
    return sortedArrayToBST(nums, 0, nums.size() - 1);
}

// ========================================================================
//  题目 8: 二叉树的锯齿形层序遍历 (LeetCode 103, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给你二叉树的根节点 root，返回其节点值的锯齿形层序遍历。
 *  (即先从左往右，再从右往左，下一层再从左往右... 交替进行)
 *
 * 【示意图】
 *           3
 *          / \
 *         9  20
 *           /  \
 *          15   7
 *
 *  层序遍历:   [[3], [9, 20], [15, 7]]
 *  锯齿形遍历: [[3], [20, 9], [15, 7]]
 *                  ←      →       ←
 *  (第 0 层从左到右，第 1 层从右到左，第 2 层从左到右...)
 *
 * 【解法一】BFS + 方向标志
 *  思路: 在普通层序遍历基础上，用标志位控制插入方向
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);
    bool leftToRight = true;  // 标志位: true 表示从左到右

    while (!q.empty()) {
        int levelSize = q.size();
        deque<int> level;  // 使用双端队列方便两端插入

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();

            // 【关键】根据方向标志决定插入位置
            if (leftToRight) {
                level.push_back(node->val);   // 从左到右: 尾部插入
            } else {
                level.push_front(node->val);  // 从右到左: 头部插入
            }

            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        result.push_back(vector<int>(level.begin(), level.end()));
        leftToRight = !leftToRight;  // 每层切换方向
    }
    return result;
}

/*
 * 【解法二】BFS + 翻转法
 *  思路: 先做普通层序遍历，再对偶数层进行翻转
 *  实现更简单，但多一次翻转操作
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
vector<vector<int>> zigzagLevelOrder_Reverse(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<TreeNode*> q;
    q.push(root);
    int levelIndex = 0;

    // 第一步: 普通层序遍历
    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> currentLevel;

        for (int i = 0; i < levelSize; i++) {
            TreeNode* node = q.front();
            q.pop();
            currentLevel.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }

        // 第二步: 奇数层翻转
        if (levelIndex % 2 == 1) {
            reverse(currentLevel.begin(), currentLevel.end());
        }

        result.push_back(currentLevel);
        levelIndex++;
    }
    return result;
}

// ========================================================================
//  题目 9: 二叉树的中序遍历 (LeetCode 94, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给定一个二叉树的根节点 root，返回它的中序遍历。
 *  中序遍历顺序: 左子树 -> 根节点 -> 右子树
 *
 * 【示意图】
 *       1
 *        \
 *         2
 *        /
 *       3
 *
 *  输入: [1, null, 2, 3]
 *  输出: [1, 3, 2]
 *
 *  另一个示例:
 *         4
 *        / \
 *       2   5
 *      / \
 *     1   3
 *
 *  中序遍历: [1, 2, 3, 4, 5]  (对于 BST，中序遍历就是有序序列)
 *
 * 【解法一】递归法
 *  思路: 先递归遍历左子树，再访问根节点，最后递归遍历右子树
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)，递归栈深度
 */
vector<int> inorderTraversal_Recursive(TreeNode* root) {
    vector<int> result;

    // 辅助递归函数
    function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node) return;

        inorder(node->left);            // 先遍历左子树
        result.push_back(node->val);    // 访问根节点
        inorder(node->right);           // 再遍历右子树
    };

    inorder(root);
    return result;
}

/*
 * 【解法二】迭代法 (使用显式栈)
 *  思路: 用栈模拟递归过程
 *    1. 从根节点开始，不断将左子节点压栈，直到最左
 *    2. 弹出栈顶，访问该节点
 *    3. 转向右子树，重复步骤 1
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
vector<int> inorderTraversal_Iterative(TreeNode* root) {
    vector<int> result;
    stack<TreeNode*> stk;
    TreeNode* curr = root;

    // 【核心循环】curr 不为空或栈不为空时继续
    while (curr || !stk.empty()) {
        // 一路向左，将沿途节点压栈
        while (curr) {
            stk.push(curr);
            curr = curr->left;
        }

        // 到达最左，弹出栈顶并访问
        curr = stk.top();
        stk.pop();
        result.push_back(curr->val);

        // 转向右子树
        curr = curr->right;
    }

    return result;
}

/*
 * 【解法三】Morris 中序遍历 (O(1) 空间)
 *  思路: 利用叶子节点的空指针建立临时线索，避免使用栈
 *    1. 如果当前节点没有左子树，直接访问，然后转向右子树
 *    2. 如果有左子树，找到左子树的最右节点 (前驱):
 *       - 如果前驱的右指针为空: 将其指向当前节点 (建立线索)，向左走
 *       - 如果前驱的右指针指向当前节点: 说明左子树已遍历完，断开线索，
 *         访问当前节点，转向右子树
 *  时间复杂度: O(n) (每条边最多经过两次)
 *  空间复杂度: O(1) (不使用栈或递归)
 */
vector<int> inorderTraversal_Morris(TreeNode* root) {
    vector<int> result;
    TreeNode* curr = root;

    while (curr) {
        if (!curr->left) {
            // 【情况 1】没有左子树，直接访问当前节点
            result.push_back(curr->val);
            curr = curr->right;
        } else {
            // 【情况 2】有左子树，找前驱 (左子树的最右节点)
            TreeNode* predecessor = curr->left;
            while (predecessor->right && predecessor->right != curr) {
                predecessor = predecessor->right;
            }

            if (!predecessor->right) {
                // 前驱右指针为空: 建立线索，指向当前节点
                predecessor->right = curr;
                curr = curr->left;  // 向左走
            } else {
                // 前驱右指针指向当前节点: 说明左子树已遍历完
                predecessor->right = nullptr;  // 断开线索 (恢复树结构)
                result.push_back(curr->val);   // 访问当前节点
                curr = curr->right;            // 转向右子树
            }
        }
    }

    return result;
}

// ========================================================================
//  题目 10: 二叉树的前序遍历 (LeetCode 144, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你二叉树的根节点 root，返回它节点值的前序遍历。
 *  前序遍历顺序: 根节点 -> 左子树 -> 右子树
 *
 * 【示意图】
 *       1
 *        \
 *         2
 *        /
 *       3
 *
 *  输入: [1, null, 2, 3]
 *  输出: [1, 2, 3]
 *
 *  另一个示例:
 *         4
 *        / \
 *       2   5
 *      / \
 *     1   3
 *
 *  前序遍历: [4, 2, 1, 3, 5]
 *
 * 【解法一】递归法
 *  思路: 先访问根节点，再递归遍历左子树，最后递归遍历右子树
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
vector<int> preorderTraversal_Recursive(TreeNode* root) {
    vector<int> result;

    function<void(TreeNode*)> preorder = [&](TreeNode* node) {
        if (!node) return;

        result.push_back(node->val);    // 先访问根节点
        preorder(node->left);           // 再遍历左子树
        preorder(node->right);          // 最后遍历右子树
    };

    preorder(root);
    return result;
}

/*
 * 【解法二】迭代法 (使用栈)
 *  思路:
 *    1. 将根节点压栈
 *    2. 循环弹出栈顶，访问该节点
 *    3. 先将右子节点压栈，再将左子节点压栈 (栈是后进先出，
 *       所以先压右再压左，保证左先被访问)
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
vector<int> preorderTraversal_Iterative(TreeNode* root) {
    vector<int> result;
    if (!root) return result;

    stack<TreeNode*> stk;
    stk.push(root);

    while (!stk.empty()) {
        TreeNode* node = stk.top();
        stk.pop();
        result.push_back(node->val);

        // 【关键】先右后左压栈，保证左子节点先被访问
        if (node->right) stk.push(node->right);
        if (node->left) stk.push(node->left);
    }

    return result;
}

// ========================================================================
//  题目 11: 对称二叉树 (LeetCode 101, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你一个二叉树的根节点 root，检查它是否轴对称。
 *
 * 【示意图】
 *  对称二叉树:            非对称二叉树:
 *       1                      1
 *      / \                    / \
 *     2   2                  2   2
 *    / \ / \                    / \
 *   3  4 4  3                  3   3
 *
 *  输入: [1, 2, 2, 3, 4, 4, 3]
 *  输出: true
 *
 *  输入: [1, 2, 2, null, 3, null, 3]
 *  输出: false
 *
 * 【核心思想】
 *  一棵树是对称的，等价于其左子树和右子树互为"镜像"。
 *  两棵树互为镜像的条件:
 *    1. 两个根节点值相同
 *    2. 每棵树的左子树与另一棵树的右子树互为镜像
 *
 * 【解法一】递归法
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
bool isSymmetric_Recursive(TreeNode* root) {
    if (!root) return true;

    // 辅助函数: 判断两棵树是否互为镜像
    function<bool(TreeNode*, TreeNode*)> isMirror = [&](TreeNode* left, TreeNode* right) -> bool {
        // 两个都为空，对称
        if (!left && !right) return true;
        // 只有一个为空，不对称
        if (!left || !right) return false;
        // 值不同，不对称
        if (left->val != right->val) return false;

        // 【关键】左的左 vs 右的右，左的右 vs 右的左
        return isMirror(left->left, right->right)
            && isMirror(left->right, right->left);
    };

    return isMirror(root->left, root->right);
}

/*
 * 【解法二】迭代法 (使用队列)
 *  思路: 用队列成对地取出需要比较的节点
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
bool isSymmetric_Iterative(TreeNode* root) {
    if (!root) return true;

    queue<TreeNode*> q;
    q.push(root->left);
    q.push(root->right);

    while (!q.empty()) {
        TreeNode* left = q.front(); q.pop();
        TreeNode* right = q.front(); q.pop();

        // 两个都为空，继续检查下一对
        if (!left && !right) continue;
        // 只有一个为空，或值不同
        if (!left || !right || left->val != right->val) return false;

        // 【关键】按镜像顺序入队
        q.push(left->left);
        q.push(right->right);
        q.push(left->right);
        q.push(right->left);
    }

    return true;
}

// ========================================================================
//  题目 12: 路径总和 (LeetCode 112, Easy)
// ========================================================================
/*
 * 【题目描述】
 *  给你二叉树的根节点 root 和一个表示目标和的整数 targetSum。
 *  判断该树中是否存在根节点到叶子节点的路径，使得路径上所有节点值
 *  相加等于目标和 targetSum。
 *  叶子节点: 是指没有子节点的节点。
 *
 * 【示意图】
 *           5
 *          / \
 *         4   8
 *        /   / \
 *       11  13  4
 *      /  \      \
 *     7    2      1
 *
 *  输入: root = [5, 4, 8, 11, null, 13, 4, 7, 2, null, null, null, 1],
 *        targetSum = 22
 *  输出: true
 *  路径: 5 -> 4 -> 11 -> 2 = 22
 *
 * 【解法一】递归法 (减去当前值)
 *  思路: 到达叶子节点时，检查剩余值是否等于叶子节点值
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
bool hasPathSum_Recursive(TreeNode* root, int targetSum) {
    // 空节点，不存在路径
    if (!root) return false;

    // 到达叶子节点: 检查目标值是否等于当前节点值
    if (!root->left && !root->right) {
        return targetSum == root->val;
    }

    // 【关键】从目标值中减去当前节点值，递归检查左右子树
    return hasPathSum_Recursive(root->left, targetSum - root->val)
        || hasPathSum_Recursive(root->right, targetSum - root->val);
}

/*
 * 【解法二】DFS 迭代法 (使用栈)
 *  思路: 用栈同时存储节点和到达该节点时的剩余目标值
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
bool hasPathSum_DFS(TreeNode* root, int targetSum) {
    if (!root) return false;

    // 栈中存储 (节点, 剩余目标值) 对
    stack<pair<TreeNode*, int>> stk;
    stk.push({root, targetSum});

    while (!stk.empty()) {
        auto [node, remaining] = stk.top();
        stk.pop();

        // 到达叶子节点，检查剩余值
        if (!node->left && !node->right && remaining == node->val) {
            return true;
        }

        // 将子节点入栈，目标值减去当前节点值
        if (node->right) stk.push({node->right, remaining - node->val});
        if (node->left) stk.push({node->left, remaining - node->val});
    }

    return false;
}

// ========================================================================
//  题目 13: 二叉树展开为链表 (LeetCode 114, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给你二叉树的根结点 root，请你将它展开为一个单链表:
 *    - 展开后的单链表应该与二叉树的前序遍历顺序相同
 *    - 展开后的链表使用同样的 TreeNode 类型
 *    - right 子指针指向链表中下一个结点，左子指针始终为 null
 *    - 必须原地展开 (不能创建新的节点)
 *
 * 【示意图】
 *  展开前:                展开后 (前序: 1,2,3,4,5,6):
 *       1                   1
 *      / \                   \
 *     2   5                   2
 *    / \   \                   \
 *   3   4   6                   3
 *                                \
 *                                 4
 *                                  \
 *                                   5
 *                                    \
 *                                     6
 *
 * 【解法一】反向前序遍历 (右-左-根)
 *  思路:
 *    前序遍历是 根-左-右，如果我们反过来 (右-左-根) 遍历，
 *    就可以利用一个"前驱"指针，把每个节点的 right 指向前一个访问的节点。
 *    使用栈来实现反向前序遍历。
 *  时间复杂度: O(n)
 *  空间复杂度: O(n)
 */
void flatten_ReversePreorder(TreeNode* root) {
    if (!root) return;

    stack<TreeNode*> stk;
    stk.push(root);
    TreeNode* prev = nullptr;  // 前一个访问的节点

    while (!stk.empty()) {
        TreeNode* curr = stk.top();
        stk.pop();

        // 【关键】将当前节点的 right 指向前一个节点
        if (prev) {
            prev->right = curr;
            prev->left = nullptr;
        }

        // 先压左再压右 (栈后进先出，所以右先被弹出 → 实现右-左-根)
        if (curr->right) stk.push(curr->right);
        if (curr->left) stk.push(curr->left);

        prev = curr;
    }
}

/*
 * 【解法二】原地展开 (寻找前驱节点)
 *  思路:
 *    对于当前节点，如果它有左子树:
 *    1. 找到左子树的最右节点 (即左子树前序遍历的最后一个节点)
 *    2. 将该最右节点的 right 指向当前节点的 right 子树
 *    3. 将当前节点的 right 指向左子树，left 置空
 *  时间复杂度: O(n) (虽然有嵌套循环，但每个节点最多被访问两次)
 *  空间复杂度: O(1)
 */
void flatten_Inplace(TreeNode* root) {
    TreeNode* curr = root;

    while (curr) {
        if (curr->left) {
            // 【步骤 1】找左子树的最右节点 (前驱)
            TreeNode* predecessor = curr->left;
            while (predecessor->right) {
                predecessor = predecessor->right;
            }

            // 【步骤 2】将前驱的 right 指向当前节点的 right 子树
            predecessor->right = curr->right;

            // 【步骤 3】将当前节点的 right 指向左子树，left 置空
            curr->right = curr->left;
            curr->left = nullptr;
        }

        // 移动到下一个节点 (即原来的左子树，现在变成了右子树)
        curr = curr->right;
    }
}

// ========================================================================
//  题目 14: 从前序与中序遍历序列构造二叉树 (LeetCode 105, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给定两个整数数组 preorder 和 inorder，其中 preorder 是二叉树的前序遍历，
 *  inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。
 *
 * 【示意图】
 *  前序遍历: [3, 9, 20, 15, 7]
 *  中序遍历: [9, 3, 15, 20, 7]
 *
 *  构建的二叉树:
 *       3
 *      / \
 *     9  20
 *       /  \
 *      15   7
 *
 * 【核心思想 - 分治法】
 *  前序遍历的第一个元素一定是根节点。
 *  在中序遍历中找到根节点的位置，其左边是左子树，右边是右子树。
 *  根据左右子树的大小，可以在前序遍历中确定左右子树的范围。
 *  递归地对左右子树执行相同操作。
 *
 *  前序: [ 根 | ----左子树---- | ----右子树---- ]
 *  中序: [ ----左子树---- | 根 | ----右子树---- ]
 *
 * 【解法】递归 + 哈希表加速查找
 *  时间复杂度: O(n)，哈希表使得查找根节点位置为 O(1)
 *  空间复杂度: O(n)，哈希表 + 递归栈
 */
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    // 用哈希表存储中序遍历中每个值对应的索引，加速查找
    unordered_map<int, int> inorderIndex;
    for (int i = 0; i < (int)inorder.size(); i++) {
        inorderIndex[inorder[i]] = i;
    }

    // 递归构建函数
    // preStart, preEnd: 前序遍历的当前范围
    // inStart, inEnd: 中序遍历的当前范围
    function<TreeNode*(int, int, int, int)> build =
        [&](int preStart, int preEnd, int inStart, int inEnd) -> TreeNode* {
        if (preStart > preEnd || inStart > inEnd) return nullptr;

        // 【步骤 1】前序遍历的第一个元素就是根节点
        int rootVal = preorder[preStart];
        TreeNode* root = new TreeNode(rootVal);

        // 【步骤 2】在中序遍历中找到根节点的位置
        int rootIndex = inorderIndex[rootVal];

        // 【步骤 3】计算左子树的节点数
        int leftSize = rootIndex - inStart;

        // 【步骤 4】递归构建左右子树
        // 左子树: 前序 [preStart+1, preStart+leftSize], 中序 [inStart, rootIndex-1]
        root->left = build(preStart + 1, preStart + leftSize, inStart, rootIndex - 1);
        // 右子树: 前序 [preStart+leftSize+1, preEnd], 中序 [rootIndex+1, inEnd]
        root->right = build(preStart + leftSize + 1, preEnd, rootIndex + 1, inEnd);

        return root;
    };

    return build(0, preorder.size() - 1, 0, inorder.size() - 1);
}

// ========================================================================
//  题目 15: 二叉搜索树中第K小的元素 (LeetCode 230, Medium)
// ========================================================================
/*
 * 【题目描述】
 *  给定一个二叉搜索树的根节点 root 和一个整数 k，
 *  请你设计一个算法查找其中第 k 小的元素 (从 1 开始计数)。
 *
 * 【示意图】
 *         3
 *        / \
 *       1   4
 *        \
 *         2
 *
 *  输入: root = [3, 1, 4, null, 2], k = 1
 *  输出: 1
 *
 *  输入: root = [5, 3, 6, 2, 4, null, null, 1], k = 3
 *       5
 *      / \
 *     3   6
 *    / \
 *   2   4
 *  /
 * 1
 *  输出: 3
 *
 * 【核心思想】
 *  BST 的中序遍历就是有序序列，第 k 个被访问的节点就是第 k 小的元素。
 *
 * 【解法一】递归中序遍历
 *  思路: 中序遍历 BST，计数到第 k 个时记录结果
 *  时间复杂度: O(h + k)，h 为树高 (最好 O(log n + k)，最差 O(n))
 *  空间复杂度: O(h)
 */
int kthSmallest_Recursive(TreeNode* root, int k) {
    int result = -1;
    int count = 0;  // 已访问的节点计数

    function<void(TreeNode*)> inorder = [&](TreeNode* node) {
        if (!node || result != -1) return;  // 找到结果后提前终止

        inorder(node->left);

        count++;
        if (count == k) {
            result = node->val;  // 【关键】第 k 个节点即为答案
            return;
        }

        inorder(node->right);
    };

    inorder(root);
    return result;
}

/*
 * 【解法二】迭代中序遍历 (使用栈)
 *  思路: 用栈模拟中序遍历，弹出第 k 个节点即为答案
 *  优点: 找到后可立即返回，无需遍历整棵树
 *  时间复杂度: O(h + k)
 *  空间复杂度: O(h)
 */
int kthSmallest_Iterative(TreeNode* root, int k) {
    stack<TreeNode*> stk;
    TreeNode* curr = root;
    int count = 0;

    while (curr || !stk.empty()) {
        // 一路向左压栈
        while (curr) {
            stk.push(curr);
            curr = curr->left;
        }

        // 弹出栈顶 (当前最小的未访问节点)
        curr = stk.top();
        stk.pop();
        count++;

        // 【关键】第 k 个即为答案
        if (count == k) return curr->val;

        // 转向右子树
        curr = curr->right;
    }

    return -1;  // k 超出范围 (正常情况不会到达)
}

// ========================================================================
//  题目 16: 二叉树的最大路径和 (LeetCode 124, Hard)
// ========================================================================
/*
 * 【题目描述】
 *  路径被定义为一条从树中任意节点出发，沿父节点-子节点连接，
 *  达到任意节点的序列。同一个节点在一条路径序列中至多出现一次。
 *  该路径至少包含一个节点，且不一定经过根节点。
 *  给你一个二叉树的根节点 root，返回其最大路径和。
 *
 * 【示意图】
 *  示例 1:
 *       -10
 *       /  \
 *      9   20
 *         /  \
 *        15   7
 *
 *  最大路径和: 15 -> 20 -> 7 = 42
 *
 *  示例 2:
 *        1
 *       / \
 *      2   3
 *
 *  最大路径和: 2 -> 1 -> 3 = 6
 *
 *  示例 3 (含负数):
 *      -3
 *
 *  最大路径和: -3 (只有一个节点)
 *
 * 【核心思想】
 *  对于每个节点，计算经过该节点的最大路径和:
 *    经过该节点的最大路径和 = 节点值 + max(左子树贡献, 0) + max(右子树贡献, 0)
 *
 *  其中 "子树贡献" 是指从该子树中选取一条向上的单边路径能带来的最大增益。
 *  注意: 贡献值不能为负 (如果子树贡献为负，不如不选)。
 *
 *  但是向父节点返回时，只能选择一边 (左或右)，因为路径不能分叉。
 *
 * 【解法】DFS + 全局最大值
 *  时间复杂度: O(n)
 *  空间复杂度: O(h)
 */
int maxPathSum(TreeNode* root) {
    int globalMax = INT_MIN;  // 全局最大路径和

    // 辅助函数: 返回以 node 为起点的单边最大路径和 (向上贡献)
    function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
        if (!node) return 0;

        // 递归计算左右子树的单边最大贡献
        // 如果贡献为负，则取 0 (不选择该子树)
        int leftGain = max(dfs(node->left), 0);
        int rightGain = max(dfs(node->right), 0);

        // 【关键】经过当前节点的最大路径和 = 节点值 + 左贡献 + 右贡献
        // 这条路径在当前节点处 "拐弯"，不能继续向上传递
        int pathThroughNode = node->val + leftGain + rightGain;
        globalMax = max(globalMax, pathThroughNode);

        // 【返回值】向父节点返回单边最大路径 (只能选左或右，不能分叉)
        return node->val + max(leftGain, rightGain);
    };

    dfs(root);
    return globalMax;
}

// ========================================================================
//  辅助函数: 创建测试用的二叉树
// ========================================================================

// 创建题目 1 的测试树: [3, 9, 20, null, null, 15, 7]
TreeNode* createTree1() {
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    return root;
}

// 创建题目 2 的测试树: [4, 2, 7, 1, 3, 6, 9]
TreeNode* createTree2() {
    TreeNode* root = new TreeNode(4);
    root->left = new TreeNode(2);
    root->right = new TreeNode(7);
    root->left->left = new TreeNode(1);
    root->left->right = new TreeNode(3);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(9);
    return root;
}

// 创建题目 4 的有效 BST: [5, 1, 7, null, null, 6, 8]
TreeNode* createValidBST() {
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(1);
    root->right = new TreeNode(7);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(8);
    return root;
}

// 创建题目 4 的无效 BST: [5, 1, 7, null, null, 4, 8]
// (右子树中有 4 < 5，违反 BST 性质)
TreeNode* createInvalidBST() {
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(1);
    root->right = new TreeNode(7);
    root->right->left = new TreeNode(4);  // 错误! 4 < 5 但位于右子树
    root->right->right = new TreeNode(8);
    return root;
}

// 创建题目 5 的测试树: [3, 5, 1, 6, 2, 0, 8, null, null, 7, 4]
TreeNode* createTreeForLCA() {
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(5);
    root->right = new TreeNode(1);
    root->left->left = new TreeNode(6);
    root->left->right = new TreeNode(2);
    root->right->left = new TreeNode(0);
    root->right->right = new TreeNode(8);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(4);
    return root;
}

// 创建题目 6 的测试树: [1, 2, 3, 4, 5]
TreeNode* createTreeForDiameter() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    return root;
}

// 创建题目 9/10 的测试树: [1, null, 2, 3]
//       1
//        \
//         2
//        /
//       3
TreeNode* createTree9() {
    TreeNode* root = new TreeNode(1);
    root->right = new TreeNode(2);
    root->right->left = new TreeNode(3);
    return root;
}

// 创建题目 11 的对称测试树: [1, 2, 2, 3, 4, 4, 3]
//       1
//      / \
//     2   2
//    / \ / \
//   3  4 4  3
TreeNode* createSymmetricTree() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(3);
    return root;
}

// 创建题目 11 的非对称测试树: [1, 2, 2, null, 3, null, 3]
//       1
//      / \
//     2   2
//      \   \
//       3   3
TreeNode* createAsymmetricTree() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(2);
    root->left->right = new TreeNode(3);
    root->right->right = new TreeNode(3);
    return root;
}

// 创建题目 12 的测试树:
//           5
//          / \
//         4   8
//        /   / \
//       11  13  4
//      /  \      \
//     7    2      1
TreeNode* createTreeForPathSum() {
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(4);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(11);
    root->left->left->left = new TreeNode(7);
    root->left->left->right = new TreeNode(2);
    root->right->left = new TreeNode(13);
    root->right->right = new TreeNode(4);
    root->right->right->right = new TreeNode(1);
    return root;
}

// 创建题目 13 的测试树:
//       1
//      / \
//     2   5
//    / \   \
//   3   4   6
TreeNode* createTreeForFlatten() {
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(5);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(4);
    root->right->right = new TreeNode(6);
    return root;
}

// 创建题目 14 的测试树: 中序 [9,3,15,20,7], 前序 [3,9,20,15,7]
//       3
//      / \
//     9  20
//       /  \
//      15   7

// 创建题目 15 的 BST:
//         5
//        / \
//       3   6
//      / \
//     2   4
//    /
//   1
TreeNode* createBSTForKthSmallest() {
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(6);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);
    root->left->left->left = new TreeNode(1);
    return root;
}

// 创建题目 16 的测试树:
//       -10
//       /  \
//      9   20
//         /  \
//        15   7
TreeNode* createTreeForMaxPathSum() {
    TreeNode* root = new TreeNode(-10);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    return root;
}

// ========================================================================
//  主函数: 测试所有题目
// ========================================================================
int main() {
    cout << "==========================================" << endl;
    cout << "  力扣经典例题 - 二叉树专题测试" << endl;
    cout << "==========================================" << endl;

    // ----- 题目 1: 二叉树的最大深度 -----
    {
        cout << "\n【题目 1】二叉树的最大深度 (LeetCode 104)" << endl;
        cout << "  树结构:" << endl;
        TreeNode* root = createTree1();
        printTree(root);

        int ans1 = maxDepth_DFS(root);
        int ans2 = maxDepth_BFS(root);
        cout << "  解法一 (递归 DFS): " << ans1 << endl;
        cout << "  解法二 (迭代 BFS): " << ans2 << endl;
        cout << "  期望输出: 3" << endl;
        deleteTree(root);
    }

    // ----- 题目 2: 翻转二叉树 -----
    {
        cout << "\n【题目 2】翻转二叉树 (LeetCode 226)" << endl;
        TreeNode* root = createTree2();
        cout << "  翻转前:" << endl;
        printTree(root);

        // 测试递归解法
        invertTree_Recursive(root);
        cout << "  翻转后 (递归法):" << endl;
        printTree(root);

        // 再次翻转恢复，测试迭代解法
        invertTree_Iterative(root);
        cout << "  再次翻转 (迭代法，恢复原状):" << endl;
        printTree(root);
        deleteTree(root);
    }

    // ----- 题目 3: 二叉树的层序遍历 -----
    {
        cout << "\n【题目 3】二叉树的层序遍历 (LeetCode 102)" << endl;
        TreeNode* root = createTree1();
        vector<vector<int>> result = levelOrder(root);
        cout << "  输入树 (与题目 1 相同): [3, 9, 20, null, null, 15, 7]" << endl;
        cout << "  层序遍历结果: [";
        for (size_t i = 0; i < result.size(); i++) {
            cout << "[";
            for (size_t j = 0; j < result[i].size(); j++) {
                cout << result[i][j];
                if (j + 1 < result[i].size()) cout << ", ";
            }
            cout << "]";
            if (i + 1 < result.size()) cout << ", ";
        }
        cout << "]" << endl;
        cout << "  期望输出: [[3], [9, 20], [15, 7]]" << endl;
        deleteTree(root);
    }

    // ----- 题目 4: 验证二叉搜索树 -----
    {
        cout << "\n【题目 4】验证二叉搜索树 (LeetCode 98)" << endl;

        // 测试有效 BST
        TreeNode* validBST = createValidBST();
        cout << "  有效 BST [5, 1, 7, null, null, 6, 8]:" << endl;
        printTree(validBST);
        cout << "  解法一 (中序遍历): " << (isValidBST_Inorder(validBST) ? "true" : "false") << endl;
        cout << "  解法二 (范围限制): " << (isValidBST_Range(validBST) ? "true" : "false") << endl;
        cout << "  期望输出: true" << endl;
        deleteTree(validBST);

        // 测试无效 BST
        TreeNode* invalidBST = createInvalidBST();
        cout << "\n  无效 BST [5, 1, 7, null, null, 4, 8]:" << endl;
        printTree(invalidBST);
        cout << "  (注意: 右子树中有 4 < 5，违反 BST 性质)" << endl;
        cout << "  解法一 (中序遍历): " << (isValidBST_Inorder(invalidBST) ? "true" : "false") << endl;
        cout << "  解法二 (范围限制): " << (isValidBST_Range(invalidBST) ? "true" : "false") << endl;
        cout << "  期望输出: false" << endl;
        deleteTree(invalidBST);
    }

    // ----- 题目 5: 二叉树的最近公共祖先 -----
    {
        cout << "\n【题目 5】二叉树的最近公共祖先 (LeetCode 236)" << endl;
        TreeNode* root = createTreeForLCA();
        cout << "  树结构:" << endl;
        printTree(root);

        // 在树中找到 p=5 和 q=1 的节点
        TreeNode* p = root->left;     // 节点 5
        TreeNode* q = root->right;    // 节点 1

        TreeNode* lca1 = lowestCommonAncestor(root, p, q);
        cout << "  LCA(5, 1) = " << lca1->val << " (期望: 3)" << endl;

        // 测试 LCA(5, 4): 5 是 4 的祖先
        TreeNode* q2 = root->left->right->right;  // 节点 4
        TreeNode* lca2 = lowestCommonAncestor(root, p, q2);
        cout << "  LCA(5, 4) = " << lca2->val << " (期望: 5)" << endl;

        // 测试 LCA(6, 4)
        TreeNode* p3 = root->left->left;          // 节点 6
        TreeNode* lca3 = lowestCommonAncestor(root, p3, q2);
        cout << "  LCA(6, 4) = " << lca3->val << " (期望: 5)" << endl;

        deleteTree(root);
    }

    // ----- 题目 6: 二叉树的直径 -----
    {
        cout << "\n【题目 6】二叉树的直径 (LeetCode 543)" << endl;
        TreeNode* root = createTreeForDiameter();
        cout << "  树结构:" << endl;
        printTree(root);
        cout << "  最长路径: [4, 2, 1, 3] 或 [5, 2, 1, 3]" << endl;

        int diameter = diameterOfBinaryTree(root);
        cout << "  直径 = " << diameter << " (期望: 3)" << endl;

        // 测试单节点树
        TreeNode* single = new TreeNode(1);
        cout << "  单节点树直径 = " << diameterOfBinaryTree(single) << " (期望: 0)" << endl;

        deleteTree(root);
        deleteTree(single);
    }

    // ----- 题目 7: 将有序数组转换为二叉搜索树 -----
    {
        cout << "\n【题目 7】将有序数组转换为二叉搜索树 (LeetCode 108)" << endl;
        vector<int> nums = {-10, -3, 0, 5, 9};
        cout << "  输入数组: [-10, -3, 0, 5, 9]" << endl;

        TreeNode* bst = sortedArrayToBST(nums);
        cout << "  构建的平衡 BST:" << endl;
        printTree(bst);
        cout << "  (根节点应为中间元素 0)" << endl;

        // 验证中序遍历结果是否与原数组一致
        vector<int> inorder;
        function<void(TreeNode*)> getInorder = [&](TreeNode* node) {
            if (!node) return;
            getInorder(node->left);
            inorder.push_back(node->val);
            getInorder(node->right);
        };
        getInorder(bst);
        cout << "  中序遍历验证: [";
        for (size_t i = 0; i < inorder.size(); i++) {
            cout << inorder[i];
            if (i + 1 < inorder.size()) cout << ", ";
        }
        cout << "]" << endl;

        deleteTree(bst);
    }

    // ----- 题目 8: 二叉树的锯齿形层序遍历 -----
    {
        cout << "\n【题目 8】二叉树的锯齿形层序遍历 (LeetCode 103)" << endl;
        TreeNode* root = createTree1();  // [3, 9, 20, null, null, 15, 7]
        cout << "  树结构:" << endl;
        printTree(root);

        // 解法一: BFS + 方向标志
        vector<vector<int>> zigzag1 = zigzagLevelOrder(root);
        cout << "  解法一 (BFS + 方向标志): [";
        for (size_t i = 0; i < zigzag1.size(); i++) {
            cout << "[";
            for (size_t j = 0; j < zigzag1[i].size(); j++) {
                cout << zigzag1[i][j];
                if (j + 1 < zigzag1[i].size()) cout << ", ";
            }
            cout << "]";
            if (i + 1 < zigzag1.size()) cout << ", ";
        }
        cout << "]" << endl;

        // 解法二: BFS + 翻转法
        vector<vector<int>> zigzag2 = zigzagLevelOrder_Reverse(root);
        cout << "  解法二 (BFS + 翻转法):   [";
        for (size_t i = 0; i < zigzag2.size(); i++) {
            cout << "[";
            for (size_t j = 0; j < zigzag2[i].size(); j++) {
                cout << zigzag2[i][j];
                if (j + 1 < zigzag2[i].size()) cout << ", ";
            }
            cout << "]";
            if (i + 1 < zigzag2.size()) cout << ", ";
        }
        cout << "]" << endl;
        cout << "  期望输出: [[3], [20, 9], [15, 7]]" << endl;
        deleteTree(root);
    }

    // ----- 题目 9: 二叉树的中序遍历 -----
    {
        cout << "\n【题目 9】二叉树的中序遍历 (LeetCode 94)" << endl;
        TreeNode* root = createTree9();
        cout << "  树结构 (1, null, 2, 3):" << endl;
        printTree(root);

        vector<int> ans1 = inorderTraversal_Recursive(root);
        vector<int> ans2 = inorderTraversal_Iterative(root);
        vector<int> ans3 = inorderTraversal_Morris(root);

        auto printVec = [](const string& label, const vector<int>& v) {
            cout << "  " << label << ": [";
            for (size_t i = 0; i < v.size(); i++) {
                cout << v[i];
                if (i + 1 < v.size()) cout << ", ";
            }
            cout << "]" << endl;
        };

        printVec("解法一 (递归)      ", ans1);
        printVec("解法二 (迭代+栈)   ", ans2);
        printVec("解法三 (Morris O(1))", ans3);
        cout << "  期望输出: [1, 3, 2]" << endl;
        deleteTree(root);
    }

    // ----- 题目 10: 二叉树的前序遍历 -----
    {
        cout << "\n【题目 10】二叉树的前序遍历 (LeetCode 144)" << endl;
        TreeNode* root = createTree9();  // 同一棵树: 1, null, 2, 3
        cout << "  树结构 (1, null, 2, 3):" << endl;
        printTree(root);

        vector<int> ans1 = preorderTraversal_Recursive(root);
        vector<int> ans2 = preorderTraversal_Iterative(root);

        auto printVec = [](const string& label, const vector<int>& v) {
            cout << "  " << label << ": [";
            for (size_t i = 0; i < v.size(); i++) {
                cout << v[i];
                if (i + 1 < v.size()) cout << ", ";
            }
            cout << "]" << endl;
        };

        printVec("解法一 (递归)   ", ans1);
        printVec("解法二 (迭代+栈)", ans2);
        cout << "  期望输出: [1, 2, 3]" << endl;
        deleteTree(root);
    }

    // ----- 题目 11: 对称二叉树 -----
    {
        cout << "\n【题目 11】对称二叉树 (LeetCode 101)" << endl;

        // 测试对称树
        TreeNode* sym = createSymmetricTree();
        cout << "  对称树 [1, 2, 2, 3, 4, 4, 3]:" << endl;
        printTree(sym);
        cout << "  解法一 (递归): " << (isSymmetric_Recursive(sym) ? "true" : "false") << endl;
        cout << "  解法二 (迭代): " << (isSymmetric_Iterative(sym) ? "true" : "false") << endl;
        cout << "  期望输出: true" << endl;
        deleteTree(sym);

        // 测试非对称树
        TreeNode* asym = createAsymmetricTree();
        cout << "\n  非对称树 [1, 2, 2, null, 3, null, 3]:" << endl;
        printTree(asym);
        cout << "  解法一 (递归): " << (isSymmetric_Recursive(asym) ? "true" : "false") << endl;
        cout << "  解法二 (迭代): " << (isSymmetric_Iterative(asym) ? "true" : "false") << endl;
        cout << "  期望输出: false" << endl;
        deleteTree(asym);
    }

    // ----- 题目 12: 路径总和 -----
    {
        cout << "\n【题目 12】路径总和 (LeetCode 112)" << endl;
        TreeNode* root = createTreeForPathSum();
        cout << "  树结构:" << endl;
        printTree(root);

        int target = 22;
        bool ans1 = hasPathSum_Recursive(root, target);
        bool ans2 = hasPathSum_DFS(root, target);
        cout << "  targetSum = " << target << endl;
        cout << "  解法一 (递归): " << (ans1 ? "true" : "false") << endl;
        cout << "  解法二 (DFS):  " << (ans2 ? "true" : "false") << endl;
        cout << "  期望输出: true (路径: 5->4->11->2 = 22)" << endl;

        // 测试不存在的路径
        int target2 = 26;
        cout << "\n  targetSum = " << target2 << endl;
        cout << "  解法一 (递归): " << (hasPathSum_Recursive(root, target2) ? "true" : "false") << endl;
        cout << "  解法二 (DFS):  " << (hasPathSum_DFS(root, target2) ? "true" : "false") << endl;
        cout << "  期望输出: false" << endl;
        deleteTree(root);
    }

    // ----- 题目 13: 二叉树展开为链表 -----
    {
        cout << "\n【题目 13】二叉树展开为链表 (LeetCode 114)" << endl;

        // 测试解法一: 反向前序遍历
        TreeNode* root1 = createTreeForFlatten();
        cout << "  展开前:" << endl;
        printTree(root1);
        flatten_ReversePreorder(root1);
        cout << "  解法一 (反向前序) 展开后 (沿 right 遍历): ";
        TreeNode* curr = root1;
        while (curr) {
            cout << curr->val;
            if (curr->right) cout << " -> ";
            curr = curr->right;
        }
        cout << endl;
        cout << "  期望: 1 -> 2 -> 3 -> 4 -> 5 -> 6" << endl;
        deleteTree(root1);

        // 测试解法二: 原地展开
        TreeNode* root2 = createTreeForFlatten();
        flatten_Inplace(root2);
        cout << "  解法二 (原地展开) 展开后: ";
        curr = root2;
        while (curr) {
            cout << curr->val;
            if (curr->right) cout << " -> ";
            curr = curr->right;
        }
        cout << endl;
        cout << "  期望: 1 -> 2 -> 3 -> 4 -> 5 -> 6" << endl;
        deleteTree(root2);
    }

    // ----- 题目 14: 从前序与中序遍历序列构造二叉树 -----
    {
        cout << "\n【题目 14】从前序与中序遍历序列构造二叉树 (LeetCode 105)" << endl;
        vector<int> preorder = {3, 9, 20, 15, 7};
        vector<int> inorder = {9, 3, 15, 20, 7};

        cout << "  前序遍历: [3, 9, 20, 15, 7]" << endl;
        cout << "  中序遍历: [9, 3, 15, 20, 7]" << endl;

        TreeNode* root = buildTree(preorder, inorder);
        cout << "  构建的二叉树:" << endl;
        printTree(root);

        // 验证: 对构建的树做前序和中序遍历
        vector<int> resultPre = preorderTraversal_Recursive(root);
        vector<int> resultIn = inorderTraversal_Recursive(root);

        auto printVec = [](const string& label, const vector<int>& v) {
            cout << "  " << label << ": [";
            for (size_t i = 0; i < v.size(); i++) {
                cout << v[i];
                if (i + 1 < v.size()) cout << ", ";
            }
            cout << "]" << endl;
        };

        printVec("验证前序遍历", resultPre);
        printVec("验证中序遍历", resultIn);
        cout << "  期望: 前序 [3, 9, 20, 15, 7], 中序 [9, 3, 15, 20, 7]" << endl;
        deleteTree(root);
    }

    // ----- 题目 15: 二叉搜索树中第K小的元素 -----
    {
        cout << "\n【题目 15】二叉搜索树中第K小的元素 (LeetCode 230)" << endl;
        TreeNode* root = createBSTForKthSmallest();
        cout << "  BST 结构:" << endl;
        printTree(root);
        cout << "  中序遍历 (有序): 1, 2, 3, 4, 5, 6" << endl;

        for (int k = 1; k <= 6; k++) {
            int ans1 = kthSmallest_Recursive(root, k);
            int ans2 = kthSmallest_Iterative(root, k);
            cout << "  k=" << k << ": 递归=" << ans1 << ", 迭代=" << ans2 << endl;
        }
        cout << "  期望: k=1 -> 1, k=2 -> 2, k=3 -> 3, k=4 -> 4, k=5 -> 5, k=6 -> 6" << endl;
        deleteTree(root);
    }

    // ----- 题目 16: 二叉树的最大路径和 -----
    {
        cout << "\n【题目 16】二叉树的最大路径和 (LeetCode 124)" << endl;

        // 测试 1: [-10, 9, 20, null, null, 15, 7]
        TreeNode* root1 = createTreeForMaxPathSum();
        cout << "  测试 1 树结构:" << endl;
        printTree(root1);
        int ans1 = maxPathSum(root1);
        cout << "  最大路径和 = " << ans1 << " (期望: 42, 路径: 15->20->7)" << endl;
        deleteTree(root1);

        // 测试 2: [1, 2, 3]
        TreeNode* root2 = new TreeNode(1);
        root2->left = new TreeNode(2);
        root2->right = new TreeNode(3);
        cout << "\n  测试 2 树结构 [1, 2, 3]:" << endl;
        printTree(root2);
        int ans2 = maxPathSum(root2);
        cout << "  最大路径和 = " << ans2 << " (期望: 6, 路径: 2->1->3)" << endl;
        deleteTree(root2);

        // 测试 3: [-3] (单个负数节点)
        TreeNode* root3 = new TreeNode(-3);
        cout << "\n  测试 3 树结构 [-3]:" << endl;
        printTree(root3);
        int ans3 = maxPathSum(root3);
        cout << "  最大路径和 = " << ans3 << " (期望: -3)" << endl;
        deleteTree(root3);

        // 测试 4: [2, -1]
        TreeNode* root4 = new TreeNode(2);
        root4->left = new TreeNode(-1);
        cout << "\n  测试 4 树结构 [2, -1]:" << endl;
        printTree(root4);
        int ans4 = maxPathSum(root4);
        cout << "  最大路径和 = " << ans4 << " (期望: 2, 只选根节点)" << endl;
        deleteTree(root4);
    }

    cout << "\n==========================================" << endl;
    cout << "  所有测试用例执行完毕!" << endl;
    cout << "==========================================" << endl;

    return 0;
}
