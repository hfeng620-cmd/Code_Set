/*
 * ============================================================
 * 文件名: 43_指针与结构体.cpp
 * 主题: C++ 指针与结构体的结合使用
 * ============================================================
 * 本文件涵盖:
 *   1. 结构体指针的声明与初始化
 *   2. -> 运算符: 通过指针访问成员
 *   3. 将结构体指针传递给函数
 *   4. 使用 new 动态创建结构体
 *   5. 结构体数组与指针
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
// 1. 定义结构体
// ============================================================

// 学生结构体
struct Student {
    string name;
    int age;
    double score;
    string className;

    // 打印信息的方法
    void print() const {
        cout << "    姓名: " << name << endl;
        cout << "    年龄: " << age << endl;
        cout << "    分数: " << score << endl;
        cout << "    班级: " << className << endl;
    }
};

// 链表节点结构体
struct ListNode {
    int data;
    ListNode* next;  // 指向下一个节点的指针
};

// 函数声明
void updateStudent(Student* stu, const string& name, int age, double score);
void printStudent(const Student* stu);
ListNode* createList(int arr[], int size);
void printList(const ListNode* head);
void deleteList(ListNode* head);

// ============================================================
// main 函数
// ============================================================
int main() {

    // ----------------------------------------------------------
    // 1. 结构体指针的声明与初始化
    // ----------------------------------------------------------
    cout << "===== 1. 结构体指针 =====" << endl;

    // 创建结构体变量
    Student stu1 = {"张三", 20, 95.5, "计算机1班"};

    // 声明结构体指针并初始化
    Student* ptr = &stu1;

    cout << "  结构体地址: " << ptr << endl;
    cout << "  结构体大小: " << sizeof(Student) << " 字节" << endl;

    // ----------------------------------------------------------
    // 2. -> 运算符: 通过指针访问成员
    // ----------------------------------------------------------
    cout << "\n===== 2. -> 运算符 =====" << endl;

    // 方式1: 使用 -> 运算符 (推荐!)
    cout << "  ptr->name = " << ptr->name << endl;
    cout << "  ptr->age = " << ptr->age << endl;
    cout << "  ptr->score = " << ptr->score << endl;

    // 方式2: 使用 (*ptr).成员 (等价但繁琐)
    cout << "  (*ptr).name = " << (*ptr).name << endl;
    cout << "  (*ptr).age = " << (*ptr).age << endl;

    // 通过指针修改成员
    ptr->age = 21;
    ptr->score = 98.0;
    cout << "\n  修改后:" << endl;
    cout << "  ptr->age = " << ptr->age << endl;
    cout << "  ptr->score = " << ptr->score << endl;

    // 通过指针调用成员函数
    cout << "\n  调用 print():" << endl;
    ptr->print();

    // ----------------------------------------------------------
    // 3. 将结构体指针传递给函数
    // ----------------------------------------------------------
    cout << "\n===== 3. 指针传参 =====" << endl;

    Student stu2 = {"李四", 19, 88.0, "计算机2班"};
    cout << "  修改前:" << endl;
    printStudent(&stu2);

    // 通过指针修改
    updateStudent(&stu2, "李四", 19, 95.0);
    cout << "\n  修改后:" << endl;
    printStudent(&stu2);

    // ----------------------------------------------------------
    // 4. 使用 new 动态创建结构体
    // ----------------------------------------------------------
    cout << "\n===== 4. 动态创建结构体 =====" << endl;

    // 使用 new 在堆上创建结构体
    Student* stuPtr = new Student;
    stuPtr->name = "王五";
    stuPtr->age = 22;
    stuPtr->score = 92.5;
    stuPtr->className = "软件工程1班";

    cout << "  动态创建的学生:" << endl;
    stuPtr->print();

    // 使用 new 并初始化 (C++11)
    Student* stuPtr2 = new Student{"赵六", 21, 90.0, "软件工程2班"};
    cout << "\n  动态创建并初始化:" << endl;
    stuPtr2->print();

    // 使用完毕后释放内存
    delete stuPtr;
    stuPtr = nullptr;  // 好习惯: delete 后置空

    delete stuPtr2;
    stuPtr2 = nullptr;

    // ----------------------------------------------------------
    // 5. 结构体数组与指针
    // ----------------------------------------------------------
    cout << "\n===== 5. 结构体数组与指针 =====" << endl;

    // 结构体数组
    Student students[] = {
        {"学生A", 18, 85.0, "1班"},
        {"学生B", 19, 92.0, "2班"},
        {"学生C", 20, 78.5, "1班"},
        {"学生D", 18, 96.0, "3班"},
    };
    int count = 4;

    // 指针指向数组
    Student* arrPtr = students;

    cout << "  遍历结构体数组 (用指针):" << endl;
    for (int i = 0; i < count; i++) {
        cout << "  " << (arrPtr + i)->name
             << ", 分数: " << (arrPtr + i)->score << endl;
    }

    // 指针递增遍历
    cout << "\n  指针递增遍历:" << endl;
    for (Student* p = students; p < students + count; p++) {
        cout << "  " << p->name << ", 年龄: " << p->age << endl;
    }

    // ----------------------------------------------------------
    // 6. 链表示例 (结构体指针的典型应用)
    // ----------------------------------------------------------
    cout << "\n===== 6. 链表 =====" << endl;

    int listData[] = {1, 2, 3, 4, 5};
    ListNode* head = createList(listData, 5);

    cout << "  链表: ";
    printList(head);

    // 释放链表
    deleteList(head);
    head = nullptr;

    return 0;
}

// ============================================================
// 函数定义
// ============================================================

// 通过指针修改学生信息
void updateStudent(Student* stu, const string& name, int age, double score) {
    // 使用 -> 运算符通过指针访问成员
    stu->name = name;
    stu->age = age;
    stu->score = score;
}

// 通过指针打印学生信息 (const 指针, 不修改数据)
void printStudent(const Student* stu) {
    cout << "    姓名: " << stu->name << endl;
    cout << "    年龄: " << stu->age << endl;
    cout << "    分数: " << stu->score << endl;
    cout << "    班级: " << stu->className << endl;
}

// 创建链表 (从数组)
ListNode* createList(int arr[], int size) {
    if (size <= 0) return nullptr;

    // 创建头节点
    ListNode* head = new ListNode{arr[0], nullptr};
    ListNode* current = head;

    // 创建后续节点
    for (int i = 1; i < size; i++) {
        current->next = new ListNode{arr[i], nullptr};
        current = current->next;
    }

    return head;
}

// 打印链表
void printList(const ListNode* head) {
    const ListNode* current = head;
    while (current != nullptr) {
        cout << current->data;
        if (current->next != nullptr) cout << " -> ";
        current = current->next;
    }
    cout << " -> NULL" << endl;
}

// 释放链表内存
void deleteList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

/*
 * 总结:
 * 1. 结构体指针: Student* ptr = &student;
 * 2. 访问成员: ptr->member 或 (*ptr).member
 * 3. -> 运算符是 (*ptr).member 的简写
 * 4. 函数传参: 传递指针可以修改原结构体
 * 5. 动态创建: new Student {...}, 用 delete 释放
 * 6. 链表: 结构体包含指向自身的指针
 * 常见错误: 用 . 而不是 -> 访问指针成员; 忘记 delete 导致内存泄漏
 */
