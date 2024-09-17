CREATE OR REPLACE VIEW dm_applist AS
SELECT
    b.id,
    b.name AS name,
    b.created,
    COALESCE(b.parentid, b.id) as parentid,
    c.name AS environment_name,
    a.deploymentid,
    a.finishts,
    a.exitcode,
    b.domainid,
    b.predecessorid,
    f.name AS predecessor_name,
    g.fullname AS fullname
FROM
    dm.dm_deployment a
    JOIN dm.dm_application b ON a.appid = b.id
    JOIN dm.dm_environment c ON a.envid = c.id
    JOIN dm.dm_application f ON b.predecessorid = f.id
    JOIN dm.dm_domain g ON b.domainid = g.id
WHERE
    b.status = 'N'
    AND b.isrelease = 'N'
    AND (a.appid, a.envid, a.deploymentid) IN
    (SELECT appid, envid, MAX(deploymentid)
     FROM dm.dm_deployment
     GROUP BY appid, envid)
UNION
SELECT
    b.id,
    b.name,
    b.created,
    COALESCE(b.parentid, b.id) as parentid,
    '',
    -99,
    NOW()::timestamp,
    -1,
    b.domainid,
    b.predecessorid,
    f.name,
    g.fullname AS fullname
FROM
    dm.dm_application b
    JOIN dm.dm_application f ON b.predecessorid = f.id
    JOIN dm.dm_domain g ON b.domainid = g.id
WHERE
    b.status = 'N'
    AND b.isrelease = 'N'
    AND b.id NOT IN
    (SELECT appid FROM dm.dm_deployment WHERE finishts IS NOT NULL)
UNION
SELECT
    b.id,
    b.name,
    b.created,
    COALESCE(b.parentid, b.id) as parentid,
    c.name,
    a.deploymentid,
    a.finishts,
    a.exitcode,
    b.domainid,
    b.predecessorid,
    NULL,
    g.fullname AS fullname
FROM
    dm.dm_deployment a
    JOIN dm.dm_application b ON a.appid = b.id
    JOIN dm.dm_environment c ON a.envid = c.id
    JOIN dm.dm_domain g ON b.domainid = g.id
WHERE
    b.status = 'N'
    AND b.isrelease = 'N'
    AND b.predecessorid IS NULL
    AND (a.appid, a.envid, a.deploymentid) IN
    (SELECT appid, envid, MAX(deploymentid)
     FROM dm.dm_deployment
     GROUP BY appid, envid)
UNION
SELECT
    b.id,
    b.name,
    b.created,
    COALESCE(b.parentid, b.id) as parentid,
    '',
    -99,
    NOW()::timestamp,
    -1,
    b.domainid,
    b.predecessorid,
    NULL,
    g.fullname AS fullname
FROM
    dm.dm_application b
    JOIN dm.dm_domain g ON b.domainid = g.id
WHERE
    b.status = 'N'
    AND b.isrelease = 'N'
    AND b.predecessorid IS NULL
    AND b.id NOT IN
    (SELECT appid FROM dm.dm_deployment WHERE finishts IS NOT NULL)
ORDER BY 2, 4;
