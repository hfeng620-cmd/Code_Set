/*
 * =====================================================================================
 *  排序算法大全 (Complete Sorting Algorithm Reference)
 * =====================================================================================
 *
 *  本文件实现了所有经典排序算法，包含：
 *  1. 冒泡排序 (Bubble Sort)         - O(n^2), 稳定
 *  2. 选择排序 (Selection Sort)       - O(n^2), 不稳定
 *  3. 插入排序 (Insertion Sort)       - O(n^2), 稳定
 *  4. 希尔排序 (Shell Sort)           - O(n^1.25), 不稳定
 *  5. 归并排序 (Merge Sort)           - O(n log n), 稳定
 *  6. 快速排序 (Quick Sort)           - O(n log n) 平均, 不稳定
 *  7. 堆排序 (Heap Sort)             - O(n log n), 不稳定
 *  8. 计数排序 (Counting Sort)       - O(n+k), 稳定
 *  9. 基数排序 (Radix Sort)           - O(d*(n+k)), 稳定
 *
 *  编译: g++ -std=c++17 -O2 -o sorting 13_排序算法大全.cpp
 *  运行: ./sorting
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <string>
#include <cmath>
#include <climits>
#include <cstring>

using namespace std;

// ============================================================================
//  辅助工具函数
// ============================================================================

// 打印数组
void printArray(const vector<int>& arr, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        cout << arr[i];
        if (i < (int)arr.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// 打印带高亮的数组（标记正在比较/交换的元素）
void printArrayHighlight(const vector<int>& arr, int idx1, int idx2, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        if (i == idx1 || i == idx2)
            cout << "{" << arr[i] << "}";
        else
            cout << " " << arr[i] << " ";
        if (i < (int)arr.size() - 1) cout << ",";
    }
    cout << "]" << endl;
}

// 验证数组是否已排序
bool isSorted(const vector<int>& arr) {
    for (int i = 1; i < (int)arr.size(); i++) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

// 分隔线
void separator(const string& title) {
    cout << "\n" << string(70, '=') << endl;
    cout << "  " << title << endl;
    cout << string(70, '=') << endl;
}


// ============================================================================
//  1. 冒泡排序 (Bubble Sort)
// ============================================================================
/*
 * 基本思想：
 *   反复遍历数组，比较相邻元素，如果顺序错误就交换它们。
 *   每一轮遍历会把当前未排序部分的最大值"冒泡"到末尾。
 *
 * 过程图示 (对 [5, 3, 8, 1, 2] 排序)：
 *
 *   原始数组: [5, 3, 8, 1, 2]
 *
 *   第1轮: 比较相邻元素，把最大值冒泡到最后
 *     [5, 3, 8, 1, 2]  比较 5>3, 交换 -> [3, 5, 8, 1, 2]
 *     [3, 5, 8, 1, 2]  比较 5<8, 不换 -> [3, 5, 8, 1, 2]
 *     [3, 5, 8, 1, 2]  比较 8>1, 交换 -> [3, 5, 1, 8, 2]
 *     [3, 5, 1, 8, 2]  比较 8>2, 交换 -> [3, 5, 1, 2, 8]  <-- 8就位
 *
 *   第2轮:
 *     [3, 5, 1, 2, 8]  比较 3<5, 不换 -> [3, 5, 1, 2, 8]
 *     [3, 5, 1, 2, 8]  比较 5>1, 交换 -> [3, 1, 5, 2, 8]
 *     [3, 1, 5, 2, 8]  比较 5>2, 交换 -> [3, 1, 2, 5, 8]  <-- 5就位
 *
 *   第3轮:
 *     [3, 1, 2, 5, 8]  比较 3>1, 交换 -> [1, 3, 2, 5, 8]
 *     [1, 3, 2, 5, 8]  比较 3>2, 交换 -> [1, 2, 3, 5, 8]  <-- 3就位
 *
 *   第4轮: 无交换，排序完成 -> [1, 2, 3, 5, 8]
 *
 * 时间复杂度: 最好 O(n), 最坏 O(n^2), 平均 O(n^2)
 * 空间复杂度: O(1)
 * 稳定性: 稳定 (相等元素不交换，相对位置不变)
 */

// 基本冒泡排序
void bubbleSortBasic(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 优化冒泡排序：如果某一轮没有发生交换，说明已经有序，提前终止
void bubbleSortOptimized(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;  // 标记本轮是否发生交换
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // 如果本轮没有交换，数组已经有序
        if (!swapped) {
            cout << "    [优化] 第" << i + 1 << "轮无交换，提前终止" << endl;
            break;
        }
    }
}

