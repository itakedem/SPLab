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
    print("What is your request:")
    while True:        
        request = input()
        if request == f"mount":
            data = '6'
            isMounted = True
            UDPClientSocket.sendto(data.encode('utf-8'), server)
            print(recvPackets.get()[0].rstrip())
        elif isMounted and request == f"cd {host}:{port}:/Server":
            print("Entered Server")
            inServer = True
        elif inServer:
            UDPClientSocket.sendto(request.encode('utf-8'), server)
            print(recvPackets.get()[0].rstrip())
        elif request == 'qqq':
            break
        else:
            cleanData = request.split(' ')
            if (cleanData[0] == 'cd'):
                os.chdir(cleanData[1])
                print(os.getcwd())
            else:
                result, err = subprocess.Popen(cleanData,
                                               stderr=subprocess.PIPE,
                                               stdout=subprocess.PIPE).communicate()
                print(result.decode('utf-8'))
    #UDPClientSocket.sendto(data.encode('utf-8'),server)
    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

#mount private 127.0.0.1:6864:/Server




if __name__ == '__main__':

    RunClient(sys.argv[1])


    #cd 127.0.1.1:9774:/Server