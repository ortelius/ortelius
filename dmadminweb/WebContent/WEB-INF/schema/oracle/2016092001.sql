-- Add bug tracking datasource to applications and components
alter table dm.dm_application add datasourceid integer;
alter table dm.dm_component add datasourceid integer;
-- Defect table
CREATE TABLE dm_defects (
    bugid varchar2(128),
    compid integer,
    appid integer,
    title varchar2(256),
    apiurl varchar2(1024),
    htmlurl varchar2(1024)
);
-- Add details for bugzilla, github and jira datasources
INSERT INTO dm.dm_providerdef(id,name,kind,kindstr) VALUES(18,'bugzilla',10,'datasource');
INSERT INTO dm.dm_providerdef(id,name,kind,kindstr) VALUES(19,'github',10,'datasource');
INSERT INTO dm.dm_providerdef(id,name,kind,kindstr) VALUES(20,'jira',10,'datasource');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(18,'Server URL','Y','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(18,'Product','N','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(18,'Component','N','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(18,'Status','N','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(19,'Repository','Y','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(19,'Product','Y','N');
INSERT INTO dm.dm_propertydef(defid,name,required,appendable) VALUES(20,'Server URL','Y','N');
-- add restful_get
INSERT INTO dm.dm_action(id,name,summary,domainid,function,pluginid,resultisexpr,status,kind,categoryid) VALUES(17,'restful_get','makes RESTful calls to remote servers',1,'Y',6,'Y','N',5,1);
