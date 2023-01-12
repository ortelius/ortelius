On Error Resume Next
iisversion = 0
Set wmi = GetObject("winmgmts://./root/MicrosoftIISv2")
Set colItems = wmi.ExecQuery("Select * from IIsWebInfo")
For Each objItem In colItems
  WScript.Echo objItem.MajorIIsVersionNumber & "." & objItem.MinorIIsVersionNumber
  iisversion = objItem.MajorIIsVersionNumber
Next

If iisversion = 0 Then
  WScript.Echo "IIS Version is not installed"
  WScript.Quit(0)
End If

WScript.Echo "IIS Version is " & iisversion
WScript.Echo

WScript.Echo "Sites:"

If iisversion = 6 Then

  Set colItems = wmi.ExecQuery("Select * from IIsWebServerSetting")
  For Each objItem In colItems
    'WScript.Echo objItem.Name & " " & objItem.ServerComment
    Set colItems2 = wmi.ExecQuery("Select * from IIsWebVirtualDirSetting where Name = '" & objItem.Name & "/root'")
    For Each objItem2 In colItems2
      WScript.Echo objItem.ServerComment & " -> " & objItem2.Path
    Next
  Next

Else

  Set wmi2 = GetObject("winmgmts://./root/WebAdministration")
  Set colItems = wmi.ExecQuery("Select * from VirtualDirectory")
  For Each objItem In colItems
    WScript.Echo objItem.SiteName & " " & objItem.PhysicalPath
  Next

End If

WScript.Echo "Done."
