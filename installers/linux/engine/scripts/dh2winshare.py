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

import tempfile
import os
import shutil
import subprocess
import re
import click

@click.command()
@click.option('--winshare_user', help='Windows User Name', required=True)
@click.option('--winshare_pass', help='Windows Password', required=True)
@click.option('--winshare_sharename', help='Windows Share Name', required=True)
@click.option('--winshare_folder', help='Windows Folder', default="", required=False)
@click.option('--winshare_userdom', help='User domain', required=False)

def main(winshare_user, winshare_pass, winshare_sharename, winshare_folder, winshare_userdom):
    """Main entry point"""

    if (winshare_userdom is not None):
        user = winshare_userdom + "/" + winshare_user + "%" + winshare_pass
    else:
        user = winshare_user + "%" + winshare_pass

    srvname = winshare_sharename
    folder = winshare_folder
    mntpoint = "/winshares/" + re.sub(r"\\", r"/", srvname)
    mntpoint = re.sub("/{2,}", "/", mntpoint)

    tempfd, zipfile = tempfile.mkstemp(suffix=".tar", prefix="dh")
    my_env = os.environ.copy()

    cmd = "smbclient '" + srvname + "' \"\" -U '" + user + "' -Tc " + zipfile + " " + folder
    cmd_display = "smbclient '" + srvname + "' \"\" -Tc " + zipfile + " " + folder
    print(cmd_display)
    print(mntpoint + " " + zipfile)
    pid = subprocess.Popen(cmd, env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in pid.stdout.readlines():
        print(line.decode('utf-8').strip('\n'))

    pid.wait()

    if (os.path.exists(mntpoint)):
        shutil.rmtree(mntpoint)

    os.makedirs(mntpoint, exist_ok=True)
    os.chdir(mntpoint)

    pid = subprocess.Popen('tar xvf ' + zipfile, env=my_env, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in pid.stdout.readlines():
        print(line.decode('utf-8').strip('\n'))

    pid.wait()

    os.unlink(zipfile)
    os.close(tempfd)

    exit(pid.returncode)


if __name__ == "__main__":
    main()
