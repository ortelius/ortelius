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

-- add the new serverstatus table and add uname to server table
-- 
create table dm_serverstatus(
serverid	integer not null,
nameresolution	char(1),
ping		char(1),
connection	char(1),
basedir		char(1),
ipaddr		char(16),
pingtime	integer,
lasterror	varchar2(1024),
lasttime	integer,
autoping	char(1),
automd5		char(1),
starttime	integer,
endtime		integer,
checkinterval	integer
);

alter table dm_server add uname varchar2(256);

