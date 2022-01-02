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
    temp = currPath.split('/')
    temp[len(temp) - 1] = "Server"
    serverRoot = '/'.join(temp)
    clientRoot = os.getcwd()


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
        elif request == f"cd :/Server":
            if not isMounted:
                print("You need to mount the server first!")
                continue
            print("Entered Server")
            UDPClientSocket.sendto(("enterServer").encode('utf-8'), server)
            inServer = True
        elif inServer:
            if len(splitted) > 1 and (splitted[0] == "cp" and 'cwd' in splitted[2]):
                indOfStart = splitted[1].find('/')
                targetLoc = splitted[1][indOfStart:] if indOfStart > 0 else ""
                target = f"{clientRoot}{targetLoc}"
                request = f"cp {splitted[1]} {target}"
            if len(splitted) > 1 and (':' in splitted[1]):
                request = f"cd {clientRoot}"
            UDPClientSocket.sendto(request.encode('utf-8'), server)
            if splitted[0] != "cd":
                result = recvPackets.get()[0].rstrip()
                if len(result) > 0:
                    print(result)
            else:
                newPath = recvPackets.get()[0].rstrip()
                inServer = verifyInServer(serverRoot, newPath)
        elif request == 'qqq':
            break
        else:
            if splitted[0] == 'cd':
                if verifyInServer(serverRoot, f"{currPath}/{splitted[1]}") and not inServer:
                    print("Unauthorized. Use cd :/Server to enter the server")
                    continue
                os.chdir(splitted[1])
            else:
                result, err = subprocess.Popen(splitted,
                                               stderr=subprocess.PIPE,
                                               stdout=subprocess.PIPE).communicate()
                print(result.decode('utf-8').rstrip())

        if inServer:
            UDPClientSocket.sendto(("cwd").encode('utf-8'), server)
            currPath = recvPackets.get()[0].rstrip()
        else:
            currPath = os.getcwd()

        if inServer and not verifyInServer(serverRoot, currPath):
            print("Left Server")
            inServer = False

    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

#mount private 127.0.0.1:6864:/Server

def verifyInServer(serverRoot: str, currPath: str):
    common = os.path.commonpath([serverRoot, currPath])
    return len(serverRoot) == len(common)



if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunClient(sys.argv[1])
    else:
        RunClient("127.0.0.1")


