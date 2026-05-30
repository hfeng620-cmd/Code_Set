/*
 * 47_结构体进阶用法.cpp
 * 本文件讲解结构体的高级用法：数组、指针、嵌套、函数传参、排序等
 * 这些是实际开发中结构体最常用的场景
 * 编译命令: g++ -o 47_结构体进阶用法 47_结构体进阶用法.cpp -std=c++11
 */

#include <iostream>
#include <cstring>
using namespace std;

// ===================== 结构体定义 =====================
// 学生信息结构体
struct Student {
    char name[50];
    int age;
    float score;
};

// 嵌套结构体：日期
struct Date {
    int year;
    int month;
    int day;
};

// 嵌套结构体：员工（包含Date作为生日）
struct Employee {
    char name[50];
    int id;
    Date birthday;  // 嵌套Date结构体
    float salary;
};

// ===================== 1. 结构体作为函数参数（值传递） =====================
// 值传递：函数内操作的是副本，不影响原数据
// 缺点：结构体较大时拷贝开销大
void printStudentByValue(Student s) {
    cout << "[值传递] " << s.name << ", " << s.age << "岁, " << s.score << "分" << endl;
    // 修改s不会影响原变量
    s.age = 999;  // 只修改了副本
}

// ===================== 2. 结构体作为函数参数（地址传递/指针） =====================
// 指针传递：传入结构体地址，效率高，可以修改原数据
void printStudentByPointer(const Student *s) {
    // 使用箭头运算符(->)访问指针指向的结构体成员
    cout << "[指针传递] " << s->name << ", " << s->age << "岁, " << s->score << "分" << endl;
    // const修饰表示不能通过该指针修改结构体内容
    // s->age = 100;  // 错误！const保护
}

// 通过指针修改结构体数据的函数
void addBonus(Student *s, float bonus) {
    // 箭头运算符(->)：等价于 (*s).score
    s->score += bonus;
    if (s->score > 100.0f) {
        s->score = 100.0f;  // 最高100分
    }
}

// ===================== 3. 结构体作为函数参数（引用传递） =====================
// 引用传递：既高效又安全，推荐方式
void printStudentByRef(const Student &s) {
    cout << "[引用传递] " << s.name << ", " << s.age << "岁, " << s.score << "分" << endl;
    // const引用：不能修改，且无拷贝开销
}

// 通过引用修改结构体
void updateScore(Student &s, float newScore) {
    s.score = newScore;  // 直接修改原结构体
}

