#!/bin/perl
use strict;
use warnings;
use POSIX qw(strftime);

my $build_number = $ARGV[0];
my $date_string = strftime '%Y%m%d', gmtime();
my $build_string = $date_string . '_' . $build_number;

my $web_xml = $ENV{'REF_DIR'} . "dmadminweb/WebContent/WEB-INF/web.xml";

open (OLD, $web_xml);
my @lines = <OLD>;
close OLD;

foreach my $line (@lines)
{
  next unless ($line =~ /\>X_BUILD_VERSION_X\</);
  $line =~ s{X_BUILD_VERSION_X}{\Q$build_string\E};
}

open (NEW, ">$web_xml");
print (NEW $_) foreach (@lines);
close NEW;
exit 0;
