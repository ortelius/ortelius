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

update dm.dm_action set copy = 'Y' where id = 1;
delete from dm.dm_fragmentattrs where typeid = 120;

INSERT INTO dm.dm_fragmentattrs VALUES (12001, 120, 'Entry', 'AppName',  NULL, 'N', 1, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12002, 120, 'Entry', 'EarFile', NULL, 'N', 2, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12003, 120, 'Entry', 'ContextRoot',  NULL, 'N', 3, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12004, 120, 'Entry', 'ServerName',   NULL, 'N', 4, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12005, 120, 'Entry', 'NodeName',  NULL, 'N', 5, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12006, 120, 'Entry', 'CellName',   NULL, 'N', 6, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12007, 120, 'Checkbox', 'ProcessEmbeddedConfig',   NULL, 'N', 7, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12008, 120, 'Checkbox',  'CreateMbeansForResource',   NULL, 'N', 8, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12009, 120, 'Checkbox',  'DeployWs',   NULL, 'N', 9, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12010, 120, 'Checkbox',  'DeployEjb',  NULL, 'N', 10, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12011, 120, 'Checkbox',  'ValidateInstall',   NULL, 'N', 11, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12012, 120, 'Checkbox',  'ReloadEnabled',   NULL, 'N', 12, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12013, 120, 'Checkbox',  'MapToVirtualHost',   NULL, 'N', 13, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12014, 120, 'Checkbox',  'DistributeApp',   NULL, 'N', 14, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12015, 120, 'Checkbox',  'PrecompileJsps',   NULL, 'N', 15, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12016, 120, 'Checkbox',  'UseMetadataFromBinary',  NULL, 'N', 16, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12017, 120, 'Checkbox',  'AllowDispatchRemoteInclude',   NULL, 'N', 17, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12018, 120, 'Checkbox',  'UseAutoLink',   NULL, 'N', 18, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12019, 120, 'Checkbox',  'AllowServiceRemoteInclude',  NULL, 'N', 19, 'Y') ;
INSERT INTO dm.dm_fragmentattrs VALUES (12020, 120, 'Entry',  'Tasks',   NULL, 'N', 20, 'Y') ;
