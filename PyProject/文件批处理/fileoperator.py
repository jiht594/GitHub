# coding=utf8
import os, os.path
import copy
import shutil 

def VisitDir_MoveFileByextToNewDir(arg, dirname, names):
    global g_rootpath
    for filename in names:
        fullpath = os.path.join(dirname, filename)
        if os.path.isdir(fullpath):
            continue
            #print "****" + fullpath
        else:
            extname = os.path.splitext(filename)
            if extname[1] in g_filetypesList :
                os.rename(fullpath, g_newdir + "\\" + filename)  
                print "copy: " + fullpath
                print "to: " + g_newdir
            #print fullpath
            #print '------------'
            
def VisitDir(arg, dirname, names):
    global g_rootpath
    for filename in names:
        fullpath = os.path.join(dirname, filename)
        if os.path.isdir(fullpath):
            if filename == "Debug" or filename == "Release" :
                shutil.rmtree(fullpath)
            try:
                os.rmdir(fullpath)
            except OSError, e:
                print e.strerror
                continue
            continue
            #print "****" + fullpath
        else:
            #if g_rootpath != dirname:
                #print "(()())()()" + dirname
            #print dirname
            extname = os.path.splitext(filespath)
            if extname[1] in g_filetypesList :
                os.remove(fullpath)    
                print "delete" + fullpath
            #print fullpath
            #print '------------'


def VisitDirNoSubDir(arg, dirname, names):
    global g_rootpath
    global g_count
    
    for filename in names:
        fullpath = os.path.join(dirname, filename)
        new_fullpath = os.path.join(dirname, g_newfilename)
        if len(g_newfilename) <= 0 :
            break
        if os.path.isdir(fullpath):
            continue
            #print "****" + fullpath
        else:
            if g_rootpath != dirname:
                break
                #print "(()())()()" + dirname
            #print dirname
            #print fullpath
            obj = os.path.splitext(filename) 
            new_fullpath_ext = new_fullpath + str(g_count) + obj[1]
            while os.path.isfile(new_fullpath_ext) == True :
                g_count = g_count + 1
                new_fullpath_ext = new_fullpath + str(g_count) + obj[1]
            os.rename(fullpath, new_fullpath_ext)
            g_count = g_count + 1


def renamefiles(olddir, newfilename):
    global g_rootpath
    global g_newfilename
    global g_count
    
    g_count = 1
    g_rootpath = olddir
    g_newfilename = newfilename
    
    os.path.walk(g_rootpath, VisitDirNoSubDir, ())

def renamePartfilenames(arg, dirname, names):
    global g_rootpath
    global g_filenameTruc
    start = g_filenameTruc[0]
    counts = g_filenameTruc[1]

    for filename in names:
        fullpath = os.path.join(dirname, filename)
        if os.path.isdir(fullpath):
            continue
        else:
            if g_rootpath != dirname:#不考虑子文件夹
                break
            obj = os.path.splitext(filename) #文件名分割为 名 和 .后缀
            #删除的字符数counts太多, 滤过   
            if start > 0:
                temp_start = start - 1
                if len(obj[0]) <= counts + temp_start:
                    break                         
                #print   obj[0][0:temp_start]
                #print   obj[0][temp_start+counts:]             
                new_fullpath_ext = g_rootpath + "\\" + obj[0][0:temp_start] + obj[0][temp_start+counts:]
            else:
                temp_start = start + 1
                if len(obj[0]) <= counts - temp_start:
                    break             
                #print  obj[0][0:temp_start-counts]
                #print  obj[0][temp_start:]
                new_fullpath_ext = g_rootpath + "\\" + obj[0][0:temp_start-counts] + obj[0][temp_start:]
            if os.path.isfile(new_fullpath_ext) == True :
                #重名加后缀
                postfix = 1
                while os.path.isfile(new_fullpath_ext) == True :
                    postfix = postfix + 1
                new_fullpath_ext = new_fullpath_ext + str(postfix)
            new_fullpath_ext = new_fullpath_ext + obj[1]
            print "-----------------"
            print fullpath
            print new_fullpath_ext
            os.rename(fullpath, new_fullpath_ext)


#startAndcount has 2 int values
#start position and character counts
def renamefilesPartname(olddir, startAndcounts):
    global g_rootpath
    global g_count
    global g_filenameTruc

    g_count = 1
    g_rootpath = olddir
    startAndcounts = str(startAndcounts).replace(" ","").replace("\t","").strip().split(',')
    try :
        start = int(startAndcounts[0])
        counts = int(startAndcounts[1])
    except :
        return u"部分重命名的输入命令格式不对, 请输入:数字逗号数字"
    g_filenameTruc = [start, counts]
    os.path.walk(g_rootpath, renamePartfilenames, ())

def clearfiles(olddir, filetypes):
    global g_filetypes
    global g_rootpath
    global g_filetypesList
    
    g_rootpath = olddir
    
    if len(filetypes) > 0:
        g_filetypes = str(filetypes)
    g_filetypesList = str(g_filetypes).replace(" ","").replace("\t","").strip().split(',')
    
    os.path.walk(g_rootpath, VisitDir, ())
    
def copyfileto(olddir, newdir, filetypes):
    global g_filetypes
    global g_rootpath
    global g_filetypesList
    global g_newdir
    
    g_newdir = newdir
    g_rootpath = olddir
    
    if len(filetypes) > 0:
        g_filetypes = str(filetypes)
    g_filetypesList = str(g_filetypes).replace(" ","").replace("\t","").strip().split(',')
    
    os.path.walk(g_rootpath, VisitDir_MoveFileByextToNewDir, ())    
    
    
g_rootpath = ""
g_newdir = ""
g_newfilename = ""
g_count = 0 #文件重命名后缀
g_filetypes = ""#".sdf, .ipch, .tlog, .pdb, .APS, .ncb, .obj, .pch"
g_filetypesList = []
g_filenameTruc = []

if __name__ == "__main__":
    global g_filetypes
    clearfiles(g_filetypes)
    #g_rootpath = u"D:\\新建文件夹"
    #os.path.walk(g_rootpath, VisitDir,())
    #os.path.walk(g_rootpath, VisitDirNoSubDir, ())
