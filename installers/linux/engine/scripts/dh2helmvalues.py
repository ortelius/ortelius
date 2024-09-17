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
    helmvalues = newvals.get('HelmValues', '')

    # make a temp dir for the clone
    tempdir = tempfile.TemporaryDirectory()
    os.chdir(tempdir.name)

    # clone and switch branch
    run_git('git clone --depth 1  --branch ' + gitbranch + ' ' + gitrepo + ' .')
    run_git('git config --global user.name "' + newvals.get('GitUserName', 'ortelius') + '"')
    run_git('git config --global user.email "' + newvals.get('GitUserEmail', 'ortelius@users.noreply.github.com') + '"')

    # read in the helmvalues overlay file
    k_vals = {}

    if (os.path.exists(helmvalues)):
        print("Loading " + helmvalues)
        with open(helmvalues, 'r') as stream:
            k_vals = yaml.safe_load(stream)

    # look over the helmvalues yaml keys
    for key, val in k_vals.items():
        if (isinstance(val,list)):
            index = 0
            for item in (val):
                if (isinstance(item, dict)):
                    for subkey in item.keys():
                        lookup = flat_newvals.get(key + '.' + subkey, None)
                        if (lookup is not None):
                            k_vals[key][index][subkey] = lookup
                index = index+1
        elif (isinstance(val,dict)):
            for subkey in val.keys():
                lookup = flat_newvals.get(key + '.' + subkey, None)
                if (lookup is not None):
                    k_vals[key][subkey] = lookup
        else:
            lookup = flat_newvals.get(key, None)
            if (lookup is not None):
                k_vals[key] = lookup

    with open(helmvalues, 'w') as file:
        yaml.dump(k_vals, file)

    lines = subprocess.run(['cat', helmvalues], check=False, stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
    for line in lines:
        print(line)

    run_git('git add ' + helmvalues)

# bump chart version number
    helmvalues = helmvalues.replace('values.yaml', 'Chart.yaml')
    if (os.path.exists(helmvalues)):
        print("Loading " + helmvalues)
        with open(helmvalues, 'r') as stream:
            k_vals = yaml.safe_load(stream)

        version = k_vals.get('version', None)

        if (version is not None):
            parts = version.split('.')
            vnum = int(parts[-1])
            vnum = vnum + 1
            parts[-1] = str(vnum)
            k_vals['version'] = ".".join(parts)
            version = k_vals.get('version', None)

            with open(helmvalues, 'w') as file:
                yaml.dump(k_vals, file)

        chartname = k_vals.get('name', None)
        appname = newvals.get('application', None)

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
                json.dump(comp2app, file)


    run_git('git add ' + helmvalues)
    run_git('git commit -m "[DeployHub deployment: ' + flat_newvals.get('images.tag', flat_newvals.get('images.newTag', flat_newvals.get('DockerTag', ''))) + ']"')
    run_git('git push')
    os.chdir('/tmp')

if __name__ == '__main__':
    main()
