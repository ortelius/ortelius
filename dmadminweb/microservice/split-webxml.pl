#!/usr/bin/perl

# use module
use XML::Simple;
use Data::Dumper;
use Cwd;
use Storable qw(dclone);
$webxml_in = $ARGV[0];
$mapping_in = $ARGV[1];
$outputdir = $ARGV[2];
$tgt = $ARGV[3];

# create object
$xml = new XML::Simple;

# read XML file
$data = $xml->XMLin($webxml_in);

open(FP,"<$mapping_in");
@lines = <FP>;
close(FP);

@zones = qw(default);

foreach $zone (@zones)
{

	mkdir($outputdir . "/location");

	foreach $line (@lines)
	{
		$line =~ s/\n|\r//g;
		next if ($line !~ /:/);

		@parts = split(/:/,$line);
		$key = $parts[0];
		$dhport = $parts[1];
		$val = $parts[2];

		$dhhost = "$key.$zone.svc.cluster.local";
		$backend = "$key-backend";

	 $loc = "location /dmadminweb/$val {\n".
	       "   proxy_http_version 1.1;\n" .
	       "   proxy_set_header Connection \"\";\n" .
	       "   proxy_set_header X-Forwarded-Host \$host;\n" .
	       "   proxy_set_header X-Forwarded-Server \$host;\n" .
	       "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
	       "   proxy_pass http://$backend/dmadminweb/" . $val . ";\n" .
								"\n" .
				    "   set \$cors '';\n" .
				    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
				    "           set \$cors 'true';\n" .
				    "   }\n" .
        "\n" .
				    "   if (\$cors = 'true') {\n" .
				    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
				    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
				    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
				    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
				    "           # required to be able to read Authorization header in frontend\n" .
				    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
				    "   }\n" .
        "\n" .
				    "   if (\$request_method = 'OPTIONS') {\n" .
				    "           # Tell client that this pre-flight info is valid for 20 days\n" .
				    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
				    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
				    "           add_header 'Content-Length' 0;\n" .
				    "           return 204;\n" .
				    "   }\n" .
								"}\n";


		if ($val =~ /Home/)
	 {
		 $loc .= "\n";
		 $loc .= "location /dmadminweb/css {\n".
         "   proxy_http_version 1.1;\n" .
         "   proxy_set_header Connection \"\";\n" .
		       "   proxy_set_header X-Forwarded-Host \$host;\n" .
		       "   proxy_set_header X-Forwarded-Server \$host;\n" .
		       "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
		       "   proxy_pass http://$backend/dmadminweb/css;\n" .
									"\n" .
					    "   set \$cors '';\n" .
					    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
					    "           set \$cors 'true';\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$cors = 'true') {\n" .
					    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
					    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
					    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
					    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
					    "           # required to be able to read Authorization header in frontend\n" .
					    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$request_method = 'OPTIONS') {\n" .
					    "           # Tell client that this pre-flight info is valid for 20 days\n" .
					    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
					    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
					    "           add_header 'Content-Length' 0;\n" .
					    "           return 204;\n" .
					    "   }\n" .
									"}\n";

			$loc .= "\n";
			$loc .= "location /dmadminweb/js {\n".
         "   proxy_http_version 1.1;\n" .
         "   proxy_set_header Connection \"\";\n" .
			      "   proxy_set_header X-Forwarded-Host \$host;\n" .
			      "   proxy_set_header X-Forwarded-Server \$host;\n" .
			      "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
			      "   proxy_pass http://$backend/dmadminweb/js;\n" .
									"\n" .
					    "   set \$cors '';\n" .
					    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
					    "           set \$cors 'true';\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$cors = 'true') {\n" .
					    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
					    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
					    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
					    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
					    "           # required to be able to read Authorization header in frontend\n" .
					    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$request_method = 'OPTIONS') {\n" .
					    "           # Tell client that this pre-flight info is valid for 20 days\n" .
					    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
					    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
					    "           add_header 'Content-Length' 0;\n" .
					    "           return 204;\n" .
					    "   }\n" .
									"}\n";

			$loc .= "\n";
			$loc .= "location /dmadminweb/images {\n".
         "   proxy_http_version 1.1;\n" .
         "   proxy_set_header Connection \"\";\n" .
			      "   proxy_set_header X-Forwarded-Host \$host;\n" .
			      "   proxy_set_header X-Forwarded-Server \$host;\n" .
			      "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
						   "   proxy_pass http://$backend/dmadminweb/images;\n" .
									"\n" .
					    "   set \$cors '';\n" .
					    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
					    "           set \$cors 'true';\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$cors = 'true') {\n" .
					    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
					    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
					    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
					    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
					    "           # required to be able to read Authorization header in frontend\n" .
					    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$request_method = 'OPTIONS') {\n" .
					    "           # Tell client that this pre-flight info is valid for 20 days\n" .
					    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
					    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
					    "           add_header 'Content-Length' 0;\n" .
					    "           return 204;\n" .
					    "   }\n" .
									"}\n";

			$loc .= "\n";
			$loc .= "location /dmadminweb {\n".
         "   proxy_http_version 1.1;\n" .
         "   proxy_set_header Connection \"\";\n" .
			      "   proxy_set_header X-Forwarded-Host \$host;\n" .
			      "   proxy_set_header X-Forwarded-Server \$host;\n" .
			      "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
				     "   proxy_pass http://$backend/dmadminweb;\n" .
									"\n" .
					    "   set \$cors '';\n" .
					    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
					    "           set \$cors 'true';\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$cors = 'true') {\n" .
					    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
					    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
					    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
					    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
					    "           # required to be able to read Authorization header in frontend\n" .
					    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$request_method = 'OPTIONS') {\n" .
					    "           # Tell client that this pre-flight info is valid for 20 days\n" .
					    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
					    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
					    "           add_header 'Content-Length' 0;\n" .
					    "           return 204;\n" .
					    "   }\n" .
									"}\n";

			$loc .= "\n";
			$loc .= "location = / {\n".
         "   proxy_http_version 1.1;\n" .
         "   proxy_set_header Connection \"\";\n" .
			      "   proxy_set_header X-Forwarded-Host \$host;\n" .
			      "   proxy_set_header X-Forwarded-Server \$host;\n" .
			      "   proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;\n" .
				     "   proxy_pass http://$backend/dmadminweb/Home/;\n" .
									"\n" .
					    "   set \$cors '';\n" .
					    "   if (\$http_origin ~ '^https?://(localhost|console\.deployhub\.com|www\.deployhub\.com|deployhub\.com|www.\.jsfiddle\.net)') {\n" .
					    "           set \$cors 'true';\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$cors = 'true') {\n" .
					    "           add_header 'Access-Control-Allow-Origin' \"\$http_origin\" always;\n" .
					    "           add_header 'Access-Control-Allow-Credentials' 'true' always;\n" .
					    "           add_header 'Access-Control-Allow-Methods' 'GET, POST, PUT, DELETE, OPTIONS' always;\n" .
					    "           add_header 'Access-Control-Allow-Headers' 'Accept,Authorization,Cache-Control,Content-Type,DNT,If-Modified-Since,Keep-Alive,Origin,User-Agent,X-Requested-With' always;\n" .
					    "           # required to be able to read Authorization header in frontend\n" .
					    "           #add_header 'Access-Control-Expose-Headers' 'Authorization' always;\n" .
					    "   }\n" .
         "\n" .
					    "   if (\$request_method = 'OPTIONS') {\n" .
					    "           # Tell client that this pre-flight info is valid for 20 days\n" .
					    "           add_header 'Access-Control-Max-Age' 1728000;\n" .
					    "           add_header 'Content-Type' 'text/plain charset=UTF-8';\n" .
					    "           add_header 'Content-Length' 0;\n" .
					    "           return 204;\n" .
					    "   }\n" .
									"}\n";

		}

		if ($key !~ /ms-init/)
		{
	  open(FPLOC,">$outputdir/location/$key-" . $val . ".conf");
	  print FPLOC $loc;
	  close(FPLOC);
		}

 if (exists $mapping{$key})
	{
		$list = $mapping{$key};
	 push(@$list,$val);
		$mapping{$key} = $list;
	}
	else
	{
		$list = [];
		push(@$list,$val);
		$mapping{$key} = $list;
	}
}
}

