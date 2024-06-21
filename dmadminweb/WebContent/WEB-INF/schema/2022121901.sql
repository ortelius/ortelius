update dm.dm_componentvars a set name = 'JobTriggeredBy' where name = 'JobTriggerdBy' and not exists (select 1 from dm.dm_componentvars where compid = a.compid and name = 'JobTriggeredBy');
