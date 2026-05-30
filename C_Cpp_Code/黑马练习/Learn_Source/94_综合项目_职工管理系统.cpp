/*
 * ============================================================================
 * 黑马程序员 C++课程 - 综合项目：职工管理系统
 * ============================================================================
 *
 * 【项目背景】
 * 职工管理系统是黑马程序员C++面向对象（OOP）部分的核心综合项目。
 * 综合运用了：类与对象、继承、多态、抽象类、文件操作等OOP核心概念。
 *
 * 【功能需求】
 * 1. 增加职工  2. 显示职工  3. 删除职工  4. 修改职工
 * 5. 查找职工  6. 排序      7. 清空      0. 退出
 *
 * 【类设计】（核心OOP设计）
 *                    Worker (抽象基类)
 *                   /      |       \
 *           Employee     Manager     Boss
 *          (普通员工)    (经理)      (老板)
 *
 * 【技术要点】
 * - 多态：通过基类指针调用不同子类的showInfo()
 * - 抽象类：Worker不能直接实例化，只定义接口
 * - 虚析构函数：确保通过基类指针删除子类对象时正确释放资源
 * - 文件I/O：职工数据持久化存储到文件
 * - vector容器：动态管理职工列表
 *
 * 编译方法：g++ 94_综合项目_职工管理系统.cpp -o employee_mgr
 * ============================================================================
 */

#include <iostream>    // 标准输入输出
#include <string>      // 字符串类
#include <vector>      // 动态数组容器
#include <fstream>     // 文件流操作
#include <algorithm>   // STL算法（sort等）
#include <limits>      // 数值极限（用于清空缓冲区）

using namespace std;

// ============================================================================
// 【常量定义】
// ============================================================================

// 职工岗位编号常量：使用有意义的常量名比魔术数字更易读、易维护
const int EMPLOYEE = 1;   // 普通员工
const int MANAGER  = 2;   // 经理
const int BOSS     = 3;   // 老板
const string DATA_FILE = "employees.txt";  // 数据文件路径

// ============================================================================
// 【抽象基类：Worker】
// ============================================================================

/*
 * Worker 抽象基类
 *
 * 三种职工有共同的属性（编号、姓名、岗位编号）和共同的行为（显示信息、获取部门名称），
 * 但具体实现不同。抽象基类定义了"接口规范"，子类提供具体实现。
 * 通过基类指针/引用调用虚函数时，会根据实际对象类型调用对应子类的实现（运行时多态）。
 */
class Worker
{
public:
    /*
     * 虚析构函数：当通过基类指针（Worker*）删除子类对象时，
     * 如果析构函数不是virtual的，只会调用基类的析构函数，
     * 子类的析构函数不会被调用，导致内存泄漏。
     * 声明为virtual后，delete基类指针时会先调用子类析构函数再调用基类析构函数。
     */
    virtual ~Worker() {}

    // 纯虚函数：包含纯虚函数的类是抽象类，不能创建对象，子类必须重写所有纯虚函数
    virtual void showInfo() = 0;       // 显示职工信息
    virtual string getDeptName() = 0;  // 获取部门名称

    // 公共属性（所有职工共有）
    int m_Id;          // 职工编号
    string m_Name;     // 职工姓名
    int m_DeptId;      // 岗位编号（1=员工, 2=经理, 3=老板）
};

// ============================================================================
// 【子类：Employee（普通员工）】
// ============================================================================

/*
 * public继承表示"is-a"关系：Employee is a Worker。
 * 公有继承保证基类的public成员在派生类中仍然是public的，
 * 这样可以通过Worker*指针调用Employee的方法。
 */
class Employee : public Worker
{
public:
    Employee(int id, string name)
    {
        m_Id = id;
        m_Name = name;
        m_DeptId = EMPLOYEE;  // 岗位编号固定为1
    }

    // 重写纯虚函数：virtual关键字可选（基类已声明为virtual），但写上提高可读性
    virtual void showInfo() override
    {
        cout << "职工编号: " << m_Id
             << "\t职工姓名: " << m_Name
             << "\t岗位: " << getDeptName()
             << "\t职责: 完成经理交给的任务" << endl;
    }

    virtual string getDeptName() override
    {
        return "普通员工";
    }
};

// ============================================================================
// 【子类：Manager（经理）】
// ============================================================================

