Set oUTF8 = CreateObject("System.Text.UTF8Encoding")
Set oMD5 = CreateObject("System.Security.Cryptography.MD5CryptoServiceProvider")
Set fso = CreateObject("Scripting.FileSystemObject")

If WScript.Arguments.Count = 0 Then
  WScript.Echo "Usage: checksum.vbs <filename1> [<filename2> ...]"
  WScript.Quit(1)
End If

For f = 0 To WScript.Arguments.Count-1
	DoChecksum(WScript.Arguments(f))
Next

Sub DoChecksum(filename)
	Set file = fso.GetFile(filename)
	oMD5.Initialize()
	Dim cksum
	cksum = oMD5.ComputeHash_2( ReadBinaryFile(filename) )
	WScript.Echo ByteArrayToHexString(cksum) & " " & file.Name
End Sub

Function ByteArrayToHexString(bytearray)
  Dim I
  Redim B(lenb(bytearray)-1)

  For I=1 to lenb(bytearray)
    B(I-1) = right("0" & hex(AscB(MidB(bytearray,I,1))),2)
  Next
  ByteArrayToHexString = Join(B,"")

End Function

Function ReadBinaryFile(filename)
        Const adTypeBinary = 1
        Dim objStream
        Set objStream = CreateObject("ADODB.Stream")
        objStream.Type = adTypeBinary
        objStream.Open
        objStream.LoadFromFile filename
        readBinaryFile = objStream.Read
        objStream.Close
        Set objStream = Nothing
End Function
