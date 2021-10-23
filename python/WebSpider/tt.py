from aip import AipOcr
    
# ��ֵ���㷨
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

# ȥ���������㷨
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
    # ת��Ϊ�Ҷ�ͼ
    image = image.convert('L')
    # ��ͼƬ��ɶ�ֵͼ��
    image = binarizing(image, hold)
    #   ȥ������
    image=depoint(image)
    return image

def ocr_img(image):
    '''image�� PIL ģ��� Image.open()������ͼƬ����
    return��ʶ����
    '''
    image = pretreatment(image)
    
#     # ��tesseract��������δ���ã���Ҫ�������²���
#     # tesseract ·��
#     pytesseract.pytesseract.tesseract_cmd = r'E:\SoftInstall\TesseractOCR\Tesseract-OCR'
#     # ���԰�Ŀ¼�Ͳ���
#     tessdata_dir_config = r'--tessdata-dir "E:\SoftInstall\TesseractOCR\Tesseract-OCR\tessdata" --psm 6'
#     result = pytesseract.image_to_string(image, config=tessdata_dir_config)

    result = pytesseract.image_to_string(image)
     #ȥ���Ƿ��ַ���ֻ������ĸ����
    result = re.sub("\W", "", result)
    return result

response = ''

def baiduOcr(image):
    '''image�� PIL ģ��� Image.open()������ͼƬ����
    return��ʶ����
    '''
    result = ''
#     options = {'language_type':'ENG'}
    image = pretreatment(image)
    image.save('temp.jpg')
    with open('temp.jpg','rb') as f:
        image = f.read()
#     �ٶ�����ʶ��ӿڣ���ȷʶ��ʱ��᳤һ�㣬�����û���ʶ��
    try:
        response = client.basicGeneral(image)
        result = response['words_result'][0]['words']
    except Exception as e:
        print (response,e)
    return re.sub("\W", "", result)

APP_ID = '16456650'
API_KEY = 'b8oSXq9N8lmx5XfyyCEczUcL'
SECRET_KEY = '0cwgwmSEEAQHOHxy6SK0B1T6yNAPQTVY'

client = AipOcr(APP_ID, API_KEY, SECRET_KEY)


import re
import time
import requests
from bs4 import BeautifulSoup
from PIL import Image, ImageEnhance
import pandas as pd

#�����ɼ���
df_data = pd.DataFrame(columns=["ѧ��","ѧ��","�γ̴���","�γ�����","�γ�����","�γ̹���","ѧ��","����","ƽʱ�ɼ�","���гɼ�","��ĩ�ɼ�","ʵ��ɼ�","�ɼ�","���ޱ��","�����ɼ�","���޳ɼ�","����ѧԺ","��ע","���ޱ��"])

header = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
    "Accept-Encoding": "gzip, deflate",
    "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
    "User-Agent": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36",
    "Referer": "http://218.56.144.61/",
    "Host": "218.56.144.61",
    "Cache-Control": "max-age=0"
}
header_code = {
    "User-Agent": "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 10.0; WOW64; Trident/7.0; .NET4.0C; \.NET4.0E; .NET CLR 2.0.50727; .NET CLR 3.0.30729; .NET CLR 3.5.30729)",
    "Referer": "http://218.56.144.61/",
    "Host": "218.56.144.61",
    "Cache-Control": "max-age=0"
}
URL = "http://jw2.ahu.cn/default2.aspx"
URL_CODE = "http://jw2.ahu.cn/CheckCode.aspx"

S = requests.session()

# ��ȡ��¼���ݵĺ���  url����¼ҳ�棬user��ѧ�ţ�pwd������
def get_post_data(url,user,pwd):
    re = S.get(url)

    #�����õ�__VIEWSTATEֵ
    soup = BeautifulSoup(re.text, features="html.parser")
    viewState = soup.find('input', attrs={'name': '__VIEWSTATE'})['value']
    #print(viewState)

    # ��cookieת����ֵ�����
    cookies = requests.utils.dict_from_cookiejar(S.cookies)
    
    header.update(cookies)
    header_code.update(cookies)

    # ��ȡ��֤�룬���ص�����
    code = S.get(URL_CODE, headers=header_code)
    with open("code.jpg", "wb") as f:
        f.write(code.content)

    # ����֤��ͼƬ
    image = Image.open('code.jpg')
    #Ԥ����ͼƬ
    image = image.convert('RGB')
    enhancer = ImageEnhance.Contrast(image)
    image = enhancer.enhance(20)
    #�Զ�ʶ����֤��
    checkCode = baiduOcr(image)

    # post�ĵ�¼����
    login_info = {
        "__VIEWSTATE": viewState,
        "txtUserName": user,
        "TextBox2": pwd,
        "txtSecretCode": checkCode,
        "RadioButtonList1": "%D1%A7%C9%FA",#ѧ��ѡ��
        "Button1": "",
        "lbLanguage": ""
    }
    return login_info

