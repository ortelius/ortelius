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

alter table dm.dm_tasktypes add component character(1);
alter table dm.dm_tasktypes add compversion character(1);
insert into dm.dm_fragments(id,name,summary,categoryid,exitpoints) values(95,'Run Task','Execute a Specific DeployHub Task',1,1);
insert into dm.dm_fragmenttext(fragmentid,data,type) values(95,'runtask(task:"@taskname@");',0);
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,tablename,inherit,atorder,required) values(951,95,'dropdown','TaskName','dm_task','Y',1,'Y');
insert into dm.dm_fragments(id,name,summary,categoryid,exitpoints) values(96,'Wait for Task','"Wait for a Specific DeployHub Task to be Run"',1,1);
insert into dm.dm_fragmenttext(fragmentid,data,type) values(96,'waitfor(task: "@taskname@");',0);
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,tablename,inherit,atorder,required) values(961,96,'dropdown','TaskName','dm_task','Y',1,'Y');
insert into dm.dm_fragment_categories(id,categoryid) values(95,1);
insert into dm.dm_fragment_categories(id,categoryid) values(96,1);
update dm.dm_tasktypes set component='N';
update dm.dm_tasktypes set compversion='N';
update dm.dm_tasktypes set name='RunAction',component='Y',compversion='Y' where id=5;
