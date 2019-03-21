"""This module interfaces with the DeployHub RestAPIs to perform login, deploy, move and approvals."""

import json
import urllib
import time
import requests


def get_json(url, cookies):
    """Get URL as json string.
    Returns: json string"""
    res = requests.get(url, cookies=cookies)
    if (res is None):
        return None
    return res.json()


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
    return get_json(dhurl + "/dmadminweb/API/deploy/" + urllib.parse.quote(app) + "/" + urllib.parse.quote(env), cookies)


def move_application(app, from_domain, task, dhurl, cookies):
    """Move an application from the from_domain using the task"""
    # Get appid
    data = get_json(dhurl + "/dmadminweb/API/application/" + urllib.parse.quote(app), cookies)
    appid = str(data['result']['id'])

    # Get from domainid
    data = get_json(dhurl + "/dmadminweb/API/domain/" + urllib.parse.quote(from_domain), cookies)
    fromid = str(data['result']['id'])

    # Get from Tasks
    data = get_json(dhurl + "/dmadminweb/GetTasks?domainid=" + fromid, cookies)
    taskid = "0"

    for atask in data:
        if (atask['name'] == task):
            taskid = str(atask['id'])

    # Move App Version
    data = get_json(dhurl + "/dmadminweb/RunTask?f=run&tid=" + taskid + "&notes=&id=" + appid + "&pid=" + fromid, cookies)
    return(data)


def approve_application(app, dhurl, cookies):
    """Approve the application for the current domain that it is in."""
    # Get appid
    data = get_json(dhurl + "/dmadminweb/API/application/" + urllib.parse.quote(app), cookies)
    appid = str(data['result']['id'])

    data = get_json(dhurl + "/dmadminweb/API/approve/" + appid, cookies)
    return data


def is_deployment_done(deployment_id, dhurl, cookies):
    """Check to see if the deployment has completed"""
    data = get_json(dhurl + "/dmadminweb/API/log/" + deployment_id + "?checkcomplete=Y", cookies)

    if (data is None):
        return [False, "Could not get log #" + deployment_id]

    if (is_empty(data.text)):
        return [False, "Could not get log #" + deployment_id]

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


def get_attrs(app, comp, env, srv, dhurl, cookies):
    """Get the attributes for this deployment base on app version and env.
    Returns: json of attributes"""

    data = get_json(dhurl + "/dmadminweb/API/environment/" + urllib.parse.quote(env), cookies)
    envid = str(data['result']['id'])
    servers = data['result']['servers']

    data = get_json(dhurl + "/dmadminweb/API/getvar/environment/" + envid, cookies)
    env_attrs = data['attributes']

    for a_srv in servers:
        if (srv == a_srv['name']):
            srvid = str(a_srv['id'])
            data = get_json(dhurl + "/dmadminweb/API/getvar/server/" + srvid, cookies)
            srv_attrs = data['attributes']
            break

    data = get_json(dhurl + "/dmadminweb/API/application/" + app, cookies)

    if (app == data['result']['name']):
        appid = str(data['result']['id'])
    else:
        for a_ver in data['result']['versions']:
            if (app == a_ver['name']):
                appid = str(a_ver['id'])
                break

    data = get_json(dhurl + "/dmadminweb/API/getvar/application/" + appid, cookies)
    app_attrs = data['attributes']

    data = get_json(dhurl + "/dmadminweb/API/component/" + comp, cookies)
    compid = str(data['result']['id'])

    data = get_json(dhurl + "/dmadminweb/API/getvar/component/" + compid, cookies)
    comp_attrs = data['attributes']

    result = {}
    for entry in env_attrs:
        result.update(entry)

    for entry in srv_attrs:
        result.update(entry)

    for entry in app_attrs:
        result.update(entry)

    for entry in comp_attrs:
        result.update(entry)

    return result
