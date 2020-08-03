update dm.dm_propertydef c set name = 'Organization' where (c.defid, c.name) in (select id, b.name from dm.dm_providerdef a, dm.dm_propertydef b where a.name = 'github' and
a.id = b.defid and b.name = 'Repository');
update dm.dm_propertydef c set name = 'Repository' where (c.defid, c.name) in (select id, b.name from dm.dm_providerdef a, dm.dm_propertydef b where a.name = 'github' and
a.id = b.defid and b.name = 'Product');
update dm.dm_datasourceprops c set name = 'Organization' where (c.datasourceid, c.name) in (select b.id, c.name from dm.dm_providerdef a, dm.dm_datasource b, dm.dm_datasourceprops c where a.name = 'github' and a.id = b.defid
and b.id = c.datasourceid and c.name = 'Repository');
update dm.dm_datasourceprops c set name = 'Repository' where (c.datasourceid, c.name) in (select b.id, c.name from dm.dm_providerdef a, dm.dm_datasource b, dm.dm_datasourceprops c where a.name = 'github' and a.id = b.defid
and b.id = c.datasourceid and c.name = 'Product');