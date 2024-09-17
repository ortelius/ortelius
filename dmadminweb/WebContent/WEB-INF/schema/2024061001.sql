CREATE or REPLACE VIEW dm_complist AS
SELECT DISTINCT
    d.id,
    d.name,
    d.created,
    e.name AS environment_name,
    a.deploymentid,
    a.finishts,
    a.exitcode,
    d.domainid,
    d.predecessorid,
    COALESCE(d.parentid, d.id) AS parentid,
    f.name AS predecessor_name,
    g.fullname as fullname
FROM
    dm.dm_deployment a
    JOIN dm.dm_application b ON a.appid = b.id
    JOIN dm.dm_deploymentcomps c ON a.deploymentid = c.deploymentid
    JOIN dm.dm_component d ON c.compid = d.id
    JOIN dm.dm_environment e ON e.id = a.envid
    JOIN dm.dm_component f ON d.predecessorid = f.id
    JOIN dm.dm_domain g ON d.domainid = g.id
WHERE
    d.status = 'N'
    AND (a.envid, c.compid, a.deploymentid) IN (
        SELECT
            x.envid,
            b.compid,
            MAX(x.deploymentid) AS deploymentid
        FROM
            dm.dm_deployment x
            JOIN dm.dm_deploymentcomps b ON x.deploymentid = b.deploymentid
        GROUP BY
            x.envid,
            b.compid
    )
UNION
SELECT DISTINCT
    d.id,
    d.name,
    d.created,
    '' AS environment_name,
    -99 AS deploymentid,
    NOW()::timestamp AS finishts,
    0 AS exitcode,
    d.domainid,
    d.predecessorid,
    COALESCE(d.parentid, d.id) AS parentid,
    f.name AS predecessor_name,
    g.fullname as fullname
FROM
    dm.dm_component d
    JOIN dm.dm_component f ON d.predecessorid = f.id
    JOIN dm.dm_domain g ON d.domainid = g.id
WHERE
    d.status = 'N'
    AND d.id NOT IN (SELECT compid FROM dm.dm_deploymentcomps)
UNION
SELECT DISTINCT
    d.id,
    d.name,
    d.created,
    e.name AS environment_name,
    a.deploymentid,
    a.finishts,
    a.exitcode,
    d.domainid,
    d.predecessorid,
    COALESCE(d.parentid, d.id) AS parentid,
    NULL AS predecessor_name,
    g.fullname as fullname
FROM
    dm.dm_deployment a
    JOIN dm.dm_application b ON a.appid = b.id
    JOIN dm.dm_deploymentcomps c ON a.deploymentid = c.deploymentid
    JOIN dm.dm_component d ON c.compid = d.id
    JOIN dm.dm_environment e ON e.id = a.envid
    JOIN dm.dm_domain g ON d.domainid = g.id
WHERE
    d.status = 'N'
    AND d.predecessorid IS NULL
    AND (a.envid, c.compid, a.deploymentid) IN (
        SELECT
            x.envid,
            b.compid,
            MAX(x.deploymentid) AS deploymentid
        FROM
            dm.dm_deployment x
            JOIN dm.dm_deploymentcomps b ON x.deploymentid = b.deploymentid
        GROUP BY
            x.envid,
            b.compid
    )
UNION
SELECT DISTINCT
    d.id,
    d.name,
    d.created,
    '' AS environment_name,
    -99 AS deploymentid,
    NOW()::timestamp AS finishts,
    0 AS exitcode,
    d.domainid,
    d.predecessorid,
    COALESCE(d.parentid, d.id) AS parentid,
    NULL AS predecessor_name,
    g.fullname as fullname
FROM
    dm.dm_component d
	JOIN dm.dm_domain g ON d.domainid = g.id
WHERE
    d.status = 'N'
    AND d.predecessorid IS NULL
    AND d.id NOT IN (SELECT compid FROM dm.dm_deploymentcomps);
