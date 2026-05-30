/*
 * ============================================================================
 * 黑马程序员 C++课程 - STL实践：评委打分系统
 * ============================================================================
 *
 * 【项目背景】
 * 评委打分系统是STL部分的入门级实战项目，帮助学员掌握STL容器和算法
 * 的基本使用。相比演讲比赛项目，本项目更加聚焦于STL的核心用法。
 *
 * 【功能需求】
 * - N名选手参加比赛
 * - 每位选手由M位评委打分
 * - 去掉一个最高分和一个最低分
 * - 计算剩余分数的平均分作为最终得分
 * - 按最终得分排序，显示排名
 *
 * 【STL知识点覆盖】
 * 1. deque容器：双端队列，存储评委打分
 * 2. vector容器：存储选手信息
 * 3. sort算法：对分数和选手排序
 * 4. for_each算法：遍历容器并执行操作
 * 5. transform算法：对容器元素进行变换
 * 6. accumulate算法：数值累加
 * 7. max_element/min_element：查找极值
 * 8. Lambda表达式：自定义操作和比较规则
 *
 * 【项目重点】
 * 本项目重点练习以下STL技能：
 * - deque的使用（push_back, pop_front, pop_back）
 * - sort()的自定义排序（lambda表达式）
 * - for_each()的使用（输出和处理）
 * - transform()的使用（数据变换）
 * - Lambda表达式的各种用法（值捕获、引用捕获）
 *
 * 编译方法：g++ 96_STL实践_评委打分.cpp -o judge_scoring
 * ============================================================================
 */

#include <iostream>      // 标准输入输出
#include <string>        // 字符串类
#include <vector>        // vector容器
#include <deque>         // deque容器（双端队列）
#include <algorithm>     // STL算法：sort, for_each, transform, max_element, min_element
#include <numeric>       // 数值算法：accumulate
#include <functional>    // 函数对象
#include <iomanip>       // 格式化输出
#include <ctime>         // 时间函数
#include <cstdlib>       // 随机数函数

using namespace std;

// ============================================================================
// 【常量定义】
// ============================================================================

const int PLAYER_COUNT = 5;   // 选手数量（可修改）
const int JUDGE_COUNT  = 7;   // 评委数量（可修改）

// ============================================================================
// 【数据结构】
// ============================================================================

/*
 * 选手结构体
 *
 * 使用deque存储分数的原因详解：
 *
 * deque（Double-Ended Queue，双端队列）是STL提供的容器之一。
 * 它的特点是：
 * 1. 两端插入/删除都是O(1)的时间复杂度
 * 2. 支持随机访问（可以通过下标访问任意元素）
 * 3. 内存不是完全连续的（分段存储），但对外表现得像连续的
 *
 * 与vector的对比：
 * - vector在尾部操作是O(1)，但在头部操作是O(n)
 * - deque在两端操作都是O(1)
 * - vector的随机访问略快于deque（内存连续性更好）
 *
 * 在本项目中，排序后需要pop_front()去掉最小值，pop_back()去掉最大值，
 * deque的双端操作特性非常适合这个场景。
 */
struct Player
{
    int id;                   // 选手编号
    string name;              // 选手姓名
    deque<double> scores;     // 评委打分（使用deque存储）
    double finalScore;        // 最终得分
    int rank;                 // 排名

    // 默认构造函数
    Player() : id(0), name(""), finalScore(0.0), rank(0) {}

    // 带参构造函数
    Player(int id, string name) : id(id), name(name), finalScore(0.0), rank(0) {}
};

// ============================================================================
// 【辅助函数：生成随机分数】
// ============================================================================

/*
 * generateScore() - 生成一个随机评分
 *
 * 返回值：60.0 - 100.0 之间的随机分数（保留一位小数）
 *
 * 实现细节：
 * rand() % 401 生成 0-400 的整数
 * 除以 10.0 得到 0.0-40.0 的浮点数
 * 加上 60.0 得到 60.0-100.0 的范围
 *
 * 注意：这只是一种简单的随机数生成方式。
 * C++11推荐使用 <random> 头文件中的随机数生成器，
 * 如 mt19937 + uniform_real_distribution，更均匀、更随机。
 */
double generateScore()
{
    // rand() % 401 产生 0 到 400 的整数
    // 除以 10.0 得到 0.0 到 40.0
    // 加上 60.0 得到 60.0 到 100.0
    return 60.0 + (rand() % 401) / 10.0;
}

