#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  8 13:03:13 2021

@author: alex
"""
import queue
import socket
import subprocess
import threading
import sys
import random
import os
from pathlib import Path


bufferSize = 1024


#Original Code
def ReceiveData(sock):
    while True:
        try:
            data,addr = sock.recvfrom(bufferSize)
            print(data.decode('utf-8'))
        except:
            pass

#Client Code
def RecvData(sock,recvPackets):
    while True:
        data, addr = sock.recvfrom(bufferSize)
        recvPackets.put((data.decode('utf-8'),addr))

def RunClient(serverIP):
    host = socket.gethostbyname(socket.gethostname())
    port = random.randint(6000,10000)
    print('Client IP->'+str(host)+' Port->'+str(port))
    server = (str(serverIP),5000)
    UDPClientSocket = socket.socket(family=socket.AF_INET,type=socket.SOCK_DGRAM)
    UDPClientSocket.bind((host,port))
    name = 'make connection to the server'
    UDPClientSocket.sendto(name.encode('utf-8'),server)
    recvPackets = queue.Queue()
    threading.Thread(target=RecvData,args=(UDPClientSocket,recvPackets)).start()
    isMounted = False
    inServer = False
    currPath = os.getcwd()
    serverRoot = ''


    while True:
        print(currPath, end="$ ")
        request = input()
        splitted = request.split(' ')
        if splitted[0] == f"mount":
            if len(splitted) == 2:
                isMounted = True
                UDPClientSocket.sendto(request.encode('utf-8'), server)
                print(recvPackets.get()[0].rstrip())
            else:
                print("Not enough arguments for mount")
        elif request == "unmount":
            isMounted = False
            print("Unmounted from server")
        elif isMounted and request == f"cd :/Server":
            print("Entered Server")
            UDPClientSocket.sendto(("cwd").encode('utf-8'), server)
            serverRoot = Path(recvPackets.get()[0].rstrip())
            inServer = True
        elif inServer:
            UDPClientSocket.sendto(request.encode('utf-8'), server)
            if splitted[0] != "cd":
                print(recvPackets.get()[0].rstrip())
        elif request == 'qqq':
            break
        else:
            if splitted[0] == 'cd':
                os.chdir(splitted[1])
                print(os.getcwd())
            else:
                result, err = subprocess.Popen(splitted,
                                               stderr=subprocess.PIPE,
                                               stdout=subprocess.PIPE).communicate()
                print(result.decode('utf-8'))
        if inServer:
            UDPClientSocket.sendto(("cwd").encode('utf-8'), server)
            currPath = recvPackets.get()[0].rstrip()
        else:
            currPath = os.getcwd()

        if serverRoot.root != currPath and serverRoot not in Path(currPath).parents:
            inServer = False

    #UDPClientSocket.sendto(data.encode('utf-8'),server)
    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

#mount private 127.0.0.1:6864:/Server




if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunClient(sys.argv[1])
    else:
        RunClient("127.0.0.1")