/*
 * 通过继承，Manager复用了Worker的属性（id, name, deptId），
 * 只需要重写不同的行为（showInfo, getDeptName），体现面向对象的"代码复用"。
 */
class Manager : public Worker
{
public:
    Manager(int id, string name)
    {
        m_Id = id;
        m_Name = name;
        m_DeptId = MANAGER;
    }

    virtual void showInfo() override
    {
        cout << "职工编号: " << m_Id
             << "\t职工姓名: " << m_Name
             << "\t岗位: " << getDeptName()
             << "\t职责: 完成老板交给的任务，并下发给员工" << endl;
    }

    virtual string getDeptName() override { return "经理"; }
};

// ============================================================================
// 【子类：Boss（老板）】
// ============================================================================

class Boss : public Worker
{
public:
    Boss(int id, string name)
    {
        m_Id = id;
        m_Name = name;
        m_DeptId = BOSS;
    }

    virtual void showInfo() override
    {
        cout << "职工编号: " << m_Id
             << "\t职工姓名: " << m_Name
             << "\t岗位: " << getDeptName()
             << "\t职责: 管理公司所有事务" << endl;
    }

    virtual string getDeptName() override { return "总裁"; }
};

// ============================================================================
// 【管理类：WorkerManager】
// ============================================================================

/*
 * 将所有管理操作（增删改查、文件操作等）封装在一个类中，
 * 体现了面向对象的"单一职责原则"和"封装"思想。
 *
 * 职工存储：使用 vector<Worker*> 存储基类指针。
 * 通过多态，每个指针可以指向Employee、Manager或Boss对象。
 * 调用showInfo()时会自动调用对应子类的实现。
 *
 * 实际项目中建议使用 unique_ptr<Worker> 自动管理内存。
 */
class WorkerManager
{
public:
    // 构造函数：初始化时尝试从文件加载数据
    WorkerManager()
    {
        loadFromFile();
    }

    // 析构函数：释放所有职工对象的内存
    // vector中存储的是指针，vector析构时不会自动delete指针指向的对象
    ~WorkerManager()
    {
        for (size_t i = 0; i < m_Workers.size(); i++)
        {
            if (m_Workers[i] != nullptr)
            {
                delete m_Workers[i];      // 释放职工对象
                m_Workers[i] = nullptr;    // 置空指针，防止野指针
            }
        }
        m_Workers.clear();
    }

    // ---- 显示菜单 ----
    void showMenu()
    {
        cout << "\n╔══════════════════════════════════════╗" << endl;
        cout << "║       欢迎使用职工管理系统 v2.0       ║" << endl;
        cout << "╠══════════════════════════════════════╣" << endl;
        cout << "║  1. 增加职工                          ║" << endl;
        cout << "║  2. 显示职工                          ║" << endl;
        cout << "║  3. 删除职工                          ║" << endl;
        cout << "║  4. 修改职工                          ║" << endl;
        cout << "║  5. 查找职工                          ║" << endl;
        cout << "║  6. 按编号排序                        ║" << endl;
        cout << "║  7. 清空所有                          ║" << endl;
        cout << "║  0. 退出系统                          ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
    }

