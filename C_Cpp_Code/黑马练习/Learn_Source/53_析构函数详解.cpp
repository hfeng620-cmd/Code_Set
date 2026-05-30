/*
 * ============================================================================
 * 53_析构函数详解.cpp
 * ============================================================================
 *
 * 本文件详细讲解 C++ 中析构函数(Destructor)的概念和使用。
 * 析构函数是对象销毁时自动调用的特殊成员函数，用于释放资源。
 *
 * 知识点：
 *   1. 析构函数语法：~ClassName()
 *   2. 析构函数的调用时机（作用域结束、delete）
 *   3. 栈对象的析构顺序（后进先出）
 *   4. 资源清理：内存、文件等
 *   5. 编译器提供的默认析构函数
 *   6. RAII 原则
 *
 * 编译命令：g++ -std=c++11 53_析构函数详解.cpp -o 53_destructor
 * ============================================================================
 */

#include <iostream>
#include <cstring>
using namespace std;

// ===========================================================================
// 第一部分：析构函数的基本语法
// ===========================================================================
// 析构函数的特点：
//   1. 函数名是 ~类名
//   2. 没有参数，没有返回值
//   3. 每个类只能有一个析构函数（不能重载）
//   4. 在对象销毁时由编译器自动调用
//   5. 通常用于释放动态分配的内存、关闭文件等资源清理工作

class Simple
{
private:
    string m_name;

public:
    // 构造函数
    Simple(const string &name) : m_name(name)
    {
        cout << "  [构造] " << m_name << endl;
    }

    // 析构函数
    // 特点：无参数、无返回值、不能重载
    ~Simple()
    {
        cout << "  [析构] " << m_name << endl;
    }

    void show() const { cout << "  对象：" << m_name << endl; }
};

// ===========================================================================
// 第二部分：析构函数的调用时机
// ===========================================================================
// 栈对象：离开作用域时自动析构
// 堆对象：调用 delete 时析构（不 delete 就不析构 -> 内存泄漏！）

class Timer
{
private:
    string m_label;

public:
    Timer(const string &label) : m_label(label)
    {
        cout << "  [启动] " << m_label << endl;
    }

    ~Timer()
    {
        cout << "  [销毁] " << m_label << endl;
    }
};

// ===========================================================================
// 第三部分：析构顺序 —— 后进先出（LIFO）
// ===========================================================================
// 栈上的对象析构顺序与构造顺序相反，就像堆盘子一样：
// 先构造的后析构，后构造的先析构。

class Order
{
private:
    int m_id;
    static int s_count;   // 静态计数器，记录创建的对象数量

public:
    Order() : m_id(++s_count)
    {
        cout << "  [构造] 对象 #" << m_id << endl;
    }

    ~Order()
    {
        cout << "  [析构] 对象 #" << m_id << endl;
        s_count--;
    }

    int getId() const { return m_id; }
};

int Order::s_count = 0;

// ===========================================================================
// 第四部分：资源清理 —— 动态数组
// ===========================================================================
// 如果类中使用了 new 分配内存，析构函数必须用 delete 释放。
// 否则会造成内存泄漏。

class DynamicArray
{
private:
    int *m_data;      // 动态分配的数组
    int m_size;       // 数组大小
    string m_name;    // 对象名（便于调试观察）

public:
    // 构造函数：分配内存
    DynamicArray(const string &name, int size) : m_name(name), m_size(size)
    {
        m_data = new int[m_size];  // 在堆上分配数组
        for (int i = 0; i < m_size; i++)
            m_data[i] = 0;
        cout << "  [构造] " << m_name << "：分配了 "
             << m_size << " 个 int（" << m_size * sizeof(int) << " 字节）" << endl;
    }

    // 析构函数：释放内存
    // 这是析构函数最重要的用途之一！
    ~DynamicArray()
    {
        cout << "  [析构] " << m_name << "：释放 " << m_size << " 个 int" << endl;
        delete[] m_data;     // 释放数组必须用 delete[]
        m_data = nullptr;    // 置空指针（好习惯，虽然析构后对象已销毁）
    }

