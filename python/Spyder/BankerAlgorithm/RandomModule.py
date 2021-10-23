# -*- coding: utf-8 -*-
"""
Created on Thu Mar 21 18:06:52 2019

@author: zLimbo
"""

import random
import DataModule

def Initial():
    for i in range(0, DataModule._CustomerNum):
        myAvailableResource = []
        myNeedResource = []
        for j in range(0, DataModule._ResourceNum):
            myAvailableResource.append(random.randint(0, 10))
            myNeedResource.append(random.randint(0, 10))
        DataModule._AllocationResource.append(myAvailableResource)
        DataModule._NeedResource.append(myNeedResource)
        
    for i in range(0, DataModule._ResourceNum):
        DataModule._AvailableResource.append(random.randint(0, 10))
        