    // ---- 主循环：显示菜单 -> 获取输入 -> 调用功能 ----
    void run()
    {
        int select = 0;
        while (true)
        {
            showMenu();
            cout << "\n请输入您的选择: ";
            cin >> select;

            // 输入验证：如果输入不是数字，清除错误标志和缓冲区
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[错误] 请输入有效的数字！" << endl;
                continue;
            }

            switch (select)
            {
            case 1: addWorker();     break;
            case 2: showWorkers();   break;
            case 3: deleteWorker();  break;
            case 4: modifyWorker();  break;
            case 5: findWorker();    break;
            case 6: sortWorkers();   break;
            case 7: clearAll();      break;
            case 0:
                saveToFile();
                cout << "\n[系统] 数据已保存，感谢使用，再见！" << endl;
                return;
            default:
                cout << "[提示] 无效选择，请输入0-7。" << endl;
                break;
            }

            // 暂停让用户查看结果
            cout << "\n按回车键继续...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }

    // ========================================================================
    // 【功能1：增加职工】
    // ========================================================================

    /*
     * 多态的体现：无论创建的是Employee、Manager还是Boss，
     * 都以Worker*指针的形式存入vector。后续调用showInfo()时，
     * 会根据实际类型调用对应的实现（向上转型 upcasting）。
     */
    void addWorker()
    {
        cout << "\n--- 增加职工 ---" << endl;

        // 选择职工类型
        int deptSelect = 0;
        cout << "请选择职工岗位：" << endl;
        cout << "  1. 普通员工" << endl;
        cout << "  2. 经理" << endl;
        cout << "  3. 老板" << endl;
        cout << "请输入选择(1-3): ";
        cin >> deptSelect;

        if (deptSelect < 1 || deptSelect > 3)
        {
            cout << "[错误] 无效的岗位选择！" << endl;
            return;
        }

        // 输入职工姓名
        string name;
        cout << "请输入职工姓名: ";
        cin >> name;

        // 生成职工编号：当前最大编号 + 1，空系统从1001开始
        int newId = 1001;
        if (!m_Workers.empty())
        {
            int maxId = 0;
            for (size_t i = 0; i < m_Workers.size(); i++)
            {
                if (m_Workers[i]->m_Id > maxId)
                    maxId = m_Workers[i]->m_Id;
            }
            newId = maxId + 1;
        }

        // 根据岗位类型创建对应的子类对象（向上转型为Worker*）
        Worker* worker = nullptr;
        switch (deptSelect)
        {
        case EMPLOYEE: worker = new Employee(newId, name); break;
        case MANAGER:  worker = new Manager(newId, name);  break;
        case BOSS:     worker = new Boss(newId, name);     break;
        }

        m_Workers.push_back(worker);

        cout << "\n[成功] 职工添加成功！" << endl;
        cout << "  编号: " << newId << "  姓名: " << name
             << "  岗位: " << worker->getDeptName() << endl;
        cout << "  当前职工总数: " << m_Workers.size() << endl;
    }

    // ========================================================================
    // 【功能2：显示所有职工】 - 多态的典型应用
    // ========================================================================

    /*
     * 遍历vector中的所有Worker*指针，调用统一的showInfo()接口。
     * 虽然代码写的是同一个函数调用，但根据指针实际指向的对象类型不同，
     * 会调用Employee、Manager或Boss各自的showInfo()实现。
     */
    void showWorkers()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中暂无职工信息。" << endl;
            return;
        }

        cout << "\n--- 职工列表（共 " << m_Workers.size() << " 人）---" << endl;
        cout << "--------------------------------------------------------" << endl;

        for (size_t i = 0; i < m_Workers.size(); i++)
        {
            // 多态调用：根据实际对象类型调用对应的showInfo()
            m_Workers[i]->showInfo();
        }

