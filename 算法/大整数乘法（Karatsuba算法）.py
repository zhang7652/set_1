def karatsuba_multiply(x, y):
    # 将输入的大整数转换为字符串
    x_str = str(x)
    y_str = str(y)

    # 如果输入的整数太小，直接返回它们的乘积
    if len(x_str) == 1 or len(y_str) == 1:
        return x * y

    # 计算输入整数的位数并将其分为两半
    n = max(len(x_str), len(y_str))
    m = n // 2

    # 将输入整数分解为高位和低位
    a = int(x_str[:-m]) if len(x_str[:-m]) > 0 else 0
    b = int(x_str[-m:]) if len(x_str[-m:]) > 0 else 0
    c = int(y_str[:-m]) if len(y_str[:-m]) > 0 else 0
    d = int(y_str[-m:]) if len(y_str[-m:]) > 0 else 0

    # 递归计算子问题
    ac = karatsuba_multiply(a, c)
    bd = karatsuba_multiply(b, d)
    ad_plus_bc = karatsuba_multiply(a + b, c + d) - ac - bd

    # 计算结果
    result = ac * (10**(2*m)) + ad_plus_bc * (10**m) + bd

    return result