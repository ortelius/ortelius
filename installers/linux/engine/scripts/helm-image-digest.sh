#!/bin/bash

helmrepouser=""
helmrepopass=""
helmrepo="bitnami"
helmrepourl="https://charts.bitnami.com/bitnami"

dockeruser="sbtaylor15"
dockerpass='XXXXXXX'

chartorg="bitnami"
chartname="wordpress"
chartversion="9.3.8"

helmexe="helm"

if [ "$dockeruser" == "" ]; then
  dockeruser=$helmrepouser
fi

if [ "$dockerpass" == "" ]; then
  dockerpass=$helmrepopass
fi

if [ "$helmrepouser" != "" ]; then
  helmlogin="--username $helmrepouser"
fi

if [ "$helmrepouser" != "" ]; then
  helmlogin="$helmlogin --password $helmrepopass"
fi

if [ "$chartorg" == "" ]; then
  chartorg=`echo $chartname | cut -d / -f 1`
  chartname=`echo $chartname | cut -d / -f 2`
fi

# Add Helm Repo
$helmexe repo add --force-update $helmlogin $helmrepo $helmrepourl 2>&1 1>/dev/null

# Get latest Helm Charts
$helmexe repo update 2>&1 1>/dev/null

# Fetch the Helm Chart as .tgz
$helmexe fetch $chartorg/$chartname --version $chartversion 2>&1 1>/dev/null

# Calc digest of the Helm Chart
digest=`sha256sum $chartname*.tgz | cut -f1 -d " "`

# Print out the starting JSON
echo "{"
echo "\"digest\": \"$digest\","
echo "\"images\": ["
tar -xf *.tgz
IFS=$'\n'

# Find the image: fields in the expanded Helm Chart and loop over each image line
let cnt=0
for line in $($helmexe template $chartname | grep "image:" | cut -f 2- -d ":" | tr -d '"' | tr -d " ")
do
 if [ "$cnt" -gt "0" ]; then
  echo ","
 fi

 let cnt=$cnt+1

 # find the number of slashes since registry name and org are not required for images in docker.io
 slashes=$(echo $line | awk -F"/" '{print NF-1}')

 if [ "$slashes" == "2" ]; then
   reg=$(echo $line | cut -d / -f 1)
   org=$(echo $line | cut -d / -f 2)
   img=$(echo $line | cut -d / -f 3- | cut -d ":" -f 1)
   tag=$(echo $line | cut -d / -f 3- | cut -d ":" -f 2)
 elif [ "$slashes" == "1" ]; then
   reg="docker.io"
   org=$(echo $line | cut -d / -f 1)
   img=$(echo $line | cut -d / -f 2- | cut -d ":" -f 1)
   tag=$(echo $line | cut -d / -f 2- | cut -d ":" -f 2)
 else
   reg="docker.io"
   org="library"
   img=$(echo $line | cut -d ":" -f 1)
   tag=$(echo $line | cut -d ":" -f 2)
 fi

 # docker.io v2 API endpoint lives in a different location than the registry url
 if [ "$reg" == "docker.io" ]; then
   apidomain="registry-1.docker.io"
 else
   apidomain=$reg
 fi

 # Find the docker authentication relm
 authdomsvc=`curl --head "https://${apidomain}/v2/" 2>&1 | grep realm | cut -f2- -d "=" | tr "," "?" | tr -d '"' | tr -d "\r"`

 # Set the scope to look for our image
 authscope="repository:${org}/${img}:pull"

 # Login to docker v2 API endpoint and save token
 TOKEN=$(curl -s -X GET -u ${dockeruser}:${dockerpass} "${authdomsvc}&scope=${authscope}&offline_token=1&client_id=shell" | jq -r '.token')

 # Grab the image digest from the Docker-Content-Digest: that is in the http header
 # Use "Accept: application/vnd.docker.distribution.manifest.v2+json" in order to get the version 2 schema
 digest=$(curl --head -s -H "Authorization: Bearer ${TOKEN}" -H "Accept: application/vnd.docker.distribution.manifest.v2+json" https://${apidomain}/v2/${org}/${img}/manifests/$tag | grep "Docker-Content-Digest:" | cut -f 2 -d " " | tr -d '"' | tr -d "\r")

 # Print out the details for each image
 cat <<- EOF
        {
         "registry": "$reg",
         "organization": "$org",
         "image_name": "$img",
         "image_tag": "$tag",
         "image_digest": "$digest"
        }
EOF
done

# Print closing JSON
echo "]"
echo "}"

# Cleanup
rm $chartname*.tgz
