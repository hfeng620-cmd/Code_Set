# const修饰指针详解

## 一、为什么需要用const修饰指针

> **const 是"常量"的意思，用来修饰变量使其不可被修改。**
> 当 const 和指针一起使用时，可以限制"能改什么、不能改什么"。

使用 const 的好处：
```
1. 防止意外修改数据（安全性）
2. 明确告诉使用者"这个数据不应该被修改"（可读性）
3. 让编译器帮你检查错误（编译期保护）
4. const 引用可以接收更多类型的参数（灵活性）
```

---

## 二、三种const修饰指针的情况

### 2.1 情况一：常量指针（const int *p）

> **常量指针：指针指向的值不能改，但指向可以改。**
> 记忆：`const int *p` = "p is a pointer to const int" = p指向一个常量int

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // 常量指针：const修饰的是 int（即*p），所以 *p 不能改
    const int *p = &a;

    // *p = 100;     // 错误！不能通过p修改指向的值
    // 这行代码会报错：assignment of read-only location '* p'

    p = &b;          // OK！p本身的值（地址）可以改
    cout << "*p = " << *p << endl;   // 输出 20

    // 虽然不能通过 p 修改值，但可以直接修改 a 和 b
    a = 100;         // OK！a不是const
    b = 200;         // OK！b不是const
    cout << "*p = " << *p << endl;   // 输出 200

    return 0;
}
```

**图解：**
```
const int *p = &a;

  a (= 10)          b (= 20)
  +--------+        +--------+
  |  10    |        |  20    |
  +--------+        +--------+
       ^
       |
  +--------+
  |   p    |   ----> 可以改为指向 b
  +--------+

  *p 不能改（不能通过 p 修改 a 或 b 的值）
  p 可以改（可以指向其他变量）
```

### 2.2 情况二：指针常量（int *const p）

> **指针常量：指针的指向不能改，但指向的值可以改。**
> 记忆：`int *const p` = "p is a const pointer to int" = p是一个常量指针

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // 指针常量：const修饰的是 p，所以 p 的指向不能改
    int *const p = &a;

    *p = 100;        // OK！可以通过 p 修改指向的值
    cout << "a = " << a << endl;   // 输出 100

    // p = &b;        // 错误！p的指向不能改
    // 这行代码会报错：assignment of read-only variable 'p'

    return 0;
}
```

**图解：**
```
int *const p = &a;

  a (= 100)         b (= 20)
  +--------+        +--------+
  |  100   |        |  20    |
  +--------+        +--------+
       ^
       | （不能改为指向b）
  +--------+
  |   p    |   ----> 指向固定，不能改
  +--------+

  *p 可以改（可以通过 p 修改 a 的值）
  p 不能改（不能让 p 指向其他变量）
```

### 2.3 情况三：都const（const int *const p）

> **既不能改指向，也不能改指向的值。**
> 最严格的限制。

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // 都const：指向不能改，值也不能改
    const int *const p = &a;

    // *p = 100;     // 错误！不能修改值
    // p = &b;       // 错误！不能修改指向

    // 只能读取
    cout << "*p = " << *p << endl;   // 输出 10

    // 但可以直接修改 a
    a = 50;          // OK！a不是const
    cout << "*p = " << *p << endl;   // 输出 50

    return 0;
}
```

---

## 三、记忆技巧

### 3.1 看 const 在 * 的左边还是右边

```
记忆口诀：const 离谁近，谁就不能改

const int *p;        -> const在*左边 -> 值不能改（*p不能改）
                       等价于 int const *p;

int *const p;        -> const在*右边 -> 指向不能改（p不能改）

const int *const p;  -> 两边都有const -> 都不能改
```

### 3.2 读法技巧

```cpp
// 从右往左读

const int *p;        // p is a pointer to const int
                     // p 是一个指向"常量int"的指针
                     // 指向的int值不能改

int *const p;        // p is a const pointer to int
                     // p 是一个"常量指针"指向int
                     // p本身（地址）不能改

const int *const p;  // p is a const pointer to const int
                     // p 是一个"常量指针"指向"常量int"
                     // 都不能改
