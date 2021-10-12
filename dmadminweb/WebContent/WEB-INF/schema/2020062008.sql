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

delete from dm.dm_type f where f.name in ('Websphere War', 'Websphere Jar') and not f.id in (select b.id from dm.dm_component a, dm.dm_type b where a.comptypeid = b.id union select e.id from dm.dm_server c, dm.dm_type e where c.typeid = e.id );