// ===================== 4. 冒泡排序（结构体数组） =====================
// 按成绩从高到低排序
void bubbleSortByScore(Student arr[], int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            // 如果前面的成绩比后面低，就交换（降序）
            if (arr[j].score < arr[j + 1].score) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// 按姓名排序（使用strcmp比较字符串）
void bubbleSortByName(Student arr[], int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - 1 - i; j++) {
            // strcmp > 0 表示第一个字符串"大于"第二个
            if (strcmp(arr[j].name, arr[j + 1].name) > 0) {
                Student temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// 打印学生数组
void printStudentArray(const Student arr[], int len, const char *title) {
    cout << "--- " << title << " ---" << endl;
    for (int i = 0; i < len; i++) {
        cout << "  " << arr[i].name << "  年龄:" << arr[i].age
             << "  成绩:" << arr[i].score << endl;
    }
    cout << endl;
}

// ===================== 5. 函数声明 =====================
void structArrayDemo();
void structPointerDemo();
void nestedStructDemo();
void functionParamDemo();
void bubbleSortDemo();

// ===================== 主函数 =====================
int main() {
    cout << "========================================" << endl;
    cout << "       C++ 结构体进阶用法" << endl;
    cout << "========================================" << endl << endl;

    structArrayDemo();     // 结构体数组
    structPointerDemo();   // 结构体指针
    nestedStructDemo();    // 嵌套结构体
    functionParamDemo();   // 结构体与函数
    bubbleSortDemo();      // 冒泡排序

    cout << "程序执行完毕！" << endl;
    return 0;
}

// ===================== 结构体数组 =====================
void structArrayDemo() {
    cout << "===== 结构体数组 =====" << endl;

    // 声明并初始化结构体数组
    Student class1[5] = {
        {"张三", 20, 95.5f},
        {"李四", 19, 88.0f},
        {"王五", 21, 76.5f},
        {"赵六", 20, 92.0f},
        {"钱七", 19, 85.0f}
    };

    // 遍历结构体数组
    int len = sizeof(class1) / sizeof(class1[0]);  // 计算数组长度
    printStudentArray(class1, len, "原始顺序");

    // 计算平均分
    float sum = 0;
    for (int i = 0; i < len; i++) {
        sum += class1[i].score;
    }
    cout << "班级平均分: " << sum / len << endl;

    // 找最高分学生
    int maxIdx = 0;
    for (int i = 1; i < len; i++) {
        if (class1[i].score > class1[maxIdx].score) {
            maxIdx = i;
        }
    }
    cout << "最高分: " << class1[maxIdx].name << " (" << class1[maxIdx].score << "分)" << endl;

    cout << endl;
}

// ===================== 结构体指针 =====================
void structPointerDemo() {
    cout << "===== 结构体指针与箭头运算符 =====" << endl;

    Student stu = {"张三", 20, 95.5f};

    // 获取结构体指针
    Student *p = &stu;

    // 访问成员的两种方式：
    // 方式1：(*指针).成员  （先解引用，再用点号）
    cout << "方式1 (*p).name: " << (*p).name << endl;

    // 方式2：指针->成员  （箭头运算符，更常用）
    cout << "方式2 p->name: " << p->name << endl;
    cout << "方式2 p->age: " << p->age << endl;
    cout << "方式2 p->score: " << p->score << endl;

    // 通过指针修改成员
    p->age = 21;
    p->score = 98.0f;
    cout << "修改后: " << p->name << ", " << p->age << "岁, " << p->score << "分" << endl;

    // 常见错误：优先级问题
    // cout << *p.name;  // 错误！点号(.)优先级高于解引用(*)
    // 正确写法：
    cout << "(*p).name = " << (*p).name << endl;

    // 动态创建结构体（堆上）
    Student *pStu = new Student;
    strcpy(pStu->name, "动态学生");
    pStu->age = 22;
    pStu->score = 90.0f;
    cout << "\n动态创建: " << pStu->name << ", " << pStu->age << "岁" << endl;
    delete pStu;  // 记得释放内存
    pStu = nullptr;

    cout << endl;
}

// ===================== 嵌套结构体 =====================
void nestedStructDemo() {
    cout << "===== 嵌套结构体 =====" << endl;

    // 创建Employee变量并初始化嵌套成员
    Employee emp;

    // 访问嵌套成员：使用多个点号
    strcpy(emp.name, "李明");
    emp.id = 10086;
    emp.birthday.year = 1995;    // 访问嵌套的Date结构体
    emp.birthday.month = 8;
    emp.birthday.day = 15;
    emp.salary = 20000.0f;

    cout << "员工: " << emp.name << endl;
    cout << "工号: " << emp.id << endl;
    cout << "生日: " << emp.birthday.year << "年"
         << emp.birthday.month << "月"
         << emp.birthday.day << "日" << endl;
    cout << "薪资: " << emp.salary << endl;

    // 嵌套结构体也可以整体初始化
    Employee emp2 = {"王芳", 10087, {1998, 3, 20}, 18000.0f};
    cout << "\n员工2: " << emp2.name
         << ", 生日: " << emp2.birthday.year << "-"
         << emp2.birthday.month << "-" << emp2.birthday.day << endl;

    // 结构体嵌套结构体数组也是可以的
    struct Team {
        char teamName[50];
        Employee members[3];
        int memberCount;
    };

    Team team = {"开发组", {
        {"张三", 2001, {1990, 1, 1}, 25000},
        {"李四", 2002, {1992, 5, 10}, 22000},
        {"王五", 2003, {1995, 12, 25}, 20000}
    }, 3};

    cout << "\n团队: " << team.teamName << endl;
    for (int i = 0; i < team.memberCount; i++) {
        cout << "  " << team.members[i].name
             << " - 工资:" << team.members[i].salary << endl;
    }

    cout << endl;
}

// ===================== 结构体与函数 =====================
void functionParamDemo() {
    cout << "===== 结构体作为函数参数 =====" << endl;

    Student stu = {"张三", 20, 85.0f};

    // 1. 值传递（拷贝整个结构体）
    cout << "--- 值传递 ---" << endl;
    printStudentByValue(stu);
    cout << "原数据不受影响: " << stu.name << ", age=" << stu.age << endl;

    // 2. 指针传递（传地址，高效）
    cout << "\n--- 指针传递 ---" << endl;
    printStudentByPointer(&stu);  // 传入地址
    addBonus(&stu, 10.0f);       // 通过指针加分
    cout << "加分后: " << stu.score << "分" << endl;

    // 3. 引用传递（最推荐）
    cout << "\n--- 引用传递 ---" << endl;
    printStudentByRef(stu);      // const引用，不拷贝
    updateScore(stu, 100.0f);    // 引用修改
    cout << "更新后: " << stu.score << "分" << endl;

    // 三种传参方式对比：
    // 值传递：安全但慢（大结构体拷贝开销大）
    // 指针传递：快但语法复杂（需要->，注意空指针）
    // 引用传递：快且语法简洁（推荐使用const引用传参）

    cout << endl;
}

// ===================== 冒泡排序 =====================
void bubbleSortDemo() {
    cout << "===== 结构体数组冒泡排序 =====" << endl;

    Student students[] = {
        {"Charlie", 20, 78.5f},
        {"Alice", 19, 95.0f},
        {"Eve", 21, 88.0f},
        {"Bob", 20, 62.0f},
        {"David", 19, 91.5f}
    };
    int len = sizeof(students) / sizeof(students[0]);

    // 按成绩降序排序
    printStudentArray(students, len, "排序前");
    bubbleSortByScore(students, len);
    printStudentArray(students, len, "按成绩降序排序后");

    // 按姓名升序排序（字母序）
    bubbleSortByName(students, len);
    printStudentArray(students, len, "按姓名升序排序后");

    // 总结：
    // 结构体数组排序的关键是定义比较规则
    // 可以按任意成员排序，只需修改if条件中的比较逻辑
    // 实际开发中，可以使用qsort或std::sort简化排序代码

    cout << endl;
}
