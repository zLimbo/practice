from aip import AipOcr
import re
import pytesseract

# 二值化算法
def binarizing(img,threshold):
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

def pretreatment(image, hold=10):
    # 转化为灰度图
    image = image.convert('L')
    # 把图片变成二值图像
    image = binarizing(image, hold)
    #   去干扰线
    image=depoint(image)
    return image


def preOcr(image):
    '''image： PIL 模块的 Image.open()创建的图片对象
    return：识别结果
    '''
    image = pretreatment(image)
    
#     # 若tesseract环境变量未配置，需要设置以下参数
#     # tesseract 路径
#    pytesseract.pytesseract.tesseract_cmd = r'D:\Program Files\Tesseract-OCR'
#     # 语言包目录和参数
 #   tessdata_dir_config = r'--tessdata-dir "D:\Program Files\Tesseract-OCR\tessdata" --psm 6'
#    result = pytesseract.image_to_string(image, config=tessdata_dir_config)

    result = pytesseract.image_to_string(image)
     #去掉非法字符，只保留字母数字
    result = re.sub("\W", "", result)
    return result

response = ''

def baiduOcr(image):
    '''image： PIL 模块的 Image.open()创建的图片对象
    return：识别结果
    '''
    result = ''
    options = {'language_type':'ENG'}
    image = pretreatment(image)
    image.save('temp.jpg')
    with open('temp.jpg','rb') as f:
        image = f.read()
#     百度文字识别接口，精确识别时间会长一点，所以用基础识别
    try:
        response = client.basicGeneral(image, options)
        result = response['words_result'][0]['words']
    except Exception as e:
        print (response,e)
    return re.sub("\W", "", result)

APP_ID = '16456650'
API_KEY = 'b8oSXq9N8lmx5XfyyCEczUcL'
SECRET_KEY = '0cwgwmSEEAQHOHxy6SK0B1T6yNAPQTVY'

client = AipOcr(APP_ID, API_KEY, SECRET_KEY)