// ============================================================================
// 【核心功能函数】
// ============================================================================

/*
 * initPlayers() - 初始化选手列表
 *
 * 参数：无
 * 返回值：vector<Player> 选手列表
 *
 * STL知识点：
 * - vector的push_back()方法
 * - vector的构造和初始化
 *
 * 设计说明：使用工厂函数模式，返回初始化好的选手列表。
 * 这种模式让main()函数更简洁，也便于测试。
 */
vector<Player> initPlayers()
{
    vector<Player> players;

    // 预设选手姓名
    string names[] = {"张伟", "王芳", "李明", "赵丽", "刘强",
                      "陈静", "杨勇", "黄敏", "周杰", "吴婷"};

    int count = min(PLAYER_COUNT, 10);  // 确保不超过预设姓名数量

    for (int i = 0; i < count; i++)
    {
        /*
         * 使用emplace_back()或push_back()添加元素
         *
         * emplace_back(args...)：直接在vector末尾构造对象
         * push_back(obj)：将已有对象拷贝/移动到vector末尾
         *
         * emplace_back通常更高效（避免拷贝），但push_back更直观。
         * 这里使用push_back便于初学者理解。
         */
        Player p(i + 1, names[i]);
        players.push_back(p);
    }

    return players;
}

/*
 * judgeScoring() - 为所有选手打分
 *
 * 参数：players - 选手列表（引用传递，直接修改原数据）
 * 返回值：无
 *
 * 功能：
 * 遍历每位选手，为每位选手生成JUDGE_COUNT个随机分数。
 * 分数存储在选手的scores（deque）中。
 *
 * STL知识点：
 * - deque的push_back()方法：在末尾添加元素
 * - 范围for循环遍历vector
 */
void judgeScoring(vector<Player>& players)
{
    cout << "\n--- 评委打分 ---" << endl;

    /*
     * 使用范围for循环遍历选手列表
     *
     * 语法：for (类型 变量名 : 容器) { ... }
     * - 类型：容器元素的类型
     * - 变量名：当前元素的副本（或引用&表示直接操作原数据）
     * - 容器：要遍历的容器
     *
     * 使用引用（&）可以避免拷贝开销，也可以直接修改原数据。
     * 如果只需要读取，可以用 const auto& 防止意外修改。
     */
    for (auto& player : players)
    {
        // 清空之前的分数（如果有）
        player.scores.clear();

        // 生成评委分数
        for (int j = 0; j < JUDGE_COUNT; j++)
        {
            double score = generateScore();
            player.scores.push_back(score);  // deque的push_back：O(1)
        }

        /*
         * 使用for_each算法输出每位评委的分数
         *
         * for_each(begin, end, func)：
         * 对[begin, end)范围内的每个元素执行func
         *
         * 这里的lambda表达式：
         * [&cout]：以引用方式捕获cout（因为cout不能拷贝）
         * (double s)：参数是当前元素
         * { cout << ... << " "; }：输出操作
         *
         * 注意：在C++中，lambda捕获cout需要显式写[&cout]，
         * 因为cout是全局对象，不能被隐式捕获。
         */
        cout << "选手 " << player.name << " 的评委打分: ";
        for_each(player.scores.begin(), player.scores.end(),
                 [](double s)
                 {
                     cout << fixed << setprecision(1) << s << " ";
                 });
        cout << endl;
    }
}

/*
 * calculateScores() - 计算所有选手的最终得分
 *
 * 参数：players - 选手列表（引用传递）
 * 返回值：无
 *
 * 计算规则：
 * 1. 对每位选手的分数排序
 * 2. 去掉一个最高分和一个最低分
 * 3. 计算剩余分数的平均值
 *
 * STL知识点：
 * - sort()：对deque进行排序
 * - accumulate()：累加求和
 * - deque的pop_front()和pop_back()
 */
