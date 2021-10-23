# -*- coding:UTF-8 -*-

import requests
from selenium import webdriver

chrome_path = r'D:\Green\chromedriver_win32\chromedriver.exe'
login_url = r'http://jw2.ahu.cn/default2.aspx'

wd = webdriver.Chrome(executable_path = chrome_path)
wd.get(login_url)

try:
    wd.execute_script("document.getElementById('txtUserName').value='Y11614007';")
except:
    pass
    
try:
    wd.execute_script("document.getElementById('TextBox2').value='fw123456';")
except:
    pass

try: 
    wd.execute_script("document.getElementById('RadioButtonList1_2').click();")
except:
    pass
 
secret_code = input('输入验证码:') 
try:
    wd.execute_script("document.getElementById('txtSecretCode').value='" + secret_code + "';")
except:
    pass
    
try:
    wd.execute_script("document.getElementById('Button1').click();")
except:
    pass

req = requests.Session()
cookies = wd.get_cookies()
for cookie in cookies:
    req.cookies.set(cookie['name'], cookie['value'])
test = req.get('http://jw2.ahu.cn/xs_main.aspx?xh=Y11614007')
print(cookie)