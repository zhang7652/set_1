def linear_search(arr, target):
    """
    线性查找算法实现

    参数：
        arr (list): 需要查找的列表
        target (int/float): 需要查找的目标元素

    返回：
        int/float: 目标元素在列表中的位置，如果不存在则返回None
    """
    # 确定列表中元素的存储位置，这里使用数组来实现
    arr_len = len(arr)
    for i in range(arr_len):
        # 从列表的第一个元素开始遍历，比较当前元素的值与目标元素的值是否相等
        if arr[i] == target:
            # 如果相等，则返回当前元素的位置
            return i
    # 如果遍历完整个列表都没有找到目标元素，则返回None
    return None