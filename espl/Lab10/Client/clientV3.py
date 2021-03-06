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
import time
from pathlib import Path

bufferSize = 1024
currInput = None


# Original Code
def ReceiveData(sock):
    while True:
        data, addr = sock.recvfrom(bufferSize)
        print(data.decode('utf-8'))


# Client Code
def RecvData(sock, recvPackets):
    while True:
        data, addr = sock.recvfrom(bufferSize)
        try:
            dataToQ = data.decode('utf-8')
        except:
            dataToQ = data

        recvPackets.put((dataToQ, addr))


def waitForInput():
    global currInput
    while True:
        currInput = input()


def RunClient(serverIP):
    global currInput
    host = socket.gethostbyname(socket.gethostname())
    port = random.randint(6000, 10000)
    print('Client IP->' + str(host) + ' Port->' + str(port))
    server = (str(serverIP), 5000)
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    UDPClientSocket.bind((host, port))
    name = 'make connection to the server'
    UDPClientSocket.sendto(name.encode('utf-8'), server)
    recvPackets = queue.Queue()
    threading.Thread(target=RecvData, args=(UDPClientSocket, recvPackets)).start()
    # threading.Thread(target=waitForInput).start()
    isMounted = False
    inServer = False
    currInput = "Started"
    currPath = os.getcwd()
    temp = currPath.split('/')
    temp[len(temp) - 1] = "Server"
    serverRoot = '/'.join(temp)
    clientRoot = os.getcwd()

    while True:
        print(f"{currPath}$", end=" ")
        while currInput is None:
            if not recvPackets.empty():
                ans = recvPackets.get()[0].rstrip()
                if "no" in ans:
                    curr = recvPackets.get()[0].rstrip()
                    continue
                else:
                    print(ans)
                    if "cd" in ans:
                        currPath = recvPackets.get()[0].rstrip()

                break
        request = input()
        currInput = request
        splitted = request.split(' ')

        if splitted[0] == f"mount":
            if len(splitted) == 1:  # didn't send a server address
                print("Not enough arguments for mount, using default value")
                request = f"{request} {serverIP}"
            elif len(splitted) == 2:
                if splitted[1] == "shared":
                    print("Not enough arguments for mount, using default value")
                    request = f"{request} {serverIP}"
                elif splitted[1] != serverIP:
                    print(f"the server in {splitted[1]} is not connected. Please try again")
                    continue
            elif splitted[2] != serverIP:  # sent unknown server address
                print(f"the server in {splitted[1]} is not connected. Please try again")
                continue
            isMounted = True
            UDPClientSocket.sendto(request.encode('utf-8'), server)
            print(recvPackets.get()[0].rstrip())

        elif request == "unmount":
            isMounted = False
            print("Unmounted from server")

        elif request == f"cd :/Server":
            if not isMounted:
                print("You need to mount the server first!")
                continue  # can't enter server without mount
            print("Entered Server")
            UDPClientSocket.sendto(("cd :/Server").encode('utf-8'), server)
            inServer = True

        elif inServer:
            if splitted[0] == "get" and len(splitted) < 3:
                print("Not enough arguments!")
                continue

            if len(splitted) > 1 and (
                    splitted[0] == "get" and 'cwd' in splitted[2]):  # coping file from server to client
                request = f"get {splitted[1]} {clientRoot}"

            elif request == "cd :/local":  # leaving server
                print("Left Server")
                os.chdir(clientRoot)
                currPath = clientRoot
                inServer = False
                continue

            UDPClientSocket.sendto(request.encode('utf-8'), server)  # sending the command to the server


            if splitted[0] == "get":
                path = f"{clientRoot}/{splitted[1]}"
                handleFiles(recvPackets, path)
                continue
            elif splitted[0] != "cd":
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
                try:
                    os.chdir(splitted[1])
                except:
                    print("Wrong Path!")
                    continue
            else:
                result = ""
                try:
                    result, err = subprocess.Popen(splitted,
                                                   stderr=subprocess.PIPE,
                                                   stdout=subprocess.PIPE).communicate()
                except:
                    print(f"ERROR!")
                    continue

                print(result.decode('utf-8').rstrip())

        if inServer:  # setting the current directory from server or client, depends on "inServer"
            UDPClientSocket.sendto(("cwd").encode('utf-8'), server)
            currPath = recvPackets.get()[0].rstrip()
        else:
            currPath = os.getcwd()

        if inServer and not verifyInServer(serverRoot, currPath):  # exiting server
            print("Left Server")
            os.chdir(clientRoot)
            currPath = clientRoot
            inServer = False

    UDPClientSocket.close()
    os._exit(1)


# Client Code Ends Here

# mount private 127.0.0.1:6864:/Server

def verifyInServer(serverRoot: str, currPath: str):
    try:
        common = os.path.commonpath([serverRoot, currPath])
    except:
        return False
    return len(serverRoot) == len(common)


def handleFiles(packets, path):
    time.sleep(1)
    if os.path.exists(path):
        os.remove(path)
    with open(path, 'wb') as f:
        while (not packets.empty()):
            data, addr = packets.get()
            if isinstance(data, str):
                f.write(data.encode('utf-8'))
            else:
                f.write(data)
    f.close()
    print("Copied the file!")


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunClient(sys.argv[1])
    else:
        RunClient("127.0.0.1")