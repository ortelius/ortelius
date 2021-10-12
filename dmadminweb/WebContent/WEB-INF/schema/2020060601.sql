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

alter table dm.dm_user add IF NOT EXISTS actionfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS procedurefilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS notifierfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS repositoryfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS datasourcefilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS credentialfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS domainfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS customfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS userfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS groupfilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS servercomptypefilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS templatefilter character varying(1024);
alter table dm.dm_user add IF NOT EXISTS buildjobfilter character varying(1024);