# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 14:26:47 2019

@author: zLimbo
"""

class ShortInputException(Exception):
    """ 自定义异常类 """
    def __init__(self, length, atleast):
        Exception.__init__(self)
        self.length = length
        self.atleast = atleast
        
        
try:
    s = input('请输入--->')
    if len(s) < 3:
        raise ShortInputException(len(s), 3)
except EOFError:
    print('您输入了一个结束标记')
except ShortInputException as x:
    print('length=%d, atleast=%d'%(x.length, x.atleast))