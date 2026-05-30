/*
 * ============================================================================
 * 黑马程序员 C++课程 - 综合项目：通讯录管理系统
 * ============================================================================
 *
 * 【项目背景】
 * 通讯录管理系统是黑马程序员C++课程中，学员学完基础语法后的第一个综合实战项目。
 * 该项目综合运用了：结构体、函数、数组、循环、条件判断、输入输出等基础知识。
 *
 * 【功能需求】
 * 1. 添加联系人 - 录入姓名、性别、年龄、电话、住址
 * 2. 显示联系人 - 列表展示所有已添加的联系人信息
 * 3. 删除联系人 - 按姓名查找并删除指定联系人（带确认提示）
 * 4. 查找联系人 - 按姓名模糊或精确查找联系人
 * 5. 修改联系人 - 按姓名找到后修改其各项信息
 * 6. 清空联系人 - 一次性清空所有联系人数据（带确认提示）
 * 0. 退出系统   - 退出通讯录管理系统
 *
 * 【技术要点】
 * - 使用结构体（struct）定义联系人和通讯录数据类型
 * - 使用函数封装每个功能模块，实现模块化编程
 * - 使用数组存储联系人，通过计数器管理有效数据量
 * - 输入验证：确保用户输入的性别、年龄等数据合法
 * - 删除和清空操作带确认提示，防止误操作
 *
 * 【设计思路】
 * 1. 先设计数据结构：联系人结构体 + 通讯录结构体
 * 2. 再设计菜单函数：显示操作选项供用户选择
 * 3. 逐一实现每个功能函数
 * 4. 在主函数中通过switch-case调用对应功能
 *
 * 编译方法：g++ 93_综合项目_通讯录管理系统.cpp -o contact_mgr
 * ============================================================================
 */

#include <iostream>  // 标准输入输出流，用于cin/cout
#include <string>    // 字符串类，用于存储姓名、电话、地址等
#include <limits>    // 用于清除输入缓冲区

using namespace std;

// ============================================================================
// 【数据结构定义】
// ============================================================================

/*
 * 联系人结构体 - 存储单个联系人的基本信息
 *
 * 设计思考：为什么用结构体而不是类？
 * 在这个阶段，我们学习的是C++基础语法，结构体是最直观的数据组织方式。
 * 后续学习面向对象后，可以改用类（class）来实现更好的封装。
 */
struct Contact
{
    string name;     // 联系人姓名
    string gender;   // 性别（"男"或"女"）
    int age;         // 年龄
    string phone;    // 电话号码
    string address;  // 家庭住址
};

/*
 * 通讯录结构体 - 管理所有联系人的容器
 *
 * 设计思考：为什么需要size字段？
 * 因为C风格数组是固定大小的，我们需要一个计数器来记录
 * 当前实际存储了多少个联系人，以便知道下一个应该存到哪个位置。
 */
#define MAX_CONTACTS 1000  // 通讯录最大容量（宏定义常量）

struct AddressBook
{
    Contact contacts[MAX_CONTACTS];  // 联系人数组（固定大小）
    int size;                         // 当前联系人数量
};

// ============================================================================
// 【函数声明】 - 先声明函数原型，再在后面定义具体实现
// ============================================================================

void showMenu();                          // 显示主菜单
void addContact(AddressBook* abs);        // 添加联系人
void showContacts(AddressBook* abs);      // 显示所有联系人
void deleteContact(AddressBook* abs);     // 删除联系人
void searchContact(AddressBook* abs);     // 查找联系人
void modifyContact(AddressBook* abs);     // 修改联系人
void clearContacts(AddressBook* abs);     // 清空联系人
int findByName(AddressBook* abs, string name);  // 按姓名查找（内部辅助函数）
void clearInputBuffer();                  // 清除输入缓冲区（辅助函数）

// ============================================================================
// 【主函数】 - 程序入口，实现主循环和菜单调度
// ============================================================================