void testBubbleSort() {
    separator("1. 冒泡排序 (Bubble Sort)");
    cout << endl;

    // 基本版
    cout << "--- 基本冒泡排序 ---" << endl;
    vector<int> arr1 = {5, 3, 8, 1, 2};
    cout << "排序过程:" << endl;
    // 逐步展示过程
    {
        vector<int> a = arr1;
        int n = a.size();
        for (int i = 0; i < n - 1; i++) {
            cout << "  第" << i + 1 << "轮:" << endl;
            for (int j = 0; j < n - 1 - i; j++) {
                if (a[j] > a[j + 1]) {
                    swap(a[j], a[j + 1]);
                    printArrayHighlight(a, j, j + 1, "    交换");
                }
            }
        }
    }

    vector<int> arr2 = {5, 3, 8, 1, 2};
    bubbleSortBasic(arr2);
    printArray(arr2, "基本版结果");
    cout << "已排序: " << (isSorted(arr2) ? "是" : "否") << endl;

    // 优化版
    cout << "\n--- 优化冒泡排序 ---" << endl;
    vector<int> arr3 = {1, 2, 3, 5, 8};  // 已排序的数组
    printArray(arr3, "输入(已排序)");
    bubbleSortOptimized(arr3);
    printArray(arr3, "优化版结果");

    vector<int> arr4 = {5, 3, 8, 1, 2};
    bubbleSortOptimized(arr4);
    printArray(arr4, "优化版结果");

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: 最好 O(n), 最坏 O(n^2), 平均 O(n^2)" << endl;
    cout << "  空间复杂度: O(1)" << endl;
    cout << "  稳定性: 稳定" << endl;
    cout << "  适用场景: 小规模数据, 或基本有序的数据(优化版)" << endl;
}


// ============================================================================
//  2. 选择排序 (Selection Sort)
// ============================================================================
/*
 * 基本思想：
 *   每一轮从未排序部分找到最小元素，放到已排序部分的末尾。
 *
 * 过程图示 (对 [5, 3, 8, 1, 2] 排序)：
 *
 *   原始: [5, 3, 8, 1, 2]
 *          ^^^^^^^^^^^^^^  未排序
 *
 *   第1轮: 找最小值 1，与位置0交换
 *     [5, 3, 8, 1, 2] -> 最小值1在位置3
 *     交换 5 和 1: [1, 3, 8, 5, 2]
 *     ^             已排序
 *
 *   第2轮: 在 [3,8,5,2] 中找最小值 2，与位置1交换
 *     [1, 3, 8, 5, 2] -> 最小值2在位置4
 *     交换 3 和 2: [1, 2, 8, 5, 3]
 *     ^^^           已排序
 *
 *   第3轮: 在 [8,5,3] 中找最小值 3，与位置2交换
 *     [1, 2, 8, 5, 3] -> 最小值3在位置4
 *     交换 8 和 3: [1, 2, 3, 5, 8]
 *     ^^^^^         已排序
 *
 *   第4轮: 在 [5,8] 中找最小值 5，已在正确位置
 *     [1, 2, 3, 5, 8]  不变
 *     ^^^^^^^^         已排序 -> 完成!
 *
 * 时间复杂度: O(n^2) (最好/最坏/平均都是)
 * 空间复杂度: O(1)
 * 稳定性: 不稳定 (例如 [5, 8, 5, 2] 第一轮交换 5 和 2 后，两个5的相对顺序改变)
 */
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;  // 假设当前位置就是最小值
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;  // 更新最小值位置
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
}

void testSelectionSort() {
    separator("2. 选择排序 (Selection Sort)");
    cout << endl;

    cout << "排序过程:" << endl;
    vector<int> a = {5, 3, 8, 1, 2};
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIdx]) minIdx = j;
        }
        cout << "  第" << i + 1 << "轮: 找到最小值 " << a[minIdx]
             << " 在位置" << minIdx << endl;
        if (minIdx != i) {
            swap(a[i], a[minIdx]);
            printArray(a, "    交换后");
        } else {
            printArray(a, "    无需交换");
        }
    }

    vector<int> arr = {5, 3, 8, 1, 2};
    selectionSort(arr);
    printArray(arr, "结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    // 展示不稳定性
    cout << "\n不稳定性示例:" << endl;
    cout << "  [5a, 8, 5b, 2] 第一轮找到最小值2与5a交换" << endl;
    cout << "  -> [2, 8, 5b, 5a]  5a和5b的相对顺序改变" << endl;

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(n^2) (最好/最坏/平均)" << endl;
    cout << "  空间复杂度: O(1)" << endl;
    cout << "  稳定性: 不稳定" << endl;
    cout << "  适用场景: 小规模数据, 交换次数少(最多n-1次交换)" << endl;
}


// ============================================================================
//  3. 插入排序 (Insertion Sort)
// ============================================================================
/*
 * 基本思想：
 *   类似于整理扑克牌，每次取一张牌，插入到已排序部分的正确位置。
 *
 * 过程图示 (对 [5, 3, 8, 1, 2] 排序)：
 *
 *   原始: [5 | 3, 8, 1, 2]
 *          ^  已排序  未排序
 *
 *   取 3: [3, 5 | 8, 1, 2]    3插入到5前面
 *          ^^^
 *
 *   取 8: [3, 5, 8 | 1, 2]    8已在正确位置
 *          ^^^^^
 *
 *   取 1: [1, 3, 5, 8 | 2]    1插入到最前面
 *          ^^^^^^^
 *
 *   取 2: [1, 2, 3, 5, 8]     2插入到1和3之间
 *          ^^^^^^^^^^
 *
 * 时间复杂度: 最好 O(n) (已有序), 最坏 O(n^2), 平均 O(n^2)
 * 空间复杂度: O(1)
 * 稳定性: 稳定
 */
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];     // 待插入的元素
        int j = i - 1;
        // 将比key大的元素向右移动
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;    // 插入到正确位置
    }
}

