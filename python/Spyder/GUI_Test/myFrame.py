# -*- coding: utf-8 -*-
"""
Created on Tue Apr 23 15:10:23 2019

@author: zLimbo
"""

import wx

class MyFrame(wx.Frame):
    
    def __init__(self, superior):
        wx.Frame.__init__(self, parent=superior, title=u'My First Form',
                          size = (300, 300))
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_MOVE, self.OnFrameMove)
        
        panel = wx.Panel(self, -1)
        label1 = wx.StaticText(panel, -1, "FrameSize:")
        label2 = wx.StaticText(panel, -1, "FramePos:")
        label3 = wx.StaticText(parent=panel, label="MousePos:")
        
        self.sizeFrame = wx.TextCtrl(panel, -1, "", style = wx.TE_READONLY)
        self.posFrame = wx.TextCtrl(panel, -1, "", sytle= wx.TE_READONLY)
        self.posMouse = wx.TextCtrl(panel, -1, "", style = wx.TE_READONLY)
        
        panel.Bind(wx.EVT_MOTION, self.OnMouseMove)
        self.panel = panel
        
        sizer = wx.FlexGridSizer(3, 2, 5, 5)
        sizer.Add(label1)
        sizer.Add(self.ssizeFrame)
        sizer.Add(label2)
        sizer.Add(self.pposFrame)
        sizer.Add(label3)
        sizer.Add(self.posMouse)
        
        border = wx.BoxSizer()
        border.Add(sizer, 0, wx.ALL, 15)
        panel.SetSizerAndFit(border)
        self.Fit()
        
    def OnSize(self, event):
        size = event.GetSize()
        self.sizeFrame.SetValue("%s, %s"%(size.width, size.height))
        event.Skip()
        
    def OnFrameMove(self, event):
        pos = event.GetPosition()
        self.posFrame.SetValue("%s, %s"%(pos.x, pos.y))
        
    def OnMouseMove(self, event):
        pos = event.GetPosition()
        self.posMouse.SetValue("%s, %s"%(pos.x, pos.y))
        
if __name__ == '__main__':
    app = wx.App()
    frame = MyFrame(None)
    frame.Show(True)
    app.MainLoop()