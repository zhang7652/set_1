def merge_sort(arr):
    """
    归并排序算法实现

    参数：
        arr (list): 需要排序的列表

    返回：
        list: 排序后的列表
    """
    if len(arr) <= 1:  # 如果列表只有一个元素或为空，直接返回
        return arr

    mid = len(arr) // 2  # 选择中间元素作为基准值
    left = arr[:mid]  # 将左边的元素(除基准值外)组成一个新的列表
    right = arr[mid:]  # 将右边的元素(除基准值外)组成一个新的列表

    return merge(merge_sort(left), merge_sort(right))  # 递归地对左右两个列表进行排序，并将结果合并

def merge(left, right):
    """
    合并两个有序列表

    参数：
        left (list): 左边的有序列表
        right (list): 右边的有序列表

    返回：
        list: 合并后的有序列表
    """
    result = []
    i = j = 0

    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1

    result += left[i:]
    result += right[j:]

    return result