void testInsertionSort() {
    separator("3. 插入排序 (Insertion Sort)");
    cout << endl;

    cout << "排序过程:" << endl;
    vector<int> a = {5, 3, 8, 1, 2};
    int n = a.size();

    // 手动展示每步
    printArray(a, "初始");
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
        cout << "  插入 " << key << " 到位置 " << (j + 1) << ": ";
        printArray(a, "");
    }

    vector<int> arr = {5, 3, 8, 1, 2};
    insertionSort(arr);
    printArray(arr, "结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    // 测试已排序数组（最佳情况）
    cout << "\n最佳情况(已排序数组):" << endl;
    vector<int> arr2 = {1, 2, 3, 4, 5};
    insertionSort(arr2);
    printArray(arr2, "结果");

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: 最好 O(n), 最坏 O(n^2), 平均 O(n^2)" << endl;
    cout << "  空间复杂度: O(1)" << endl;
    cout << "  稳定性: 稳定" << endl;
    cout << "  适用场景: 小规模数据, 几乎有序的数据, 作为高级排序的子过程" << endl;
}


// ============================================================================
//  4. 希尔排序 (Shell Sort)
// ============================================================================
/*
 * 基本思想：
 *   希尔排序是插入排序的改进版。通过定义一个间隔序列(gap sequence)，
 *   先对间隔较大的元素进行插入排序，逐步缩小间隔，最终间隔为1时
 *   就是普通的插入排序。由于前面的排序让数组基本有序，
 *   最后一次插入排序效率很高。
 *
 * 间隔序列: 通常使用 n/2, n/4, ..., 1 (Shell原始序列)
 * 更好的序列: Knuth序列 1, 4, 13, 40, ... (h = 3*h + 1)
 *
 * 过程图示 (对 [8, 5, 9, 3, 2, 7, 1, 6] 排序, gap序列 [4, 2, 1]):
 *
 *   原始: [8, 5, 9, 3, 2, 7, 1, 6]
 *
 *   gap=4: 比较间隔为4的元素组
 *     组1: 8, 2  -> 排序后 2, 8
 *     组2: 5, 7  -> 排序后 5, 7
 *     组3: 9, 1  -> 排序后 1, 9
 *     组4: 3, 6  -> 排序后 3, 6
 *     结果: [2, 5, 1, 3, 8, 7, 9, 6]
 *
 *   gap=2: 比较间隔为2的元素组
 *     组1: 2, 1, 8, 9 -> 排序后 1, 2, 8, 9
 *     组2: 5, 3, 7, 6 -> 排序后 3, 5, 6, 7
 *     结果: [1, 3, 2, 5, 8, 6, 9, 7]
 *
 *   gap=1: 普通插入排序（此时数组已基本有序，效率高）
 *     结果: [1, 2, 3, 5, 6, 7, 8, 9]
 *
 * 时间复杂度: O(n^1.25) (使用Knuth序列), 最坏 O(n^2)
 * 空间复杂度: O(1)
 * 稳定性: 不稳定 (跨间隔交换会改变相等元素的相对位置)
 */
void shellSort(vector<int>& arr) {
    int n = arr.size();
    // 使用Knuth序列计算初始gap: 1, 4, 13, 40, 121, ...
    int gap = 1;
    while (gap < n / 3) {
        gap = 3 * gap + 1;  // 生成最大gap
    }

    while (gap >= 1) {
        // 对每个间隔组进行插入排序
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 3;  // 缩小间隔
    }
}

// 使用简单gap序列的版本（便于理解）
void shellSortSimple(vector<int>& arr) {
    int n = arr.size();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
    }
}

void testShellSort() {
    separator("4. 希尔排序 (Shell Sort)");
    cout << endl;

    cout << "排序过程 (使用 n/2 gap序列):" << endl;
    vector<int> a = {8, 5, 9, 3, 2, 7, 1, 6};
    int n = a.size();
    printArray(a, "初始");

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = a[i];
            int j = i;
            while (j >= gap && a[j - gap] > temp) {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = temp;
        }
        printArray(a, "gap=" + to_string(gap));
    }

    vector<int> arr = {8, 5, 9, 3, 2, 7, 1, 6};
    shellSort(arr);
    printArray(arr, "Knuth序列结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(n^1.25) (Knuth序列), O(n^2) 最坏" << endl;
    cout << "  空间复杂度: O(1)" << endl;
    cout << "  稳定性: 不稳定" << endl;
    cout << "  适用场景: 中等规模数据, 嵌入式系统(代码简单, 原地排序)" << endl;
}


// ============================================================================
//  5. 归并排序 (Merge Sort)
// ============================================================================
/*
 * 基本思想：
 *   分治法(Divide and Conquer)。将数组递归地分成两半，
 *   分别排序后，再合并两个有序数组。
 *
 * 过程图示 (对 [5, 3, 8, 1, 2, 7] 排序):
 *
 *   分割阶段:
 *                    [5, 3, 8, 1, 2, 7]
 *                   /                   \
 *            [5, 3, 8]              [1, 2, 7]
 *            /       \              /       \
 *         [5, 3]    [8]         [1, 2]    [7]
 *         /    \               /    \
 *       [5]   [3]           [1]   [2]
 *
 *   合并阶段:
 *       [5]   [3]           [1]   [2]
 *         \    /               \    /
 *         [3, 5]    [8]       [1, 2]    [7]
 *            \       /            \       /
 *            [3, 5, 8]          [1, 2, 7]
 *                   \                   /
 *                   [1, 2, 3, 5, 7, 8]
 *
 * 合并两个有序数组的过程:
 *   合并 [3, 5, 8] 和 [1, 2, 7]:
 *     左: [3, 5, 8]   右: [1, 2, 7]   结果: []
 *     比较 3 和 1: 1小 -> 结果: [1]
 *     比较 3 和 2: 2小 -> 结果: [1, 2]
 *     比较 3 和 7: 3小 -> 结果: [1, 2, 3]
 *     比较 5 和 7: 5小 -> 结果: [1, 2, 3, 5]
 *     比较 8 和 7: 7小 -> 结果: [1, 2, 3, 5, 7]
 *     左边剩余 8   -> 结果: [1, 2, 3, 5, 7, 8]
 *
 * 时间复杂度: O(n log n) (最好/最坏/平均)
 * 空间复杂度: O(n) (需要额外空间存放合并结果)
 * 稳定性: 稳定
 */
