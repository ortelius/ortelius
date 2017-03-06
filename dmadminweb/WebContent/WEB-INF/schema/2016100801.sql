

ALTER TABLE dm.dm_buildhistory add commit character varying(256);

CREATE TABLE dm_buildfiles
(
  buildjobid integer,
  buildnumber integer,
  filename character varying(256),
  CONSTRAINT "buildfiles-fk" FOREIGN KEY (buildjobid)
      REFERENCES dm_buildjob (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);

INSERT INTO dm_privileges VALUES (2, 24);