use strict;
use warnings;
use Cwd;



# my $log_file = 'E:\ombuild\re-installers\winscp.log';
# my $src_file = 'E:\ombuild\re-installers\ReleaseEngineer.msi'
# my $dest_file = '/home/openmake/public_html/qa/ReleaseEngineer.msi'

my $cwd = cwd();
$cwd =~ s{\/}{\\}g;

my $cmd = $ARGV[0];
my $filename = $ARGV[1]; 
my $dest_file = $ARGV[2];
my $src_file;


my $log_file =  $cwd . "\\winscp.log" ;
if ($cmd eq "put")
{
  $src_file = $cwd . "\\" . $filename;
  $dest_file = $ARGV[2] . '/' . $filename;
}
else
{
  $src_file = $filename;
}

my $RC = 0;

$cmd =~ s/ln/call ln -s/;
print "$cmd\n";

my $winscp_cmd = '"C:\Program Files (x86)\WinSCP\WinSCP.exe" /console /command "option batch abort" "option confirm off" "open oms-qa" "' . $cmd . ' ' . $src_file . ' ' . $dest_file . '" "exit" /log=' . $log_file;
my @winscp_out = `$winscp_cmd`;
$RC = $?;

open LOG , $log_file;
my @log_lines = <LOG>;
print $RC ? "Error running WinSCP:\n@log_lines" : "WinSCP completed successfully!\n";