void calculateScores(vector<Player>& players)
{
    cout << "\n--- 计算最终得分 ---" << endl;

    for (auto& player : players)
    {
        /*
         * 步骤1：对分数排序
         *
         * sort(begin, end)：默认升序排列
         * 排序后，scores[0]是最小值，scores[size-1]是最大值
         *
         * deque支持随机访问迭代器，所以可以使用sort()。
         * 注意：list容器不支持随机访问，不能用sort()，
         * 但list有自己的sort()成员函数。
         */
        sort(player.scores.begin(), player.scores.end());

        // 记录最低分和最高分（用于显示）
        double lowest = player.scores.front();   // 第一个元素（最小值）
        double highest = player.scores.back();   // 最后一个元素（最大值）

        /*
         * 步骤2：去掉一个最低分和一个最高分
         *
         * deque的两端操作：
         * - pop_front()：移除第一个元素，O(1)
         * - pop_back()：移除最后一个元素，O(1)
         *
         * 这就是为什么我们选择deque而不是vector：
         * vector的pop_front()不是O(1)的，需要移动所有后续元素。
         */
        player.scores.pop_front();  // 去掉最低分
        player.scores.pop_back();   // 去掉最高分

        /*
         * 步骤3：计算平均分
         *
         * accumulate(begin, end, init) 函数：
         * - 来自 <numeric> 头文件
         * - 计算 [begin, end) 范围内所有元素的累加和
         * - init 是初始值（通常为0或0.0）
         *
         * 注意：初始值的类型决定了累加的类型。
         * 如果用0（int），结果会是整数除法，丢失小数部分。
         * 用0.0（double），结果才是浮点数。
         */
        double total = accumulate(
            player.scores.begin(),
            player.scores.end(),
            0.0  // 初始值，使用double类型确保浮点运算
        );

        player.finalScore = total / player.scores.size();

        // 显示计算结果
        cout << "选手 " << player.name
             << ": 去掉最低分 " << fixed << setprecision(1) << lowest
             << "，去掉最高分 " << highest
             << "，最终得分 " << player.finalScore << endl;
    }
}

/*
 * rankPlayers() - 对选手进行排名
 *
 * 参数：players - 选手列表（引用传递）
 * 返回值：无
 *
 * STL知识点：
 * - sort() + lambda表达式：自定义排序规则
 * - transform()算法：对容器元素进行变换
 *
 * 排序规则：按最终得分降序排列（得分高的排在前面）
 */
void rankPlayers(vector<Player>& players)
{
    /*
     * 使用sort() + lambda表达式进行降序排序
     *
     * Lambda表达式详解：
     * [捕获列表](参数列表) -> 返回类型 { 函数体 }
     *
     * 各部分说明：
     * - []：捕获列表，指定lambda可以访问的外部变量
     *   - []：不捕获任何外部变量
     *   - [=]：以值方式捕获所有外部变量
     *   - [&]：以引用方式捕获所有外部变量
     *   - [x]：以值方式捕获变量x
     *   - [&x]：以引用方式捕获变量x
     *   - [=, &x]：以值方式捕获所有变量，但x以引用方式捕获
     *
     * - (参数列表)：函数参数，与普通函数相同
     *   - 这里是两个const Player&，避免拷贝开销
     *   - const表示不会修改参数
     *
     * - -> 返回类型：可省略，编译器会自动推导
     *
     * - { 函数体 }：函数实现
     *   - 这里是比较逻辑：a的分数 > b的分数 时返回true
     *   - 意味着a排在b前面（降序）
     *
     * 注意：> 实现降序，< 实现升序。
     */
    sort(players.begin(), players.end(),
         [](const Player& a, const Player& b)
         {
             return a.finalScore > b.finalScore;
         });

    /*
     * 使用transform算法设置排名
     *
     * transform(begin, end, begin_out, func)：
     * 对[begin, end)范围内的每个元素执行func，
     * 将结果存储到begin_out开始的位置。
     *
     * 这里是就地变换（输入和输出是同一个容器），
     * 为每个选手设置排名（1, 2, 3, ...）。
     *
     * 注意：transform的输出不能与输入重叠（除非是同一位置）。
     * 这里players既是输入又是输出，但每个位置只读写一次，是安全的。
     */
    int rankCounter = 0;
    transform(players.begin(), players.end(), players.begin(),
              [&rankCounter](Player& p)
              {
                  rankCounter++;
                  p.rank = rankCounter;
                  return p;
              });
}

/*
 * displayRankings() - 显示最终排名
 *
 * 参数：players - 选手列表（const引用，只读）
 * 返回值：无
 *
 * STL知识点：
 * - for_each()算法：遍历并执行操作
 * - const_iterator：只读迭代器
 * - 格式化输出：setw, left, fixed, setprecision
 *
 * 使用const引用是因为此函数只负责显示，不修改数据。
 * 这是良好的编程习惯：明确表达函数的意图。
 */
