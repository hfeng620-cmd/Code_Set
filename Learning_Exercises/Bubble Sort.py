def bubble_sort(num_list):
    n = len(num_list)
    for i in range(n):
        for j in range(0, n-i-1):
            if num_list[j] > num_list[j+1]:
                num_list[j], num_list[j+1] = num_list[j+1], num_list[j]
    return num_list
lista=eval(input("请输入一个列表:"))
sorted_list = bubble_sort(lista)
print("排序后的列表为:", sorted_list)   