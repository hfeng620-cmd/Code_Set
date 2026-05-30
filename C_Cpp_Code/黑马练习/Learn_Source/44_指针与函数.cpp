/*
 * ============================================================
 * 文件名: 44_指针与函数.cpp
 * 主题: C++ 指针与函数的高级应用
 * ============================================================
 * 本文件涵盖:
 *   1. 传递指针给函数 (地址传递)
 *   2. 从函数返回指针 (悬空指针警告)
 *   3. 函数指针的声明与使用
 *   4. 函数指针数组
 *   5. 回调函数 (Callback Function)
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 1. 传递指针给函数
// ============================================================

// 通过指针交换两个值
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 通过指针修改数组
void doubleArray(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] *= 2;  // 等价于 *(arr + i) *= 2
    }
}

// 通过指针查找最大值和最小值
void findMinMax(int* arr, int size, int* minVal, int* maxVal) {
    *minVal = arr[0];
    *maxVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < *minVal) *minVal = arr[i];
        if (arr[i] > *maxVal) *maxVal = arr[i];
    }
}

// ============================================================
// 2. 从函数返回指针
// ============================================================

// 返回动态分配的内存 (需要调用者释放)
int* createDynamicArray(int size) {
    int* arr = new int[size];
    for (int i = 0; i < size; i++) arr[i] = (i + 1) * 10;
    return arr;  // 调用者需要 delete[] arr
}

// 错误示例 (已注释): 返回局部变量的指针会导致悬空指针!
// int* dangerousFunction() { int local = 42; return &local; }

// ============================================================
// 3. 函数指针
// ============================================================
/*
 * 函数指针是指向函数的指针变量。
 * 函数在内存中也有地址, 函数指针存储这个地址。
 *
 * 声明语法:
 *   返回类型 (*指针名)(参数类型列表);
 *
 * 例如:
 *   int (*funcPtr)(int, int);
 *   声明一个指向"接受两个int参数, 返回int"的函数的指针
 */

// 一些简单的数学函数
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) {
    if (b == 0) {
        cout << "  [错误] 除数不能为0!" << endl;
        return 0;
    }
    return a / b;
}

// 比较函数 (用于排序回调)
bool ascending(int a, int b) { return a < b; }
bool descending(int a, int b) { return a > b; }

// ============================================================
// 4. 回调函数 (Callback Function)
// ============================================================
// 回调函数: 将函数指针作为参数传递给另一个函数

// 使用回调函数的排序 (冒泡排序)
void bubbleSort(int arr[], int size, bool (*compare)(int, int)) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (compare(arr[j + 1], arr[j])) {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
}

// 使用回调函数的数组操作
void processArray(int arr[], int size, int (*operation)(int)) {
    for (int i = 0; i < size; i++) arr[i] = operation(arr[i]);
}

// 操作函数
int square(int x) { return x * x; }
int doubleValue(int x) { return x * 2; }
int negate(int x) { return -x; }

// ============================================================
// main 函数
// ============================================================
int main() {

    // ----------------------------------------------------------
    // 1. 传递指针给函数
    // ----------------------------------------------------------
    cout << "===== 1. 传递指针给函数 =====" << endl;

    // 1.1 通过指针交换
    int x = 10, y = 20;
    cout << "  交换前: x=" << x << ", y=" << y << endl;
    swap(&x, &y);
    cout << "  交换后: x=" << x << ", y=" << y << endl;

    // 1.2 通过指针修改数组
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;
    cout << "\n  原始数组: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    doubleArray(arr, size);
    cout << "  翻倍后: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    // 1.3 通过指针返回多个值
    int data[] = {3, 7, 1, 9, 4, 6, 2, 8, 5};
    int minVal, maxVal;
    findMinMax(data, 9, &minVal, &maxVal);
    cout << "\n  数组最小值: " << minVal << endl;
    cout << "  数组最大值: " << maxVal << endl;

    // ----------------------------------------------------------
    // 2. 从函数返回指针
    // ----------------------------------------------------------
    cout << "\n===== 2. 从函数返回指针 =====" << endl;

    // 返回动态分配的指针
    int* dynamicArr = createDynamicArray(5);
    cout << "  动态数组: ";
    for (int i = 0; i < 5; i++) cout << dynamicArr[i] << " ";
    cout << endl;
    delete[] dynamicArr;   // 必须手动释放动态内存!
    dynamicArr = nullptr;

    cout << "\n  [警告] 不要返回局部变量的指针! 局部变量在函数结束后被销毁" << endl;
    // int* dangerous = dangerousFunction();  // 危险!
    // *dangerous = 100;  // 未定义行为!

    // ----------------------------------------------------------
    // 3. 函数指针
    // ----------------------------------------------------------
    cout << "\n===== 3. 函数指针 =====" << endl;

    // 3.1 声明函数指针
    // 声明: 返回类型 (*指针名)(参数类型列表)
    int (*funcPtr)(int, int);

    // 3.2 给函数指针赋值
    funcPtr = add;  // 函数名就是函数的地址
    cout << "  funcPtr = add: " << funcPtr(3, 5) << endl;

    funcPtr = subtract;
    cout << "  funcPtr = subtract: " << funcPtr(3, 5) << endl;

    funcPtr = multiply;
    cout << "  funcPtr = multiply: " << funcPtr(3, 5) << endl;

    funcPtr = divide;
    cout << "  funcPtr = divide: " << funcPtr(10, 3) << endl;

    // 3.3 声明时直接初始化
    int (*calcFunc)(int, int) = add;
    cout << "  calcFunc(10, 20) = " << calcFunc(10, 20) << endl;

    // 3.4 使用 typedef 简化
    typedef int (*MathFunc)(int, int);
    MathFunc func1 = add;
    MathFunc func2 = multiply;
    cout << "  typedef: func1(5,6) = " << func1(5, 6) << endl;
    cout << "  typedef: func2(5,6) = " << func2(5, 6) << endl;

    // ----------------------------------------------------------
    // 4. 函数指针数组
    // ----------------------------------------------------------
    cout << "\n===== 4. 函数指针数组 =====" << endl;

    // 声明函数指针数组
    const char* opNames[] = {"+", "-", "*", "/"};
    int (*operations[])(int, int) = {add, subtract, multiply, divide};

    int a = 20, b = 5;
    cout << "  a = " << a << ", b = " << b << endl;
    for (int i = 0; i < 4; i++) {
        cout << "  a " << opNames[i] << " b = " << operations[i](a, b) << endl;
    }

    // ----------------------------------------------------------
    // 5. 回调函数
    // ----------------------------------------------------------
    cout << "\n===== 5. 回调函数 =====" << endl;

    // 自定义排序
    int sortArr[] = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    bubbleSort(sortArr, 9, ascending);
    cout << "  升序: ";
    for (int i = 0; i < 9; i++) cout << sortArr[i] << " ";
    cout << endl;
    bubbleSort(sortArr, 9, descending);
    cout << "  降序: ";
    for (int i = 0; i < 9; i++) cout << sortArr[i] << " ";
    cout << endl;

    // 使用回调处理数组
    int processArr[] = {1, 2, 3, 4, 5};
    processArray(processArr, 5, square);
    cout << "  平方: ";
    for (int i = 0; i < 5; i++) cout << processArr[i] << " ";
    cout << endl;

    return 0;
}

/*
 * 总结: 1. 传递指针给函数可以修改原值 2. 从函数返回指针需注意悬空指针
 * 3. 函数指针: 返回类型 (*指针名)(参数类型列表) 4. 回调函数: 将函数指针作为参数传递
 */