        cout << "--------------------------------------------------------" << endl;
    }

    // ========================================================================
    // 【功能3：删除职工】
    // ========================================================================

    /*
     * 从vector中移除指针前，必须先delete释放对象内存，否则内存泄漏。
     * erase()会将后面的元素前移，自动调整vector大小。
     */
    void deleteWorker()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中暂无职工信息。" << endl;
            return;
        }

        cout << "\n--- 删除职工 ---" << endl;
        int id;
        cout << "请输入要删除的职工编号: ";
        cin >> id;

        int index = findIndexById(id);
        if (index == -1)
        {
            cout << "[提示] 未找到编号为 " << id << " 的职工。" << endl;
            return;
        }

        // 显示信息并确认删除
        cout << "\n找到以下职工：" << endl;
        m_Workers[index]->showInfo();

        string confirm;
        cout << "\n确认删除该职工？(y/n): ";
        cin >> confirm;
        if (confirm != "y" && confirm != "Y")
        {
            cout << "[提示] 已取消删除操作。" << endl;
            return;
        }

        // 先释放对象内存，再从vector中移除指针
        delete m_Workers[index];
        m_Workers.erase(m_Workers.begin() + index);

        cout << "\n[成功] 职工已删除。当前职工总数: " << m_Workers.size() << endl;
    }

    // ========================================================================
    // 【功能4：修改职工】
    // ========================================================================

    /*
     * 修改策略：找到目标职工后，删除旧对象，创建新对象替换。
     * 另一种方式是给Worker类添加setter方法直接修改属性。
     */
    void modifyWorker()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中暂无职工信息。" << endl;
            return;
        }

        cout << "\n--- 修改职工 ---" << endl;
        int id;
        cout << "请输入要修改的职工编号: ";
        cin >> id;

        int index = findIndexById(id);
        if (index == -1)
        {
            cout << "[提示] 未找到编号为 " << id << " 的职工。" << endl;
            return;
        }

        cout << "\n当前职工信息：" << endl;
        m_Workers[index]->showInfo();

        string name;
        cout << "\n请输入新姓名: ";
        cin >> name;

        int deptSelect;
        cout << "请选择新岗位（1.普通员工 2.经理 3.老板）: ";
        cin >> deptSelect;

        if (deptSelect < 1 || deptSelect > 3)
        {
            cout << "[错误] 无效的岗位选择！" << endl;
            return;
        }

        // 保存原编号，删除旧对象，创建新对象
        int oldId = m_Workers[index]->m_Id;
        delete m_Workers[index];

        switch (deptSelect)
        {
        case EMPLOYEE: m_Workers[index] = new Employee(oldId, name); break;
        case MANAGER:  m_Workers[index] = new Manager(oldId, name);  break;
        case BOSS:     m_Workers[index] = new Boss(oldId, name);     break;
        }

        cout << "\n[成功] 职工信息已更新：" << endl;
        m_Workers[index]->showInfo();
    }

    // ========================================================================
    // 【功能5：查找职工】
    // ========================================================================

    void findWorker()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中暂无职工信息。" << endl;
            return;
        }

        cout << "\n--- 查找职工 ---" << endl;
        cout << "1. 按编号查找" << endl;
        cout << "2. 按姓名查找" << endl;

        int select;
        cout << "请选择查找方式: ";
        cin >> select;

        if (select == 1)
        {
            // 按编号查找
            int id;
            cout << "请输入职工编号: ";
            cin >> id;
            int index = findIndexById(id);

            if (index != -1)
            {
                cout << "\n查找到职工信息：" << endl;
                m_Workers[index]->showInfo();
            }
            else
            {
                cout << "[提示] 未找到编号为 " << id << " 的职工。" << endl;
            }
        }
        else if (select == 2)
        {
            // 按姓名查找（可能有多人同名）
            string name;
            cout << "请输入职工姓名: ";
            cin >> name;

            bool found = false;
            cout << "\n查找结果：" << endl;
            for (size_t i = 0; i < m_Workers.size(); i++)
            {
                if (m_Workers[i]->m_Name == name)
                {
                    m_Workers[i]->showInfo();
                    found = true;
                }
            }
            if (!found)
                cout << "[提示] 未找到姓名为 \"" << name << "\" 的职工。" << endl;
        }
        else
        {
            cout << "[错误] 无效的选择！" << endl;
        }
    }

    // ========================================================================
    // 【功能6：排序】 - 使用STL sort + lambda表达式
    // ========================================================================

    /*
     * sort(begin, end, comp)：对vector排序。
     * lambda表达式语法：[捕获列表](参数列表){函数体}
     * 这里的lambda比较两个Worker*指针指向对象的m_Id
     */
    void sortWorkers()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中暂无职工信息。" << endl;
            return;
        }

        cout << "\n--- 排序 ---" << endl;
        cout << "1. 按编号升序排列" << endl;
        cout << "2. 按编号降序排列" << endl;

        int select;
        cout << "请选择排序方式: ";
        cin >> select;

        if (select == 1)
        {
            // 升序：编号小的在前，lambda返回true表示a应排在b前面
            sort(m_Workers.begin(), m_Workers.end(),
                 [](Worker* a, Worker* b) { return a->m_Id < b->m_Id; });
            cout << "[成功] 已按编号升序排列。" << endl;
        }
        else if (select == 2)
        {
            // 降序：编号大的在前
            sort(m_Workers.begin(), m_Workers.end(),
                 [](Worker* a, Worker* b) { return a->m_Id > b->m_Id; });
            cout << "[成功] 已按编号降序排列。" << endl;
        }
        else
        {
            cout << "[错误] 无效的选择！" << endl;
            return;
        }

        showWorkers();  // 显示排序后的结果
    }

    // ========================================================================
    // 【功能7：清空所有】
    // ========================================================================

    void clearAll()
    {
        if (m_Workers.empty())
        {
            cout << "\n[提示] 系统中已无职工信息。" << endl;
            return;
        }

        cout << "\n--- 清空所有职工 ---" << endl;
        cout << "[警告] 此操作将删除所有 " << m_Workers.size() << " 名职工信息！" << endl;

        string confirm;
        cout << "确认清空？(y/n): ";
        cin >> confirm;
        if (confirm != "y" && confirm != "Y")
        {
            cout << "[提示] 已取消清空操作。" << endl;
            return;
        }

        // 释放所有对象的内存
        for (size_t i = 0; i < m_Workers.size(); i++)
        {
            delete m_Workers[i];
            m_Workers[i] = nullptr;
        }
        m_Workers.clear();

        // 同时清空文件数据
        ofstream ofs(DATA_FILE, ios::trunc);
        ofs.close();

        cout << "[成功] 所有职工信息已清空。" << endl;
    }

    // ========================================================================
    // 【文件操作：保存和加载】
    // ========================================================================

    /*
     * 文件格式（每行一个职工）：编号\t姓名\t岗位编号
     * 只保存这三个字段，其他信息（部门名称、职责）可通过岗位编号推导。
     */
    void saveToFile()
    {
        ofstream ofs(DATA_FILE, ios::out);
        if (!ofs.is_open())
        {
            cout << "[错误] 无法打开文件进行保存：" << DATA_FILE << endl;
            return;
        }

        for (size_t i = 0; i < m_Workers.size(); i++)
        {
            ofs << m_Workers[i]->m_Id << "\t"
                << m_Workers[i]->m_Name << "\t"
                << m_Workers[i]->m_DeptId << endl;
        }
        ofs.close();
    }

    /*
     * 从文件加载职工数据。文件不存在说明是第一次运行，初始化为空。
     * 根据岗位编号创建对应的子类对象，实现多态存储。
     */
    void loadFromFile()
    {
        ifstream ifs(DATA_FILE, ios::in);
        if (!ifs.is_open())
        {
            cout << "[系统] 未找到数据文件，将初始化为空系统。" << endl;
            return;
        }

        int id, deptId;
        string name;
        while (ifs >> id >> name >> deptId)
        {
            Worker* worker = nullptr;
            switch (deptId)
            {
            case EMPLOYEE: worker = new Employee(id, name); break;
            case MANAGER:  worker = new Manager(id, name);  break;
            case BOSS:     worker = new Boss(id, name);     break;
            default:
                cout << "[警告] 跳过无效记录：编号=" << id << endl;
                continue;
            }
            m_Workers.push_back(worker);
        }
        ifs.close();

        if (!m_Workers.empty())
            cout << "[系统] 已从文件加载 " << m_Workers.size() << " 名职工数据。" << endl;
    }

