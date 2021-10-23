import selenium
from selenium import webdriver
from selenium.webdriver.support.select import Select
from selenium.webdriver.common.action_chains import ActionChains
from bs4 import BeautifulSoup
import re
import time
import pytesseract
from PIL import Image

chrome_path = r'D:\Green\chromedriver_win32\chromedriver.exe'

TARGET_URL = "http://jw2.ahu.cn/default2.aspx"
RESULT_FILE_NAME = "result.txt"
ACCT_NAME = input("username:")
PWD = input("password:")
tessdata_dir_config = r'--tessdata-dir "F:\tesseract-ocr\tessdata"'

driver = webdriver.Chrome()
driver.get(TARGET_URL)


def isElementExist(xpath):
    try:
        driver.find_element_by_xpath(xpath)
        return True
    except:
        return False


def findSubstring(string, substring, times):
    current = 0
    for i in range(1, times + 1):
        current = string.find(substring, current + 1)
        if current == -1:
            return -1
    return current


while not isElementExist(r'//*[@id="headDiv"]/ul/li[5]/a/span'):
    driver.find_element_by_id("txtUserName").clear()
    driver.find_element_by_id("txtUserName").send_keys(ACCT_NAME)
    driver.find_element_by_id("Textbox1").click()
    driver.find_element_by_id("TextBox2").send_keys(PWD)

    driver.get_screenshot_as_file('captcha.png')
    im = Image.open('captcha.png')
    im = im.crop((1105, 418, 1200, 450))
    im = im.convert('L')
    threshold = 110
    table = []
    for i in range(256):
        if i < threshold:
            table.append(0)
        else:
            table.append(1)
    resImg = im.point(table, '1')
    CAPTCHA = pytesseract.image_to_string(resImg, config=tessdata_dir_config)

    driver.find_element_by_id("txtSecretCode").send_keys(CAPTCHA)
    driver.find_element_by_id("Button1").click()
    try:
        driver.switch_to.alert.accept()
    except:
        continue

ActionChains(driver).move_by_offset(680, 290).click().perform()
ActionChains(driver).move_by_offset(740, 500).click().perform()
driver.get(driver.find_element_by_xpath(r'//*[@id="headDiv"]/ul/li[5]/ul/li[6]/a').get_attribute("href"))
driver.find_element_by_xpath(r'/html/body/h2/a').click()
Select(driver.find_element_by_xpath(r'//*[@id="ddlXN"]')).select_by_value("2018-2019")
soup = BeautifulSoup(driver.page_source, "html.parser")
tbody = soup.find_all("tbody")
raw_items = BeautifulSoup(str(tbody[0]), "html.parser").find_all("tr")
raw_items.pop(0)
with open("ahu.json", "w", encoding="utf-8") as f:
    for item in raw_items:
        sp = BeautifulSoup(str(item), "html.parser")
        list = sp.find_all("td")
        course_code = str(list[1])
        course_code = course_code[4:11]
        course_name = str(list[2])
        course_name = course_name[findSubstring(course_name, ">", 2) + 1:-9]
        credit = str(list[6])
        credit = credit[findSubstring(credit, ">", 1) + 1:-5]
        teaching_time = str(list[8]).replace('\n', '').replace(' ', '')
        teaching_time = teaching_time[teaching_time.find("周"):-12]
        f.write(r'{"course code":"' + course_code + r'","course name":"' + course_name + r'","credit":"' \
                + credit + r'","teaching time":"' + teaching_time + r'"}' + '\n')

# 0：选课课号
# 1：课程代码
# 2：含有课程名
# 3：课程性质
# 4：是否选课
# 5：授课教师
# 6：学分
# 7：周学时
# 8：上课时间
# 9：教室
# 10：教材