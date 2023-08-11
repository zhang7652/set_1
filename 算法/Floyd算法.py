# Floyd算法实现
def floyd(graph, start):
    """
    Floyd算法实现

    参数：
        graph (dict): 图的邻接表表示，key为节点，value为与该节点相邻的节点及其权值的列表
        start (str): 起始节点

    返回：
        dict: 每个节点到其他所有节点的最短路径长度
    """
    n = len(graph)  # 节点个数
    dist = [[float('inf')] * n for _ in range(n)]  # 初始化距离矩阵，将所有元素设为无穷大
    dist[start][start] = 0  # 将起始节点到自身的距离设为0

    for k in range(n):  # 对于每个节点k,更新其到其他所有节点的最短路径长度
        for i in range(n):
            for j in range(n):
                if graph[i][j] and dist[i][k] + graph[i][j] < dist[k][j]:
                    dist[k][j] = dist[i][k] + graph[i][j]

    return dist
