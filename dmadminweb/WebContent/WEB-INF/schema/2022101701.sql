drop view if exists dm_scorecard_ui;
drop view if exists dm_scorecard;
drop view if exists dm_scorecard_nv;

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
		   'JobTriggerdBy', 'SonarBugs', 'SonarCodeSmells', 'SonarViolations', 'SonarProjectStatus', 'VericodeScore');

create view dm_scorecard as 
select id,
max(case when (b.name='license') then CASE WHEN b.value='Y' THEN b.value ELSE 'N' END ELSE 'N' END) as license,
max(case when (b.name='readme') then CASE WHEN b.value='Y' THEN b.value ELSE 'N' END ELSE 'N' END) as readme,
max(case when (b.name='swagger') then CASE WHEN b.value='Y' THEN b.value ELSE 'N' END ELSE 'N' END) as swagger,
max(case when (b.name='GitLinesAdded') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE 0 END ELSE NULL END) as git_lines_added,
max(case when (b.name='GitLinesDeleted') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE 0 END ELSE NULL END) as git_lines_deleted,
max(case when (b.name='GitLinesTotals') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE 0 END ELSE NULL END) as git_lines_total,
max(case when (b.name='GitTotalCommittersCnt') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE 0 END ELSE NULL END) as git_total_committers_cnt,
max(case when (b.name='GitCommittersCnt') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE 0 END ELSE NULL END) as git_committers_cnt,
max(case when (b.name='JobTriggerdBy') then CASE WHEN b.value~E'Started by user' THEN b.value ELSE b.value END ELSE '' END) as git_trigger,
max(case when (b.name='SonarBugs') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE -1 END ELSE -1 END) as sonar_bugs,
max(case when (b.name='SonarCodeSmells') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE -1 END ELSE -1 END) as sonar_code_smells,
max(case when (b.name='SonarViolations') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE -1 END ELSE -1 END) as sonar_violations,
max(case when (b.name='SonarProjectStatus') then CASE WHEN (b.value='OK' OR b.value='WARN' OR b.value='ERROR' OR b.value='NONE')  THEN b.value ELSE '' END ELSE '' END) as sonar_project_status,
max(case when (b.name='VericodeScore') then CASE WHEN b.value~E'^\\d+$' THEN b.value::integer ELSE -1 END ELSE -1 END) as vericode_score
from dm_scorecard_nv b
group by id;

create view dm_scorecard_ui as
SELECT distinct c.domainid as "domain", c.name as "application", b.name as "component", license, readme, swagger, 
COALESCE(round((git_lines_added + git_lines_deleted)/NULLIF(git_lines_total, 0)*100.00,0),100)::text  AS lines_changed, 
COALESCE(round(git_committers_cnt/NULLIF(git_total_committers_cnt, 0)::float*100.00),0)::text  AS contributing_committers,
git_total_committers_cnt AS total_committers, git_trigger, sonar_bugs, sonar_code_smells, sonar_violations, sonar_project_status, vericode_score
FROM dm.dm_scorecard a, dm.dm_component b, dm.dm_application c, dm.dm_applicationcomponent d 
WHERE a.id = b.id and c.id = d.appid and d.compid = a.id and c.status = 'N';