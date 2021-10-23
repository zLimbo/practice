# -*- coding: utf-8 -*-
"""
Created on Tue May  7 09:00:39 2019

@author: zLimbo
"""
a = 3
b = 4
try:
    assert a==b, 'a must be equal to b'
except AssertionError as reason:
    print('%s:%s'%(reason.__class__.__name__, reason))
