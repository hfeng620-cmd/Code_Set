#enumerate函数的使用
'''a=['apple','banana','orange']
b=enumerate(a,start=1)
listb=list(b)
print(listb[0])
tk1="ABC123"
for i in tk1:
    if i != "3":
       print(i,end=" ")
    else:
        break
print(" ",end="\n")
#使用enumerate函数
for index,fruit in enumerate(a,start=1):
    print(f"第{index}个水果是{fruit}")
x1,x2=1,1
print(x1,"\t",x2,"\t")
for i in range(1,31):
    x3=x1+x2
    print(x3,end="\t")
    if i % 10==0:
        print()
    x1,x2=x2,x3

from random import randint
a=randint(1,100)
print(a)'''
#第二章 数据表示与输出
#常见的Python转义字符:1.\ (在行尾时)续航符 2.\n 3.\\ 用于输出“\”   4.\' 用于输出'  5.\" 用于输出” 6.\t 横向制表符 \
#7.\Oyy 八进制数 对应ASCII表的字，例如\O60表示字符“O” 8.\xyy 十六进制数 对应ASCII码表的字符，例如\x41表示字符“A”
#print("子鼠\t丑牛\t寅虎\t卯兔\n辰龙\t巳蛇\t午马\t未羊\n申猴\t酉鸡\t戌狗\t亥猪")
#位运算：1.按位与（&）2.按位或（|）3.按位异或（^）4.按位取反（~）5.左移（<<） 6.右移（>>）
'''
2.4.常用函数
1.chr()指定返回整数对应的数值 2.dir（）返回对象的属性和方法列表 3.eval（）执行字符串的表达式 
4.exec()执行字符串或代码对象中的Python代码 5.fliter（）根据指定条件过滤可迭代对象中的代码
6.help（） 提供对象的帮助信息  7.isinstance（） 检查对象是否属于指定的类型
8.ord（）返回指定整数   9.pow（）返回值定数的幂  10.round（） 对指定数进行四舍五入 
11.将多个可迭代对象打包成一个元组序列
12.bin（） oct（）  hex（） 分别将一个整数转化为二进制 八进制 十六进制
2.5.输入与输出
输出函数：
print([value1,value2,value3....][,sep=""][,end="\n"][,file=sys.stdout])
（1）使用占位符：
%s：字符串   %d:整数  %f：浮点数  %o：八进制数   %x：十六进制数
（2）使用format方法：
（3）f-string
'''
#第三章

