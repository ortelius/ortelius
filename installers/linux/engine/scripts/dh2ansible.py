#!/usr/bin/python3

from pprint import pprint
import sys
import tempfile
import os
import shutil
import subprocess
import socket


def main():
    """Main entry point"""
    targetOS = 'linux'
    user = ''
    pw = ''
    taskfile = ''
    servers = ''
    su = ''
    supw = ''
    envvars = {}

    with open(sys.argv[1],"r") as fp:
     line = fp.readline()
     while line:
       line = line.strip()
       if (line):
         key, val = line.split(':',1)  
         envvars[key] = val
       line = fp.readline() 

    if (envvars['server_hostname']):
      hname = envvars['server_hostname'].strip()
      envvars['server_ip'] = socket.gethostbyname(hname)

    tempdir = tempfile.mkdtemp()
    os.chdir(tempdir)

    os.mkdir('group_vars')
    fp = open('group_vars/all.yml', 'w')
    fp.write("---\n")
    fp.write("ansible_user: " + envvars['sshuser'] + "\n")
    fp.write("ansible_password: " + envvars['sshpass'] + "\n")

    if (envvars['suuser']):
        fp.write("ansible_become: yes\n")
        fp.write("ansible_become_method: su\n")
        fp.write("ansible_become_user: " + envvars['suuser'] + "\n")

    if (envvars['supass']):
        fp.write("ansible_become_pass: " + envvars['supass'] + "\n")

    if (targetOS == 'windows'):
        fp.write("ansible_port: '5985'\n")
        fp.write("ansible_connection: winrm\n")
        fp.write("ansible_winrm_transport: credssp\n")
        fp.write("ansible_winrm_server_cert_validation: ignore\n")
        fp.write("validate_certs: false\n")

    for k in sorted(envvars.keys()):
      if (('?' not in k) and ('$' not in envvars[k])):
         fp.write(k + ': ' + envvars[k] + "\n")

    fp.close()

    fp = open('runit.yml', 'w')
    fp.write("---\n")

    fp.write("\n")
    dropzone = envvars[ 'dropzone']
    taskfile = envvars['taskfile']
    tfile = dropzone.strip() + "/" + taskfile.strip()
    tasks = subprocess.run(['cat',tfile], stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")

    for line in tasks:
       fp.write(line +"\n")

    fp.close()

    my_env = os.environ.copy()
    # my_env['ANSIBLE_STDOUT_CALLBACK'] = 'minimal'
    p = subprocess.Popen('ansible-playbook runit.yml -i ' + envvars['server_hostname'] + ',', env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    json_str = ""
    for line in p.stdout.readlines():
        print(line.decode('ascii').strip('\n'))

    p.wait()

    os.chdir('/tmp')
    if ('debug_ansible' in envvars):
      print(tempdir)
    else:
      shutil.rmtree(tempdir)

    exit(p.returncode)


if __name__ == "__main__":
    main()
