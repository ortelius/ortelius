insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Project Key','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Filter','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(18,'Poll Interval','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(19,'Poll Interval','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(20,'Poll Interval','N','N');

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


CREATE TABLE dm_buildfiles
(
  buildjobid integer,
  buildnumber integer,
  filename varchar2(256),
  CONSTRAINT "buildfiles-fk" FOREIGN KEY (buildjobid) REFERENCES dm_buildjob (id)
);

INSERT INTO dm_privileges VALUES (2, 24);
