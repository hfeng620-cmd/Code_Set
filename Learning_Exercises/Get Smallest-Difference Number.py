# 1. 获取用户输入的列表
'''lista = eval(input('输入一个列表: '))
# 2. 对列表降序排序（用sorted()，返回新列表，不修改原列表）
listb = sorted(lista, reverse=True)

# 3. 初始化：取前两个元素的差值为初始最小差值
diff = listb[0] - listb[1]
numa = listb[0]
numb = listb[1]

# 4. 遍历排序后的列表，找相邻元素的最小差值
for i in range(1, len(listb)-1):  # 从索引1到倒数第2个元素（确保i+1不越界）
    current_diff = listb[i] - listb[i+1]
    if current_diff < diff:
        diff = current_diff
        numa = listb[i]
        numb = listb[i+1]

# 5. 获取这两个元素在原列表中的索引（处理重复元素：记录所有索引，取匹配的相邻索引）
# 先获取原列表中numa、numb的所有出现位置
indices_numa = [idx for idx, val in enumerate(lista) if val == numa]
indices_numb = [idx for idx, val in enumerate(lista) if val == numb]
# 取第一个匹配的索引（若需更精准，需结合排序前的位置，这里简化处理）
indexa = indices_numa[0]
indexb = indices_numb[0]

# 6. 输出结果
print(f'差值最小的两个数为: [{numa}, {numb}]')
print(f'它们在列表中的索引号为: [{indexa}, {indexb}]')'''
lista=eval(input("输入一个列表: "))
listb=sorted(lista,reverse=True)
diff=listb[0]-listb[1]
indexa=0
indexb=0
for i in range(2,len(listb)-1):
    if diff>listb[i]-listb[i+1]:
        diff=listb[i]-listb[i+1]
        numa=listb[i]
        numb=listb[i+1]
indexa=lista.index(numa)
indexb=lista.index(numb)
print(f"差值最小的两个数为: [{numa}, {numb}]")
print(f"它们在列表中的索引号为: [{indexa}, {indexb}]")