foreach $key (keys %mapping)
{
 $webxml = dclone($data);

 delete($webxml->{'servlet'});
 delete($webxml->{'servlet-mapping'});
	delete($webxml->{'xmlns'});
	delete($webxml->{'xmlns:web'});
	delete($webxml->{'xmlns:xsi'});
	delete($webxml->{'xsi:schemaLocation'});

 $list = $mapping{$key};
	@l = @$list;
	@servletlist = ();
	@servletmapping = ();

	foreach $servlet (@l)
	{
		$s = $data->{'servlet'};
		$len = scalar @$s;
		for ($i=0;$i<$len;$i++)
		{
			$master = $data->{'servlet'}[$i];
			if ($master->{'servlet-name'} eq $servlet)
			{
					push(@servletlist,$master);
			}
		}

		$s = $data->{'servlet-mapping'};
		$len = scalar @$s;
		for ($i=0;$i<$len;$i++)
		{
			$master = $data->{'servlet-mapping'}[$i];
			if ($master->{'servlet-name'} eq $servlet)
			{
				push(@servletmapping,$master);
			}
		}
	}

	$webxml->{'servlet'} = \@servletlist;
	$webxml->{'servlet-mapping'} = \@servletmapping;
	$outxml = $xml->XMLout($webxml, NoAttr => 1);
	$outxml =~ s/\<opt\>//g;
	$outxml =~ s/\<\/opt\>/\<\/web-app\>/g;

	$pwd = cwd();

	my @folders = split /\//, $outputdir . "/$key";
	$folders[0] = '/';
	map { mkdir $_; chdir $_; } @folders;
	chdir $pwd;

	open(FP,">$outputdir/$key/web.xml");
	print FP '<?xml version="1.0" encoding="UTF-8"?>' . "\n";
	print FP '<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:web="http://java.sun.com/xml/ns/javaee" xmlns="http://java.sun.com/xml/ns/javaee" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd" id="WebApp_ID" version="2.5">' . "\n";
	print FP $outxml;
	close(FP);

	open(FP,"<$tgt/deployhub-webadmin.war.tgt");
	@lines = <FP>;
	close(FP);

	open(FP,">$tgt/$key.war.tgt");
	foreach $line (@lines)
	{
		$line =~ s/ \</\</g;
		$line =~ s/deployhub-webadmin/$key/g;
		$line =~ s/dmadminweb\/WebContent\/WEB-INF\/web.xml/dmadminweb\/microservice\/$key\/web.xml/g;
		print FP "$line";
	}
	close(FP);

}
# print output
#print Dumper($data);
#print Dumper(%mapping);
