n=eval(input())
t=0
for i in range(1,n+1):
    p=(-1)**(i+1)
    m=2*i-1
    k=1
    
    for j in range(1,m+1):
        
        k*=j
    b=p/k
    t+=b
print(t)
    