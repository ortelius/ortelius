# Microsoft Developer Studio Project File - Name="rtoexecp2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=rtoexecp2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rtoexecp2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rtoexecp2.mak" CFG="rtoexecp2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rtoexecp2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "rtoexecp2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rtoexecp2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /I ".." /I "../openbsd-compat" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 libeay32.lib ssleay32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib netapi32.lib zlib.lib /nologo /subsystem:console /machine:I386 /out:"Release/sshexec.exe" /libpath:"./LIBRARIES"

!ELSEIF  "$(CFG)" == "rtoexecp2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /I ".." /I "../openbsd-compat" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libeay32.lib ssleay32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib netapi32.lib zlib.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"./LIBRARIES"

!ENDIF 

# Begin Target

# Name "rtoexecp2 - Win32 Release"
# Name "rtoexecp2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\acss.c
# End Source File
# Begin Source File

SOURCE=..\atomicio.c
# End Source File
# Begin Source File

SOURCE=..\authfd.c
# End Source File
# Begin Source File

SOURCE=..\authfile.c
# End Source File
# Begin Source File

SOURCE=..\base64.c
# End Source File
# Begin Source File

SOURCE="..\bsd-arc4random.c"
# End Source File
# Begin Source File

SOURCE="..\bsd-asprintf.c"
# End Source File
# Begin Source File

SOURCE="..\bsd-getpeereid.c"
# End Source File
# Begin Source File

SOURCE="..\bsd-misc.c"
# End Source File
# Begin Source File

SOURCE=..\bufaux.c
# End Source File
# Begin Source File

SOURCE=..\buffer.c
# End Source File
# Begin Source File

SOURCE=..\canohost.c
# End Source File
# Begin Source File

SOURCE=..\channels.c
# End Source File
# Begin Source File

SOURCE="..\cipher-3des1.c"
# End Source File
# Begin Source File

SOURCE="..\cipher-acss.c"
# End Source File
# Begin Source File

SOURCE="..\cipher-aes.c"
# End Source File
# Begin Source File

SOURCE="..\cipher-bf1.c"
# End Source File
# Begin Source File

SOURCE="..\cipher-ctr.c"
# End Source File
# Begin Source File

SOURCE=..\cipher.c
# End Source File
# Begin Source File

SOURCE=..\cleanup.c
# End Source File
# Begin Source File

SOURCE=..\clientloop.c
# End Source File
# Begin Source File

SOURCE=..\compat.c
# End Source File
# Begin Source File

SOURCE=..\compress.c
# End Source File
# Begin Source File

SOURCE=..\crc32.c
# End Source File
# Begin Source File

SOURCE=..\daemon.c
# End Source File
# Begin Source File

SOURCE=..\deattack.c
# End Source File
# Begin Source File

SOURCE=..\dh.c
# End Source File
# Begin Source File

SOURCE=..\dispatch.c
# End Source File
# Begin Source File

SOURCE=..\dns.c
# End Source File
# Begin Source File

SOURCE=..\entropy.c
# End Source File
# Begin Source File

SOURCE=..\fatal.c
# End Source File
# Begin Source File

SOURCE=..\getrrsetbyname.c
# End Source File
# Begin Source File

SOURCE="..\gss-genr.c"
# End Source File
# Begin Source File

SOURCE=..\hostfile.cpp
# End Source File
# Begin Source File

SOURCE=..\kex.c
# End Source File
# Begin Source File

SOURCE=..\kexdh.c
# End Source File
# Begin Source File

SOURCE=..\kexdhc.c
# End Source File
# Begin Source File

SOURCE=..\kexgex.c
# End Source File
# Begin Source File

SOURCE=..\kexgexc.c
# End Source File
# Begin Source File

SOURCE=..\key.c
# End Source File
# Begin Source File

SOURCE=..\log.c
# End Source File
# Begin Source File

SOURCE=..\mac.c
# End Source File
# Begin Source File

SOURCE=..\match.c
# End Source File
# Begin Source File

SOURCE=..\misc.c
# End Source File
# Begin Source File

SOURCE=..\mktemp.c
# End Source File
# Begin Source File

SOURCE=..\moduli.c
# End Source File
# Begin Source File

SOURCE=..\monitor_fdpass.c
# End Source File
# Begin Source File

SOURCE=..\msg.c
# End Source File
# Begin Source File

SOURCE=..\nchan.c
# End Source File
# Begin Source File

SOURCE=..\dm2\options.c
# End Source File
# Begin Source File

SOURCE=..\packet.c
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-win32.c"
# End Source File
# Begin Source File

