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

