update dm.dm_componentitem set rollup = 0 where rollup is null and rollback = 1;
update dm.dm_componentitem set rollback = 0 where rollup = 1 and rollback is null;
