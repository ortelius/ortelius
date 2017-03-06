
Set fso = CreateObject("Scripting.FileSystemObject")

Sub UnzipFiles(folder, file)
        Dim sa, filesInzip, zfile, i : i = 1
        Set sa = CreateObject("Shell.Application")
	folder = Replace(folder,"/","\")
	' WScript.Echo folder & "\" & file
        Set filesInzip=sa.NameSpace(folder & "\" & file).items
        For Each zfile In filesInzip
	    WScript.Echo "Extracting " & zfile
            If Not fso.FileExists(folder & "\" & zfile) Then
                sa.NameSpace(folder).CopyHere(zfile), &H654
                i = i + 1
            End If
            If i = 99 Then
            zCleanup file, i 
            i = 1
            End If
        Next
        If i > 1 Then 
            zCleanup file, i
        End If
        fso.DeleteFile(folder & "\" & file)
End Sub

Sub zCleanUp(file, count)   
        Dim i
        
        For i = 1 To count
           If fso.FolderExists(fso.GetSpecialFolder(2) & "\Temporary Directory " & i & " for " & file) = True Then
           text = fso.DeleteFolder(fso.GetSpecialFolder(2) & "\Temporary Directory " & i & " for " & file, True)
           Else
              Exit For
           End If
        Next
End Sub


UnzipFiles WScript.Arguments(0), WScript.Arguments(1)