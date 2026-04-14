#include<iostream>
using namespace std;

int main(){
/*
//switch语句
//给电影打分
10-~9分经典
8~7非常好
6~5一般
5一下 烂片
*/ 
//1.用户输入电影的分数
cout <<"请给电影打分"<<endl;
int score =0;
cin >>score;
cout <<"你给电影的分数是"<<score<<endl;
//2.根据用户输入的分数，给出对应的评价
switch(score){
    case 10:
    cout << "您认为是经典电影"<<endl;
    break; 
    case 9:
 cout <<"您认为是经典电影"<<endl;
    break;
    case 8:
    cout <<"您认为是非常好电影"<<endl;
    break;
    case 7:
    cout <<"您认为是非常好电影"<<endl;
    break;
    case 6:
    cout <<"您认为是一般电影"<<endl;
    break;
    case 5:     
    cout <<"您认为是一般电影"<<endl;
    break;
    default:
    cout <<"您认为是烂片"<<endl;
    break;
}
//if和switch的区别
//1.if语句的条件可以是任意的表达式，switch语句的
//条件只能是整数或者字符类型的常量表达式
//2.if语句的条件可以是一个范围，switch语句的条件只能是一个具体的值
    return 0;
}