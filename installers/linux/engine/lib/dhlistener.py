#!/usr/bin/env python
import logging
import math
import os
import shlex
import signal
import subprocess
import sys
import tempfile
from threading import Thread
from time import sleep
from urllib.parse import unquote_plus

import requests
import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# Run the command and log output


def runcmd(event, dhurl):
    event['command'] = 'dm ' + unquote_plus(event['command'])  # Setup command line

    my_env = os.environ.copy()
    if (event['credentails'] != ''):
        my_env["TRIFIELD1"] = unquote_plus(event['credentails'])  # Push password for dm to the TRIFIELD1 env

    cmd = event['command'].replace("\",\"", "\" \"")  # Command is seperated by commas in the db. change it to spaces

    tmp = tempfile.NamedTemporaryFile(delete=False)  # Create a temp file for stdin
    stdinStr = unquote_plus(event['stdin'])        # Decode escaped chars
    if (len(stdinStr) > 0):               # Write the stdin to the file
        with open(tmp.name, 'w') as f:
            f.write(stdinStr)
            f.close()
    else:                                 # No stdin
        with open(tmp.name, 'w') as f:
            f.write('\n')
            f.close()

    waitfor = event['waitfor']     # Set waitfor from db
    id = event['id']               # Set id from db
    clientid = event['clientid']   # Set clientid from db

    logging.info("Running: %s", str(event))  # Log raw event from db for debugging

    args = shlex.split(cmd)             # Split the cmd into an array
    logging.info("stdin= %s", tmp.name)   # Log temp file name
    myinput = open(tmp.name)            # Open temp file for read access
    p = subprocess.Popen(args, stdin=myinput, stdout=subprocess.PIPE, stderr=subprocess.PIPE,  env=my_env)  # Run the cmd
    (out, err) = p.communicate()
    retval = p.returncode

    uplog = out.decode('utf-8') + err.decode('utf-8')
    logging.info(uplog)
    logging.info("Completed: %s", str(event))
    logging.info("ExitCode: %s", str(retval))

    if (waitfor == 1):
        postURL(dhurl + "/dmadminweb/EngineEvent?clientid=" + clientid + "&id=" + str(id), "output",  uplog.strip())  # Log output back to the db
        postURL(dhurl + "/dmadminweb/EngineEvent?clientid=" + clientid + "&id=" + str(id) + "&exitcode=" + str(retval), "output", "")  # Log return code back to db
        logging.info("Log Uploaded: %s", str(event))

# Get the data from the url and retry if error happens


def getURL(url):
    logging.debug(url)
    error = True
    r = "{}"
    retrycnt = 1
    delay = 0.0
    while (error):
        try:
            r = requests.get(url, verify=False)                                 # Get with parameters
            error = False
        except requests.exceptions.RequestException as e:        # Server is not responding so do a progressive delay
            logging.warn("Delay(" + str(delay) + ") " + str(e))
            error = True
            delay = (math.pow(2, retrycnt) - 1) / 2.0
            retrycnt += 1
            sleep(delay)
        if (hasattr(r, 'status_code')):
            if (r.status_code == 504):                               # Server timed out
                logging.warn("Delay( %s ) 504", str(delay))
                error = True
                sleep(delay)
    return r

# Get the data from the url and retry if error happens


def postURL(url, key, postdata):
    error = True
    r = "{}"
    retrycnt = 1
    delay = 0.0
    while (error):
        try:
            r = requests.post(url,  data={key: postdata}, verify=False)        # Post with payload
            error = False
        except requests.exceptions.RequestException as e:        # Server is not responding so do a progressive delay
            logging.warn("Delay( %s ) %s", str(delay), str(e))
            error = True
            delay = (math.pow(2, retrycnt) - 1) / 2.0
            retrycnt += 1
            sleep(delay)
    return r

# Main:  take the DeployHub base URL and the Client Id for the Saas Engine as the parameters


def main(dhurl, clientid,  DMHOME, dm_database, dm_port):

    if (dm_database != "" and dm_database != ""):
        lineList = [line.rstrip('\n') for line in open("/etc/odbc.ini")]
        newlist = []
        for line in lineList:
            if ("Servername=" in line):
                line = "Servername=" + dm_database
            if ("Port=" in line):
                line = "Port=" + dm_port
            newlist.append(line)
        with open("/etc/odbc.ini", 'w') as fpout:
            fpout.write('\n'.join(newlist))

    since = ""
    logfilename = DMHOME + "/log/listener.log"   # Log file name
    if (os.path.exists(logfilename)):
        os.unlink(logfilename)
    logging.basicConfig(filename=logfilename, level=logging.INFO,  format='%(asctime)s - %(threadName)s - %(message)s')  # Setup logger

    while True:    # Do Forever

        logging.debug("Waiting for event")
        if (since == ""):
            r = getURL(dhurl + "/dmadminweb/EngineEvent?clientid=" + clientid)    # First time so get all messages in queue table
        else:
            r = getURL(dhurl + "/dmadminweb/EngineEvent?clientid=" + clientid + "&since=" + str(since))  # Get all new messages since last time

        if (r is None):
            continue

        logging.debug("Received event")
        logging.debug(r.text)

        data = r.json()  # Convert output to json
        if (data['message'] == "found"):  # found some data to read
            r = getURL(dhurl + "/dmadminweb/EngineEvent?clientid=" + clientid + "&getevents=y")  # Get the events in the queue table.  Server side deletes them so they are not read twice
            data = r.json()      # Convert output to json
            for event in data:   # Loop over all events in queue
                since = event['lastupdate']  # Save last event time (already in order by the server side)
                t = Thread(target=runcmd, args=(event, dhurl))  # Create a thread to run the event as a command line pgm
                t.start()  # Start the thread


def signal_handler(signal, frame):
    sys.exit(0)


# Entry Point
if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    dhurl = sys.argv[1]   # DeployHub Url Passed in
    clientid = sys.argv[2]   # DeployHub Client Id Passed In
    DMHOME = sys.argv[3]   # DeployHub HOME dir
    dm_database = ""
    dm_port = ""

    if (len(sys.argv) > 3):
        dm_database = sys.argv[4]

    if (len(sys.argv) > 4):
        dm_port = sys.argv[5]

    main(dhurl, clientid, DMHOME, dm_database, dm_port)
