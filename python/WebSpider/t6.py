# -*- coding: UTF-8 -*-

import json
import requests
import re
from PIL import Image, ImageEnhance
from aip import AipOcr
import pytesseract

# 定义常量
APP_ID = '16456650'
API_KEY = 'b8oSXq9N8lmx5XfyyCEczUcL'
SECRET_KEY = '0cwgwmSEEAQHOHxy6SK0B1T6yNAPQTVY'
 
# 初始化AipFace对象
client = AipOcr(APP_ID, API_KEY, SECRET_KEY)

check_code_src = r'http://jw2.ahu.cn/CheckCode.aspx'


# 二值化算法\n",
def binarizing(img, threshold):
    pixdata = img.load()
    w, h = img.size
    for y in range(h):
        for x in range(w):
            if pixdata[x, y] < threshold:
                pixdata[x, y] = 0
            else:
               pixdata[x, y] = 255
    return img
        
# 去除干扰线算法
def depoint(img):   #input: gray image
    pixdata = img.load()
    w,h = img.size
    for y in range(1,h-1):
        for x in range(1,w-1):
            count = 0
            if pixdata[x,y-1] > 245:
                count = count + 1
            if pixdata[x,y+1] > 245:
              count = count + 1
            if pixdata[x-1,y] > 245:
                count = count + 1
            if pixdata[x+1,y] > 245:
                count = count + 1
            if count > 2:
                pixdata[x,y] = 255
    return img
        
def pretreatment(image, hold=60):
    # 转化为灰度图
    image = image.convert('L')
    # 把图片变成二值图像
    image = binarizing(image, hold)
    #   去干扰线\n",
    image=depoint(image)
    return image
    
def baiduOcr(image):
    image = pretreatment(image)
    image.show()
    image.save('tmp.jpg')
    with open('tmp.jpg', 'rb') as file:
        image = file.read()
    ans = client.accurate(image)
    result = ans['words_result'][0]['words']
    return re.sub("\W", "", result)
   
if __name__ == '__main__':

    check_code = requests.get(check_code_src)

    with open('CheckCode.jpg', 'wb') as file:
        file.write(check_code.content)
        
    check_code_jpg = Image.open('CheckCode.jpg')

    check_code_jpg = check_code_jpg.convert('RGB')
    enhancer = ImageEnhance.Contrast(check_code_jpg)
    check_code_jpg = enhancer.enhance(20)
    result = baiduOcr(check_code_jpg)
    print(result)
    
