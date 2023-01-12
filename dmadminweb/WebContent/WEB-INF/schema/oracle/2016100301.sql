create table dm.dm_buildengine(
id integer,
name varchar2(2048),
domainid integer,
ownerid integer,
ogrpid integer,
defid integer,
summary varchar2(256),
credid integer,
creatorid integer,
created integer,
modifierid integer,
modified integer,
status char(1) NOT NULL,
CONSTRAINT "buildengine-pk" PRIMARY KEY (id),
CONSTRAINT "buildengine-credentials-fk" FOREIGN KEY (credid)
	REFERENCES dm_credentials (id) MATCH SIMPLE
	ON UPDATE NO ACTION ON DELETE NO ACTION,
CONSTRAINT "buildengine-ogrpid-fk" FOREIGN KEY (ogrpid)
	REFERENCES dm_usergroup (id) MATCH SIMPLE
	ON UPDATE NO ACTION ON DELETE NO ACTION,
CONSTRAINT "buildengine-ownerid-fk" FOREIGN KEY (ownerid)
	REFERENCES dm_user (id) MATCH SIMPLE
	ON UPDATE NO ACTION ON DELETE NO ACTION
);


CREATE TABLE dm_buildengineaccess
(
  builderid integer NOT NULL,
  usrgrpid integer NOT NULL,
  writeaccess char(1),
  readaccess char(1),
  viewaccess char(1),
  updateaccess char(1),
  CONSTRAINT "buildengineaccess-buildengine-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "buildengineaccess-usergroup-fk" FOREIGN KEY (usrgrpid)
      REFERENCES dm_usergroup (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE dm_buildengineprops
(
  builderid integer NOT NULL,
  name varchar2(256) NOT NULL,
  value varchar2(2048),
  encrypted char(1),
  overridable char(1),
  appendable char(1),
  CONSTRAINT "buildengineprops-pk" PRIMARY KEY (builderid, name),
  CONSTRAINT "buildengineprops-notify-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE dm_buildjob
(
  id integer,
  name varchar2(128),
  summary varchar2(256),
  builderid integer,
  creatorid integer,
  modifierid integer,
  created integer,
  modified integer,
  status char(1),
  projectname varchar2(256),
  CONSTRAINT "buildjob-pk" PRIMARY KEY (id),
  CONSTRAINT "buildjob-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

CREATE TABLE dm_buildhistory
(
  buildjobid integer NOT NULL,
  buildnumber integer NOT NULL,
  compid integer NOT NULL,
  userid integer NOT NULL,
  timestamp integer,
  success char(1),
  CONSTRAINT "buildhistory-fk" FOREIGN KEY (buildjobid)
      REFERENCES dm_buildjob (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "buildhistory-comp-fk" FOREIGN KEY(compid)
      REFERENCES dm_component (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

ALTER TABLE dm.dm_component ADD buildjobid integer;
ALTER TABLE dm.dm_component ADD lastbuildnumber integer;
ALTER TABLE dm.dm_compsonserv ADD buildnumber integer;
ALTER TABLE dm.dm_historycomment ADD subid integer;
ALTER TABLE dm.dm_deploymentxfer ADD buildnumber integer;
INSERT INTO dm.dm_providerdef(id,name,kind,kindstr) VALUES(21,'jenkins',24,'buildengine');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(21,'Server URL','Y','N');
