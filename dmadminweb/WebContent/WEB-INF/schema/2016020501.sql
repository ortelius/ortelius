alter table dm.dm_action add usetty character(1);
update dm.dm_action set usetty='N';
