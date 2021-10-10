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

UPDATE
  dm.dm_compsonserv c
SET
  deploymentid=a.deploymentid
FROM dm.dm_deployment a, dm.dm_application b, dm.dm_component d, dm.dm_environment e 
WHERE a.appid = b.id and c.modified = a.started and c.compid = d.id and e.id = a.envid and d.status = 'N' and a.deploymentid >= 0;