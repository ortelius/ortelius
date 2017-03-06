#!/bin/sh
#broker=garfield
broker=nermal

machine=nermal
#machine=garfield
#machine=sun2

#protocol=
#protocol=ssh
protocol=ftp
#protocol=ftps

#rlogin="-usr harvest"
#rlogin="-usr AS400"
#rlogin="-prompt"
rlogin="-er AS400.dfo"
#auth="-pw harvest"
#auth="-pw h4rv3st"

#cmd="ls -l"
cmd="ls"
#cmd="CHGPFM FILE(DMDEVASRC/QRPGLESRC) MBR(TST001) TEXT('Money Management -Income & Expenditure (Prompt).') SRCTYPE(RPGLE)"
#cmd="DSPFD FILE(DMDEVASRC//QDDSSRC) TYPE(*MBR) OUTPUT(*OUTFILE) OUTFILE(QTEMP/RTI18532)"

echo rtoexecp -b ${broker} -m ${machine} -rp ${protocol} -syn ${rlogin} ${cmd}
./Debug/rtoexecp -b ${broker} -m ${machine} -rp ${protocol} -syn ${rlogin} ${cmd}
