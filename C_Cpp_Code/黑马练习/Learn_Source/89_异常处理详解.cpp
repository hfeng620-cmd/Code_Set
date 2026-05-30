/*
 * 89_异常处理详解.cpp
 * 本文件演示 C++ 异常处理机制
 * 包括: try/catch/throw, 按类型捕获, catch(...), 标准异常类,
 *       自定义异常类, 异常层次结构, noexcept, 栈展开, 最佳实践
 */

#include <iostream>
#include <stdexcept>    // 标准异常类
#include <string>
#include <vector>
#include <memory>       // unique_ptr
#include <exception>
using namespace std;

// ============ 自定义异常类 ============

// 基础自定义异常类（继承 std::exception）
class MyException : public exception {
public:
    MyException(const string& msg) : message(msg) {}

    // 重写 what() 虚函数，返回错误信息
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    string message;
};

// 带错误码的异常类
class ErrorCodeException : public MyException {
public:
    ErrorCodeException(const string& msg, int code)
        : MyException(msg), errorCode(code) {}

    int getErrorCode() const { return errorCode; }

private:
    int errorCode;
}

;
// 用于演示栈展开的类
class Resource {
public:
    Resource(const string& n) : name(n) {
        cout << "  [Resource] 获取资源: " << name << endl;
    }
    ~Resource() {
        cout << "  [Resource] 释放资源: " << name << endl;
    }
private:
    string name;
};

