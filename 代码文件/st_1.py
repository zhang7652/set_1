# 爬取一七小说
import os
import string
import threading
import time
from multiprocessing import Queue
from urllib import parse

import requests
from bs4 import BeautifulSoup

hear = {
    'User-Agent': 'Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) '
                  'Chrome/111.0.0.0 Mobile Safari/537.36 Edg/111.0.1661.41'
}


# 找书籍
def pa(q, q1, a):
    try:
        for i in range(1, int(a[-1]) + 1):
            re = requests.get(f'https://m.1qxs.com/list/{q1}/{i}', headers=hear).text
            co = {q.put(j.text.strip()) for j in BeautifulSoup(re, 'html.parser').find_all('li')}
        q.put('鹤顶红')
    except RuntimeError:
        print("找寻结束")
        pass


def xz(q, q1, a, st):
    try:
        st1 = os.getcwd()
        folder = os.path.exists(st1 + r'\小说')
        folder1 = os.path.exists(st1 + r'\小说' + st)
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
                    with open(f'小说/{st}/{str(s1[1:]).strip(string.digits)}.txt', 'at', encoding='utf-8') as f:
                        f.write(str(s1[1:]).strip(string.digits))
                        f.write(s.text.strip())
                print(f"爬取{str(s1[1:]).strip(string.digits)}小说成功")
        print("小说下载成功")
    except RuntimeError:
        pass


if __name__ == "__main__":
    while True:
        sf = []
        sf1 = []
        st = str(input("请输入小说名字：>>").strip())
        st_1 = st
        print(f"正在查找小说:{st_1},请等待")
        st1 = requests.get('https://m.1qxs.com/search?{}'.format(parse.urlencode({'kw': st})), headers=hear)
        fw = {sf.append(i.find('div', attrs={'class': 'name line_1'}).text) for i in
              BeautifulSoup(st1.text, 'html.parser').find_all('div', attrs={'class': 'book'})}
        fw1 = {sf1.append(i.find('a')['href'][4:]) for i in BeautifulSoup(st1.text, 'html.parser').find_all('ul')}
        if st in sf:
            st = 1, sf1
        else:
            st = 0, sf1
        a = []
        co1 = {a.append(i['value']) for i in
               BeautifulSoup(requests.get(f'https://m.1qxs.com/list/{st[1][0]}/1', headers=hear).text,
                             'html.parser').find_all('option')}
        sd1 = BeautifulSoup(requests.get(f'https://m.1qxs.com/list/{st[1][0]}/1', headers=hear).text,
                            'html.parser').find_all('option', attrs={'value': a[-1]})
        sd = 0
        for i in sd1:
            sd = int(i.text.split('-')[1].rstrip('章'))
            if st[0] != 1:
                break
            else:
                print(f"一共{sd}章")
                break
        if st[0] == 1:
            s = Queue()
            T_1 = threading.Thread(target=pa, args=(s, st[1][0], a,))
            T_2 = threading.Thread(target=xz, args=(s, st[1][0], sd, st_1,))
            T_1.start()  # 启动
            T_2.start()  # 启动
        else:
            print("小说名有问题,请重新输入小说名字")
            continue