```

### 3.3 速查表

```cpp
/*
+-----------------------+-------------------+-------------------+
| 声明                  | 能否改 *p（值）    | 能否改 p（指向）   |
+-----------------------+-------------------+-------------------+
| const int *p          | 不能              | 能                |
| int const *p          | 不能              | 能                |
| int *const p          | 能                | 不能              |
| const int *const p    | 不能              | 不能              |
+-----------------------+-------------------+-------------------+

简单记忆：
  const 在 * 左边 -> 值不能改
  const 在 * 右边 -> 指向不能改
  两边都有       -> 都不能改
*/
```

---

## 四、三种情况的完整对比示例

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10, b = 20;

    // ====== 情况1：常量指针（值不能改，指向可改）======
    cout << "===== 常量指针 =====" << endl;
    const int *p1 = &a;
    // *p1 = 100;      // 错误！值不能改
    p1 = &b;           // OK！指向可以改
    cout << "*p1 = " << *p1 << endl;  // 20

    // ====== 情况2：指针常量（值可改，指向不能改）======
    cout << "===== 指针常量 =====" << endl;
    int *const p2 = &a;
    *p2 = 100;         // OK！值可以改
    // p2 = &b;        // 错误！指向不能改
    cout << "a = " << a << endl;     // 100

    // ====== 情况3：都const（都不能改）======
    cout << "===== 都const =====" << endl;
    const int *const p3 = &a;
    // *p3 = 200;      // 错误！值不能改
    // p3 = &b;        // 错误！指向不能改
    cout << "*p3 = " << *p3 << endl; // 100

    return 0;
}
```

---

## 五、const在函数参数中的应用

### 5.1 保护传入的数据不被修改

```cpp
#include <iostream>
using namespace std;

// 用 const 保护指针参数，确保函数不会修改传入的数据
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
        // arr[i] = 0;   // 错误！const保护，不能修改
    }
    cout << endl;
}

// 用 const 保护字符串
void printString(const char *str) {
    // str[0] = 'X';    // 错误！不能修改
    cout << str << endl;
}

int main() {
    int nums[] = {1, 2, 3, 4, 5};
    printArray(nums, 5);

    printString("Hello World");

    return 0;
}
```

### 5.2 const 引用参数

```cpp
#include <iostream>
#include <string>
using namespace std;

// const 引用：既能避免拷贝（高效），又能保护数据（安全）
void greet(const string &name) {
    // name = "hacked";   // 错误！不能修改
    cout << "你好，" << name << "！" << endl;
}

// 对比：值传递（会拷贝，开销大）
void greetByValue(string name) {
    cout << "你好，" << name << "！" << endl;
}

int main() {
    string s = "小明";
    greet(s);            // 高效且安全
    greetByValue(s);     // 安全但有拷贝开销

    return 0;
}
```

---

## 六、const 与 非const 的转换规则

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;

    // ====== 规则1：非const可以赋给const（安全的，放宽限制）======
    int *p1 = &a;
    const int *p2 = p1;    // OK！非const指针 -> const指针
    // 这是安全的：本来可以改，现在限制为不能改

    // ====== 规则2：const不能直接赋给非const（危险的）======
    const int *p3 = &a;
    // int *p4 = p3;        // 错误！const指针 -> 非const指针
    // 如果允许，就能通过 p4 修改本来被保护的值

    // ====== 规则3：强制转换（不推荐，但语法上可行）======
    int *p5 = const_cast<int*>(p3);  // 去掉const限定
    *p5 = 100;    // 这样可以修改，但要非常小心！

    cout << "a = " << a << endl;  // 100

    return 0;
}
```

---

## 七、为什么在编程中大量使用const

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
使用 const 的理由：

1. 安全性：防止意外修改
   - 函数参数加const，保证不会误改传入的数据
   - 遍历时不修改元素

2. 代码自文档化：
   - 看到 const 就知道"这个不会被修改"
   - 减少阅读代码的心智负担

3. 编译器优化：
   - 编译器知道 const 值不会变，可以做更多优化

4. 能接受更多类型的参数：
   - const引用可以接受临时对象、字面量
   - 非const引用不能接受临时对象
*/

// 好的做法：参数加 const
double calculateDistance(const double &x1, const double &y1,
                         const double &x2, const double &y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return dx * dx + dy * dy;  // 简化版，不开根号
}

// 不好的做法：参数没加 const
void badPrint(string &s) {
    // 这个函数不能接受字面量：
    // badPrint("hello");   // 错误！字面量不能绑定到非const引用
    cout << s << endl;
}

// 好的做法：加 const
void goodPrint(const string &s) {
    cout << s << endl;
}

int main() {
    goodPrint("hello");    // OK！const引用可以接受字面量

    return 0;
}
```

