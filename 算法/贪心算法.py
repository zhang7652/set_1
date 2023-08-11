def greedy_algorithm(items, max_weight):
    """
    贪心算法实现

    参数：
        items (list): 物品列表，每个物品由重量和价值两个属性组成，如[{'weight': 10, 'value': 60}, {'weight': 20, 'value': 100}]
        max_weight (int): 背包的最大承重

    返回：
        list: 能够装入背包的物品组合
    """
    # 按照物品的价值/重量比进行排序
    items.sort(key=lambda x: x['value']/x['weight'], reverse=True)

    result = []  # 存储能够装入背包的物品组合
    total_value = 0  # 总价值

    for item in items:
        if total_value + item['value'] <= max_weight:  # 如果当前物品能够装入背包
            result.append(item)  # 将当前物品添加到结果中
            total_value += item['value']  # 更新总价值

    return result
