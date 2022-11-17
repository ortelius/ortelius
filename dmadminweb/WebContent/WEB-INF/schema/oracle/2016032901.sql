create table dm.dm_taskparams(
taskid integer,
pos integer,
label varchar2(40),
variable varchar2(40),
type char(10),
arrname varchar2(40),
);

ALTER TABLE dm.dm_taskparams
    ADD CONSTRAINT "taskparams-taskid-fk" FOREIGN KEY (taskid) REFERENCES dm_task(id);
