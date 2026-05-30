# string 字符串详解

## 一、string 概述

`string` 是 C++ STL 中的字符串类，封装了字符数组的操作，比 C 风格字符串（`char*`）
更安全、更方便。使用时需要包含 `<string>` 头文件。

```cpp
#include <string>       // string 头文件
using namespace std;    // 标准命名空间
```

---

## 二、string 的构造函数

### 2.1 五种构造方式

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    // 方式1：默认构造（空字符串）
    string s1;
    cout << "s1 = \"" << s1 << "\"" << endl;           // s1 = ""

    // 方式2：用字符串字面量构造
    string s2("Hello World");
    cout << "s2 = \"" << s2 << "\"" << endl;           // s2 = "Hello World"

    // 方式3：用另一个 string 对象拷贝构造
    string s3(s2);
    cout << "s3 = \"" << s3 << "\"" << endl;           // s3 = "Hello World"

    // 方式4：用 n 个相同字符构造
    string s4(5, 'A');
    cout << "s4 = \"" << s4 << "\"" << endl;           // s4 = "AAAAA"

    // 方式5：用字符串的部分子串构造
    string s5(s2, 6, 5);   // 从 s2 的下标6开始，取5个字符
    cout << "s5 = \"" << s5 << "\"" << endl;           // s5 = "World"

    return 0;
}
```

输出：
```
s1 = ""
s2 = "Hello World"
s3 = "Hello World"
s4 = "AAAAA"
s5 = "World"
```

---

## 三、string 赋值操作

### 3.1 使用 = 赋值

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s1;

    // 赋值方式1：用 = 号赋值字符串
    s1 = "Hello";
    cout << "s1 = " << s1 << endl;         // Hello

    // 赋值方式2：用 = 号赋值另一个 string
    string s2;
    s2 = s1;
    cout << "s2 = " << s2 << endl;         // Hello

    // 赋值方式3：用 = 号赋值单个字符
    s1 = 'A';
    cout << "s1 = " << s1 << endl;         // A

    return 0;
}
```

### 3.2 使用 assign 赋值

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;

    // assign 方式1：赋值字符串
    s.assign("World");
    cout << s << endl;                     // World

    // assign 方式2：赋值另一个 string
    string s2("C++");
    s.assign(s2);
    cout << s << endl;                     // C++

    // assign 方式3：赋值 n 个字符
    s.assign(5, '*');
    cout << s << endl;                     // *****

    // assign 方式4：赋值部分字符串
    string s3("Hello World");
    s.assign(s3, 0, 5);    // 从下标0开始取5个字符
    cout << s << endl;                     // Hello

    return 0;
}
```

---

## 四、string 拼接操作

### 4.1 使用 += 拼接

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello";

    // 拼接字符串
    s += " World";
    cout << s << endl;              // Hello World

    // 拼接另一个 string
    string s2(" !!!");
    s += s2;
    cout << s << endl;              // Hello World !!!

    // 拼接单个字符
    s += '?';
    cout << s << endl;              // Hello World !!!?

    return 0;
}
```

### 4.2 使用 append 拼接

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello";

    // append 方式1：拼接字符串
    s.append(" World");
    cout << s << endl;                      // Hello World

    // append 方式2：拼接另一个 string
    string s2("!!!");
    s.append(s2);
    cout << s << endl;                      // Hello World!!!

    // append 方式3：拼接 n 个字符
    s.append(3, '?');
    cout << s << endl;                      // Hello World!!!???

    // append 方式4：拼接部分字符串
    string s3(" ABCDEFG");
    s.append(s3, 1, 3);    // 从下标1开始取3个字符
    cout << s << endl;                      // Hello World!!!???BCD

    return 0;
}
```

---

## 五、string 查找和替换

### 5.1 查找 find

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello World Hello C++ Hello";

    // find：从左往右查找，返回第一次出现的位置
    int pos = s.find("Hello");
    cout << "第一次出现位置：" << pos << endl;      // 0

    // find 第二个参数：从指定位置开始查找
    pos = s.find("Hello", 1);
    cout << "从位置1开始找：" << pos << endl;       // 12

    // 如果找不到，返回 string::npos
    pos = s.find("Python");
    if (pos == string::npos) {
        cout << "未找到" << endl;                   // 输出这行
    }
}
```

### 5.2 反向查找 rfind

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello World Hello C++ Hello";

    // rfind：从右往左查找，返回最后一次出现的位置
    int pos = s.rfind("Hello");
    cout << "最后一次出现位置：" << pos << endl;    // 20

    // 从右往左找第一次出现的位置
    pos = s.rfind("Hello", 15);     // 从位置15往前找
    cout << "从位置15往前找：" << pos << endl;      // 12

    return 0;
}
```

### 5.3 查找子串出现次数

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "abcabcabcabc";
    string target = "abc";

    int count = 0;
    int pos = 0;

    // 循环查找
    while ((pos = s.find(target, pos)) != string::npos) {
        count++;
        pos += target.length();     // 移动到下一个位置，避免重复计数
    }

    cout << "\"" << target << "\" 出现了 " << count << " 次" << endl;
    // 输出："abc" 出现了 4 次

    return 0;
}
```

