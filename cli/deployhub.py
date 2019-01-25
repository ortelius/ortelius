#!/usr/bin/env python
"""This module interfaces with the DeployHub RestAPIs to perform login, deploy, move and approvals."""

import json
import urllib
import time
import requests
import click


def is_empty(my_string):
    """Is the string empty"""
    return not (my_string and my_string.strip())


def is_not_empty(my_string):
    """Is the string NOT empty"""
    return bool(my_string and my_string.strip())


def login(dhurl, user, password):
    """Login to DeployHub using the DH Url, userid and password.
    Returns: cookies to be used in subsequent API calls"""
    res = requests.post(dhurl + "/dmadminweb/API/login", data={'user': user, 'pass': password})
    cookies = res.cookies
    return cookies


def deploy_application(app, env, dhurl, cookies):
    """Deploy the application to the environment
    Returns: deployment_id"""
    url = dhurl + "/dmadminweb/API/deploy/" + urllib.parse.quote(app) + "/" + urllib.parse.quote(env)

    res = requests.get(url, cookies=cookies)
    return res.json()


def move_application(app, from_domain, task, dhurl, cookies):
    """Move an application from the from_domain using the task"""
    # Get appid
    url = dhurl + "/dmadminweb/API/application/" + urllib.parse.quote(app)
    res = requests.get(url, cookies=cookies)
    data = res.json()
    appid = str(data['result']['id'])

    # Get from domainid
    url = dhurl + "/dmadminweb/API/domain/" + urllib.parse.quote(from_domain)
    res = requests.get(url, cookies=cookies)
    data = res.json()
    fromid = str(data['result']['id'])

    # Get from Tasks
    url = dhurl + "/dmadminweb/GetTasks?domainid=" + fromid
    res = requests.get(url, cookies=cookies)
    data = res.json()
    taskid = "0"

    for atask in data:
        if (atask['name'] == task):
            taskid = str(atask['id'])

    # Move App Version
    url = dhurl + "/dmadminweb/RunTask?f=run&tid=" + taskid + "&notes=&id=" + appid + "&pid=" + fromid

    res = requests.get(url, cookies=cookies)
    return(res.json())


def approve_application(app, dhurl, cookies):
    """Approve the application for the current domain that it is in."""
    # Get appid
    url = dhurl + "/dmadminweb/API/application/" + urllib.parse.quote(app)
    res = requests.get(url, cookies=cookies)
    data = res.json()
    appid = str(data['result']['id'])

    url = dhurl + "/dmadminweb/API/approve/" + appid
    res = requests.get(url, cookies=cookies)
    return res.json()


def is_deployment_done(deployment_id, dhurl, cookies):
    """Check to see if the deployment has completed"""
    url = dhurl + "/dmadminweb/API/log/" + deployment_id + "?checkcomplete=Y"
    res = requests.get(url, cookies=cookies)

    if (res is None):
        return [False, "Could not get log #" + deployment_id]

    if (is_empty(res.text)):
        return [False, "Could not get log #" + deployment_id]

    data = res.json()
    return [True, data]


def get_logs(deployment_id, dhurl, cookies):
    """Get the logs for the deployment.
    Returns: log as a String"""

    while (True):
        res = is_deployment_done(deployment_id, dhurl, cookies)

        if (res[0] and res[1]['success'] and res[1]['iscomplete']):
            url = dhurl + "/dmadminweb/API/log/" + deployment_id
            res = requests.get(url, cookies=cookies)
            return res.json()

        time.sleep(10)


@click.command()
@click.option('--cmd', help='Command:  deploy, approve, getlogs, move', required=True)
@click.option('--dhurl', help='DeployHub Server URL')
@click.option('--userid', help='Login Userid')
@click.option('--password', help='Login Password')
@click.option('--deployid', help='Deployment Id')
@click.option('--app', help='Application Name')
@click.option('--env', help='Deployment Environment')
@click.option('--from_domain', help='Move from Domain')
@click.option('--move_task', help='Task to Move the Application')
def main(dhurl, userid, password, cmd, app, env, deployid, from_domain, move_task):
    retcode = 0
    cookies = login(dhurl, userid, password)
    if (cmd == "getlogs"):
        log = get_logs(deployid, dhurl, cookies)
        print("\n".join(log['logoutput']))
    elif (cmd == "deploy"):
        res = deploy_application(app, env, dhurl, cookies)
        deployid = str(res['deploymentid'])
        log = get_logs(deployid, dhurl, cookies)
        print("\n".join(log['logoutput']))
        print("\n")
        if (log['exitcode'] == 0):
            print("Deployment #" + str(res['deploymentid']) + " ran successfuly")
        else:
            print("Deployment #" + str(res['deploymentid']) + " ran unsuccessfuly")
            retcode = 1
    elif (cmd == "move"):
        res = move_application(app, from_domain, move_task, dhurl, cookies)
        if (res['result']):
            print("Move successful")
        else:
            print("Move unsuccessful")
            retcode = 1
    elif (cmd == "approve"):
        res = approve_application(app, dhurl, cookies)
        if (res['success']):
            print("Approval successful")
        else:
            print("Approval unsuccessful")
            retcode = 1
    exit(retcode)


# Entry Point
if __name__ == "__main__":
    main()
