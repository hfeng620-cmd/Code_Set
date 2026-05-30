/*
 * ============================================================================
 * 黑马程序员 C++课程 - STL实践：演讲比赛管理系统
 * ============================================================================
 *
 * 【项目背景】
 * 演讲比赛管理系统是黑马程序员课程中STL（标准模板库）部分的核心实战项目。
 * 该项目综合运用了多种STL容器和算法，是理解STL实际应用的经典案例。
 *
 * 【比赛规则】
 * - 共有12名选手参加比赛
 * - 比赛分为2轮：初赛和决赛
 * - 每轮比赛有10位评委打分
 * - 每位选手的最终得分 = 去掉一个最高分和一个最低分后的平均分
 * - 每轮比赛后，排名前6的选手晋级下一轮
 * - 决赛后产生最终名次
 *
 * 【STL容器使用】
 * - vector<选手信息>：存储选手列表，支持随机访问和排序
 * - deque<double>：存储评委打分，方便两端操作（去最高/最低）
 * - map<int, 选手信息>：按编号存储选手，方便按编号查找
 * - multimap<double, int, greater<>>：按分数排序（降序）
 *
 * 【STL算法使用】
 * - sort()：对选手按分数排序
 * - for_each()：遍历并显示选手信息
 * - accumulate()：计算总分
 * - max_element() / min_element()：找最高/最低分
 * - random_shuffle() / shuffle()：随机抽签分组
 *
 * 【数据结构设计】
 * 选手信息结构体包含：编号、姓名、分数列表、最终得分
 *
 * 编译方法：g++ 95_STL实践_演讲比赛.cpp -o speech_contest
 * ============================================================================
 */

#include <iostream>      // 标准输入输出
#include <string>        // 字符串类
#include <vector>        // vector容器 - 存储选手列表
#include <deque>         // deque容器 - 存储评委打分
#include <map>           // map容器 - 按编号存储选手
#include <multimap>      // multimap - 按分数排序
#include <algorithm>     // STL算法：sort, for_each, shuffle等
#include <numeric>       // 数值算法：accumulate
#include <functional>    // 函数对象：greater<>
#include <iomanip>       // 格式化输出
#include <ctime>         // 时间函数，用于随机数种子
#include <cstdlib>       // rand, srand
#include <fstream>       // 文件输出
#include <sstream>       // 字符串流

using namespace std;

// ============================================================================
// 【常量定义】
// ============================================================================

const int PLAYER_COUNT   = 12;   // 选手总数
const int JUDGE_COUNT    = 10;   // 评委人数
const int ADVANCE_COUNT  = 6;    // 每轮晋级人数
const int ROUND_COUNT    = 2;    // 比赛轮数

// ============================================================================
// 【数据结构：选手信息】
// ============================================================================

/*
 * 选手结构体 - 存储单个选手的所有信息
 *
 * 设计思考：为什么把分数列表也放在结构体里？
 * 因为每位选手在每轮比赛中都有自己的评分列表，
 * 将分数和选手信息放在一起，便于管理和传递。
 *
 * 使用deque存储分数的原因：
 * deque（双端队列）支持高效的两端插入和删除操作。
 * 在计算最终得分时，我们需要去掉最高分和最低分，
 * 如果使用deque，可以在排序后直接pop_front()和pop_back()，
 * 比vector的erase操作更高效。
 */
struct Speaker
{
    int id;                   // 选手编号
    string name;              // 选手姓名
    deque<double> scores;     // 评委打分列表
    double finalScore;        // 最终得分（去极值后的平均分）

    // 构造函数，初始化成员变量
    Speaker() : id(0), name(""), finalScore(0.0) {}
    Speaker(int id, string name) : id(id), name(name), finalScore(0.0) {}
};

// ============================================================================
// 【辅助函数】
// ============================================================================

