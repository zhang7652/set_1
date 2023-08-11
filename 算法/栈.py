class Stack:
    """
    栈的实现

    参数：
        max_size (int): 栈的最大容量
    """
    def __init__(self, max_size):
        self.stack = []  # 用列表来实现栈
        self.max_size = max_size

    def push(self, item):
        """
        入栈操作

        参数：
            item: 要入栈的元素

        """
        if len(self.stack) >= self.max_size:  # 如果栈已满，直接返回
            return
        self.stack.append(item)  # 将元素添加到栈顶

    def pop(self):
        """
        出栈操作

        返回：
            弹出的元素
        """
        if len(self.stack) == 0:  # 如果栈为空，直接返回None
            return None
        return self.stack.pop()  # 将栈顶元素弹出并返回

    def peek(self):
        """
        查看栈顶元素的操作，但不弹出元素

        返回：
            栈顶元素，如果栈为空则返回None
        """
        if len(self.stack) == 0:  # 如果栈为空，直接返回None
            return None
        return self.stack[-1]  # 返回栈顶元素
