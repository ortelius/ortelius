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

insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle) values ('My Pipeline',(select max(id)+1 from dm.dm_domain),1,'N',1,'Y');
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Development',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',0);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Integration',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',1);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Testing',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',2);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Production',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',3);


