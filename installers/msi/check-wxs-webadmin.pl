use strict;
use File::Find;
use Cwd 'abs_path';
use File::Basename;

chdir(dirname(abs_path($0))) or die "Can't change to script directory: $!";

open WXS, "ReleaseEngineer.wxs";
my @wxs_lines = <WXS>;
close WXS;
my @missing_files;
find(\&CompareWXSWebAdmin,'WebAdmin');

if (scalar @missing_files)
{
 print "Missing file(s):\n";
 print "\t$_\n" for (@missing_files);
 exit 1;
}
else
{
 print "\nAll good\n";
 exit 0;
}


sub CompareWXSWebAdmin {

 my $file = $_;

 #return unless $class_file =~ /\$/;

 my $found_file = 0;
 foreach my $line (@wxs_lines)
 {
   next unless $line =~ m{\Q$file\E}i;
   $found_file = 1;
   last;
 }
 push (@missing_files, $File::Find::name) unless ($found_file)
}