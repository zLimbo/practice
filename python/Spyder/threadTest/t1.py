# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 15:12:58 2019

@author: zLimbo
"""

import threading
import time

def func1(x, y):
    for i in range(x, y):
        print(i, end=' ')
    time.sleep(100)
    
t1 = threading.Thread(target = func1, args = (0, 100))
t1.start()
t1.join(5)
t2 = threading.Thread(target = func1, args = (100, 200))
t2.start()
t2.join(5)
print('t1:', t1.is_alive())
print('t2:', t2.is_alive())
