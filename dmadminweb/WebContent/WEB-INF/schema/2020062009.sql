update dm.dm_repository set domainid = 1, name = 'File System' where name = 'demorep';
update dm.dm_repositoryprops set overridable = 'Y' where repositoryid in (select id from dm.dm_repository b where b.name = 'File System' and b.domainid = 1);
