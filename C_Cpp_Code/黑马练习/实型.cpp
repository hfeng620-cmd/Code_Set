#include<iostream>
using namespace std;
int main() {
    //1、 单精度 float
    //2、 双精度 double
    float f1 = 3.14f;
     
    cout << "f1 = " << f1 << endl;

    double d1 = 3.14 ;

    cout << "d1 = " << d1 << endl;

    cout <<"float占用的内存空间是：" << sizeof(float) <<endl ;//4 byte
    
    cout << "double 占用的内存空间是：" << sizeof(double) << endl ;
    
    //科学计数法
    float f2 =3e2 ;//3*10**2
    //取模运算本质上是除法运算，结果是商的小数部分
    int a3 =10;
    int b3=3;
    cout << "a3%b3=" << a3%b3 <<endl;
    //两个小b不可以做取模运算
    double d2 = 3e-2;//3*10**-2
    double d3 = 3.25;//3*10**-2
    // cout << "d2%d3=" << d2 % d3 <<endl;
    //只有整形可以进行qu取模运算，浮点数不可以进行取模运算 
   //1.前置递增：++a3; 先自增1，再使用
    //2.后置递增：a3++; 先使用，再自增1
    int a=10;
    ++a;
    cout << "a=" << a <<endl;
    
    int b=10;
    b++;
    cout << "b=" << b <<endl;
// qian zhi he hou zhi的区别在于：前置递增先自增1，再使用；后置递增先使用，再自增1
    // = + - * / % ++ --
   //!逻辑非运算符：对一个表达式取反，结果是一个bool值
    bool flag = true;
    cout << "flag=" << flag <<endl;
    cout << "!flag=" << !flag <<endl;
    //&&逻辑与运算符：当两个表达式都为true时，结果才为true
    bool flag1 = true;
    bool flag2 = false;
    cout << "flag1 && flag2=" << (flag1 && flag2) <<endl;
    //||逻辑或运算符：当两个表达式都为false时，结果才为false
    cout << "flag1 || flag2=" << (flag1 || flag2) <<endl; 
    cout << "f2=" << f2 <<endl;
    return 0;
   
}