void displayRankings(const vector<Player>& players)
{
    cout << "\n========================================" << endl;
    cout << "          评委打分最终排名" << endl;
    cout << "========================================" << endl;

    // 表头
    cout << left
         << setw(6) << "排名"
         << setw(8) << "编号"
         << setw(10) << "姓名"
         << setw(14) << "最终得分" << endl;
    cout << "----------------------------------------" << endl;

    /*
     * 使用for_each算法显示所有选手
     *
     * for_each(begin, end, func)：
     * 对范围内每个元素执行func
     *
     * 这里的lambda以值方式捕获局部变量&cout：
     * [&cout] 以引用方式捕获cout对象
     * （cout不能被拷贝，必须用引用）
     *
     * 使用for_each而不是普通for循环，是为了展示STL算法的用法。
     * 实际开发中，普通for循环可能更直观。
     */
    for_each(players.begin(), players.end(),
             [](const Player& p)
             {
                 cout << left
                      << setw(6) << p.rank
                      << setw(8) << p.id
                      << setw(10) << p.name
                      << fixed << setprecision(1)
                      << setw(14) << p.finalScore << endl;
             });

    cout << "----------------------------------------" << endl;

    /*
     * 使用算法计算并显示统计信息
     *
     * max_element(begin, end)：返回指向最大元素的迭代器
     * min_element(begin, end)：返回指向最小元素的迭代器
     *
     * 注意：这些算法返回的是迭代器，不是值。
     * 需要用 * 运算符解引用才能得到值。
     *
     * 这里使用lambda自定义比较规则（按finalScore比较）。
     */
    auto maxIt = max_element(players.begin(), players.end(),
                             [](const Player& a, const Player& b)
                             {
                                 return a.finalScore < b.finalScore;
                             });

    auto minIt = min_element(players.begin(), players.end(),
                             [](const Player& a, const Player& b)
                             {
                                 return a.finalScore < b.finalScore;
                             });

    /*
     * 使用accumulate计算总分和平均分
     *
     * 注意：accumulate需要自定义操作来提取Player中的finalScore。
     * 默认的accumulate使用+运算符，但Player之间不能直接相加。
     * 所以我们需要提供一个自定义的二元操作函数。
     *
     * 这里使用lambda作为自定义操作：
     * [](double sum, const Player& p) { return sum + p.finalScore; }
     * 表示：累加器 = 当前总和 + 当前选手的分数
     */
    double totalScore = accumulate(
        players.begin(), players.end(),
        0.0,
        [](double sum, const Player& p)
        {
            return sum + p.finalScore;
        });

    double avgScore = totalScore / players.size();

    cout << "\n统计信息：" << endl;
    cout << "  最高分: " << maxIt->name << " (" << maxIt->finalScore << " 分)" << endl;
    cout << "  最低分: " << minIt->name << " (" << minIt->finalScore << " 分)" << endl;
    cout << "  平均分: " << fixed << setprecision(1) << avgScore << " 分" << endl;
    cout << "  总分:   " << totalScore << " 分" << endl;
}

// ============================================================================
// 【演示Lambda表达式的各种用法】
// ============================================================================

/*
 * demonstrateLambda() - 演示Lambda表达式的各种用法
 *
 * Lambda表达式是C++11引入的重要特性，在STL算法中广泛使用。
 * 本函数通过实际例子展示Lambda的各种用法。
 */