void merge(vector<int>& arr, int left, int mid, int right) {
    // 创建临时数组
    vector<int> temp(right - left + 1);
    int i = left;      // 左半部分指针
    int j = mid + 1;   // 右半部分指针
    int k = 0;         // 临时数组指针

    // 比较两个子数组，按顺序合并
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    // 复制左半部分剩余元素
    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    // 复制右半部分剩余元素
    while (j <= right) {
        temp[k++] = arr[j++];
    }

    // 将临时数组复制回原数组
    for (int t = 0; t < k; t++) {
        arr[left + t] = temp[t];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;  // 防止溢出
    mergeSort(arr, left, mid);       // 排序左半部分
    mergeSort(arr, mid + 1, right);  // 排序右半部分
    merge(arr, left, mid, right);    // 合并
}

void mergeSort(vector<int>& arr) {
    if (!arr.empty()) {
        mergeSort(arr, 0, arr.size() - 1);
    }
}

// 打印归并排序的分割过程
void mergeSortDraw(vector<int>& arr, int left, int right, int depth) {
    if (left >= right) return;

    string indent(depth * 4, ' ');
    int mid = left + (right - left) / 2;

    // 打印当前分割
    cout << indent << "分割: [";
    for (int i = left; i <= right; i++) {
        cout << arr[i];
        if (i < right) cout << ", ";
    }
    cout << "] -> 左[";
    for (int i = left; i <= mid; i++) {
        cout << arr[i];
        if (i < mid) cout << ", ";
    }
    cout << "] 右[";
    for (int i = mid + 1; i <= right; i++) {
        cout << arr[i];
        if (i < right) cout << ", ";
    }
    cout << "]" << endl;

    mergeSortDraw(arr, left, mid, depth + 1);
    mergeSortDraw(arr, mid + 1, right, depth + 1);
    merge(arr, left, mid, right);

    cout << indent << "合并: [";
    for (int i = left; i <= right; i++) {
        cout << arr[i];
        if (i < right) cout << ", ";
    }
    cout << "]" << endl;
}

void testMergeSort() {
    separator("5. 归并排序 (Merge Sort)");
    cout << endl;

    cout << "分治过程:" << endl;
    vector<int> a = {5, 3, 8, 1, 2, 7};
    printArray(a, "原始数组");
    cout << endl;
    mergeSortDraw(a, 0, a.size() - 1, 0);

    cout << "\n最终结果:" << endl;
    vector<int> arr = {5, 3, 8, 1, 2, 7};
    mergeSort(arr);
    printArray(arr, "归并排序");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    // 测试各种情况
    vector<int> arr2 = {1};
    mergeSort(arr2);
    printArray(arr2, "单元素");

    vector<int> arr3 = {};
    mergeSort(arr3);
    printArray(arr3, "空数组");

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(n log n) (最好/最坏/平均)" << endl;
    cout << "  空间复杂度: O(n)" << endl;
    cout << "  稳定性: 稳定" << endl;
    cout << "  适用场景: 大规模数据, 需要稳定排序, 链表排序, 外部排序" << endl;
}


// ============================================================================
//  6. 快速排序 (Quick Sort)
// ============================================================================
/*
 * 基本思想：
 *   选择一个"基准(pivot)"元素，将数组分为两部分：
 *   左边所有元素 <= pivot，右边所有元素 > pivot。
 *   然后递归地对左右两部分排序。
 *
 * 过程图示 - Lomuto分区 (对 [5, 3, 8, 1, 2, 7] 排序):
 *
 *   选择最后一个元素 7 为 pivot:
 *     [5, 3, 8, 1, 2, 7]  pivot=7
 *      ^           ^
 *     i=0        j遍历
 *
 *   遍历数组，将 <= pivot 的元素交换到左边:
 *     j=0: 5<=7, 交换 arr[0]和arr[0] -> [5, 3, 8, 1, 2, 7], i=1
 *     j=1: 3<=7, 交换 arr[1]和arr[1] -> [5, 3, 8, 1, 2, 7], i=2
 *     j=2: 8>7,  跳过
 *     j=3: 1<=7, 交换 arr[2]和arr[3] -> [5, 3, 1, 8, 2, 7], i=3
 *     j=4: 2<=7, 交换 arr[3]和arr[4] -> [5, 3, 1, 2, 8, 7], i=4
 *
 *   最后将 pivot 交换到正确位置:
 *     交换 arr[4]和arr[5] -> [5, 3, 1, 2, 7, 8]
 *                              ^^^^^^^^^^   ^
 *                              <=7 部分    >7 部分
 *
 *   递归排序左右两部分...
 *
 * 时间复杂度: 平均 O(n log n), 最坏 O(n^2) (已排序数组+选首元素)
 * 空间复杂度: O(log n) (递归栈)
 * 稳定性: 不稳定
 */

// Lomuto分区方案（选择最后一个元素为pivot）
int lomutoPartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // 选择最后一个元素作为基准
    int i = low - 1;        // i 指向小于等于pivot区域的末尾

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);  // 将pivot放到正确位置
    return i + 1;                  // 返回pivot的最终位置
}

