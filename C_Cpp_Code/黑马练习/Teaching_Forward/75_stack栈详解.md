# stack 栈详解

## 一、栈的概念

栈（Stack）是一种 **先进后出**（FILO, First In Last Out）的数据结构。
可以想象成一个弹夹：最后压入的子弹最先被射出。

```
栈的操作示意：

  push(1)  push(2)  push(3)     pop()     pop()
                  +---+        +---+
         +---+   | 3 |        |   |       +---+
+---+    | 2 |   | 2 |        | 2 |       |   |
| 1 |    | 1 |   | 1 |        | 1 |       | 1 |
+---+    +---+   +---+        +---+       +---+
                    |              |           |
                   栈顶          栈顶        栈顶
```

**核心特点：**
- 只能在 **栈顶** 进行插入和删除操作
- 先进后出（FILO）
- 不支持随机访问，不能遍历

```cpp
#include <stack>        // stack 头文件
using namespace std;
```

---

## 二、stack 的构造

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    // 默认构造（底层容器为 deque）
    stack<int> s1;

    // 也可以在构造时指定底层容器（后面会讲）

    cout << "栈是否为空：" << s1.empty() << endl;   // 1 (true)

    return 0;
}
```

---

## 三、stack 的基本操作

### 3.1 push 和 pop

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> s;

    // push：入栈（压栈）
    s.push(10);
    s.push(20);
    s.push(30);

    cout << "栈顶元素：" << s.top() << endl;    // 30
    cout << "栈大小：" << s.size() << endl;     // 3

    // pop：出栈（弹栈）
    s.pop();    // 弹出栈顶元素 30
    cout << "pop 后栈顶：" << s.top() << endl;  // 20
    cout << "pop 后大小：" << s.size() << endl; // 2

    s.pop();    // 弹出 20
    s.pop();    // 弹出 10

    cout << "全部弹出后是否为空：" << s.empty() << endl;  // 1 (true)

    return 0;
}
```

### 3.2 top 查看栈顶

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;

int main() {
    stack<string> s;

    s.push("第一个");
    s.push("第二个");
    s.push("第三个");

    // top() 只是查看栈顶元素，不会删除
    cout << "栈顶：" << s.top() << endl;    // 第三个
    cout << "栈顶：" << s.top() << endl;    // 第三个（没有变）

    // 修改栈顶元素
    s.top() = "修改后的";
    cout << "修改后栈顶：" << s.top() << endl;  // 修改后的

    return 0;
}
```

### 3.3 empty 和 size

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> s;

    cout << "空栈 empty = " << s.empty() << endl;  // 1 (true)
    cout << "空栈 size = " << s.size() << endl;     // 0

    s.push(1);
    s.push(2);
    s.push(3);

    cout << "push后 empty = " << s.empty() << endl; // 0 (false)
    cout << "push后 size = " << s.size() << endl;   // 3

    return 0;
}
```

---

## 四、stack 不能遍历

栈不支持迭代器，不能用 for 循环遍历。如果想看栈中所有元素，只能一个一个弹出。

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);
    s.push(40);

    // 错误：stack 没有 begin() 和 end()，不能用迭代器
    // for (auto it = s.begin(); it != s.end(); it++) { ... }

    // 正确做法：逐个弹出并打印
    cout << "栈中元素（从顶到底）：" << endl;
    while (!s.empty()) {
        cout << s.top() << " ";     // 查看栈顶
        s.pop();                    // 弹出栈顶
    }
    cout << endl;   // 输出：40 30 20 10

    // 注意：打印后栈就空了
    cout << "打印后 size = " << s.size() << endl;   // 0

    return 0;
}
```

**如果不希望破坏原栈，可以拷贝一份再遍历：**

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    stack<int> s;
    s.push(10);
    s.push(20);
    s.push(30);

    // 拷贝一份
    stack<int> temp = s;

    cout << "遍历（不破坏原栈）：" << endl;
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;   // 30 20 10

    // 原栈不受影响
    cout << "原栈大小：" << s.size() << endl;   // 3

    return 0;
}
```

---

## 五、stack 的底层容器

### 5.1 默认底层容器

`stack` 是一个 **容器适配器**，它不直接存储数据，而是借助底层容器来实现。
默认底层容器是 `deque`。

