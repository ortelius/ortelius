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

alter table dm.dm_action 
  add interpreter character varying(2048);
  
delete from dm.dm_actionarg where actionid = 1);
delete from dm.dm_action where id = 1);

INSERT INTO dm.dm_action VALUES (1, 'WebsphereDeploy', '${DMHOME}\scripts\deployws.py', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, 1430328839, 1, 1430330271, NULL, 'N', NULL, 3, NULL, '${PATH_TO_WSADMIN}\wsadmin.${EXT} -lang jython -user ${WSUSERNAME} -password ${WSPASSWORD} -f ');
INSERT INTO dm.dm_actionarg VALUES (1, 'AppName', 'entry', 1, 'Y', '-appName', 'N', 1, 'S', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'EarFile', 'entry', 2, 'Y', '-earfile', 'N', 2, 'S', '');
INSERT INTO dm.dm_actionarg VALUES (1, 'ContextRoot', 'entry', 3, 'Y', '-contextroot', 'N', 4, 'S', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'ServerName', 'entry', 4, 'Y', '-server', 'N', 3, 'S', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'NodeName', 'entry', 5, 'Y', '-node', 'N', 5, 'S', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'CellName', 'entry', 6, 'Y', '-cell', 'N', 6, 'S', '');
INSERT INTO dm.dm_actionarg VALUES (1, 'ProcessEmbeddedConfig', 'checkbox', 7, 'N', '-processEmbeddedConfig', 'N', 16, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'CreateMbeansForResource', 'checkbox', 8, 'N', '-createMBeansForResources', 'N', 13, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'DeployWs', 'checkbox', 9, 'N', '-deployws', 'N', 15, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'DeployEjb', 'checkbox', 10, 'N', '-deployejb', 'N', 12, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'ValidateInstall', 'entry', 11, 'N', '-validateinstall', 'N', 7, 'S', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'ReloadEnabled', 'checkbox', 12, 'N', '-reloadEnabled', 'N', 14, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'MapToVirtualHost', 'checkbox', 13, 'N', '-Map2VH', 'N', 9, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'DistributeApp', 'checkbox', 14, 'N', '-distributeApp', 'N', 10, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'PrecompileJsps', 'checkbox', 15, 'N', '-precompileJSPs', 'N', 8, 'B', '');
INSERT INTO dm.dm_actionarg VALUES (1, 'UseMetadataFromBinary', 'checkbox', 16, 'N', '-useMetaDataFromBinary', 'N', 11, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'AllowDispatchRemoteInclude', 'checkbox', 17, 'N', '-allowDispatchRemoteInclude', 'N', 17, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'UseAutoLink', 'checkbox', 18, 'N', '-useAutoLink', 'N', 19, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'AllowServiceRemoteInclude', 'checkbox', 19, 'N', '-allowServiceRemoteInclude', 'N', 18, 'B', NULL);
INSERT INTO dm.dm_actionarg VALUES (1, 'Tasks', 'entry', 20, 'N', '-tasks', 'N', 20, 'S', NULL);