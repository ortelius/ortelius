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

delete from dm.dm_propertydef where defid=14 and name='pattern';
update dm.dm_propertydef set name='pattern' where name='filename' and defid=14;
delete from dm.dm_compitemprops where compitemid in (
select	a.compitemid
from	dm.dm_compitemprops	a,
		dm.dm_componentitem	b,
		dm.dm_repository	c
where	c.defid=14
and		b.repositoryid = c.id
and		a.compitemid = b.id
and		a.name='filename');