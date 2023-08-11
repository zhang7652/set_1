def extended_gcd(a, b):  # 定义扩展欧几里得算法函数
    if b == 0:  # 如果b等于0,则返回a作为最大公约数和余数
        return a, 1, 0
    x, y, u, v = extended_gcd(b, a % b)  # 递归调用扩展欧几里得算法，求解更小的系数
    w = x - u * (a // b)  # 根据公式计算新的系数
    return w, y, v + (a // b) * u
