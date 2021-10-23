# -*- coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup

if __name__ == '__main__':

    with open('ahu.txt', 'wb') as file:
        for page_no in range(1, 21):
            #print('page', page_no)
            target_url = 'http://jwc.ahu.cn/main/more.asp?classid=2&page=' + str(page_no)
            req = requests.get(url = target_url)
            req.encoding = 'gb18030'

            html = req.text
            bs = BeautifulSoup(html, features="html.parser")
     
            for tag in bs.find_all('tr', bgcolor='#ffffff'):
                try:
                    title = tag.a.string.strip()
                    date = tag.find('td', class_='timecss').string.strip()
                    item = '{"title":"%s","date":"%s"}\n'%(title, date)
                    file.write(item.encode('gb18030'))
                except Exception as e:  
                    print("fail in : title=", title, 'date=', date)
                    print(e)