/*
 * generateName() - 生成选手姓名
 *
 * 为了演示方便，我们自动生成12位选手的姓名。
 * 实际项目中可以从文件或用户输入获取。
 *
 * 设计说明：使用字符串数组存储预设姓名，
 * 这是一种简单直接的测试数据生成方式。
 */
string generateName(int index)
{
    // 预设的选手姓名列表
    static string names[] = {
        "张三", "李四", "王五", "赵六", "钱七", "孙八",
        "周九", "吴十", "郑一", "冯二", "陈三", "褚四"
    };

    if (index >= 0 && index < PLAYER_COUNT)
    {
        return names[index];
    }
    return "未知选手";
}

/*
 * printLine() - 打印分隔线
 * 用于美化输出格式
 */
void printLine(int length = 60)
{
    for (int i = 0; i < length; i++)
    {
        cout << "-";
    }
    cout << endl;
}

// ============================================================================
// 【比赛管理类】
// ============================================================================

/*
 * SpeechContest 类 - 演讲比赛管理系统
 *
 * 设计思路：
 * 将比赛的所有逻辑封装在一个类中，包括：
 * - 选手初始化
 * - 抽签分组
 * - 比赛进行
 * - 评分计算
 * - 结果显示和保存
 *
 * 数据存储方案：
 * - vector<Speaker>：存储每轮比赛的选手列表
 * - map<int, vector<Speaker>>：按轮次存储所有选手
 * - 多个容器配合使用，发挥各自优势
 */
class SpeechContest
{
public:
    // 构造函数
    SpeechContest()
    {
        // 设置随机数种子，确保每次运行结果不同
        srand(static_cast<unsigned int>(time(nullptr)));

        // 初始化12名选手
        initPlayers();
    }

    // ---- 显示菜单 ----
    void showMenu()
    {
        cout << "\n╔══════════════════════════════════════╗" << endl;
        cout << "║       演讲比赛管理系统 v1.0           ║" << endl;
        cout << "╠══════════════════════════════════════╣" << endl;
        cout << "║  1. 开始比赛                          ║" << endl;
        cout << "║  2. 查看往届记录                      ║" << endl;
        cout << "║  3. 清空记录                          ║" << endl;
        cout << "║  0. 退出系统                          ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
    }

    // ---- 主运行函数 ----
    void run()
    {
        int select = 0;

        while (true)
        {
            showMenu();
            cout << "\n请输入选择: ";
            cin >> select;

            switch (select)
            {
            case 1:
                startContest();
                break;
            case 2:
                showRecords();
                break;
            case 3:
                clearRecords();
                break;
            case 0:
                cout << "\n感谢使用，再见！" << endl;
                return;
            default:
                cout << "[提示] 无效选择，请重新输入。" << endl;
            }
        }
    }

private:
    // 本轮比赛的选手列表
    vector<Speaker> m_players;

    // 历史比赛记录：key=轮次, value=该轮选手列表
    map<int, vector<Speaker>> m_records;

    // ========================================================================
    // 【初始化选手】
    // ========================================================================

    /*
     * initPlayers() - 初始化12名选手
     *
     * 使用vector存储所有选手。
     * 选手编号从10001开始，便于区分。
     *
     * STL知识点：
     * vector的push_back()方法在末尾添加元素。
     * 如果vector容量不足，会自动扩容（通常翻倍）。
     * 这就是vector相比普通数组的优势之一。
     */
    void initPlayers()
    {
        m_players.clear();

        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            Speaker s(10001 + i, generateName(i));
            m_players.push_back(s);
        }

