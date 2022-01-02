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
    sharedUsers = []
    serverRoot = os.getcwd()

    while True:
        while not recvPackets.empty():
            data, fullAddr = recvPackets.get()
            addr = fullAddr[0]
            isMounted = True if mountedUsers.get(addr) is True else False
            data = data.decode('utf-8')
            splitted = data.split(' ')

            if splitted[0] == "mount":
                if host in data:
                    if len(splitted) == 3:
                        if splitted[1] == "shared":
                            sharedUsers.append(addr)
                        else:
                            UDPServerSocket.sendto("Error Mounting, too many arguments".encode('utf-8'), fullAddr)
                    elif len(splitted) == 1:
                        UDPServerSocket.sendto("Error Mounting, not enough arguments".encode('utf-8'), fullAddr)
                        continue
                else:
                    UDPServerSocket.sendto("Error Mounting, wrong arguments".encode('utf-8'), fullAddr)
                    continue

                mountedUsers[addr] = True
                print(f"{addr} mounted the server")
                UDPServerSocket.sendto("Mounting Completed".encode('utf-8'), fullAddr)

            elif isMounted and data == "unmount":
                mountedUsers[addr] = False
                if addr in sharedUsers:
                    sharedUsers.remove(addr)
                print(f"{addr} unmounted from server")

            elif data == "enterServer":
                os.chdir(serverRoot)

            elif isMounted:
                if splitted[0] == 'cwd':
                    loc = os.getcwd()
                    if addr in sharedUsers:
                        sendGroupMsg(UDPServerSocket, loc, sharedUsers, addr)
                    else:
                        UDPServerSocket.sendto(loc.encode('utf-8'), fullAddr)

                elif (splitted[0] == 'cd'):
                    currPath = os.getcwd()
                    os.chdir(currPath + f"/{splitted[1]}")
                    if addr in sharedUsers:
                        sendGroupMsg(UDPServerSocket, currPath, sharedUsers, addr)
                    else:
                        UDPServerSocket.sendto(currPath.encode('utf-8'), fullAddr)

                else:
                    try:
                        result, err = subprocess.Popen(splitted,
                                                  stderr=subprocess.PIPE,
                                                  stdout=subprocess.PIPE).communicate()
                    except:
                        error = f"SERVER ERROR!".encode('utf-8')
                        UDPServerSocket.sendto(error, fullAddr)
                        continue

                    if addr in sharedUsers:
                        sendGroupMsg(UDPServerSocket, result.decode('utf-8'), sharedUsers, addr)
                    else:
                        UDPServerSocket.sendto(result, fullAddr)

    UDPServerSocket.close()
#Server Code Ends Here

def sendGroupMsg(UDPSocket, msg, addrList, notSend):
    for addr in addrList:
        if addr is not notSend:
            UDPSocket.sendTo(msg.encode('utf-8'), addr)


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunServer(sys.argv[1])
    else:
        RunServer("127.0.0.1")

 