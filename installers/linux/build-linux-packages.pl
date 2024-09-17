#/usr/bin/perl
use strict;
use File::Find;
use Openmake::File;
use File::Copy 'cp', 'mv';
use File::Copy::Recursive 'dircopy';
use File::Remove 'remove';
use Cwd;

my @build_file_list = ( 'bin/dm', 'bin/listener', 'bin/libdmapi.so', 'bin/libharvest.so', 'bin/libhttp.so', 'bin/libiisplugin.so', 'bin/libperforce.so', 'bin/librestful.so',
			 'lib/dmtransfer', 'lib/libftplib.so', 'lib/libpsftplib.so', 'lib/libwinlib.so', 'trilogyd', 'trilogycli', 'setodbc', 'lib/libtrilogycli.so', 'lib/libtrilogy.so');
my @static_file_list = ( 'dm.odbc', 'dm.asc', 'trilogy.conf', 'trilogy.lic', 'README.txt', 'odbc.template', 'post-install.sh', 'post-install.pl', 're-engine.service', 're-engine.sh');
my @webadmin_static_files = ('re-webadmin.sh', 're-webadmin.service', 'post-install.sh', 'post-install.pl');
my @webadmin_build_files = ('re-webadmin.war');
my @wildcard_dir_list = ('demo', 'demorep', 'scripts','lib');
my $build_dir = '/ombuild/751/linux-rocket/release-engineer/engine-64/DEBUG';
my $webadmin_build_dir = '/ombuild/751/linux-rocket/release-engineer/webadmin';
my $git_files_dir = $ENV{REF_DIR} . '/installers/linux';
my $engine_dir = 'engine';
my $webadmin_dir = 'webadmin';
my $package_prefix = 'opt/release-engineer';
my $RC;


my $orig_cwd = cwd();
print "Cleaning current directory \'$orig_cwd\' ...\n";
remove (\1, '*');

mkdir ($engine_dir);
chdir ($engine_dir);
my $cwd = cwd();

print "Copying installer files to current directory ...\n";
cp ($git_files_dir . '/engine/' . $_,  $cwd) foreach (@static_file_list);

foreach my $file (@build_file_list)
{
 die "Can't find file \'$file\' in build directory \'$build_dir\': $!" unless (-f $build_dir . '/' . $file);
 my $dir = ".";
 if ($file =~ m/(.*)\/.*$/)
 {
  $dir = $1;
  mkdir ($dir) unless (-d $dir);
 }
 cp ($build_dir . '/' . $file, $dir);
}

foreach my $dir (@wildcard_dir_list)
{
 my $src_dir = $git_files_dir . '/engine/' . $dir;
 die "Can't find source directory \'$src_dir\': $!" unless (-d $src_dir);
 dircopy ($src_dir, $dir);
}

# Now copy the webadmin files
chdir ($orig_cwd);
mkdir ($webadmin_dir);
chdir ($webadmin_dir);
$cwd = cwd();
cp ($git_files_dir . '/webadmin/' . $_,  $cwd) foreach (@webadmin_static_files);
cp ($webadmin_build_dir . '/' . $_,  $cwd) foreach (@webadmin_build_files);

# Run commands to create packages
print "Building Linux packages ...\n";
chdir ($orig_cwd);
foreach my $component ("engine", "webadmin")
{
  foreach my $pack_type ("rpm", "deb")
  {
   my $fpm_cmd, my $pack_file, my @deps;

   my $pack_name = "re-$component";

   if ($component eq "engine")
   {
     @deps = ( "unixODBC > 2.1", "postgresql-server > 9.2", "postgresql-odbc > 9.2", "samba-client > 4.0", "perl > 5.10", "sshpass > 1.0", "libiodbc > 3.50", "glibc(x86-32)", "libstdc++(x86-32)", "openvas-smb", "gnutls", "ansible" );
   }
   else #webadmin
   {
     @deps = ( "re-engine >= 7.7.0", "java-1.8.0-openjdk > 1.8.0.0");
   }

   my $fpm_deps = join ('" -d "', @deps);
   $fpm_deps = "-d \"$fpm_deps\"";

   if ($pack_type eq "rpm")
   {
     $fpm_cmd =  "fpm -s dir -t $pack_type -n $pack_name -v 7.7.0 --rpm-defattrfile \"755\" --after-install $component/post-install.sh --prefix /opt/release-engineer $fpm_deps $component";
     $pack_file = $pack_name . "-7.7.0-1.x86_64.rpm";
   }
   elsif ($pack_type eq "deb")
   {
     $fpm_cmd =  "fpm -s dir -t $pack_type -n $pack_name -v 7.7.0 --after-install $component/post-install.sh --prefix /opt/release-engineer $fpm_deps $component";
     $pack_file = $pack_name . "_7.7.0_amd64.deb";
   }

   print "\n\nBuilding package \'$pack_file\' ...\nCurrent working directory is: " . cwd() . "\nfpm command is: $fpm_cmd\n\n";
   my @fpm_out = `$fpm_cmd 2>&1`;
   $RC = $?;
   print "Error running fpm command:\n" if $RC;
   print $_ foreach(@fpm_out);
   mv ($pack_file, $orig_cwd) or warn "Can't move file \'$pack_file\' to $orig_cwd: $!";
   print "Package \'$pack_file\' complete!\n\n"
  }
}

print "Complete!\n";
$RC ? exit (1) : exit(0);
