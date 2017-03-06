use strict;
use File::Find;
use Cwd 'abs_path';
use File::Basename;
use Data::GUID;
  
my @missing_files = <>;
sort @missing_files;


foreach (@missing_files)
{
 chomp $_;
 $_ =~ s{\/}{\\}g;
 my $file = basename($_);
 
 my $file_mixed_case = $file;
 my $file_in_caps = "\U$file";
 my $path_mixed_case =  "$_";
 my $uuid = Data::GUID->new();
 
 my $id =  $file_in_caps =~ m/^[A-Z_]/ ? $file_in_caps : '_' . $file_in_caps;
 my $wxs_xml = <<WXS;
											<Component Id="$id" DiskId="1" Guid="$uuid">
												<File Id="$id" Name="$file_mixed_case" Source="$path_mixed_case" />
											</Component>
WXS
 
 print $wxs_xml;  
}


foreach (@missing_files)
{
 my $file = basename($_);
 chomp $file;
 my $file_in_caps = "\U$file";
 my $id =  $file_in_caps =~ m/^[A-Z_]/ ? $file_in_caps : '_' . $file_in_caps;
 my $wxs_xml = <<WXS;
			<ComponentRef Id="$id"/>
WXS
 
 print $wxs_xml;  
}




# chdir(dirname(abs_path($0))) or die "Can't change to script directory: $!";

# open WXS, "ReleaseEngineer.wxs";
# my @wxs_lines = <WXS>;
# close WXS;
# my @missing_files;
# find(\&CompareWXSWebAdmin,'WebAdmin');

# if (scalar @missing_files)
# {
 # print "Missing file(s):\n";
 # print "\t$_\n" for (@missing_files);
 # exit 1;
# }
# else
# {
 # print "\nAll good\n";
 # exit 0;
# }

# sub CompareWXSWebAdmin {

 # my $file = $_;


 # my $found_file = 0;
 # foreach my $line (@wxs_lines)
 # {
   # next unless $line =~ m{\Q$file\E}i;
   # $found_file = 1;
   # last;
 # }
 # push (@missing_files, $File::Find::name) unless ($found_file)
# }