void demonstrateLambda()
{
    cout << "\n========================================" << endl;
    cout << "      Lambda表达式用法演示" << endl;
    cout << "========================================" << endl;

    // ---- 示例1：基本Lambda ----
    cout << "\n--- 示例1：基本Lambda ---" << endl;

    /*
     * 最简单的Lambda：无参数，无捕获
     * 语法：[]{ 函数体 }
     * 可以直接调用：lambda()
     */
    auto sayHello = []()
    {
        cout << "Hello, Lambda!" << endl;
    };
    sayHello();  // 调用Lambda

    // ---- 示例2：带参数的Lambda ----
    cout << "\n--- 示例2：带参数的Lambda ---" << endl;

    /*
     * 带参数的Lambda
     * 语法：[](参数列表){ 函数体 }
     * 参数类型与普通函数相同
     */
    auto add = [](int a, int b) -> int
    {
        return a + b;
    };
    cout << "3 + 5 = " << add(3, 5) << endl;

    /*
     * 返回类型可以省略，编译器自动推导
     * 但如果逻辑复杂，显式指定返回类型更安全
     */
    auto multiply = [](double a, double b)
    {
        return a * b;
    };
    cout << "2.5 * 4.0 = " << multiply(2.5, 4.0) << endl;

    // ---- 示例3：值捕获 ----
    cout << "\n--- 示例3：值捕获 [=] ---" << endl;

    /*
     * 值捕获：Lambda内部获得外部变量的副本
     * [=]：以值方式捕获所有外部变量
     * [x]：只捕获变量x的值
     *
     * 特点：
     * - Lambda内部修改的是副本，不影响外部变量
     * - Lambda内部的变量是const的（除非声明mutable）
     */
    int base = 100;
    auto addBase = [=](int x)
    {
        // base是副本，不能修改
        return base + x;
    };
    cout << "base=100, addBase(50) = " << addBase(50) << endl;
    cout << "base仍然是: " << base << endl;  // base没有被修改

    // ---- 示例4：引用捕获 ----
    cout << "\n--- 示例4：引用捕获 [&] ---" << endl;

    /*
     * 引用捕获：Lambda内部直接操作外部变量
     * [&]：以引用方式捕获所有外部变量
     * [&x]：只捕获变量x的引用
     *
     * 特点：
     * - Lambda内部修改会直接影响外部变量
     * - 必须确保Lambda执行时，被捕获的变量仍然有效
     */
    int counter = 0;
    auto increment = [&counter]()
    {
        counter++;  // 直接修改外部变量
    };
    increment();
    increment();
    increment();
    cout << "counter经过3次increment后: " << counter << endl;  // 输出3

    // ---- 示例5：混合捕获 ----
    cout << "\n--- 示例5：混合捕获 [=, &x] ---" << endl;

    /*
     * 混合捕获：部分值捕获，部分引用捕获
     * [=, &counter]：所有变量值捕获，但counter用引用捕获
     *
     * 这在需要读取大部分变量、但只修改少数变量时很有用
     */
    int a = 10, b = 20;
    int result = 0;
    auto calc = [=, &result](int multiplier)
    {
        // a和b是值捕获（只读）
        // result是引用捕获（可修改）
        result = (a + b) * multiplier;
    };
    calc(3);
    cout << "a=10, b=20, calc(3) => result = " << result << endl;

    // ---- 示例6：mutable Lambda ----
    cout << "\n--- 示例6：mutable Lambda ---" << endl;

    /*
     * mutable Lambda：允许修改值捕获的变量（只影响副本）
     *
     * 默认情况下，值捕获的变量在Lambda内部是const的。
     * 加上mutable关键字后，可以修改这些副本。
     * 但修改只在Lambda内部有效，不影响外部变量。
     */
    int value = 42;
    auto modifyValue = [value]() mutable
    {
        value += 10;  // 修改的是副本
        cout << "Lambda内部value: " << value << endl;
    };
    modifyValue();
    cout << "Lambda外部value: " << value << endl;  // 仍然是42

    // ---- 示例7：在STL算法中使用Lambda ----
    cout << "\n--- 示例7：Lambda在STL算法中的应用 ---" << endl;

    vector<int> numbers = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // 使用sort + Lambda进行降序排序
    sort(numbers.begin(), numbers.end(),
         [](int a, int b) { return a > b; });

    cout << "降序排序结果: ";
    for_each(numbers.begin(), numbers.end(),
             [](int n) { cout << n << " "; });
    cout << endl;

    // 使用count_if统计满足条件的元素个数
    int evenCount = count_if(numbers.begin(), numbers.end(),
                             [](int n) { return n % 2 == 0; });
    cout << "偶数个数: " << evenCount << endl;

    // 使用transform将所有元素翻倍
    vector<int> doubled(numbers.size());
    transform(numbers.begin(), numbers.end(), doubled.begin(),
              [](int n) { return n * 2; });

    cout << "翻倍后: ";
    for_each(doubled.begin(), doubled.end(),
             [](int n) { cout << n << " "; });
    cout << endl;

    // 使用remove_if + erase 删除满足条件的元素（erase-remove惯用法）
    // 删除所有小于5的元素
    numbers.erase(
        remove_if(numbers.begin(), numbers.end(),
                  [](int n) { return n < 5; }),
        numbers.end()
    );

    cout << "删除小于5的元素后: ";
    for_each(numbers.begin(), numbers.end(),
             [](int n) { cout << n << " "; });
    cout << endl;
}

