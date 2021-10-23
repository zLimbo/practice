# -*- coding:UTF-8 -*-

import requests

# 创建session对象，可以保存Cookie值
ssion = requests.session()

# 处理 headers
#headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36"}
headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.80 Safari/537.36"}

# 3需要登录的用户名和密码
data = {"txtUserName":"Y11614007", "TextBox2":"fw123456"}  

# 发送附带用户名和密码的请求，并获取登录后的Cookie值，保存在ssion里
ssion.post("http://jw2.ahu.cn/default2.aspx", data = data)

#  ssion包含用户登录后的Cookie值，可以直接访问那些登录后才可以访问的页面
#response = ssion.get("http://jw2.ahu.cn/xs_main.aspx?xh=Y11614007")

# 打印响应内容
#print(response.text)