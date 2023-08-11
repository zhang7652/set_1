# coding: utf-8
# 服务端
import os
import pinyin
import random
import shutil
import socketserver
import threading
from tkinter import filedialog
from tkinter.filedialog import askdirectory
from tkinter.messagebox import askyesno
import cv2
import numpy as np
import pyautogui
from PIL import ImageGrab
import string
import time
import requests
from multiprocessing import Queue
from bs4 import BeautifulSoup
from urllib import parse
import tkinter
from tkinter import *

hear = {
    'User-Agent': 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) '
                  'Chrome/111.0.0.0 Mobile Safari/537.36 Edg/111.0.1661.41'
}


# 请求文件
class Resddutfd(socketserver.BaseRequestHandler):
    def handle(self):
        x = 0
        k = []
        with open(r'服务器\链接文件.txt', 'at', encoding='utf-8') as ft, \
                open(r'服务器\ip文件.txt', 'at', encoding='utf-8') as ff:
            ft.write(f'链接对象:{self.request}\n')
            ff.write(f'客服端ip+端口:{self.client_address}\n')
        filePath = r'D:\PycharmProjects\pythonProject6\应用\代码\服务器\主服务器文件'
        for i, j, k in os.walk(filePath):
            self.request.send(str(f'文件包:{k}').encode('utf-8'))
            x = len(k) - 1
            k = k
        while True:
            try:
                date = self.request.recv(1024)
            except Exception as tt:
                with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                    bao_c.write(f'(1)报错类型:{tt}!\n')
                    break
            else:
                if not date:
                    continue
                date = date.decode('utf-8')
                se_f, se_t = self.client_address
                with open(r'服务器\请求文件.txt', 'at', encoding='utf-8') as d, \
                        open(r'服务器\保存文件.txt', 'at', encoding='utf-8') as d1:
                    d.write(f'客服端{se_f}请求{date}文件.\n')
                    st = self.request.recv(1024).decode('utf-8')
                    d1.write(f'端口号:{se_f}下载的文件:{st}\n')
                while x >= 0:
                    try:
                        if date == k[x]:
                            with open(rf'主服务器文件\{k[x]}', 'rb') as f:
                                for intem in f:
                                    time.sleep(0.3)
                                    self.request.send(intem)
                                self.request.send(str('BS传输结束').encode('utf-8'))
                                break
                        else:
                            x -= 1
                    except Exception as tt:
                        with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                            bao_c.write(f'(2)报错类型:{tt}!\n')
                            break
                x = len(k) - 1


