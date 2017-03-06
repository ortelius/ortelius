service = WScript.Arguments(0)
action = WScript.Arguments(1)
timeout=30
If WScript.Arguments.Count>2 Then
	timeout=WScript.Arguments(2)
End If

qry = "SELECT * FROM Win32_Service WHERE DisplayName='" & service & "'"
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




For Each s In wmi.ExecQuery(qry)
  Select Case action
    Case "start": StartService s
    Case "stop" : StopService s
    Case Else   : WScript.Echo "Invalid action: " & action
  End Select
Next
WScript.Echo "Failed to " & action & " """ & service & """ service: No such Service"
WScript.Quit(1)
