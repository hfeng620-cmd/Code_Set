#include<iostream>
using namespace std;

int main() {
//三目运算符

//创建三个变量abc
//将a和b做一个比较
int a=10;
int b =20;
int c=0;
c=(a>b ?a:b);
cout <<"c="<<c<<endl;
//在C++中三目运算符返回的是变量，可以继续复制
(a>b?a:b)=100;
cout <<"a="<<a<<endl;
cout <<"b="<<b<<endl;


    return 0;   
}