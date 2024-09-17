#!/usr/bin/python3

from pprint import pprint
import sys
import tempfile
import os
import shutil
import subprocess

def main():

    for arg in sys.argv[1:]:
        if ("--user=" in arg):
          user = arg.split('=')[1]
        elif ("--pass=" in arg):
          pw=arg.split('=')[1]
        elif ("--from=" in arg):
          fromdir=arg.split('=')[1]
        elif ("--to=" in arg):
          todir=arg.split('=')[1]
        elif ("--share=" in arg):
          share=arg.split('=')[1]
        elif ("--host=" in arg):
          host=arg.split('=')[1]
        if ("--shareuser=" in arg):
          shareuser = arg.split('=')[1]
        elif ("--sharepass=" in arg):
          sharepass = arg.split('=')[1]

    tempdir = tempfile.mkdtemp()
    os.chdir(tempdir)
    print(tempdir)

    if (len(shareuser) == 0):
      shareuser = user

    if (len(sharepass) == 0):
      sharepass = pw

    fp = open("robocp.cmd","w")
    fp.write("net use " + share + " /user:" + shareuser + " " + sharepass + "\n")
    fp.write("robocopy " + fromdir + " " + todir + " /MIR /R:0 /W:0\n")
    fp.write("net use " + share + " /d\n")
    fp.close()

    fp = open('hosts','w')
    fp.write("[windows]\n")
    fp.write(host + "\n")
    fp.close()

    os.mkdir('group_vars')

    fp = open('group_vars/windows.yml','w')
    fp.write("ansible_user: '" + user + "'\n")
    fp.write("ansible_password: '" + pw + "'\n")
    fp.write("ansible_port: '5985'\n")
    fp.write("ansible_connection: winrm\n")
    fp.write("ansible_winrm_transport: credssp\n")
    fp.write("ansible_winrm_server_cert_validation: ignore\n")
    fp.write("validate_certs: false\n")
    fp.close()

    fp = open('runit.yml','w')
    fp.write("---\n")
    fp.write("- name: robocopy\n")
    fp.write("  hosts: all\n")
    fp.write("  gather_facts: no\n")
    fp.write("\n")
    fp.write("  tasks:\n")
    fp.write("  - name: copy cmd\n")
    fp.write("    win_copy:\n")
    fp.write("      src: robocp.cmd\n")
    fp.write("      dest: c:\\temp\\robocp.cmd\n")
    fp.write("      force: yes\n")
    fp.write("\n")
    fp.write("  - name: Exe Copy\n")
    fp.write("    win_command: c:\\temp\\robocp.cmd\n")
    fp.write("    register: cat\n")
    fp.write("    changed_when: False\n")
    fp.write("  - name: Print Output\n")
    fp.write("    debug: var=cat.stdout_lines\n")
    fp.close()

    my_env = os.environ.copy()
    my_env['ANSIBLE_STDOUT_CALLBACK'] = 'minimal'
    p = subprocess.Popen('ansible-playbook runit.yml -i hosts', env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in p.stdout.readlines():
        print(line.decode('uft-8'))

    p.wait()

    os.chdir('/tmp')
#    shutil.rmtree(tempdir)

if __name__ == "__main__":
    main()
