-- Defect table
CREATE TABLE dm_defects (
    bugid character varying(128),
    compid integer,
    appid integer,
    title character varying(256),
    apiurl character varying(1024),
    htmlurl character varying(1024)
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
