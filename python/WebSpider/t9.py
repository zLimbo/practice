import image_recognize
import time
import requests
from bs4 import BeautifulSoup
from PIL import Image, ImageEnhance
import pandas as pd
import json

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

# 获取登录数据的函数  url：登录页面，user：学号，pwd：密码
def get_post_data(url,user,pwd):
    re = S.get(url)

    #解析得到__VIEWSTATE值
    soup = BeautifulSoup(re.text, features="html.parser")
    viewState = soup.find('input', attrs={'name': '__VIEWSTATE'})['value']
    #print(viewState)

    # 把cookie转变成字典类型
    cookies = requests.utils.dict_from_cookiejar(S.cookies)
    header.update(cookies)
    header_code.update(cookies)
    #print(cookies)
    #cookie: 'ASP.NET_SessionId': 'i0uqsnzujwoopseqonnhijrk'}
    # 获取验证码，下载到本地
    code = S.get(URL_CODE, headers=header_code)
    with open("code.jpg", "wb") as f:
        f.write(code.content)

    # 打开验证码图片
    image = Image.open('code.jpg')
    #预处理图片
    image = image.convert('RGB')
    enhancer = ImageEnhance.Contrast(image)
    image = enhancer.enhance(20)
    #自动识别验证码
    checkCode = image_recognize.baiduOcr(image)
    #print('checkCode: ', checkCode)

    # post的登录数据
    login_info = {
        "__VIEWSTATE": viewState,
        "txtUserName": user,
        "TextBox2": pwd,
        "txtSecretCode": checkCode,
        "RadioButtonList1": "%D1%A7%C9%FA",#学生选项
        "Button1": "",
        "lbLanguage": ""
    }
    return login_info

# 登录操作 url为登录界面  data为登录账号密码
def login(url, data):
    req = S.post(url=url, data=data, headers=header)
    geren_html = req.text
    #print(geren_html)
    
    # 判断登录数据是否正确
    if judge(geren_html) == 1:
        #print(geren_html)
        z_name = re.compile('<span id="xhxm">(.*?)</span></em>',re.S)
        name = re.findall(z_name,geren_html)
        print('登录成功！'+name[0],'欢迎您')
        xh = data['txtUserName']
        xm = name[0]
        
        #进行获取操作
        getCourse(xh,xm)
    else:
        print(judge(geren_html))
        print("正在重新登录...")
        time.sleep(1)
        main()
    
    
def getCourse(xh, xm):
    #获取学生信息所需提交的数据
    data={
        'ddlXN':'2018-2019',
        'ddlXQ':'1',
        '__EVENTVALIDATION': '',
        '__EVENTTARGET':'',   
        '__EVENTARGUMENT' :'',
        '__VIEWSTATE':'',
        'hidLanguage':'',
        'ddl_kcxz':'',
    }
    
    #课程信息页面获取关键字段
    course_html_1=S.get('http://jw2.ahu.cn/xsxkqk.aspx?xh='+xh+'&xm='+xm+'&gnmkdm=N121605',headers=header)
    soup=BeautifulSoup(course_html_1.text, features="html.parser")
    #print(soup)
    value3=soup.find('input', attrs={'name': '__VIEWSTATE'})['value']
    data['__VIEWSTATE']=value3
    
    course_html_2 = S.post('http://jw2.ahu.cn/xsxkqk.aspx?xh='+xh+'&xm='+xm+'&gnmkdm=N121605',data=data,headers=header)
    
    with open("course.html", "wb") as f:
        f.write(course_html_2.text.encode('gb18030'))
    soup2 = BeautifulSoup(course_html_2.text, 'html.parser')
    
    # 获得课程信息，写入json文件
    table = soup2.find('table')
    cnt = 0
    courses = {"course":[]}
    for child in table.children:
        if child.name == 'tr':
            cnt += 1
            if cnt != 1:
                tds = child.find_all('td')
                course = dict()
                course["课程代码"] = tds[1].string
                course["课程名称"] = tds[2].string
                course["学分"] = tds[6].string
                course["上课时间"] = tds[8].span.string
                #print(course)
                courses["course"].append(course)
    with open("ahu.json", "w", encoding='gb18030') as f:
        json.dump(courses, f, ensure_ascii=False)
        print('写入成功')
    
# 判断函数  script是提取js弹窗信息 返回值1为登录成功
def judge(html):
    soup_judge = BeautifulSoup(html, 'html.parser')
    script = soup_judge.find_all('script')[1].text #在第二个（序号为1）script部分获取错误状态
    if "验证码不正确！！" in script:
        return "验证码错误"
    elif "用户名不存在" in script:
        return "用户名不存在"
    elif "密码错误" in script:
        return "密码错误"
    elif "验证码不能为空，如看不清请刷新！！" in script:
        return "验证码为空"
    else:
        return 1
    
def main():
    data = get_post_data(URL,user,pwd)
    login(URL, data)

if __name__ == '__main__':
    #输入学号密码
    user = input("学号:")
    pwd = input("密码:")
    main()