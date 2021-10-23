import socket
import threading
import time

activeDegree = dict()
flag = 1

def main():
    global activeDegree
    global flag
    HOST=socket.gethostbyname(socket.gethostname())
    s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_IP)
    s.bind((HOST, 0))
    s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
    s.ioctl(socket.STO_RCVALL, socket.RCVALL_ON)
    while flag:
        c = s.recvfrom(65535)
        host = c[1][0]
        activeDegree[host] = activeDegree.get(host, 0)+1
        if c[1][0] != '192.168.25.1':
            print(c)
        s.ioctl(socket.SIO_RCVALL, socket.RCVALL_PFF)
        s.close()
  
print("active.py")
t = threading.Thread(target=main)
t.start()
time.sleep(60)
flag = 0
t.join()
for item in activeDegree.items():
    print(item)