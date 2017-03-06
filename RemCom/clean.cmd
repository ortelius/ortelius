@echo off

rmdir Debug /s /q
rmdir Release /s /q
rmdir RemComSvc\Debug /s /q
rmdir RemComSvc\Release /s /q

rmdir _ReSharper.Remote~1 /s/q

rmdir ProcComs\Debug /s /q
rmdir ProcComs\Release /s /q

del *.ncb /s
del *.opt /s
del *.plg /s
del *.aps /s
del *.clw /s
del *.positions /s
