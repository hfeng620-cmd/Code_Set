/*
 * 86_文件写操作详解.cpp
 * 本文件演示 C++ 文件写操作的详细用法
 * 包括: ofstream 的打开、写入、关闭，写入模式，<< 运算符，put()，
 *       is_open() 检查，RAII 自动关闭
 */

#include <iostream>
#include <fstream>      // 文件流头文件
#include <string>
#include <vector>
#include <iomanip>      // setw, setprecision
using namespace std;

int main() {
    // 文件路径（使用相对路径，文件会创建在程序运行的目录下）
    string filepath = "test_output.txt";

    cout << "========== 1. ofstream 基本写入 ==========" << endl;
    // ofstream: 输出文件流（Output File Stream）
    // 创建 ofstream 对象时可以同时指定打开的文件

    // 方式一：构造时打开文件
    // 如果文件不存在，会自动创建
    // 如果文件已存在，默认会清空原内容（ios::out 模式）
    {
        ofstream ofs(filepath);  // RAII: 离开作用域时自动关闭
        if (!ofs.is_open()) {
            cout << "无法打开文件!" << endl;
            return 1;
        }

        // 使用 << 运算符写入文本，与 cout 用法完全相同
        ofs << "Hello, File!" << endl;
        ofs << "这是第一行文本" << endl;
        ofs << "数字: " << 42 << endl;
        ofs << "浮点数: " << 3.14159 << endl;

        // ofs 离开作用域时自动调用 close()
        // 也可以显式调用 ofs.close()
    }  // <-- ofs 在这里自动关闭

    cout << "文件写入完成: " << filepath << endl;

    cout << "\n========== 2. 显式 open 和 close ==========" << endl;
    // 方式二：先创建对象，再调用 open()
    {
        ofstream ofs;
        ofs.open(filepath);  // 打开文件

        // 检查文件是否成功打开
        if (!ofs.is_open()) {
            cerr << "错误: 无法打开文件 " << filepath << endl;
            return 1;
        }

        ofs << "显式 open 方式写入" << endl;
        ofs << "第二行内容" << endl;

        ofs.close();  // 显式关闭文件
        // 关闭后不能再写入
        // ofs << "这会失败" << endl;  // 错误！文件已关闭

        // 可以重新打开同一个文件
        ofs.open(filepath);
        if (ofs.is_open()) {
            ofs << "重新打开后写入" << endl;
            ofs.close();
        }
    }

    cout << "\n========== 3. 文件写入模式 ==========" << endl;
    // ios::out   - 输出模式（默认），文件不存在则创建，存在则清空
    // ios::app   - 追加模式，在文件末尾写入
    // ios::trunc - 截断模式，清空文件内容（默认行为）
    // ios::binary - 二进制模式
    // ios::ate   - 打开后定位到文件末尾（但可以在任意位置写入）

    // 追加模式: 在文件末尾添加内容，不覆盖原有内容
    {
        ofstream ofs(filepath, ios::app);  // 追加模式
        if (ofs.is_open()) {
            ofs << "这是追加的内容1" << endl;
            ofs << "这是追加的内容2" << endl;
            ofs.close();
        }
    }
    cout << "追加模式写入完成" << endl;

    // 截断模式: 清空文件后写入（这是默认行为）
    {
        ofstream ofs(filepath, ios::out | ios::trunc);
        if (ofs.is_open()) {
            ofs << "截断模式: 文件被清空后重新写入" << endl;
            ofs.close();
        }
    }
    cout << "截断模式写入完成" << endl;

    cout << "\n========== 4. put() 写入单个字符 ==========" << endl;
    // put(): 写入单个字符
    {
        ofstream ofs("char_output.txt");
        if (ofs.is_open()) {
            // 写入单个字符
            ofs.put('A');
            ofs.put('B');
            ofs.put('C');
            ofs.put('\n');  // 换行

            // 使用循环写入字母表
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                ofs.put(ch);
            }
            ofs.put('\n');

            // 写入 ASCII 码对应的字符
            ofs.put(65);  // 'A'
            ofs.put(66);  // 'B'
            ofs.put('\n');

            ofs.close();
        }
    }
    cout << "put() 写入完成" << endl;

    cout << "\n========== 5. 格式化输出 ==========" << endl;
    // 使用 <iomanip> 中的操纵符进行格式化
    {
        ofstream ofs("formatted_output.txt");
        if (ofs.is_open()) {
            // 设置精度
            ofs << fixed << setprecision(2);
            ofs << "PI = " << 3.14159265 << endl;

            // 设置宽度和填充
            ofs << "|" << setw(10) << "Name" << "|"
                << setw(8) << "Score" << "|" << endl;
            ofs << "|" << string(10, '-') << "|"
                << string(8, '-') << "|" << endl;
            ofs << "|" << setw(10) << "Alice" << "|"
                << setw(8) << 95.5 << "|" << endl;
            ofs << "|" << setw(10) << "Bob" << "|"
                << setw(8) << 87.3 << "|" << endl;

            // 左对齐
            ofs << left;
            ofs << "|" << setw(10) << "Charlie" << "|"
                << setw(8) << 92.0 << "|" << endl;

            // 恢复右对齐
            ofs << right;
            ofs << "|" << setw(10) << "David" << "|"
                << setw(8) << 88.8 << "|" << endl;

            // 八进制和十六进制输出
            ofs << dec << 255 << endl;   // 十进制: 255
            ofs << oct << 255 << endl;   // 八进制: 377
            ofs << hex << 255 << endl;   // 十六进制: ff
            ofs << dec;  // 恢复十进制

            ofs.close();
        }
    }
    cout << "格式化输出完成" << endl;

    cout << "\n========== 6. 写入 vector 数据 ==========" << endl;
    // 将容器中的数据写入文件
    {
        vector<string> names = {"张三", "李四", "王五", "赵六", "钱七"};
        vector<int> scores = {85, 92, 78, 95, 88};

        ofstream ofs("student_data.txt");
        if (ofs.is_open()) {
            // 写入表头
            ofs << "姓名,成绩" << endl;

            // 写入数据
            for (size_t i = 0; i < names.size(); ++i) {
                ofs << names[i] << "," << scores[i] << endl;
            }
            ofs.close();
        }
    }
    cout << "CSV 数据写入完成" << endl;

    cout << "\n========== 7. 二进制写入 ==========" << endl;
    // 二进制模式写入（详见后续二进制文件专题）
    {
        ofstream ofs("binary_output.bin", ios::binary);
        if (ofs.is_open()) {
            int num = 12345;
            double pi = 3.14159;
            char str[] = "Hello";

            // write() 函数: write(地址, 字节数)
            ofs.write(reinterpret_cast<char*>(&num), sizeof(num));
            ofs.write(reinterpret_cast<char*>(&pi), sizeof(pi));
            ofs.write(str, sizeof(str));

            ofs.close();
        }
    }
    cout << "二进制写入完成" << endl;

    cout << "\n========== 8. 错误处理 ==========" << endl;
    // 文件写入可能失败的原因:
    // - 路径不存在
    // - 没有写入权限
    // - 磁盘已满
    // - 文件被其他程序占用
    {
        ofstream ofs;
        ofs.open("/invalid/path/file.txt");  // 无效路径

        // 检查是否成功打开
        if (!ofs.is_open()) {
            cout << "无法打开文件（路径无效）" << endl;
        }

        // 即使打开成功，写入也可能失败
        ofs.open(filepath);
        if (ofs.is_open()) {
            ofs << "测试写入" << endl;

            // 检查写入是否成功
            if (ofs.good()) {
                cout << "写入成功" << endl;
            } else if (ofs.bad()) {
                cout << "严重错误（不可恢复）" << endl;
            } else if (ofs.fail()) {
                cout << "写入失败（可恢复）" << endl;
            }

            ofs.close();
        }
    }

    cout << "\n========== 9. RAII 模式详解 ==========" << endl;
    // RAII (Resource Acquisition Is Initialization)
    // 资源获取即初始化
    // ofstream 的析构函数会自动关闭文件，这就是 RAII 的体现

    // 推荐写法: 使用 {} 限制作用域
    {
        ofstream ofs(filepath);
        ofs << "RAII 模式: 离开作用域自动关闭" << endl;
        // 不需要手动 close()
    }  // ofs 在这里析构，自动关闭文件

    // 不推荐写法: 手动管理
    // ofstream ofs(filepath);
    // ... 大量代码 ...
    // 如果这里抛出异常，文件不会被关闭！
    // ofs.close();

    cout << "\n========== 10. 常见错误 ==========" << endl;
    // 错误1: 忘记检查文件是否打开成功
    // ofstream ofs("file.txt");
    // ofs << "data";  // 如果打开失败，写入会静默失败

    // 错误2: 文件已关闭后继续写入
    // ofstream ofs("file.txt");
    // ofs.close();
    // ofs << "data";  // 未定义行为

    // 错误3: 使用错误的路径分隔符（Windows）
    // ofstream ofs("C:\new\file.txt");  // \n 是换行符！
    // 正确: ofstream ofs("C:\\new\\file.txt");
    // 或使用原始字符串: ofstream ofs(R"(C:\new\file.txt)");

    // 错误4: 追加模式和截断模式混淆
    // ios::app - 追加，每次写入都在末尾
    // ios::out (默认) - 截断，每次打开都清空文件

    // 错误5: 多线程同时写入同一个文件
    // 需要使用互斥锁或文件锁

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 11. 清理临时文件 ==========" << endl;
    // 删除测试文件
    remove("char_output.txt");
    remove("formatted_output.txt");
    remove("student_data.txt");
    remove("binary_output.txt");
    cout << "临时文件已清理" << endl;
    // 注意: test_output.txt 保留供查看

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
