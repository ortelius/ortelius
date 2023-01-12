UPDATE
  dm.dm_appsinenv c
SET
  deploymentid=a.deploymentid
FROM dm.dm_deployment a, dm.dm_application b, dm.dm_environment e
WHERE a.appid = b.id and c.modified = a.started and c.appid = a.appid and e.id = a.envid and b.status = 'N' and a.deploymentid >= 0;
