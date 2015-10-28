On Error Resume Next 
Set IE = CreateObject("InternetExplorer.Application") 
IE.Navigate "http://zhidao.baidu.com" 
IE.Visible = 1 'Visible数值0隐藏，1最小化，2新窗口 
Wscript.Sleep 5000 

Set O = IE.Document 
Do Until i = 100 '100代表刷新100次 
i = i + 1 
Wscript.Sleep 1000*60*10/60 '最后10代表每隔10分钟刷新 
O.Location.Reload(True) 
If Err <> 0 Then 
MsgBox "错误"& Err.Description ,4096+64 :Wscript.Quit 
End If 
Loop 
IE.Quit 
CreateObject("WScript.Shell").Popup "任务以完成，窗口已关闭",6,"提示",4096+64 