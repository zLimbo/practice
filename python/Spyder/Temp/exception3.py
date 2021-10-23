# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 14:35:02 2019

@author: zLimbo
"""

aList = [1, 2, 3]

while True:
    try:
        n = int(input('n='))
        print(aList[n])
    except ValueError:
        print('please input integer')
    except IndexError:
        print('Index out of array')
    else:
        break
        