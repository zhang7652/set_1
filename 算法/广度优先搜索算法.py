from collections import deque


def bfs(graph, start):
    """
    广度优先搜索算法实现

    参数：
        graph (dict): 图的邻接表表示
        start (int): 起始节点

    返回：
        list: 遍历的路径列表
    """
    visited = set()  # 已访问节点集合
    queue = deque([start])  # 用双端队列来实现队列
    traversal = []  # 存储遍历的路径

    while queue:
        node = queue.popleft()  # 从队列左侧弹出一个节点
        if node not in visited:
            visited.add(node)  # 将节点标记为已访问
            traversal.append(node)  # 将节点添加到路径中
            for neighbor in graph[node]:
                queue.append(neighbor)  # 将邻居节点加入队列中

    return traversal
