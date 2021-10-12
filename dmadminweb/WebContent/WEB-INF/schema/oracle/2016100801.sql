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

insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Project Key','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Filter','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(18,'Poll Interval','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(19,'Poll Interval','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Poll Interval','N','N');

CREATE TABLE dm.dm_bugtrackingstates(
  defid integer,
  status varchar2(128),
  colour varchar2(40),
  isclosed char(1));

INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'verified','green','Y');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'resolved','blue','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'in_progress','yellow','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'confirmed','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(19,'open','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(19,'closed','green','Y');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'to do','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'in progress','yellow','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'done','green','Y');

ALTER TABLE dm.dm_defects add status varchar2(40);
ALTER TABLE dm.dm_defects add datasourceid integer;
ALTER TABLE dm.dm_buildhistory add commit varchar2(256);

CREATE TABLE dm.dm_defecthistory(
datasourceid	integer NOT NULL,
timestamp		integer NOT NULL, 
appid			integer,
compid			integer,
open			integer NOT NULL,
closed			integer NOT NULL,
CONSTRAINT "defecthistory-app-fk"  FOREIGN KEY (appid)  REFERENCES dm_application (id),
CONSTRAINT "defecthistory-comp-fk" FOREIGN KEY (compid) REFERENCES dm_component (id)
);

CREATE TABLE dm_builddefects
(
  buildjobid integer,
  buildnumber integer,
  bugid varchar2(128),
  htmlurl varchar2(1024),
  title varchar2(256),
  CONSTRAINT "builddefects-fk" FOREIGN KEY (buildjobid) REFERENCES dm_buildjob (id)
);

CREATE TABLE dm_buildfiles
(
  buildjobid integer,
  buildnumber integer,
  filename varchar2(256),
  CONSTRAINT "buildfiles-fk" FOREIGN KEY (buildjobid) REFERENCES dm_buildjob (id)
);

INSERT INTO dm_privileges VALUES (2, 24);
