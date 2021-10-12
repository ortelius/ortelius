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

alter table dm.dm_componentitem add IF NOT EXISTS serviceowner character varying(80);
alter table dm.dm_componentitem add IF NOT EXISTS serviceowneremail character varying(80);
alter table dm.dm_componentitem add IF NOT EXISTS serviceownerphone character varying(20);
alter table dm.dm_componentitem add IF NOT EXISTS slackchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS discordchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS hipchatchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS pagerdutyurl character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS pagerdutybusinessurl character varying(1024);
