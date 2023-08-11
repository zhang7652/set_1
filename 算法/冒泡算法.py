def bubble_sort(arr):
    """
    冒泡排序算法实现

    参数：
        arr (list): 需要排序的列表

    返回：
        list: 排序后的列表
    """
    n = len(arr)  # 获取列表长度
    for i in range(n):  # 遍历列表
        for j in range(0, n - i - 1):  # 遍历剩余未排序的元素
            if arr[j] > arr[j + 1]:  # 如果当前元素大于下一个元素
                arr[j], arr[j + 1] = arr[j + 1], arr[j]  # 交换两个元素的位置


# 示例
arr = [64, 34, 25, 12, 22, 11, 90]
bubble_sort(arr)
print("排序后的数组是：", arr)
