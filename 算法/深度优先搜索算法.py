def dfs(graph, node, visited):
    """
    深度优先搜索算法实现

    参数：
        graph (dict): 图的邻接表表示
        node (int): 当前需要访问的节点
        visited (list): 已访问节点的标记列表

    返回：
        None
    """
    # 如果当前节点已经被访问过，直接返回
    if node in visited:
        return

    # 将当前节点标记为已访问
    visited.append(node)

    # 对当前节点的每一个邻居进行递归调用
    for neighbor in graph[node]:
        dfs(graph, neighbor, visited)