// ============================================================================
// 【主函数】
// ============================================================================

int main()
{
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "\n========================================" << endl;
    cout << "      欢迎使用评委打分系统" << endl;
    cout << "========================================" << endl;
    cout << "选手人数: " << PLAYER_COUNT << endl;
    cout << "评委人数: " << JUDGE_COUNT << endl;

    // 步骤1：初始化选手
    cout << "\n--- 步骤1：初始化选手 ---" << endl;
    vector<Player> players = initPlayers();

    // 使用for_each显示所有选手信息
    cout << "已初始化 " << players.size() << " 名选手：" << endl;
    for_each(players.begin(), players.end(),
             [](const Player& p)
             {
                 cout << "  " << p.id << ". " << p.name << endl;
             });

    // 步骤2：评委打分
    judgeScoring(players);

    // 步骤3：计算最终得分（去极值求平均）
    calculateScores(players);

    // 步骤4：排名
    rankPlayers(players);

    // 步骤5：显示最终排名
    displayRankings(players);

    // 步骤6：演示Lambda表达式的用法
    cout << "\n是否查看Lambda表达式用法演示？(y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
        demonstrateLambda();
    }

    cout << "\n========================================" << endl;
    cout << "      感谢使用评委打分系统！" << endl;
    cout << "========================================" << endl;

    return 0;
}

/*
 * ============================================================================
 * 【STL容器和算法总结】
 * ============================================================================
 *
 * 【deque（双端队列）要点】
 * 1. 底层实现：分段连续的内存块
 * 2. 两端操作O(1)，中间操作O(n)
 * 3. 支持随机访问（通过下标）
 * 4. 常用方法：
 *    - push_back() / push_front()：两端插入
 *    - pop_back() / pop_front()：两端删除
 *    - front() / back()：访问首尾元素
 *    - size()：元素个数
 *    - empty()：是否为空
 *
 * 【常用STL算法要点】
 *
 * 1. sort(begin, end, comp)
 *    - 功能：排序
 *    - 时间复杂度：O(n log n)
 *    - comp可选，默认升序，可传lambda自定义
 *
 * 2. for_each(begin, end, func)
 *    - 功能：遍历执行操作
 *    - 时间复杂度：O(n)
 *    - func可以是函数、函数对象或lambda
 *
 * 3. transform(begin, end, begin_out, func)
 *    - 功能：对每个元素执行变换
 *    - 时间复杂度：O(n)
 *    - 输出可以是同一容器或不同容器
 *
 * 4. accumulate(begin, end, init, op)
 *    - 功能：累加/自定义聚合
 *    - 时间复杂度：O(n)
 *    - op可选，默认用+运算符
 *
 * 5. max_element / min_element(begin, end, comp)
 *    - 功能：查找最大/最小元素
 *    - 返回迭代器，需要*解引用
 *    - 时间复杂度：O(n)
 *
 * 6. count_if(begin, end, pred)
 *    - 功能：统计满足条件的元素个数
 *    - pred是谓词（返回bool的函数/lambda）
 *
 * 7. remove_if(begin, end, pred) + erase
 *    - 功能：删除满足条件的元素
 *    - 注意：remove_if不真正删除元素，只是移动元素
 *    - 必须配合erase才能真正删除（erase-remove惯用法）
 *
 * 【Lambda表达式要点】
 * 1. 语法：[捕获](参数) -> 返回类型 { 函数体 }
 * 2. 捕获方式：[] [=] [&] [x] [&x] [=, &x]
 * 3. mutable：允许修改值捕获的变量
 * 4. 常用于STL算法的自定义操作参数
 * 5. 可以赋值给auto变量，实现延迟调用
 *
 * 【最佳实践】
 * 1. 优先使用STL算法而非手写循环（更简洁、不易出错）
 * 2. Lambda参数尽量用const引用（避免拷贝）
 * 3. 捕获时明确列出需要的变量（不要用[=]或[&]全部捕获）
 * 4. 简单操作用lambda，复杂逻辑用命名函数
 * ============================================================================
 */