### 5.4 替换 replace

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello World";

    // replace：从位置6开始，替换5个字符为 "C++"
    s.replace(6, 5, "C++");
    cout << s << endl;                  // Hello C++

    // 替换为不同长度的字符串
    string s2 = "I like Java";
    s2.replace(7, 4, "Python");         // 将 "Java" 替换为 "Python"
    cout << s2 << endl;                 // I like Python

    // 批量替换示例：将所有空格替换为 "%%"
    string s3 = "a b c d e";
    int pos = 0;
    while ((pos = s3.find(" ", pos)) != string::npos) {
        s3.replace(pos, 1, "%%");
        pos += 2;       // 跳过替换的字符
    }
    cout << s3 << endl;                 // a%%b%%c%%d%%e

    return 0;
}
```

---

## 六、string 比较操作

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s1 = "Hello";
    string s2 = "Hello";
    string s3 = "World";
    string s4 = "Hell";

    // compare 返回值：
    //   0  ：相等
    //   >0 ：s1 大于参数
    //   <0 ：s1 小于参数

    cout << s1.compare(s2) << endl;     // 0   （相等）
    cout << s1.compare(s3) << endl;     // 负数 （"Hello" < "World"）
    cout << s3.compare(s1) << endl;     // 正数 （"World" > "Hello"）
    cout << s1.compare(s4) << endl;     // 正数 （"Hello" > "Hell"）

    // 也可以用关系运算符直接比较（因为 string 重载了这些运算符）
    if (s1 == s2) cout << "s1 等于 s2" << endl;
    if (s1 < s3) cout << "s1 小于 s3" << endl;
    if (s3 > s1) cout << "s3 大于 s1" << endl;

    return 0;
}
```

---

## 七、string 字符存取

### 7.1 [] 和 at 方式

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello";

    // 方式1：[] 运算符
    cout << s[0] << endl;       // H
    cout << s[1] << endl;       // e
    s[0] = 'h';                 // 修改第一个字符
    cout << s << endl;          // hello

    // 方式2：at() 函数
    cout << s.at(0) << endl;    // h
    cout << s.at(4) << endl;    // o
    s.at(4) = 'O';              // 修改最后一个字符
    cout << s << endl;          // hellO

    // [] 和 at 的区别：
    // [] 越界时行为未定义（可能崩溃）
    // at() 越界时抛出 out_of_range 异常
    try {
        // cout << s[100] << endl;    // 未定义行为，不安全
        cout << s.at(100) << endl;    // 抛出异常，安全
    } catch (out_of_range& e) {
        cout << "异常：" << e.what() << endl;
    }

    return 0;
}
```

### 7.2 front 和 back

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello";

    // front：获取第一个字符（等价于 s[0]）
    cout << s.front() << endl;      // H

    // back：获取最后一个字符（等价于 s[s.size()-1]）
    cout << s.back() << endl;       // o

    // 也可以修改
    s.front() = 'h';
    s.back() = 'O';
    cout << s << endl;              // hellO

    return 0;
}
```

---

## 八、string 插入和删除

### 8.1 插入 insert

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello World";

    // 在指定位置插入字符串
    s.insert(5, ",");
    cout << s << endl;                  // Hello, World

    // 在指定位置插入 n 个字符
    s.insert(0, 3, '*');
    cout << s << endl;                  // ***Hello, World

    // 在指定位置插入另一个 string
    string s2 = "Beautiful ";
    s.insert(10, s2);
    cout << s << endl;                  // ***Hello, Beautiful World

    return 0;
}
```

### 8.2 删除 erase

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello Beautiful World";

    // erase：从指定位置开始删除 n 个字符
    s.erase(5, 10);     // 从位置5开始删除10个字符（" Beautiful"）
    cout << s << endl;              // Hello World

    // 删除所有内容
    string s2 = "Test";
    s2.erase(0, s2.length());       // 删除全部
    cout << "\"" << s2 << "\"" << endl;   // ""

    return 0;
}
```

---

## 九、string 子串

