#include<iostream>
using namespace std;
int main() {

    int a = 10;
    
    cout << "a = " << a << endl;

    a++;//a=a+1; a+=1; a++
    
    cout << "a = " << a << endl;

    ++a;//a=a+1; a+=1; ++a

    cout << "a = " << a << endl;
    //前置和后置的区别
    //++a 先对a进行自增，再使用a的值
    // a++ 先使用a的值，再对a进行自增
    int a2 = 10;
    int b2 = ++a2*10;//a2先自增为11，再和10相乘，b2的值是110
    cout << "a2 = " << a2 << endl;//11
    cout << "b2 = " << b2 << endl;//110

    int a3 = 10;
    int b3 = a3++*10;//a3先和10相乘，再对a3进行自增，b3的值是100
    //b3的值是100，再对a3进行自增，a3的值是11
    cout << "a3 = " << a3 << endl;//11
    cout << "a3 "<< a3++ << endl;//先输出a3的值11，再对a3进行自增，a3的值是12
    cout << "a3 = " << a3 << endl;
    cout << "b3 = " << b3 << endl;//100

    return 0;
}