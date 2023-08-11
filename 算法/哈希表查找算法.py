class HashTable:
    def init(self, size):
        """
    哈希表类的构造函数

        参数：
            size (int): 哈希表的大小
        """
        self.size = size
        self.table = [None] * size

    def hash_func(self, key):
        """
        哈希函数，将目标元素映射到哈希表的一个位置上

        参数：
            key (int/float): 需要查找的目标元素

        返回：
            int: 目标元素在哈希表中的位置，如果发生冲突则需要进行处理(如链地址法)
        """
        return key % self.size

    def search(self, key):
        """
        哈希表查找算法实现

        参数：
            key (int/float): 需要查找的目标元素

        返回：
            int/float: 目标元素在哈希表中的位置，如果不存在则返回None或者采取其他处理方式(如插入新元素)
        """
        # 将目标元素通过哈希函数映射到哈希表的一个位置上
        index = self.hash_func(key)
        # 在哈希表中查找该位置上的元素，如果找到了则返回该元素；如果没有找到则说明目标元素不在哈希表中，可以采取其他处理方式(如插入新元素)
        if self.table[index] is not None and self.table[index][0] == key:
            return index, self.table[index][1]
        else:
            return None, None