        cout << "[系统] 已初始化 " << m_players.size() << " 名选手。" << endl;
    }

    // ========================================================================
    // 【开始比赛】
    // ========================================================================

    /*
     * startContest() - 开始比赛的主流程
     *
     * 比赛流程：
     * 1. 第一轮：12名选手全部参加，抽签决定出场顺序
     * 2. 评委打分，计算得分，选出前6名晋级
     * 3. 第二轮：6名晋级选手参加，同样流程
     * 4. 产生最终名次
     *
     * 每一轮的流程相同，所以可以用循环来处理。
     * 这种"重复的流程用循环"的思路是编程的基本功。
     */
    void startContest()
    {
        cout << "\n========================================" << endl;
        cout << "          演讲比赛正式开始！" << endl;
        cout << "========================================" << endl;

        // 重新初始化选手（新一轮比赛）
        initPlayers();

        // 存储每轮晋级的选手
        vector<Speaker> currentRound = m_players;

        /*
         * 比赛主循环：共进行ROUND_COUNT轮
         * 每轮结束后，淘汰后半部分选手，前半部分晋级
         */
        for (int round = 1; round <= ROUND_COUNT; round++)
        {
            cout << "\n【第 " << round << " 轮比赛】" << endl;
            printLine();

            // 步骤1：抽签（打乱出场顺序）
            cout << "\n>> 抽签中..." << endl;
            drawLots(currentRound);

            // 步骤2：比赛（评委打分）
            cout << "\n>> 比赛进行中..." << endl;
            playRound(currentRound);

            // 步骤3：显示本轮成绩
            cout << "\n>> 第 " << round << " 轮比赛结果：" << endl;
            showRoundResult(currentRound, round);

            // 步骤4：保存本轮记录
            m_records[round] = currentRound;

            // 步骤5：如果不是最后一轮，选出晋级选手
            if (round < ROUND_COUNT)
            {
                currentRound = getAdvancers(currentRound);
                cout << "\n>> 晋级选手（前 " << ADVANCE_COUNT << " 名）：" << endl;
                showRoundResult(currentRound, round);

                // 保存晋级记录
                m_records[round + 10] = currentRound;  // 用11, 12表示晋级结果
            }
        }

        // 显示最终结果
        cout << "\n========================================" << endl;
        cout << "          比赛结束！最终排名：" << endl;
        cout << "========================================" << endl;
        showRoundResult(currentRound, ROUND_COUNT);

        // 保存最终结果到文件
        saveResult(currentRound);

        cout << "\n[系统] 比赛结果已保存到文件。" << endl;
    }

    // ========================================================================
    // 【抽签】
    // ========================================================================

    /*
     * drawLots() - 随机打乱选手出场顺序
     *
     * STL算法应用：
     * 使用 random_shuffle() 或 shuffle() 算法随机打乱vector中的元素。
     *
     * random_shuffle() 在C++14中已被弃用，推荐使用 shuffle()。
     * 但黑马课程中主要讲的是random_shuffle()，这里两种都展示。
     *
     * 原理：对vector中的元素进行随机交换，时间复杂度O(n)。
     * 这比手动实现随机排序要简洁高效得多。
     */
    void drawLots(vector<Speaker>& players)
    {
        /*
         * C++11及以后推荐使用 shuffle + default_random_engine
         * 但为了兼容黑马课程的讲解方式，这里使用random_shuffle
         *
         * random_shuffle(begin, end)：
         * 随机重排 [begin, end) 范围内的元素
         */
        random_shuffle(players.begin(), players.end());

        // 显示抽签后的出场顺序
        cout << "出场顺序：" << endl;
        for (size_t i = 0; i < players.size(); i++)
        {
            cout << "  第 " << (i + 1) << " 位: "
                 << players[i].name << " (编号: " << players[i].id << ")" << endl;
        }
    }

    // ========================================================================
    // 【比赛评分】
    // ========================================================================

    /*
     * playRound() - 进行一轮比赛（为所有选手打分）
     *
     * 评分流程：
     * 1. 遍历每位选手
     * 2. 10位评委分别为其打分（随机生成60-100分）
     * 3. 分数存入deque
     * 4. 排序后去掉最高分和最低分
     * 5. 计算剩余分数的平均值作为最终得分
     *
     * 为什么用deque存储分数？
     * deque（双端队列）在两端的操作（push/pop）是O(1)的，
     * 排序后可以直接pop_front()去掉最小值，pop_back()去掉最大值。
     * 虽然vector也能做到，但deque的语义更清晰。
     */
    void playRound(vector<Speaker>& players)
    {
        for (size_t i = 0; i < players.size(); i++)
        {
            // 清空上一轮的分数
            players[i].scores.clear();

            /*
             * 10位评委打分
             * 分数范围：60.0 - 100.0（模拟真实评分场景）
             * 使用rand()生成随机数，除以10得到一位小数
             *
             * 分数计算公式：
             * 基础分60 + 随机部分(0-40) = 60-100分
             * 保留一位小数，更符合实际评分场景
             */
            for (int j = 0; j < JUDGE_COUNT; j++)
            {
                // 生成60.0到100.0之间的随机分数（保留一位小数）
                double score = 60.0 + (rand() % 401) / 10.0;
                players[i].scores.push_back(score);
            }

            /*
             * 计算最终得分：去掉一个最高分和一个最低分，求平均
             *
             * 算法步骤：
             * 1. 对deque排序（默认升序）
             * 2. 去掉第一个（最低分）和最后一个（最高分）
             * 3. 计算剩余分数的总和
             * 4. 除以剩余分数个数得到平均分
             */
            calculateScore(players[i]);

            // 显示进度
            cout << "  选手 " << players[i].name << " 评分完成"
                 << "，最终得分: " << fixed << setprecision(1)
                 << players[i].finalScore << endl;
        }
    }

    /*
     * calculateScore() - 计算单个选手的最终得分
     *
     * 参数：speaker - 选手引用
     *
     * 计算逻辑：
     * 1. 将deque中的分数排序
     * 2. 去掉最高分和最低分
     * 3. 用accumulate()计算剩余分数的总和
     * 4. 除以剩余个数得到平均分
     *
     * STL算法应用：
     * - sort()：排序
     * - accumulate()：累加求和（来自<numeric>头文件）
     */
    void calculateScore(Speaker& speaker)
    {
        // 步骤1：对分数排序（升序）
        // 排序后，第一个元素是最小值，最后一个是最大值
        sort(speaker.scores.begin(), speaker.scores.end());

        // 步骤2：去掉一个最低分和一个最高分
        // pop_front()移除deque的第一个元素（最低分）
        // pop_back()移除deque的最后一个元素（最高分）
        speaker.scores.pop_front();  // 去掉最低分
        speaker.scores.pop_back();   // 去掉最高分

        /*
         * 步骤3：计算平均分
         *
         * accumulate() 函数（来自<numeric>）：
         * 参数：起始迭代器, 结束迭代器, 初始值
         * 返回：所有元素的累加和 + 初始值
         *
         * 这里初始值为0.0（注意要用double类型，否则整数除法会丢失精度）
         */
        double total = accumulate(
            speaker.scores.begin(),   // 起始位置
            speaker.scores.end(),     // 结束位置
            0.0                        // 初始值（double类型确保浮点运算）
        );

        // 计算平均分
        speaker.finalScore = total / speaker.scores.size();
    }

    // ========================================================================
    // 【显示结果】
    // ========================================================================

    /*
     * showRoundResult() - 显示一轮比赛的结果
     *
     * 参数：
     *   players - 选手列表
     *   round   - 轮次编号
     *
     * 显示前需要对选手按分数降序排序。
     * 使用sort() + 自定义比较函数（lambda表达式）。
     *
     * STL知识点：
     * sort(begin, end, comp)：
     * - begin, end：排序范围
     * - comp：比较函数，返回true表示第一个参数应排在前面
     *
     * lambda表达式：[](const Speaker& a, const Speaker& b) { return a.finalScore > b.finalScore; }
     * - []：捕获列表（不捕获外部变量）
     * - (参数列表)：两个const引用，避免拷贝开销
     * - {函数体}：比较逻辑，> 实现降序排列
     */
    void showRoundResult(vector<Speaker>& players, int round)
    {
        // 按分数降序排序
        sort(players.begin(), players.end(),
             [](const Speaker& a, const Speaker& b)
             {
                 return a.finalScore > b.finalScore;
             });

        // 表头
        printLine();
        cout << left << setw(8) << "排名"
             << setw(10) << "编号"
             << setw(10) << "姓名"
             << setw(12) << "最终得分" << endl;
        printLine();

        /*
         * 使用for_each算法遍历并显示选手信息
         *
         * for_each(begin, end, func)：
         * 对[begin, end)范围内的每个元素执行func函数
         *
         * 这里使用带索引的for循环更直观，
         * for_each更适合不需要索引的场景。
         * 两种方式都展示，供学习参考。
         */
        for (size_t i = 0; i < players.size(); i++)
        {
            cout << left << setw(8) << (i + 1)
                 << setw(10) << players[i].id
                 << setw(10) << players[i].name
                 << fixed << setprecision(1)
                 << setw(12) << players[i].finalScore << endl;
        }

        printLine();
    }

    // ========================================================================
    // 【晋级筛选】
    // ========================================================================

    /*
     * getAdvancers() - 获取晋级选手
     *
     * 参数：players - 本轮所有选手
     * 返回值：排名前ADVANCE_COUNT名的选手
     *
     * 算法：
     * 1. 按分数降序排序
     * 2. 取前ADVANCE_COUNT个元素
     *
     * vector的特性：
     * 支持随机访问，所以可以直接用下标访问前N个元素。
     * 也支持用迭代器范围构造新的vector。
     *
     * 这里使用了vector的迭代器范围构造函数：
     * vector<T>(begin, end) 会拷贝[begin, end)范围内的元素。
     */
    vector<Speaker> getAdvancers(vector<Speaker>& players)
    {
        // 先按分数降序排序
        sort(players.begin(), players.end(),
             [](const Speaker& a, const Speaker& b)
             {
                 return a.finalScore > b.finalScore;
             });

        // 取前N名晋级
        // 确保不会越界
        int count = min(ADVANCE_COUNT, static_cast<int>(players.size()));

        /*
         * 使用迭代器范围构造新vector
         * players.begin() 到 players.begin() + count
         * 表示取前count个元素
         */
        vector<Speaker> advancers(players.begin(), players.begin() + count);

        return advancers;
    }

    // ========================================================================
    // 【保存结果到文件】
    // ========================================================================

    /*
     * saveResult() - 保存比赛结果到文件
     *
     * 文件格式：
     * - 第一行：比赛日期和时间
     * - 后续行：排名、编号、姓名、分数
     *
     * 文件操作使用ofstream（输出文件流）。
     * 文件不存在时会自动创建，已存在时会覆盖。
     */
    void saveResult(vector<Speaker>& finalPlayers)
    {
        ofstream ofs("speech_contest_result.txt", ios::out);

        if (!ofs.is_open())
        {
            cout << "[错误] 无法创建结果文件！" << endl;
            return;
        }

        // 写入文件头
        ofs << "========================================" << endl;
        ofs << "       演讲比赛最终结果" << endl;
        ofs << "========================================" << endl;
        ofs << endl;

        // 按分数降序排序
        sort(finalPlayers.begin(), finalPlayers.end(),
             [](const Speaker& a, const Speaker& b)
             {
                 return a.finalScore > b.finalScore;
             });

        // 写入排名信息
        for (size_t i = 0; i < finalPlayers.size(); i++)
        {
            ofs << "第" << (i + 1) << "名: "
                << finalPlayers[i].name
                << " (编号: " << finalPlayers[i].id << ")"
                << " 得分: " << fixed << setprecision(1)
                << finalPlayers[i].finalScore << endl;
        }

        ofs << endl;
        ofs << "========================================" << endl;

        ofs.close();
        cout << "[系统] 结果已保存到 speech_contest_result.txt" << endl;
    }

    // ========================================================================
    // 【查看历史记录】
    // ========================================================================

    /*
     * showRecords() - 显示历史比赛记录
     *
     * 使用map的特性：
     * map会自动按key排序，所以遍历时会按轮次顺序显示。
     *
     * map遍历方式：
     * 使用范围for循环（C++11新特性）：
     * for (auto& pair : m_map) { ... }
     * pair.first 是key，pair.second 是value
     */
    void showRecords()
    {
        if (m_records.empty())
        {
            cout << "\n[提示] 暂无比赛记录。" << endl;
            return;
        }

        cout << "\n--- 历史比赛记录 ---" << endl;

        /*
         * 使用范围for遍历map
         * auto自动推导类型，pair的类型是 pair<const int, vector<Speaker>>
         *
         * map的遍历顺序是按key升序排列的（因为map底层是红黑树）
         * 所以我们会按轮次顺序看到记录
         */
        for (const auto& record : m_records)
        {
            int round = record.first;
            const vector<Speaker>& players = record.second;

            if (round <= ROUND_COUNT)
            {
                cout << "\n第 " << round << " 轮：" << endl;
            }
            else
            {
                cout << "\n第 " << (round - 10) << " 轮晋级名单：" << endl;
            }

            // 显示该轮选手信息
            for (const auto& player : players)
            {
                cout << "  " << player.name
                     << " (编号: " << player.id << ")"
                     << " 得分: " << fixed << setprecision(1)
                     << player.finalScore << endl;
            }
        }
    }

    // ========================================================================
    // 【清空记录】
    // ========================================================================

    /*
     * clearRecords() - 清空历史记录
     */
    void clearRecords()
    {
        if (m_records.empty())
        {
            cout << "\n[提示] 暂无记录可清空。" << endl;
            return;
        }

        string confirm;
        cout << "\n确认清空所有历史记录？(y/n): ";
        cin >> confirm;

        if (confirm == "y" || confirm == "Y")
        {
            m_records.clear();
            cout << "[成功] 历史记录已清空。" << endl;
        }
        else
        {
            cout << "[提示] 已取消操作。" << endl;
        }
    }
};