# ��¼���� urlΪ��¼����  dataΪ��¼�˺�����
def login(url, data):
    req = S.post(url=url, data=data, headers=header)
    geren_html = req.text
    #print(geren_html)
    
    # �жϵ�¼�����Ƿ���ȷ
    if judge(geren_html) == 1:
        z_name = re.compile('<span id="xhxm">(.*?)</span></em>',re.S)
        name = re.findall(z_name,geren_html)
        print('��¼�ɹ���'+name[0],'��ӭ��')
        xh = data['txtUserName']
        xm = name[0]
        
        #���л�ȡ����
        #findGrade(xh,xm)
    else:
        print(judge(geren_html))
        print("�������µ�¼...")
        time.sleep(1)
        main()

#��ȡ�ɼ���Ϣ����
def findGrade(xh,xm):
    #��ȡѧ����Ϣ�����ύ������
    data={
        'btn_zcj':'%C0%FA%C4%EA%B3%C9%BC%A8',#ѧ��ɼ���btn_xn ����ɼ���btn_zcj
        'ddlXN':'',
        'ddlXQ':'',
        '__EVENTVALIDATION': '',
        '__EVENTTARGET':'',   
        '__EVENTARGUMENT' :'',
        '__VIEWSTATE':'',
        'hidLanguage':'',
        'ddl_kcxz':'',
    }
    
    #���ɼ�ҳ���ȡ�ؼ��ֶ�
    cj_html_1=S.get('http://jw2.ahu.cn/xscjcx.aspx?xh='+xh+'&xm='+xm+'&gnmkdm=N121605',headers=header)
    soup=BeautifulSoup(cj_html_1.text,'lxml')
    #print(soup)
    value3=soup.find('input', attrs={'name': '__VIEWSTATE'})['value']
    data['__VIEWSTATE']=value3
    
    cj_html_2 = S.post('http://jw2.ahu.cn/xscjcx.aspx?xh='+xh+'&xm='+xm+'&gnmkdm=N121605',data=data,headers=header)
    #print (cj_html_2.text)
    
    z_cj=re.compile('<td>(.*?)</td>',re.S)
    cj = re.findall(z_cj,cj_html_2.text)
    
    #�������ܿ�Ŀ��
    max = int((len(cj)-24)/19)
    for i in range(0,max): #ѭ��ѧ�������п�Ŀ
        hang = [] #���ڴ洢������Ϣ��Ϊһ������
        for j in range(20+i*19,20+i*19+19): #ѭ����ȡ�ɼ���ϸ��Ϣ
            if(cj[j]=='&nbsp;'):
                cj[j]=''
            hang.append(cj[j])
        #print(hang)
        df_data.loc[i]=hang
    df_data.to_excel(xm+'�ĳɼ���.xlsx')
    print('�ļ������ɹ���')
    
# �жϺ���  script����ȡjs������Ϣ ����ֵ1Ϊ��¼�ɹ�
def judge(html):
    soup_judge = BeautifulSoup(html, 'html.parser')
    script = soup_judge.find_all('script')[1].text #�ڵڶ��������Ϊ1��script���ֻ�ȡ����״̬
    if "��֤�벻��ȷ����" in script:
        return "��֤�����"
    elif "�û���������" in script:
        return "�û���������"
    elif "�������" in script:
        return "�������"
    elif "��֤�벻��Ϊ�գ��翴������ˢ�£���" in script:
        return "��֤��Ϊ��"
    else:
        return 1
    
def main():
    data = get_post_data(URL,user,pwd)
    login(URL, data)

if __name__ == '__main__':
    #����ѧ������
    #user = input("ѧ��:")
    #pwd = input("���루Ĭ��Ϊ���֤�ţ�:")
    user = 'Y11614007'
    pwd = 'fw123456'
    main()