/*
 * 87_文件读操作详解.cpp
 * 本文件演示 C++ 文件读操作的详细用法
 * 包括: ifstream 的打开、读取、关闭，>> 运算符，getline()，
 *       get(), peek(), eof()，读取整个文件，错误处理
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>     // stringstream
using namespace std;

int main() {
    // 先创建一个测试文件用于读取演示
    {
        ofstream ofs("test_read.txt");
        ofs << "Hello World" << endl;
        ofs << "这是第二行" << endl;
        ofs << "123 456 789" << endl;
        ofs << "Alice 85.5" << endl;
        ofs << "Bob 92.0" << endl;
        ofs << "Charlie 78.8" << endl;
        ofs << "最后一行没有换行";
        ofs.close();
    }
    cout << "测试文件已创建" << endl;

    cout << "\n========== 1. ifstream 基本读取 ==========" << endl;
    // ifstream: 输入文件流（Input File Stream）
    // 用法与 cin 完全相同
    {
        ifstream ifs("test_read.txt");

        // 检查文件是否成功打开
        if (!ifs.is_open()) {
            cerr << "无法打开文件!" << endl;
            return 1;
        }

        // 使用 >> 运算符读取（以空格/换行分隔）
        string word1, word2;
        ifs >> word1 >> word2;
        cout << "前两个单词: [" << word1 << "] [" << word2 << "]" << endl;

        // 读取整数
        int a, b, c;
        ifs >> a >> b >> c;
        cout << "三个数字: " << a << " " << b << " " << c << endl;

        ifs.close();
    }

    cout << "\n========== 2. getline 逐行读取 ==========" << endl;
    // getline: 读取一整行（包括空格），直到遇到换行符
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) {
            cerr << "无法打开文件!" << endl;
            return 1;
        }

        string line;
        int lineNum = 1;

        // 逐行读取整个文件
        while (getline(ifs, line)) {
            cout << "第" << lineNum++ << "行: " << line << endl;
        }

        ifs.close();
    }

    cout << "\n========== 3. 指定分隔符的 getline ==========" << endl;
    // getline 默认以 '\n' 为分隔符，可以指定其他分隔符
    {
        // 创建一个用逗号分隔的测试文件
        ofstream ofs("test_csv.txt");
        ofs << "name,score,grade" << endl;
        ofs << "Alice,85,B" << endl;
        ofs << "Bob,92,A" << endl;
        ofs << "Charlie,78,C" << endl;
        ofs.close();

        ifstream ifs("test_csv.txt");
        string line;

        while (getline(ifs, line)) {
            // 使用 stringstream 解析逗号分隔的数据
            stringstream ss(line);
            string field;
            cout << "行数据: ";
            while (getline(ss, field, ',')) {  // 以逗号为分隔符
                cout << "[" << field << "] ";
            }
            cout << endl;
        }
        ifs.close();
    }

    cout << "\n========== 4. get() 读取单个字符 ==========" << endl;
    // get(): 读取单个字符（包括空格和换行符）
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        // 逐字符读取
        char ch;
        int charCount = 0;
        cout << "前20个字符: ";
        while (ifs.get(ch) && charCount < 20) {
            cout << ch;
            charCount++;
        }
        cout << endl;

        // 回到文件开头
        ifs.clear();  // 清除 EOF 标志（重要！）
        ifs.seekg(0, ios::beg);

        // 使用 get() 读取到指定分隔符
        char buffer[100];
        ifs.get(buffer, 100, '\n');  // 读取到换行符或最多100字符
        cout << "第一行(get): " << buffer << endl;

        // 跳过换行符
        ifs.get();  // 读取并丢弃换行符

        // 再读一行
        ifs.get(buffer, 100, '\n');
        cout << "第二行(get): " << buffer << endl;

        ifs.close();
    }

    cout << "\n========== 5. peek() 预览下一个字符 ==========" << endl;
    // peek(): 查看下一个字符但不读取（不移动读取位置）
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        // 使用 peek 判断下一个字符的类型
        char ch;
        while ((ch = ifs.peek()) != EOF) {
            if (isdigit(ch)) {
                int num;
                ifs >> num;
                cout << "读取数字: " << num << endl;
            } else if (isalpha(ch)) {
                string word;
                ifs >> word;
                cout << "读取单词: " << word << endl;
            } else if (ch == '\n') {
                ifs.get();  // 跳过换行
            } else if (ch == ' ') {
                ifs.get();  // 跳过空格
            } else {
                cout << "其他字符: " << (char)ifs.get() << endl;
            }
        }

        ifs.close();
    }

    cout << "\n========== 6. eof() 判断文件结束 ==========" << endl;
    // eof(): 判断是否到达文件末尾
    // 注意: eof() 只有在读取失败后才会返回 true
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        string word;
        int count = 0;

        // 正确的 eof 用法: 先读取，再检查
        while (ifs >> word) {
            count++;
        }
        cout << "文件中共有 " << count << " 个单词" << endl;

        // 错误的 eof 用法（常见错误）:
        // while (!ifs.eof()) { ifs >> word; cout << word; }
        // 这会导致最后一个单词输出两次！
        // 因为 eof() 只有在读取失败后才返回 true

        ifs.close();
    }

    cout << "\n========== 7. 读取整个文件到字符串 ==========" << endl;
    // 方法一: 使用迭代器
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        // 使用 istreambuf_iterator 读取整个文件
        string content((istreambuf_iterator<char>(ifs)),
                        istreambuf_iterator<char>());
        cout << "方法一(迭代器) - 文件内容:" << endl;
        cout << content << endl;
        cout << "--- 文件结束 ---" << endl;

        ifs.close();
    }

    // 方法二: 使用 rdbuf
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        // rdbuf() 返回流缓冲区的指针
        stringstream ss;
        ss << ifs.rdbuf();
        string content = ss.str();
        cout << "方法二(rdbuf) - 文件大小: " << content.size() << " 字节" << endl;

        ifs.close();
    }

    // 方法三: seekg + read
    {
        ifstream ifs("test_read.txt", ios::binary);
        if (!ifs.is_open()) return 1;

        // 获取文件大小
        ifs.seekg(0, ios::end);       // 移动到文件末尾
        streamsize size = ifs.tellg(); // 获取当前位置（即文件大小）
        ifs.seekg(0, ios::beg);       // 回到文件开头

        // 一次性读取
        string content(size, '\0');
        ifs.read(&content[0], size);
        cout << "方法三(seekg+read) - 文件大小: " << size << " 字节" << endl;

        ifs.close();
    }

    cout << "\n========== 8. tellg() 获取读取位置 ==========" << endl;
    // tellg(): 返回当前读取位置（get position）
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        cout << "初始位置: " << ifs.tellg() << endl;

        string word;
        ifs >> word;
        cout << "读取 [" << word << "] 后位置: " << ifs.tellg() << endl;

        string line;
        ifs.clear();
        ifs.seekg(0, ios::beg);  // 回到开头
        getline(ifs, line);
        cout << "读取第一行后位置: " << ifs.tellg() << endl;

        ifs.close();
    }

    cout << "\n========== 9. seekg() 移动读取位置 ==========" << endl;
    // seekg(offset, direction): 移动读取位置
    // direction: ios::beg(开头), ios::cur(当前位置), ios::end(末尾)
    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        // 移到文件开头
        ifs.seekg(0, ios::beg);
        string line1;
        getline(ifs, line1);
        cout << "第一行: " << line1 << endl;

        // 跳过第二行，直接读第三行
        ifs.seekg(0, ios::beg);  // 回到开头
        getline(ifs, line1);     // 跳过第一行
        getline(ifs, line1);     // 跳过第二行
        string line3;
        getline(ifs, line3);
        cout << "第三行: " << line3 << endl;

        // 相对移动
        ifs.seekg(0, ios::beg);
        ifs.seekg(5, ios::cur);  // 从当前位置后移5个字节
        string rest;
        getline(ifs, rest);
        cout << "跳过5字符后: " << rest << endl;

        ifs.close();
    }

    cout << "\n========== 10. 结构化数据读取 ==========" << endl;
    // 从文件读取结构化数据
    struct Student {
        string name;
        double score;
    };

    {
        ifstream ifs("test_read.txt");
        if (!ifs.is_open()) return 1;

        vector<Student> students;
        string name;
        double score;

        // 跳过前3行（非学生数据）
        string skip;
        getline(ifs, skip);
        getline(ifs, skip);
        getline(ifs, skip);

        // 读取学生数据: 每行格式为 "姓名 成绩"
        while (ifs >> name >> score) {
            students.push_back({name, score});
        }

        cout << "读取到 " << students.size() << " 个学生:" << endl;
        for (const auto& s : students) {
            cout << "  " << s.name << ": " << s.score << endl;
        }

        ifs.close();
    }

    cout << "\n========== 11. 错误处理 ==========" << endl;
    // 文件读取的错误处理
    {
        // 尝试打开不存在的文件
        ifstream ifs("nonexistent_file.txt");
        if (!ifs.is_open()) {
            cout << "文件不存在或无法打开" << endl;
        }

        // 检查流状态
        ifs.open("test_read.txt");
        if (ifs.is_open()) {
            string word;

            // 正常读取
            ifs >> word;
            cout << "状态检查 - good(): " << ifs.good() << endl;
            cout << "状态检查 - eof():  " << ifs.eof() << endl;
            cout << "状态检查 - fail(): " << ifs.fail() << endl;
            cout << "状态检查 - bad():  " << ifs.bad() << endl;

            // 读取到文件末尾
            while (ifs >> word) {}  // 读完所有内容
            cout << "读取完毕后 - eof():  " << ifs.eof() << endl;
            cout << "读取完毕后 - fail(): " << ifs.fail() << endl;

            // 清除错误状态后可以重新使用
            ifs.clear();
            ifs.seekg(0, ios::beg);
            cout << "clear() 后 - good(): " << ifs.good() << endl;
        }

        ifs.close();
    }

    cout << "\n========== 12. 常见错误 ==========" << endl;
    // 错误1: eof 用法错误导致最后一行重复
    // while (!ifs.eof()) { getline(ifs, line); cout << line; }
    // 正确: while (getline(ifs, line)) { ... }

    // 错误2: 读取失败后不清除错误状态
    // int num; ifs >> num;  // 如果输入不是数字，流进入错误状态
    // ifs.clear();  // 必须清除错误状态
    // ifs.ignore(numeric_limits<streamsize>::max(), '\n');  // 丢弃错误输入

    // 错误3: >> 和 getline 混用时忘记处理换行符
    // ifs >> word;  // 读取单词，换行符留在缓冲区
    // getline(ifs, line);  // 会读到空行！
    // 正确: 在 >> 后加 ifs.ignore() 跳过换行符

    // 错误4: 忘记检查文件是否打开
    // ifstream ifs("file.txt");
    // ifs >> data;  // 如果文件不存在，静默失败

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 13. 清理临时文件 ==========" << endl;
    remove("test_read.txt");
    remove("test_csv.txt");
    cout << "临时文件已清理" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
