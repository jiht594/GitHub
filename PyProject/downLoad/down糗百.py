#coding:utf8
'''
Created on 2014-12-12

@author: cl282907
'''
import urllib2
import re
import time
import datetime
import thread
from urllib2 import URLError

class my_spider_of_qiushibaike:
    
    def __init__(self):
        self.page=1
        self.download_page={}
        self.obtain=False
        self.NowPrintPage=1
    
    ######得到每一页的文字信息###########
    def GetPageInfo(self,page):
        
        items_of_1page=[]
        
        RequestUrl='http://www.qiushibaike.com/late/page/%s' %page
        user_agent = 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)'
        headers={'User-Agent':user_agent}
        #socket.settimeout(3)
        request=urllib2.Request(RequestUrl, None, headers)
        initial_downloadInfo=urllib2.urlopen(request).read().decode('utf-8')
        
        my_re=re.compile('<div class="content" title="(.*?)">(.*?)</div>',re.DOTALL)
        for item in my_re.findall(initial_downloadInfo):
            with open(r'E:\spider','a') as file:
                file.write(str(item))
            items_of_1page.append([page,item[0].replace('\n',''),item[1].replace('\n','')])
            
        return items_of_1page
     
    #######当在list中的信息不超过5页时自动加载下一页##################
    def LoadNewPages(self):
        while self.obtain:
            try:
                if len(self.download_page)<5:
                    #self.download_page.append(self.GetPageInfo(self.page))
                    items = self.GetPageInfo(self.page)
                    if len(items) <= 0 :
                        return 0
                    self.download_page[self.page] = items
                    self.page+=1
                else :
                    break
            except URLError,e:
                if hasattr(e, 'code'):
                    print 'this is a URLError and We failed to reach a server,',e.code
                if hasattr(e, 'reason'):
                    print 'this is an HTTPError',e.reason
            else:
                time.sleep(1)
    
    ############显示已经爬取的网页信息#########            
    def showPageContent(self):
        
        while(self.NowPrintPage < self.page):     #self.page是当前已经爬取到的页数，self.NowPrintPage是当前显示到的页数
            
            for items in self.download_page[self.NowPrintPage]:
                str1 = unicode(str(items[0]), 'utf8', errors = 'ignore')
                str2 = unicode(str(items[1]), 'utf8', errors = 'ignore')
                str3 = str(items[2]).decode('utf8', 'ignore').encode('gbk', 'ignore').decode('gbk', 'ignore')
                print u"第%s页" % str1, str2, str3
                fileHandle.write(u"第%s页 %s %s\n" % (str1, str2, str3))
                my_input= ''#raw_input('input \"enter\", to stop, input \"quit\"')
                if my_input=='quit':
                    self.obtain=False
                    break
            self.NowPrintPage+=1
        if self.NowPrintPage >= self.page :
            self.download_page = {}            
                
            
    def start(self):
        
            
        self.obtain=True
    #    page=self.page
        
        print u"正在拼命加载中。。。"
        #thread.start_new_thread(self.LoadNewPages,())
        self.LoadNewPages()
        while self.obtain:
            
            if self.download_page:
                self.showPageContent()
            else :
                print u"正在拼命加载中。。。"
                if self.LoadNewPages() == 0 :
                    print u"全部加载完毕"
                    break
                
                
    
print u'请按下回车来浏览今日糗百内容'

g_toDate = ""
#raw_input()
my_spider=my_spider_of_qiushibaike()
fileHandle = open ( 'test.txt', 'a' )
my_spider.start()
fileHandle.close()
            