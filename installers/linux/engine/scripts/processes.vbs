If WScript.Arguments.Count>0 Then
	qry = "SELECT * FROM win32_Process WHERE Name='" & WScript.Arguments(0) & "'"
Else
	qry = "SELECT * FROM Win32_Process"
End If

Set wmi = GetObject("winmgmts://./root/cimv2")
WScript.Echo "["
First = True

For Each s In wmi.ExecQuery(qry)
	Res = s.GetOwner(strNameOfUser,strNameofDomain)
	If Res <> 0 Then
	        'Wscript.Echo "Could not get owner info for process " & _  
	        '    s.Name & VBNewLine _
	        '    & "Error = " & Res
		strNameOfUser="unknown"
	End If
	
	'strCommandLine = s.CommandLine
	strCommandLine = "Wibble"
	
	If Not First Then WScript.Echo "," End If
	WScript.Echo "{ ""name"": """ & s.Name & """, ""command"": """ & strCommandLine _
		& """, ""pid"": """ & s.ProcessID & """, ""ppid"": """ & s.ParentProcessID _
		& """, ""user"": """ & strNameOfUser & """}"
	First = False
Next
WScript.Echo "]"
