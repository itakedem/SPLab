#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  8 13:03:13 2021

@author: alex
"""
import queue
import socket
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
    flag = False
    print("What is your request:")
    while True:        
        request = input()
        if request == 'new user':
            print("Enter: user name, user id ")
            user_name, user_id = input().split()
            data = '0' + ' ' + user_name + ' ' + user_id
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get()[0])
        elif request == 'remove user':
            print("Enter: User name ")
            user_name = input()
            data = '1' + ' ' + user_name
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get())
        elif request == 'connect to group':
            print("Enter: group name ")
            group_name = input()
            data = '2' + ' ' + group_name
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get()[0])
        elif request == 'disconnect from group':
            print("Enter: Group name ")
            group_name = input()
            data = '3' + ' ' + group_name
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get()[0])
        elif request == 'send message to user':
            print("Enter: user name to whom to sent the messege")
            user_name_toSend = input()  
            print("Enter: The messege")
            mssg = input()
            data = '4' + ' ' + user_name_toSend + ' ' + mssg
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get()[0])
        elif request == 'send message to group':
            print("Enter: group name to sent the messege")
            group_name_toSend = input()  
            print("Enter: The messege")
            mssg = input()
            data = '5' + ' ' + group_name_toSend + ' ' + mssg
            UDPClientSocket.sendto(data.encode('utf-8'),server)
            print(recvPackets.get()[0])
        #data = input()
        elif request == f"mount":
            data = '6'
            flag = True
            UDPClientSocket.sendto(data.encode('utf-8'), server)
            print(recvPackets.get()[0].rstrip())
        elif flag == True:
            UDPClientSocket.sendto(request.encode('utf-8'), server)
            print(recvPackets.get()[0].rstrip())
        elif request == 'qqq':
            break
    #UDPClientSocket.sendto(data.encode('utf-8'),server)
    UDPClientSocket.close()
    os._exit(1)
#Client Code Ends Here

#mount private 127.0.0.1:6864:/Server




if __name__ == '__main__':

    RunClient(sys.argv[1])
    