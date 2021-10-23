# -*- coding: utf-8 -*-
"""
Created on Tue Apr 30 14:31:23 2019

@author: zLimbo
"""

import socket
words = {'how are you?':'Fine, thank you.'}
HOST = ''
PORT = 50007
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen()
print('Listening at port:', PORT)
conn, addr = s.accept()
print('Connected by', addr)
while True:
    data = conn.recv(1024)
    data = data.decode()
    if not data:
        break
    print('Received message:', data)
    conn.sendall(words.get(data, 'Nothing').encode())
conn.close()
s.close()