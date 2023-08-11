def binary_search(arr, x):
    """
    二分查找算法实现

    参数：
        arr (list): 有序数组
        x (int): 要查找的元素

    返回：
        int: 元素在数组中的索引，如果不存在则返回-1
    """
    low = 0
    high = len(arr) - 1
    mid = 0

    while low <= high:

        mid = (high + low) // 2

        # 如果x大于中间值，则在右半部分继续查找
        if arr[mid] < x:
            low = mid + 1

        # 如果x小于中间值，则在左半部分继续查找
        elif arr[mid] > x:
            high = mid - 1

        # x等于中间值，返回索引
        else:
            return mid

    # 如果没有找到，返回-1
    return -1
