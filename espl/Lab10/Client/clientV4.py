#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  8 13:03:13 2021

@author: alex
"""
import socket
import threading
import sys
import random
import os
import subprocess
import queue

bufferSize = 1024
cwd = os.getcwd()


def execute_command(request, UDPClientSocket, is_local, server):
    global cwd
    if is_local == True:
        if (request.split()[0] == 'cd'):
            os.chdir(request.split()[1])
            cwd = os.getcwd()
        else:
            output = subprocess.run(request, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                    shell=True).stdout.decode('utf-8')
            if (output != None):
                print(output)
    else:
        UDPClientSocket.sendto(request.encode('utf-8'), server)


# Client Code
def RecvData(sock, recvPackets):
    while True:
        data, addr = sock.recvfrom(bufferSize)
        recvPackets.put((data, addr))
        if data.decode() != '' and data.decode() != None and (data.decode().split()[0] == 'HANDLE'):
            data = recvPackets.get()[0]
            handle_recieved_msg(data)


def handle_recieved_msg(data):
    global cwd
    data = data.decode('utf-8')
    shared = False
    if (data != None and data != ''):
        # if the output is from other client, first print the client details and and the client's command
        if (data.split()[0] == 'HANDLE'):
            shared = True
            command = data.split('#')[0].split()
            data = data.split('#')[1]  # change the data to the server's output to match the regular output protocol
            print(f"({command[1]} {command[2]}):{' '.join(command[3:])}")
            if data == None or data == '':
                return

        if (data.split()[0] == 'cwd'):
            cwd = data.split()[1]
        else:
            print(data)
            if (shared == True):
                print(cwd, end='$ ', flush=True)


def handle_recieved_file(recvPackets, path):
    if os.path.exists(path):
        os.remove(path)
    with open(path, 'wb') as f:
        while (recvPackets.empty() == False):
            data, addr = recvPackets.get()
            f.write(data)
    f.close()


def RunClient(serverIP):
    global cwd
    host = socket.gethostbyname(socket.gethostname())
    port = random.randint(6000, 10000)
    print('Client IP->' + str(host) + ' Port->' + str(port))
    server = (str(serverIP), 5000)
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    UDPClientSocket.bind((host, port))
    recvPackets = queue.Queue()
    threading.Thread(target=RecvData, args=(UDPClientSocket, recvPackets)).start()

    print("Connected to the server")
    is_local = True
    is_mounted = False
    while True:
        print(cwd, end='$ ')
        request = input()
        splitted_request = request.split()
        if request == '':
            continue
        if splitted_request[0] == 'mount':
            is_mounted = True
            if splitted_request[1] == 'shared':
                request = f'mount shared {serverIP}:{port}:/Server'
            else:
                request = f'mount private {serverIP}:{port}:/Server'
            UDPClientSocket.sendto(request.encode('utf-8'), server)
        elif request == 'cd :/Server' and is_mounted == True:
            is_local = False
            request = "cd 127.0.0.1:5000:/Server"
            execute_command(request, UDPClientSocket, is_local, server)
            data = recvPackets.get()[0]
            handle_recieved_msg(data)
        elif splitted_request[0] == 'cd' and (splitted_request[1].split(':')[0] == 'local'):
            if is_mounted == True:
                is_local = True
                parsed_command = splitted_request[0] + ' ' + splitted_request[1].split(':')[1]
                execute_command(parsed_command, UDPClientSocket, is_local, server)
        elif request == 'qqq':
            break
        elif splitted_request[0] == 'get':
            if is_mounted == True and is_local == False:
                path = splitted_request[2]
                if (path == 'cwd'):
                    path = cwd
                path = path + '/' + splitted_request[1]
                execute_command(request, UDPClientSocket, is_local, server)
                handle_recieved_file(recvPackets, path)

            else:
                print("You are not connected to the server")
        else:
            execute_command(request, UDPClientSocket, is_local, server)
            if (is_local == False):
                data = recvPackets.get()[0]
                handle_recieved_msg(data)

    UDPClientSocket.close()
    os._exit(1)


# Client Code Ends Here


if __name__ == '__main__':
    if len(sys.argv) == 2:
        RunClient(sys.argv[1])
    else:
        RunClient("127.0.0.1")

