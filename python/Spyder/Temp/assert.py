# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 15:04:08 2019

@author: zLimbo
"""
while True:
    try:
        x = int(input('x='))
        if x == -1:
            break
        y = int(input('y='))
        assert x == y, 'x is not equal y'
    except AssertionError as reason:
        print("%s:%s"%(reason.__class__.__name__, reason))
        