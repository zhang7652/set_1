def naive_pattern_search(text, pattern):
    """
    朴素模式匹配算法实现

    参数：
        text (str): 需要搜索的文本
        pattern (str): 需要匹配的模式

    返回：
        list: 匹配到的模式在文本中的起始位置列表，如果没有匹配到则返回空列表
    """
    # 初始化结果列表，用于存储匹配到的模式在文本中的起始位置
    result = []

    # 遍历文本中的每个字符
    for i in range(len(text) - len(pattern) + 1):
        # 如果当前字符与模式的第一个字符相等，则继续比较后续字符是否匹配
        if text[i] == pattern[0]:
            match = True
            # 从第二个字符开始依次比较，如果发现不匹配的字符，则将match标记为False并退出循环
            for j in range(1, len(pattern)):
                if text[i + j] != pattern[j]:
                    match = False
                    break
            # 如果所有字符都匹配成功，则将当前位置添加到结果列表中
            if match:
                result.append(i)

    # 返回匹配到的模式在文本中的起始位置列表
    return result
