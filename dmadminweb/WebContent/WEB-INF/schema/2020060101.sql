UPDATE
  dm.dm_compsonserv c
SET
  deploymentid=a.deploymentid
FROM dm.dm_deployment a, dm.dm_application b, dm.dm_component d, dm.dm_environment e 
WHERE a.appid = b.id and c.modified = a.started and c.compid = d.id and e.id = a.envid and d.status = 'N' and a.deploymentid >= 0;