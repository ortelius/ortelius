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

create or replace function dm.dm_insert_repo_recursive() returns void as $$ begin IF NOT EXISTS (SELECT 1 from dm.dm_repositoryprops a, dm.dm_repository b where a.repositoryid = b.id and a.name = 'recursive' and b.name = 'File System'  and b.domainid = 1) THEN insert into dm.dm_repositoryprops (repositoryid, name, value, encrypted, overridable, appendable) (select id,'recursive' as name, '', 'N', 'Y', 'N' from dm.dm_repository where name = 'File System'  and domainid = 1);END IF;end;$$ LANGUAGE plpgsql;
