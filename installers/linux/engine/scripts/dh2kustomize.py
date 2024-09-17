#!/usr/bin/python3

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
    kustomize = newvals.get('KustomizeOverlay', '')

    # make a temp dir for the clone
    tempdir = tempfile.TemporaryDirectory()
    os.chdir(tempdir.name)

    # clone and switch branch
    run_git('git clone ' + gitrepo + ' .')
    run_git('git checkout ' + gitbranch)

    # read in the Kustomize overlay file
    k_vals = {}

    if (os.path.exists(kustomize)):
        print("Loading " + kustomize)
        with open(kustomize, 'r') as stream:
            k_vals = yaml.safe_load(stream)


    # look over the kustomize yaml keys
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
        else:
            lookup = flat_newvals.get(key, None)
            if (lookup is not None):
                k_vals[key] = lookup

    with open(kustomize, 'w') as file:
        yaml.dump(k_vals, file)

    lines = subprocess.run(['cat', kustomize], check=False, stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
    for line in lines:
        print(line)

    run_git('git config --global user.name "' + newvals.get('GitUserName', 'ortelius') + '"')
    run_git('git config --global user.email "' + newvals.get('GitUserEmail', 'ortelius@users.noreply.github.com') + '"')
    run_git('git add ' + kustomize)
    run_git('git commit -m "[DeployHub deployment: ' + flat_newvals.get('images.tag', flat_newvals.get('images.newTag', '')) + ']"')
    run_git('git push')
    os.chdir('/tmp')

if __name__ == '__main__':
    main()