class Pyt_f2:
    def __init__(self):
        self.int1 = tkinter.Tk()
        self.int1.title('服务器')
        w, h = self.int1.maxsize()
        self.int1.geometry("{}x{}".format(w, h))
        self.st = 0
        self.fu()
        self.kais2()

    def fu1(self):
        try:
            while True:
                with open(r'服务器\ip文件.txt', 'r', encoding='utf-8') as ff, \
                        open(r'服务器\报错文件.txt', 'r', encoding='utf-8') as ff1, \
                        open(r'服务器\保存文件.txt', 'r', encoding='utf-8') as ff2, \
                        open(r'服务器\请求文件.txt', 'r', encoding='utf-8') as ff3:
                    self.var6 = StringVar()
                    self.dt = Label(self.int1, font=20, textvariable=self.var6, width=20, fg='blue')
                    self.dt.grid(row=0, column=1, padx='10', pady='10', sticky='W')
                    self.var6.set(str(time.strftime('%Y-%m-%d %H:%M:%S')))
                    time.sleep(1)
                    self.height_width_text2.insert('1.0', ff2.readlines()[-1])
                    self.height_width_text.insert('1.0', ff.readlines()[-1])
                    self.height_width_text1.insert('1.0', ff1.readlines()[-1])
                    self.height_width_text3.insert('1.0', ff3.readlines()[-1])
                    self.int1.update()
        except Exception as tt:
            with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                bao_c.write(f'(2)报错类型:{tt}!\n')

    def fu(self):
        # 标签
        self.dt = Label(self.int1, text='服务器监控界面', font=50)
        self.dt.grid(row=0, columnspan=5, pady='10', padx='10', column=5)
        self.dt = Label(self.int1, text='上传文件')
        self.dt.grid(row=2)
        self.dt = Label(self.int1, text='卸下文件')
        self.dt.grid(row=3)
        self.dt = Label(self.int1, text='客户端IP端口')
        self.dt.grid(row=1, column=5, sticky=W)
        self.dt = Label(self.int1, text='报错类型')
        self.dt.grid(row=1, column=6, sticky=W)
        self.dt = Label(self.int1, text='端口号下载文件')
        self.dt.grid(row=1, column=7, sticky=W)
        self.dt = Label(self.int1, text='客服端请求文件')
        self.dt.grid(row=1, column=8, sticky=W)

        # 按钮
        self.na_luz = Button(self.int1, text='清除记录', command=self.na_luq)
        self.na_luz.grid(row=5, column=2, sticky=W)
        self.na_lu = Button(self.int1, text='关闭服务器', command=self.na_liu)
        self.na_lu.grid(row=5, column=1)
        self.anniu1 = Button(self.int1, text='返回登录', command=self.fan_hu)
        self.anniu1.grid(row=4, column=2, sticky=W)
        self.an1 = Button(self.int1, text='开启服务器', command=self.kais1)
        self.an1.grid(row=5, column=0)
        self.an1 = Button(self.int1, text='开始卸下', command=self.xiex)
        self.an1.grid(row=4, column=0, sticky=W)
        self.an1 = Button(self.int1, text='开始上传', command=self.shuanc)
        self.an1.grid(row=4, column=1)
        self.anniu = Button(self.int1, text='设置文件位置', width='15', command=self.start4)
        self.anniu.grid(row=2, column=2, sticky=W)
        self.anniu = Button(self.int1, text='设置文件位置', width='15', command=self.start5)
        self.anniu.grid(row=3, column=2, sticky=W)
        # 输入框
        self.var0 = tkinter.StringVar()
        self.Entory0 = tkinter.Entry(self.int1, textvariable=self.var0)
        self.Entory0.grid(row=2, column=1, sticky=W)
        self.height_width_text = Text(self.int1, height=10, width=40)
        self.height_width_text.grid(row=2, column=5, sticky=W, rowspan=6)
        self.height_width_text1 = Text(self.int1, height=10, width=40, fg='red')
        self.height_width_text1.grid(row=2, column=6, sticky=W, rowspan=6)
        self.height_width_text2 = Text(self.int1, height=10, width=40)
        self.height_width_text2.grid(row=2, column=7, sticky=W, rowspan=6)
        self.height_width_text3 = Text(self.int1, height=10, width=40)
        self.height_width_text3.grid(row=2, column=8, sticky=W, rowspan=6)
        self.var7 = tkinter.StringVar()
        self.Entory1 = tkinter.Entry(self.int1, textvariable=self.var7)
        self.Entory1.grid(row=3, column=1, sticky=W)

    def na_luq(self):
        try:
            self.height_width_text.delete('1.0', 'end')
            self.height_width_text1.delete('1.0', 'end')
            self.height_width_text2.delete('1.0', 'end')
            self.height_width_text3.delete('1.0', 'end')
            self.var0.set('')
            self.var7.set('')
        except Exception as tt:
            with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                bao_c.write(f'(2)报错类型:{tt}!\n')

    def fan_hu(self):
        self.int1.destroy()
        tf = Pyt_f3()
        tf.int2.mainloop()

    # 关闭服务器 ///
    def na_liu(self):
        try:
            pass
        except Exception as tt:
            with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                bao_c.write(f'(2)报错类型:{tt}!\n')

    # 上传文件
    def shuanc(self):
        try:
            if self.var0.get() == '':
                askyesno(title='是/否对话框', message='提示！！没有输入文件名')
            else:
                movabs_path = r"D:\PycharmProjects\pythonProject6\应用\代码\服务器\主服务器文件"
                rawabs_path = self.var0.get()
                self.st += 1
                shutil.move(rawabs_path, movabs_path + f"/{self.st}.py")
                askyesno(title='是/否对话框', message='提示！！卸下成功')
        except Exception as tt:
            with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                bao_c.write(f'(2)报错类型:{tt}!\n')

    # 卸下文件
    def xiex(self):
        try:
            if self.var6.get() == '':
                askyesno(title='是/否对话框', message='提示！！没有输入文件名')
            else:
                os.remove(self.var6.get())
                askyesno(title='是/否对话框', message='提示！！卸下成功')
        except Exception as tt:
            with open(r'服务器\报错文件.txt', 'at', encoding='utf-8') as bao_c:
                bao_c.write(f'(2)报错类型:{tt}!\n')

    def denl(self):
        self.sk = socketserver.ThreadingTCPServer(('192.168.0.3', 5000), Resddutfd, bind_and_activate=False)
        self.sk.allow_reuse_address = True
        self.sk.server_bind()
        self.sk.server_activate()
        self.sk.serve_forever()

    # 打开文件位置
    def start4(self):
        # 单个文件选择
        selected_file_path = tkinter.filedialog.askopenfilename()  # 使用askopenfilename函数选择单个文件
        self.var0.set(selected_file_path)

    def start5(self):
        # 单个文件选择
        selected_file_path = tkinter.filedialog.askopenfilename()  # 使用askopenfilename函数选择单个文件
        self.var7.set(selected_file_path)

    def kais1(self):
        self.T = threading.Thread(target=self.denl)  # 多线程
        self.T.daemon = True  # 线程守护，即主进程结束后，此线程也结束。否则主进程结束子进程不结束
        self.T.start()  # 启动

    def kais2(self):
        self.T = threading.Thread(target=self.fu1)  # 多线程
        self.T.daemon = True  # 线程守护，即主进程结束后，此线程也结束。否则主进程结束子进程不结束
        self.T.start()  # 启动


