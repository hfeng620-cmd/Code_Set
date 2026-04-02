#include<iostream>

using namespace std;

#include <string>

int main() {

    //1、 转义字符  \n  \t  \\  \"  \'

    cout << "hello world" << endl;//endl是换行

    cout << "hello\tworld" << endl;//\t是制表符，输出一个tab键的空格

    cout << "hello\\world" << endl;//\\输出一个\

    cout << "hello\"world\"" << endl;//\"输出一个"

    cout << "hello\'world\'" << endl;//\'输出一个'

    /*
    常用的转义字符：
    \n  换行
    \t  制表符  
    \\  输出一个\
    \"  输出一个"
    \'  输出一个'       
    */
    
    cout << "hello world\n";
    
    //反斜杠   \\
    
    cout << "\\"<<endl;

    //水平制表符   \t
    cout << "aaa\t hello world" << endl;
    
    cout << "aa\t hello world" << endl;
    
    cout << "a\t hello world" << endl;

    /*C语言风格创建字符串*/char ch1[]= "hello world";//字符数组的形式创建字符串，字符串常量 "hello world" 以\0结尾的字符数组的形式存在的
    
        /*要加[]*/

    /*C++风格创建字符串*/string str1= "hello world";//变量名➕=字符串常量
    
    //字符串常量  "hello world"  是一个字符串常量，字符串常量在内存中是以\0结尾的字符数组的形式存在的
    
    cout << ch1 << endl;

    cout << str1 << endl;

    cout << ch1 <<endl;
    
    bool flag = true;//true 1  false 0
    
    cout << flag << endl;
    
    flag = false;   
    
    cout << flag << endl;

    cout << "bool占用的内存空间是：" << sizeof(bool) << endl;//1 byte

    cout << "请给整形变量a赋值";
    int a;
    //整形
    cin >> a;
    cout << "a的值是：" << a << endl;
    //浮点型
    cout << "请给浮点型变量b赋值";
    float b;
    cin >> b;
    cout << "b的值是：" << b << endl;
    
    //字符型
    cout << "请给字符型变量c赋值";
    char c;
    cin >> c;
    cout << "c的值是：" << c << endl;   
    //字符串型
    cout << "请给字符串型变量str赋值";
    string str;
    cin >> str;
    cout << "str的值是：" << str << endl;

    //布尔型
    cout << "请给布尔型变量flag赋值";
    cin >> flag;
    cout << "flag的值是：" << flag << endl; 
    //输入布尔型变量时，输入0表示false，输入非0表示true

    return 0;

}