// 数组越界异常
class OutOfRangeException : public exception {
public:
    OutOfRangeException(int idx, int sz)
        : index(idx), size(sz) {
        message = "索引 " + to_string(index) + " 超出范围 [0, " + to_string(size) + ")";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    int index;
    int size;
    string message;
};

// ============ 演示函数 ============

// 可能抛出异常的函数
int divide(int a, int b) {
    if (b == 0) {
        throw runtime_error("除数不能为零!");
    }
    return a / b;
}

// 多层调用中的异常传播
void level3() {
    cout << "  进入 level3" << endl;
    Resource r3("level3资源");
    throw MyException("在 level3 中抛出异常");
    cout << "  离开 level3 (不会执行)" << endl;
}

void level2() {
    cout << "  进入 level2" << endl;
    Resource r2("level2资源");
    level3();
    cout << "  离开 level2 (不会执行)" << endl;
}

void level1() {
    cout << "  进入 level1" << endl;
    Resource r1("level1资源");
    level2();
    cout << "  离开 level1 (不会执行)" << endl;
}

int main() {
    cout << "========== 1. try/catch/throw 基本用法 ==========" << endl;
    // try: 包含可能抛出异常的代码
    // throw: 抛出异常
    // catch: 捕获并处理异常

    try {
        int result = divide(10, 0);
        cout << "结果: " << result << endl;  // 不会执行
    }
    catch (const runtime_error& e) {
        // 捕获 runtime_error 类型的异常
        cout << "捕获异常: " << e.what() << endl;
    }

    // 正常情况不会抛出异常
    try {
        int result = divide(10, 2);
        cout << "正常结果: " << result << endl;
    }
    catch (const runtime_error& e) {
        cout << "捕获异常: " << e.what() << endl;
    }

    cout << "\n========== 2. 多个 catch 块 ==========" << endl;
    // 可以有多个 catch 块，按顺序匹配
    // 基类的 catch 应该放在派生类之后
    for (int i = 0; i < 4; ++i) {
        try {
            switch (i) {
                case 0: throw 42;                    // int
                case 1: throw 3.14;                  // double
                case 2: throw string("字符串异常");   // string
                case 3: throw runtime_error("运行时错误"); // runtime_error
            }
        }
        catch (int e) {
            cout << "捕获 int 异常: " << e << endl;
        }
        catch (double e) {
            cout << "捕获 double 异常: " << e << endl;
        }
        catch (const string& e) {
            cout << "捕获 string 异常: " << e << endl;
        }
        catch (const exception& e) {
            // 基类 catch 放在最后，捕获所有标准异常
            cout << "捕获 exception: " << e.what() << endl;
        }
    }

    cout << "\n========== 3. catch(...) 捕获所有异常 ==========" << endl;
    // catch(...) 可以捕获任何类型的异常
    // 通常用于最后的兜底处理
    try {
        throw "未知类型异常";  // C 风格字符串
    }
    catch (...) {
        cout << "catch(...) 捕获了异常（不知道具体类型）" << endl;
        // 无法获取异常信息
        // 可以记录日志后重新抛出
        // throw;  // 重新抛出当前异常
    }

    // 实际应用: 资源清理 + catch(...)
    try {
        Resource r("try块中的资源");
        throw 1;  // 某些异常
    }
    catch (...) {
        cout << "捕获异常，资源已通过 RAII 释放" << endl;
        // 不要在这里吞掉异常，至少记录日志
    }

    cout << "\n========== 4. 标准异常类层次 ==========" << endl;
    // std::exception 是所有标准异常的基类
    // 主要派生类:
    //   logic_error: 程序逻辑错误（可以在编程时避免）
    //     - invalid_argument: 无效参数
    //     - domain_error: 域错误
    //     - length_error: 长度超出限制
    //     - out_of_range: 超出范围
    //   runtime_error: 运行时错误（无法在编程时避免）
    //     - range_error: 范围错误
    //     - overflow_error: 上溢
    //     - underflow_error: 下溢

    // invalid_argument 示例
    try {
        int age = -5;
        if (age < 0) {
            throw invalid_argument("年龄不能为负数: " + to_string(age));
        }
    }
    catch (const invalid_argument& e) {
        cout << "invalid_argument: " << e.what() << endl;
    }

    // out_of_range 示例
    try {
        vector<int> v = {1, 2, 3};
        v.at(10);  // at() 会检查越界并抛出 out_of_range
    }
    catch (const out_of_range& e) {
        cout << "out_of_range: " << e.what() << endl;
    }

    // overflow_error 示例
    try {
        throw overflow_error("数值溢出");
    }
    catch (const overflow_error& e) {
        cout << "overflow_error: " << e.what() << endl;
    }

    cout << "\n========== 5. 自定义异常类 ==========" << endl;
    // 使用自定义异常类
    try {
        throw MyException("这是自定义异常");
    }
    catch (const MyException& e) {
        cout << "自定义异常: " << e.what() << endl;
    }

    // 带错误码的异常
    try {
        throw ErrorCodeException("文件操作失败", 404);
    }
    catch (const ErrorCodeException& e) {
        cout << "错误码异常: " << e.what()
             << " (错误码: " << e.getErrorCode() << ")" << endl;
    }

    // 数组越界异常
    try {
        vector<int> arr = {1, 2, 3, 4, 5};
        int idx = 10;
        if (idx < 0 || idx >= static_cast<int>(arr.size())) {
            throw OutOfRangeException(idx, arr.size());
        }
    }
    catch (const OutOfRangeException& e) {
        cout << "越界异常: " << e.what() << endl;
    }

    cout << "\n========== 6. 栈展开（Stack Unwinding） ==========" << endl;
    // 当异常被抛出时，程序会沿着调用栈向上查找匹配的 catch 块
    // 在这个过程中，所有已构造的局部对象都会被析构（RAII）
    cout << "调用 level1 -> level2 -> level3:" << endl;
    try {
        level1();
    }
    catch (const MyException& e) {
        cout << "在 main 中捕获异常: " << e.what() << endl;
    }
    cout << "注意资源的释放顺序（与构造顺序相反）" << endl;

    cout << "\n========== 7. noexcept 关键字 ==========" << endl;
    // noexcept: 声明函数不会抛出异常
    // 如果 noexcept 函数抛出了异常，程序会直接调用 terminate()
    // 用于优化: 编译器可以对 noexcept 函数做更多优化

    // 声明不抛出异常的函数
    auto safeFunc = [](int a, int b) noexcept -> int {
        // 承诺: 这个函数不会抛出异常
        // 如果真的抛出异常，程序会终止
        return a + b;
    };

    cout << "noexcept 函数结果: " << safeFunc(3, 4) << endl;

    // 条件 noexcept
    auto conditionalFunc = [](int val) noexcept(val > 0) -> int {
        // 只有当 val > 0 时才承诺不抛异常
        if (val <= 0) {
            throw runtime_error("值必须为正数");
        }
        return val * 2;
    };

    try {
        cout << "条件 noexcept (正数): " << conditionalFunc(5) << endl;
        cout << "条件 noexcept (负数): ";
        cout << conditionalFunc(-1) << endl;
    }
    catch (const runtime_error& e) {
        cout << "捕获异常: " << e.what() << endl;
    }

    // 查询函数是否是 noexcept
    cout << "safeFunc 是否 noexcept: " << noexcept(safeFunc(1, 2)) << endl;

    cout << "\n========== 8. 异常与资源管理（RAII） ==========" << endl;
    // RAII 是 C++ 中管理资源的最佳方式
    // 资源（文件、内存、锁等）在构造函数中获取，在析构函数中释放
    // 即使发生异常，析构函数也会被调用，确保资源不泄漏

    try {
        cout << "--- 开始 RAII 演示 ---" << endl;

        // 使用智能指针管理动态内存
        auto ptr = make_unique<int[]>(100);
        ptr[0] = 42;
        cout << "智能指针创建成功, 值: " << ptr[0] << endl;

        // 模拟异常
        throw runtime_error("模拟异常");

        // unique_ptr 会在栈展开时自动释放内存
    }
    catch (const exception& e) {
        cout << "异常处理: " << e.what() << endl;
        cout << "unique_ptr 已自动释放内存" << endl;
    }

    cout << "\n========== 9. 重新抛出异常 ==========" << endl;
    // 在 catch 块中可以使用 throw 重新抛出当前异常
    // 常用于: 记录日志后继续传播异常
    try {
        try {
            throw runtime_error("原始异常");
        }
        catch (const runtime_error& e) {
            cout << "内层 catch: 记录日志 - " << e.what() << endl;
            throw;  // 重新抛出同一个异常
            // 注意: throw; 保留原始异常类型
            // 如果用 throw e; 会发生对象切片
        }
    }
    catch (const exception& e) {
        cout << "外层 catch: " << e.what() << endl;
    }

    cout << "\n========== 10. 函数 try 块 ==========" << endl;
    // 函数 try 块: 将整个函数体放在 try 块中
    // 常用于构造函数中捕获初始化列表的异常
    auto funcTryBlock = []() try {
        cout << "函数 try 块执行" << endl;
        throw runtime_error("函数 try 块中的异常");
    }
    catch (const exception& e) {
        cout << "函数 try 块 catch: " << e.what() << endl;
    };

    funcTryBlock();

    cout << "\n========== 11. 最佳实践 ==========" << endl;
    cout << "异常处理最佳实践:" << endl;
    cout << "1. 用异常处理真正的错误，不要用于正常控制流" << endl;
    cout << "2. 按引用捕获异常: catch (const ExceptionType& e)" << endl;
    cout << "3. 派生类异常的 catch 放在基类之前" << endl;
    cout << "4. 使用 RAII 管理资源，而不是在 catch 中手动释放" << endl;
    cout << "5. 构造函数中可以抛出异常（对象未构造完成）" << endl;
    cout << "6. 析构函数中不应该抛出异常（noexcept）" << endl;
    cout << "7. 使用标准异常类或继承自 std::exception" << endl;
    cout << "8. catch(...) 用于兜底，但不要吞掉异常" << endl;
    cout << "9. 用 noexcept 标记不会抛异常的函数" << endl;
    cout << "10. 不要抛出裸指针，抛出对象或智能指针" << endl;

    cout << "\n========== 12. 常见错误 ==========" << endl;
    // 错误1: 按值捕获导致对象切片
    // catch (exception e) { ... }  // 错误! 派生类信息丢失
    // 正确: catch (const exception& e) { ... }

    // 错误2: 在析构函数中抛出异常
    // class Bad { ~Bad() { throw 1; } };  // 程序会调用 terminate()

    // 错误3: catch 后不处理就吞掉异常
    // try { ... } catch (...) { /* 啥都不做 */ }

    // 错误4: 用异常做正常控制流
    // try { return v.at(i); } catch (...) { return -1; }
    // 应该先检查: if (i < v.size()) return v[i]; else return -1;

    // 错误5: throw e; 而不是 throw;
    // throw e; 会导致对象切片
    // throw;   保留原始异常类型

    cout << "常见错误已注释说明，请查看源码" << endl;

    cout << "\n========== 程序结束 ==========" << endl;
    return 0;
}
