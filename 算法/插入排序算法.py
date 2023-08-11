def insertion_sort(arr):
    """
    插入排序算法实现

    参数：
        arr (list): 需要排序的列表

    返回：
        None
    """
    for i in range(1, len(arr)):
        key = arr[i]  # 选择当前位置需要比较的元素
        j = i - 1  # 从当前位置的前一个位置开始比较

        while j >= 0 and arr[j] > key:  # 如果前一个元素比当前元素大，则将它向后移动一位
            arr[j + 1] = arr[j]
            j -= 1

        arr[j + 1] = key  # 将当前元素插入到正确的位置上
