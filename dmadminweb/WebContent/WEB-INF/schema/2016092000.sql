-- Add bug tracking datasource to applications and components
alter table dm.dm_application add datasourceid integer;
alter table dm.dm_component add datasourceid integer;

-- add restful_get
INSERT INTO dm.dm_action(id,name,summary,domainid,function,pluginid,resultisexpr,status,kind,categoryid) VALUES(17,'restful_get','makes RESTful calls to remote servers',1,'Y',6,'Y','N',5,1);
