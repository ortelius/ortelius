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

update dm.dm_type set name = 'Kubernetes' where name = 'Container for K8S';
update dm.dm_type set name = 'Kubernetes' where name = 'Container';
update dm.dm_type set name = 'Application Server' where name = 'Application File';
update dm.dm_type set name = 'Websphere' where name = 'Websphere Ear';
update dm.dm_type set name = 'Oracle' where name = 'Oracle DDL & SQL';
update dm.dm_type set name = 'Postgres' where name = 'Postgres DDL & SQL';
update dm.dm_type set name = 'SQL Server' where name = 'SQLServer DDL & SQL';
update dm.dm_type set name = 'DB2' where name = 'DB2 DDL & SQL';