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

CREATE TABLE dm.dm_discovery
(
  id integer NOT NULL,
  serverid integer NOT NULL,
  servername character varying(64) NOT NULL,
  targetfilename character varying(2048),
  md5 character varying(256),
  discovery_time integer
)  
WITH (
  OIDS=FALSE
);