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

#Find Postgres home directory
my @echo_out = `sudo -u postgres -i eval \'echo \$HOME\'`;
warn "Could not determine postgres home directory" if $?;
my $pg_home = $echo_out[0];
$pg_home = '/var/lib' unless ($pg_home);
chomp $pg_home;

#Find PostgreSQL data directory, if it exists
our ($pg_data_dir, $pg_data_parent);
find (\&find_pg_data, $pg_home);

#pg_ctl is not strictly required but often is the only way to perform initdb
our ($pg_ctl, $pg_driver);
foreach ('/bin/pg_ctl', '/usr/bin/pg_ctl')
{
  $pg_ctl = $_ if (-f $_);
}
unless ($pg_ctl)
{
  find (\&find_pg_ctl, '/usr');
  die "Unable to find \'pg_ctl\' executable to initialize PostgtresSQL DB" unless ($pg_ctl);
}

unless ($pg_data_dir)
{
 $pg_data_parent = $pg_home unless ($pg_data_parent);
 $pg_data_dir = $pg_data_parent . '/data';
 print "No PostgreSQL database found.  Initializing new PostgreSQL database at: \'$pg_data_dir\' ...\n";
 my $pg_ctl_cmd = 'sudo -u postgres ' . $pg_ctl . ' initdb --pgdata=' . $pg_data_dir;
 $RC = run_cmd($pg_ctl_cmd);
}

#Replace driver location in odbcinst.ini file if default referenced file is not found
my $odbcinst = '/etc/odbcinst.ini';
my $odbcinst_bak = $odbcinst . '.bak';
my $replace_driver;
my @out_lines;

open my $in, '<', $odbcinst or die "Can't read odbcinst.ini file : $!";
my $out;

while( <$in> )
{
  if ($_ =~ /Driver64/)
  {
    my @split_line = split /=/, $_ ;
	my $driver_tmp = $split_line[1];
	$driver_tmp =~ s/^\s+|\s+$//g;
	unless (-f $driver_tmp)
	{
      find (\&find_pg_driver, '/usr');
	  $replace_driver = 1;
	  s/\Q$driver_tmp\E/$pg_driver/g;
	}
  }
  push (@out_lines, $_);
}

if ($replace_driver)
{
  print "Updating unixODBC odbcinst.ini file with PostgreSQL odbc driver ...\n";
  mv ($odbcinst, $odbcinst_bak) or die "Can't rename old odbcinst.ini file for backup: $!";
  open $out, '>', $odbcinst_bak or die "Can't write new file: $!";
  print $out $_ foreach @out_lines;
  close $out;
}

#Configure RE-PostgreSQL ODBC data source
print "Configuring ODBC data source \'RE-PostgreSQL\' ....\n";
my $odbc_cmd = 'odbcinst -i -s -l -f ' . $re_home . '/odbc.template';
$RC = run_cmd($odbc_cmd);

#Replace 'ident' with 'trust' in pg_hba.conf file
print "Updating pg_hba.conf file to trust local connections ...\n";
my $pg_conf = $pg_data_dir . '/pg_hba.conf';
my $pg_conf_bak = $pg_conf . '.bak';

mv ($pg_conf, $pg_conf_bak) or die "Can't rename pg_hba.conf file: $!";

open $in, '<', $pg_conf_bak or die "Can't read pg_hba_conf file : $!";
open $out, '>', $pg_conf or die "Can't write new file: $!";

while( <$in> )
{
  s/\b(ident)\b/trust/g;
  print $out $_;
}
close $out;
chown ("postgres","postgres",$pg_conf);


#Enable postgres server
$systemd ? my $service_cmd = "systemctl enable postgresql" : "chkconfig postgresql on";
my @service_out = `$service_cmd 2>&1`;

#Reload postgres server to pick up changes to pg_hba.conf or start it if it's not running
$systemd ? my $service_cmd = "systemctl status postgresql" : "service postgresql status";
my @service_out = `$service_cmd 2>&1`;
$RC = $?;
if ($RC)
{
 print "PostgreSQL server not running, starting now ...\n";
 $systemd ? my $start_cmd = "systemctl start postgresql" : "service postgresql start";
 my $RC = run_cmd($start_cmd);
}
else
{
 print "Reloading PostgresSQL server\n";
 $systemd ? my $reload_cmd = "systemctl reload postgresql" : "service postgresql reload";
 $RC = run_cmd($reload_cmd);
}

# If this system uses systemd, move deployhub-engine.service file into place and enable.
# Otherwise, notify user that init.d script will have to be configured manually

if ($systemd)
{
  print "Installing engine listener as systemd service ...\n";
  mv ($re_home . '/deployhub-engine.service', '/usr/lib/systemd/system') or die "Can't move deployhub-engine.service file into \'/usr/lib/systemd/system\': $!\n";

  my @systemctl_cmds = ("systemctl daemon-reload", "systemctl enable deployhub-engine", "systemctl start deployhub-engine");
  run_cmd ($_) foreach (@systemctl_cmds);
}
else
{
 print "This system does not use systemd.  In order for the Release Engineer engine listener to run as a service, you will need to manually configure the \'deployhub-engine.sh\' init script.\nPlease consult the product documentation or support for help\n";
}
system("mkdir $logs");
system("chown -R omreleng:omreleng $re_home");
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

sub find_pg_data
{
 #  Take data parent just in case we can't find an pg_hba.conf file.
 #  This directory is normally a version number subdirectory of pg_home
 $pg_data_parent = $File::Find::dir if ($File::Find::dir =~ /$pg_home\/\d\.\d/);
 $pg_data_dir    = $File::Find::dir if ($_ eq "pg_hba.conf");
}

sub find_pg_ctl
{
 $pg_ctl = $File::Find::name if ($_ eq "pg_ctl");
}

sub find_pg_driver
{
 next unless ($_ =~ /^psqlodbc\w?\.so/);
 $pg_driver = $File::Find::name if ($_ eq "psqlodbc.so" || $_ eq "psqlodbca.so" || $_ eq "psqlodbcw.so");  #So far these are the known good names for the driver file.  Don't use psqlodbcS.so; it's a setup lib
}