int main()
{
    /*
     * 创建通讯录变量
     * 注意：这里创建的是局部变量，存在于栈内存中
     * 初始化size为0，表示通讯录当前为空
     */
    AddressBook abs;
    abs.size = 0;

    // 定义用户选择的变量
    int select = 0;

    cout << "========================================" << endl;
    cout << "    欢迎使用通讯录管理系统 v1.0" << endl;
    cout << "========================================" << endl;

    /*
     * 主循环 - 使用while(true)实现无限循环
     * 用户选择"退出"时通过break跳出循环
     *
     * 设计思考：为什么用while而不是do-while？
     * while(true)是最常见的菜单循环写法，逻辑清晰，
     * 每次循环先显示菜单、获取输入、处理选择。
     */
    while (true)
    {
        // 显示菜单选项
        showMenu();

        // 获取用户输入
        cout << "\n请输入您的选择: ";
        cin >> select;

        /*
         * 输入验证：如果输入不是数字（比如输入了字母）
         * cin会进入错误状态，需要清除错误标志和缓冲区
         */
        if (cin.fail())
        {
            cin.clear();  // 清除错误标志
            clearInputBuffer();  // 清空缓冲区中的无效字符
            cout << "[错误] 请输入有效的数字选项！" << endl;
            continue;  // 跳过本次循环，重新显示菜单
        }

        /*
         * 使用switch-case处理用户选择
         *
         * 设计思考：为什么用switch而不是if-else？
         * 当分支条件是对同一个变量的等值比较时，
         * switch-case比if-else更清晰、效率更高。
         */
        switch (select)
        {
        case 1:  // 1 - 添加联系人
            addContact(&abs);  // 传入通讯录地址（指针传递）
            break;
        case 2:  // 2 - 显示联系人
            showContacts(&abs);
            break;
        case 3:  // 3 - 删除联系人
            deleteContact(&abs);
            break;
        case 4:  // 4 - 查找联系人
            searchContact(&abs);
            break;
        case 5:  // 5 - 修改联系人
            modifyContact(&abs);
            break;
        case 6:  // 6 - 清空联系人
            clearContacts(&abs);
            break;
        case 0:  // 0 - 退出系统
            cout << "\n========================================" << endl;
            cout << "    感谢使用通讯录管理系统，再见！" << endl;
            cout << "========================================" << endl;
            return 0;  // 结束程序，返回0表示正常退出
        default:  // 其他无效输入
            cout << "[提示] 无效的选择，请输入0-6之间的数字。" << endl;
            break;
        }

        /*
         * 每次操作完成后暂停，让用户查看结果
         * 这是控制台程序的常见做法，提升用户体验
         */
        cout << "\n按回车键继续...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清除之前残留的换行符
        cin.get();  // 等待用户按回车
    }

    return 0;
}

// ============================================================================
// 【菜单显示函数】
// ============================================================================

/*
 * showMenu() - 显示系统的功能菜单
 *
 * 参数：无
 * 返回值：无
 *
 * 设计说明：将菜单独立为函数，便于维护和修改。
 * 如果将来要添加新功能，只需修改此函数和switch-case即可。
 */
void showMenu()
{
    cout << "\n┌────────────────────────────────────┐" << endl;
    cout << "│        通讯录管理系统               │" << endl;
    cout << "├────────────────────────────────────┤" << endl;
    cout << "│  1. 添加联系人                      │" << endl;
    cout << "│  2. 显示联系人                      │" << endl;
    cout << "│  3. 删除联系人                      │" << endl;
    cout << "│  4. 查找联系人                      │" << endl;
    cout << "│  5. 修改联系人                      │" << endl;
    cout << "│  6. 清空联系人                      │" << endl;
    cout << "│  0. 退出系统                        │" << endl;
    cout << "└────────────────────────────────────┘" << endl;
}

// ============================================================================
// 【辅助函数】
// ============================================================================

/*
 * clearInputBuffer() - 清除输入缓冲区
 *
 * 设计说明：当用户输入错误类型的数据时（如要求输入数字却输入了字母），
 * cin会进入错误状态，缓冲区中残留的字符会影响后续输入。
 * 此函数用于清除缓冲区中的所有残留字符，直到遇到换行符。
 */
void clearInputBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
 * findByName() - 按姓名在通讯录中查找联系人
 *
 * 参数：
 *   abs  - 通讯录指针
 *   name - 要查找的姓名
 *
 * 返回值：
 *   找到 - 返回该联系人在数组中的索引（0开始）
 *   未找到 - 返回-1
 *
 * 设计说明：将查找逻辑封装为独立函数，
 * 因为删除、查找、修改功能都需要用到按姓名查找。
 * 这体现了"代码复用"的编程思想。
 */
