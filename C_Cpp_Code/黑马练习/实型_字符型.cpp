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
    float f2 =3e2 ;//3*10^2
    
    cout << "f2=" << f2 <<endl;
    
    //1、字符型变量创建方式
    //单引号
    //2、字符型变量占用内存空间  可以sizeof
    cout << "char占用的内存空间是：" << sizeof(char) << endl;//1 byte
    //3、字符型变量只能存储一个字符
    char ch = 'a';//不能用双引号，要用单引号
    /*“djfuh”这种就不行，单引号内只能有一个字符 */

    //4、字符型变量可以存储数字  因为字符型变量底层是以ASCII码的形式存储的
    cout  << int(ch) << endl;//强制类型转换，输出字符a对应的ASCII码值
    //a - 97
    //A - 65
    //0 - 48
    

    char hu = 'x';
    
    cout << hu << endl;

    return 0;

}
