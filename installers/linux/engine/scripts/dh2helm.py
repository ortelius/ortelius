#!/usr/bin/python3

# Copyright (c) 2021 Linux Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import base64
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time

import qtoml
import yaml
from deployhub import dhapi

#from pprint import pprint


def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))


def upload_helm(override, chartvalues, newvals, helmtemplate, gitdir):
    my_env = os.environ.copy()

    os.makedirs(os.path.dirname("helm/" + chartvalues), exist_ok=True)
    shutil.copy(override, "helm/" + chartvalues)

    my_file = open("helm/" + chartvalues, "r")
    content_list = my_file.readlines()
    my_file.close()
    content_list = list(filter(lambda x: 'pwd' not in x, content_list))
    content_list = list(filter(lambda x: 'pass' not in x, content_list))
    content_list = list(filter(lambda x: 'userid' not in x, content_list))
    content_list = list(filter(lambda x: 'username' not in x, content_list))
    content_list = list(filter(lambda x: 'aws_access_key_id' not in x, content_list))
    content_list = list(filter(lambda x: 'aws_secret_access_key' not in x, content_list))
    content_list = list(filter(lambda x: 'serviceprincipal' not in x, content_list))
    content_list = list(filter(lambda x: 'tenant' not in x, content_list))

    my_file = open("helm/" + chartvalues, "w")
    my_file.writelines(content_list)
    my_file.close()

    dhurl = newvals.get('dhurl', '')
    dhusr = newvals.get('dhuser', '')
    dhpw = newvals.get('dhpass', '')

    repovars = newvals.get('helmrepo', {})

    if ('helmrepouser' in newvals):
        helmrepouser = newvals['helmrepouser']
    else:
        helmrepouser = repovars.get('username', '')

    if ('helmrepopass' in newvals):
        helmrepopass = newvals['helmrepopass']
    else:
        helmrepopass = repovars.get('password', '')

    deployid = newvals.get('dropzone', "")
    deployid = deployid.split('.')[1]

    upload = {}
    upload['files'] = []
    upload['deployid'] = deployid
    upload['component'] = newvals.get('component', '')
    upload['helmrepo'] = repovars.get('name', '')
    upload['helmrepourl'] = repovars.get('url', '')

    chart = newvals.get('chart', '')

    if ('/' not in chart):
        chart = 'library/' + chart

    upload['chartorg'] = chart.split('/')[0]
    upload['chartname'] = chart.split('/')[1]
    upload['chartversion'] = newvals.get('chartversion', '')

    my_env['helmrepouser'] = helmrepouser
    my_env['helmrepopass'] = helmrepopass
    my_env['dockeruser'] = newvals.get('dockeruser', helmrepouser)
    my_env['dockerpass'] = newvals.get('dockerpass', helmrepopass)
    my_env['helmrepo'] = upload['helmrepo']
    my_env['helmrepourl'] = upload['helmrepourl']
    my_env['chartorg'] = upload['chartorg']
    my_env['chartname'] = upload['chartname']
    my_env['chartversion'] = upload['chartversion']
    my_env['chartvalues'] = chartvalues
    my_env['helmopts'] = newvals.get('helmopts', '')
    my_env['helmtemplate'] = helmtemplate
    my_env['environment'] = newvals.get('environment', 'ci')
    my_env['component'] = newvals.get('component','')

    if (newvals.get('helmcapture', None) is None):
        return

    if ('helmrepo' not in newvals):
        return

    os.chdir('helm')
    print("Starting Helm Capture")

    pid = subprocess.Popen(get_script_path() + "/helminfo.sh", env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    jstr = ""
    for line in pid.stdout.readlines():
        line = line.decode('utf-8')
        jstr = jstr + line
    pid.wait()

    # print(jstr)
    dobj = json.loads(jstr)
    upload['chartdigest'] = dobj.get("chartdigest", "")
    upload['images'] = dobj.get("images", [])

    start_dir = "."

    filelist = []
    for root, d_names, f_names in os.walk(start_dir):  # pylint: disable=W0612
        for fname in f_names:
            if ('.DS_Store' not in fname):
                filelist.append(os.path.join(root, fname))

    filelist.sort()

    for fname in filelist:
        contents = {}
        contents['filename'] = fname

        file1 = open(fname, "rb")
        data = file1.read()
        file1.close()

        # second: base64 encode read data
        # result: bytes (again)
        base64_bytes = base64.b64encode(data)

        # third: decode these bytes to text
        # result: string (in utf-8)
        base64_string = base64_bytes.decode("utf-8")

        contents['data'] = base64_string
        upload['files'].append(contents)

    errors = []

    cookies = dhapi.login(dhurl, dhusr, dhpw, errors)
    # pprint(errors)
    dhapi.post_json(dhurl + "/dmadminweb/API/uploadhelm", json.dumps(upload), cookies)
    print("Finished Helm Capture")


def runcmd(fp_task, workdir, cmd):
    tname = cmd
    if ('--install' in cmd):
        tname = cmd.split('--install')[0]
    elif ('aws_access_key_id' in cmd):
        tname = cmd.split('aws_access_key_id')[0] + ' aws_access_key_id'
    elif ('aws_secret_access_key' in cmd):
        tname = cmd.split('aws_secret_access_key')[0] + ' aws_secret_access_key'
    elif ('--service-principal' in cmd):
        tname = cmd.split('--service-principal')[0] + ' --service-principal'

    fp_task.write("  - name: " + tname + "\n")
    fp_task.write("    shell: " + cmd + " 2>&1 \n")
    fp_task.write("    args:\n")
    fp_task.write("      chdir: " + workdir + "\n")
    fp_task.write("    changed_when: False\n")
    fp_task.write("    register: cat\n")
    fp_task.write("  - name: Print Output\n")
    fp_task.write("    debug: var=cat.stdout_lines\n")
    fp_task.write("\n")


def main():
    """Main entry point <Chart Directory> <Override Values Toml>"""

    newvals = {}
    releasename = sys.argv[1]
    chart = sys.argv[2]

    if (not releasename or not chart):
        return 0

    chartvalues = "override/" + sys.argv[2] + "/values.yaml"
    rspfile = sys.argv[3]

    if (';' in releasename):
        releasename = releasename.split(';')[0]

    releasename = releasename.replace(';', '-v').replace('_', '-').lower()

    if (os.path.exists(chartvalues)):
        stream = open(chartvalues, 'r')
        values = yaml.load(stream)
        newvals.update(values)
        stream.close()

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

            if ('?' not in key):
                cleanvals[key] = value

    valstr = ""
    for key, value in cleanvals.items():
        valstr = valstr + key + " = " + value + "\n"

    values = qtoml.loads(valstr)
    newvals.update(values)

    if (os.path.exists(chartvalues)):
        os.rename(chartvalues, chartvalues + ".bak")

    if not os.path.exists(os.path.dirname(chartvalues)):
        os.makedirs(os.path.dirname(chartvalues))

    stream = open(chartvalues, "w")
    yaml.dump(newvals, stream)
    stream.close()
    override = os.path.abspath(chartvalues)

    # Use $component name to derive chartversion <component>;<variant>;<version>
    # version = v<schematic>_g<git commit>
    component = newvals.get('component','')
    parts=component.split(';')[-1].split('_g')
    clean_ver=re.sub(r'^v|^V|^v_|^V_','', parts[0])
    clean_ver=re.sub(r'_','.', clean_ver)
    parts=clean_ver.split('.')

    if (len(parts) <= 3):
        derived_chartversion = clean_ver
    else:
        derived_chartversion = '.'.join(parts[:3]) + '-build.' + '.'.join(parts[3:])

    if (newvals.get('chartversion', None) is not None):
      derived_chartversion = newvals.get('chartversion')

    if (os.path.exists(chart + "/Chart.yaml")):
        my_file = open(chart + "/Chart.yaml", "r")
        content_list = my_file.readlines()
        my_file.close()
        content_list = list(filter(lambda x: 'Version' not in x, content_list))
        content_list = list(filter(lambda x: 'version' not in x, content_list))
        content_list.append('version: "' + derived_chartversion + '"')

        my_file = open(chart + "/Chart.yaml", "w")
        my_file.writelines(content_list)
        my_file.close()

    timestamp = int(time.time()*1000.0)
    to_dir = "/tmp/dh" + str(timestamp)
    from_dir = os.getcwd()
    tempdir = tempfile.mkdtemp()
    os.chdir(tempdir)

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
    fp_task.write("  - name: Sync dropzone to work area\n")
    fp_task.write("    synchronize:\n")
    fp_task.write("      src: " + from_dir + "/\n")
    fp_task.write("      dest: " + to_dir + "\n")
    fp_task.write("\n")

    # pprint(newvals)
    if ('gcloud' in newvals):
        if ('compute' in newvals['gcloud'] and 'zone' in newvals['gcloud']['compute']):
            runcmd(fp_task, to_dir, 'gcloud config set compute/zone ' + newvals['gcloud']['compute']['zone'])

        if ('container' in newvals['gcloud'] and 'cluster' in newvals['gcloud']['container']):
            runcmd(fp_task, to_dir, 'gcloud config set container/cluster ' + newvals['gcloud']['container']['cluster'])

        if ('core' in newvals['gcloud'] and 'account' in newvals['gcloud']['core']):
            runcmd(fp_task, to_dir, 'gcloud config set core/account ' + newvals['gcloud']['core']['account'])

        if ('core' in newvals['gcloud'] and 'disable_usage_reporting' in newvals['gcloud']['core']):
            runcmd(fp_task, to_dir, 'gcloud config set core/disable_usage_reporting ' + newvals['gcloud']['core']['disable_usage_reporting'])

        if ('core' in newvals['gcloud'] and 'project' in newvals['gcloud']['core']):
            runcmd(fp_task, to_dir, 'gcloud config set core/project ' + newvals['gcloud']['core']['project'])

        if ('oauth' in newvals['gcloud'] and 'account' in newvals['gcloud']['oauth']):
            runcmd(fp_task, to_dir, 'gcloud auth activate-service-account ' + newvals['gcloud']['oauth']['account'] + ' --key-file=' + newvals['gcloud']['oauth']['keyfile'])

        if ('container' in newvals['gcloud'] and 'cluster' in newvals['gcloud']['container']):
            runcmd(fp_task, to_dir, 'gcloud container clusters get-credentials ' + newvals['gcloud']['container']['cluster'])

    if ('eks' in newvals):
        if ('aws_access_key_id' in newvals['eks']):
            runcmd(fp_task, to_dir, 'aws --profile default configure set aws_access_key_id ' + newvals['eks']['aws_access_key_id'])

        if ('aws_secret_access_key' in newvals['eks']):
            runcmd(fp_task, to_dir, 'aws --profile default configure set aws_secret_access_key ' + newvals['eks']['aws_secret_access_key'])

        if ('cluster' in newvals['eks'] and 'region' in newvals['eks']):
            region = newvals['eks']['region']
            cluster = newvals['eks']['cluster']
            optional = newvals['eks'].get('optional', '')
            runcmd(fp_task, to_dir, 'aws eks --region ' + region + ' update-kubeconfig --name ' + cluster + ' ' + optional)

    if ('aks' in newvals):
        if ('serviceprincipal' in newvals['aks'] and 'certificate' in newvals['aks'] and 'tenant' in newvals['aks']):
            serviceprincipal = newvals['aks']['serviceprincipal']
            certificate = newvals['aks']['certificate']
            tenant = newvals['aks']['tenant']
            runcmd(fp_task, to_dir, 'az login --service-principal -u ' + serviceprincipal + ' -p ' + certificate + ' --tenant ' + tenant)

        if ('cluster' in newvals['aks'] and 'resourcegroup' in newvals['aks']):
            resourcegroup = newvals['aks']['resourcegroup']
            cluster = newvals['aks']['cluster']
            admin = ''
            if ('admin' in newvals['aks']):
                admin = ' --admin'
            runcmd(fp_task, to_dir, 'az aks get-credentials --resource-group ' + resourcegroup + ' --name ' + cluster + admin)

    if ('kubectl_context' in newvals):
        runcmd(fp_task, to_dir, 'kubectl config use-context ' + newvals['kubectl_context'])

    helm_exe = newvals.get("helm_exe", "helm")
    if (helm_exe.lower() == "helm2"):
        runcmd(fp_task, to_dir, helm_exe + ' init --client-only --upgrade')

    runcmd(fp_task, to_dir, 'cat ' + to_dir + "/" + chartvalues + " | grep -v pass | grep -v ssh | grep -v aws_access_key_id | grep -v aws_secret_access_key | grep -v serviceprincipal | grep -v tenant")

    if ('helmrepo' in newvals and 'url' in newvals['helmrepo']):
        mylogin = " "

        if ('username' in newvals['helmrepo']):
            mylogin = mylogin + "--username " + newvals['helmrepo']['username'] + " "

        if ('password' in newvals['helmrepo']):
            mylogin = mylogin + "--password " + newvals['helmrepo']['password'] + " "

        if ('helmrepouser' in newvals):
            mylogin = mylogin + "--username " + newvals['helmrepouser'] + " "

        if ('helmrepopass' in newvals):
            mylogin = mylogin + "--password " + newvals['helmrepopass'] + " "

        if (helm_exe.lower() == "helm2"):
            runcmd(fp_task, to_dir, helm_exe + ' repo add ' + mylogin + newvals['helmrepo']['name'] + " " + newvals['helmrepo']['url'])
        else:
            runcmd(fp_task, to_dir, helm_exe + ' repo add --force-update ' + mylogin + newvals['helmrepo']['name'] + " " + newvals['helmrepo']['url'])
        # runcmd(fp_task, to_dir, helm_exe + ' repo add ' + mylogin + newvals['helmrepo']['name'] + " " + newvals['helmrepo']['url'])
        runcmd(fp_task, to_dir, helm_exe + ' repo update')

    version = newvals.get('chartversion', None)

    if (version is not None):
        version = '--version "' + version + '"'
    else:
        version = ''

    namespace = ""
    if ('chartnamespace' in newvals):
        namespace = '--namespace ' + newvals['chartnamespace']

    helmopts = newvals.get('helmopts', '')
    helmtemplateopts = newvals.get('helmtemplateopts', '')

#    helmextract = newvals.get('helmextract', None)
    if (newvals.get('helmrepo', None) is not None):
        runcmd(fp_task, to_dir, helm_exe + ' fetch "' + chart + '" ' + version + ' --untar')

    if (newvals.get('helmcapture', None) is not None):
        if ('/' in chart):
            chartname = '/'.join(chart.split('/')[1:])
        else:
            chartname = chart

        fp_task.write("  - name: helm template\n")
        fp_task.write('    shell: ' + helm_exe + ' template ' + chartname + ' ' + namespace + ' ' + helmtemplateopts + ' -f ' + chartvalues + "\n")
        fp_task.write("    args:\n")
        fp_task.write("      chdir: " + to_dir + "\n")
        fp_task.write("    changed_when: False\n")
        fp_task.write("    register: myshell_output\n")
        fp_task.write("  - name: copy the output to a local file\n")
        fp_task.write("    copy:\n")
        fp_task.write("      content: \"{{ myshell_output.stdout }}\"\n")
        fp_task.write("      dest: \"" + to_dir + ".yml\"\n")
        fp_task.write("    delegate_to: localhost\n")

    runcmd(fp_task, to_dir, helm_exe + ' upgrade "' + releasename + '" "' + chart + '" ' + version + ' ' + namespace + ' ' + helmopts + ' --install -f ' + chartvalues)

    fp_task.close()

    if (os.path.exists(rspfile)):
        os.remove(rspfile)

    my_env = os.environ.copy()
    my_env['ANSIBLE_STDOUT_CALLBACK'] = 'yaml'
    pid = subprocess.Popen('ansible-playbook runit.yml --ssh-common-args="-o StrictHostKeyChecking=no" ' + localconnection + ' -i ' +
                           newvals['server_hostname'] + ',', env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in pid.stdout.readlines():
        line = line.decode('utf-8').strip('\n')
        if ('ssh' not in line and 'pass' not in line):
            print(line)

    pid.wait()

    upload_helm(override, chartvalues, newvals, to_dir + ".yml", from_dir)

    os.chdir('/tmp')

    if ('debug_ansible' in newvals):
        print(tempdir)
    else:
        shutil.rmtree(tempdir)
        if (os.path.exists(to_dir + ".yml")):
           os.remove(to_dir + ".yml")

    exit(pid.returncode)


if __name__ == '__main__':
    main()
