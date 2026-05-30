# new 和 delete 详解

## 一、为什么需要堆区

栈区有以下限制：
1. **空间有限**：一般只有 1~8 MB，大数组会栈溢出
2. **生命周期受限**：局部变量函数结束就销毁，无法跨函数使用
3. **大小必须编译时确定**：不能动态决定数组大小

堆区解决了这些问题：
- 空间大（取决于可用内存）
- 生命周期由程序员控制
- 可以在运行时动态决定大小

---

## 二、new 的基本用法

### 2.1 new 一个基本类型

```cpp
#include <iostream>
using namespace std;

int main()
{
    // new int(10)：在堆区创建一个 int，初始值为 10
    // 返回的是指针，必须用指针接收
    int *p = new int(10);
    cout << "*p = " << *p << endl;  // 10

    // 通过指针修改值
    *p = 20;
    cout << "*p = " << *p << endl;  // 20

    // 用完必须释放
    delete p;
    p = nullptr;

    return 0;
}
```

### 2.2 new 一个数组

```cpp
#include <iostream>
using namespace std;

int main()
{
    // new int[5]：在堆区创建一个长度为 5 的 int 数组
    int *arr = new int[5];

    for (int i = 0; i < 5; i++)
    {
        arr[i] = (i + 1) * 10;
    }

    for (int i = 0; i < 5; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;  // 10 20 30 40 50

    // 释放数组必须用 delete[]
    delete[] arr;
    arr = nullptr;

    return 0;
}
```

### 2.3 new 数组并初始化

```cpp
#include <iostream>
using namespace std;

int main()
{
    // C++11 用花括号初始化
    int *arr = new int[5]{10, 20, 30, 40, 50};

    for (int i = 0; i < 5; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
    arr = nullptr;

    return 0;
}
```

---

## 三、new int 和 new int() 的区别

这是一个容易混淆的点：

```cpp
#include <iostream>
using namespace std;

int main()
{
    // new int    ：不初始化，值是随机的（未定义）
    int *p1 = new int;
    cout << "new int:    " << *p1 << endl;  // 随机值

    // new int()  ：初始化为 0
    int *p2 = new int();
    cout << "new int():  " << *p2 << endl;  // 0

    // new int(10)：初始化为指定值
    int *p3 = new int(10);
    cout << "new int(10): " << *p3 << endl;  // 10

    // 数组也有区别：
    int *arr1 = new int[3];       // 随机值
    int *arr2 = new int[3]();     // 全部为 0
    int *arr3 = new int[3]{1,2,3}; // 指定值

    delete p1; delete p2; delete p3;
    delete[] arr1; delete[] arr2; delete[] arr3;

    return 0;
}
```

| 写法 | 初始值 |
|------|--------|
| `new int` | 未定义（随机值） |
| `new int()` | 0 |
| `new int(10)` | 10 |
| `new int[5]` | 未定义（随机值） |
| `new int[5]()` | 全部为 0 |
| `new int[5]{1,2,3,4,5}` | 指定值 |

---

## 四、delete 的基本用法

### 4.1 delete 单个变量

```cpp
int *p = new int(10);
// ... 使用 p ...
delete p;
p = nullptr;
```

### 4.2 delete 数组

```cpp
int *arr = new int[5]{1, 2, 3, 4, 5};
// ... 使用 arr ...
delete[] arr;
arr = nullptr;
```

### 4.3 new 和 delete 必须配对

| 分配方式 | 释放方式 |
|---------|---------|
| `new` | `delete` |
| `new[]` | `delete[]` |

错误配对会导致内存泄漏或程序崩溃。

---

## 五、堆区的生命周期

堆区数据的生命周期：从 `new` 开始，到 `delete` 结束。

```cpp
#include <iostream>
using namespace std;

int* createInt(int value)
{
    int *p = new int(value);  // 在堆区创建
    return p;                  // 返回指针，堆区数据不会销毁
}

void useInt(int *p)
{
    cout << "值为: " << *p << endl;
    // 函数结束，p 被销毁，但堆区数据还在
}

void deleteInt(int *p)
{
    delete p;
    cout << "已释放" << endl;
}

int main()
{
    int *p = createInt(100);
    useInt(p);
    deleteInt(p);
    p = nullptr;
    return 0;
}
```

