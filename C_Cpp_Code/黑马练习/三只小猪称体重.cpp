#include <iostream>
using namespace std;


int main (){
    //三只小猪体重，判断那只最重
    //1.用户输入三只小猪的体重
    int num1 =0;
    
    int num2 =0;

    int num3 =0;
    
    cout <<"请输入第一支小猪的体重"<<endl;
    cin >>num1;
    cout <<"请输入第二支小猪的体重"<<endl;
    cin >>num2;
    cout <<"请输入第三支小猪的体重"<<endl;
    cin >>num3;
    //2.打印用户输入的三只小猪的体重
    cout <<"第一只小猪的体重是"<<num1<<endl;
    cout <<"第二只小猪的体重是"<<num2<<endl;
    cout <<"第三只小猪的体重是"<<num3<<endl;
    //3.判断那只小猪最重
    if(num1>num2 ){
        if (num1>num3){
            cout <<"第一只小猪最重"<<endl;
        }else{
            cout <<"第三只小猪最重"<<endl;
        }
    }else{
        if(num2>num3){
            cout <<"第二只小猪最重"<<endl;
        }else{
            cout <<"第三只小猪最重"<<endl;
        }
    }   
    
    return 0;
    


}