// Hoare分区方案（选择第一个元素为pivot，效率更高）
int hoarePartition(vector<int>& arr, int low, int high) {
    int pivot = arr[low];  // 选择第一个元素作为基准
    int i = low - 1;
    int j = high + 1;

    while (true) {
        // 从左找第一个 > pivot 的元素
        do { i++; } while (arr[i] < pivot);
        // 从右找第一个 < pivot 的元素
        do { j--; } while (arr[j] > pivot);
        if (i >= j) return j;
        swap(arr[i], arr[j]);
    }
}

// 三路快排（处理大量重复元素的优化版本）
// 将数组分为三部分: <pivot, ==pivot, >pivot
void threeWayPartition(vector<int>& arr, int low, int high) {
    if (low >= high) return;

    int pivot = arr[low];
    int lt = low;      // arr[low..lt-1] < pivot
    int gt = high;     // arr[gt+1..high] > pivot
    int i = low + 1;   // arr[lt..i-1] == pivot

    while (i <= gt) {
        if (arr[i] < pivot) {
            swap(arr[lt++], arr[i++]);
        } else if (arr[i] > pivot) {
            swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    // 递归排序小于和大于pivot的部分
    threeWayPartition(arr, low, lt - 1);
    threeWayPartition(arr, gt + 1, high);
}

// 快速排序（使用Lomuto分区）
void quickSortLomuto(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = lomutoPartition(arr, low, high);
        quickSortLomuto(arr, low, pi - 1);
        quickSortLomuto(arr, pi + 1, high);
    }
}

// 快速排序（使用Hoare分区）
void quickSortHoare(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = hoarePartition(arr, low, high);
        quickSortHoare(arr, low, pi);
        quickSortHoare(arr, pi + 1, high);
    }
}

// 便捷接口
void quickSort(vector<int>& arr) {
    if (!arr.empty()) {
        quickSortLomuto(arr, 0, arr.size() - 1);
    }
}

void testQuickSort() {
    separator("6. 快速排序 (Quick Sort)");
    cout << endl;

    // Lomuto分区演示
    cout << "--- Lomuto 分区方案 ---" << endl;
    cout << "分区过程 (pivot = 最后一个元素):" << endl;
    {
        vector<int> a = {5, 3, 8, 1, 2, 7};
        printArray(a, "初始");
        int pivot = a.back();
        int i = -1;
        for (int j = 0; j < (int)a.size() - 1; j++) {
            if (a[j] <= pivot) {
                i++;
                swap(a[i], a[j]);
                cout << "  " << a[j] << "<=" << pivot
                     << ", 交换到位置" << i << ": ";
                printArray(a, "");
            }
        }
        swap(a[i + 1], a.back());
        cout << "  pivot " << pivot << " 放到位置 " << (i + 1) << ": ";
        printArray(a, "");
    }

    // Lomuto快排
    cout << "\n--- Lomuto 快速排序 ---" << endl;
    vector<int> arr1 = {5, 3, 8, 1, 2, 7};
    quickSortLomuto(arr1, 0, arr1.size() - 1);
    printArray(arr1, "结果");

    // Hoare快排
    cout << "\n--- Hoare 快速排序 ---" << endl;
    vector<int> arr2 = {5, 3, 8, 1, 2, 7};
    quickSortHoare(arr2, 0, arr2.size() - 1);
    printArray(arr2, "结果");

    // 三路快排（处理重复元素）
    cout << "\n--- 三路快速排序 (处理重复元素) ---" << endl;
    vector<int> arr3 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    printArray(arr3, "输入(含重复)");
    threeWayPartition(arr3, 0, arr3.size() - 1);
    printArray(arr3, "三路快排结果");

    // 最坏情况
    cout << "\n最坏情况分析:" << endl;
    cout << "  当数组已排序 + 选首/尾元素为pivot时，时间退化为 O(n^2)" << endl;
    cout << "  解决方案: 随机选择pivot 或 三数取中法" << endl;

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: 平均 O(n log n), 最坏 O(n^2)" << endl;
    cout << "  空间复杂度: O(log n)" << endl;
    cout << "  稳定性: 不稳定" << endl;
    cout << "  适用场景: 通用排序首选, 平均性能最优, 原地排序" << endl;
}