class Pyt_f3:
    def __init__(self):
        self.int2 = tkinter.Tk()
        self.int2.title('服务器')
        self.int2.geometry('500x500')
        self.de()

    def de(self):
        self.st1 = Label(self.int2, text='服务器登录')
        self.st1.grid(row=0, column=3)
        self.st2 = Label(self.int2, text='账号:')
        self.st2.grid(row=1, column=0)
        self.st3 = Label(self.int2, text='密码:')
        self.st3.grid(row=2, column=0)

        self.st4 = tkinter.StringVar()
        self.sg = tkinter.Entry(self.int2, textvariable=self.st4)
        self.sg.grid(row=1, column=1)
        self.st5 = tkinter.StringVar()
        self.sg1 = tkinter.Entry(self.int2, textvariable=self.st5)
        self.sg1.grid(row=2, column=1)

        self.nl = Button(self.int2, text='登录', command=self.den_lu)
        self.nl.grid(row=3, column=1, sticky=W)
        self.nl1 = Button(self.int2, text='退出', command=self.tu_cu)
        self.nl1.grid(row=3, column=1)
        self.nl2 = Button(self.int2, text='注销', command=self.zhu_x)
        self.nl2.grid(row=3, column=2, sticky=W)

    def zhu_x(self):
        pass

    def den_lu(self):
        self.int2.destroy()
        td = Pyt_f2()
        td.int1.mainloop()

    def tu_cu(self):
        self.int2.destroy()
        gf1 = MY_GUI1()
        gf1.init_de.mainloop()


