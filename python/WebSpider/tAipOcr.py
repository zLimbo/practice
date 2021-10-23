
# -*- coding: UTF-8 -*-
 
from aip import AipOcr
import json
import requests
from PIL import Image

# 定义常量
APP_ID = '16456650'
API_KEY = 'b8oSXq9N8lmx5XfyyCEczUcL'
SECRET_KEY = '0cwgwmSEEAQHOHxy6SK0B1T6yNAPQTVY'
 
# 初始化AipFace对象
client = AipOcr(APP_ID, API_KEY, SECRET_KEY)

check_code_src = r'http://jw2.ahu.cn/CheckCode.aspx'

check_code = requests.get(check_code_src)

with open('CheckCode.jpg', 'wb') as file:
    file.write(check_code.content)
    
check_code_jpg = Image.open('CheckCode.jpg')

check_code_jpg = check_code_jpg.convert('L')

check_code_jpg.save('tmp.jpg')

with open('tmp.jpg', 'rb') as file:
    check_code_jpg = file.read()

ans = client.accurate(check_code_jpg)
print(ans)