int findByName(AddressBook* abs, string name)
{
    /*
     * 遍历通讯录中的所有联系人
     * 注意：只遍历到abs->size，而不是MAX_CONTACTS
     * 因为size之后的位置是未使用的，不需要查找
     */
    for (int i = 0; i < abs->size; i++)
    {
        if (abs->contacts[i].name == name)
        {
            return i;  // 找到了，返回索引
        }
    }
    return -1;  // 遍历完都没找到，返回-1
}

// ============================================================================
// 【功能函数实现】
// ============================================================================

/*
 * addContact() - 添加联系人
 *
 * 参数：abs - 通讯录指针（传指针可以直接修改原数据）
 *
 * 功能流程：
 * 1. 检查通讯录是否已满
 * 2. 依次输入联系人的各项信息
 * 3. 对输入进行验证（性别只能是"男"或"女"，年龄必须合理等）
 * 4. 将信息存入通讯录数组
 * 5. 更新联系人计数
 *
 * 设计思考：为什么参数用指针而不是值传递？
 * 因为我们需要修改通讯录的内容。如果用值传递，
 * 函数内修改的是副本，原数据不会改变。
 * 指针传递让我们能直接操作原始数据。
 */
void addContact(AddressBook* abs)
{
    // 步骤1：检查通讯录是否已满
    if (abs->size >= MAX_CONTACTS)
    {
        cout << "[错误] 通讯录已满，无法添加更多联系人！" << endl;
        cout << "       最大容量为 " << MAX_CONTACTS << " 人。" << endl;
        return;  // 直接返回，不执行后续操作
    }

    cout << "\n--- 添加联系人 ---" << endl;

    // 步骤2：输入姓名
    string name;
    cout << "请输入姓名: ";
    cin >> name;

    /*
     * 去除前后空格（简单版本）
     * 注意：这里简化处理，实际项目中可能需要更完善的字符串处理
     */
    abs->contacts[abs->size].name = name;

    // 步骤3：输入性别（带验证）
    string gender;
    while (true)
    {
        cout << "请输入性别（男/女）: ";
        cin >> gender;
        if (gender == "男" || gender == "女")
        {
            abs->contacts[abs->size].gender = gender;
            break;  // 输入合法，跳出验证循环
        }
        else
        {
            cout << "[提示] 性别只能输入\"男\"或\"女\"，请重新输入。" << endl;
        }
    }

    // 步骤4：输入年龄（带验证）
    int age;
    while (true)
    {
        cout << "请输入年龄: ";
        cin >> age;

        // 检查输入是否为数字
        if (cin.fail())
        {
            cin.clear();
            clearInputBuffer();
            cout << "[提示] 请输入有效的数字！" << endl;
            continue;
        }

        // 检查年龄范围是否合理
        if (age > 0 && age <= 150)
        {
            abs->contacts[abs->size].age = age;
            break;
        }
        else
        {
            cout << "[提示] 年龄应在1-150之间，请重新输入。" << endl;
        }
    }

    // 步骤5：输入电话号码
    string phone;
    cout << "请输入联系电话: ";
    cin >> phone;
    abs->contacts[abs->size].phone = phone;

    // 步骤6：输入住址
    /*
     * 注意：这里使用cin，所以地址不能包含空格。
     * 如果需要支持带空格的地址，应该使用getline()。
     * 但getline()和cin混用时需要注意缓冲区问题。
     */
    string address;
    cout << "请输入家庭住址: ";
    cin >> address;
    abs->contacts[abs->size].address = address;

    // 步骤7：更新联系人计数
    abs->size++;

    // 步骤8：显示添加成功的提示
    cout << "\n[成功] 联系人 \"" << name << "\" 已成功添加到通讯录！" << endl;
    cout << "       当前通讯录共有 " << abs->size << " 位联系人。" << endl;
}

/*
 * showContacts() - 显示所有联系人
 *
 * 参数：abs - 通讯录指针
 *
 * 功能：遍历通讯录，以格式化的表格形式显示所有联系人信息。
 *
 * 设计说明：使用iomanip或手动控制空格来对齐输出，
 * 让显示效果更加美观。这里使用简单的\t制表符对齐。
 */
