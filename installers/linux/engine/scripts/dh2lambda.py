#!/usr/bin/python3

import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from pathlib import Path

from zipfile import ZipFile
import qtoml
import yaml

#from pprint import pprint

def prefix_param(prefix, val):
    if (val is None):
        return ""
    else:
        return prefix + " " + val


def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))


def runcmd(fp_task, workdir, cmd):
    tname = cmd
    if ('--install' in cmd):
        tname = cmd.split('--install')[0]

    fp_task.write("  - name: " + tname + "\n")
    fp_task.write("    shell: " + cmd + "\n")
    fp_task.write("    args:\n")
    fp_task.write("      chdir: " + workdir + "\n")
    fp_task.write("    changed_when: False\n")
    fp_task.write("    register: cat\n")
    fp_task.write("  - name: Print Output\n")
    fp_task.write("    debug: var=cat.stdout_lines\n")
    fp_task.write("\n")


def main():
    """Main entry point <Function Directory> <Override Values Toml>"""

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

    print(valstr)
    values = qtoml.loads(valstr)
    newvals.update(values)

    timestamp = int(time.time()*1000.0)
    to_dir = "/tmp/dh" + str(timestamp)
    from_dir = os.getcwd()
    tempdir = tempfile.mkdtemp()

    if ('lambda' not in newvals):
        print("lamba variables not defined\n")
        return

    funcname = newvals['lambda'].get('functionname', None)
    if (funcname is not None):
        funcname = funcname.strip()

    funcfile = newvals['lambda'].get('functionfile', None)
    arcname = Path(funcfile).name
    if (funcfile is not None):
        with ZipFile(tempdir + "/" + funcname + '.zip', 'w') as myzip:
            myzip.write(filename=funcfile, arcname=arcname)
        funcfile = "--zip-file fileb://" + tempdir + "/" + funcname + '.zip'
    else:
        funcfile = ""

    localconnection = ""
    if ('server_hostname' in newvals):
        hname = newvals['server_hostname']
        newvals['server_ip'] = socket.gethostbyname(hname)

        if (hname == "localhost"):
            newvals['server_ip'] = "127.0.0.1"
            localconnection = " --connection=local"

    os.mkdir('group_vars')
    fp_all = open('group_vars/all.yml', 'w')
    fp_all.write("---\n")

    if ('sshuser' in newvals):
        fp_all.write("ansible_user: " + newvals['sshuser'] + "\n")

    if ('sshpass' in newvals):
        fp_all.write("ansible_password: " + newvals['sshpass'] + "\n")

    if ('sshkeyfile' in newvals):
        fp_all.write("ansible_ssh_private_key_file: " + newvals['sshkeyfile'] + "\n")

    if ('suuser' in newvals):
        fp_all.write("ansible_become: yes\n")
        fp_all.write("ansible_become_method: su\n")
        fp_all.write("ansible_become_user: " + newvals['suuser'] + "\n")

    if ('supass' in newvals):
        fp_all.write("ansible_become_pass: " + newvals['supass'] + "\n")

    fp_all.write("validate_certs: false\n")
    yaml.dump(newvals, fp_all)
    fp_all.close()

    fp_task = open('runit.yml', 'w')
    fp_task.write("---\n")
    fp_task.write("- name: Transfer File\n")
    fp_task.write("  hosts: all\n")
    fp_task.write("  gather_facts: no\n")
    fp_task.write("\n")
    fp_task.write("  tasks:\n")
    fp_task.write("  - name: Make Dir\n")
    fp_task.write("    file:\n")
    fp_task.write("      path: " + to_dir + "\n")
    fp_task.write("      state: directory\n")
    fp_task.write("\n")
    fp_task.write("  - name: Copy File\n")
    fp_task.write("    copy:\n")
    fp_task.write("      src: " + tempdir + "/" + funcname + ".zip\n")
    fp_task.write("      dest: " + to_dir + "\n")
    fp_task.write("\n")

    if ('aws' in newvals):
        if ('aws_access_key_id' in newvals['aws']):
            runcmd(fp_task, to_dir, 'aws --profile default configure set aws_access_key_id ' + newvals['aws']['aws_access_key_id'])

        if ('region' in newvals['aws']):
            runcmd(fp_task, to_dir, 'aws --profile default configure set region ' + newvals['aws']['region'])

        if ('aws_secret_access_key' in newvals['aws']):
            runcmd(fp_task, to_dir, 'aws --profile default configure set aws_secret_access_key ' + newvals['aws']['aws_secret_access_key'])

    s3bucket = prefix_param("--s3-bucket", newvals['lambda'].get('s3bucket', None))
    s3key = prefix_param("--s3-key", newvals['lambda'].get('s3key', None))
    s3objectversion = prefix_param("--s3-object-version", newvals['lambda'].get('s3objectversion', None))
    publish = newvals['lambda'].get('publish', '--publish')
    revisionid = prefix_param("--revision-id", newvals['lambda'].get('revisionid', None))
    funcname = prefix_param("--function-name", funcname)

    cmd = "aws lambda update-function-code " + funcname + " " + funcfile + " " + s3bucket + " " + s3key + " " + s3objectversion + " " + publish + " " + revisionid
    runcmd(fp_task, to_dir, cmd)
    fp_task.close()

    if (os.path.exists(rspfile)):
        os.remove(rspfile)

    print(tempdir)
    my_env = os.environ.copy()
    my_env['ANSIBLE_STDOUT_CALLBACK'] = 'yaml'
    pid = subprocess.Popen('ansible-playbook runit.yml --ssh-common-args="-o StrictHostKeyChecking=no" ' + localconnection + ' -i ' +
                           newvals['server_hostname'] + ',', env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in pid.stdout.readlines():
        line = line.decode('utf-8').strip('\n')
        if ('ssh' not in line and 'pass' not in line):
            print(line)

    pid.wait()
    os.chdir('/tmp')

    if ('debug_ansible' in newvals):
        print(tempdir)
    else:
        shutil.rmtree(tempdir)

    exit(pid.returncode)


if __name__ == '__main__':
    main()
