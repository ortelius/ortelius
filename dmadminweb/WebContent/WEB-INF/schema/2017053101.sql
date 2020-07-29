drop table dm.dm_queue;
create table dm.dm_queue(
id           integer,
clientid     character varying(32),
credentials  character varying(512),
stdin        character varying(2048),
command      character varying(1024),
stdout       character varying(2048)
);
