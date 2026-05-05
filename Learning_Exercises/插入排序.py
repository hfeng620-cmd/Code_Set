def insert_sort(arr):
    """Perform an insertion sort on the given list."""
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        # Move elements of arr[0..i-1], that are greater than key,
        # to one position ahead of their current position
        while j>=0  and key < arr[j]:
            arr[j + 1] = arr[j]
            print(f"移动 arr[{j}] 到 arr[{j+1}]，当前数组: {arr}")  # 添加动画输出
            j -= 1
        arr[j + 1] = key
        print(f"插入 key={key} 到位置 {j+1}，当前数组: {arr}")  # 添加插入后的输出
    return arr
#测试用例
test_array = [12, 11, 13, 5, 6]
test_array1 = [12, 11, 13, 5, 6]
sorted_array = insert_sort(test_array1)
print("Old_array is:", test_array)
print("Sorted_array is:", sorted_array)