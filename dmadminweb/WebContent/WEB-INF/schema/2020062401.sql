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

DROP TABLE dm.dm_helm;

CREATE TABLE dm.dm_helm
(
    appid integer NOT NULL,
    deployid integer,
    filename character varying(1024),
    lineno integer,
    line character varying(4096),
    compid integer,
    envid integer,
    helmrepo character varying(256),
    helmrepourl character varying(512),
    chartname character varying(80),
    chartversion character varying(80),
    chartdigest character varying(256),
    appparentid integer
);

CREATE TABLE dm_helmimages (
    appid integer NOT NULL,
    deployid integer,
    compid integer,
    envid integer,
    chartdigest character varying(512),
    registry character varying(256),
    organization character varying(256),
    imagename character varying(256),
    imagetag character varying(256),
    imagedigest character varying(512)
);