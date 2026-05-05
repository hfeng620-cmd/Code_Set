def mao_pao(num_list):
    n = len(num_list)
    for i in range(n):
        sign = False  # 初始化sign变量
        for j in range(0, n - i - 1):
            if num_list[j] > num_list[j + 1]:
                num_list[j], num_list[j + 1] = num_list[j + 1], num_list[j]
                sign = True
        if not sign:
            break
    return num_list  # 返回排序后的整个列表
#!/usr/bin/env python3
p=[23,45,12,67,34,89,90,11,78]
print(mao_pao(p))