# 生成随机数
def fu1(length=16):
    pwd = []
    for _ in range(length // 4 + 1):
        char = [chr(random.randint(97, 122)),
                chr(random.randint(65, 90)),
                chr(random.randint(48, 57)),
                chr(random.randint(33, 47))
                ]
        pwd.extend(char)
    pwd = pwd[:length]
    random.shuffle(pwd)
    return ''.join(pwd)


def fu(flag):
    if flag:
        return 'break'


def video_record(st1=None, st="1", bianm="MP4V"):  # 屏幕录制
    flag = False
    p = ImageGrab.grab()  # 当前屏幕
    a, b = p.size  # 屏幕的大小
    fourcc = cv2.VideoWriter_fourcc(*f"{bianm}")  # 编码格式XVID  mp4v
    video = cv2.VideoWriter(fr"{st1}\{st}.avi", fourcc, 20, (a, b))
    while True:
        im = ImageGrab.grab()
        imm = cv2.cvtColor(np.array(im), cv2.COLOR_RGB2BGR)
        video.write(imm)
        fu(flag)
        if flag == 'break':
            break
    video.release()


class MY_GUI:
    def __init__(self, st1=None, st=None):
        self.st1 = st1
        self.st = st
        self.init_window_name = tkinter.Tk()  # 实例化出一个父窗口
        self.init_window_name.title("录屏工具")  # 窗口名
        self.init_window_name.geometry('1100x800')
        self.set_init_window()

    def set_init_window(self):
        # 标签
        self.init_data_label = Label(self.init_window_name, text='录制工具(zhang hong yin)', font=20)
        self.init_data_label.grid(row=0, padx='10', pady='10', column=3)
        self.init_data_label = Label(self.init_window_name, text="影片名:")
        self.init_data_label.grid(row=1, padx='10', pady='10', column=0)
        self.init_data_label = Label(self.init_window_name, text='文件路径：')
        self.init_data_label.grid(row=1, padx='10', pady='20', column=3)
        self.init_data_label = Label(self.init_window_name, text='文件格式')
        self.init_data_label.grid(row=2, padx='10', pady='20', column=0)

        # 文本框
        self.Entory0 = tkinter.Entry(self.init_window_name)
        self.Entory0.grid(row=1, column=1, padx='10', pady='10')
        self.var0 = tkinter.StringVar()
        self.Entory1 = tkinter.Entry(self.init_window_name, textvariable=self.var0)
        self.Entory1.grid(row=1, column=4, padx='10', pady='10')
        self.var2 = tkinter.StringVar()
        self.Entory2 = tkinter.Entry(self.init_window_name, textvariable=self.var2)
        self.Entory2.grid(row=2, column=1, padx='10', pady='10')

        # 按钮
        self.anniu = Button(self.init_window_name, text='开始', width='15', command=self.kais1)
        self.anniu.grid(row=3, column=0, pady='10')
        self.anniu = Button(self.init_window_name, text='暂停', width='15', command=self.start1)
        self.anniu.grid(row=3, column=1, pady='10')
        self.anniu = Button(self.init_window_name, text='获取当前位置地址', width='15', command=self.start2)
        self.anniu.grid(row=1, column=5, pady='10')
        self.anniu = Button(self.init_window_name, text='设置文件位置', width='15', command=self.start4)
        self.anniu.grid(row=1, column=6, pady='10')
        self.anniu = Button(self.init_window_name, text='获取随机影片名字', width='15', command=self.start3)
        self.anniu.grid(row=1, column=2, pady='10')
        self.anniu = Button(self.init_window_name, text='返回按钮', width='15', command=self.init_window_name.destroy)
        self.anniu.grid(row=3, column=2, pady='10')
        self.anniu = Button(self.init_window_name, text='默认格式', width='15', command=self.start5)
        self.anniu.grid(row=2, column=2, pady='10')

    def kaisi1(self):
        self.bianm = str(self.Entory2.get())
        self.st = str(self.Entory0.get())
        self.st1 = self.Entory1.get()
        if self.st == '' or self.st1 == '' or self.bianm == '':
            askyesno(title='是/否对话框', message='提示！！输入框没文件地址或者没影片名字！')
        else:
            askyesno(title='是/否对话框', message='提示录制已经开始,想结束录制请按暂停键否则可能出现错误')
            self.th = threading.Thread(target=video_record(self.st1, self.st, self.bianm))
            self.th.setDaemon(True)
            self.th.start()

    # 开始
    def kais1(self):
        self.T = threading.Thread(target=self.kaisi1)  # 多线程
        self.T.setDaemon(True)  # 线程守护，即主进程结束后，此线程也结束。否则主进程结束子进程不结束
        self.T.start()  # 启动

    # 暂停
    def start1(self):
        self.bianm = str(self.Entory2.get())
        self.st = str(self.Entory0.get())
        self.st1 = self.Entory1.get()
        if self.st == '' or self.st1 == '' or self.bianm == '':
            askyesno(title='是/否对话框', message='提示！！输入框没文件地址或者没影片名字！')
        else:
            self.flag = True
            fu(self.flag)
            self.T = False
            self.th = False
            askyesno(title='是/否对话框', message=f'提示！！当前文件保存在{self.st1}')
            self.Entory0.delete('0')

    # 获取地址
    def start2(self):
        self.st1 = os.getcwd()
        self.Entory1.delete(0, tkinter.END)
        self.Entory1.insert(tkinter.INSERT, self.st1)
        self.Entory1.update()  # 刷新

    # 获取随机名字
    def start3(self):
        self.st = fu1(3)
        self.Entory0.delete(0, tkinter.END)
        self.Entory0.insert(tkinter.INSERT, self.st)
        self.Entory0.update()  # 刷新

    # 打开文件位置
    def start4(self):
        self.path_ = askdirectory()  # 使用askdirectory()方法返回文件夹的路径
        if self.path_ == "":
            self.var0.get()  # 当打开文件路径选择框后点击"取消" 输入框会清空路径，所以使用get()方法再获取一次路径
        else:
            self.path_ = self.path_.replace("/", "\\")  # 实际在代码中执行的路径为“\“ 所以替换一下
            self.var0.set(self.path_)

    # 设置默认格式
    def start5(self):
        fo = ['MP42', 'XVID', 'MP4V', 'DIV3']
        xtc = random.choice(fo)
        askyesno(title='是/否对话框', message='提示！！录屏格式不同可能导致录屏出来得文件大小和播放方式不同')
        self.var2.set(xtc)
        self.Entory2.update()  # 刷新


class MU_TR:
    def __init__(self):
        self.init_dett = tkinter.Tk()
        self.init_dett.title('工具')
        self.init_dett.geometry('1000x800')
        self.init_det()

    def init_det(self):
        self.init_data_label = Label(self.init_dett, text='自动化输入工具(zhang hong yin)', font=20)
        self.init_data_label.grid(row=0, column=3, padx='20', pady='20')
        self.init_data_label = Label(self.init_dett, text='你要输入的：', font=20)
        self.init_data_label.grid(row=2, padx='10', pady='10', column=0)
        self.init_data_label = Label(self.init_dett, text='执行次数：', font=20)
        self.init_data_label.grid(row=2, padx='10', pady='10', column=2)
        self.init_data_label = Label(self.init_dett, text='快捷键：', font=20)
        self.init_data_label.grid(row=2, padx='10', pady='10', column=4)
        self.anniu = Button(self.init_dett, text='开始输入', width='15', command=self.kais1)
        self.anniu.grid(row=3, column=0, pady='10')
        self.anniu = Button(self.init_dett, text='返回', width='15', command=self.init_dett.destroy)
        self.anniu.grid(row=3, column=1, pady='10')
        self.iv_default = IntVar()
        self.rb_default1 = Radiobutton(self.init_dett, text='中文', value=1, variable=self.iv_default)
        self.rb_default2 = Radiobutton(self.init_dett, text='英文或者数字', value=2, variable=self.iv_default)
        self.rb_default1.grid(row=3, column=2, sticky='W')
        self.rb_default2.grid(row=3, column=3, sticky='W')
        self.var0 = tkinter.StringVar()
        self.Entory0 = tkinter.Entry(self.init_dett, textvariable=self.var0)
        self.Entory0.grid(row=2, column=1, padx='10', pady='10')
        self.Entory1 = tkinter.Entry(self.init_dett)
        self.Entory1.grid(row=2, column=3, padx='10', pady='10')
        self.Entory2 = tkinter.Entry(self.init_dett)
        self.Entory2.grid(row=2, column=5, padx='10', pady='10')

    def fun(self):
        if self.iv_default.get() == 0:
            askyesno(title='是/否对话框', message='提示！！请选择中文or英文')
        else:
            self.rt = self.Entory0.get()
            self.rt1 = int(self.Entory1.get())
            self.rt2 = str(self.Entory2.get()).split('+')
            askyesno(title='是/否对话框', message='提示！！10秒后执行，请选择输入好位置')
            st = list(pinyin.get(self.rt, format="strip", delimiter=" "))
            time.sleep(10)
            self.i = 0
            while self.rt1 > 0:
                if st[self.i] == ' ':
                    self.i += 1
                    continue
                pyautogui.keyDown(st[self.i])
                self.i += 1
                if self.i == len(st):
                    self.i = 0
                    self.rt1 -= 1
                    if self.iv_default.get() == 1:
                        pyautogui.keyDown('1')
                    if len(self.rt2) > 1:
                        self.str1, self.str2 = self.rt2
                        pyautogui.hotkey(self.str1, self.str2)
                    else:
                        pyautogui.keyDown(str(self.rt2))
            askyesno(title='是/否对话框', message='提示！！执行完毕')

    # 开始
    def kais1(self):
        self.T = threading.Thread(target=self.fun)  # 多线程
        self.T.setDaemon(True)  # 线程守护，即主进程结束后，此线程也结束。否则主进程结束子进程不结束
        self.T.start()  # 启动


class MY_GUI2:
    def __init__(self):
        self.init_de = tkinter.Tk()
        self.init_de.title('工具')
        self.init_de.geometry('1010x1000')
        self.init_det1()

    def init_det1(self):
        self.init_data_label = Label(self.init_de, text='爬小说工具(zhang hong yin)', font=20)
        self.init_data_label.grid(row=0, padx='10', pady='10', column=1, columnspan=3)
        self.anniu = Button(self.init_de, text='开始抓取', width='15', command=self.init_det2)
        self.anniu.grid(row=3, column=0, pady='10')
        self.anniu = Button(self.init_de, text='退出程序', width='15', command=self.init_de.destroy)
        self.anniu.grid(row=3, column=1, pady='10')
        self.init_data_label = Label(self.init_de, text='小说名:')
        self.init_data_label.grid(row=1, padx='10', pady='10', column=0)
        self.var0 = tkinter.StringVar()
        self.Entory0 = tkinter.Entry(self.init_de, textvariable=self.var0)
        self.Entory0.grid(row=1, column=1, sticky=W)
        self.height_width_text1 = Text(self.init_de, height=10, width=46)
        self.height_width_text1.grid(row=1, column=2, sticky=W, rowspan=6)
        self.height_width_text2 = Text(self.init_de, height=10, width=60)
        self.height_width_text2.grid(row=1, column=3, sticky=W, rowspan=6)

    def init_det2(self):
        try:
            a = []
            sd = 0
            st = self.fu()
            co1 = {a.append(i['value']) for i in
                   BeautifulSoup(requests.get(f'https://m.1qxs.com/list/{st[1][0]}/1', headers=hear).text,
                                 'html.parser').find_all('option')}
            sd1 = BeautifulSoup(requests.get(f'https://m.1qxs.com/list/{st[1][0]}/1', headers=hear).text,
                                'html.parser').find_all('option', attrs={'value': a[-1]})
            for i in sd1:
                sd = int(i.text.split('-')[1].rstrip('章'))
                break
            if st[0] == 1:
                s = Queue()
                self.kais1(s, st[1][0], a, self.pa)
                self.kais1(s, st[1][0], sd, self.xz)
            else:
                askyesno(title='是/否对话框', message='提示！！请重新输入一七小说名')
        except IndexError:
            askyesno(title='是/否对话框', message='提示！！请输入一七小说名')

    # 找书籍
    def pa(self, q, q1, a):
        try:
            for i in range(1, int(a[-1]) + 1):
                re = requests.get(f'https://m.1qxs.com/list/{q1}/{i}', headers=hear).text
                co = {q.put(j.text.strip()) for j in BeautifulSoup(re, 'html.parser').find_all('li')}
                self.height_width_text1.insert('1.0', '爬取页面成功{}'.format(f'第{i}页\n'))
                self.init_de.update()
            q.put('鹤顶红')
        except RuntimeError:
            pass

    def xz(self, q, q1, a):
        try:
            st = str(self.var0.get())
            self.st1 = os.getcwd()
            folder = os.path.exists(self.st1 + r'\小说')
            folder1 = os.path.exists(self.st1 + r'\小说' + st)
            if not folder:
                os.makedirs('小说')
            if not folder1:
                os.makedirs(fr'小说\{st}')
            time.sleep(5)
            for i in range(1, a + 1):
                s1 = q.get()
                if s1 == '鹤顶红':
                    break
                for j in range(1, 4):
                    re = requests.get(f'https://m.1qxs.com/xs/{q1}/{i}/{j}', headers=hear).text
                    time.sleep(0.2)
                    co = BeautifulSoup(re, 'html.parser').find_all('div', attrs={'class': 'content'})
                    for s in co:
                        if not s:
                            break
                        with open(f'小说/{st}/{str(s1[1:]).strip(string.digits)}.text', 'at', encoding='utf-8') as f:
                            f.write(str(s1[1:]).strip(string.digits))
                            f.write(s.text.strip())
                self.height_width_text2.insert('1.0', '下载成功{}'.format(
                    str(s1[1:]).strip(string.digits)) + f' https://m.1qxs.com/xs/{q1}/{i}\n')
                self.init_de.update()
            askyesno(title='是/否对话框', message='提示！！下载成功')
        except RuntimeError:
            pass

    def fu(self):
        sf = []
        sf1 = []
        st = str(self.var0.get())
        st1 = requests.get('https://m.1qxs.com/search?{}'.format(parse.urlencode({'kw': st})), headers=hear)
        fw = {sf.append(i.find('div', attrs={'class': 'name line_1'}).text) for i in
              BeautifulSoup(st1.text, 'html.parser').find_all('div', attrs={'class': 'book'})}
        fw1 = {sf1.append(i.find('a')['href'][4:]) for i in BeautifulSoup(st1.text, 'html.parser').find_all('ul')}
        if st in sf:
            return 1, sf1
        else:
            return 0, sf1

    def kais1(self, a, b, c, s):
        self.T = threading.Thread(target=s, args=(a, b, c,))  # 多线程
        self.T.setDaemon(True)  # 线程守护，即主进程结束后，此线程也结束。否则主进程结束子进程不结束
        self.T.start()  # 启动


class MY_GUI1:
    def __init__(self):
        self.init_de = tkinter.Tk()
        self.init_de.title('工具')
        self.init_de.geometry('680x800')
        self.init_det()

    def init_det(self):
        self.init_data_label = Label(self.init_de, text='工具(zhang hong yin)', font=20)
        self.init_data_label.grid(row=0, padx='10', pady='10', column=2, columnspan=2)
        self.anniu = Button(self.init_de, text='录屏工具', width='15', command=self.init_det1)
        self.anniu.grid(row=3, column=0, pady='10')
        self.anniu = Button(self.init_de, text='自动输入', width='15', command=self.init_dett)
        self.anniu.grid(row=3, column=1, pady='10')
        self.anniu = Button(self.init_de, text='退出程序', width='15', command=self.init_de.destroy)
        self.anniu.grid(row=3, column=2, pady='10')
        self.anniu_2 = Button(self.init_de, text='自服务器', width='15', command=self.init_de1)
        self.anniu_2.grid(row=3, column=3, pady='10')
        self.anniu_2 = Button(self.init_de, text='小说工具', width='15', command=self.init_de2)
        self.anniu_2.grid(row=3, column=4, pady='10')

    def init_de2(self):
        self.init_de.destroy()
        gf2 = MY_GUI2()
        gf2.init_de.mainloop()
        gf = MY_GUI1()
        gf.init_de.mainloop()

    def init_de1(self):
        self.init_de.destroy()
        tf = Pyt_f3()
        tf.int2.mainloop()
        gf = MY_GUI1()
        gf.init_de.mainloop()

    def init_det1(self):
        self.init_de.destroy()
        gf = MY_GUI()
        gf.init_window_name.mainloop()
        gf = MY_GUI1()
        gf.init_de.mainloop()

    def init_dett(self):
        self.init_de.destroy()
        gf = MU_TR()
        gf.init_dett.mainloop()
        gf = MY_GUI1()
        gf.init_de.mainloop()


if __name__ == '__main__':
    gf1 = MY_GUI1()
    gf1.init_de.mainloop()
