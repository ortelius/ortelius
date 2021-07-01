#!/usr/bin/python3

import subprocess
import sys
import time

import jenkins
import qtoml


def runcmd(cmd):
    print(cmd)
    lines = subprocess.run(cmd, check=False, shell=True, stderr=subprocess.STDOUT,
                           stdout=subprocess.PIPE).stdout.decode('utf-8')
    print(lines)


def main():
    """Main entry point <Values Toml>"""

    newvals = {}
    rspfile = sys.argv[1]

    print("RSP=" + rspfile)
    lines = subprocess.run(['cat', rspfile], check=False, stdout=subprocess.PIPE).stdout.decode('utf-8').split("\n")
    #    print(lines)

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

    # now create jenkins job
    jenkins_url = newvals.get("jenkins_url", "")
    jenkins_user = newvals.get("jenkins_user", newvals.get('jenkinsuser', '')).strip()
    jenkins_token = newvals.get("jenkins_token", newvals.get('jenkinspass', '')).strip()
    jenkins_job = newvals.get("jenkins_job", "")
    jenkins_job_params = newvals.get("jenkins_job_params", {})
    jenkins_log = newvals.get("jenkins_log", "true")

    if jenkins_user == '':
        jenkins_user = None
    if jenkins_token == '':
        jenkins_token = None

    server = jenkins.Jenkins(jenkins_url, username=jenkins_user, password=jenkins_token)

    server.build_job(jenkins_job, jenkins_job_params)
    print('Running....')
    time.sleep(10)

    while True:
        if server.get_job_info(jenkins_job)['lastCompletedBuild']['number'] == \
          server.get_job_info(jenkins_job)['lastBuild']['number']:
            print("Build Number: " + str(server.get_job_info(jenkins_job)['lastCompletedBuild']['number']))
            break

        time.sleep(3)

    time.sleep(5)
    if server.get_job_info(jenkins_job)['lastCompletedBuild']['number'] == \
      server.get_job_info(jenkins_job)['lastSuccessfulBuild']['number']:
        status = 'SUCCESS'
    else:
        status = 'FAILED'
    print(f"Finished with status {status}...\n")

    console_output = server.get_build_console_output(jenkins_job,
                                                     server.get_job_info(jenkins_job)['lastBuild']['number'])

    if (jenkins_log):
        print(console_output)


if __name__ == '__main__':
    main()
