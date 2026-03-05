#这次版本的“右对齐+固定宽度”有学习意义
x1 = 1
x2 = 1
result_list = []  # 存储所有递推结果的列表

# 先计算所有结果
for i in range(3, 31):
    x3 = x1 + x2
    result_list.append(str(x3))  # 将结果转字符串存入列表
    x1, x2 = x2, x3

# 按每行10个输出，固定宽度对齐
width = 6  # 每个数的显示宽度（可根据数值长度调整）
for idx in range(0, len(result_list), 10):
    # 取当前行的10个元素，用格式化拼接
    row = result_list[idx:idx+10]
    formatted_row = [f"{num:>{width}}" for num in row]
#居右对齐的案例
    print("".join(formatted_row))#join连接字符串
    #-"".join(...)会把列表中所有元素用空字符串""连接成一个完整的字符串
list_row=list(formatted_row)
#改良版本的斐波那契数列，每列第一个数字对齐
x1 = 1
x2 = 1
result_list = []  # 存储所有递推结果

# 计算所有结果
for i in range(3, 31):
    x3 = x1 + x2
    result_list.append(str(x3))
    x1, x2 = x2, x3

# 关键：左对齐 + 统一固定宽度（根据最大数值长度设置，这里设为8）
width = 8
print("每行10个数，列首数字对齐：")
for idx in range(0, len(result_list), 10):
    # 取当前行的10个元素，左对齐并填充到width宽度
    row = [f"{num:<{width}}" for num in result_list[idx:idx+10]]
    print("".join(row))
