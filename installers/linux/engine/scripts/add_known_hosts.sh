#!/bin/sh
HOST=$1
HOME=$(getent passwd `whoami` | cut -d: -f6)
ssh-keyscan -t rsa,dsa $HOST>> $HOME/.ssh/known_hosts
sort -u $HOME/.ssh/known_hosts > $HOME/.ssh/known_hosts.unique
cat $HOME/.ssh/known_hosts.unique > $HOME/.ssh/known_hosts