    // 设置值
    void set(int index, int value)
    {
        if (index >= 0 && index < m_size)
            m_data[index] = value;
        else
            cout << "  [错误] 索引越界：" << index << endl;
    }

    // 获取值
    int get(int index) const
    {
        if (index >= 0 && index < m_size)
            return m_data[index];
        return -1;
    }

    // 显示内容
    void show() const
    {
        cout << "  " << m_name << " [";
        for (int i = 0; i < m_size; i++)
        {
            if (i > 0) cout << ", ";
            cout << m_data[i];
        }
        cout << "]" << endl;
    }
};

// ===========================================================================
// 第五部分：资源清理 —— 文件句柄（RAII 模式）
// ===========================================================================
// RAII = Resource Acquisition Is Initialization（资源获取即初始化）
// 核心思想：在构造函数中获取资源，在析构函数中释放资源。
// 利用栈对象自动析构的特性，确保资源一定被释放。

class FileHandle
{
private:
    string m_filename;
    bool m_isOpen;

public:
    // 构造函数：打开资源
    FileHandle(const string &filename) : m_filename(filename), m_isOpen(true)
    {
        cout << "  [打开文件] " << m_filename << endl;
    }

    // 析构函数：关闭资源
    ~FileHandle()
    {
        if (m_isOpen)
        {
            cout << "  [关闭文件] " << m_filename << endl;
            m_isOpen = false;
        }
    }

    void write(const string &content)
    {
        if (m_isOpen)
            cout << "  [写入] " << m_filename << "：" << content << endl;
        else
            cout << "  [错误] 文件未打开！" << endl;
    }
};

// ===========================================================================
// 第六部分：编译器提供的默认析构函数
// ===========================================================================
// 如果我们不写析构函数，编译器会提供一个空的默认析构函数：
//   ~ClassName() {}
// 对于没有动态资源的类，默认析构就够了。
// 但如果类中有 new 分配的资源，必须自定义析构函数！

class NoPointer
{
public:
    int value;
    string name;
    // 不需要自定义析构函数，编译器提供的默认析构（什么都不做）足够了
    // 因为 int 和 string 会在对象销毁时自动清理
};

class HasPointer
{
private:
    int *m_data;
public:
    HasPointer(int val)
    {
        m_data = new int(val);   // 分配了堆内存
    }
    // 必须自定义析构函数！否则 m_data 指向的内存永远不会被释放
    ~HasPointer()
    {
        delete m_data;
        m_data = nullptr;
    }
};

// ===========================================================================
// 第七部分：堆对象与 delete
// ===========================================================================
// 堆上创建的对象不会自动析构，必须手动 delete。
// 如果不 delete，析构函数永远不会被调用，导致资源泄漏。

class HeapDemo
{
private:
    string m_name;

public:
    HeapDemo(const string &name) : m_name(name)
    {
        cout << "  [构造] " << m_name << "（堆对象）" << endl;
    }

    ~HeapDemo()
    {
        cout << "  [析构] " << m_name << "（堆对象）" << endl;
    }
};