// ============================================================================
//  7. 堆排序 (Heap Sort)
// ============================================================================
/*
 * 基本思想：
 *   利用堆(完全二叉树)数据结构。
 *   1. 将数组构建成最大堆(Max Heap)
 *   2. 反复取出堆顶(最大值)放到数组末尾
 *   3. 调整堆使其重新满足最大堆性质
 *
 * 最大堆性质: 父节点 >= 子节点
 *
 * 堆的数组表示: 索引i的子节点在 2i+1 和 2i+2
 *
 * 构建最大堆过程 (对 [4, 10, 3, 5, 1] 排序):
 *
 *   原始数组视为完全二叉树:
 *            4
 *           / \
 *         10   3
 *        / \
 *       5   1
 *
 *   从最后一个非叶子节点开始下沉调整:
 *     节点10(索引1): 10>5, 10>1, 不需调整
 *     节点4(索引0):  4<10, 交换4和10
 *            10
 *           /  \
 *         4     3
 *        / \
 *       5   1
 *     然后 4<5, 交换4和5
 *            10
 *           /  \
 *         5     3
 *        / \
 *       4   1
 *
 *   最大堆构建完成: [10, 5, 3, 4, 1]
 *
 *   排序过程:
 *     取出10, 与末尾交换: [1, 5, 3, 4 | 10]
 *     调整堆:             [5, 4, 3, 1 | 10]
 *     取出5,  与末尾交换: [1, 4, 3 | 5, 10]
 *     调整堆:             [4, 1, 3 | 5, 10]
 *     取出4,  与末尾交换: [3, 1 | 4, 5, 10]
 *     调整堆:             [3, 1 | 4, 5, 10]
 *     取出3,  与末尾交换: [1 | 3, 4, 5, 10]
 *     最终:               [1, 3, 4, 5, 10]
 *
 * 时间复杂度: O(n log n) (建堆O(n) + n次调整O(log n))
 * 空间复杂度: O(1)
 * 稳定性: 不稳定
 */

// 下沉调整（维护最大堆性质）
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;        // 假设父节点最大
    int left = 2 * i + 1;  // 左子节点
    int right = 2 * i + 2; // 右子节点

    // 找到父节点和子节点中的最大值
    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // 如果最大值不是父节点，交换并继续调整
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);  // 递归调整受影响的子树
    }
}

// 堆排序
void heapSort(vector<int>& arr) {
    int n = arr.size();

    // 步骤1: 构建最大堆 (从最后一个非叶子节点开始)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // 步骤2: 反复取出最大值
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);  // 将堆顶(最大值)移到末尾
        heapify(arr, i, 0);    // 调整剩余元素为最大堆
    }
}

// 打印堆的树形结构
void printHeapTree(const vector<int>& arr, int n) {
    if (n == 0) return;
    int levels = 0;
    int temp = n;
    while (temp > 0) {
        levels++;
        temp >>= 1;
    }

    int idx = 0;
    for (int level = 0; level < levels && idx < n; level++) {
        int nodesInLevel = 1 << level;  // 2^level
        int spaces = (1 << (levels - level)) - 1;

        // 打印前导空格
        cout << string(spaces * 2, ' ');

        for (int i = 0; i < nodesInLevel && idx < n; i++) {
            cout << setw(3) << arr[idx];
            idx++;
            // 打印节点间空格
            int betweenSpaces = (1 << (levels - level + 1)) - 1;
            cout << string(betweenSpaces * 2, ' ');
        }
        cout << endl;
    }
}

void testHeapSort() {
    separator("7. 堆排序 (Heap Sort)");
    cout << endl;

    cout << "建堆过程:" << endl;
    vector<int> a = {4, 10, 3, 5, 1};
    printArray(a, "原始数组");
    cout << "\n原始二叉树:" << endl;
    printHeapTree(a, a.size());

    // 展示建堆
    int n = a.size();
    cout << "\n从最后一个非叶子节点(索引" << n / 2 - 1 << ")开始调整:" << endl;
    for (int i = n / 2 - 1; i >= 0; i--) {
        cout << "  调整节点 " << a[i] << "(索引" << i << "):" << endl;
        heapify(a, n, i);
        cout << "    ";
        printArray(a, "");
    }

    cout << "\n最大堆:" << endl;
    printHeapTree(a, a.size());

    cout << "\n排序过程:" << endl;
    for (int i = n - 1; i > 0; i--) {
        cout << "  取出堆顶 " << a[0] << " 与位置" << i << "交换: ";
        swap(a[0], a[i]);
        heapify(a, i, 0);
        printArray(a, "");
    }

    vector<int> arr = {4, 10, 3, 5, 1};
    heapSort(arr);
    printArray(arr, "最终结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(n log n)" << endl;
    cout << "  空间复杂度: O(1)" << endl;
    cout << "  稳定性: 不稳定" << endl;
    cout << "  适用场景: 需要原地O(n log n)排序, 嵌入式系统, 优先队列" << endl;
}


// ============================================================================
//  8. 计数排序 (Counting Sort)
// ============================================================================
/*
 * 基本思想：
 *   统计每个值出现的次数，然后根据统计结果直接输出有序数组。
 *   适用于值域范围不大的非负整数排序。
 *
 * 过程图示 (对 [4, 2, 2, 8, 3, 3, 1] 排序):
 *
 *   1. 统计每个值出现次数:
 *      值:    0  1  2  3  4  5  6  7  8
 *      次数:  0  1  2  2  1  0  0  0  1
 *
 *   2. 计算累积次数(确定每个值的最终位置):
 *      值:    0  1  2  3  4  5  6  7  8
 *      累积:  0  1  3  5  6  6  6  6  7
 *      (值<=0的有0个, 值<=1的有1个, 值<=2的有3个, ...)
 *
 *   3. 从后往前遍历原数组，根据累积次数放置元素:
 *      取 1: 累积[1]=1, 放到位置0, 累积[1]-- -> [1, _, _, _, _, _, _]
 *      取 3: 累积[3]=5, 放到位置4, 累积[3]-- -> [1, _, _, _, 3, _, _]
 *      取 3: 累积[3]=4, 放到位置3, 累积[3]-- -> [1, _, _, 3, 3, _, _]
 *      取 8: 累积[8]=7, 放到位置6, 累积[8]-- -> [1, _, _, 3, 3, _, 8]
 *      取 2: 累积[2]=3, 放到位置2, 累积[2]-- -> [1, _, 2, 3, 3, _, 8]
 *      取 2: 累积[2]=2, 放到位置1, 累积[2]-- -> [1, 2, 2, 3, 3, _, 8]
 *      取 4: 累积[4]=6, 放到位置5, 累积[4]-- -> [1, 2, 2, 3, 3, 4, 8]
 *
 * 时间复杂度: O(n + k), k为值域范围
 * 空间复杂度: O(k)
 * 稳定性: 稳定 (从后往前遍历保证稳定性)
 */
void countingSort(vector<int>& arr) {
    if (arr.empty()) return;

    // 找到最大值和最小值
    int maxVal = *max_element(arr.begin(), arr.end());
    int minVal = *min_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    // 创建计数数组
    vector<int> count(range, 0);
    vector<int> output(arr.size());

    // 统计每个值出现的次数
    for (int x : arr) {
        count[x - minVal]++;
    }

    // 计算累积次数
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    // 从后往前遍历，保证稳定性
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }

    // 复制回原数组
    arr = output;
}

