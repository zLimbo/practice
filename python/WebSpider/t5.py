# -*- coding:UTF-8 -*-

import requests
from bs4 import BeautifulSoup

target_url = 'http://jw2.ahu.cn/default2.aspx'
headers = {
    'Host': 'jw2.ahu.cn',
    'Connection': 'keep-alive',
    'Content-Length': '232',
    'Cache-Control': 'max-age=0',
    'Origin': 'http://jw2.ahu.cn',
    'Upgrade-Insecure-Requests': '1',
    'Content-Type': 'application/x-www-form-urlencoded',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.80 Safari/537.36',
    'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3',
    'Referer': 'http://jw2.ahu.cn/default2.aspx',
    'Accept-Encoding': 'gzip, deflate',
    'Accept-Language': 'en,zh-CN;q=0.9,zh;q=0.8',
    'Cookie': 'ASP.NET_SessionId=nuptfyz2diatxl55u3zn0ras'
}

se = requests.session()

re = requests.session().get(target_url)
soup = BeautifulSoup(re.text, features="html.parser")
viewState = soup.find('input', attrs={'name': '__VIEWSTATE'})['value']

code_img_src = r'http://jw2.ahu.cn/CheckCode.aspx'

img = requests.get(code_img_src)
with open('CheckCode.aspx', 'wb') as imgFile:
    imgFile.write(img.content)

info = {
    "__VIEWSTATE": viewState,
    "txtUserName": "Y11614007", 
    "TextBox2": "fw123456",
    "txtSecretCode": "",
    "RadioButtonList1": "%D1%A7%C9%FA", #学生选项
    "Button1": "",
    "lbLanguage": ""
}