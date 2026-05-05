print("输入一个正整数:")
n = int(input())
print("该数的质因子有:")
for i in range(2, n + 1):
    while n!=i:
        if n%i==0:
           print(i, end=' ')
           n /= i
        else:
           break
print(int(n))





