#!/usr/bin/python3

from pprint import pprint
import sys
import yaml
import qtoml
import os
import subprocess

def runcmd(cmd):
    my_env = os.environ.copy()
    my_env['PATH'] = my_env['PATH'] + ":/usr/local/google-cloud-sdk/bin"
    print(cmd)
    p = subprocess.Popen(cmd, env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    p.wait()
    retry = 0

    for line in p.stdout.readlines():
        line = line.decode('ascii').strip('\n')
        print(line)
        if ("Error" in line) and ("already exists" in line):
           retry = 1
    return retry


def main():
    """Main entry point <Chart Directory> <Override Values Toml>"""

    newvals = {}
    my_env = os.environ.copy()
    releasename = sys.argv[1]
    chart       = sys.argv[2]

    if (not releasename or not chart):
       return 0

    chartvalues = sys.argv[2] + "/values.yaml"
    rspfile     = sys.argv[3]
    releasename = releasename.replace(';','-v').replace('_','-').lower()

    stream = open(chartvalues, 'r')
    values = yaml.load(stream)
    newvals.update(values)
    stream.close()

    print("RSP="+ rspfile)
    lines = subprocess.run(['cat',rspfile], stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
    
    cleanvals = ""
    for line in lines:
     (k,v) = line.split('=')

     if ('?' not in k):
       cleanvals = cleanvals + line + "\n" 

    values = qtoml.loads(cleanvals)
    newvals.update(values)
    os.rename(chartvalues, chartvalues + ".bak")

    stream = open(chartvalues,"w")
    yaml.dump(newvals,stream)
    stream.close()
    
#    pprint(newvals)
    runcmd('gcloud config set compute/zone ' + newvals['compute']['zone'])
    runcmd('gcloud config set container/cluster ' + newvals['container']['cluster'])
    runcmd('gcloud config set core/account ' + newvals['core']['account'])
    runcmd('gcloud config set core/disable_usage_reporting ' + newvals['core']['disable_usage_reporting'])
    runcmd('gcloud config set core/project ' + newvals['core']['project'])
    runcmd('gcloud auth activate-service-account ' + newvals['gcloud']['oauth']['account'] + ' --key-file=' + newvals['gcloud']['oauth']['keyfile'])
    runcmd('gcloud container clusters get-credentials ' + newvals['container']['cluster'])
    runcmd('helm init --upgrade')
    runcmd('cat ' + chartvalues)

    retry = runcmd('helm upgrade "' + releasename + '" "' + chart + '" --install --force --debug')
    
    if (retry):
      print("Retrying helm upgrade")
      runcmd('helm upgrade "' + releasename + '" "' + chart + '" --install --force')

    os.remove(chartvalues)
    os.remove(rspfile)

if __name__ == '__main__':
    main()    
