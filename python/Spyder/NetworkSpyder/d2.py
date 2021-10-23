# -*- coding:UTF-8 -*-
from bs4 import BeautifulSoup
import requests, sys

class downloader(object):
	
	def __init__(self):
		self.server = 'https://www.biqukan.com'
		self.target = 'https://www.biqukan.com/54_54610/'
		self.chapters = []
		self.urls = []
		self.num = 0
		
	def get_download_url(self):
		response = requests.get(url = self.target)
		html = response.text
		bf = BeautifulSoup(html)
		div = bf.find_all('div', class_ = 'listmain')
		a_bf = BeautifulSoup(str(div[0]))
		a = a_bf.find_all('a')
		self.num = len(a[12:])
		for each in a[12:]:
			self.chapters.append(each.string)
			self.urls.append(self.server + each.get('href'))
			
	def get_content(self, target):
		response = requests.get(url = target)
		html = response.text
		html.encoding('utf-8')
		bf = BeautifulSoup(html)
		content = bf.find_all('div', class_ = 'showtxt')
		content = content[0].text.replace('\xa0'*8, '\n\n')
		return content
		
	def write(self, chapter, path, content):
		write_flag = True
		with open(path, 'a', encoding='utf-8') as file:
			file.write(chapter + '\n')
			file.writelines(content)
			file.write('\n\n')
			
			
if __name__ == '__main__':
	dl = downloader()
	dl.get_download_url()
	print('《天下第九》开始下载：')
	for i in range(dl.num):
		dl.write(dl.chapters[i], '天下第九.txt', dl.get_content(dl.urls[i]))
		sys.stdout.write("	已下载：%.3f%%"%float(i / dl.num) + '\r')
		sys.stdout.flush()
	print('《天下第九》下载完成')