void showContacts(AddressBook* abs)
{
    // 先检查通讯录是否为空
    if (abs->size == 0)
    {
        cout << "\n[提示] 通讯录为空，还没有添加任何联系人。" << endl;
        cout << "       请先使用\"添加联系人\"功能添加联系人。" << endl;
        return;
    }

    cout << "\n--- 联系人列表（共 " << abs->size << " 人）---" << endl;

    // 打印表头
    cout << "----------------------------------------";
    cout << "----------------------------------------" << endl;
    cout << "序号\t姓名\t\t性别\t年龄\t电话\t\t住址" << endl;
    cout << "----------------------------------------";
    cout << "----------------------------------------" << endl;

    /*
     * 遍历并打印每个联系人的信息
     * 使用\t（制表符）来对齐列
     * 注意：中文字符宽度不一致，对齐可能不完美
     */
    for (int i = 0; i < abs->size; i++)
    {
        cout << (i + 1) << "\t";                           // 序号（从1开始显示）
        cout << abs->contacts[i].name << "\t\t";           // 姓名
        cout << abs->contacts[i].gender << "\t";           // 性别
        cout << abs->contacts[i].age << "\t";              // 年龄
        cout << abs->contacts[i].phone << "\t";            // 电话
        cout << abs->contacts[i].address << endl;          // 住址
    }

    cout << "----------------------------------------";
    cout << "----------------------------------------" << endl;
    cout << "共 " << abs->size << " 位联系人" << endl;
}

/*
 * deleteContact() - 删除联系人
 *
 * 参数：abs - 通讯录指针
 *
 * 功能流程：
 * 1. 提示用户输入要删除的联系人姓名
 * 2. 在通讯录中查找该联系人
 * 3. 如果找到，显示联系人信息并确认是否删除
 * 4. 用户确认后，将后面的联系人依次前移覆盖
 * 5. 更新联系人计数
 *
 * 设计思考：删除操作为什么需要"前移覆盖"？
 * 数组是连续存储的，不能直接"留空"。
 * 所以删除某个元素后，需要把它后面的所有元素向前移动一位，
 * 填补空出来的位置。这也是数组删除操作的时间复杂度为O(n)的原因。
 */
void deleteContact(AddressBook* abs)
{
    // 检查通讯录是否为空
    if (abs->size == 0)
    {
        cout << "\n[提示] 通讯录为空，没有可删除的联系人。" << endl;
        return;
    }

    cout << "\n--- 删除联系人 ---" << endl;

    // 步骤1：获取要删除的联系人姓名
    string name;
    cout << "请输入要删除的联系人姓名: ";
    cin >> name;

    // 步骤2：查找联系人
    int index = findByName(abs, name);

    // 步骤3：判断是否找到
    if (index == -1)
    {
        cout << "[提示] 未找到姓名为 \"" << name << "\" 的联系人。" << endl;
        return;
    }

    // 步骤4：显示找到的联系人信息，让用户确认
    cout << "\n找到以下联系人：" << endl;
    cout << "  姓名: " << abs->contacts[index].name << endl;
    cout << "  性别: " << abs->contacts[index].gender << endl;
    cout << "  年龄: " << abs->contacts[index].age << endl;
    cout << "  电话: " << abs->contacts[index].phone << endl;
    cout << "  住址: " << abs->contacts[index].address << endl;

    // 步骤5：确认删除（防止误操作）
    string confirm;
    cout << "\n确认删除该联系人？(y/n): ";
    cin >> confirm;

    if (confirm != "y" && confirm != "Y")
    {
        cout << "[提示] 已取消删除操作。" << endl;
        return;
    }

    /*
     * 步骤6：执行删除 - 数据前移覆盖
     *
     * 举例说明：
     * 假设有5个联系人 [A, B, C, D, E]，要删除B（索引1）
     * 第1次循环：contacts[1] = contacts[2]  -> [A, C, C, D, E]
     * 第2次循环：contacts[2] = contacts[3]  -> [A, C, D, D, E]
     * 第3次循环：contacts[3] = contacts[4]  -> [A, C, D, E, E]
     * 然后size减1：size = 4
     * 有效数据变为 [A, C, D, E]，最后的E虽然还在但不会被访问
     */
    for (int i = index; i < abs->size - 1; i++)
    {
        abs->contacts[i] = abs->contacts[i + 1];
    }

    // 步骤7：更新联系人数量
    abs->size--;

    cout << "\n[成功] 联系人 \"" << name << "\" 已成功删除。" << endl;
    cout << "       当前通讯录共有 " << abs->size << " 位联系人。" << endl;
}

