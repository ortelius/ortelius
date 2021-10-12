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

delete from dm.dm_action where id = 5;
delete from dm.dm_action where id = 6;
delete from dm.dm_action where id = 7;
delete from dm.dm_action where id = 8;

delete from dm.dm_actionarg where actionid = 5;
delete from dm.dm_actionarg where actionid = 6;
delete from dm.dm_actionarg where actionid = 7;
delete from dm.dm_actionarg where actionid = 8;

