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

id

#DOCKERFILE=`echo $1 | sed "s/Dockerfile.*/Dockerfile/"`
#mv $1 $DOCKERFILE
#cat $DOCKERFILE
echo ========== $1 ==============
cat $1
echo
echo ========== $1 ==============
docker build -f $1 -t $2:$3 $PWD | grep -v "Sending build context to Docker daemon"
