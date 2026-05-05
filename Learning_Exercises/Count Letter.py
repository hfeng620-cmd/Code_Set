test_str=input("Enter a character: ")
test_char=input("Enter a string: ")
count=0
pos=[]
for i in range(len(test_str)):
    if test_str[i]==test_char:
        count+=1
        pos.append(i)
print(f"Character occurs {count} times at positions {pos}")
