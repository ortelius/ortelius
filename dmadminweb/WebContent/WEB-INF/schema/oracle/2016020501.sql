alter table dm.dm_action add usetty char(1);
update dm.dm_action set usetty='N';