---

## 八、常见错误与注意事项

```cpp
#include <iostream>
using namespace std;

int main() {
    int a = 10;
    int b = 20;

    // ====== 错误1：试图通过常量指针修改值 ======
    const int *p1 = &a;
    // *p1 = 100;       // 编译错误！

    // ====== 错误2：试图改变指针常量的指向 ======
    int *const p2 = &a;
    // p2 = &b;         // 编译错误！

    // ====== 错误3：const指针赋给非const指针 ======
    const int *p3 = &a;
    // int *p4 = p3;    // 编译错误！

    // ====== 注意：const只限制通过指针的修改 ======
    const int *p5 = &a;
    a = 100;            // OK！直接修改a是允许的
    cout << "*p5 = " << *p5 << endl;  // 100

    // ====== 注意：指针常量必须在声明时初始化 ======
    // int *const p6;   // 错误！指针常量必须在声明时就指定指向
    int *const p7 = &a;  // OK

    return 0;
}
```

---

## 九、综合对比示例

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 10, y = 20;

    // 完全对比：三种情况
    cout << "===== 常量指针（const在*左边）=====" << endl;
    const int *p1 = &x;
    // *p1 = 100;    // X 不能改值
    p1 = &y;         // OK 能改指向
    cout << "*p1=" << *p1 << endl;    // 20

    cout << "===== 指针常量（const在*右边）=====" << endl;
    int *const p2 = &x;
    *p2 = 100;       // OK 能改值
    // p2 = &y;      // X 不能改指向
    cout << "x=" << x << endl;        // 100

    cout << "===== 都const =====" << endl;
    const int *const p3 = &x;
    // *p3 = 200;    // X 不能改值
    // p3 = &y;      // X 不能改指向
    cout << "*p3=" << *p3 << endl;    // 100

    return 0;
}
```

---

## 十、注意事项总结

| 序号 | 知识点 | 说明 |
|------|--------|------|
| 1 | `const int *p` | 常量指针：值不能改，指向可以改 |
| 2 | `int *const p` | 指针常量：指向不能改，值可以改 |
| 3 | `const int *const p` | 都不能改 |
| 4 | 记忆技巧 | const离谁近，谁就不能改；或从右往左读 |
| 5 | const在*左边 | 限制的是值（*p） |
| 6 | const在*右边 | 限制的是指针本身（p） |
| 7 | 指针常量必须初始化 | 声明时就要指定指向 |
| 8 | 函数参数加const | 保护数据不被修改，且能接受更多类型 |

---

## 十一、练习题

1. 声明一个常量指针，让它分别指向两个不同的 int 变量，验证值不能改但指向可以改。
2. 声明一个指针常量，通过它修改指向变量的值，验证指向不能改。
3. 写一个函数 `void printInfo(const char *name, const int &age)` 并调用它。

```cpp
// 练习1 参考答案：
#include <iostream>
using namespace std;

int main() {
    int a = 10, b = 20;
    const int *p = &a;
    cout << "*p = " << *p << endl;   // 10
    // *p = 100;    // 错误！
    p = &b;
    cout << "*p = " << *p << endl;   // 20
    return 0;
}

// 练习2 参考答案：
int main() {
    int a = 10, b = 20;
    int *const p = &a;
    *p = 100;        // OK
    // p = &b;       // 错误！
    cout << "a = " << a << endl;   // 100
    return 0;
}

// 练习3 参考答案：
void printInfo(const char *name, const int &age) {
    // name[0] = 'X';   // 错误！
    // age = 100;        // 错误！
    cout << "姓名：" << name << "，年龄：" << age << endl;
}

int main() {
    printInfo("张三", 18);
    return 0;
}
```
