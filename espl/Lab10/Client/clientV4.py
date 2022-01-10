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
clientRoot = os.getcwd()


def execute_command(request, UDPClientSocket, isLocal, server):
    global clientRoot
    if isLocal == True:
        if (request.split()[0] == 'cd'):
            os.chdir(request.split()[1])
            clientRoot = os.getcwd()
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
    global clientRoot
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
            clientRoot = data.split()[1]
        else:
            print(data)
            if (shared == True):
                print(clientRoot, end='$ ', flush=True)


def handle_recieved_file(recvPackets, path):
    if os.path.exists(path):
        os.remove(path)
    with open(path, 'wb') as f:
        while (recvPackets.empty() == False):
            data, addr = recvPackets.get()
            f.write(data)
    f.close()


def RunClient(serverIP):
    global clientRoot
    host = socket.gethostbyname(socket.gethostname())
    port = random.randint(6000, 10000)
    print('Client IP->' + str(host) + ' Port->' + str(port))
    server = (str(serverIP), 5000)
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    UDPClientSocket.bind((host, port))
    recvPackets = queue.Queue()
    threading.Thread(target=RecvData, args=(UDPClientSocket, recvPackets)).start()

    print("Connected to the server")
    isLocal = True
    isMounted = False
    while True:
        print(clientRoot, end='$ ')
        request = input()
        splitted = request.split()
        if request == '':
            continue
        if splitted[0] == 'mount':
            if len(splitted) == 1:  # didn't send a server address
                print("Not enough arguments for mount, using default value")
                request = f"{request} private {serverIP}"
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

        elif splitted[0] == 'cd' and isMounted == True and (
                splitted[1] == ':/Server'):
            isLocal = False
            execute_command(request, UDPClientSocket, isLocal, server)
            data = recvPackets.get()[0]
            handle_recieved_msg(data)
        elif splitted[0] == 'cd' and (splitted[1].split(':')[0] == 'local'):
            if isMounted == True:
                isLocal = True
                parsed_command = splitted[0] + ' ' + splitted[1].split(':')[1]
                execute_command(parsed_command, UDPClientSocket, isLocal, server)
        elif request == 'qqq':
            break
        elif splitted[0] == 'get':
            if isMounted == True and isLocal == False:
                path = splitted[2]
                if (path == 'cwd'):
                    path = clientRoot
                path = path + '/' + splitted[1]
                execute_command(request, UDPClientSocket, isLocal, server)
                handle_recieved_file(recvPackets, path)

            else:
                print("You are not connected to the server")
        else:
            execute_command(request, UDPClientSocket, isLocal, server)
            if (isLocal == False):
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