SOURCE=..\progressmeter.c
# End Source File
# Begin Source File

SOURCE=..\readconf.c
# End Source File
# Begin Source File

SOURCE=..\readpass.c
# End Source File
# Begin Source File

SOURCE=..\readpassphrase.c
# End Source File
# Begin Source File

SOURCE=..\rijndael.c
# End Source File
# Begin Source File

SOURCE=..\rsa.c
# End Source File
# Begin Source File

SOURCE="..\scard-opensc.c"
# End Source File
# Begin Source File

SOURCE=..\scard.c
# End Source File
# Begin Source File

SOURCE="..\ssh-dss.c"
# End Source File
# Begin Source File

SOURCE="..\ssh-rsa.c"
# End Source File
# Begin Source File

SOURCE=..\ssh.cpp
# End Source File
# Begin Source File

SOURCE=..\sshconnect.c
# End Source File
# Begin Source File

SOURCE=..\sshconnect1.c
# End Source File
# Begin Source File

SOURCE=..\sshconnect2.c
# End Source File
# Begin Source File

SOURCE=..\sshtty.c
# End Source File
# Begin Source File

SOURCE=..\strsep.c
# End Source File
# Begin Source File

SOURCE=..\strtonum.c
# End Source File
# Begin Source File

SOURCE=..\ttymodes.c
# End Source File
# Begin Source File

SOURCE=..\uidswap.c
# End Source File
# Begin Source File

SOURCE=..\uuencode.c
# End Source File
# Begin Source File

SOURCE=..\vis.c
# End Source File
# Begin Source File

SOURCE=..\winstore.cpp
# End Source File
# Begin Source File

SOURCE=..\xmalloc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\acss.h
# End Source File
# Begin Source File

SOURCE=..\atomicio.h
# End Source File
# Begin Source File

SOURCE=..\authfd.h
# End Source File
# Begin Source File

SOURCE=..\authfile.h
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\base64.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\bsd-cray.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\bsd-cygwin_util.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\bsd-misc.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\bsd-nextstep.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\bsd-waitpid.h"
# End Source File
# Begin Source File

SOURCE=..\bufaux.h
# End Source File
# Begin Source File

SOURCE=..\buffer.h
# End Source File
# Begin Source File

SOURCE=..\channels.h
# End Source File
# Begin Source File

SOURCE=..\cipher.h
# End Source File
# Begin Source File

SOURCE=..\compat.h
# End Source File
# Begin Source File

SOURCE=..\config.h
# End Source File
# Begin Source File

SOURCE=..\crc32.h
# End Source File
# Begin Source File

SOURCE=..\openssl\crypto.h
# End Source File
# Begin Source File

SOURCE=..\deattack.h
# End Source File
# Begin Source File

SOURCE=..\defines.h
# End Source File
# Begin Source File

SOURCE=..\dispatch.h
# End Source File
# Begin Source File

SOURCE=..\entropy.h
# End Source File
# Begin Source File

SOURCE=..\getput.h
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\getrrsetbyname.h"
# End Source File
# Begin Source File

SOURCE=..\includes.h
# End Source File
# Begin Source File

SOURCE=..\kex.h
# End Source File
# Begin Source File

SOURCE=..\key.h
# End Source File
# Begin Source File

SOURCE=..\log.h
# End Source File
# Begin Source File

SOURCE=..\mac.h
# End Source File
# Begin Source File

SOURCE=..\match.h
# End Source File
# Begin Source File

SOURCE=..\misc.h
# End Source File
# Begin Source File

SOURCE=..\msg.h
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\openbsd-compat.h"
# End Source File
# Begin Source File

SOURCE=..\openssl\opensslv.h
# End Source File
# Begin Source File

SOURCE=..\packet.h
# End Source File
# Begin Source File

SOURCE=..\pathnames.h
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-aix.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-irix.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-tun.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-uw.h"
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\port-win32.h"
# End Source File
# Begin Source File

SOURCE=..\readconf.h
# End Source File
# Begin Source File

SOURCE="..\openbsd-compat\readpassphrase.h"
# End Source File
# Begin Source File

SOURCE=..\rijndael.h
# End Source File
# Begin Source File

SOURCE=..\ssh.h
# End Source File
# Begin Source File

SOURCE=..\ssh1.h
# End Source File
# Begin Source File

SOURCE=..\ssh2.h
# End Source File
# Begin Source File

SOURCE=..\storage.h
# End Source File
# Begin Source File

SOURCE=..\uuencode.h
# End Source File
# Begin Source File

SOURCE=..\version.h
# End Source File
# Begin Source File

SOURCE=..\xmalloc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
