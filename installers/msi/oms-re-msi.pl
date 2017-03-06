use strict;
use warnings;

use Archive::Zip;
use Archive::Extract;
use File::Copy;
use File::Find;
use File::Path qw(remove_tree);
use Cwd 'abs_path';
use File::Basename;
use syntax 'junction';

#TODO: Detect missing files for Engine

# First copy the latest ReleaseEngineer.wxs file to the cwd

my $re_wxs = 'E:/omci/OMS-RE/installers/msi/ReleaseEngineer.wxs';
copy ($re_wxs, '.') or die "Can't copy ReleaseEngineer.wxs from git repo :$!";

# Clean WebAdmin/tomcat/webapps/dmadminweb and extract latest .war file

my $war_file = "E:/ombuild/dmadminweb/dmadminweb.war";
print "Cleaning old WebAdmin/tomcat/webapps/dmadminweb directory and extracting $war_file ...\n";

if (-d 'WebAdmin/tomcat/webapps/dmadminweb')
{
 die "Can't delete dmadminweb directory: $!\n" unless remove_tree( 'WebAdmin/tomcat/webapps/dmadminweb' );
}

my $ae = Archive::Extract->new( archive => $war_file );
my $ok = $ae->extract( to => 'WebAdmin/tomcat/webapps/dmadminweb');
die "Can't extract new dmadminweb.war file: $ae->error()\n" unless $ok;

# Read build number from web.xml

print "Reading build number from dmadminweb.war web.xml file ...\n";
open WEBXML, "WebAdmin/tomcat/webapps/dmadminweb/WEB-INF/web.xml";
my @webxml_lines = <WEBXML>;
close WEBXML;
my $job_number = '';

foreach my $line (@webxml_lines)
{
 $job_number = $1 if $line =~ m{<param-value>\d{8}_(\d{3,})</param-value>};
}
die "Cannot read build number from dmadminweb web.xml file!" unless $job_number;
print "Extracted build number: $job_number\n";
 
# Compare file system to .wxs file to see
#   if anything is missing

print "Reading ReleaseEngineer.wxs file to search for missing file references ...\n";
open WXS, "ReleaseEngineer.wxs";
my @wxs_lines = <WXS>;
close WXS;
my @missing_files;
find(\&CompareWXSWebAdmin,'WebAdmin');

if (scalar @missing_files)
{
 print "File(s) in WebAdmin missing from ReleaseEngineer.wxs file:\n";
 print "\t$_\n" for (@missing_files);
 exit 1;
}
else
{
 print "ReleaseEngineer.wxs source and WebAdmin directory comparison OK\n";
}

print "Writing build number $job_number into ReleaseEngineer.wxs file ...\n\n";
open WXS, ">ReleaseEngineer.wxs";
my $token = '_JOB_NUMBER_';
foreach my $line (@wxs_lines)
{
 $line =~ s{\Q$token\E}{$job_number};
 print WXS $line;
}
close WXS;

# Copy latest binaries to Engine directories

my $engine_dir = "E:/ombuild/release-engineer-engine/RELEASE";
my @lib_dir_binaries = ('dmtransfer.exe', 'psftplib.dll', 'ftplib.dll', 'sftplib.dll', 'sshexec.exe', 'winexec.exe', 'winlib.dll');
my @root_dir_binaries = ('trilogyserver.exe', 'trilogytty.exe', 'trinotify.exe', 'xdr.dll');

opendir BIN, "$engine_dir" or die $!;
while (my $file = readdir(BIN)) {

	next if ($file =~ m/^\./);
	next unless ($file =~ m/(\.dll|\.exe)$/);
    my $abs_file = "$engine_dir\/$file";
	
	if (-f "$abs_file")
	{
      if (any(@lib_dir_binaries) eq $file)
	  {
	    copy ($abs_file, 'Engine/lib') or die "Can't copy file $file:$!";
	  }
      elsif (any(@root_dir_binaries) eq $file)
	  {
	    copy ($abs_file, 'Engine') or die "Can't copy file $file:$!";
	  }	  
	  else
	  {
	    copy ($abs_file, 'Engine/bin') or die "Can't copy file $file:$!";
	  }
	}
	else
	{
	  warn "Can't access binary file $abs_file:$!\n";
	}
}

