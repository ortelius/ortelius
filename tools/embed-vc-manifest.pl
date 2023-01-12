#!/bin/perl

# QAB 08/22/2014
use strict;
use Cwd;

my $cwd = cwd();


print "Embedding manifest files for VC++ native binaries\n"
my $dir = $ENV{CFG} ? $ENV{CFG} : "RELEASE";
opendir DIR, "$dir" or die $!;

while (my $file = readdir(DIR)) {

	next if ($file =~ m/^\./);
	next unless ($file =~ m/\.dll|\.exe$/);

	if (-f "$file")
	{
	  my $bin_type = $file =~ /\.exe/ ? "1" : "2";
	  my $manifest_file = $file . '.manifest';
	  my $mt_cmd = "mt.exe –manifest $manifest_file -outputresource:$file;$bin_type";
      print "$mt_cmd\n";
      @mt_out = `$mt_cmd`;
      $RC = $?;
      die "Failed to embed manmifest for file \'$file\' via mt.exe:\n\n@mt_out" if $RC;
	}
	else
	{

	}
}

print "Completed binary manifest embedding successfully.\n";
