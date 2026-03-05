x1 = 1
x2 = 1
result_list = []  # 存储所有递推结果的列表

# 先计算所有结果
for i in range(3, 31):
    x3 = x1 + x2
    result_list.append(str(x3))  # 将结果转字符串存入列表
    x1, x2 = x2, x3
width = 6
for idx in range(0, len(result_list), 10):
    row = result_list[idx:idx+10]
    formatted_row = [f"{num:>{width}}" for num in row]
    print((formatted_row))
#输入输出结果相同函数自定义
def f1():
  def f2():
    print("请输入:")
    t=input()
    print("输出:")
    print(t)
  f2()
f1()
#求解二元一次方程组
from math import *
def solve(a, b, c):
    if a == 0:
      if b == 0:
        print("该等式不是方程！")
      else :
        print("x=%.2f" % (-c / b))
    else :
      delta = b ** 2 - 4 * a * c
      if delta > 0:
        x1 = (-b + sqrt(delta)) / (2 * a)
        x2 = (-b - sqrt(delta)) / (2 * a)
        print("x1=%.2f" % x1,", ","x2=%.2f" % x2)
      else :
        if delta == 0:
          x = (-b + sqrt(delta)) / 2 * a
          print("x1=x2=%.2f" % x)
        else :
          print("无实根！")
print("请输入三个数a,b,c:")
A, B, C = eval(input())
solve(A, B, C)