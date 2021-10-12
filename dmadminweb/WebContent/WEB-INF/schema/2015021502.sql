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


ALTER TABLE dm.dm_component ADD comptypeid integer;

insert into dm_type values (1,'Oracle DDL & SQL','Y','N');
insert into dm_type values (2,'Postgres DDL & SQL','Y','N');
insert into dm_type values (3,'Websphere War','N','Y');
insert into dm_type values (4,'Websphere Ear','N','Y');
insert into dm_type values (5,'Websphere Jar','N','N');
insert into dm_type values (6,'Application File','N','N');
insert into dm_type values (7,'DB2 DDL & SQL','Y','N');
insert into dm_type values (8,'SQLServer DDL & SQL','Y','N');

update dm.dm_component set comptypeid = 6 where comptypeid is null;

