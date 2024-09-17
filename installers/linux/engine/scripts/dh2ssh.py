#!/usr/bin/python3

import os
import re
import subprocess
import sys

import qtoml


def run_ssh(cmd):
    print(cmd)
    pid = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    retval = ""
    for line in pid.stdout.readlines():
        print(line.decode('utf-8').strip())


def main():
    """Main entry point <Override Values Toml>"""

    newvals = {}
    rspfile = sys.argv[1]

    print("RSP=" + rspfile)
    lines = subprocess.run(['cat', rspfile], check=False, stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")

    cleanvals = {}
    for line in lines:
        if ('=' in line):
            key = line.split('=')[0]
            value = line.split('=', 1)[-1].strip()
            if (value.lower() == '"true"' or value.lower() == "'true'"):
                value = "true"
            if (value.lower() == '"false"' or value.lower() == "'false'"):
                value = "false"

            if (key.lower() == 'dep.filelist'):
                continue

            if ('?' not in key):
                cleanvals[key] = value

    valstr = ""
    for key, value in cleanvals.items():
        valstr = valstr + key + " = " + value + "\n"

    values = qtoml.loads(valstr)
    newvals.update(values)

    # find server list for env and application

    appname = newvals.get('appname', '')
    envname = newvals.get('environment', '')

    appname = re.sub('[\\W_]+', '', appname)
    envname = re.sub('[\\W_]+', '', envname)

    parts = compname.split('.')
    projname = parts[-2]
    compname = parts[-1]
    compname = re.sub('[\\W_]+', '', compname)

    batchscriptext = newvals.get('batchscriptext', '.cmd')
    cmdname = newvals.get('batchscript', projname + '-' + compname + batchscriptext)

    projdict = newvals.get(projname, None)
    pprint(projdict)

    servers = None

    appdict = newvals.get(appname, None)

    if (appdict is not None):
        envdict = appdict.get(envname, None)

        if (envdict is not None):
            servers = envdict.get('servers', None)

    if (servers is None):
        print("No servers defined for key " + appname + "." + envname + ".servers")
        sys.exit(1)

    cmdname = newvals.get('batchscript', None)

    fullcmd = '/opt/deployhub/ext/scripts/' + envname + '/' + cmdname

    if (not os.path.exists(fullcmd)):
        fullcmd = '/opt/deployhub/ext/scripts/common/' + cmdname

        if (not os.path.exists(fullcmd)):
            print("Cannot find script " + '/opt/deployhub/ext/scripts/' + envname + '/' + cmdname + " or " + '/opt/deployhub/ext/scripts/common/' + cmdname)
            sys.exit(1)

    onremote = newvals.get('onremote', None)

    if (onremote is None):
        onremote = False
    else:
        onremote = True

    sshuser = newvals.get('sshuser', "administrator")
    servers = servers.split(",")

    for server in servers:
        server = server.strip()
        runcmd = fullcmd

        if (not onremote):
            if ('@' in server):
               run_ssh('ssh -o "StrictHostKeyChecking no" ' + server + ' "mkdir c:\\temp"')
               run_ssh('scp -o "StrictHostKeyChecking no" ' + fullcmd + ' ' + server + ':/c:/temp/' + cmdname)
            else:
               run_ssh('ssh -o "StrictHostKeyChecking no" ' + sshuser + '@' + server + ' "mkdir c:\\temp"')
               run_ssh('scp -o "StrictHostKeyChecking no" ' + fullcmd + ' ' + sshuser + '@' + server + ':/c:/temp/' + cmdname)
            runcmd = "c:/temp/" + cmdname

        runcmd = runcmd.replace("\\", "/")
        if ('@' in server):
           run_ssh('ssh -o "StrictHostKeyChecking no" ' + server + ' ' + runcmd)
        else:
           run_ssh('ssh -o "StrictHostKeyChecking no" ' + sshuser + '@' + server + ' ' + runcmd)

if __name__ == '__main__':
    main()
