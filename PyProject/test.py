#coding=utf8
#!/usr/bin/python
from __future__ import print_function 
import struct,socket
import hashlib
import threading,random
import time
from base64 import b64encode, b64decode

connectionlist = {}

def parse_data(msg):
    code_length = ord(msg[1]) & 127

    if code_length == 126:
        masks = msg[4:8]
        data = msg[8:]
    elif code_length == 127:
        masks = msg[10:14]
        data = msg[14:]
    else:
        masks = msg[2:6]
        data = msg[6:]

    i = 0
    raw_str = ''

    for d in data:
        raw_str += chr(ord(d) ^ ord(masks[i%4]))
        i += 1
    return raw_str  
        
def sendMessage(message):
    global connectionlist
    for connection in connectionlist.values():
        back_str = []
        back_str.append('\x81')
        data_length = len(message)
    
        if data_length <= 125:
            back_str.append(chr(data_length))
        else:
            back_str.append(chr(126))
            back_str.append(chr(data_length >> 8))
            back_str.append(chr(data_length & 0xFF))
    
        back_str = "".join(back_str) + message        
        #connection.send(str.encode(str(u"\x00%s\xFF\n\n" % message))) #这个是旧版
        print (u'send message:' +  message)
        connection.send(back_str)
 
def deleteconnection(item):
    global connectionlist
    del connectionlist['connection'+item]

class WebSocket(threading.Thread):#继承Thread

    GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

    def __init__(self,conn,index,name,remote, path="/"):
        threading.Thread.__init__(self)#初始化父类Thread
        self.conn = conn
        self.index = index
        self.name = name
        self.remote = remote
        self.path = path
        self.buffer = ""     
    def run(self):#重载Thread的run
        print('Socket%s Start!' % self.index)
        headers = {}
        self.handshaken = False

        while True:
            if self.handshaken == False:
                print ('Socket%s Start Handshaken with %s!' % (self.index,self.remote))
                self.buffer += bytes.decode(self.conn.recv(1024))

                if self.buffer.find('\r\n\r\n') != -1:
                    header, data = self.buffer.split('\r\n\r\n', 1)
                    for line in header.split("\r\n")[1:]:
                        key, value = line.split(": ", 1)
                        headers[key] = value

                    headers["Location"] = ("ws://%s%s" %(headers["Host"], self.path))
                    key = headers['Sec-WebSocket-Key']
                    token = b64encode(hashlib.sha1(str.encode(str(key + self.GUID))).digest())

                    handshake="HTTP/1.1 101 Switching Protocols\r\n"\
                        "Upgrade: websocket\r\n"\
                        "Connection: Upgrade\r\n"\
                        "Sec-WebSocket-Accept: "+bytes.decode(token)+"\r\n"\
                        "WebSocket-Origin: "+str(headers["Origin"])+"\r\n"\
                        "WebSocket-Location: "+str(headers["Location"])+"\r\n\r\n"

                    self.conn.send(str.encode(str(handshake)))
                    self.handshaken = True  
                    print ('Socket %s Handshaken with %s success!' %(self.index, self.remote))  
                    sendMessage('Welcome, ' + self.name + ' !')  

            else:
                mm=self.conn.recv(64)
                '''self.buffer +=mm.decode('utf-8', 'ignore')
                #旧版
                if self.buffer.find(u"\xFF")!=-1:
                    s = self.buffer.split("\xFF")[0][1:]
                    if s=='quit':
                        print ('Socket%s Logout!' % (self.index))
                        sendMessage(self.name+' Logout')
                        deleteconnection(str(self.index))
                        self.conn.close()
                        break
                    else:
                        print ('Socket%s Got msg:%s from %s!' % (self.index,s,self.remote))
                        sendMessage(self.name+':'+s)'''
                msg_utf8 = parse_data(mm) #utf8
                msg_unicode = msg_utf8.decode('utf-8', 'ignore') #unicode
                if msg_unicode=='quit':
                    print ('Socket%s Logout!' % (self.index))
                    nowTime = time.strftime('%H:%M:%S',time.localtime(time.time()))
                    sendMessage('%s %s say: %s' % (nowTime, self.remote, self.name+' Logout'))                      
                    deleteconnection(str(self.index))
                    self.conn.close()
                    break #退出线程
                else:
                    print ('Socket%s Got msg:%s from %s!' % (self.index, msg_unicode, self.remote))
                    nowTime = time.strftime('%H:%M:%S',time.localtime(time.time()))
                    sendMessage('%s %s say: %s' % (nowTime, self.remote, msg_utf8))       
                
            self.buffer = ""


class WebSocketServer(object):
    def __init__(self):
        self.socket = None
    def begin(self):
        print( 'WebSocketServer Start!')
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.bind(("127.0.0.1",12345))
        self.socket.listen(50)

        global connectionlist

        i=0
        while True:
            connection, address = self.socket.accept()

            username=address[0]     
            newSocket = WebSocket(connection,i,username,address)
            newSocket.start() #开始线程,执行run函数
            connectionlist['connection'+str(i)]=connection
            i = i + 1

if __name__ == "__main__":
    server = WebSocketServer()
    server.begin()