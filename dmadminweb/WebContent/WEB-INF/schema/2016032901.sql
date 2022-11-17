create table dm.dm_taskparams(
taskid integer,
pos integer,
label character varying(40),
variable character varying(40),
type character(10),
arrname character varying(40),
CONSTRAINT "taskparams-fk" FOREIGN KEY (taskid)
      REFERENCES dm_task (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
