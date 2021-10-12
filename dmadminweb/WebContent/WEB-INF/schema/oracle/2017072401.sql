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

insert into dm.dm_providerdef(id,name,kind,kindstr) values(22,'hipchat',9,'notify');
insert into dm.dm_providerdef(id,name,kind,kindstr) values(23,'slack',9,'notify');

insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'webhook','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'logfile','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'HTML','N','N');

insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'webhook','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'channel','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'logfile','N','N');
