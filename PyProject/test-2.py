# coding=utf8
# -*- coding: utf-8 -*-
"""
Created on Thu Mar 05 15:19:16 2015

@author: Administrator
"""

import os
import urllib2
import urllib
import cookielib
#request = urllib2.Request('http://www.cqumzh.cn/bbs/logging.php?action=login')
#request.add_header('User-Agent', 'fake-client')
#response = urllib2.urlopen(request)
#print response.read()
cookiejar=cookielib.CookieJar()
urlopener=urllib2.build_opener(urllib2.HTTPCookieProcessor(cookiejar))
urllib2.install_opener(urlopener)
urlopener.addheaders.append(('Accept', 'application/xhtml+xml, */*'))
urlopener.addheaders.append(('Accept-Language', 'zh-CN'))
urlopener.addheaders.append(('Host', 'www.cqumzh.cn'))
urlopener.addheaders.append(('User-Agent', 'Mozilla/5.0 (compatible; MISE 9.0; Windows NT 6.1); Trident/5.0'))
urlopener.addheaders.append(('Connection', 'Keep-Alive'))


req = urllib2.Request('http://www.cqumzh.cn/bbs/logging.php?action=login')
print urllib2.urlopen(req).read()

AbK_sid = ""
for cookie in cookiejar :
    if cookie.name == "AbK_sid":
        AbK_sid = cookie.value
    print AbK_sid
'''urlopener.addheaders.append(('Referer', 'http://www.cqumzh.cn/bbs/logging.php?action=login'))
urlopener.addheaders.append(('Accept-Language', 'zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3'))
urlopener.addheaders.append(('Host', 'www.cqumzh.cn'))
urlopener.addheaders.append(('User-Agent', 'Mozilla/5.0 (compatible; MISE 9.0; Windows NT 6.1); Trident/5.0'))
urlopener.addheaders.append(('Connection', 'Keep-Alive'))
urlopener.addheaders.append(('Accept', 'image/png,image/*;q=0.8,*/*;q=0.5'))'''
print 'XXX Login......'
imgurl=r'http://www.cqumzh.cn/bbs/seccode.php'
req2 = urllib2.Request(imgurl)
req2.add_header('AbK_sid', AbK_sid);
req2.add_header('Accept', 'image/png,image/*;q=0.8,*/*;q=0.5');
req2.add_header('Referer', 'http://www.cqumzh.cn/bbs/logging.php?action=login')
req2.add_header('Accept-Language', 'zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3')
req2.add_header('Host', 'www.cqumzh.cn')
req2.add_header('User-Agent', 'Mozilla/5.0 (compatible; MISE 9.0; Windows NT 6.1); Trident/5.0')
req2.add_header('Connection', 'Keep-Alive')

#print urlopener.open(imgurl).read()
outfile=open(r'./code.jpg', 'w')
outfile.write(urlopener.open(req2).read())
outfile.flush()
outfile.close()