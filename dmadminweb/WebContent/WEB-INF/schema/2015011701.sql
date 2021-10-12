-- Copyright (c) 2021 Linux Foundation
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

update dm.dm_actionarg set inpos = 1 where name = 'ApplicationName' and actionid = 1;
update dm.dm_actionarg set inpos = 2 where name = 'earfile' and actionid = 1;
update dm.dm_actionarg set inpos = 3 where name = 'AllowDispatchingIncludesToRemoteResources' and actionid = 1;
update dm.dm_actionarg set inpos = 4 where name = 'AllowEJBReferenceTargetsToResolveAutomatically' and actionid = 1;
update dm.dm_actionarg set inpos = 5 where name = 'AllowServicingIncludesFromRemoteResources' and actionid = 1;
update dm.dm_actionarg set inpos = 6 where name = 'CreateMBeansForResources' and actionid = 1;
update dm.dm_actionarg set inpos = 7 where name = 'DeployEnterpriseBeans' and actionid = 1;
update dm.dm_actionarg set inpos = 8 where name = 'DeployWebService' and actionid = 1;
update dm.dm_actionarg set inpos = 9 where name = 'DistributeApplication' and actionid = 1;
update dm.dm_actionarg set inpos = 10 where name = 'EnableClassReloading' and actionid = 1;
update dm.dm_actionarg set inpos = 11 where name = 'PrecompileJSPs' and actionid = 1;
update dm.dm_actionarg set inpos = 12 where name = 'ProcessEmbeddedConfiguration' and actionid = 1;
update dm.dm_actionarg set inpos = 13 where name = 'UseBinaryConfiguration' and actionid = 1;
update dm.dm_actionarg set inpos = 14 where name = 'ValidateInstall' and actionid = 1;

update dm.dm_fragmenttext set data = 'using component "@component@" { deploy; }' where fragmentid = 10;
