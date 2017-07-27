delete from dm.dm_action where id=20 and name='soap';
insert into dm.dm_action(id,name,summary,domainid,function,resultisexpr,pluginid,status,kind,categoryid)
values(20,'soap','makes SOAP based calls to remote servers',1,'Y','Y',6,'N',5,1);
