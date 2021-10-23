# -*- coding:UTF-8 -*-

import requests
from selenium import webdriver

chrome_path = r'D:\Green\chromedriver_win32\chromedriver.exe'
login_url = r'http://jw2.ahu.cn/default2.aspx'

wd = webdriver.Chrome(executable_path = chrome_path)
wd.get(login_url)

try:
    wd.find_element_by_xpath('//*[@id="txtUserName"]').send_keys('Y11614007')
except:
    pass
wd.find_element_by_xpath('//*[@id="Textbox1"]').send_keys('fw123456')
wd.find_element_by_xpath('//*[@id="RadioButtonList1_2"]').click()
wd.find_element_by_xpath('//*[@id="txtSecretCode"]').send_keys(input('输入验证码:'))
wd.find_element_by_xpath('//*[@id="Button1"]').click()


req = requests.Session()
cookies = wd.get_cookies()
for cookie in cookies:
    req.cookies.set(cookie['name'], cookie['value'])
test = req.get('http://jw2.ahu.cn/xs_main.aspx?xh=Y11614007')