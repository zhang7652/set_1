def quick_sort(arr):
    """
    快速排序算法实现

    参数：
        arr (list): 需要排序的列表

    返回：
        list: 排序后的列表
    """
    if len(arr) <= 1:  # 如果列表只有一个元素或为空，直接返回
        return arr
    pivot = arr[len(arr) // 2]  # 选择中间元素作为基准值
    left = [x for x in arr if x < pivot]  # 将小于基准值的元素放到左边
    middle = [x for x in arr if x == pivot]  # 将等于基准值的元素放到中间
    right = [x for x in arr if x > pivot]  # 将大于基准值的元素放到右边
    return quick_sort(left) + middle + quick_sort(right)  # 递归地对左右两边的列表进行快速排序，并将结果拼接起来

# 示例
arr = [64, 34, 25, 12, 22, 11, 90]
quick_sort(arr)
print("排序后的数组是：", arr)
