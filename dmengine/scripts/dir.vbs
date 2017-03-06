WScript.Echo "{"
Set objFSO = CreateObject("Scripting.FileSystemObject")
objStartFolder = WScript.Arguments(0)
Set objFolder = objFSO.GetFolder(objStartFolder)
Set colFiles = objFolder.Files
sep = ""
For Each objFile in colFiles
	WScript.Echo sep
    Wscript.Echo "'" & objFile.Name & "' => {"
	WScript.Echo "'size' => '" & objFile.Size & "',"
	WScript.Echo "'path' => '" & objFile.Path & "',"
	WScript.Echo "'shortpath' => '" & objFile.ShortPath & "',"
	WScript.Echo "'shortname' => '" & objFile.ShortName & "',"
	WScript.Echo "'drive' => '" & objFile.Drive & "'"
    WScript.Echo "}"
	sep=","
	
Next
WScript.Echo "}"