// ============================================================================
// 【主函数】
// ============================================================================

int main()
{
    SpeechContest contest;

    cout << "\n========================================" << endl;
    cout << "    欢迎使用演讲比赛管理系统" << endl;
    cout << "========================================" << endl;

    contest.run();

    return 0;
}

/*
 * ============================================================================
 * 【STL知识点总结】
 * ============================================================================
 *
 * 1. vector（向量容器）：
 *    - 连续存储，支持随机访问（O(1)）
 *    - 尾部插入/删除高效（均摊O(1)）
 *    - 中间插入/删除需要移动元素（O(n)）
 *    - 适用于：需要频繁随机访问的场景
 *
 * 2. deque（双端队列）：
 *    - 两端插入/删除高效（O(1)）
 *    - 支持随机访问（O(1)，但比vector稍慢）
 *    - 适用于：需要在两端操作的场景（如本项目的评分去极值）
 *
 * 3. map（映射容器）：
 *    - 基于红黑树实现，自动按键排序
 *    - 查找、插入、删除都是O(log n)
 *    - 适用于：需要按键快速查找的场景
 *
 * 4. 常用算法：
 *    - sort()：排序，时间复杂度O(n log n)
 *    - for_each()：遍历执行操作
 *    - accumulate()：累加求和
 *    - random_shuffle()：随机打乱（C++14弃用，推荐shuffle）
 *
 * 5. Lambda表达式（C++11新特性）：
 *    - 语法：[捕获列表](参数列表){函数体}
 *    - 常用于sort()等算法的自定义比较函数
 *    - 可以捕获外部变量（=值捕获, &引用捕获）
 * ============================================================================
 */
