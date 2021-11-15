#!/bin/sh


# Variables passed in env
#   chart
#   component
#   environment

# Use $chart to derive chartname <org>/<chartname>
chartname=`echo $chart | cut -d '/' -f2`

# Use $component name to derive chartversion <component>;<variant>;<version>
# version = v<schematic>_g<git commit>
chartversion=`echo $component | cut -d";" -f 3 | sed "s/_g/\n/" | head -1`

# Use environment name to derive clean name
environment=`echo $environment | sed "s/[ |-]//g"`

cat $chart/Chart.yaml | grep -v -i "^version:" > work.yaml
echo "version: \"$chartversion\"" >> work.yaml
mv work.yaml $chart/Chart.yaml

tar -czvf $chartname-$chartversion.tgz $chart
echo $GH_TOKEN | gh auth login --with-token
gh release create v$chartversion $chartname-$chartversion-$environment.tgz --title "$chartname:$chartversion" --notes "bugfix release"
