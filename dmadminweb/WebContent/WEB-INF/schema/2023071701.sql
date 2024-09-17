DROP VIEW dm.dm_app_lag;
DROP VIEW dm.dm_app_scorecard;

CREATE OR REPLACE VIEW dm.dm_app_scorecard
 AS
 SELECT c.domainid,
 	c.parentid as parentid,
    c.name AS application,
    d.name AS environment,
    date_trunc('month'::text, b.startts::date::timestamp with time zone) AS monthly
   FROM dm_application a,
    dm_deployment b,
    dm_application c,
    dm_environment d
  WHERE a.id = b.appid AND a.parentid = c.id AND b.envid = d.id
UNION ALL
 SELECT a.domainid,
  	a.id as parentid,
    a.name AS application,
    d.name AS environment,
    date_trunc('month'::text, b.startts::date::timestamp with time zone) AS monthly
   FROM dm_application a,
    dm_deployment b,
    dm_environment d
  WHERE a.id = b.appid AND a.parentid IS NULL AND b.envid = d.id;
