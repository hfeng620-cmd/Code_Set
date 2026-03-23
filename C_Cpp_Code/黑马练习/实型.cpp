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

    cout << "f2=" << f2 <<endl;
    return 0;

}
