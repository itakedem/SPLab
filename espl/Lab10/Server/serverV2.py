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
    users = {}

    while True:
        while not recvPackets.empty():
            data,addr = recvPackets.get()
            data = data.decode('utf-8')
            li = list(data.split(" "))
            if (li[0] == f"mount {host}"):
                users[addr] = True
                print("Got mount command")
                UDPServerSocket.sendto("Mounting Completed".encode('utf-8'), addr)
                flag = True
            elif (users.get(users[addr[0]]) is not None and users.get(users[addr[0]]) == True):
                cleanData = data.split(' ')
                if (cleanData[0] == 'cd'):
                    os.chdir(cleanData[1])
                    UDPServerSocket.sendto(os.getcwd().encode('utf-8'), addr)
                else:
                    result, err = subprocess.Popen(cleanData,
                                              stderr=subprocess.PIPE,
                                              stdout=subprocess.PIPE).communicate()
                    UDPServerSocket.sendto(result, addr)
            if li[0] == "unmount":
                users[addr] = False
                print(f"{addr} unmounted from server")
    UDPServerSocket.close()
    data_base.close()
#Serevr Code Ends Here

def create_connection(db_file):
    """ create a database connection to the SQLite database
        specified by db_file
    :param db_file: database file
    :return: Connection object or None
    """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        return conn
    except Error as e:
        print(e)

    return conn


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunServer(sys.argv[1])
    else:
        RunServer("127.0.0.1")

 