/*
 * searchContact() - 查找联系人
 *
 * 参数：abs - 通讯录指针
 *
 * 功能：按姓名精确查找联系人，找到后显示其详细信息。
 *
 * 设计说明：这里实现的是精确查找（完全匹配姓名）。
 * 实际项目中可以扩展为模糊查找（包含关键字），
 * 甚至支持按电话号码、住址等多条件查找。
 */
void searchContact(AddressBook* abs)
{
    if (abs->size == 0)
    {
        cout << "\n[提示] 通讯录为空，没有可查找的联系人。" << endl;
        return;
    }

    cout << "\n--- 查找联系人 ---" << endl;

    // 获取要查找的姓名
    string name;
    cout << "请输入要查找的联系人姓名: ";
    cin >> name;

    // 调用辅助函数查找
    int index = findByName(abs, name);

    if (index == -1)
    {
        cout << "[提示] 未找到姓名为 \"" << name << "\" 的联系人。" << endl;
        cout << "       请检查姓名是否正确。" << endl;
        return;
    }

    // 显示找到的联系人详细信息
    cout << "\n查找到联系人信息如下：" << endl;
    cout << "┌──────────────────────────────────┐" << endl;
    cout << "│  姓名: " << abs->contacts[index].name << endl;
    cout << "│  性别: " << abs->contacts[index].gender << endl;
    cout << "│  年龄: " << abs->contacts[index].age << endl;
    cout << "│  电话: " << abs->contacts[index].phone << endl;
    cout << "│  住址: " << abs->contacts[index].address << endl;
    cout << "└──────────────────────────────────┘" << endl;
}

/*
 * modifyContact() - 修改联系人信息
 *
 * 参数：abs - 通讯录指针
 *
 * 功能流程：
 * 1. 按姓名查找要修改的联系人
 * 2. 显示当前信息
 * 3. 逐项询问是否修改，用户输入新值则更新
 * 4. 如果用户直接回车（不输入），则保留原值（简化版直接要求输入）
 *
 * 设计说明：修改功能是查找功能的扩展。
 * 先找到目标联系人，再允许用户修改其各项信息。
 * 这里采用"每项都重新输入"的简化方式，
 * 实际项目中可以实现"只修改指定字段"的功能。
 */
void modifyContact(AddressBook* abs)
{
    if (abs->size == 0)
    {
        cout << "\n[提示] 通讯录为空，没有可修改的联系人。" << endl;
        return;
    }

    cout << "\n--- 修改联系人 ---" << endl;

    // 步骤1：获取要修改的联系人姓名
    string name;
    cout << "请输入要修改的联系人姓名: ";
    cin >> name;

    // 步骤2：查找联系人
    int index = findByName(abs, name);

    if (index == -1)
    {
        cout << "[提示] 未找到姓名为 \"" << name << "\" 的联系人。" << endl;
        return;
    }

    // 步骤3：显示当前信息
    cout << "\n当前联系人信息：" << endl;
    cout << "  姓名: " << abs->contacts[index].name << endl;
    cout << "  性别: " << abs->contacts[index].gender << endl;
    cout << "  年龄: " << abs->contacts[index].age << endl;
    cout << "  电话: " << abs->contacts[index].phone << endl;
    cout << "  住址: " << abs->contacts[index].address << endl;

    // 步骤4：确认修改
    string confirm;
    cout << "\n确认修改该联系人信息？(y/n): ";
    cin >> confirm;

    if (confirm != "y" && confirm != "Y")
    {
        cout << "[提示] 已取消修改操作。" << endl;
        return;
    }

    // 步骤5：逐项修改
    cout << "\n请重新输入联系人信息（每项必填）：" << endl;

    // 修改姓名
    cout << "请输入新姓名（当前: " << abs->contacts[index].name << "）: ";
    cin >> abs->contacts[index].name;

    // 修改性别（带验证）
    string gender;
    while (true)
    {
        cout << "请输入新性别（当前: " << abs->contacts[index].gender << "）: ";
        cin >> gender;
        if (gender == "男" || gender == "女")
        {
            abs->contacts[index].gender = gender;
            break;
        }
        cout << "[提示] 性别只能输入\"男\"或\"女\"。" << endl;
    }

    // 修改年龄（带验证）
    int age;
    while (true)
    {
        cout << "请输入新年龄（当前: " << abs->contacts[index].age << "）: ";
        cin >> age;
        if (cin.fail())
        {
            cin.clear();
            clearInputBuffer();
            cout << "[提示] 请输入有效的数字！" << endl;
            continue;
        }
        if (age > 0 && age <= 150)
        {
            abs->contacts[index].age = age;
            break;
        }
        cout << "[提示] 年龄应在1-150之间。" << endl;
    }

    // 修改电话
    cout << "请输入新电话（当前: " << abs->contacts[index].phone << "）: ";
    cin >> abs->contacts[index].phone;

    // 修改住址
    cout << "请输入新住址（当前: " << abs->contacts[index].address << "）: ";
    cin >> abs->contacts[index].address;

    cout << "\n[成功] 联系人信息已更新！" << endl;
}