// ===========================================================================
// 主函数
// ===========================================================================
int main()
{
    cout << "============================================" << endl;
    cout << "            C++ 析构函数详解" << endl;
    cout << "============================================" << endl;

    // ---- 第一部分：基本语法 ----
    cout << "\n===== 第一部分：析构函数基本语法 =====" << endl;
    {
        Simple s1("对象A");
        Simple s2("对象B");
        s1.show();
        s2.show();
        cout << "  --- 即将离开作用域 ---" << endl;
    }
    // s2 先析构，s1 后析构（后进先出）
    cout << "  --- 已离开作用域 ---" << endl;

    // ---- 第二部分：调用时机 ----
    cout << "\n===== 第二部分：析构函数调用时机 =====" << endl;

    // 栈对象：离开作用域时析构
    cout << "--- 栈对象 ---" << endl;
    {
        Timer t1("栈对象Timer");
    }
    cout << "  栈对象已自动析构\n" << endl;

    // 堆对象：调用 delete 时析构
    cout << "--- 堆对象 ---" << endl;
    Timer *pt = new Timer("堆对象Timer");
    cout << "  堆对象已创建，但未 delete，析构函数不会被调用" << endl;
    delete pt;   // 手动 delete，触发析构
    pt = nullptr;
    cout << "  堆对象已 delete 并析构" << endl;

    // 警告：不 delete 堆对象会导致内存泄漏
    // Timer *leak = new Timer("泄漏的对象");
    // 如果不 delete leak，析构函数永远不会被调用

    // ---- 第三部分：析构顺序 ----
    cout << "\n===== 第三部分：析构顺序（后进先出） =====" << endl;
    {
        Order o1;  // #1
        Order o2;  // #2
        Order o3;  // #3
        cout << "  --- 当前有 " << o3.getId() << " 个对象 ---" << endl;
        cout << "  --- 即将离开作用域 ---" << endl;
    }
    // 析构顺序：#3 -> #2 -> #1（与构造顺序相反）

    // ---- 第四部分：资源清理 ----
    cout << "\n===== 第四部分：动态数组资源清理 =====" << endl;
    {
        DynamicArray arr("数组A", 5);
        arr.set(0, 10);
        arr.set(1, 20);
        arr.set(2, 30);
        arr.show();
        cout << "  --- 即将离开作用域，析构函数自动释放内存 ---" << endl;
    }

    // ---- 第五部分：RAII 模式 ----
    cout << "\n===== 第五部分：RAII 文件句柄 =====" << endl;
    {
        FileHandle fh("data.txt");
        fh.write("Hello, World!");
        fh.write("第二行数据");
        cout << "  --- 离开作用域时自动关闭文件 ---" << endl;
    }

    // RAII 在 C++ 标准库中的应用：
    //   std::unique_ptr  -> 智能指针，析构时自动 delete
    //   std::lock_guard  -> 互斥锁，析构时自动解锁
    //   std::fstream     -> 文件流，析构时自动关闭文件

    // ---- 第六部分：默认析构函数 ----
    cout << "\n===== 第六部分：何时需要自定义析构 =====" << endl;
    cout << "  不需要自定义析构的情况：" << endl;
    cout << "    - 类中只有基本类型成员（int, double 等）" << endl;
    cout << "    - 类中有 string、vector 等自动管理资源的成员" << endl;
    cout << "  需要自定义析构的情况：" << endl;
    cout << "    - 类中使用了 new 分配堆内存" << endl;
    cout << "    - 类中打开了文件、网络连接等需要手动关闭的资源" << endl;
    cout << "    - 类中获取了互斥锁等需要手动释放的资源" << endl;

    // ---- 第七部分：堆对象的注意事项 ----
    cout << "\n===== 第七部分：堆对象与 delete =====" << endl;
    {
        // 创建堆对象
        HeapDemo *p1 = new HeapDemo("堆对象A");
        HeapDemo *p2 = new HeapDemo("堆对象B");

        // 使用对象
        cout << "  堆对象已创建" << endl;

        // 必须手动 delete
        delete p1;
        p1 = nullptr;
        cout << "  堆对象A 已释放" << endl;

        // 如果忘记 delete p2，会导致内存泄漏
        delete p2;
        p2 = nullptr;
        cout << "  堆对象B 已释放" << endl;
    }

    // 常见错误总结：
    cout << "\n===== 常见错误总结 =====" << endl;
    cout << "  1. new 了内存忘记在析构函数中 delete -> 内存泄漏" << endl;
    cout << "  2. 用 delete 而不是 delete[] 释放数组 -> 未定义行为" << endl;
    cout << "  3. 堆对象不调用 delete -> 析构函数不执行" << endl;
    cout << "  4. 同一块内存 delete 两次 -> 双重释放崩溃" << endl;
    cout << "  5. 有指针成员但不自定义析构 -> 编译器默认析构不释放堆内存" << endl;

    cout << endl;
    return 0;
}
