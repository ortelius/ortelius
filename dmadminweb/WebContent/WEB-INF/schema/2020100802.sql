insert into dm.dm_deploymentcomps (deploymentid, compid) (select b.deploymentid, a.compid  from dm.dm_applicationcomponent a, dm.dm_deployment b where a.appid = b.appid and b.deploymentid > 0);
