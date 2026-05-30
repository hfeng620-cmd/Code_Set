/*
 * ============================================================================
 * 主题: C++ 字符串型详解
 * ============================================================================
 * 字符串是字符的序列, C++ 有两种字符串表示方式:
 *   - C 风格字符串: char 数组, 以 '\0' 结尾
 *   - C++ string 类: 标准库提供的字符串类 (推荐使用)
 * 本文件介绍两种方式的用法和区别。
 * ============================================================================
 */

#include <iostream>
#include <cstring>   // C 风格字符串函数: strlen, strcpy, strcmp 等
#include <string>    // C++ string 类
using namespace std;

int main() {

    // ==================== C 风格字符串 ====================
    // C 风格字符串是一个 char 数组, 以空字符 '\0' 结尾
    // '\0' 是字符串的结束标志, ASCII 值为 0
    cout << "===== C 风格字符串 (char 数组) =====" << endl;

    // 方式1: 用双引号初始化 (自动添加 '\0')
    char str1[] = "Hello";   // 实际存储: {'H','e','l','l','o','\0'}, 大小为 6
    cout << "str1 = " << str1 << endl;
    cout << "str1 占用字节: " << sizeof(str1) << " (包含 '\\0')" << endl;

    // 方式2: 指定大小
    char str2[20] = "World";  // 预留 20 字节空间
    cout << "str2 = " << str2 << endl;

    // 方式3: 逐字符初始化 (必须手动添加 '\0')
    char str3[] = {'C', '+', '+', '\0'};
    cout << "str3 = " << str3 << endl;

    // 错误示例: 忘记 '\0' (会导致输出乱码)
    // char bad[] = {'H', 'i'};  // 没有 '\0', cout 不知道在哪里停止

    // ==================== C 风格字符串函数 ====================
    cout << "\n===== C 风格字符串函数 (cstring) =====" << endl;

    char source[] = "Hello, C++!";
    char dest[50];

    // strlen: 获取字符串长度 (不含 '\0')
    cout << "strlen(\"" << source << "\") = " << strlen(source) << endl;
    cout << "sizeof(数组) = " << sizeof(source) << " (含 '\\0')" << endl;

    // strcpy: 复制字符串
    strcpy(dest, source);   // 将 source 复制到 dest
    cout << "strcpy 后 dest = " << dest << endl;

    // strcat: 追加字符串
    char greeting[50] = "Hello";
    strcat(greeting, ", World!");   // 在 greeting 后追加
    cout << "strcat 后 greeting = " << greeting << endl;

    // strcmp: 比较字符串
    // 返回值: 0 表示相等, 负数表示 str1 < str2, 正数表示 str1 > str2
    char s1[] = "apple";
    char s2[] = "banana";
    char s3[] = "apple";
    cout << "strcmp(\"apple\", \"banana\") = " << strcmp(s1, s2) << endl;  // 负数
    cout << "strcmp(\"apple\", \"apple\") = " << strcmp(s1, s3) << endl;   // 0
    cout << "strcmp(\"banana\", \"apple\") = " << strcmp(s2, s1) << endl;  // 正数

    // strstr: 查找子串
    char text[] = "Hello, World!";
    char* found = strstr(text, "World");   // 返回指向首次出现位置的指针
    if (found != NULL) {
        cout << "找到 \"World\" 在位置: " << (found - text) << endl;
    }

    // strchr: 查找字符
    char* pos = strchr(text, 'W');
    if (pos != NULL) {
        cout << "找到 'W' 在位置: " << (pos - text) << endl;
    }

    // ==================== C++ string 类 ====================
    cout << "\n===== C++ string 类 =====" << endl;
    // string 是 C++ 标准库提供的字符串类, 更安全、更方便
    // 需要 #include <string>
    // 自动管理内存, 不需要担心缓冲区溢出

    // 创建 string 对象
    string str10 = "Hello";
    string str11("World");
    string str12 = string("C++");
    string str13;           // 空字符串
    string str14(5, 'A');   // 5 个 'A': "AAAAA"

    cout << "str10 = " << str10 << endl;
    cout << "str11 = " << str11 << endl;
    cout << "str12 = " << str12 << endl;
    cout << "str13 = [" << str13 << "]" << endl;
    cout << "str14 = " << str14 << endl;

    // ==================== string 基本操作 ====================
    cout << "\n===== string 基本操作 =====" << endl;

    string name = "张三";
    string greeting2 = "你好";

    // 字符串拼接: 使用 + 运算符
    string message = greeting2 + ", " + name + "!";
    cout << "拼接: " << message << endl;

    // 获取长度: size() 或 length()
    cout << "长度: " << message.size() << endl;    // 返回字符数
    cout << "长度: " << message.length() << endl;  // 同 size()

    // 判断是否为空: empty()
    string empty = "";
    cout << "空字符串? " << (empty.empty() ? "是" : "否") << endl;
    cout << "message 为空? " << (message.empty() ? "是" : "否") << endl;

    // 访问单个字符: 使用 [] 或 at()
    string word = "Hello";
    cout << "word[0] = " << word[0] << endl;     // 'H'
    cout << "word.at(1) = " << word.at(1) << endl; // 'e'
    // 区别: [] 不检查越界, at() 越界会抛异常

    // 修改单个字符
    word[0] = 'h';   // 将 'H' 改为 'h'
    cout << "修改后: " << word << endl;

    // ==================== string 常用方法 ====================
    cout << "\n===== string 常用方法 =====" << endl;

    string text2 = "Hello, World! Hello, C++!";

    // substr: 获取子串
    // substr(起始位置, 长度)
    string sub1 = text2.substr(0, 5);    // 从位置 0 开始, 取 5 个字符
    string sub2 = text2.substr(7, 5);    // 从位置 7 开始, 取 5 个字符
    cout << "substr(0,5) = " << sub1 << endl;  // "Hello"
    cout << "substr(7,5) = " << sub2 << endl;  // "World"

    // find: 查找子串
    // 返回首次出现的位置, 找不到返回 string::npos
    size_t pos2 = text2.find("Hello");
    cout << "第一次 \"Hello\" 的位置: " << pos2 << endl;

    size_t pos3 = text2.find("Hello", 1);  // 从位置 1 开始查找
    cout << "第二次 \"Hello\" 的位置: " << pos3 << endl;

    size_t pos4 = text2.find("Python");
    if (pos4 == string::npos) {
        cout << "未找到 \"Python\"" << endl;
    }

    // rfind: 从后向前查找
    size_t pos5 = text2.rfind("Hello");
    cout << "rfind \"Hello\" 的位置: " << pos5 << endl;

    // replace: 替换子串
    string str20 = "Hello, World!";
    str20.replace(7, 5, "C++");   // 从位置 7 开始, 替换 5 个字符为 "C++"
    cout << "replace 后: " << str20 << endl;

    // insert: 插入字符串
    string str21 = "Hello World!";
    str21.insert(5, ", Beautiful");   // 在位置 5 插入
    cout << "insert 后: " << str21 << endl;

    // erase: 删除字符
    string str22 = "Hello, World!";
    str22.erase(5, 2);   // 从位置 5 开始删除 2 个字符
    cout << "erase 后: " << str22 << endl;

    // ==================== string 比较 ====================
    cout << "\n===== string 比较 =====" << endl;
    // string 可以直接用 ==, !=, <, >, <=, >= 比较

    string sa = "apple";
    string sb = "banana";
    string sc = "apple";

    cout << "\"apple\" == \"apple\" ? " << (sa == sc ? "true" : "false") << endl;
    cout << "\"apple\" != \"banana\" ? " << (sa != sb ? "true" : "false") << endl;
    cout << "\"apple\" < \"banana\" ? " << (sa < sb ? "true" : "false") << endl;

    // ==================== string 输入 ====================
    cout << "\n===== string 输入 =====" << endl;

    // cin >> 遇到空格停止
    // getline 读取整行 (包括空格)
    // 这里只做演示, 不实际等待输入

    // 用法示例 (取消注释可以测试):
    // string input1;
    // cout << "请输入一个单词 (cin): ";
    // cin >> input1;           // 遇到空格停止

    // string input2;
    // cout << "请输入一行文字 (getline): ";
    // getline(cin, input2);   // 读取整行

    // ==================== C 风格 vs C++ string 对比 ====================
    cout << "\n===== C 风格 vs C++ string =====" << endl;
    // 1. C 风格: 手动管理内存, 可能缓冲区溢出
    //    C++ string: 自动管理内存, 更安全
    // 2. C 风格: 使用函数 (strlen, strcpy...)
    //    C++ string: 使用成员方法 (size(), substr()...)
    // 3. C 风格: 不能用 + 拼接
    //    C++ string: 可以用 + 拼接
    // 4. C 风格: 不能用 == 比较
    //    C++ string: 可以用 == 比较
    // 5. 建议: 优先使用 C++ string

    // C 风格与 C++ string 互转
    // string -> char*: 使用 c_str() 方法
    string cppStr = "Hello";
    const char* cStr = cppStr.c_str();
    cout << "string 转 C 风格: " << cStr << endl;

    // char* -> string: 直接赋值或构造
    char cArr[] = "World";
    string fromC = cArr;   // 自动转换
    cout << "C 风格转 string: " << fromC << endl;

    return 0;
}

/*
 * ==================== 总结 ====================
 * 1. C 风格字符串: char[], 以 '\0' 结尾
 * 2. C 风格函数: strlen, strcpy, strcmp, strcat
 * 3. C++ string 类: 自动管理内存, 更安全方便
 * 4. string 操作: + 拼接, size() 长度, substr() 子串
 * 5. string 查找: find() 返回位置, npos 表示未找到
 * 6. string 比较: 直接用 ==, !=, <, >
 * 7. 推荐使用 C++ string, 更安全、更易用
 * ===============================================
 */
