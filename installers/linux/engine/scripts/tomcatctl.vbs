If WScript.Arguments.Count<4 Then
	WScript.Echo "ERROR: USAGE tomcatctl Pre|Post service rootdir appname [timeout]"
	WScript.Quit(1)
End If
mode = WScript.Arguments(0)
service = WScript.Arguments(1)
rootdir = WScript.Arguments(2)
appname = WScript.Arguments(3)
timeout=30
If WScript.Arguments.Count>4 Then
	timeout=WScript.Arguments(4)
End If

Select Case mode
Case "Pre"
	'No actions
Case "Post"
	If Len(appname) = 0 Then
		WScript.Echo "ERROR: APPLICATION_NAME has not been specified"
		WScript.Quit(1)
	Else
		w = InStrRev(LCase(appname),".war")
		If w > 0 Then
			appname = Left(appname,w-1)
		End If
	End If
Case Else
	WScript.Echo "ERROR: MODE must be either ""Pre"" or ""Post"""
	WScript.Quit(1)
End Select

Set fs = WScript.CreateObject ("Scripting.FileSystemObject")
Set sh = WScript.CreateObject("WScript.Shell")


If Len(rootdir) > 0 Then
	If fs.FolderExists(rootdir) Then
		If fs.FolderExists(rootdir & "\bin") and fs.FolderExists(rootdir & "\conf") and fs.FolderExists(rootdir & "\webapps") Then
			' rootdir seems valid
		Else
			WScript.Echo "ERROR: Specified TOMCAT_ROOT (" & rootdir & ") is not a valid Tomcat root directory"
			WScript.Quit(1)
		End If
	Else
		WScript.Echo "ERROR: Specified TOMCAT_ROOT (" & rootdir & ") does not exist"
		WScript.Quit(1)
	End If
	If Len(Service) = 0 Then
		If fs.FileExists(rootdir & "\bin\startup.bat") Then
		Else
			Wscript.Echo "ERROR: TOMCAT_SERVICE_NAME not specified but startup.bat not found in bin directory"
			WScript.Quit(1)
		End If
	End If
Else
	If Len(service) = 0 Then
		WScript.Echo "ERROR: Neither TOMCAT_ROOT nor TOMCAT_SERVICE_NAME has been specified"
		WScript.Quit(1)
	End If
End If

Dim n,f

Set wmi = GetObject("winmgmts://./root/cimv2")

Sub ExitWithError(et,res)
  Select Case res
	Case 0:    t=""
	Case 1:    t="Not Supported"
	Case 2:    t="Access Denied"
	Case 3:    t="Dependent Services Running"
	Case 4:    t="Invalid Service Control"
	Case 5:    t="Service Cannot Accept Control"
	Case 6:    t="Service Not Active"
	Case 7:    t="Service Request Timeout"
	Case 8:    t="Unknown Failure"
	Case 9:    t="Path Not Found"
	Case 10:   t="Service Already Running"
	Case 11:   t="Service Database Locked"
	Case 12:   t="Service Dependency Deleted"
	Case 13:   t="Service Dependency Failure"
	Case 14:   t="Service Disabled"
	Case 15:   t="Service Logon Failure"
	Case 16:   t="Service Marked For Deletion"
	Case 17:   t="Service No Thread"
	Case 18:   t="Status Circular Dependency"
	Case 19:   t="Status Duplicate Name"
	Case 20:   t="Status Invalid Name"
	Case 21:   t="Status Invalid Parameter"
	Case 22:   t="Status Invalid Service Account"
	Case 23:   t="Status Service Exists"
	Case 24:   t="Service Already Paused"
	Case Else: t="Unknown Error"
  End Select
  WScript.Echo et & t
  WScript.Quit(res)
End Sub

