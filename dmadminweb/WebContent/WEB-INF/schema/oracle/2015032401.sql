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

 
ALTER TABLE dm.dm_tableinfo ADD status varchar2(256);

update dm.dm_tableinfo set status = 'ZYPT-EYDH-Z3UA-11AE';

INSERT INTO dm_action VALUES (105, 'Run_SQL_Script_Postgres', NULL, 'Run SQL Script for Postgres', 1, NULL, NULL, 'N', NULL, NULL, NULL, 'Y', 1, 1, 1427405404, 1, 1427405662, NULL, 'N', NULL, 6, NULL) ;
INSERT INTO dm_actionflows VALUES (105, 1, 0, 1, 1) ;
INSERT INTO dm_actionfragattrs VALUES (105, 1, 2951, '${dep.files}') ;
INSERT INTO dm_actionfrags VALUES (105, 1, 460, 160, 295, 'RunSQLScript Postgres', '', NULL) ;

update dm.dm_application set preactionid  = 105 where preactionid = 11; 
update dm.dm_application set postactionid = 105 where postactionid = 11; 
update dm.dm_component set preactionid  = 105 where preactionid = 11; 
update dm.dm_component set postactionid = 105 where postactionid = 11; 

delete from dm.dm_action where id = 11;

update dm.dm_keys set id = 105 where object = 'Action';
update dm.dm_keys set id = 296 where object = 'fragments';






