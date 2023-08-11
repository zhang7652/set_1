def longest_common_subsequence(s1, s2):
    """
    最长公共子序列算法实现

    参数：
        s1 (str): 字符串1
        s2 (str): 字符串2

    返回：
        int: 两个字符串的最长公共子序列的长度
    """
    m = len(s1)
    n = len(s2)

    # 创建一个二维数组来存储动态规划的结果
    dp = [[0] * (n + 1) for _ in range(m + 1)]

    # 遍历两个字符串，计算它们的最长公共子序列的长度
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if s1[i - 1] == s2[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
            else:
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1])

    return dp[m][n]
