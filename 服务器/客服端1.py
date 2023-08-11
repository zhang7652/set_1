# coding: utf-8
# 客服端
import socket
import time
import sys


# def progress_bar():
#     for i in range(1, 101):
#         print("\r", end="")
#         print("下载中: {}%: ".format(i), "▋" * (i // 2), end="")
#         sys.stdout.flush()
#         time.sleep(0.05)
#     print('\r')


# 1.创建socket对象
soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # 流式协议(tcp协议)
# 2.建立链接
soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
soc.connect(('192.168.0.2', 80))
print(soc.recv(1024).decode('utf-8'))
while True:
    sed = input('请输入>>:').strip()
    # if sed == '退出':
    #     break
    # if not sed:
    #     continue
    # 3.传输数据
    soc.send(sed.encode('utf-8'))
    # 4.接收数据
    # sta = input('请输入文件名>>').strip()
    # soc.send(sta.encode('utf-8'))
    # std = 0
    # with open(fr'客户端文件\{sta}', 'wt', encoding='utf-8') as f:
    #     while True:
    #         st = soc.recv(1024).decode('utf-8').rstrip()
    #         if st == 'BS传输结束':
    #             break
    #         std += 1
    #         f.write(f'{st}\r')
    #         time.sleep(0.1)
    # progress_bar()

# 5.关闭链接
soc.close()
