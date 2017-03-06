update dm.dm_component a set domainid = 1 where a.name like 'ansible_%';
update dm.dm_action a set domainid = 1 where a.name like 'ansible_%';
select dm.dm_insert_infra();
update dm.dm_component a set domainid = (select b.id from dm.dm_domain b where b.name = 'Infrastructure') where a.name like 'ansible_%';
update dm.dm_action a set domainid = (select b.id from dm.dm_domain b where b.name = 'Infrastructure') where a.name like 'ansible_%';