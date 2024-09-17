#!/bin/sh
# Detects which OS and if it is Linux then it will detect which Linux
# Distribution.

OS=`uname -s`
REV=`uname -r`
MACH=`uname -m`

GetVersionFromFile()
{
    VERSION=`cat $1 | tr "\n" ' ' | sed s/.*VERSION.*=\ // `
}

if grep -q "Red Hat Linux release 9  " /etc/redhat-release ; then
  DIST="rh9"
  DIR=redhat/9
  echo
  echo "$RELEASE is no longer supported."
  echo
  exit 1
elif grep -q "Fedora Core release 2 " /etc/redhat-release ; then
  DIST="fc2"
  DIR=fedora/2
  echo
  echo "$RELEASE is no longer supported."
  echo
  exit 1
elif grep -q "Fedora Core release 3 " /etc/redhat-release ; then
  DIST="fc3"
  DIR=fedora/3
  echo
  echo "$RELEASE is no longer supported."
  echo
  exit 1
  #YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yum"
elif grep -q "Fedora Core release 4 " /etc/redhat-release ; then
  DIST="fc4"
  DIR=fedora/4
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yum"
  PLESKREPO="plesk-fedora"
elif grep -q "Fedora Core release 5 " /etc/redhat-release ; then
  DIST="fc5"
  DIR=fedora/5
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yum"
  PLESKREPO="plesk-fedora"
elif grep -q "Fedora Core release 6 " /etc/redhat-release ; then
  DIST="fc6"
  DIR=fedora/6
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yum rpm-python"
  PLESKREPO="plesk-fedora"
elif grep -q "Fedora release 7 " /etc/redhat-release ; then
  DIST="fc7"
  DIR=fedora/7
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
elif grep -q "Fedora release 8 " /etc/redhat-release ; then
  DIST="fc8"
  DIR=fedora/8
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
elif grep -q "Fedora release 9 " /etc/redhat-release ; then
  DIST="fc9"
  DIR=fedora/9
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 10 " /etc/redhat-release ; then
  DIST="fc10"
  DIR=fedora/10
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 11 " /etc/redhat-release ; then
  DIST="fc11"
  DIR=fedora/11
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 12 " /etc/redhat-release ; then
  DIST="fc12"
  DIR=fedora/12
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 13 " /etc/redhat-release ; then
  DIST="fc13"
  DIR=fedora/13
 G YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 14 " /etc/redhat-release ; then
  DIST="fc14"
  DIR=fedora/14
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 15 " /etc/redhat-release ; then
  DIST="fc15"
  DIR=fedora/15
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 16 " /etc/redhat-release ; then
  DIST="fc16"
  DIR=fedora/16
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
  PLESKREPO="plesk-fedora"
  DISABLE_PLESK=yes
elif grep -q "Fedora release 17 " /etc/redhat-release ; then
  DIST="fc17"
  DIR=fedora/17
  YUMDEPS="fedora-release python-elementtree python-sqlite python-urlgrabber yumrpm-python"
elif grep -q "Fedora release 18 " /etc/redhat-release ; then
  DIST="fc18"
  DIR=fedora/18
elif grep -q "Fedora release 19 " /etc/redhat-release ; then
  DIST="fc19"
  DIR=fedora/19
elif grep -q "Fedora release 20 " /etc/redhat-release ; then
  DIST="fc20"
  DIR=fedora/20
elif grep -q "Fedora release 21 " /etc/redhat-release ; then
  DIST="fc21"
  DIR=fedora/21
elif grep -q "Fedora release 22 " /etc/redhat-release ; then
  DIST="fc22"
  DIR=fedora/22
elif grep -q "Fedora release 23 " /etc/redhat-release ; then
  DIST="fc23"
  DIR=fedora/23
elif egrep -q "Red Hat Enterprise Linux (A|E)S release 3 " /etc/redhat-release ; then
  DIST="el3"
  DIR=redhat/3
  echo
  echo "$RELEASE is not supported at this time, you will need to configure yum manually:"
  echo "see http://$SERVER/channels for instructions"
  echo
  exit 1
elif grep -q "CentOS release 3" /etc/redhat-release ; then
  DIST="el3"
  DIR=centos/3
  echo
  echo "$RELEASE is not supported at this time, you will need to configure yum manually:"
  echo "see http://$SERVER/channels for instructions"
  echo
  exit 1
elif egrep -q "Red Hat Enterprise Linux (A|E|W)S release 4" /etc/redhat-release ; then
  REDHAT=1
  DIST="el4"
  DIR=redhat/4
  YUMDEPS="python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-redhat"
elif egrep -q "Red Hat Enterprise Linux.*release 5" /etc/redhat-release ; then
  REDHAT=1
  DIST="el5"
  DIR=redhat/5
  YUMDEPS="rpm-python python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-redhat"
elif egrep -q "Red Hat Enterprise Linux.*release 6" /etc/redhat-release ; then
  REDHAT=1
  DIST="el6"
  DIR=redhat/6
  YUMDEPS="rpm-python python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-redhat"
elif egrep -q "Red Hat Enterprise Linux.* 7" /etc/redhat-release ; then
  REDHAT=1
  DIST="el7"
  DIR=redhat/7
  YUMDEPS="rpm-python python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-redhat"
elif grep -q "CentOS release 3" /etc/redhat-release ; then
  DIST="el3"
  DIR=centos/3
  YUMDEPS="centos-release python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-centos"
