Welcome to the Deploy+ 8.0 Engine
---------------------------------

Your deployment engine has now been installed. It should be running
as a service called "DeployEngine".

The engine works by accepting requests from the web interface. The
port on which it listens for incoming requests was stipulated
during the install. You can also see (and change) the port number
by opening the trilogy.conf file in the engine's installation
directory.

A TCP connection is made to this port when the web interface needs
the engine to perform an action (such as approving, moving or
deploying an application). You should make sure that there are no
firewall rules preventing this connection.

The engine needs an ODBC connection to the Deploy+ 8.0 database.
You should ensure that the ODBC DSN is configured. Note, if you have
installed the 32-bit installer on a 64-bit Windows instance then
you HAVE to install the relevant 32-bit database driver and configure
the 32-bit ODBC connection. (Use the odbcad32.exe in SysWOW64).

The Datasource Name and the credentials used for connection are
encrypted in the dm.odbc file in the engine's installation
directory. You can change the content of this file by running
the "setodbc.exe" program in the engine's "bin" directory.

Scheduler
---------

The engine connects to the Deploy+ 8.0 database every 15 minutes
to see if a scheduled task needs to be run. These scheduled tasks
are set in the target environment's "Calendar" in the web interface.

Deployments
-----------

The engine will deploy over open protocols to allow agentless
file transfer and script execution. The protocols are:

File Transfer
-------------
FTP
FTPS (FTP over Secure Sockets Layer)
SFTP (File transfer over Secure Shell)
Windows Share

Remote Script Execution
-----------------------
FTP/FTPS (where "exec" command is supported, e.g.: iSeries)
SSH (Secure Shell)
Windows Admin

Windows and Domains
-------------------
In order to connect seamlessly, the engine should be installed on a
windows server which is in the same windows domain as the target servers.
If this is not the case then any target server after Windows Server 2003
will not allow the engine to connect to its ADMIN$ share.

You can work around this issue by installing multiple engines (for
example one to deploy to test servers and another to deploy into
production). The Web UI will determine which engine to use based on
its deployment domain.

The target servers can also be set to accept connections to their ADMIN$ shares
(regardless of the engine's domain) by making a change to the following registry key
on each target server:

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System\LocalAccountTokenFilterPolicy = 1


Remote script execution works by connecting to the ADMIN$ share and pushing a small
program which is started as a service using remote WMI interface. This service then
accepts incoming requests to run jobs on the remote server. When the job completes
the service shuts down and uninstalls itself automatically.
