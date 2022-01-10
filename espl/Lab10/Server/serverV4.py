#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec  3 13:05:25 2021

@author: alex
"""
import socket
import threading
import queue
import sys
from sqlite3 import Error
import subprocess
import os

bufferSize = 1024

def execute_shell_command(command):
    return subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True).stdout.decode('utf-8')

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
    clients = {}
    # client:
    #         'shared'/'private'
    #         'path'
    #         'addr'

    while True:
        while not recvPackets.empty():
            data,addr = recvPackets.get()
            data = data.decode('utf-8')
            li = data.split()
            full_addr = addr[0]+':'+str(addr[1])
            if li[0] == 'mount':
                if host in data:
                    if len(li) != 3:
                        UDPServerSocket.sendto("Error Mounting, too many arguments".encode('utf-8'), addr)
                else:
                    UDPServerSocket.sendto("Error Mounting, wrong arguments".encode('utf-8'), addr)
                    continue
                obj = {
                    'Type': li[1],
                    'Path': os.path.realpath(sys.argv[0]).split('/Server')[0] + '/Server',
                    'Addr': addr
                    }
                clients[full_addr] = obj
            else:
                # when client send a message, first change to the client's directory
                os.chdir(clients[full_addr]['Path'])
                # handle commands
                if li[0] == 'cd':
                    msg = ''
                    # handle cd ip:port:/Server
                    if data == "cd :/Server":
                        os.chdir(os.path.realpath(sys.argv[0]).split('/Server')[0] + '/Server')
                        msg = 'cwd /Server'
                        reply(msg, addr, UDPServerSocket)
                    # handle other cd commands
                    else:
                        # change dir to the desired path
                        os.chdir(li[1])
                        output = 'cwd /Server' + os.getcwd().split('/Server')[1]
                        # send the output to the client
                        reply(output, addr, UDPServerSocket)
                        clients[full_addr]['Path'] = os.getcwd()
                        # if the client is shared, send the output to the other clients and update their paths
                        if clients[full_addr]['Type'] == 'shared':
                            msg = f'HANDLE Client {full_addr} {data} #{output}'
                            reply(msg, addr, UDPServerSocket, clients)
                            update_shared_path(clients, os.getcwd())
                elif li[0] == 'get':
                    # We allow to send file only to private clients
                    if clients[full_addr]['Type'] == 'private':
                        if os.path.isfile(li[1]):
                            f = open(li[1], 'rb')
                            l = f.read(bufferSize)
                            while (l):
                                UDPServerSocket.sendto(l, addr)
                                l = f.read(bufferSize)
                            f.close()
                else:
                    # for any other command, execute the command and handle the output
                    output = execute_shell_command(data)
                    if (output != None):
                        # send the output to the client
                        reply(output, addr, UDPServerSocket)
                        # if the client is shared, send the output to the other clients
                        if clients[full_addr]['Type'] == 'shared':
                            msg = f'HANDLE Client {full_addr} {data} #{output}'
                            reply(msg, addr, UDPServerSocket, clients)

    UDPServerSocket.close()
#Serevr Code Ends Here
def reply(data, addr, UDPServerSocket, clients=None):
    # if no clients are provided, send to specific client
    if (clients == None):
        send(data, addr, UDPServerSocket)
    # if clients are provided, send to all shared clients except the one that sent the message
    else:
        print("Client list: ", clients)
        print("Messaage: ", data)
        for client in clients.values():
            if client['Type'] == 'shared' and client['Addr'] != addr:
                send(data, client['Addr'], UDPServerSocket)

def update_shared_path(clients, path):
    for client in clients.values():
        if client['Type'] == 'shared':
            client['Path'] = path

def send(data,addr, UDPServerSocket):
    UDPServerSocket.sendto(data.encode('utf-8'), addr)

if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunServer(sys.argv[1])
    else:
        RunServer("127.0.0.1")

# cd 127.0.0.1:5000:/Server
