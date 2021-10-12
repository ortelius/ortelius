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

update dm.dm_applicationvars set name = trim(name), value = trim(value);
update dm.dm_componentvars set name = trim(name), value = trim(value);
update dm.dm_datasourcevars set name = trim(name), value = trim(value);
update dm.dm_environmentvars set name = trim(name), value = trim(value);
update dm.dm_notifyvars set name = trim(name), value = trim(value);
update dm.dm_repositoryvars set name = trim(name), value = trim(value);
update dm.dm_servervars set name = trim(name), value = trim(value);