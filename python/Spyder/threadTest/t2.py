# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 15:32:29 2019

@author: zLimbo
"""

import threading
import time

class MyThread(threading.Thread):
    def __init__(self, num, threadname):
        threading.Thread.__init__(self, name = threadname)
        self.num = num
        # self.daemon = True
    
    def run(self):
        time.sleep(self.num)
        print(self.num)
        
t1 = MyThread(1, 't1')
t2 = MyThread(2, 't2')
t2.daemon = True
print(t1.daemon)
print(t2.daemon)
t1.start()
t2.start()