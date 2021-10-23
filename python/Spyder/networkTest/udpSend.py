# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 14:18:31 2019

@author: zLimbo
"""

import socket
import sys
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(("", 5001))
s.sendto(sys.argv[1].encode(), ("192.168.25.1", 5000))
s.close()