#3.1:程序设计思想
'''
3.2算法，流程图与程序设计：一个算法具有以下特性：有穷性 确切性 有效性 输入 输出
程序设计：分析问题 设计算法 编写程序 运行程序  编写程序文档
程序控制结构：顺序结构 分支结构 循环结构
'''
#3.3顺序结构
''''
from math import*
r=eval(input())
c=2*pi*r
s=pi*(r**2)
print(f"圆的周长是{c:.2f},圆的面积是{s:.2f}")
'''
#3.4分支结构
'''
3.4.1单分支结构   if <条件表达式>:
                   <语句块>
3.4.2双分支结构
if <条件表达式>:
   <语句块一>
else：
   <语句块二> 
3.5：循环结构
（1）.for循环：
for <循环变量> in <可迭代对象>：
    <循环体> 
<可迭代对象>有字符串，列表，元组，字典，集合

lst="abc123"
for i in lst:
    print(i,end=" ")



    
'''
'''x1=2
x2=3
result_list=[]
for i in range(3, 31):
    x3 = x1 + x2
    result_list.append(str(x3))
    x1,x2=x2,x3
    if i ==5:
        break
print(x3)
print(result_list)
'''
'''#倒序操作的实现
stra="万过薪月,黑乐月明,天资勇羽"
strb=stra.split(",")[1].replace("月","料")[::-1]
print(strb)
'''
'''info_dict={
    "Wang":{
        "Depart":"Science",
        "Salary":5000,
        "Rating":4



    },
    "Kitty":{
        "Depart":"Teach",
        "Salary":3000,
        "Rating":3
    }

}
for name in info_dict:
    if info_dict[name]["Rating"]==4:
        #对员工升职加薪操作
        #获取员工信息
        employee_info=info_dict[name]
        #修改员工信息
        employee_info["Rating"]=5
        employee_info["Salary"]=7000
        #将员工信息update into info_dict
        info_dict[name]=employee_info
print(info_dict)'''
'''max_int=0
min_int=0
lista=list(map(int,input().split()))
for i in range(len(lista)):
    if lista[i]>max_int:
        max_int=lista[i]
    if lista[i]<min_int:
        min_int=lista[i]
print(f":最小值是{max_int},最小值是{min_int},:")
'''
'''a,n=eval(input())

s=0
a=str(a)
for i in range(n):
    s+=int(a*(1+i))
print(s)
'''
#基本捕获语法
# try:
#     f = open("D:/VS Code/abc.text", "r", encoding="utf-8")
# except:
#     print("文件不存在")
#     f = open("D:/VS Code/abc.text", "w", encoding="utf-8")
# #捕获指定异常
# try:
#     print(x)
# except NameError as e:
#     print(e)
"""
演示读取文件，课后练习题
"""
# #打开文件，以只读模式打开
#f = open("D:/textp.text","r",encoding="utf-8")
# #方式1：读取全部内容，通过字符串 count方法统计“测试词”出现的次数
# content = f.read()
# word = "u"
# count = content.count(word)
#方法2：读取内容，一行一行读
'''count=0
readliunes = f.readlines()
for line in  readliunes:
    line = line.strip()  #去掉每行的首尾空白符和尾部“\n”换行符
    words = line.split(" ")
    print(words)
    for word in words:
        if word =="u":
            count += 1
print(f"文件中“测试词”出现的次数是：{count}")
f.close()
'''
'''# import time
# f= open("D:/test.txt","w",encoding="UTF-8")
# #write 写入
# f.write("hello world!\n")
# f.write("今天天气真好！\n")
# f.close()
# print("文件创建成功")
#"w"模式每次打开文件都会清空原有内容 没有这个位置的文件会创建新文件'''
#追加模式 "a"   
# f= open("D:/test.txt","a",encoding="UTF-8")
# f.write("追加内容1\n")      
# f.write("追加内容2\n")
# f.flush()
# f.close()
# print("追加成功")

#打开文件得到文件对象，准备读取
# fr = open("D:/test.txt","r",encoding="UTF-8")
# fw = open("D:/test_copy.txt","w",encoding="UTF-8")

# for line in fr:
#     line = line.strip()
#     #判断内容 将满足的内容写入新文件
#     if line.split(",")[4]=="测试":
#         continue
#     fw.write(line)
#     #由于对前面的内容进行了strip操作，所以这里需要添加换行符
#     fw.write("\n")

# fr.close()
# fw.close()   #写出文件调用close()方法会自动保存文件内容flush（）

'''
演示文件操作案例：备份
'''
# Python异常捕获
# 基本语法：try：可能发生错误的代码
#         except：处理错误的代码
#         finally：无论是否发生错误都要执行的代码
# try:
#     f = open("D:/VS Code/abc.txt", "r", encoding="utf-8")
# except:
#     print("文件不存在")
#     f = open("D:/VS Code/abc.txt", "w", encoding="utf-8")
# 捕获多个异常
# 捕获指定异常
# 语法：try：可能发生错误的代码
#       except <异常类型1>：处理错误的代码1
#       except <异常类型2>：处理错误的代码(Name)
# try:
    # 1/0
    # print(name)
# except NameError as e:
#     print("出现了变量未定义的异常")
# except ZeroDivisionError as e:  
#     print("出现了除零异常")
#     print(e)
# except (NameError,ZeroDivisionError) as e:
#     print("出现了变量未定义或者除零异常")
#     print(e)
# 捕获所有异常
# 语法：try：可能发生错误的代码
#       except Exception as e：处理错误的代码
try:
    print("Hello")
    1/0
except Exception as e:
    print("出现了异常")
    print(e)
else:
    print("好高兴，没有出现异常")












