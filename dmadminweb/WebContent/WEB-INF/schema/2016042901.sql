alter table dm.dm_component add deploysequentially char(1);
update dm.dm_fragments set summary='Deploys the component on the stack to the server on the stack' where name='Transfer';
delete from dm.dm_fragmentattrs where typeid=(select id from dm.dm_fragments where name='Transfer');