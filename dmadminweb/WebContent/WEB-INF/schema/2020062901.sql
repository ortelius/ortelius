update dm.dm_component b set comptypeid = (select id from dm.dm_type where name = 'Kubernetes') where b.id in (select id from dm.dm_component a where a.name like '%service;master%');
update dm.dm_component b set comptypeid = (select id from dm.dm_type where name = 'Kubernetes') where b.id in (select id from dm.dm_component a where a.name like 'loadgenerator;master%');
update dm.dm_component b set comptypeid = (select id from dm.dm_type where name = 'Kubernetes') where b.id in (select id from dm.dm_component a where a.name like 'frontend;master%');
