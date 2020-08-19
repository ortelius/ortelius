#!/usr/bin/python3

import sys
import tempfile
import os
import shutil
import subprocess
import socket


def main():
    """Main entry point"""
    envvars = {}

    with open(sys.argv[1], "r") as fp_env:
        line = fp_env.readline()
        while line:
            line = line.strip()
            if (line):
                key, val = line.split(':', 1)
                key = key.strip()
                key = key.replace('.', '_')
                val = val.strip()
                val = val.strip('"')
                envvars[key] = val
            line = fp_env.readline()

    os.unlink(sys.argv[1])
    target_os = envvars.get('targetOS', 'linux')

    localconnection = ""
    if ('server_hostname' in envvars):
        hname = envvars['server_hostname']
        envvars['server_ip'] = socket.gethostbyname(hname)

        if (hname == "localhost"):
            envvars['server_ip'] = "127.0.0.1"
            localconnection = " --connection=local"

    tempdir = tempfile.mkdtemp()
    os.chdir(tempdir)

    os.mkdir('group_vars')
    fp_all = open('group_vars/all.yml', 'w')
    fp_all.write("---\n")
    fp_all.write("ansible_user: " + envvars['sshuser'] + "\n")
    fp_all.write("ansible_password: " + envvars['sshpass'] + "\n")

    if ('suuser' in envvars):
        fp_all.write("ansible_become: yes\n")
        fp_all.write("ansible_become_method: su\n")
        fp_all.write("ansible_become_user: " + envvars['suuser'] + "\n")

    if ('supass' in envvars):
        fp_all.write("ansible_become_pass: " + envvars['supass'] + "\n")

    if (target_os == 'windows'):
        fp_all.write("ansible_port: '5985'\n")
        fp_all.write("ansible_connection: winrm\n")
        fp_all.write("ansible_winrm_transport: credssp\n")
        fp_all.write("ansible_winrm_server_cert_validation: ignore\n")
        fp_all.write("validate_certs: false\n")

    for k in sorted(envvars.keys()):
        if (('?' not in k) and ('$' not in envvars[k])):
            fp_all.write(k + ': ' + envvars[k] + "\n")

    fp_all.close()

    fp_run = open('runit.yml', 'w')
    fp_run.write("---\n")

    fp_run.write("\n")

    dropzone = envvars.get('dropzone', '')
    taskfile = envvars.get('taskfile', '')

    tfile = taskfile.strip()

    if ('http' in tfile):
        tasks = subprocess.run(['curl', '-sL', '-o', '-', tfile], stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
    else:
        tfile = dropzone.strip() + "/" + taskfile.strip()
        tasks = subprocess.run(['cat', tfile], stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")

    for line in tasks:
        fp_run.write(line + "\n")

    fp_run.close()

    my_env = os.environ.copy()
    # my_env['ANSIBLE_STDOUT_CALLBACK'] = 'minimal'
    pid = subprocess.Popen('ansible-playbook runit.yml --ssh-common-args="-o StrictHostKeyChecking=no" ' + localconnection + ' -i ' +
                           envvars['server_hostname'] + ',', env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in pid.stdout.readlines():
        print(line.decode('utf-8').strip('\n'))

    pid.wait()

    os.chdir('/tmp')
    if ('debug_ansible' in envvars):
        print(tempdir)
    else:
        shutil.rmtree(tempdir)

    exit(pid.returncode)


if __name__ == "__main__":
    main()
