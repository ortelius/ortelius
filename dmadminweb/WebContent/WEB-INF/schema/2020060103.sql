insert into dm.dm_appsallowedinenv (envid, appid)
(select distinct a.envid, a.appid from dm.dm_deployment a, dm.dm_application b where a.appid = b.id and b.parentid is null and not exists (SELECT c.envid, c.appid FROM dm.dm_appsallowedinenv c where a.appid = c.appid and a.envid = c.envid));
