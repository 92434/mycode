On Error Resume Next 
Set IE = CreateObject("InternetExplorer.Application") 
IE.Navigate "http://zhidao.baidu.com" 
IE.Visible = 1 'Visible��ֵ0���أ�1��С����2�´��� 
Wscript.Sleep 5000 

Set O = IE.Document 
Do Until i = 100 '100����ˢ��100�� 
i = i + 1 
Wscript.Sleep 1000*60*10/60 '���10����ÿ��10����ˢ�� 
O.Location.Reload(True) 
If Err <> 0 Then 
MsgBox "����"& Err.Description ,4096+64 :Wscript.Quit 
End If 
Loop 
IE.Quit 
CreateObject("WScript.Shell").Popup "��������ɣ������ѹر�",6,"��ʾ",4096+64 