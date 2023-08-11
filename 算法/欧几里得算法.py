def gcd(a, b):  # 定义一个求最大公约数的函数，接收两个参数a和b
    while b != 0:  # 当b不等于0时，执行循环体
        a, b = b, a % b  # 将a赋值为b,将b赋值为a除以b的余数
    return a  # 返回a作为最大公约数
