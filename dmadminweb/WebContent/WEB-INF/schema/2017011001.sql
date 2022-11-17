insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle) values ('My Pipeline',(select max(id)+1 from dm.dm_domain),1,'N',1,'Y');
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Development',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',0);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Integration',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',1);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Testing',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',2);
insert into dm.dm_domain (name,id,domainid,status,ownerid,lifecycle,position) values ('Production',(select max(id)+1 from dm.dm_domain),(select id from dm.dm_domain where name = 'My Pipeline'),'N',1,'Y',3);