---

## 六、内存泄漏

### 6.1 什么是内存泄漏

内存泄漏是指程序用 `new` 分配了内存，但**忘记 `delete`**，
导致那块内存再也无法被访问也无法被释放。

```cpp
void leak()
{
    int *p = new int(100);
    // 忘记 delete p;
    // 函数结束后 p 被销毁，但堆区的内存没有释放
}
```

### 6.2 常见的内存泄漏场景

```cpp
// 场景一：提前 return 导致泄漏
void earlyReturn()
{
    int *p = new int(10);
    if (*p > 5)
    {
        return;   // 没有 delete！
    }
    delete p;
}

// 正确写法：
void earlyReturnFixed()
{
    int *p = new int(10);
    if (*p > 5)
    {
        delete p;
        return;
    }
    delete p;
}

// 场景二：循环中反复 new 不 delete
void loopLeak()
{
    for (int i = 0; i < 1000; i++)
    {
        int *p = new int(i);
        // 从不释放
    }
}

// 正确写法：
void loopFixed()
{
    for (int i = 0; i < 1000; i++)
    {
        int *p = new int(i);
        // ... 使用 p ...
        delete p;
        p = nullptr;
    }
}
```

### 6.3 如何避免内存泄漏

1. 养成好习惯，new 和 delete 成对写
2. 使用智能指针（推荐，后面会学到）
3. 尽量使用栈区变量和标准库容器

---

## 七、常见错误总结

| 错误 | 说明 |
|------|------|
| 忘记释放 | new 了没 delete，内存泄漏 |
| 释放后继续使用 | 悬空指针，未定义行为 |
| 重复释放 | 程序崩溃 |
| delete 和 delete[] 混用 | 内存泄漏或崩溃 |
| 对非 new 指针 delete | 未定义行为 |
| 数组越界 | 未定义行为 |

---

## 八、综合案例

```cpp
#include <iostream>
using namespace std;

int* createArray(int size)
{
    int *arr = new int[size]();  // 全部初始化为 0
    return arr;
}

void inputArray(int *arr, int size)
{
    cout << "请输入 " << size << " 个整数: ";
    for (int i = 0; i < size; i++)
        cin >> arr[i];
}

void printArray(const int *arr, int size)
{
    cout << "数组内容: ";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

int main()
{
    int n;
    cout << "请输入数组大小: ";
    cin >> n;

    int *arr = createArray(n);
    inputArray(arr, n);
    printArray(arr, n);

    delete[] arr;
    arr = nullptr;

    return 0;
}
```

---

## 九、动态创建二维数组

```cpp
#include <iostream>
using namespace std;

int main()
{
    int rows = 3, cols = 4;

    int **arr = new int*[rows];
    for (int i = 0; i < rows; i++)
        arr[i] = new int[cols]();

    int count = 1;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            arr[i][j] = count++;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            cout << arr[i][j] << "\t";
        cout << endl;
    }

    // 释放（先释放每一行，再释放指针数组）
    for (int i = 0; i < rows; i++)
        delete[] arr[i];
    delete[] arr;
    arr = nullptr;

    return 0;
}
```

---

## 总结

| 要点 | 说明 |
|------|------|
| 为什么用堆区 | 空间大、可动态分配、生命周期可控 |
| new 基本类型 | `new int(10)` 返回指针 |
| new 数组 | `new int[5]` 返回指针 |
| new int vs new int() | 前者不初始化，后者初始化为 0 |
| delete | 释放 `new` 分配的内存 |
| delete[] | 释放 `new[]` 分配的数组 |
| 必须配对 | `new` 对 `delete`，`new[]` 对 `delete[]` |
| 内存泄漏 | `new` 了没 `delete`，内存无法回收 |
| 避免泄漏 | 养成习惯、用智能指针、优先用容器 |
