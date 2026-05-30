/*
 * ============================================================
 *  主题: 数组排序与查找算法
 * ============================================================
 *  排序和查找是数组操作中最基础、最重要的算法。本文件涵盖:
 *    1. 冒泡排序 (Bubble Sort)
 *    2. 选择排序 (Selection Sort)
 *    3. 线性查找 (Linear Search)
 *    4. 二分查找 (Binary Search)
 *    5. 逐步演示算法执行过程
 * ============================================================
 */

#include <iostream>
using namespace std;

int main()
{
    // ========================================
    // 1. 冒泡排序 (Bubble Sort)
    // ========================================
    // 原理: 相邻元素两两比较，较大的元素"冒泡"到末尾
    // 时间复杂度: O(n^2)
    // 空间复杂度: O(1)
    // 稳定排序

    cout << "========== 冒泡排序 ==========" << endl;
    int arr1[] = {64, 34, 25, 12, 22, 11, 90};
    int len1 = sizeof(arr1) / sizeof(arr1[0]);

    cout << "排序前: ";
    for (int i = 0; i < len1; i++)
        cout << arr1[i] << " ";
    cout << endl;

    // 冒泡排序: 外层控制轮数，内层进行相邻比较
    for (int i = 0; i < len1 - 1; i++)           // n-1 轮
    {
        bool swapped = false;   // 优化: 如果某轮没有交换，说明已经有序
        for (int j = 0; j < len1 - 1 - i; j++)   // 每轮少比较一个 (末尾已排好)
        {
            if (arr1[j] > arr1[j + 1])
            {
                // 交换相邻元素
                int temp = arr1[j];
                arr1[j] = arr1[j + 1];
                arr1[j + 1] = temp;
                swapped = true;
            }
        }
        // 可视化每轮结果
        cout << "第 " << i + 1 << " 轮: ";
        for (int k = 0; k < len1; k++)
            cout << arr1[k] << " ";
        cout << endl;

        if (!swapped)
        {
            cout << "第 " << i + 1 << " 轮无交换，提前结束" << endl;
            break;
        }
    }

    cout << "排序后: ";
    for (int i = 0; i < len1; i++)
        cout << arr1[i] << " ";
    cout << endl;

    // ========================================
    // 2. 选择排序 (Selection Sort)
    // ========================================
    // 原理: 每轮从未排序部分选出最小元素，放到已排序末尾
    // 时间复杂度: O(n^2)
    // 空间复杂度: O(1)
    // 不稳定排序

    cout << endl << "========== 选择排序 ==========" << endl;
    int arr2[] = {64, 34, 25, 12, 22, 11, 90};
    int len2 = sizeof(arr2) / sizeof(arr2[0]);

    cout << "排序前: ";
    for (int i = 0; i < len2; i++)
        cout << arr2[i] << " ";
    cout << endl;

    for (int i = 0; i < len2 - 1; i++)   // n-1 轮
    {
        int minIdx = i;     // 假设当前位置是最小值
        for (int j = i + 1; j < len2; j++)   // 在未排序部分找最小值
        {
            if (arr2[j] < arr2[minIdx])
            {
                minIdx = j;   // 更新最小值下标
            }
        }
        // 将找到的最小值与当前位置交换
        if (minIdx != i)
        {
            int temp = arr2[i];
            arr2[i] = arr2[minIdx];
            arr2[minIdx] = temp;
        }
        // 可视化每轮结果
        cout << "第 " << i + 1 << " 轮: ";
        for (int k = 0; k < len2; k++)
            cout << arr2[k] << " ";
        cout << " (找到最小值 " << arr2[i] << " 放到位置 " << i << ")" << endl;
    }

    cout << "排序后: ";
    for (int i = 0; i < len2; i++)
        cout << arr2[i] << " ";
    cout << endl;

    // ========================================
    // 3. 线性查找 (Linear Search)
    // ========================================
    // 原理: 从头到尾逐个比较
    // 时间复杂度: O(n)
    // 适用于: 无序数组或小数组

    cout << endl << "========== 线性查找 ==========" << endl;
    int data[] = {45, 12, 78, 34, 56, 23, 89, 67};
    int dataLen = sizeof(data) / sizeof(data[0]);
    int target = 56;

    cout << "数组: ";
    for (int i = 0; i < dataLen; i++)
        cout << data[i] << " ";
    cout << endl;
    cout << "查找目标: " << target << endl;

    int foundIdx = -1;   // -1 表示未找到
    for (int i = 0; i < dataLen; i++)
    {
        cout << "  比较 data[" << i << "]=" << data[i];
        if (data[i] == target)
        {
            cout << " == " << target << " (找到了!)" << endl;
            foundIdx = i;
            break;       // 找到后退出
        }
        cout << " != " << target << endl;
    }

    if (foundIdx != -1)
    {
        cout << "找到目标 " << target << "，下标为 " << foundIdx << endl;
    }
    else
    {
        cout << "未找到目标 " << target << endl;
    }

    // 查找不存在的元素
    int notExist = 999;
    bool found = false;
    for (int i = 0; i < dataLen; i++)
    {
        if (data[i] == notExist)
        {
            found = true;
            break;
        }
    }
    cout << "查找 " << notExist << ": " << (found ? "找到" : "未找到") << endl;

    // ========================================
    // 4. 二分查找 (Binary Search)
    // ========================================
    // 原理: 在有序数组中，每次取中间元素比较，缩小一半范围
    // 时间复杂度: O(log n)  -- 比线性查找快得多!
    // 前提: 数组必须是有序的

    cout << endl << "========== 二分查找 ==========" << endl;
    int sorted[] = {2, 5, 8, 12, 16, 23, 38, 45, 56, 72, 91};
    int sortedLen = sizeof(sorted) / sizeof(sorted[0]);
    int target2 = 23;

    cout << "有序数组: ";
    for (int i = 0; i < sortedLen; i++)
        cout << sorted[i] << " ";
    cout << endl;
    cout << "查找目标: " << target2 << endl;

    int left = 0;                    // 左边界
    int right = sortedLen - 1;       // 右边界
    int result = -1;                 // 结果下标
    int step = 0;                    // 记录查找步数

    while (left <= right)
    {
        step++;
        int mid = left + (right - left) / 2;   // 防止 (left+right) 溢出
        cout << "  第 " << step << " 步: left=" << left
             << ", right=" << right << ", mid=" << mid
             << ", sorted[mid]=" << sorted[mid] << endl;

        if (sorted[mid] == target2)
        {
            result = mid;
            cout << "  -> 找到目标! 下标 = " << mid << endl;
            break;
        }
        else if (sorted[mid] < target2)
        {
            left = mid + 1;       // 目标在右半部分
            cout << "  -> 目标在右半部分, left = " << left << endl;
        }
        else
        {
            right = mid - 1;      // 目标在左半部分
            cout << "  -> 目标在左半部分, right = " << right << endl;
        }
    }

    if (result != -1)
    {
        cout << "二分查找: 找到 " << target2 << "，下标为 " << result
             << "，共 " << step << " 步" << endl;
    }
    else
    {
        cout << "二分查找: 未找到 " << target2 << endl;
    }

    // 对比: 线性查找需要多少步
    cout << "线性查找最多需要 " << sortedLen << " 步，二分查找最多需要约 "
         << sortedLen << " 个元素中 log2(" << sortedLen << ") ≈ 3~4 步" << endl;

    // ========================================
    // 5. 二分查找的变体: 查找插入位置
    // ========================================
    cout << endl << "--- 二分查找: 查找插入位置 ---" << endl;
    int arr3[] = {1, 3, 5, 7, 9, 11, 13};
    int arr3Len = sizeof(arr3) / sizeof(arr3[0]);
    int insertVal = 6;

    // 使用二分查找找到 insertVal 应该插入的位置
    int l = 0, r = arr3Len - 1;
    int insertPos = arr3Len;  // 默认插入到末尾

    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (arr3[m] < insertVal)
        {
            l = m + 1;
        }
        else
        {
            insertPos = m;
            r = m - 1;
        }
    }
    cout << "值 " << insertVal << " 应插入到下标 " << insertPos << " 处" << endl;

    // ========================================
    // 6. 排序算法对比
    // ========================================
    cout << endl << "========== 排序算法对比 ==========" << endl;
    cout << "算法     | 平均时间   | 最坏时间   | 空间  | 稳定" << endl;
    cout << "---------|-----------|-----------|-------|------" << endl;
    cout << "冒泡排序 | O(n^2)    | O(n^2)    | O(1)  | 是" << endl;
    cout << "选择排序 | O(n^2)    | O(n^2)    | O(1)  | 否" << endl;
    cout << "插入排序 | O(n^2)    | O(n^2)    | O(1)  | 是" << endl;
    cout << "快速排序 | O(nlogn)  | O(n^2)    | O(logn)| 否" << endl;
    cout << "归并排序 | O(nlogn)  | O(nlogn)  | O(n)  | 是" << endl;

    cout << endl << "=== 数组排序与查找算法详解完毕 ===" << endl;
    return 0;
}
