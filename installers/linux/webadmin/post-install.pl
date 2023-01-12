#!/usr/bin/perl

###############################################################
#  post-install.sh
#  1/21/2015 - QAB
#  - Enable deployhub-webadmin service for systemd

use File::Copy qw(mv);

#Declare some things up front
my ($linux, $linux_version, $systemd, $RC);
my $redhat_os_file = '/etc/redhat-release';
my $lsb_os_file = '/etc/lsb-release';
my $webadmin_home='/opt/deployhub/webadmin';

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

unless ($linux)
{
 die "\tFATAL: Either this script did not correctly deteremine your OS or your OS is not supported by this setup script.\n\tCurrently only Debian and Redhat Linux variants are supported\n\tPlease consult the product documentation or support for help";
}


# If this system uses systemd, move deployhub-webadmin.service file into place and enable.
# Otherwise, notify user that init.d script will have to be configured manually

if ($systemd)
{
  print "Installing webadmin listener as systemd service ...\n";
  mv ($webadmin_home . '/deployhub-webadmin.service', '/usr/lib/systemd/system') or die "Can't move deployhub-webadmin.service file into \'/usr/lib/systemd/system\': $!\n";

  my @systemctl_cmds = ("systemctl daemon-reload", "systemctl enable deployhub-webadmin", "systemctl start deployhub-webadmin");
  foreach my $cmd (@systemctl_cmds)
  {
    my @cmd_out = `$cmd 2>&1`;
    $RC = $?;
    if ($RC)
    {
      print "Command \'$cmd\' failed:\n";
	  print "$_" foreach (@cmd_out);
    }
  }
}
else
{
 print "This system does not use systemd.  In order for the DeployHub webadmin to run as a service, you will need to manually configure the init script. Please consult the product documentation or support for help\n";
}

system("chown -R omreleng:omreleng $webadmin_home");
print "Installation complete!\n";
exit (0);
