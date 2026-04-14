#include<iostream>
using namespace std;
//time系统时间头文件包含
#include<ctime>

int main(){
//添加随机数种子  作用是利用当前系统时间生成随机数，防止每次生成的随机数都一样
// 修改前: srand(unsigned int)time(NULL);  ← 缺少括号
srand((unsigned int)time(NULL));
//1.系统生成随机数
int num= rand() % 100 +1;//rand()%100 +1 生成1-100的随机数
cout <<"系统生成的随机数是"<<num<<endl;  
//2.玩家进行猜测
int val ;//玩家输入数字
while(1){
cin >>val;
//3.判断嗯玩家的猜测
//猜错      提醒猜的结果    过大或者过小    重新返回第2步
// 修改前: if (val>num>){  ← 语法错误，条件表达式多余
if (val > num){
cout <<"猜测过大"<<endl;
}else if(val<num){
cout <<"猜测过小"<<endl;
}else{
cout <<"恭喜你猜对了"<<endl;
break;//猜对了  退出循环
}
}


    return 0;
}




