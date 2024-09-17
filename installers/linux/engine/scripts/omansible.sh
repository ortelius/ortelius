#!/bin/bash
ID=`whoami`
declare -a ANSIBLE_OPTS
while [[ $# > 1 ]]
do
key="$1"

case $key in
    --ansible_target)
    TARGET="$2"
    shift # past argument
    ;;
    --galaxy_role)
    ROLE="$2"
    shift # past argument
    ;;
    --ansible_ssh_user)
    USERID="$2"
    shift # past argument
    ;;
    --ansible_ssh_pass)
    PASSWORD="$2"
    shift # past argument
    ;;
    --ansible_sudo_pass)
    SUDO_PW="$2"
    shift # past argument
    ;;
    --ansible_variable_file)
    VAR_FILE="$2"
    shift # past argument
    ;;
    *)
    ANSIBLE_OPTS+=($key)  # ansible role specific option
    ANSIBLE_OPTS+=($2)
    shift
    ;;
esac
shift # past argument or value
done

USERID=`echo $USERID | sed -e "s/^'//" | sed -e "s/'\$//"`
TARGET=`echo $TARGET | sed -e "s/^'//" | sed -e "s/'\$//"`
PASSWORD=`echo $PASSWORD | sed -e "s/^'//" | sed -e "s/'\$//"`
SUDO_PW=`echo $SUDO_PW | sed -e "s/^'//" | sed -e "s/'\$//"`

HOME=$(getent passwd `whoami` | cut -d: -f6)

echo "Executing Ansible role '$ROLE' against server '$TARGET' ... "

if grep -q "$TARGET" "$HOME/.ssh/known_hosts"; then
 echo "SSH Key is already configured"
else
 ssh-keyscan -t rsa,dsa $TARGET >> $HOME/.ssh/known_hosts
 sort -u $HOME/.ssh/known_hosts > $HOME/.ssh/known_hosts.unique
 cat $HOME/.ssh/known_hosts.unique > $HOME/.ssh/known_hosts
fi

if [ "$ID" = "root" ]; then
  /usr/bin/ansible-galaxy install $ROLE
else
  $TRILOGYHOME/trilogycli ANSIBLE install $ROLE
fi

echo "- hosts: all" > /tmp/$$.yml
if [[ "$SUDO_PW" != "none" && "$SUDO_PW" != "" ]]; then
 echo "  sudo: yes" >> /tmp/$$.yml
fi

echo "  gather_facts: false" >> /tmp/$$.yml
echo "  " >> /tmp/$$.yml
echo "  vars:" >> /tmp/$$.yml
if [ "$USERID" != "" ]; then
 echo "    ansible_ssh_user: $USERID" >> /tmp/$$.yml
fi

if [ "$PASSWORD" != "" ]; then
 echo "    ansible_ssh_pass: $PASSWORD" >> /tmp/$$.yml
fi

if [[ "$SUDO_PW" != "none" && "$SUDO_PW" != "" ]]; then
 echo "    ansible_sudo_pass: $SUDO_PW" >> /tmp/$$.yml
fi

ELEMENTS=${#ANSIBLE_OPTS[@]}

for ((i=0; i<$ELEMENTS; i++));
do
  tmp_var=${ANSIBLE_OPTS[${i}]}
  var=`echo $tmp_var | sed -e 's/^-\+//'`
  i=$((i+1))
  val=${ANSIBLE_OPTS[${i}]}
  echo "    $var: $val"  >> /tmp/$$.yml
done

echo "   " >> /tmp/$$.yml
echo "  pre_tasks:" >> /tmp/$$.yml
echo "   " >> /tmp/$$.yml
echo "  - name: Install python2 for Ansible" >> /tmp/$$.yml
echo '    raw: bash -c "test -e /usr/bin/python || (apt -qqy update && apt install -qqy python-minimal)"' >> /tmp/$$.yml
echo "    register: output" >> /tmp/$$.yml
echo "    changed_when:" >> /tmp/$$.yml
echo '    - output.stdout != ""' >> /tmp/$$.yml
echo '    - output.stdout != "\r\n"' >> /tmp/$$.yml
echo "  - name: Gathering Facts" >> /tmp/$$.yml
echo "    setup:" >> /tmp/$$.yml
echo "   " >> /tmp/$$.yml
echo "  roles:" >> /tmp/$$.yml
echo "     - { role: $ROLE }" >> /tmp/$$.yml

cat /tmp/$$.yml | sed "s/pass: .*/pass: XXXXXXX/"

if [ "$ID" = "root" ]; then
 $TRILOGYHOME/scripts/add_known_hosts.sh $TARGET
else
	 $TRILOGYHOME/trilogycli KNOWN_HOSTS $TARGET
fi

echo ansible /tmp/$$.yml -i "$TARGET,"

if [ "$ID" = "root" ]; then
 /usr/bin/ansible-playbook	/tmp/$$.yml -i "$TARGET,"
else
	$TRILOGYHOME/trilogycli ANSIBLE2 /tmp/$$.yml -i "$TARGET,"
fi
