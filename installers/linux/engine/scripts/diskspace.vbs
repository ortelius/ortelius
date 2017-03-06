Dim fso,basedir,drive,d
Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count > 0 Then
	basedir=WScript.Arguments(0)
	If Mid(basedir,2,1)=":" Then
		drive=Left(basedir,1) & ":"
	Else
		drive="C:"
	End If
	Set d = fso.GetDrive(drive)
	WScript.Echo d.FreeSpace
End If

