Function Ping( myHostName )
' This function returns True if the specified host could be pinged.
' myHostName can be a computer name or IP address.
' The Win32_PingStatus class used in this function requires Windows XP or later.
' This function is based on the TestPing function in a sample script by Don Jones
' http://www.scriptinganswers.com/vault/computer%20management/default.asp#activedirectoryquickworkstationinventorytxt

    ' Standard housekeeping
    Dim colPingResults, objPingResult, strQuery

    ' Define the WMI query
    strQuery = "SELECT * FROM Win32_PingStatus WHERE Address = '" & myHostName & "'"

    ' Run the WMI query
    Set colPingResults = GetObject("winmgmts://./root/cimv2").ExecQuery( strQuery )

    ' Translate the query results to either True or False
    For Each objPingResult In colPingResults
        If Not IsObject( objPingResult ) Then
            Ping = False
        ElseIf objPingResult.StatusCode = 0 Then
            Ping = True
        Else
            Ping = False
        End If
    Next

    Set colPingResults = Nothing
End Function

If Wscript.Arguments.Count > 0 Then
  HostName = Wscript.Arguments.Item(0)
  If Ping(HostName) Then
    Wscript.StdOut.WriteLine "alive"
  Else
    Wscript.StdOut.WriteLine "dead"
  End If
Else
  Wscript.StdOut.WriteLine "Usage: PingIt <hostname>"
End If
