# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 14:27:19 2019

@author: zLimbo
"""


try:
    raise Exception('spam', 'eggs')
except Exception as inst:
    print(type(inst))
    print(inst.args)
    print(inst)
    x, y = inst.args
    print('x=', x)
    print('y=', y)