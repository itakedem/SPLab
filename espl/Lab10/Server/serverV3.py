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


# Server Code
def RecvData(sock, recvPackets):
    while True:
        data, addr = sock.recvfrom(bufferSize)
        recvPackets.put((data, addr))


def RunServer(host):
    port = 5000
    print('Server hosting on IP-> ' + str(host))
    UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    UDPServerSocket.bind((host, port))
    recvPackets = queue.Queue()

    print('Server Running...')

    threading.Thread(target=RecvData, args=(UDPServerSocket, recvPackets)).start()
    mountedUsers = {}
    sharedUsers = []
    sharedUsersInServer = []
    serverRoot = os.getcwd()
    lastCommand = None

    while True:
        while not recvPackets.empty():
            data, fullAddr = recvPackets.get()
            addr = fullAddr[1]
            isMounted = True if mountedUsers.get(addr) is True else False
            data = data.decode('utf-8')
            lastCommand = data if data != "cwd" else lastCommand
            splitted = data.split(' ')

            if splitted[0] == "mount":
                if host in data:
                    if len(splitted) == 3:
                        if splitted[1] == "shared":
                            sharedUsers.append(fullAddr)
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
                if fullAddr in sharedUsers:
                    sharedUsers.remove(fullAddr)
                print(f"{addr} unmounted from server")

            elif data == "cd :/Server":
                os.chdir(serverRoot)
                if fullAddr in sharedUsers: 
                    sharedUsersInServer.append(fullAddr)
            elif isMounted:
                if splitted[0] == 'cwd':
                    loc = os.getcwd()
                    if fullAddr in sharedUsers:
                        sendGroupMsg(UDPServerSocket,'no', loc, sharedUsersInServer, fullAddr)
                    else:
                        UDPServerSocket.sendto(loc.encode('utf-8'), fullAddr)

                elif (splitted[0] == 'cd'):
                    currPath = os.getcwd()
                    try:
                        os.chdir(currPath + f"/{splitted[1]}")
                    except:
                        print(f"{fullAddr} entered wrong path.")
                    if fullAddr in sharedUsers:
                        sendGroupMsg(UDPServerSocket, data,  os.getcwd(), sharedUsersInServer, fullAddr)
                    else:
                        UDPServerSocket.sendto(currPath.encode('utf-8'), fullAddr)
                elif splitted[0] == 'get':
                    if os.path.isfile(splitted[1]):
                        file = open(splitted[1], 'rb')
                        readBuffer = file.read(bufferSize)
                        while (readBuffer):
                            UDPServerSocket.sendto(readBuffer, fullAddr)
                            readBuffer = file.read(bufferSize)
                        file.close()
                        continue
                else:
                    try:
                        result, err = subprocess.Popen(splitted,
                                                       stderr=subprocess.PIPE,
                                                       stdout=subprocess.PIPE).communicate()
                    except:
                        error = f"SERVER ERROR!".encode('utf-8')
                        UDPServerSocket.sendto(error, fullAddr)
                        continue

                    if fullAddr in sharedUsers:
                        sendGroupMsg(UDPServerSocket,data, result.decode('utf-8'), sharedUsersInServer, fullAddr)
                    else:
                        UDPServerSocket.sendto(result, fullAddr)

    UDPServerSocket.close()


# Server Code Ends Here

def sendGroupMsg(UDPSocket,command, msg, addrList, notSend):
    command = f"({notSend[1]}) {command}"
    for addr in addrList:
        if addr[1] != notSend[1]:
            UDPSocket.sendto(command.encode('utf-8'), addr)
        UDPSocket.sendto(msg.encode('utf-8'), addr)


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunServer(sys.argv[1])
    else:
        RunServer("127.0.0.1")

