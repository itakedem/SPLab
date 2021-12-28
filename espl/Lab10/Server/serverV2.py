#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  3 13:05:25 2021

@author: alex
"""
import os
import socket
import threading
import queue
import sys
import sqlite3
from sqlite3 import Error
import subprocess

bufferSize = 1024



#Server Code
def RecvData(sock,recvPackets):
    while True:
        data,addr = sock.recvfrom(bufferSize)
        recvPackets.put((data,addr))

def RunServer(host):
    port = 5000
    print('Server hosting on IP-> '+str(host))
    UDPServerSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)
    UDPServerSocket.bind((host,port))
    recvPackets = queue.Queue()

    print('Server Running...')

    threading.Thread(target=RecvData,args=(UDPServerSocket,recvPackets)).start()
    mountedUsers = {}

    while True:
        while not recvPackets.empty():
            data, fullAddr = recvPackets.get()
            addr = fullAddr[0]
            isMounted = True if mountedUsers.get(addr) is True else False
            data = data.decode('utf-8')
            if (data == f"mount {host}"):
                mountedUsers[addr] = True
                print(f"{addr} mounted the server")
                UDPServerSocket.sendto("Mounting Completed".encode('utf-8'), fullAddr)
            elif isMounted and data == "unmount":
                mountedUsers[addr] = False
                print(f"{addr} unmounted from server")
            elif isMounted:
                splitted = data.split(' ')
                if splitted[0] == 'cwd':
                    loc = os.getcwd()
                    UDPServerSocket.sendto(loc.encode('utf-8'), fullAddr)
                elif (splitted[0] == 'cd'):
                    os.chdir(os.getcwd() + f"/{splitted[1]}")
                else:
                    result, err = subprocess.Popen(splitted,
                                              stderr=subprocess.PIPE,
                                              stdout=subprocess.PIPE).communicate()
                    UDPServerSocket.sendto(result, fullAddr)

    UDPServerSocket.close()
#Serevr Code Ends Here


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunServer(sys.argv[1])
    else:
        RunServer("127.0.0.1")

 