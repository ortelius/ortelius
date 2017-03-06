alter table dm.dm_credentials add ogrpid int;
update dm.dm_credentials set ownerid=creatorid;