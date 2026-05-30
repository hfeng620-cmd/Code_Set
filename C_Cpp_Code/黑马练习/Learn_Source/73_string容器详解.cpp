/*
 * 文件名: 73_string容器详解.cpp
 * 描述: string字符串容器的详细用法
 *       包括构造、拼接、查找、截取、比较、修改、转换等
 * 编译: g++ -std=c++11 73_string容器详解.cpp -o 73_string容器详解
 */

#include <iostream>
#include <string>
#include <algorithm>  // sort, reverse等
#include <cstring>     // C字符串函数: strlen, strcmp等

using namespace std;

// 打印字符串信息的辅助函数
void printStr(const string& label, const string& s) {
    cout << label << ": \"" << s << "\" (长度=" << s.size() << ")" << endl;
}

int main() {
    cout << "========== 一、string构造方式 ==========" << endl;

    // 方式1: 默认构造（空字符串）
    string s1;
    printStr("默认构造 s1", s1);

    // 方式2: 拷贝构造
    string s2("Hello, World!");
    printStr("C字符串构造 s2", s2);

    // 方式3: 拷贝构造
    string s3(s2);
    printStr("拷贝构造 s3(s2)", s3);

    // 方式4: 子串构造（从s2的位置6开始，取5个字符）
    string s4(s2, 6, 5);
    printStr("子串构造 s4(s2,6,5)", s4);

    // 方式5: 用n个相同字符构造
    string s5(5, '*');
    printStr("重复字符 s5(5,'*')", s5);

    // 方式6: 使用初始化列表（C++11）
    string s6 = {'H', 'e', 'l', 'l', 'o'};
    printStr("初始化列表 s6", s6);

    // 方式7: 赋值操作
    string s7;
    s7 = "Assign string";
    printStr("赋值 s7", s7);

    cout << "\n========== 二、字符串拼接 ==========" << endl;

    string base = "Hello";

    // 方式1: + 运算符拼接（返回新字符串，不修改原字符串）
    string result1 = base + ", " + "World!";
    printStr("+ 运算符", result1);

    // 方式2: += 运算符（追加到自身，修改原字符串）
    string result2 = base;
    result2 += ", ";
    result2 += "C++";
    printStr("+= 运算符", result2);

    // 方式3: append() 成员函数
    string result3 = base;
    result3.append(", ");          // 追加字符串
    result3.append(3, '!');        // 追加3个'!'
    result3.append("ABC", 1, 2);  // 追加"ABC"从位置1开始的2个字符
    printStr("append()", result3);

    // 方式4: push_back() 追加单个字符
    string result4 = base;
    result4.push_back('?');
    printStr("push_back()", result4);

    cout << "\n========== 三、字符串查找 ==========" << endl;

    string text = "Hello, World! Hello, C++! Hello, STL!";

    // find(): 从左向右查找，返回第一次出现的位置
    // 如果找不到，返回 string::npos (一个很大的值，通常为-1的无符号表示)
    size_t pos = text.find("Hello");
    cout << "第一次出现Hello的位置: " << pos << endl;

    // find()从指定位置开始查找
    pos = text.find("Hello", 10);  // 从位置10开始查找
    cout << "从位置10开始找Hello: " << pos << endl;

    // 查找所有出现位置
    cout << "所有Hello的位置: ";
    size_t searchPos = 0;
    while ((searchPos = text.find("Hello", searchPos)) != string::npos) {
        cout << searchPos << " ";
        searchPos += 5;  // 跳过当前找到的"Hello"
    }
    cout << endl;

    // rfind(): 从右向左查找（查找最后一次出现的位置）
    pos = text.rfind("Hello");
    cout << "最后一次出现Hello的位置: " << pos << endl;

    // find_first_of(): 查找字符集中任意字符第一次出现的位置
    string chars = "aeiou";  // 元音字母
    pos = text.find_first_of(chars);
    cout << "第一个元音字母位置: " << pos << " (字符: '" << text[pos] << "')" << endl;

    // find_last_of(): 查找字符集中任意字符最后一次出现的位置
    pos = text.find_last_of(chars);
    cout << "最后一个元音字母位置: " << pos << " (字符: '" << text[pos] << "')" << endl;

    // find_first_not_of(): 查找不在字符集中的第一个字符
    pos = text.find_first_not_of("Hello, ");
    cout << "第一个不是'Hello, '的字符位置: " << pos << " (字符: '" << text[pos] << "')" << endl;

    cout << "\n========== 四、字符串截取 ==========" << endl;

    string full = "Hello, World!";

    // substr(pos, n): 从位置pos开始截取n个字符
    // 如果省略n，截取到字符串末尾
    string sub1 = full.substr(0, 5);     // 从位置0取5个字符
    string sub2 = full.substr(7, 5);     // 从位置7取5个字符
    string sub3 = full.substr(7);        // 从位置7取到末尾
    printStr("substr(0,5)", sub1);
    printStr("substr(7,5)", sub2);
    printStr("substr(7)", sub3);

    // 实用示例: 提取文件扩展名
    string filename = "document.txt.backup";
    size_t dotPos = filename.rfind('.');  // 从右找'.'
    if (dotPos != string::npos) {
        string ext = filename.substr(dotPos);
        printStr("文件扩展名", ext);
    }

    cout << "\n========== 五、字符串比较 ==========" << endl;

    string a = "apple";
    string b = "banana";
    string c = "apple";

    // 方式1: 关系运算符比较（推荐）
    cout << "apple == banana: " << (a == b ? "true" : "false") << endl;
    cout << "apple == apple:  " << (a == c ? "true" : "false") << endl;
    cout << "apple != banana: " << (a != b ? "true" : "false") << endl;
    cout << "apple < banana:  " << (a < b ? "true" : "false") << endl;   // 字典序
    cout << "apple > banana:  " << (a > b ? "true" : "false") << endl;

    // 方式2: compare() 成员函数
    // 返回值: 0相等, <0小于, >0大于
    int cmp = a.compare(b);
    cout << "apple.compare(banana) = " << cmp << endl;
    cout << "apple.compare(apple) = " << a.compare(c) << endl;

    // 与C字符串比较
    cout << "apple == \"apple\": " << (a == "apple" ? "true" : "false") << endl;

    cout << "\n========== 六、字符串修改 ==========" << endl;

    // replace(): 替换子串
    string s_replace = "Hello, World!";
    s_replace.replace(7, 5, "C++");  // 从位置7开始，替换5个字符为"C++"
    printStr("replace(7,5,\"C++\")", s_replace);

    // insert(): 在指定位置插入字符串
    string s_insert = "Hello World!";
    s_insert.insert(5, ", Beautiful");  // 在位置5插入
    printStr("insert(5, \", Beautiful\")", s_insert);

    // erase(): 删除子串
    string s_erase = "Hello, Beautiful World!";
    s_erase.erase(5, 11);  // 从位置5开始删除11个字符
    printStr("erase(5, 11)", s_erase);

    // 单字符操作
    string s_char = "Hello";
    s_char[0] = 'h';           // 用[]修改
    s_char.at(4) = '!';        // 用at()修改
    printStr("单字符修改", s_char);

    // clear(): 清空字符串
    string s_clear = "To be cleared";
    s_clear.clear();
    printStr("clear后", s_clear);

    cout << "\n========== 七、C字符串转换 ==========" << endl;

    string str = "Hello, C++!";

    // c_str(): 返回C风格字符串（以'\0'结尾）
    // 注意: 返回的指针在string修改后可能失效
    const char* cstr = str.c_str();
    cout << "c_str(): " << cstr << endl;
    cout << "strlen: " << strlen(cstr) << endl;

    // data(): 返回字符数组指针（C++11后也保证以'\0'结尾）
    const char* dstr = str.data();
    cout << "data(): " << dstr << endl;

    // 从C字符串构造string
    const char* cstyle = "From C string";
    string fromC(cstyle);
    printStr("从C字符串构造", fromC);

    // 实际应用: 调用需要C风格字符串的C函数
    // 例如: fopen(str.c_str(), "r");

    cout << "\n========== 八、字符串输入 ==========" << endl;

    // 注意: cin >> 遇到空格会停止读取
    // getline(): 读取一整行（包括空格）

    cout << "（以下输入示例在交互模式下可用）" << endl;

    // getline基本用法
    // string input;
    // cout << "请输入一行文字: ";
    // getline(cin, input);
    // cout << "你输入了: " << input << endl;

    // getline指定分隔符
    // getline(cin, input, '#');  // 读到'#'为止

    // 演示getline的效果（模拟）
    string demo = "Hello, World! This is a test.";
    cout << "模拟getline结果: " << demo << endl;

    cout << "\n========== 九、字符串遍历和算法 ==========" << endl;

    string text2 = "Hello, World!";

    // []运算符遍历
    cout << "[]遍历: ";
    for (size_t i = 0; i < text2.size(); ++i) {
        cout << text2[i];
    }
    cout << endl;

    // 迭代器遍历
    cout << "迭代器遍历: ";
    for (auto it = text2.begin(); it != text2.end(); ++it) {
        cout << *it;
    }
    cout << endl;

    // 范围for遍历
    cout << "范围for遍历: ";
    for (const auto& ch : text2) {
        cout << ch;
    }
    cout << endl;

    // 使用algorithm中的函数
    string text3 = "Hello";
    reverse(text3.begin(), text3.end());  // 反转
    printStr("反转后", text3);

    // 转换大小写
    string mixed = "Hello World";
    // 转大写
    for (auto& ch : mixed) {
        ch = toupper(ch);
    }
    printStr("转大写", mixed);

    // 转小写
    for (auto& ch : mixed) {
        ch = tolower(ch);
    }
    printStr("转小写", mixed);

    // sort排序字符串（按字符排序）
    string unsorted = "dcbafe";
    sort(unsorted.begin(), unsorted.end());
    printStr("排序后", unsorted);

    cout << "\n========== 十、string的其他常用成员 ==========" << endl;

    string s = "Hello";

    // empty(): 判断是否为空
    cout << "empty(): " << (s.empty() ? "空" : "非空") << endl;

    // size() / length(): 返回字符串长度（两者等价）
    cout << "size(): " << s.size() << endl;
    cout << "length(): " << s.length() << endl;

    // max_size(): 返回字符串最大可能长度
    cout << "max_size(): " << s.max_size() << endl;

    // capacity(): 返回当前分配的存储空间
    cout << "capacity(): " << s.capacity() << endl;

    // front() 和 back()（C++11）
    cout << "front(): " << s.front() << endl;
    cout << "back(): " << s.back() << endl;

    // 字符串拼接性能优化提示:
    // 如果需要大量拼接操作，预先reserve()可以提升性能
    string optimized;
    optimized.reserve(1000);  // 预分配1000字符的空间

    cout << "\n========== 十一、常见错误和注意事项 ==========" << endl;

    /*
     * 常见错误1: find返回值未检查npos
     *   错误: size_t pos = s.find("x"); cout << s[pos]; // 如果找不到，pos=npos
     *   正确: 先检查 pos != string::npos
     *
     * 常见错误2: substr越界
     *   如果pos > size()，substr会抛出out_of_range异常
     *   如果pos + n > size()，substr只取到末尾（不会抛异常）
     *
     * 常见错误3: c_str()返回的指针生命周期
     *   c_str()返回的指针在string被修改后可能失效
     *   不要保存这个指针长期使用
     *
     * 常见错误4: 混淆size()和[npos]
     *   string::npos 是一个static const值，通常等于(size_t)-1
     *   用于表示"未找到"
     *
     * 常见错误5: 空字符串操作
     *   空字符串的front()和back()是未定义行为
     *   使用前先检查empty()
     */

    cout << "string容器详解完毕！" << endl;

    return 0;
}
