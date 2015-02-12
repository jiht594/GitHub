#!/usr/bin/python
# -*- coding:utf-8 -*-

import string
import cookielib                   
import urllib,urllib2            
import time            
from os.path import basename
from os.path import abspath
from urlparse import urlsplit
import os
import sys

def addHeader1(req) :
    #去网站手动下载,看一看header内容
    #req.add_header('(Request-Line)','GET /PC/FoxitReader510.1117_chs_Setup.rar HTTP/1.1')
    #req.add_header('Host','001.bzfxw.com')
    req.add_header('User-Agent','Mozilla/5.0 (Windows NT 6.1; WOW64; rv:35.0) Gecko/20100101 Firefox/35.0')
    #req.add_header('Accept','text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')
    #req.add_header('Accept-Language','en-US,en;q=0.5')
    #req.add_header('Accept-Encoding','gzip, deflate')
    #req.add_header('Referer','http://www.bzfxw.com/soft/softdown.asp?softid=213780')
    #req.add_header('Cookie','Hm_lvt_d29ea3608fa94f6a5fce296f72d8d61c=1422361272,1422363240; Hm_lpvt_d29ea3608fa94f6a5fce296f72d8d61c=1422367322')
    req.add_header('Connection','keep-alive')
    return req
def printSpeed(startTime, now, dataWrited):
    timeDiff = int(now - startTime)
    second = timeDiff % 60
    minute = timeDiff / 60
    hour = minute / 60
    strTime = "TIME: {0:02d}:{1:02d}:{2:02d} "
    strTime = strTime.format(hour, minute, second)
    if timeDiff == 0 :
        print "%s, speed : "%(strTime)
    else :
        print "%s, speed : %.2fKb"%(strTime, dataWrited/(timeDiff))

def fixFileName(name):
    name = unicode(urllib.unquote(string.replace(urllib.quote(name),"%00", "")), 'utf8')  
    if name[0] == '"'or name[0] == "'":
        name = name[1:-1]
    '''localName = urllib.quote(localName)#第二个参数是不转换的,默认是/,还有-_.永远不转换
    localName = string.replace(localName,"%00", "")
    localName = urllib.unquote(localName)
    localName = unicode(localName, 'utf8')'''
    return name

def url2name(url):
    return basename(urlsplit(url)[2])

def fileDownload(url, localFileName = None):
    #print connecting......"
    cj = cookielib.CookieJar();
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cj));
    urllib2.install_opener(opener);
    localName = url2name(url)
    req = urllib2.Request(url)
    #部分网站下载需要header
    req = addHeader1(req)
    result = urllib2.urlopen(req)

    # If the response has Content-Disposition, we take file name from it
    if result.info().has_key('Content-Disposition'):
        localName = result.info()['Content-Disposition'].split('filename=')[1]

    # if we were redirected, the real file name we take from the final URL   
    if result.url != url:
        localName = url2name(result.url)


    localName = fixFileName(localName)

    # we can force to save the file as specified name
    if localFileName:
        localName = localFileName

    #begin to write file
    f = open(localName, 'wb')
    print abspath(f.name)
    print "begin download to %s"%localName
    meta = result.info()  
    file_size = int(meta.getheaders("Content-Length")[0])  
    print file_size
    file_size_dl = 0  
    block_sz = 1024*100# 每次读多少数据
    count = 0;#计循环数
    dataTemp = ''#数据缓存
    writeTime = 10#循环多少次写一次文件
    times = 0#写文件的次数
    dataOneWrite = writeTime*block_sz#每次写多少数据
    curSize = 0

    startTime = time.time()
    while True:  
        #read file from server
        bufferData = result.read(block_sz)
        #print bufferData
        if not bufferData:  
            f.write(dataTemp)
            print "no buffer left"
            lastBuffer = len(dataTemp)
            curSize = curSize + lastBuffer
            #file_size = file_size + lastBuffer
            dataTemp = ''
            dataWrited = curSize/1024
            print 'times=%d, writed data=%dKb, total=%dKb, %.2f%%' \
                  %(times, dataWrited, file_size/1024, curSize*100.0/file_size)
            printSpeed(startTime, time.time(), dataWrited)
            break 
        else :
            count = count + 1
            dataTemp = dataTemp + bufferData
            file_size_dl += len(bufferData)
            #print "count=%d, totle received size=%d"%(count, file_size_dl)
        if count == writeTime :
            times = times + 1
            print "----------------------------"
            curSize = times*dataOneWrite
            dataWrited = curSize/1024
            print 'times=%d, writed data=%dKb, total=%dKb, %.2f%%' \
                  %(times, dataWrited, file_size/1024, curSize*100.0/file_size)
            printSpeed(startTime, time.time(), dataWrited)
            print "----------------------------"
            count = 0
            f.write(dataTemp)
            dataTemp = ''
    f.close()  
    print "already downloaded"
print sys.argv    
URL = "http://dldir1.qq.com/qqfile/qq/QQ6.9/13786/QQ6.9.exe";
#URL = "http://lx.cdn.baidupcs.com/file/e5dd2e6f05d515f2e0bdf9c7b32d1d85?bkt=p-d024f0c0e13736c29419d7e55fda4cb9&xcode=c92ccfdc506de886c6adf369c841b0b6cc1cffb612725a05f77424e07ee197d9&fid=3590698332-250528-471883186423475&time=1422447892&sign=FDTAXERLBH-DCb740ccc5511e5e8fedcff06b081203-7+HCKtqFi67oKI61K2yOaC4h0Sc=&to=sc&fm=Nan,B,T,t&sta_dx=2&sta_cs=67&sta_ft=zip&sta_ct=7&newver=1&newfm=1&flow_ver=3&sl=81723468&expires=8h&rt=pr&r=603555788&mlogid=2888139314&vuk=3590698332&vbdid=4227705178&fin=[CSI犯罪现场调查]1－11季13集.zip&fn=[CSI犯罪现场调查]1－11季13集.zip&spe=1"
fileDownload(URL, localFileName=None);
