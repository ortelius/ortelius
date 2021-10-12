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
  status character varying(128),
  colour character varying(40),
  isclosed character(1));

INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'verified','green','Y');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'resolved','blue','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'in_progress','yellow','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(18,'confirmed','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(19,'open','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(19,'closed','green','Y');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'to do','red','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'in progress','yellow','N');
INSERT INTO dm.dm_bugtrackingstates(defid,status,colour,isclosed) VALUES(20,'done','green','Y');

ALTER TABLE dm.dm_defects add status character varying(40);
ALTER TABLE dm.dm_defects add datasourceid integer;

CREATE TABLE dm.dm_defecthistory(
timestamp		integer NOT NULL, 
appid			integer,
compid			integer,
open			integer NOT NULL,
closed			integer NOT NULL,
CONSTRAINT "defecthistory-app-fk" FOREIGN KEY (appid)
      REFERENCES dm_application (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
CONSTRAINT "defecthistory-comp-fk" FOREIGN KEY (compid)
      REFERENCES dm_component (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE dm_builddefects
(
  buildjobid integer,
  buildnumber integer,
  bugid character varying(128),
  htmlurl character varying(1024),
  title character varying(256),
  CONSTRAINT "builddefects-fk" FOREIGN KEY (buildjobid)
      REFERENCES dm_buildjob (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