```cpp
/*
    stack 的定义（简化版）：
    template <class T, class Container = deque<T>>
    class stack {
    protected:
        Container c;    // 底层容器
    public:
        void push(const T& val) { c.push_back(val); }
        void pop() { c.pop_back(); }
        T& top() { return c.back(); }
        bool empty() { return c.empty(); }
        size_t size() { return c.size(); }
    };
*/
```

### 5.2 指定底层容器

```cpp
#include <iostream>
#include <stack>
#include <vector>
#include <list>
using namespace std;

int main() {
    // 默认：底层是 deque
    stack<int> s1;

    // 指定底层为 vector
    stack<int, vector<int>> s2;

    // 指定底层为 list
    stack<int, list<int>> s3;

    // 三种用法完全一样
    s1.push(10);
    s2.push(10);
    s3.push(10);

    cout << s1.top() << endl;   // 10
    cout << s2.top() << endl;   // 10
    cout << s3.top() << endl;   // 10

    return 0;
}
```

### 5.3 底层容器的选择

```cpp
/*
    可以作为 stack 底层容器的条件：
    必须支持 back(), push_back(), pop_back(), empty(), size()
    满足条件的有：deque（默认）, vector, list

    选择建议：
    - deque（默认）：头尾操作都高效，内存分段，适合大多数场景
    - vector：连续内存，缓存友好，但扩容时需要复制
    - list：每次插入删除都是O(1)，但内存不连续
    - 一般不需要改，默认的 deque 就很好
*/
```

---

## 六、栈的应用场景

### 6.1 括号匹配

判断一个字符串中的括号是否匹配。

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;

bool isValid(string s) {
    stack<char> stk;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        // 遇到左括号，压入栈
        if (c == '(' || c == '[' || c == '{') {
            stk.push(c);
        }
        // 遇到右括号，检查栈顶
        else if (c == ')' || c == ']' || c == '}') {
            if (stk.empty()) {
                return false;   // 没有对应的左括号
            }

            char top = stk.top();
            stk.pop();

            // 检查是否匹配
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) {
                return false;   // 括号类型不匹配
            }
        }
    }

    // 栈为空说明所有括号都匹配了
    return stk.empty();
}

int main() {
    cout << isValid("()[]{}") << endl;      // 1 (true)
    cout << isValid("([{}])") << endl;      // 1 (true)
    cout << isValid("(]") << endl;          // 0 (false)
    cout << isValid("([)]") << endl;        // 0 (false)
    cout << isValid("{[]}") << endl;        // 1 (true)
    cout << isValid("(") << endl;           // 0 (false)

    return 0;
}
```

**工作流程：**
```
以 "([{}])" 为例：

