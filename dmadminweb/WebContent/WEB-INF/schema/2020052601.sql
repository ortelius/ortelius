alter table dm.dm_helm add IF NOT EXISTS compid integer;
alter table dm.dm_helm add IF NOT EXISTS envid integer;
alter table dm.dm_helm add IF NOT EXISTS appparentid integer;
alter table dm.dm_helm add IF NOT EXISTS chartname character varying(80);
alter table dm.dm_helm add IF NOT EXISTS chartversion character varying(80);
alter table dm.dm_helm add IF NOT EXISTS chartrepo character varying(256);
alter table dm.dm_helm add IF NOT EXISTS chartdigest character varying(256);
