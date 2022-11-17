alter table dm.dm_componentdeps add IF NOT EXISTS purl character varying(1024);
alter table dm.dm_componentdeps add IF NOT EXISTS pkgtype character varying(80);
