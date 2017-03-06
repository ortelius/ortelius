create table dm.dm_buildengine(
id integer,
name character varying(2048),
domainid integer,
ownerid integer,
ogrpid integer, 
defid integer,
summary character varying(256),
credid integer,
creatorid integer,
created integer,
modifierid integer,
modified integer,
status character(1) NOT NULL,
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
)
WITH (
  OIDS=FALSE
);


CREATE TABLE dm_buildengineaccess
(
  builderid integer NOT NULL,
  usrgrpid integer NOT NULL,
  writeaccess character(1),
  readaccess character(1),
  viewaccess character(1),
  updateaccess character(1),
  CONSTRAINT "buildengineaccess-buildengine-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "buildengineaccess-usergroup-fk" FOREIGN KEY (usrgrpid)
      REFERENCES dm_usergroup (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

CREATE TABLE dm_buildengineprops
(
  builderid integer NOT NULL, 
  name character varying(256) NOT NULL,
  value character varying(2048),
  encrypted character(1),
  overridable character(1),
  appendable character(1),
  CONSTRAINT "buildengineprops-pk" PRIMARY KEY (builderid, name),
  CONSTRAINT "buildengineprops-notify-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

CREATE TABLE dm_buildjob
(
  id integer,
  name character varying(128),
  summary character varying(256),
  builderid integer,
  creatorid integer,
  modifierid integer,
  created integer,
  modified integer,
  status character(1),
  projectname character varying(256),
  CONSTRAINT "buildjob-pk" PRIMARY KEY (id),
  CONSTRAINT "buildjob-fk" FOREIGN KEY (builderid)
      REFERENCES dm_buildengine (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

CREATE TABLE dm_buildhistory
(
  buildjobid integer NOT NULL,
  buildnumber integer NOT NULL,
  compid integer NOT NULL,
  userid integer NOT NULL,
  timestamp integer,
  success character(1),
  CONSTRAINT "buildhistory-fk" FOREIGN KEY (buildjobid)
      REFERENCES dm_buildjob (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "buildhistory-comp-fk" FOREIGN KEY(compid)
      REFERENCES dm_component (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

ALTER TABLE dm.dm_component ADD buildjobid integer;
ALTER TABLE dm.dm_component ADD lastbuildnumber integer;
ALTER TABLE dm.dm_compsonserv ADD buildnumber integer;
ALTER TABLE dm.dm_historycomment ADD subid integer;
ALTER TABLE dm.dm_deploymentxfer ADD buildnumber integer;
INSERT INTO dm.dm_providerdef(id,name,kind,kindstr) VALUES(21,'jenkins',24,'buildengine');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(21,'Server URL','Y','N');