void testCountingSort() {
    separator("8. 计数排序 (Counting Sort)");
    cout << endl;

    cout << "排序过程:" << endl;
    vector<int> a = {4, 2, 2, 8, 3, 3, 1};
    printArray(a, "原始数组");

    int maxVal = *max_element(a.begin(), a.end());
    int minVal = *min_element(a.begin(), a.end());

    // 统计
    vector<int> count(maxVal - minVal + 1, 0);
    for (int x : a) count[x - minVal]++;

    cout << "  值:   ";
    for (int i = minVal; i <= maxVal; i++) cout << setw(4) << i;
    cout << endl;
    cout << "  次数: ";
    for (int i = 0; i < (int)count.size(); i++) cout << setw(4) << count[i];
    cout << endl;

    // 累积
    for (int i = 1; i < (int)count.size(); i++) count[i] += count[i - 1];
    cout << "  累积: ";
    for (int i = 0; i < (int)count.size(); i++) cout << setw(4) << count[i];
    cout << endl;

    vector<int> arr = {4, 2, 2, 8, 3, 3, 1};
    countingSort(arr);
    printArray(arr, "结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    // 测试含负数
    cout << "\n含负数测试:" << endl;
    vector<int> arr2 = {-3, 5, 2, -1, 0, 4, -2};
    printArray(arr2, "输入");
    countingSort(arr2);
    printArray(arr2, "结果");

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(n + k), k为值域范围" << endl;
    cout << "  空间复杂度: O(k)" << endl;
    cout << "  稳定性: 稳定" << endl;
    cout << "  适用场景: 值域范围小的整数排序(如考试成绩0-100)" << endl;
    cout << "  注意: 当 k >> n 时不适合使用" << endl;
}


// ============================================================================
//  9. 基数排序 (Radix Sort)
// ============================================================================
/*
 * 基本思想：
 *   按位排序，从最低位(LSD)到最高位，每一位使用稳定的计数排序。
 *   由于每一位的范围只有0-9，计数排序非常高效。
 *
 * LSD (Least Significant Digit): 从个位开始排
 * MSD (Most Significant Digit):  从最高位开始排
 *
 * 过程图示 (对 [170, 45, 75, 90, 802, 24, 2, 66] 排序):
 *
 *   原始: [170, 45, 75, 90, 802, 24, 2, 66]
 *
 *   按个位排序 (LSD):
 *     0: 170, 90
 *     2: 802, 2
 *     4: 24
 *     5: 45, 75
 *     6: 66
 *     结果: [170, 90, 802, 2, 24, 45, 75, 66]
 *
 *   按十位排序:
 *     0: 802, 2
 *     2: 24
 *     4: 45
 *     6: 66
 *     7: 170, 75
 *     9: 90
 *     结果: [802, 2, 24, 45, 66, 170, 75, 90]
 *
 *   按百位排序:
 *     0: 2, 24, 45, 66, 75, 90
 *     1: 170
 *     8: 802
 *     结果: [2, 24, 45, 66, 75, 90, 170, 802]
 *
 * 时间复杂度: O(d * (n + k)), d为位数, k为基数(通常10)
 * 空间复杂度: O(n + k)
 * 稳定性: 稳定 (计数排序本身是稳定的)
 */

// 对指定位数进行计数排序
void countingSortByDigit(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);
    vector<int> count(10, 0);  // 基数为10 (0-9)

    // 统计当前位的数字出现次数
    for (int i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // 计算累积位置
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // 从后往前，保证稳定性
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    arr = output;
}

// LSD基数排序
void radixSortLSD(vector<int>& arr) {
    if (arr.empty()) return;

    // 处理负数: 分离正负数分别排序
    vector<int> negatives, positives;
    for (int x : arr) {
        if (x < 0) negatives.push_back(-x);  // 负数取绝对值
        else positives.push_back(x);
    }

    // 对正数排序
    if (!positives.empty()) {
        int maxVal = *max_element(positives.begin(), positives.end());
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSortByDigit(positives, exp);
        }
    }

    // 对负数的绝对值排序
    if (!negatives.empty()) {
        int maxVal = *max_element(negatives.begin(), negatives.end());
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSortByDigit(negatives, exp);
        }
    }

    // 合并: 负数反转取负 + 正数
    arr.clear();
    for (int i = negatives.size() - 1; i >= 0; i--) {
        arr.push_back(-negatives[i]);
    }
    for (int x : positives) {
        arr.push_back(x);
    }
}

