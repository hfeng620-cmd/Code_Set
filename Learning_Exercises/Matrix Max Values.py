n=eval(input("请输入矩阵的行数n:"))
matrix=[]
for i in range(n):
    s=list(map(int,input().split()))
    matrix.append(s)
result = 0
for i in range(n):
    if result < max(matrix[i]):
        result = max(matrix[i])
print("矩阵中的最大值为:",result)