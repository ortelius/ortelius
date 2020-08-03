Files that make up Release Engineer

dmadminweb - web UI
dmdatabase - alter scripts and master database schema creation script
dmengine   - Deployment Manager engine - calls dmtransfer for file transfer
dmtransfer - file transfer engine. Loads appropriate library at runtime dependent on selected protocol
ftplib     - FTP transfer library. Loaded at runtime by dmtransfer if protocol is FTP/FTPS
psftplib   - SSH transfer library. Loaded at runtime by dmtransfer if protocol is SFTP
winlib     - Windows transfer library. Loaded at runtime by dmtransfer if protocol is WIN
RemCom     - Remote Command Executor for Windows. Called by dmtransfer to run scripts on remote servers
             when the protocol is WIN (*)
RTOEXECP2  - Remote Command Executor for SSH. Called by dmtransfer to run scripts on remote servers when the
             protocol is SFTP (*)

Important Notice
----------------
(*) Both RemCom and RTOEXECP2 are currently built with Visual Studio 6.0.
    In the case of RemCom this is in order to link against early versions of the C Runtime Library (CRT).
    This is because RemCom contains a "service" program which is pushed onto the remote server - this needs
    to be linked against versions of the C Runtime that ship with Windows versions such as 2003 and XP in
    order to ensure that the service works and we are truly "agentless".

