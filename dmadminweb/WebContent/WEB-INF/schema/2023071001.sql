DROP VIEW dm.dm_app_lag;

CREATE OR REPLACE VIEW dm.dm_app_lag
 AS
 SELECT c.domainid,
    c.name AS application,
    d.name AS environment,
    b.deploymentid,
    b.startts
   FROM dm_application a,
    dm_deployment b,
    dm_application c,
    dm_environment d
  WHERE a.id = b.appid AND a.parentid = c.id AND b.envid = d.id
UNION ALL
 SELECT a.domainid,
    a.name AS application,
    d.name AS environment,
    b.deploymentid,
    b.startts
   FROM dm_application a,
    dm_deployment b,
    dm_environment d
  WHERE a.id = b.appid AND a.parentid IS NULL AND b.envid = d.id;

DROP VIEW dm.dm_app_scorecard;

CREATE OR REPLACE VIEW dm.dm_app_scorecard
 AS
 SELECT c.domainid,
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
    a.name AS application,
    d.name AS environment,
    date_trunc('month'::text, b.startts::date::timestamp with time zone) AS monthly
   FROM dm_application a,
    dm_deployment b,
    dm_environment d
  WHERE a.id = b.appid AND a.parentid IS NULL AND b.envid = d.id;

DROP VIEW dm.dm_scorecard_nv;

CREATE OR REPLACE VIEW dm.dm_scorecard_nv
 AS
 SELECT a.id,
    'license'::text AS name,
    'Y'::text AS value
   FROM dm_component a
  WHERE a.status = 'N'::bpchar AND (a.id IN ( SELECT DISTINCT dm_textfile.compid
           FROM dm_textfile
          WHERE dm_textfile.filetype::text = 'license'::text AND dm_textfile.compid = a.id))
UNION
 SELECT a.id,
    'readme'::text AS name,
    'Y'::text AS value
   FROM dm_component a
  WHERE a.status = 'N'::bpchar AND (a.id IN ( SELECT DISTINCT dm_textfile.compid
           FROM dm_textfile
          WHERE dm_textfile.filetype::text = 'readme'::text AND dm_textfile.compid = a.id))
UNION
 SELECT a.id,
    'swagger'::text AS name,
    'Y'::text AS value
   FROM dm_component a
  WHERE a.status = 'N'::bpchar AND (a.id IN ( SELECT DISTINCT dm_textfile.compid
           FROM dm_textfile
          WHERE dm_textfile.filetype::text = 'swagger'::text AND dm_textfile.compid = a.id))
UNION
 SELECT c.compid AS id,
    c.name,
    c.value
   FROM dm_componentvars c,
    dm_component d
  WHERE c.compid = d.id AND d.status = 'N'::bpchar AND (c.name::text = ANY (ARRAY['GitLinesAdded'::character varying::text, 'GitLinesDeleted'::character varying::text, 'GitLinesTotals'::character varying::text, 'GitTotalCommittersCnt'::character varying::text, 'GitCommittersCnt'::character varying::text, 'JobTriggeredBy'::character varying::text, 'SonarBugs'::character varying::text, 'SonarCodeSmells'::character varying::text, 'SonarViolations'::character varying::text, 'SonarProjectStatus'::character varying::text, 'VeracodeScore'::character varying::text]));
