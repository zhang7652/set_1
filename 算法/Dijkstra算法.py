# Dijkstra算法实现
import heapq


def dijkstra(graph, start):
    """
    Dijkstra算法实现

    参数：
        graph (dict): 图的邻接表表示，key为节点，value为与该节点相邻的节点及其权值的列表
        start (str): 起始节点

    返回：
        dict: 每个节点到起始节点的最短路径长度
    """
    n = len(graph)  # 节点个数
    dist = [float('inf')] * n  # 初始化距离数组，将所有节点的距离设为无穷大
    dist[start] = 0  # 将起始节点的距离设为0
    visited = [False] * n  # 初始化访问数组，将所有节点标记为未访问
    pq = [(0, start)]  # 用优先队列存储待处理的节点及其距离

    while pq:
        d, u = heapq.heappop(pq)  # 从优先队列中取出距离最小的节点u
        if visited[u]:  # 如果该节点已经被访问过，则跳过
            continue
        visited[u] = True  # 将该节点标记为已访问
        for v, w in graph[u]:  # 遍历与该节点相邻的所有节点及其权值
            if not visited[v]:  # 如果该节点未被访问过，则更新距离数组并将其加入优先队列中
                dist[v] = min(dist[v], dist[u] + w)
                heapq.heappush(pq, (dist[v], v))

    return dist