步骤1: 遇到 ( -> 压栈    栈：(
步骤2: 遇到 [ -> 压栈    栈：( [
步骤3: 遇到 { -> 压栈    栈：( [ {
步骤4: 遇到 } -> 匹配栈顶 { -> 弹栈    栈：( [
步骤5: 遇到 ] -> 匹配栈顶 [ -> 弹栈    栈：(
步骤6: 遇到 ) -> 匹配栈顶 ( -> 弹栈    栈：空

栈为空，匹配成功！
```

### 6.2 简单的表达式求值

用栈实现后缀表达式（逆波兰表达式）求值。

```cpp
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// 后缀表达式求值
// 后缀表达式：操作符在操作数后面，如 "3 4 +" 表示 3 + 4
int evalRPN(vector<string>& tokens) {
    stack<int> stk;

    for (int i = 0; i < tokens.size(); i++) {
        string token = tokens[i];

        // 如果是运算符
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            int b = stk.top(); stk.pop();   // 右操作数
            int a = stk.top(); stk.pop();   // 左操作数

            if (token == "+") stk.push(a + b);
            else if (token == "-") stk.push(a - b);
            else if (token == "*") stk.push(a * b);
            else if (token == "/") stk.push(a / b);
        }
        // 如果是数字
        else {
            stk.push(stoi(token));  // stoi：字符串转整数
        }
    }

    return stk.top();
}

int main() {
    // 表达式：(3 + 4) * (5 - 2) = 7 * 3 = 21
    // 后缀形式：3 4 + 5 2 - *
    vector<string> tokens = {"3", "4", "+", "5", "2", "-", "*"};
    cout << "结果：" << evalRPN(tokens) << endl;   // 21

    // 表达式：10 + 2 * 6 = 10 + 12 = 22
    // 后缀形式：10 2 6 * +
    vector<string> tokens2 = {"10", "2", "6", "*", "+"};
    cout << "结果：" << evalRPN(tokens2) << endl;  // 22

    return 0;
}
```

### 6.3 函数调用栈（理解）

```cpp
/*
    函数调用栈是程序运行时的内存管理机制：

    void funcA() {
        funcB();     // 调用funcB时，funcA的现场压栈
    }

    void funcB() {
        funcC();     // 调用funcC时，funcB的现场压栈
    }

    void funcC() {
        // 执行完毕，弹栈，回到funcB
    }

    调用栈状态：
    +--------+
    | funcC  | <- 栈顶（当前执行的函数）
    +--------+
    | funcB  |
    +--------+
    | funcA  |
    +--------+
    | main   |
    +--------+

    这就是为什么递归太深会导致"栈溢出"（Stack Overflow）
    -- 每次递归调用都往栈里压一个帧，栈空间有限。
*/
```

### 6.4 浏览器前进/后退

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;

class Browser {
private:
    stack<string> m_BackStack;     // 后退栈
    stack<string> m_ForwardStack;  // 前进栈
    string m_CurrentPage;          // 当前页面

public:
    // 访问新页面
    void visit(string url) {
        if (!m_CurrentPage.empty()) {
            m_BackStack.push(m_CurrentPage);    // 当前页压入后退栈
        }
        m_CurrentPage = url;
        // 清空前进栈（访问新页面后不能前进了）
        while (!m_ForwardStack.empty()) {
            m_ForwardStack.pop();
        }
        cout << "访问：" << m_CurrentPage << endl;
    }

    // 后退
    void back() {
        if (m_BackStack.empty()) {
            cout << "无法后退" << endl;
            return;
        }
        m_ForwardStack.push(m_CurrentPage);     // 当前页压入前进栈
        m_CurrentPage = m_BackStack.top();      // 从后退栈取出
        m_BackStack.pop();
        cout << "后退到：" << m_CurrentPage << endl;
    }

    // 前进
    void forward() {
        if (m_ForwardStack.empty()) {
            cout << "无法前进" << endl;
            return;
        }
        m_BackStack.push(m_CurrentPage);        // 当前页压入后退栈
        m_CurrentPage = m_ForwardStack.top();   // 从前进栈取出
        m_ForwardStack.pop();
        cout << "前进到：" << m_CurrentPage << endl;
    }
};

int main() {
    Browser browser;

    browser.visit("www.baidu.com");
    browser.visit("www.google.com");
    browser.visit("www.github.com");

    browser.back();         // 后退到 www.google.com
    browser.back();         // 后退到 www.baidu.com
    browser.forward();      // 前进到 www.google.com

    browser.visit("www.stackoverflow.com");   // 访问新页面，前进栈清空
    browser.forward();      // 无法前进（前进栈已清空）

    return 0;
}
```

---

## 七、综合练习

### 7.1 用栈反转字符串

```cpp
#include <iostream>
#include <stack>
#include <string>
using namespace std;

int main() {
    string s = "Hello World";

    stack<char> stk;

    // 所有字符压入栈
    for (int i = 0; i < s.size(); i++) {
        stk.push(s[i]);
    }

    // 依次弹出，就是反转的结果
    string reversed = "";
    while (!stk.empty()) {
        reversed += stk.top();
        stk.pop();
    }

    cout << "原字符串：" << s << endl;          // Hello World
    cout << "反转后：" << reversed << endl;    // dlroW olleH

    return 0;
}
```

### 7.2 十进制转二进制

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main() {
    int num = 42;   // 十进制 42

    stack<int> stk;

    // 除2取余法
    while (num > 0) {
        stk.push(num % 2);     // 余数压栈
        num /= 2;
    }

    // 弹出余数就是二进制表示
    cout << "42 的二进制：";
    while (!stk.empty()) {
        cout << stk.top();
        stk.pop();
    }
    cout << endl;   // 101010

    return 0;
}
```

---

## 八、总结

| 知识点 | 要点 |
|--------|------|
| 栈的特性 | 先进后出（FILO） |
| push | 入栈（压栈） |
| pop | 出栈（弹栈），不返回值 |
| top | 查看栈顶元素 |
| empty | 判断是否为空 |
| size | 返回元素个数 |
| 不能遍历 | stack 没有迭代器，不支持随机访问 |
| 底层容器 | 默认 deque，可选 vector 或 list |
| 应用场景 | 括号匹配、表达式求值、函数调用栈、浏览器前进后退 |