closedir BIN;

# Copy latest scripts from Git dmengine/scripts directory

my $scripts_dir = "E:/omci/OMS-RE/dmengine/scripts";
opendir SCRIPTS, "$scripts_dir" or die $!;
while (my $file = readdir(SCRIPTS)) {
  next if ($file =~ m/^\./);
  my $abs_file = "$scripts_dir\/$file";
  if (-f "$abs_file")
  {
    copy ($abs_file, 'Engine/scripts') or die "Can't copy file $file:$!";
  }
  else
  {
    warn "Can't access binary file $abs_file:$!\n";
  }
}

my $candle_cmd = '"C:\Program Files (x86)\WiX Toolset v3.8\bin\candle.exe" -nologo ReleaseEngineer.wxs -out ReleaseEngineer.wixobj  -ext WixUtilExtension -ext WixUIExtension';
print "Executing candle command: $candle_cmd\n";
my @candle_out = `$candle_cmd`;
my $RC =  $?;
print $RC ? "\nRunning Candle failed:@candle_out\n" :  "\nCandle suceeded!\n\n@candle_out\n";
die if $RC;
$RC = 0;

my $light_cmd = '"C:\Program Files (x86)\WiX Toolset v3.8\bin\light.exe" -nologo ReleaseEngineer.wixobj -out ReleaseEngineer-build' . $job_number . '.msi  -ext WixUtilExtension -ext WixUIExtension';
print "Executing light command: $light_cmd \n";
my @light_out = `$light_cmd`;
$RC =  $?;
print $RC ? "\nRunning Light failed:@light_out\n" :  "\nLight suceeded!\n\n@light_out\n";
die if $RC;


unlink("E:\\ombuild\\re-installers\\winscp.log");
### 2014.11.17 - QAB - Tack on WinSCP copy to server, since Meister job can't inherit job number found in web.xml file
my $winscp_cmd = "perl -S oms-winscp.pl put ReleaseEngineer-build$job_number.msi /home/openmake/public_html/qa";
print "Executing winscp perl command: $winscp_cmd \n";
my @winscp_out = `$winscp_cmd`;
$RC =  $?;
print $RC ? "\nRunning winscp failed:@winscp_out\n" :  "\nWinSCP suceeded!\n\n@winscp_out\n";
die if $RC;


### 2015.01.16 - QAB - Symbolic link ReleaseEngineer-build$job_number.msi to ReleaseEngineer.msi
$winscp_cmd = "perl -S oms-winscp.pl rm /home/openmake/public_html/qa/ReleaseEngineer.msi";
print "Executing winscp perl command: $winscp_cmd \n";
@winscp_out = `$winscp_cmd`;
$RC =  $?;
print $RC ? "\nRunning winscp failed:@winscp_out\n" :  "\nWinSCP suceeded!\n\n@winscp_out\n";
die if $RC;

$winscp_cmd = "perl -S oms-winscp.pl ln /home/openmake/public_html/qa/ReleaseEngineer-build$job_number.msi /home/openmake/public_html/qa/ReleaseEngineer.msi";
print "Executing winscp perl command: $winscp_cmd \n";
@winscp_out = `$winscp_cmd`;
$RC =  $?;
print $RC ? "\nRunning winscp failed:@winscp_out\n" :  "\nWinSCP suceeded!\n\n@winscp_out\n";
die if $RC;



sub CompareWXSWebAdmin {

 my $file = $_;
 my $found_file = 0;
 $file =~ s{\/}{\\}g;
 
 foreach my $line (@wxs_lines)
 {
   my $work_line = $line;
   $work_line =~ s{\/}{\\}g;
   next unless $work_line =~ m{\Q$file\E}i;
   $found_file = 1;
   last;
 }
 push (@missing_files, $File::Find::name) unless ($found_file)
}