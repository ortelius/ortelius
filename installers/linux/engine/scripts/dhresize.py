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

#!/usr/bin/env python3

import subprocess

import click


def runcmd(cmd):
    print(cmd)
    cmdlist = cmd.split(' ')
    lines = subprocess.run(cmdlist, check=False, stderr=subprocess.STDOUT, stdout=subprocess.PIPE).stdout.decode('utf-8')
    print(lines)


@click.command()
@click.option('--cluster-name', help='Cluster Name')
@click.option('--node-pool', help='Node Pool')
@click.option('--num-nodes', help='Node Size')
@click.option('--zone', help='Zone')
@click.option('--core-account', help='GKE Core Account')
@click.option('--core-project', help='GKE Core Project')
@click.option('--key-file', help='Key File')
@click.option('--service-account', help='GKE Service Account')
def main(cluster_name, node_pool, num_nodes, zone, core_account, core_project, key_file, service_account):

    if (core_account is not None):
        runcmd('gcloud config set core/account ' + core_account)

    if (core_project is not None):
        runcmd('gcloud config set core/project ' + core_project)

    if (zone is not None):
        runcmd('gcloud config set compute/zone ' + zone)

    if (cluster_name is not None):
        runcmd('gcloud config set container/cluster ' + cluster_name)

    if (service_account is not None and key_file is not None):
        runcmd('gcloud auth activate-service-account ' + service_account + ' --key-file=' + key_file)

    if (cluster_name is not None):
        runcmd('gcloud container clusters get-credentials ' + cluster_name)

    runcmd('gcloud container clusters resize ' + cluster_name + ' --node-pool ' + node_pool + ' --num-nodes ' + str(num_nodes) + ' --zone ' + zone + ' -q')


if __name__ == '__main__':
    main()
