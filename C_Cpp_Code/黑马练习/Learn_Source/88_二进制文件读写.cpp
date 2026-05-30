/*
 * 88_二进制文件读写.cpp
 * 本文件演示 C++ 二进制文件的读写操作
 * 包括: ios::binary 模式, write(), read(), 结构体读写,
 *       tellg/tellp, seekg/seekp 定位, 文件大小计算, 文件复制
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>     // memcpy
using namespace std;

// 用于演示的结构体
struct Student {
    char name[50];     // 使用固定大小的字符数组（不建议用 string）
    int age;
    double score;
    bool passed;
};

// 打印学生信息
void printStudent(const Student& s) {
    cout << "  姓名: " << s.name
         << ", 年龄: " << s.age
         << ", 成绩: " << s.score
         << ", 是否通过: " << (s.passed ? "是" : "否") << endl;
}

// 用于演示嵌套结构体
struct Address {
    char city[30];
    char street[50];
    int zipcode;
};

struct Employee {
    char name[50];
    int id;
    Address addr;      // 嵌套结构体
    double salary;
};

int main() {
    cout << "========== 1. 二进制写入基础 ==========" << endl;
    // ios::binary: 以二进制模式打开文件
    // 文本模式下，换行符会被转换（Windows: \n -> \r\n）
    // 二进制模式不做任何转换，原样读写
    {
        ofstream ofs("binary_data.bin", ios::binary);
        if (!ofs.is_open()) {
            cerr << "无法创建文件!" << endl;
            return 1;
        }

        // write() 函数: write(const char* buffer, streamsize count)
        // buffer: 要写入的数据的起始地址
        // count: 要写入的字节数

        // 写入基本类型
        int intVal = 42;
        double doubleVal = 3.14159;
        bool boolVal = true;
        char charVal = 'A';

        ofs.write(reinterpret_cast<const char*>(&intVal), sizeof(intVal));
        ofs.write(reinterpret_cast<const char*>(&doubleVal), sizeof(doubleVal));
        ofs.write(reinterpret_cast<const char*>(&boolVal), sizeof(boolVal));
        ofs.write(reinterpret_cast<const char*>(&charVal), sizeof(charVal));

        // 写入字符串（需要先写长度，再写内容）
        string str = "Hello Binary File";
        size_t len = str.size();
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ofs.write(str.c_str(), len);

        cout << "二进制写入完成" << endl;
        cout << "写入数据大小: int=" << sizeof(intVal)
             << ", double=" << sizeof(doubleVal)
             << ", bool=" << sizeof(boolVal)
             << ", char=" << sizeof(charVal) << endl;

        ofs.close();
    }

    cout << "\n========== 2. 二进制读取基础 ==========" << endl;
    // read() 函数: read(char* buffer, streamsize count)
    // 必须按写入时的顺序和大小读取
    {
        ifstream ifs("binary_data.bin", ios::binary);
        if (!ifs.is_open()) {
            cerr << "无法打开文件!" << endl;
            return 1;
        }

        // 按写入顺序读取
        int intVal;
        double doubleVal;
        bool boolVal;
        char charVal;

        ifs.read(reinterpret_cast<char*>(&intVal), sizeof(intVal));
        ifs.read(reinterpret_cast<char*>(&doubleVal), sizeof(doubleVal));
        ifs.read(reinterpret_cast<char*>(&boolVal), sizeof(boolVal));
        ifs.read(reinterpret_cast<char*>(&charVal), sizeof(charVal));

        cout << "读取结果:" << endl;
        cout << "  int: " << intVal << endl;
        cout << "  double: " << doubleVal << endl;
        cout << "  bool: " << boolVal << endl;
        cout << "  char: " << charVal << endl;

        // 读取字符串
        size_t len;
        ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
        string str(len, '\0');
        ifs.read(&str[0], len);
        cout << "  string: " << str << endl;

        ifs.close();
    }

    cout << "\n========== 3. 结构体的二进制读写 ==========" << endl;
    // 结构体可以直接用 write/read 进行二进制读写
    // 注意: 结构体中不能使用 string 等动态类型，必须用固定大小的数组
    {
        // 写入多个学生记录
        Student students[] = {
            {"张三", 20, 85.5, true},
            {"李四", 21, 92.0, true},
            {"王五", 19, 58.0, false},
            {"赵六", 22, 76.5, true},
            {"钱七", 20, 45.0, false}
        };
        int count = 5;

        ofstream ofs("students.bin", ios::binary);
        if (!ofs.is_open()) return 1;

        // 先写入记录数量
        ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

        // 写入所有学生数据
        ofs.write(reinterpret_cast<const char*>(students), sizeof(students));

        cout << "写入 " << count << " 个学生记录" << endl;
        cout << "每条记录大小: " << sizeof(Student) << " 字节" << endl;
        cout << "总数据大小: " << sizeof(students) << " 字节" << endl;

        ofs.close();
    }

    // 读取学生记录
    {
        ifstream ifs("students.bin", ios::binary);
        if (!ifs.is_open()) return 1;

        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));

        // 动态分配数组
        vector<Student> students(count);
        ifs.read(reinterpret_cast<char*>(students.data()), count * sizeof(Student));

        cout << "读取到 " << count << " 个学生:" << endl;
        for (const auto& s : students) {
            printStudent(s);
        }

        ifs.close();
    }

    cout << "\n========== 4. 嵌套结构体读写 ==========" << endl;
    // 嵌套结构体也可以直接二进制读写（前提是所有成员都是固定大小）
    {
        Employee emp = {"张三", 1001, {"北京", "朝阳区建国路", 100020}, 15000.0};

        // 写入
        ofstream ofs("employee.bin", ios::binary);
        ofs.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
        ofs.close();

        // 读取
        Employee empRead;
        ifstream ifs("employee.bin", ios::binary);
        ifs.read(reinterpret_cast<char*>(&empRead), sizeof(empRead));
        ifs.close();

        cout << "嵌套结构体读取:" << endl;
        cout << "  姓名: " << empRead.name << endl;
        cout << "  工号: " << empRead.id << endl;
        cout << "  城市: " << empRead.addr.city << endl;
        cout << "  街道: " << empRead.addr.street << endl;
        cout << "  邮编: " << empRead.addr.zipcode << endl;
        cout << "  薪资: " << empRead.salary << endl;
    }

    cout << "\n========== 5. tellg / tellp 获取位置 ==========" << endl;
    // tellg(): 获取"读取指针"的位置 (tell get)
    // tellp(): 获取"写入指针"的位置 (tell put)
    {
        ofstream ofs("position_test.bin", ios::binary);
        int values[] = {10, 20, 30, 40, 50};

        for (int i = 0; i < 5; ++i) {
            cout << "写入 " << values[i] << " 前, tellp = " << ofs.tellp() << endl;
            ofs.write(reinterpret_cast<const char*>(&values[i]), sizeof(int));
            cout << "写入 " << values[i] << " 后, tellp = " << ofs.tellp() << endl;
        }
        ofs.close();

        cout << endl;

        ifstream ifs("position_test.bin", ios::binary);
        int val;
        for (int i = 0; i < 5; ++i) {
            cout << "读取前 tellg = " << ifs.tellg() << "  ";
            ifs.read(reinterpret_cast<char*>(&val), sizeof(int));
            cout << "读取: " << val << ", 读取后 tellg = " << ifs.tellg() << endl;
        }
        ifs.close();
    }

    cout << "\n========== 6. seekg / seekp 定位 ==========" << endl;
    // seekg(offset, direction): 移动读取指针 (seek get)
    // seekp(offset, direction): 移动写入指针 (seek put)
    // direction: ios::beg(开头), ios::cur(当前), ios::end(末尾)
    {
        ifstream ifs("students.bin", ios::binary);
        if (!ifs.is_open()) return 1;

        // 读取记录数量
        int count;
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        cout << "学生总数: " << count << endl;

        // 直接跳到第3个学生（索引2）
        int recordIndex = 2;
        streampos offset = sizeof(int) + recordIndex * sizeof(Student);
        ifs.seekg(offset, ios::beg);

        Student s;
        ifs.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "第3个学生: ";
        printStudent(s);

        // 从末尾向前读取最后一个学生
        ifs.seekg(-static_cast<streamoff>(sizeof(Student)), ios::end);
        ifs.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "最后一个学生: ";
        printStudent(s);

        // 相对移动: 跳过一个记录
        ifs.seekg(sizeof(int), ios::beg);  // 回到第一个学生
        ifs.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "第1个学生: ";
        printStudent(s);

        ifs.seekg(sizeof(Student), ios::cur);  // 跳过一个记录
        ifs.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "第2个学生: ";
        printStudent(s);

        ifs.close();
    }

    cout << "\n========== 7. 文件大小计算 ==========" << endl;
    // 方法一: seekg + tellg
    {
        ifstream ifs("students.bin", ios::binary);
        if (!ifs.is_open()) return 1;

        ifs.seekg(0, ios::end);    // 移到文件末尾
        streamsize fileSize = ifs.tellg();  // 获取位置（即文件大小）
        ifs.seekg(0, ios::beg);    // 回到开头

        cout << "文件大小(seekg): " << fileSize << " 字节" << endl;
        ifs.close();
    }

    // 方法二: 使用 file_size (C++17 的 filesystem)
    // #include <filesystem>
    // auto size = filesystem::file_size("students.bin");

    cout << "\n========== 8. 随机访问修改记录 ==========" << endl;
    // 二进制文件支持随机读写，可以直接修改某条记录
    {
        fstream fs("students.bin", ios::binary | ios::in | ios::out);
        if (!fs.is_open()) return 1;

        // 修改第3个学生的成绩（索引2）
        int recordIndex = 2;
        streampos offset = sizeof(int) + recordIndex * sizeof(Student);
        fs.seekg(offset, ios::beg);

        Student s;
        fs.read(reinterpret_cast<char*>(&s), sizeof(Student));
        cout << "修改前: ";
        printStudent(s);

        // 修改成绩
        s.score = 75.0;
        s.passed = true;

        // 写回（注意: seekp，不是 seekg）
        fs.seekp(offset, ios::beg);
        fs.write(reinterpret_cast<const char*>(&s), sizeof(Student));

        // 验证修改
        fs.seekg(offset, ios::beg);
        Student sVerify;
        fs.read(reinterpret_cast<char*>(&sVerify), sizeof(Student));
        cout << "修改后: ";
        printStudent(sVerify);

        fs.close();
    }

    cout << "\n========== 9. 文件复制示例 ==========" << endl;
    // 使用二进制读写复制文件
    {
        const char* srcFile = "students.bin";
        const char* dstFile = "students_copy.bin";

        ifstream ifs(srcFile, ios::binary);
        ofstream ofs(dstFile, ios::binary);

        if (!ifs.is_open() || !ofs.is_open()) {
            cerr << "文件打开失败!" << endl;
            return 1;
        }

        // 获取源文件大小
        ifs.seekg(0, ios::end);
        streamsize fileSize = ifs.tellg();
        ifs.seekg(0, ios::beg);

        // 使用缓冲区分块复制
        const size_t BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        streamsize totalCopied = 0;

        while (totalCopied < fileSize) {
            // 计算本次要读取的字节数
            streamsize toRead = min(static_cast<streamsize>(BUFFER_SIZE),
                                    fileSize - totalCopied);
            ifs.read(buffer, toRead);
            ofs.write(buffer, ifs.gcount());  // gcount() 返回实际读取的字节数
            totalCopied += ifs.gcount();
        }

        cout << "文件复制完成: " << srcFile << " -> " << dstFile
             << " (" << totalCopied << " 字节)" << endl;

        ifs.close();
        ofs.close();
    }

    cout << "\n========== 10. 追加记录到二进制文件 ==========" << endl;
    // 使用 ios::app 模式追加记录
    {
        // 先读取当前记录数
        int count;
        {
            ifstream ifs("students.bin", ios::binary);
            ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
            ifs.close();
        }

        // 追加新记录
        Student newStudent = {"孙八", 23, 88.0, true};
        {
            ofstream ofs("students.bin", ios::binary | ios::in | ios::out);
            if (!ofs.is_open()) return 1;

            // 更新记录数
            count++;
            ofs.seekp(0, ios::beg);
            ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

            // 移到文件末尾追加
            ofs.seekp(0, ios::end);
            ofs.write(reinterpret_cast<const char*>(&newStudent), sizeof(newStudent));

            ofs.close();
        }

        // 验证
        ifstream ifs("students.bin", ios::binary);
        ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
        cout << "追加后共 " << count << " 条记录:" << endl;
        for (int i = 0; i < count; ++i) {
            Student s;
            ifs.read(reinterpret_cast<char*>(&s), sizeof(Student));
            printStudent(s);
        }
        ifs.close();
    }

    cout << "\n========== 11. 常见错误 ==========" << endl;
    // 错误1: 文本模式和二进制模式混用
    // 文本模式下 \n 会被转换为 \r\n (Windows)，导致读写大小不一致

    // 错误2: 结构体包含 std::string 等动态类型
    // struct Bad { string name; int age; };
    // ofs.write((char*)&bad, sizeof(Bad));  // 写入的是 string 对象的内存，不是字符串内容！
    // 正确做法: 使用 char[] 或分别写入每个字段

    // 错误3: 不同编译器/平台的结构体对齐可能不同
    // #pragma pack(push, 1) 可以取消对齐，但需要两端都使用

    // 错误4: read 的字节数不对
    // ifs.read(buf, 100);  // 如果文件不足100字节，读取会部分失败

    // 错误5: 忘记 ios::binary 模式
    // 在 Windows 上，文本模式会转换换行符，导致 seekg/tellg 返回值不准确

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 12. 清理临时文件 ==========" << endl;
    remove("binary_data.bin");
    remove("students.bin");
    remove("students_copy.bin");
    remove("position_test.bin");
    remove("employee.bin");
    cout << "临时文件已清理" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
