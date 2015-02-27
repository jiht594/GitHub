#coding=utf8
import os,os.path
import wx
import wx.lib.buttons
import copy
import fileoperator

def create(parent):
    return Frame1(parent)

[wxID_FRAME1, wxID_FRAME1DIRPICKERCTRL1, wxID_FRAME1PANEL1, wxID_FRAME1RENAME, 
 wxID_FRAME1TEXTCTRL_NEWNAME, wxID_FRAME1CLEAR, wxID_FRAME1TEXTCTRL_RenamaPart,
 wxID_FRAME1TEXTCTRL_CLEAR, wxID_FRAME1_COPYTO, wxID_FRAME1_rename_part
] = [wx.NewId() for _init_ctrls in range(10)]

class Frame1(wx.Frame):
    def _init_coll_boxSizer1_Items(self, parent):
        # generated method, don't edit

        parent.AddWindow(self.dirPickerCtrl1, 0, border=0, flag=0)
        parent.AddWindow(self.panel1, 0, border=0, flag=0)

    def _init_sizers(self):
        # generated method, don't edit
        self.boxSizer1 = wx.BoxSizer(orient=wx.VERTICAL)

        self._init_coll_boxSizer1_Items(self.boxSizer1)

        self.SetSizer(self.boxSizer1)

    def _init_ctrls(self, prnt):
        # generated method, don't edit
        wx.Frame.__init__(self, id=wxID_FRAME1, name='', parent=prnt,
              pos=wx.Point(569, 238), size=wx.Size(400, 407),
              style=wx.MINIMIZE_BOX | wx.SYSTEM_MENU | wx.CAPTION | wx.CLOSE_BOX | wx.CLIP_CHILDREN
              , title=u'文件操作工具')
        self.SetClientSize(wx.Size(384, 369))
        #############################################
        self.dirPickerCtrl1 = wx.DirPickerCtrl(id=wxID_FRAME1DIRPICKERCTRL1,
              message='Select a folder', name='dirPickerCtrl1', parent=self, 
              path='', pos=wx.Point(0, 0), size=wx.Size(382, 30),
              style=wx.DIRP_DEFAULT_STYLE)
        
        self.dirPickerCtrl1.SetPath(u'!请先选择文件夹)')
        #self.dirPickerCtrl1.SetToolTipString(u'')
        self.dirPickerCtrl1.Bind(wx.EVT_DIRPICKER_CHANGED,
              self.OnDirPickerCtrl1DirpickerChanged,
              id=wxID_FRAME1DIRPICKERCTRL1)
        ###########################################
        self.panel1 = wx.Panel(id=wxID_FRAME1PANEL1, name='panel1', parent=self,
              pos=wx.Point(0, 30), size=wx.Size(384, 369),
              style=wx.TAB_TRAVERSAL)
        ############################################
        self.textCtrlNewName = wx.TextCtrl(id=wxID_FRAME1TEXTCTRL_NEWNAME, name='textCtrlNewName',
              parent=self.panel1, pos=wx.Point(2, 32), size=wx.Size(287, 30),
              style=0, value=u'新文件名(不需要后缀名,不包括子文件夹)')
        #######################
        self.RENAME = wx.Button(id=wxID_FRAME1RENAME, label=u'批量重命名',
              name=u'RENAME', parent=self.panel1, pos=wx.Point(294, 30),
              size=wx.Size(88, 32), style=0)
        
        self.RENAME.Bind(wx.EVT_BUTTON, self.OnButtonRenameFile,
              id=wxID_FRAME1RENAME)
        ################################
        self.textCtrlClear = wx.TextCtrl(id=wxID_FRAME1TEXTCTRL_CLEAR, name='textCtrlClear',
              parent=self.panel1, pos=wx.Point(2, 64), size=wx.Size(287, 62), 
              style=wx.TE_MULTILINE, value=u'.sdf, .ipch, .tlog, .pdb, .APS, .ncb, .obj, .pch')
        #############################
        self.CLEAR = wx.Button(id=wxID_FRAME1CLEAR, label=u'批量清理',
              name=u'CLEAR', parent=self.panel1, pos=wx.Point(294, 62),
              size=wx.Size(88, 32), style=0)
        self.CLEAR.Bind(wx.EVT_BUTTON, self.OnButtonClearFile,
                             id=wxID_FRAME1CLEAR)        
        #################################
        self.MoveTo = wx.Button(id=wxID_FRAME1_COPYTO, label=u'移动到...',
                               name=u'COPYTO', parent=self.panel1, pos=wx.Point(294, 94),
                               size=wx.Size(88, 32), style=0)
        self.MoveTo.Bind(wx.EVT_BUTTON, self.OnButtonCopyFileTo,
                        id=wxID_FRAME1_COPYTO)       
        ######################################
        '''
        self.CLEAR = wx.Button(id=wxID_FRAME1CLEAR, label=u'清理',
                               name=u'CLEAR', parent=self.panel1, pos=wx.Point(294, 96),
                               size=wx.Size(88, 32), style=0)
        self.CLEAR.Bind(wx.EVT_BUTTON, self.OnButtonClearFile,
                        id=wxID_FRAME1CLEAR)              
        self.staticText_1 = wx.StaticText(id=wxID_staticText_desp1, name='staticText1',
            parent=self.panel1, pos=wx.Point(2, 138), size=wx.Size(287, 70),
            style=0, 
            label=u'修改文件名.说明(输入无括号):(12345音乐.mp3)填入(1,5), 变成音乐.mp3;填入-1,2,变成12345.mp3\n'
                 +u'第一个数字:1代表从前到后或者-1从后到前, 第二个数字:代表删除多少个字符\n' )
        ''' 
        #################################
        self.textCtrlRenamePartFile = wx.TextCtrl(id=wxID_FRAME1TEXTCTRL_RenamaPart, name='textCtrlRenamaPart',
                                         parent=self.panel1, pos=wx.Point(2, 138), size=wx.Size(287, 30), 
                                         style=0,value=u'鼠标放这里有提示(注意:重命名不包括子文件夹)')        
        self.textCtrlRenamePartFile.SetToolTipString(
            u'(输入格式:数字,数字)第一个数字:正数代表从前到后或者负数从后到前, 第二个数字:代表删除多少个字符\n'
            +u'例子:(12345音乐.mp3), 如果填入(1,5), 变成(音乐.mp3);如果填入-2,2,变成1234乐.mp3\n')
        #############################
        self.fileRenamePart = wx.Button(id=wxID_FRAME1_rename_part, label=u'部分重命名',
                               name=u'renamepart', parent=self.panel1, pos=wx.Point(294, 138),
                               size=wx.Size(88, 32), style=0)
        self.fileRenamePart.Bind(wx.EVT_BUTTON, self.OnButtonRenamePartFile,
                            id=wxID_FRAME1_rename_part)        
        
        ########################
        self._init_sizers()
        ###########################

    def __init__(self, parent):
        self._init_ctrls(parent)
        self.pickedDir = ""

    def OnGenBitmapTextButton1Button(self, event):
        event.Skip()

    def OnDirPickerCtrl1DirpickerChanged(self, event):
        self.pickedDir = self.dirPickerCtrl1.GetPath()

    def OnButtonRenameFile(self, event):
        if os.path.isdir(self.pickedDir):
            self.newname = self.textCtrlNewName.GetValue()
            if len(self.newname) > 0 :
                #wx.MessageBox(self.newname) 
                btnResult = wx.MessageBox(u"确认要重命名 \"%s\" 中的文件?" % self.pickedDir, "INFO", wx.OK | wx.CANCEL) 
                if btnResult == wx.OK :
                    fileoperator.renamefiles(copy.copy(self.pickedDir), copy.copy(self.newname))
                    wx.MessageBox(u"重命名完毕", 'INFO')
            else :
                wx.MessageBox(u"请填写新的文件名", 'INFO') 
        else :
            wx.MessageBox(u"不是有效的文件夹", 'INFO') 
    def OnButtonClearFile(self, event):
        if os.path.isdir(self.pickedDir):
            #wx.MessageBox(u"test") 
            filetypes = self.textCtrlClear.GetValue()
            if len(filetypes) > 0 :
                btnResult = wx.MessageBox(u"确认要删除%s文件?" % filetypes, "INFO", wx.OK | wx.CANCEL) 
                if btnResult == wx.OK :
                    fileoperator.clearfiles(copy.copy(self.pickedDir), filetypes)
                    fileoperator.clearfiles(copy.copy(self.pickedDir), filetypes)    
                    wx.MessageBox(u"删除完毕", 'INFO')                
            else :
                wx.MessageBox(u"请输入要删除的文件后缀(eg:.txt, .rmvb)", 'INFO')   
        else :
            wx.MessageBox(u"不是有效的文件夹", 'INFO') 
            
    def OnButtonCopyFileTo(self, event):
        if os.path.isdir(self.pickedDir):
            #wx.MessageBox(u"test") 
            filetypes = self.textCtrlClear.GetValue()
            if len(filetypes) > 0 :
                fileDlg = wx.DirDialog(self)
                if fileDlg.ShowModal() == wx.ID_OK :
                    destpath = fileDlg.GetPath()
                    btnResult = wx.MessageBox(u"确认要移动文件到 \"%s\"?" % destpath, "INFO", wx.OK | wx.CANCEL) 
                    if btnResult == wx.OK :
                        if self.pickedDir == destpath :
                            wx.MessageBox(u"2个文件夹相同, 不需要拷贝", 'INFO')    
                        else :
                            fileoperator.copyfileto(copy.copy(self.pickedDir), copy.copy(destpath), filetypes)
                            wx.MessageBox(u"移动文件完毕", 'INFO')                
            else :
                wx.MessageBox(u"请输入要删除的文件后缀(eg:.txt, .rmvb)", 'INFO')   
        else :
            wx.MessageBox(u"不是有效的文件夹", 'INFO') 
            
    def OnButtonRenamePartFile(self, event):
        if os.path.isdir(self.pickedDir):
            self.fileRenameCommand = self.textCtrlRenamePartFile.GetValue()
            if len(self.fileRenameCommand) > 0 :
                #wx.MessageBox(self.newname) 
                btnResult = wx.MessageBox(u"确认要*部分*重命名 \"%s\" 中的文件?" % self.pickedDir, "INFO", wx.OK | wx.CANCEL) 
                if btnResult == wx.OK :
                    bret = fileoperator.renamefilesPartname(copy.copy(self.pickedDir), copy.copy(self.fileRenameCommand))
                    if bret == None :
                        wx.MessageBox(u"*重命名完毕*", 'INFO')
                    else:
                        wx.MessageBox(bret, 'INFO')
            else :
                wx.MessageBox(u"请填写新的文件名", 'INFO') 
        else :
            wx.MessageBox(u"不是有效的文件夹", 'INFO')     