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

delete from dm.dm_action where id=20 and name='soap';
insert into dm.dm_action(id,name,summary,domainid,function,resultisexpr,pluginid,status,kind,categoryid)
values(20,'soap','makes SOAP based calls to remote servers',1,'Y','Y',6,'N',5,1);
