If WScript.Arguments.Count>0 Then
	qry = "SELECT * FROM win32_Service WHERE DisplayName='" & WScript.Arguments(0) & "'"
Else
	qry = "SELECT * FROM Win32_Service"
End If
Set wmi = GetObject("winmgmts://./root/cimv2")
For Each s In wmi.ExecQuery(qry)
WScript.Echo s.DisplayName & " " & s.Name & " " & s.State
Next