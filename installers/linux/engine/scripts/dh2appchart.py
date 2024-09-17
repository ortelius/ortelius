#!/usr/bin/python3

import json
import os
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import qtoml
import yaml


def flatten_dict(dd, separator ='_', prefix =''):
    return { prefix + separator + k if prefix else k : v
             for kk, vv in dd.items()
             for k, v in flatten_dict(vv, separator, kk).items()
             } if isinstance(dd, dict) else { prefix : dd }

def run_git(cmd):
    pid = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    retval = ""
    for line in pid.stdout.readlines():
        print(line.decode('utf-8').strip())

def update_from_newvals(newvals, k_values):
    for key, value in newvals.items():
        yield key
        if isinstance(value, dict):
            yield from update_from_newvals(value, k_values)

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

    # flatten to make the lookup eaiser
    flat_newvals = flatten_dict(newvals, '.')

    # find Argo Git Repo, Branch and Overlay file
    gitrepo = newvals.get('ArgoGitRepo','')
    gitbranch = newvals.get('ArgoGitBranch','main')
    helmchart = newvals.get('HelmChart', '')

    # make a temp dir for the clone
    tempdir = tempfile.TemporaryDirectory()
    os.chdir(tempdir.name)

    # clone and switch branch
    run_git('git clone ' + gitrepo + ' .')
    run_git('git checkout ' + gitbranch)

    # read in the helmchart overlay file
    k_vals = {}

    if (os.path.exists(helmchart)):
        print("Loading " + helmchart)
        with open(helmchart, 'r') as stream:
            k_vals = yaml.safe_load(stream)

    appname = newvals.get('application', None)
    comp2app = []

    if (appname is not None):
        appfile = '/tmp/' + appname.replace(';', '_') + '.json'

        if (os.path.exists(appfile)):
            data = ""
            with open(appfile) as myfile:
                data="".join(line.rstrip() for line in myfile)

            if (len(data.strip()) > 0):
                comp2app = json.loads(data)

        for index in range(len(k_vals.get('dependencies', [])):
            name = k_vals['dependencies'][index]['name']
            version = k_vals['dependencies'][index]['version']

            for updatecomp in comp2app:
                if (updatecomp.get('chartname') == chartname):
                    k_vals['dependencies'][index]['version'] = updatecomp.get('version', '')
                    break

        version = k_vals.get('version', None)

        if (version is not None):
            parts = version.split('.')
            vnum = int(parts[-1])
            vnum = vnum + 1
            parts[-1] = str(vnum)
            k_vals['version'] = ".".join(parts)
            version = k_vals.get('version', None)

            with open(helmchart, 'w') as file:
                yaml.dump(k_vals, file)

        with open(helmchart, 'w') as file:
            yaml.dump(k_vals, file)

        lines = subprocess.run(['cat', helmchart], check=False, stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
        for line in lines:
            print(line)

    # bump chart version number
        if (os.path.exists(helmchart)):
            print("Loading " + helmchart)
            with open(helmchart, 'r') as stream:
                k_vals = yaml.safe_load(stream)



            if (version is not None and chartname is not None and appname is not None):
                appfile = '/tmp/' + appname.replace(';', '_') + '.json'

                comp2app = []
                if (os.path.exists(appfile)):
                    data = ""
                    with open(appfile) as myfile:
                        data="".join(line.rstrip() for line in myfile)

                    if (len(data.strip()) > 0):
                        comp2app = json.loads(data)

                comp2app.append({ 'chartname': chartname, 'version': version })

                with open(appfile, 'w') as file:
                    json.dump(comp2app)

    run_git('git config --global user.name "' + newvals.get('GitUserName', 'ortelius') + '"')
    run_git('git config --global user.email "' + newvals.get('GitUserEmail', 'ortelius@users.noreply.github.com') + '"')
    run_git('git add ' + helmchart)
    run_git('git commit -m "[DeployHub deployment: ' + flat_newvals.get('images.tag', flat_newvals.get('images.newTag', flat_newvals.get('DockerTag', ''))) + ']"')
    run_git('git push')
    os.chdir('/tmp')

if __name__ == '__main__':
    main()
