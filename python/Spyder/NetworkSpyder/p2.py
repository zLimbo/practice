# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
import requests

if __name__ == "__main__":
	server = 'https://www.biquge.com.cn'
	target = 'https://www.biquge.com.cn/book/32101/'
	req = requests.get(url = target)
	print(req.text)