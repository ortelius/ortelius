
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

override_values=""
if [ "$chartvalues" != "" ]; then
  override_values="-f $chartvalues"
fi

echo $helmlogin $helmrepo $helmrepourl | xargs $helmexe repo add  2>&1 1>/dev/null
$helmexe repo update 2>&1 1>/dev/null
$helmexe fetch $chartorg/$chartname --version $chartversion 2>&1 1>/dev/null
digest=`sha256sum $chartname*.tgz | cut -f1 -d " "`
echo "{"
echo "\"chartdigest\": \"$digest\","
echo "\"images\": ["
tar -xf $chartname*.tgz 
IFS=$'\n'

let cnt=0
for line in $(cat $helmtemplate | dos2unix | grep "image:" | cut -f 2- -d ":" | tr -d '"' | tr -d " " | sort -u)
do
 if [ "$cnt" -gt "0" ]; then
  echo ","
 fi

 let cnt=$cnt+1

 slashes=$(echo $line | awk -F"/" '{print NF-1}')
 colons=$(echo $line | awk -F":" '{print NF-1}')

 if [ "$colons" == "0" ]; then
  line="$line:latest"
 fi

 if [ "$slashes" -ge 2 ]; then
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
 
 apiuser=$helmrepouser
 apipass=$helmrepopass

 if [ "$reg" == "docker.io" ]; then
   apidomain="registry-1.docker.io"
   if [ "$dockeruser" != "" ]; then
    apiuser=$dockeruser
   fi
   if [ "$dockerpass" != "" ]; then
    apipass=$dockerpass
   fi
 else
   apidomain=$reg
 fi

 authdomsvc=`curl --head "https://${apidomain}/v2/" 2>&1 | grep realm | cut -f2- -d "=" | tr "," "?" | tr -d '"' | tr -d "\r"`
 authscope="repository:${org}/${img}:pull"

 TOKEN=$(echo -u ${apiuser}:${apipass} | xargs curl -s -X GET "${authdomsvc}&scope=${authscope}&offline_token=1&client_id=shell" | jq -r '.token')
 digest=$(curl --head -s -H "Authorization: Bearer ${TOKEN}" -H "Accept: application/vnd.docker.distribution.manifest.v2+json" https://${apidomain}/v2/${org}/${img}/manifests/$tag | grep "Docker-Content-Digest:" | cut -f 2 -d " " | tr -d '"' | tr -d "\r")
 
 cat <<- EOF
        {
         "registry": "$reg",
         "organization": "$org",
         "imagename": "$img",
         "imagetag": "$tag",
         "imagedigest": "$digest"
        }
EOF
done 
echo "]"
echo "}"
rm $chartname*.tgz