private:
    vector<Worker*> m_Workers;  // 职工列表：存储基类指针，可指向任何子类对象

    // 根据编号查找职工在vector中的索引，未找到返回-1（内部辅助函数）
    int findIndexById(int id)
    {
        for (size_t i = 0; i < m_Workers.size(); i++)
        {
            if (m_Workers[i]->m_Id == id)
                return static_cast<int>(i);
        }
        return -1;
    }
};

// ============================================================================
// 【主函数】
// ============================================================================

/*
 * 创建WorkerManager对象，构造函数自动从文件加载历史数据。
 * 整个系统的生命周期由这个对象管理，体现了RAII（资源获取即初始化）的思想。
 */
int main()
{
    WorkerManager wm;

    cout << "\n========================================" << endl;
    cout << "    职工管理系统已启动" << endl;
    cout << "========================================" << endl;

    wm.run();  // 启动主循环
    return 0;
}

/*
 * ============================================================================
 * 【项目总结与OOP设计要点】
 * ============================================================================
 *
 * 1. 多态：Worker* 指针指向不同子类对象，调用 showInfo() 时执行不同实现
 * 2. 抽象类：Worker 定义接口规范，不能实例化，确保子类实现必要方法
 * 3. 虚析构函数：通过基类指针 delete 子类对象时，确保子类析构函数被调用
 * 4. 继承与复用：三个子类复用 Worker 的属性，只重写差异化的部分
 * 5. 封装：WorkerManager 将管理逻辑封装，main() 只需创建对象并调用 run()
 * 6. 改进方向：使用 vector<unique_ptr<Worker>> 自动管理内存，更安全
 * ============================================================================
 */
