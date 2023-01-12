
ALTER TABLE dm.dm_component ADD comptypeid integer;

insert into dm_type values (1,'Oracle DDL & SQL','Y','N');
insert into dm_type values (2,'Postgres DDL & SQL','Y','N');
insert into dm_type values (3,'Websphere War','N','Y');
insert into dm_type values (4,'Websphere Ear','N','Y');
insert into dm_type values (5,'Websphere Jar','N','N');
insert into dm_type values (6,'Application File','N','N');
insert into dm_type values (7,'DB2 DDL & SQL','Y','N');
insert into dm_type values (8,'SQLServer DDL & SQL','Y','N');

update dm.dm_component set comptypeid = 6 where comptypeid is null;