Sub StartService(s)
	WScript.Echo "Starting """ & s.DisplayName & """ Service"
	StartTime=Timer()
	res = s.StartService
	If res = 0 Then
		Do Until wmi.ExecQuery(qry & " AND State='Running'").Count > 0 Or Timer() - StartTime > timeout
			WScript.Sleep 100
		Loop
		If wmi.ExecQuery(qry & " AND State='Running'").Count > 0 Then
			WScript.Echo "Service """ & s.DisplayName & """ Started"
			WScript.Quit(0)
		Else
			WScript.Echo "Timed out waiting for service """ & s.DisplayName & """ to start"
			WScript.Quit(1)
		End If
	Else
		ExitWithError "Failed to start """ & s.DisplayName & """ Service: ",res
	End If

End Sub

Sub StopService(s)
	WScript.Echo "Stopping """ & s.DisplayName & """ Service"
	If s.State = "Stopped" Then
		WScript.Echo "Failed to stop """ & s.DisplayName & """ Service: Service is Not Running"
		WScript.Quit(1)
	End If
	StartTime=Timer()
	res = s.StopService
	If res = 0 Then
		Do Until wmi.ExecQuery(qry & " AND State='Stopped'").Count > 0 Or Timer() - StartTime > timeout
			WScript.Sleep 100
		Loop
		If wmi.ExecQuery(qry & " AND State='Stopped'").Count > 0 Then
			WScript.Echo "Service """ & s.DisplayName & """ Stopped"
			WScript.Quit(0)
		Else
			WScript.Echo "Timed out waiting for service """ & s.DisplayName & """ to stop"
			WScript.Quit(1)
		End If
	Else
		ExitWithError "Failed to stop """ & s.DisplayName & """ Service: ",res
	End If
End Sub

Sub RemoveSubFolders(Folder)
	If fs.FolderExists(Folder) Then
		For Each Subfolder In Folder.SubFolders
			Set files = SubFolder.Files
			For Each file In files
				file.Delete true
				n=n+1
			Next
			RemoveSubFolders Subfolder
		Next
		Folder.Delete true
		f=f+1
	End If
End Sub
 


Sub RemoveWorkFiles(s)
	pn = s.pathname	' format C:\path\to\tomcat\bin\tomcat.exe
	e1 = Left(pn,InStrRev(pn,".exe"))
	wf = Left(e1,InStrRev(e1,"\bin\")) & "work\catalina\localhost\" & appname
	af = Left(e1,InStrRev(e1,"\bin\")) & "webapps\" & appname
	If fs.FolderExists(wf) Then
		WScript.Echo "Cleaning Work Directory " & wf	
		n=0
		f=0
		RemoveSubFolders fs.GetFolder(wf)
		WScript.Echo "Removed " & n & " file(s), " & f & " folder(s)"
	End If
	If fs.FolderExists(af) Then
		WScript.Echo "Cleaning app Directory " & af	
		n=0
		f=0
		RemoveSubFolders fs.GetFolder(af)
		WScript.Echo "Removed " & n & " file(s), " & f & " folder(s)"
	End If
End Sub

Sub CleanFromRoot
	wf = rootdir & "work\catalina\localhost\" & appname
	af = rootdir & "webapps\" & appname
	If fs.FolderExists(wf) Then
		WScript.Echo "Cleaning Work Directory " & wf	
		n=0
		f=0
		RemoveSubFolders fs.GetFolder(wf)
		WScript.Echo "Removed " & n & " file(s), " & f & " folder(s)"
	End If
	If fs.FolderExists(af) Then
		WScript.Echo "Cleaning app Directory " & af	
		n=0
		f=0
		RemoveSubFolders fs.GetFolder(af)
		WScript.Echo "Removed " & n & " file(s), " & f & " folder(s)"
	End If
End Sub

'
'Entry
'
If Len(Service) > 0 Then
	' Service Name given
	qry = "SELECT * FROM Win32_Service WHERE DisplayName='" & service & "'"

	For Each s In wmi.ExecQuery(qry)
		If mode = "Pre" Then
			StopService s
		Else
			If Len(rootdir) > 0 Then
				CleanFromRoot
			Else
				RemoveWorkFiles s
			End If
			StartService s
		End If
	Next
	WScript.Echo "Failed to start """ & service & """ service: No such Service"
	WScript.Quit(1)
Else
	' No Service Name - use rootdir
	If mode = "Pre" Then
		Sh.Run(rootdir & "\bin\shutdown.bat")
	Else
		CleanFromRoot
		Sh.Run(rootdir & "\bin\startup.bat")
	End If
End If
