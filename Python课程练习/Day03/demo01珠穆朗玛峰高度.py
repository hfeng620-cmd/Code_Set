count = 0
sum_count = 0
while count < 5:
    sum_count += count
    count += 1
print(sum_count)
'''
一张纸的厚度是00.1毫米，请计算，对折多少次后，纸的厚度会超过8848米（珠穆朗玛峰的高度）？
'''
thickness = 0.1  # 毫米
folds = 0
while thickness < 8848000:  # 转换为毫米
    thickness *= 2
    folds += 1
    print(f"对折{folds}次后，纸的厚度是{thickness}毫米。")
print(f"对折{folds}次后，纸的厚度会超过8848米。")

















