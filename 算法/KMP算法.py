def generate_lps(pattern):
    """
    构造最长前缀后缀数组（Longest Proper Prefix which is also Suffix）
    :param pattern: 模式串
    :return: 最长前缀后缀数组
    """
    length = len(pattern)
    lps = [0] * length  # 初始化lps数组
    i = 1
    prefix_len = 0

    while i < length:
        if pattern[i] == pattern[prefix_len]:
            prefix_len += 1
            lps[i] = prefix_len
            i += 1
        else:
            if prefix_len != 0:
                prefix_len = lps[prefix_len - 1]
            else:
                lps[i] = 0
                i += 1

    return lps

def kmp_search(text, pattern):
    """
    使用KMP算法在文本串中搜索模式串
    :param text: 文本串
    :param pattern: 模式串
    :return: 匹配的起始索引列表
    """
    m = len(pattern)
    n = len(text)
    lps = generate_lps(pattern)  # 构造最长前缀后缀数组

    result = []  # 存储匹配的起始索引列表

    i = j = 0  # i: text的索引，j: pattern的索引

    while i < n:
        if pattern[j] == text[i]:
            i += 1
            j += 1

            if j == m:
                result.append(i - j)
                j = lps[j - 1]

        elif i < n and pattern[j] != text[i]:
            # 不匹配的情况下，通过lps数组跳过重复比较的部分
            if j != 0:
                j = lps[j - 1]
            else:
                i += 1

    return result