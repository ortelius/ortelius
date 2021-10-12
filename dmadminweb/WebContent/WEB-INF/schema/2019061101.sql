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

alter table dm.dm_componentitem add kind character varying(10);
alter table dm.dm_componentitem add buildid character varying(1024);
alter table dm.dm_componentitem add buildurl character varying(1024);
alter table dm.dm_componentitem add chart character varying(100);
alter table dm.dm_componentitem add operator character varying(100);
alter table dm.dm_componentitem add builddate character varying(100);
alter table dm.dm_componentitem add dockersha character varying(1024);
alter table dm.dm_componentitem add gitcommit character varying(1024);
alter table dm.dm_componentitem add gitrepo character varying(1024);
alter table dm.dm_componentitem add gittag character varying(100);
alter table dm.dm_componentitem add giturl character varying(1024);
update dm.dm_componentitem set kind = 'database' where rollup = 1 or rollback = 1;