### 9.1 substr 截取子串

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello World C++";

    // substr：从指定位置开始截取子串
    string sub1 = s.substr(0, 5);       // 从位置0开始取5个字符
    cout << sub1 << endl;               // Hello

    string sub2 = s.substr(6, 5);       // 从位置6开始取5个字符
    cout << sub2 << endl;               // World

    // 只指定起始位置，取到末尾
    string sub3 = s.substr(12);
    cout << sub3 << endl;               // C++

    return 0;
}
```

### 9.2 实用案例：截取邮箱用户名

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string email = "zhangsan@example.com";

    // 找到 @ 的位置
    int pos = email.find("@");
    if (pos != string::npos) {
        // 截取 @ 之前的部分作为用户名
        string username = email.substr(0, pos);
        // 截取 @ 之后的部分作为域名
        string domain = email.substr(pos + 1);

        cout << "邮箱：" << email << endl;
        cout << "用户名：" << username << endl;
        cout << "域名：" << domain << endl;
    }

    // 输出：
    // 邮箱：zhangsan@example.com
    // 用户名：zhangsan
    // 域名：example.com

    return 0;
}
```

### 9.3 分割字符串

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "apple,banana,cherry,date";
    char delimiter = ',';

    int start = 0;
    int end = 0;

    cout << "分割结果：" << endl;
    while ((end = s.find(delimiter, start)) != string::npos) {
        string word = s.substr(start, end - start);
        cout << "  " << word << endl;
        start = end + 1;
    }
    // 打印最后一个（没有逗号结尾的）
    cout << "  " << s.substr(start) << endl;

    // 输出：
    // 分割结果：
    //   apple
    //   banana
    //   cherry
    //   date

    return 0;
}
```

---

## 十、string 大小操作

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "Hello";

    // size() 和 length() 完全等价，都返回字符串长度
    cout << "size = " << s.size() << endl;          // 5
    cout << "length = " << s.length() << endl;      // 5

    // empty() 判断字符串是否为空
    cout << "empty = " << s.empty() << endl;        // 0 (false)

    string s2;
    cout << "s2 empty = " << s2.empty() << endl;    // 1 (true)

    // clear() 清空字符串
    s.clear();
    cout << "清空后 empty = " << s.empty() << endl; // 1 (true)

    // resize() 重新设置字符串大小
    string s3 = "Hello World";

    // resize 小于原长度：截断
    s3.resize(5);
    cout << s3 << endl;                             // Hello

    // resize 大于原长度：用默认字符('\0')填充
    s3.resize(10, '!');
    cout << s3 << endl;                             // Hello!!!!!
    // 注意：实际输出可能是 "Hello!!!!!"（5个!填充到10个字符）

    // resize 大于原长度，不指定填充字符
    string s4 = "Hi";
    s4.resize(5);
    cout << "长度：" << s4.size() << endl;          // 5

    return 0;
}
```

---

## 十一、综合练习

### 11.1 统计字符串中各字符出现次数

```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string s = "hello world hello c++ hello";

    // 统计每个字符出现的次数（使用数组模拟）
    int count[256] = {0};       // ASCII 字符计数数组

    for (int i = 0; i < s.size(); i++) {
        count[s[i]]++;
    }

    cout << "字符统计：" << endl;
    for (int i = 0; i < 256; i++) {
        if (count[i] > 0 && i != ' ') {     // 跳过空格
            cout << "  '" << (char)i << "' 出现了 " << count[i] << " 次" << endl;
        }
    }

    return 0;
}
```

### 11.2 字符串反转

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    string s = "Hello World";

    // 方式1：使用 STL 的 reverse 函数
    string s1 = s;
    reverse(s1.begin(), s1.end());
    cout << "反转：" << s1 << endl;     // dlroW olleH

    // 方式2：手动反转
    string s2 = s;
    int left = 0;
    int right = s2.size() - 1;
    while (left < right) {
        char temp = s2[left];
        s2[left] = s2[right];
        s2[right] = temp;
        left++;
        right--;
    }
    cout << "手动反转：" << s2 << endl; // dlroW olleH

    return 0;
}
```

---

## 十二、总结

| 操作 | 函数 | 说明 |
|------|------|------|
| 构造 | `string()`, `string("abc")`, `string(n, 'c')` | 5种构造方式 |
| 赋值 | `=`, `assign()` | 支持字符串、字符、string对象 |
| 拼接 | `+=`, `append()` | 支持字符串、字符、string对象 |
| 查找 | `find()`, `rfind()` | find从左找，rfind从右找 |
| 替换 | `replace()` | 替换指定位置的字符 |
| 比较 | `compare()`, `==`, `!=`, `>`, `<` | 返回0表示相等 |
| 存取 | `[]`, `at()`, `front()`, `back()` | at更安全，越界抛异常 |
| 插入 | `insert()` | 在指定位置插入 |
| 删除 | `erase()` | 删除指定位置的字符 |
| 子串 | `substr()` | 截取部分字符串 |
| 大小 | `size()`, `length()`, `empty()`, `clear()`, `resize()` | size和length等价 |