elif grep -q "CentOS release 4" /etc/redhat-release ; then
  DIST="el4"
  DIR=centos/4
  YUMDEPS="centos-release python-elementtree python-sqlite python-urlgrabber yum sqlite"
  PLESKREPO="plesk-centos"
elif egrep -q "(release 5|release 2011)" /etc/redhat-release ; then
  DIST="el5"
  DIR=centos/5
  YUMDEPS="rpm-python centos-release python-elementtree python-sqlite python-urlgrabber yum sqlite m2crypto"
  PLESKREPO="plesk-centos"
# Fc6 uses "release 6" so we need the whole thing here
elif egrep -q "(release 6|release 2012)" /etc/redhat-release ; then
  DIST="el6"
  DIR=centos/6
  YUMDEPS="rpm-python centos-release python-elementtree python-sqlite python-urlgrabber yum sqlite m2crypto"
  PLESKREPO="plesk-centos"
elif egrep -q "(release 7|release 2014)" /etc/redhat-release ; then
  DIST="el7"
  DIR=centos/7
  YUMDEPS="rpm-python centos-release python-elementtree python-sqlite python-urlgrabber yum sqlite m2crypto"
  PLESKREPO="plesk-centos"
else
  echo "Error: Unable to determine distribution type. Please send the contents of /etc/redhat-release to support@atomicrocketturtle.com"
  exit 1
fi

echo $OSSTR

if [ ! -d "re-install" ]; then
  mkdir re-install
fi
cd re-install

rm -f re-engine-7.5.1-1.x86_64.rpm 2>/dev/null
rm -f re-webadmin-7.5.1-1.x86_64.rpm 2>/dev/null

echo Downloading re-engine-7.6.0-1.x86_64.rpm
curl -s -L http://www.openmakesoftware.com/re/rpms/re-engine-7.6.0-1.x86_64.rpm -o re-engine-7.6.0-1.x86_64.rpm

echo Downloading re-webadmin-7.6.0-1.x86_64.rpm
curl -s -L http://www.openmakesoftware.com/re/rpms/re-webadmin-7.6.0-1.x86_64.rpm -o re-webadmin-7.6.0-1.x86_64.rpm

if [ "${DIST}" = "el7" ] || [ "${DIST}" = "fc22" ]; then
 echo Downloading openvas-smb-1.0.1-0.2.$DIST.art.x86_64.rpm
 curl -s -L http://www.openmakesoftware.com/re/rpms/openvas-smb-1.0.1-0.2.$DIST.art.x86_64.rpm -o openvas-smb-1.0.1-0.2.$DIST.art.x86_64.rpm
fi

if [ "${DIST}" = "el6" ] || [ "${DIST}" = "fc23" ]; then
 echo Downloading openvas-smb-1.0.1-1.$DIST.art.x86_64.rpm
 curl -s -L http://www.openmakesoftware.com/re/rpms/openvas-smb-1.0.1-1.$DIST.art.x86_64.rpm -o openvas-smb-1.0.1-1.$DIST.art.x86_64.rpm
fi

if [ "${DIST}" = "el7" ] || [ "${DIST}" = "el6" ]; then
 if [ "${DIST}" = "el7" ]; then
  echo Installing epel-release-7-5.noarch.rpm
  curl -s -L http://dl.fedoraproject.org/pub/epel/7/x86_64/e/epel-release-7-5.noarch.rpm -o epel-release-7-5.noarch.rpm
  if [ -f /usr/bin/dnf ]; then
    dnf -q -y install epel-release-7-5.noarch.rpm
  elif [ -f /usr/bin/yum ]; then
    yum -q -y install epel-release-7-5.noarch.rpm
  fi
  rm -f epel-release-7-5.noarch.rpm
 fi
 if [ "${DIST}" = "el6" ]; then
  echo Installing epel-release-6-8.noarch.rpm
  curl -s -L http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm -o epel-release-6-8.noarch.rpm
  if [ -f /usr/bin/dnf ]; then
    dnf -q -y install epel-release-6-8.noarch.rpm
  elif [ -f /usr/bin/yum ]; then
    yum -q -y install epel-release-6-8.noarch.rpm
  fi
  rm -f epel-release-6-8.noarch.rpm
 fi
fi

if [ -f /usr/bin/dnf ]; then
	echo -n "Recommend performing dnf upgrade? (y/N) "
	read INPUTTEXT < /dev/tty
	if [ "${INPUTTEXT}" = "y" ] || [ "${INPUTTEXT}" = "Y" ]; then
		dnf upgrade < /dev/tty
	fi
	echo Installing DeployHub
	dnf -y install *.rpm
elif [ -f /usr/bin/yum ]; then
	echo -n "Recommend performing yum upgrade? (y/N) "
	read INPUTTEXT < /dev/tty
	if [ "${INPUTTEXT}" = "y" ] || [ "${INPUTTEXT}" = "Y" ]; then
		yum upgrade < /dev/tty
	fi
	echo Installing DeployHub
	yum -y install *.rpm
fi

echo
echo "*****************************************************"
echo
IPADDR=`ip -f inet -o addr show | grep -v 127.0.0.1 | head -1 | awk '{split($4,a,"/") ;print a[1]}'`
echo "Access DeployHub using the following:"
echo
echo "     URL: http://$IPADDR:8080/dmadminweb/Home"
echo "  UserId: admin"
echo "Password: admin"
echo
echo "*****************************************************"
echo
