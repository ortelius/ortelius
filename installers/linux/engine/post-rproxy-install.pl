#!/usr/bin/perl

###############################################################
#  post-install.sh
#  2/5/2015 - QAB
#
#  - Determine OS
#  - Create 'omreleng' user
#  - Configure default unixODBC Data Source.  May require
#      updating PostgreSQL odbc driver file in odbcinst.ini
#  - Update pg_hba.conf file to trust local connections
#      and restart/reload postgres server
#  - Enable and start deployhub-engine service for systemd

use strict;
use File::Copy qw(mv);
use File::Find;

#Declare some things up front
my $re_user = "omreleng";
my ($linux, $linux_version, $systemd, $RC, $pg_data_dir);
my $redhat_os_file = '/etc/redhat-release';
my $lsb_os_file = '/etc/lsb-release';
my $re_home='/opt/deployhub/engine';
my $logs='/opt/deployhub/engine/log';

#Get Linux distro and version
#Currently we only support Redhat and Debian variants

if (-f $redhat_os_file)
{
  $linux = 'redhat';
  open my $fh, '<:encoding(UTF-8)', $redhat_os_file or die;
  while (my $line = <$fh>)
  {
    if ($line =~ /release\s+([0-9\.]*)\s+/)
    {     # Match "release" plus a series of numbers and dots
      $linux_version = $1;   # assign the numbers and dots to $linux_version
      my @version_numbers = split (/\./,$linux_version);
      $systemd = 1 if ($version_numbers[0] >= 7);
    }
  }
}
elsif (-f $lsb_os_file)
{
  open my $fh, '<:encoding(UTF-8)', $lsb_os_file or die;
  while (my $line = <$fh>)
  {
    if ($line =~ /DISTRIB_ID=(.*)$/)
    {
	   $linux = $1 if ($1 =~ /ubuntu|debian/i);
    }
	elsif ($line =~ /DISTRIB_RELEASE=(.*)$/)
	{
	   $linux_version = $1;
	}
  }
}

#Create omreleng user if it doesn't already exist
unless (getpwnam $re_user)
{
  my $user_add_cmd = "useradd";
  $user_add_cmd = "adduser" if ($linux =~ m/ubuntu|debian/i);
  run_cmd("$user_add_cmd $re_user");
}

unless ($linux)
{
 die "\tFATAL: Either this script did not correctly deteremine your OS or your OS is not supported by this setup script.\n\tCurrently only Debian and Redhat Linux variants are supported\n\tPlease consult the product documentation or support for help";
}

my $pg_home = '/var/lib';

#Configure RE-PostgreSQL ODBC data source

my @lines2;
open (T,"<$re_home/odbc.template");
@lines2 = <T>;
close(T);

open (T,">$re_home/odbc.template");

foreach my $line (@lines2)
{
 $line =~ s/Servername=localhost/Servername=db\.deployhub\.com/g;
	$line =~ s/Password=postgres/Password=/g;
	$line =~ s/Port=5432/Port=80/g;
	print T $line;
}

close(T);

print "Configuring ODBC data source \'RE-PostgreSQL\' ....\n";
print `cat /etc/odbcinst.ini $re_home/odbc.template > /etc/odbcinst.ini.new`;
print `cp /etc/odbcinst.ini.new /etc/odbc.ini`;

system("mkdir $logs");
# system("chown -R omreleng:omreleng $re_home");
print "Installation complete!\n";
exit (0);


sub run_cmd
{
  my $cmd = shift;
  my @cmd_out = `$cmd 2>&1`;
  my $RC = $?;
  if ($RC)
  {
    print "Command \'$cmd\' failed:\n";
	print "$_" foreach (@cmd_out);
  }
  return $RC;
}
