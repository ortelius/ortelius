delete from dm.dm_propertydef where defid=14 and name='pattern';
update dm.dm_propertydef set name='pattern' where name='filename' and defid=14;
delete from dm.dm_compitemprops where compitemid in (
select	a.compitemid
from	dm.dm_compitemprops	a,
		dm.dm_componentitem	b,
		dm.dm_repository	c
where	c.defid=14
and		b.repositoryid = c.id
and		a.compitemid = b.id
and		a.name='filename');