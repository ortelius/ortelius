Set oShell = CreateObject( "WScript.Shell" )

Function GetArg(n)
  If Wscript.Arguments.Count > n Then
    GetArg = Wscript.Arguments.Item(n)
  Else
    GetArg = ""
  End If
End Function

Function GetEnv(Name)
  EnvVar = "%TRI_" & Name & "%"
  Ret = oShell.ExpandEnvironmentStrings(EnvVar)
  If Ret = EnvVar Then
    GetEnv = ""
  Else
    GetEnv = Ret
  End If
End Function

Wscript.StdOut.WriteLine "%1 = [" & GetArg(0) & "] %2 = [" & GetArg(1) & "] " _
	& "%3 = [" & GetArg(2) & "] %4 = [" & GetArg(3) & "] " _
	& "ARG1 = [" & GetEnv("ARG1") & "] ARG2 = [" & GetEnv("ARG2") & "] " _
	& "ARG3 = [" & GetEnv("ARG3") & "] ARG4 = [" & GetEnv("ARG4") & "]"
