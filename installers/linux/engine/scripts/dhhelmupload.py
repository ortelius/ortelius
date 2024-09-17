#!/usr/bin/python3

import base64
import json
import os
import subprocess
import sys

# from pprint import pprint

def get_script_path():
    return os.path.dirname(os.path.realpath(sys.argv[0]))

def upload_helm():
    my_env = os.environ.copy()

    upload = {}
    upload['files'] = []
    upload['deployid'] = 700
    upload['component'] = "shippingservice;master;v1_2_2_17_gf3a1c49"
    upload['helmrepo'] = "bitnami"
    upload['helmrepourl'] = "https://charts.bitnami.com/bitnami"
    upload['chartorg'] = "bitnami"
    upload['chartname'] = "wordpress"
    upload['chartversion'] = "9.3.8"

    my_env['helmrepouser'] = ''
    my_env['helmrepopass'] = ''
    my_env['dockeruser'] = 'sbtaylor15'
    my_env['dockerpass'] = 'XXXXXXX'
    my_env['helmrepo'] = upload['helmrepo']
    my_env['helmrepourl'] = upload['helmrepourl']
    my_env['chartorg'] = upload['chartorg']
    my_env['chartname'] = upload['chartname']
    my_env['chartversion'] = upload['chartversion']
    my_env['chartvalues'] = ""

    pid = subprocess.Popen(get_script_path() + "/helminfo.sh", env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    jstr = ""
    for line in pid.stdout.readlines():
        line = line.decode('utf-8')
        jstr = jstr + line
        print(line)

    pid.wait()

    dobj = json.loads(jstr)
    upload['chartdigest'] = dobj.get("chartdigest", "")
    upload['images'] = dobj.get("images", [])

    start_dir = "."

    filelist = []
    for root, d_names, f_names in os.walk(start_dir):
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

    print(json.dumps(upload))


if __name__ == '__main__':
    upload_helm()
