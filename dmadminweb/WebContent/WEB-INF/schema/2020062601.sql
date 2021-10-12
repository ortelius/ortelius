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

update dm.dm_propertydef c set name = 'Organization' where (c.defid, c.name) in (select id, b.name from dm.dm_providerdef a, dm.dm_propertydef b where a.name = 'github' and
a.id = b.defid and b.name = 'Repository');
update dm.dm_propertydef c set name = 'Repository' where (c.defid, c.name) in (select id, b.name from dm.dm_providerdef a, dm.dm_propertydef b where a.name = 'github' and
a.id = b.defid and b.name = 'Product');
update dm.dm_datasourceprops c set name = 'Organization' where (c.datasourceid, c.name) in (select b.id, c.name from dm.dm_providerdef a, dm.dm_datasource b, dm.dm_datasourceprops c where a.name = 'github' and a.id = b.defid
and b.id = c.datasourceid and c.name = 'Repository');
update dm.dm_datasourceprops c set name = 'Repository' where (c.datasourceid, c.name) in (select b.id, c.name from dm.dm_providerdef a, dm.dm_datasource b, dm.dm_datasourceprops c where a.name = 'github' and a.id = b.defid
and b.id = c.datasourceid and c.name = 'Product');