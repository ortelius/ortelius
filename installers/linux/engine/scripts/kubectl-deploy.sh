# Copyright (c) 2021 Linux Foundation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#!/bin/sh

. /etc/profile
export HOME=/home/omreleng

cd $1

for f in *-ms*.yaml
do
   cat $f
   /usr/local/google-cloud-sdk/bin/kubectl apply -f $f
done

if ls *.delete > /dev/null 2>&1; then 
 for f in *.delete
 do
   DEPLOYMENT=`echo $f | sed "s/\.delete//"`
   /usr/local/google-cloud-sdk/bin/kubectl delete deployment $DEPLOYMENT
 done
fi
