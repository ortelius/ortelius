qry = "SELECT * FROM win32_ComputerSystem"
Set wmi = GetObject("winmgmts://./root/cimv2")
For Each s In wmi.ExecQuery(qry)
	WScript.Echo s.NumberOfProcessors & " " & " " & Round(s.TotalPhysicalMemory/1048576)
	WScript.Echo s.Domain & " " & s.CurrentTimeZone & " " & s.Name & " " & s.PartOfDomain &" " & s.SystemType
	WScript.Echo s.UserName
Next