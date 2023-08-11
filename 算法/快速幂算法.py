def quick_pow(base, exponent, mod):
    """
    快速幂算法实现

    参数：
        base (int): 底数
        exponent (int): 指数
        mod (int): 模数

    返回：
        int: 计算结果
    """
    result = 1  # 初始化结果为1
    while exponent > 0:
        if exponent % 2 == 1:  # 如果指数是奇数，则将当前结果乘上底数并对模数取余
            result = (result * base) % mod
        base = (base * base) % mod  # 将底数平方并对模数取余
        exponent //= 2  # 将指数除以2
    return result