void testRadixSort() {
    separator("9. 基数排序 (Radix Sort)");
    cout << endl;

    cout << "LSD基数排序过程:" << endl;
    vector<int> a = {170, 45, 75, 90, 802, 24, 2, 66};
    printArray(a, "原始数组");
    cout << endl;

    int maxVal = *max_element(a.begin(), a.end());
    // 手动展示每轮
    {
        vector<int> tmp = a;
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countingSortByDigit(tmp, exp);
            string digitName;
            if (exp == 1) digitName = "个位";
            else if (exp == 10) digitName = "十位";
            else if (exp == 100) digitName = "百位";
            printArray(tmp, "按" + digitName + "排序后");
        }
    }

    vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66};
    radixSortLSD(arr);
    printArray(arr, "结果");
    cout << "已排序: " << (isSorted(arr) ? "是" : "否") << endl;

    // 测试含负数
    cout << "\n含负数测试:" << endl;
    vector<int> arr2 = {-170, 45, -75, 90, 802, -24, 2, -66};
    printArray(arr2, "输入");
    radixSortLSD(arr2);
    printArray(arr2, "结果");

    cout << "\n特性总结:" << endl;
    cout << "  时间复杂度: O(d * (n + k)), d为最大位数, k=10" << endl;
    cout << "  空间复杂度: O(n + k)" << endl;
    cout << "  稳定性: 稳定" << endl;
    cout << "  适用场景: 整数排序, 位数不太大的情况" << endl;
    cout << "  注意: 当d很大时不如比较排序" << endl;
}


// ============================================================================
//  排序算法对比总结
// ============================================================================
void printComparisonTable() {
    separator("排序算法对比总结");

    cout << endl;
    cout << "+----------------+------------+------------+------------+--------+-----------+" << endl;
    cout << "| 算法           | 最好时间   | 最坏时间   | 平均时间   | 空间   | 稳定性    |" << endl;
    cout << "+----------------+------------+------------+------------+--------+-----------+" << endl;
    cout << "| 冒泡排序       | O(n)       | O(n^2)     | O(n^2)     | O(1)   | 稳定      |" << endl;
    cout << "| 选择排序       | O(n^2)     | O(n^2)     | O(n^2)     | O(1)   | 不稳定    |" << endl;
    cout << "| 插入排序       | O(n)       | O(n^2)     | O(n^2)     | O(1)   | 稳定      |" << endl;
    cout << "| 希尔排序       | O(nlogn)   | O(n^2)     | O(n^1.25)  | O(1)   | 不稳定    |" << endl;
    cout << "| 归并排序       | O(nlogn)   | O(nlogn)   | O(nlogn)   | O(n)   | 稳定      |" << endl;
    cout << "| 快速排序       | O(nlogn)   | O(n^2)     | O(nlogn)   | O(logn)| 不稳定    |" << endl;
    cout << "| 堆排序         | O(nlogn)   | O(nlogn)   | O(nlogn)   | O(1)   | 不稳定    |" << endl;
    cout << "| 计数排序       | O(n+k)     | O(n+k)     | O(n+k)     | O(k)   | 稳定      |" << endl;
    cout << "| 基数排序       | O(d(n+k))  | O(d(n+k))  | O(d(n+k))  | O(n+k) | 稳定      |" << endl;
    cout << "+----------------+------------+------------+------------+--------+-----------+" << endl;

    cout << "\n选择排序算法的建议:" << endl;
    cout << "  1. 小规模数据(n<50):        插入排序 (简单高效)" << endl;
    cout << "  2. 通用排序:                 快速排序 (平均最优)" << endl;
    cout << "  3. 需要稳定排序:             归并排序" << endl;
    cout << "  4. 内存受限:                 堆排序 (原地O(nlogn))" << endl;
    cout << "  5. 整数且值域小:             计数排序" << endl;
    cout << "  6. 整数且位数不大:           基数排序" << endl;
    cout << "  7. 外部排序(数据在磁盘):     归并排序" << endl;
    cout << "  8. 几乎有序:                 插入排序" << endl;
    cout << "  9. 大量重复元素:             三路快排" << endl;
}


// ============================================================================
//  主函数
// ============================================================================
int main() {
    cout << "====================================================================" << endl;
    cout << "       排序算法大全 - Complete Sorting Algorithm Reference" << endl;
    cout << "====================================================================" << endl;

    testBubbleSort();
    testSelectionSort();
    testInsertionSort();
    testShellSort();
    testMergeSort();
    testQuickSort();
    testHeapSort();
    testCountingSort();
    testRadixSort();
    printComparisonTable();

    cout << "\n" << string(70, '=') << endl;
    cout << "  所有排序算法测试完成!" << endl;
    cout << string(70, '=') << endl;

    return 0;
}
