Dim objSWbemLocator,objWMIService



'------------------------------------------ 
 Const EXW_LOGOFF   = 0 
 Const EXW_SHUTDOWN = 1 
 Const EXW_REBOOT   = 2 
 Const EXW_FORCE    = 4 
 Const EXW_POWEROFF = 8 
 '------------------------------------------ 
 Const ANONYMOUS    = 1 
 Const IDENTIFY     = 2 
 Const IMPERSONATE  = 3 
 Const DELEGATE     = 4 
 Const PKT          = 4 

If WScript.Arguments.Count <> 3 Then
   ShowUsage()
   WScript.Quit(1)
End If

Set objSWbemLocator = CreateObject("WbemScripting.SWbemLocator")
objSWbemLocator.Security_.impersonationlevel = IMPERSONATE 
objSWbemLocator.Security_.authenticationLevel = PKT 
objSWbemLocator.Security_.Privileges.AddAsString ("SeDebugPrivilege") 
objSWbemLocator.Security_.Privileges.AddAsString ("SeSecurityPrivilege") 
objSWbemLocator.Security_.Privileges.AddAsString ("SeShutdownPrivilege") 
objSWbemLocator.Security_.Privileges.AddAsString ("SeRemoteShutdownPrivilege") 

Set objWMIService = objSWbemLocator.ConnectServer(ServerName ,"root\CIMV2",strUser,strPassword)

ServerName = WScript.Arguments(0)
strUser = WScript.Arguments(1)
strPassword = WScript.Arguments(2)

Function GetServiceState(ServerName, ServiceName)
   '  Return one of:
   '  Stopped, Start Pending, Stop Pending,
   '  Running, Continue Pending, Pause Pending,
   '  Paused, Unknown

   Set colItems = objWMIService.ExecQuery( "SELECT State FROM Win32_Service WHERE Name='RpcSs'",,48) 

   For Each objItem in colItems
	GetServiceState =  objItem.State
   Next
End Function



Function RebootServer(ServerName)
   ' Returns:
   ' 1 = Successfully issued reboot command
   ' -2 = Could not reach server
   ' -3 = Reboot command failed
   Dim OpSystem
   For Each OpSystem in objWMIService.ExecQuery("select * from Win32_OperatingSystem where Primary=true")
      If IsObject(OpSystem) Then
         ' Invoke forced reboot
         If OpSystem.Win32Shutdown(6, 0) = 0 Then
            ' Success
            RebootServer = 1
         Else
            ' Command failed
            RebootServer = -3
         End If

      Else
         RebootServer = -2

      End If
   Next
End Function



' ***************************************
' *                                     *
' *       E N T R Y   P O I N T         *
' *                                     *
' ***************************************
'
' Verify server is currently up
'
WScript.StdOut.WriteLine Now & ": Verify server '" & ServerName & "' is currently up..."
If Not IsAvailable(ServerName) Then
   WScript.StdOut.WriteLine "Error: Server is down.  Reboot aborted!"
   WScript.Quit(1)
End If
WScript.StdOut.WriteLine Now & ": Server is up."

'
' Reboot server
'
WScript.StdOut.WriteLine Now & ": Rebooting server '" & ServerName & "'..."
RebootStatus = RebootServer(ServerName)
If RebootStatus < 0 Then
   WScript.StdOut.WriteLine "Error: Reboot returned error " & RebootStatus
   WScript.Quit(1)
End If
WScript.StdOut.WriteLine Now & ": Reboot command was successful"

'
' Wait for server to come down
'
WScript.StdOut.Write Now & ": Waiting for server '" & ServerName & "' to go down..."
WaitCount = 0
Do While IsAvailable(ServerName)
   WaitCount = WaitCount + 1
   If WaitCount > 60 Then ' 5 min timeout
      WScript.StdOut.WriteLine "Error: Timeout waiting for server to come down!"
      WScript.Quit(1)
   End If
   WScript.StdOut.Write(".")
   WScript.Sleep(5000)
Loop
WScript.StdOut.WriteLine "Success!"
WScript.StdOut.WriteLine Now & ": Server is down."

'
' Wait for server to come back up
'
WScript.StdOut.Write Now & ": Waiting for server '" & ServerName & "' to come back up..."
WaitCount = 0
Do While Not IsAvailable(ServerName)
   WaitCount = WaitCount + 1
   If WaitCount > 240 Then ' 20 min timeout
      WScript.StdOut.WriteLine "Error: Timeout waiting for server to come back up!"
      WScript.Quit(1)
   End If
   WScript.StdOut.Write(".")
   WScript.Sleep(5000)
Loop
WScript.StdOut.WriteLine "Success!"
WScript.StdOut.WriteLine Now & ": Server is back up after reboot."

'
' Success!
'
WScript.Quit(0)


Sub ShowUsage()
   WScript.Echo "Usage: " & WScript.ScriptName & " <Server name>"
End Sub


' Return True if available
Function IsAvailable(ServerName)
WScript.Echo "IsAvailable "& ServerName
   ' Use Windows RPC service state as vital sign
t = GetServiceState(ServerName, "RpcSs")
WScript.Echo t 
   IsAvailable = (GetServiceState(ServerName, "RpcSs") = "Running")
End Function




