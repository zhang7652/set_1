def permute(arr, l, r):
    """
    全排列算法实现

    参数：
        arr (list): 需要进行排列的列表
        l (int): 左边界
        r (int): 右边界

    返回：
        None
    """
    if l == r:  # 如果只有一个元素，直接返回
        print(arr)
        return

    for i in range(l, r + 1):  # 遍历从左边界到右边界的所有元素
        # 将当前元素与左边界的元素交换位置，然后递归地对左右两个子列表进行排列
        arr[l], arr[i] = arr[i], arr[l]
        permute(arr, l + 1, r)
        # 再将交换回来，以便进行下一次排列
        arr[l], arr[i] = arr[i], arr[l]