/*
 * clearContacts() - 清空所有联系人
 *
 * 参数：abs - 通讯录指针
 *
 * 功能：清空通讯录中的所有联系人数据。
 *
 * 设计思考：为什么需要二次确认？
 * 清空操作是不可逆的（没有撤销功能），一旦执行所有数据都会丢失。
 * 所以必须让用户确认两次，防止因误操作导致数据丢失。
 * 这种设计在实际软件中非常常见（如删除文件时的确认对话框）。
 */
void clearContacts(AddressBook* abs)
{
    if (abs->size == 0)
    {
        cout << "\n[提示] 通讯录已经是空的，无需清空。" << endl;
        return;
    }

    cout << "\n--- 清空联系人 ---" << endl;
    cout << "[警告] 此操作将删除通讯录中的所有 " << abs->size << " 位联系人！" << endl;
    cout << "       此操作不可撤销！" << endl;

    // 第一次确认
    string confirm;
    cout << "\n确认清空所有联系人？(y/n): ";
    cin >> confirm;

    if (confirm != "y" && confirm != "Y")
    {
        cout << "[提示] 已取消清空操作。" << endl;
        return;
    }

    // 第二次确认（安全措施）
    cout << "再次确认：真的要清空所有数据吗？(输入yes确认): ";
    cin >> confirm;

    if (confirm != "yes")
    {
        cout << "[提示] 已取消清空操作。" << endl;
        return;
    }

    /*
     * 执行清空操作
     *
     * 技术说明：清空通讯录只需要将size设置为0即可。
     * 为什么不需要清空数组内容？
     * 因为size字段控制了有效数据的范围，
     * size=0意味着没有任何有效数据，数组中的旧数据虽然还在，
     * 但会被视为"未使用"的空间，后续添加新数据时会被覆盖。
     *
     * 如果出于安全考虑（比如防止内存信息泄露），可以使用memset清零：
     * memset(abs->contacts, 0, sizeof(abs->contacts));
     * 但对于学习项目来说，设置size=0就足够了。
     */
    abs->size = 0;

    cout << "\n[成功] 通讯录已清空！当前联系人数量为 0。" << endl;
}

/*
 * ============================================================================
 * 【项目总结与扩展思考】
 * ============================================================================
 *
 * 1. 本项目综合运用了以下C++基础知识：
 *    - 结构体（struct）定义自定义数据类型
 *    - 函数封装实现模块化编程
 *    - 数组存储和管理数据
 *    - 指针传递实现函数间数据共享
 *    - 循环和条件判断控制程序流程
 *    - 输入输出和基本的输入验证
 *
 * 2. 可以进一步扩展的功能：
 *    - 使用文件I/O将数据保存到文件，实现持久化存储
 *    - 支持模糊查找（包含关键字即匹配）
 *    - 支持按多个条件组合查找
 *    - 使用vector替代固定大小数组，实现动态扩容
 *    - 使用类（class）重构，实现面向对象设计
 *    - 添加排序功能（按姓名、年龄等排序）
 *    - 添加导入/导出功能（CSV格式等）
 *
 * 3. 代码质量改进建议：
 *    - 输入验证可以更加完善（如电话号码格式验证）
 *    - 可以使用枚举（enum）替代魔术数字（0-6）
 *    - 错误处理可以更加统一和规范
 * ============================================================================
 */
