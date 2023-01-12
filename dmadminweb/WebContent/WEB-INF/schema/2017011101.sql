insert into dm.dm_usergroup (name,id,domainid,status,ownerid) values ('Users',(select max(id)+1 from dm.dm_domain),1,'N',1);
