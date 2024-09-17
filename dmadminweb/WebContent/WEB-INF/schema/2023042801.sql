drop view if exists dm_scorecard_ui;
drop view if exists dm_scorecard;
drop view if exists dm_scorecard_nv;
drop view if exists dm_app_scorecard;
drop view if exists dm_app_lag;

create view dm_scorecard_nv as
select id as id, 'license' as name, 'Y' as value from dm.dm_component a where status = 'N' and id in (select distinct compid from dm.dm_textfile where filetype = 'license' and compid = a.id)
union
select id as id, 'readme' as name, 'Y' as value from dm.dm_component a where status = 'N' and id in (select distinct compid from dm.dm_textfile where filetype = 'readme' and compid = a.id)
union
select id as id, 'swagger' as name, 'Y' as value from dm.dm_component a where status = 'N' and id in (select distinct compid from dm.dm_textfile where filetype = 'swagger' and compid = a.id)
union
select compid as id, c.name, c.value from dm.dm_componentvars c, dm.dm_component d
where c.compid = d.id and d.status = 'N' and
c.name in ('GitLinesAdded', 'GitLinesDeleted', 'GitLinesTotals', 'GitTotalCommittersCnt', 'GitCommittersCnt',
		   'JobTriggeredBy', 'SonarBugs', 'SonarCodeSmells', 'SonarViolations', 'SonarProjectStatus', 'VeracodeScore');

CREATE VIEW dm.dm_app_lag AS
SELECT c.domainid, c.name AS application, d.name AS environment, b.deploymentid, b.startts FROM dm_application a, dm_deployment b, dm_application c, dm_environment d WHERE a.id = b.appid AND a.parentid = c.id AND b.envid = d.id
UNION ALL
SELECT a.domainid, a.name AS application, d.name AS environment, b.deploymentid, b.startts FROM dm_application a, dm_deployment b, dm_environment d WHERE a.id = b.appid AND a.parentid IS NULL AND b.envid = d.id;

CREATE VIEW dm.dm_app_scorecard AS
SELECT c.domainid, c.name AS application, d.name AS environment, date_trunc('week'::text, b.startts::date::timestamp with time zone) AS weekly FROM dm_application a, dm_deployment b, dm_application c, dm_environment d WHERE a.id = b.appid AND a.parentid = c.id AND b.envid = d.id
UNION ALL
SELECT a.domainid, a.name AS application, d.name AS environment, date_trunc('week'::text, b.startts::date::timestamp with time zone) AS weekly FROM dm_application a, dm_deployment b, dm_environment d WHERE a.id = b.appid AND a.parentid IS NULL AND b.envid = d.id;
