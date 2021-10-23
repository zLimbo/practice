# -*- coding: utf-8 -*-
"""
Created on Fri Jun  7 12:48:33 2019

@author: zLimbo
"""

from selenium import webdriver
#
#chromePath = r'D:\Green\chromedriver_win32\chromedriver.exe'
#
#wd = webdriver.Chrome(executable_path = chromePath)
#
#wd.find_element_by_x


#//*[@id="RadioButtonList1_2"]

browser = webdriver.Chrome()
browser.get('http://www.baidu.com/')