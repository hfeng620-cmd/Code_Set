/*
 * ============================================================
 *  主题: break 和 continue 详解
 * ============================================================
 *  break 和 continue 是循环控制语句，用于改变循环的正常
 *  执行流程。本文件涵盖:
 *    1. break: 立即终止当前循环
 *    2. continue: 跳过本次迭代，进入下一次
 *    3. 在 for 和 while 中的使用
 *    4. 在嵌套循环中的 break
 *    5. 常见错误和注意事项
 * ============================================================
 */

#include <iostream>
using namespace std;

int main()
{
    // ========================================
    // 1. break 语句
    // ========================================
    // break 立即终止当前所在的最内层循环
    // 程序继续执行循环之后的代码

    // 示例: 找到第一个能被 7 整除的数就停止
    cout << "--- break 示例: 找第一个能被 7 整除的数 ---" << endl;
    for (int i = 1; i <= 100; i++)
    {
        if (i % 7 == 0)
        {
            cout << "找到: " << i << endl;
            break;      // 找到后立即退出循环
        }
        cout << i << " ";
    }
    cout << endl;

    // 在 while 中使用 break
    cout << "--- while 中的 break ---" << endl;
    int count = 0;
    while (true)   // 无限循环
    {
        count++;
        if (count > 5)
        {
            cout << "count 超过 5，退出循环" << endl;
            break;      // 条件满足时退出
        }
        cout << "count = " << count << endl;
    }

    // ========================================
    // 2. continue 语句
    // ========================================
    // continue 跳过当前迭代的剩余代码，直接进入下一次迭代
    // for 循环: 跳到更新表达式 (如 i++)
    // while 循环: 跳到条件判断

    // 示例: 打印 1~10 中的奇数 (跳过偶数)
    cout << "--- continue 示例: 只打印奇数 ---" << endl;
    for (int i = 1; i <= 10; i++)
    {
        if (i % 2 == 0)
        {
            continue;   // 跳过偶数，直接进入下一次循环
        }
        cout << i << " ";
    }
    cout << endl;

    // 等价的不使用 continue 的写法:
    cout << "--- 不使用 continue 的等价写法 ---" << endl;
    for (int i = 1; i <= 10; i++)
    {
        if (i % 2 != 0)   // 只处理奇数
        {
            cout << i << " ";
        }
    }
    cout << endl;

    // ========================================
    // 3. continue 在 for 和 while 中的区别
    // ========================================
    // for 循环中 continue: 跳转到更新表达式 (i++)
    // while 循环中 continue: 跳转到条件判断 (跳过更新!)

    cout << "--- for 中的 continue (安全) ---" << endl;
    for (int i = 1; i <= 5; i++)
    {
        if (i == 3)
        {
            continue;   // 跳过 i==3，但 i++ 仍会执行
        }
        cout << i << " ";  // 输出: 1 2 4 5
    }
    cout << endl;

    // while 中使用 continue 需要特别小心!
    cout << "--- while 中的 continue (需谨慎) ---" << endl;
    int w = 0;
    while (w < 5)
    {
        w++;            // 必须在 continue 之前更新!
        if (w == 3)
        {
            continue;   // 跳到条件判断，如果 w++ 在这里之后，会导致死循环
        }
        cout << w << " ";  // 输出: 1 2 4 5
    }
    cout << endl;

    // 危险示例 (死循环!):
    // int danger = 0;
    // while (danger < 5)
    // {
    //     if (danger == 3)
    //     {
    //         continue;   // 跳到条件判断，danger 仍是 3，无限循环!
    //     }
    //     cout << danger << endl;
    //     danger++;       // 这行在 continue 之后，当 danger==3 时永远不会执行
    // }

    // ========================================
    // 4. 嵌套循环中的 break
    // ========================================
    // break 只能跳出最内层循环，不能直接跳出外层循环

    cout << "--- 嵌套循环中的 break ---" << endl;
    for (int i = 1; i <= 3; i++)
    {
        cout << "外层 i=" << i << ": ";
        for (int j = 1; j <= 5; j++)
        {
            if (j == 3)
            {
                break;  // 只跳出内层 for (j 的循环)
            }
            cout << j << " ";
        }
        cout << endl;   // 外层循环继续执行
    }
    // 输出:
    // 外层 i=1: 1 2
    // 外层 i=2: 1 2
    // 外层 i=3: 1 2

    // 要跳出外层循环，可以使用标志变量或 goto
    cout << "--- 使用标志跳出外层循环 ---" << endl;
    bool shouldBreak = false;
    for (int i = 1; i <= 5 && !shouldBreak; i++)
    {
        for (int j = 1; j <= 5; j++)
        {
            if (i * j > 6)
            {
                cout << "i=" << i << ", j=" << j << " 时 i*j=" << i * j << " > 6，停止" << endl;
                shouldBreak = true;
                break;      // 跳出内层循环
            }
            cout << "(" << i << "," << j << ") ";
        }
        cout << endl;
    }

    // ========================================
    // 5. 常见错误和注意事项
    // ========================================

    // 错误 1: 在 switch 中误用 continue
    // continue 只能用在循环中，不能用在 switch 中!
    // switch (x) {
    //     case 1:
    //         continue;  // 编译错误! switch 不是循环
    // }

    // 错误 2: while 中 continue 前忘记更新循环变量
    // 见上面的危险示例

    // 错误 3: 混淆 break 和 continue
    cout << "--- break vs continue 对比 ---" << endl;

    // break: 遇到 5 就停止
    cout << "break: ";
    for (int i = 1; i <= 10; i++)
    {
        if (i == 5)
            break;
        cout << i << " ";   // 输出: 1 2 3 4
    }
    cout << endl;

    // continue: 跳过 5 继续
    cout << "continue: ";
    for (int i = 1; i <= 10; i++)
    {
        if (i == 5)
            continue;
        cout << i << " ";   // 输出: 1 2 3 4 6 7 8 9 10
    }
    cout << endl;

    // ========================================
    // 6. 实际应用: 查找素数
    // ========================================
    cout << "--- 20 以内的素数 ---" << endl;
    for (int num = 2; num <= 20; num++)
    {
        bool isPrime = true;
        for (int d = 2; d * d <= num; d++)   // 只需检查到 sqrt(num)
        {
            if (num % d == 0)
            {
                isPrime = false;
                break;      // 找到因子，不是素数，立即退出内层循环
            }
        }
        if (isPrime)
        {
            cout << num << " ";
        }
    }
    cout << endl;

    // ========================================
    // 7. 实际应用: 跳过特定值
    // ========================================
    // 打印 1~20 中不能被 3 整除的数的平方
    cout << "--- 不能被 3 整除的数的平方 ---" << endl;
    for (int i = 1; i <= 20; i++)
    {
        if (i % 3 == 0)
        {
            continue;   // 跳过能被 3 整除的数
        }
        cout << i * i << " ";
    }
    cout << endl;

    cout << endl << "=== break 和 continue 详解完毕 ===" << endl;
    return 0;
}
