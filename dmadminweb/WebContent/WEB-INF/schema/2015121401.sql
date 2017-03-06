create table dm.dm_taskaction(id integer,actionid integer);
insert into dm.dm_propertydef(defid,name,required,appendable) values(3,'SSL','N','N');
insert into dm.dm_plugin(id,version,library) values(6,10000,'restful');
insert into dm.dm_action(id,name,summary,domainid,function,resultisexpr,pluginid,status,kind,categoryid)
values(16,'restful_post','makes RESTful calls to remote servers',1,'Y','Y',6,'N',5,1);
