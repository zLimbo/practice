# -*- coding: utf-8 -*-
"""
Created on Thu Mar 21 17:45:18 2019

@author: zLimbo
"""

class SafeSeqCls:
    
    def __init__(self, mySeq:list, myResourceUtilizationRate:float):
        self.__sequence = mySeq
        self.__resourceUtilizationRate = myResourceUtilizationRate
        
    def Cmp(self, myRhs):
        return self.__resourceUtilizationRate < myRhs.__resourceUtilizationRate
    
    def GetSafeSeq(self):
        return self.__sequence
    
    
_CustomerNum = 9
_ResourceNum = 5
_AllocationResource = []
_NeedResource = []
_AvailableResource = []
_SafeSeqSet = set()


def PrintData():
    print(_AllocationResource)
    print(_NeedResource)
    print(_AvailableResource)
    
    
    
    """
    for x in _AllocationResource:
        for y in x:
            print(y, end='')
        print()
    for x in _AllocationResource:
        for y in x:
            print(y, end='')
    print()
    """