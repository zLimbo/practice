# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 14:42:00 2019

@author: zLimbo
"""

try:
    x = int(input('x='))
    y = int(input('y='))
    z = x / y
except ZeroDivisionError:
    print('y can\'t be 0')
except ValueError:
    print('please input number')
except NameError:
    print('no exist')
else:
    print('x/y = ', z)
finally:
    print('go go go')