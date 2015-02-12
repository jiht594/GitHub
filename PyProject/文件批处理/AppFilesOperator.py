#coding=utf8
#!/usr/bin/env python
#Boa:App:BoaApp

import wx

import MainFrame

modules ={'Frame1': [1, 'Main frame of Application', u'MainFrame.py']}

class BoaApp(wx.App):
    def OnInit(self):
        self.main = MainFrame.create(None)
        self.main.Show()
        self.SetTopWindow(self.main)
        
        wx.MessageBox(u"部分操作不可逆, 请小心操作!", 'WARNING', style = wx.ICON_EXCLAMATION) 
        return True

def main():
    application = BoaApp(0)
    application.MainLoop()

if __name__ == '__main__':
    main()
